#include "infcgamecontext.h"
#include "infcgamecontroller.h"
#include "infcplayer.h"

#include "classes/infected/hunter.h"
#include "classes/infected/slug.h"
#include "classes/humans/hero.h"

#include "entities/slug_slime.h"

#include <engine/shared/config.h>
#include <base/system.h>

template<typename C>
static CInfClassPlayerClass *constructor()
{
	return new C;
}

template<typename C>
int CInfClassGameContext::RegisterInfClassClass()
{
	InfPlayerClassConstructor *con = &constructor<C>;
	int ClassId = m_ClassConstructors.size();
	m_ClassConstructors.push_back(con);
	m_ClassIdToName.emplace(ClassId, C::ClassName());

	return ClassId;
}

template<typename C>
void RegisterInfClassEntity()
{
	C::EntityId = CGameWorld::NUM_ENTTYPES;
	++CGameWorld::NUM_ENTTYPES;
}

CInfClassGameContext::CInfClassGameContext()
{
}

void CInfClassGameContext::RegisterTypes()
{
	RegisterInfClassClass<CInfClassSlug>();
	RegisterInfClassClass<CInfClassHunter>();
	RegisterInfClassClass<CInfClassHero>();

	RegisterInfClassEntity<CSlugSlime>();
}

CGameWorld *CInfClassGameContext::GameWorld()
{
	return &m_World;
}

void CInfClassGameContext::OnInit()
{
	RegisterTypes();

	// init everything
	m_pServer = Kernel()->RequestInterface<IServer>();
	m_pConfig = Kernel()->RequestInterface<IConfigManager>()->Values();
	m_pConsole = Kernel()->RequestInterface<IConsole>();
	m_World.SetGameServer(this);
	m_World.ApplyEntitiesConfiguration();
	m_Events.SetGameServer(this);
	m_CommandManager.Init(m_pConsole, this, NewCommandHook, RemoveCommandHook);

	// HACK: only set static size for items, which were available in the first 0.7 release
	// so new items don't break the snapshot delta
	static const int OLD_NUM_NETOBJTYPES = 23;
	for(int i = 0; i < OLD_NUM_NETOBJTYPES; i++)
		Server()->SnapSetStaticsize(i, m_NetObjHandler.GetObjSize(i));

	m_Layers.Init(Kernel());
	m_Collision.Init(&m_Layers);

	// select gametype
	m_pInfcGameController = new CGameControllerInfClass(this);
	m_pController = m_pInfcGameController;

	// create all entities from the game layer
	CMapItemLayerTilemap *pTileMap = m_Layers.GameLayer();
	CTile *pTiles = (CTile *)Kernel()->RequestInterface<IMap>()->GetData(pTileMap->m_Data);
	for(int y = 0; y < pTileMap->m_Height; y++)
	{
		for(int x = 0; x < pTileMap->m_Width; x++)
		{
			int Index = pTiles[y*pTileMap->m_Width+x].m_Index;

			if(Index >= ENTITY_OFFSET)
			{
				vec2 Pos(x*32.0f+16.0f, y*32.0f+16.0f);
				m_pController->OnEntity(Index-ENTITY_OFFSET, Pos);
			}
		}
	}

	Console()->Chain("sv_motd", ConchainSpecialMotdupdate, this);

	Console()->Chain("sv_vote_kick", ConchainSettingUpdate, this);
	Console()->Chain("sv_vote_kick_min", ConchainSettingUpdate, this);
	Console()->Chain("sv_vote_spectate", ConchainSettingUpdate, this);
	Console()->Chain("sv_teambalance_time", ConchainSettingUpdate, this);
	Console()->Chain("sv_player_slots", ConchainSettingUpdate, this);

	Console()->Chain("sv_scorelimit", ConchainGameinfoUpdate, this);
	Console()->Chain("sv_timelimit", ConchainGameinfoUpdate, this);
	Console()->Chain("sv_matches_per_map", ConchainGameinfoUpdate, this);

	// clamp sv_player_slots to 0..MaxClients
	if(Config()->m_SvMaxClients < Config()->m_SvPlayerSlots)
		Config()->m_SvPlayerSlots = Config()->m_SvMaxClients;
}

void CInfClassGameContext::OnConsoleInit()
{
	CGameContext::OnConsoleInit();

	Console()->Register("inf_set_class", "is", CFGFLAG_SERVER, ConSetClass, this, "Set the class of a player");
}

void CInfClassGameContext::OnMessage(int MsgID, CUnpacker *pUnpacker, int ClientID)
{
	switch(MsgID)
	{
	case NETMSGTYPE_CL_SKINCHANGE:
		return;
	default:
		break;
	}

	CGameContext::OnMessage(MsgID, pUnpacker, ClientID);
}

void CInfClassGameContext::OnClientConnected(int ClientID, bool AsSpec)
{
	if(m_apPlayers[ClientID])
	{
		dbg_assert(m_apPlayers[ClientID] != nullptr, "invalid clientID");
		return;
	}

	m_apPlayers[ClientID] = new (ClientID) CInfClassPlayer(this, ClientID, AsSpec);

	// send active vote
	if(m_VoteCloseTime)
		SendVoteSet(m_VoteType, ClientID);

	// send motd
	SendMotd(ClientID);

	// send settings
	SendSettings(ClientID);
}

void CInfClassGameContext::AnnounceSkinChange(int ClientID)
{
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(!m_apPlayers[i] || (!Server()->ClientIngame(i) && !m_apPlayers[i]->IsDummy()) || Server()->GetClientVersion(i) < MIN_SKINCHANGE_CLIENTVERSION)
			continue;

		SendSkinChange(ClientID, i);
	}
}

CInfClassPlayer *CInfClassGameContext::GetPlayer(int ClientID)
{
	return static_cast<CInfClassPlayer*>(m_apPlayers[ClientID]);
}

CInfClassPlayerClass *CInfClassGameContext::CreateInfClass(int ClassId)
{
	InfPlayerClassConstructor *constructor = m_ClassConstructors.at(ClassId);
	CInfClassPlayerClass *infClass = constructor();
	return infClass;
}

bool CInfClassGameContext::AreConnected(vec2 Pos1, vec2 Pos2, float Radius)
{
	if(distance(Pos1, Pos2) > Radius)
		return false;

	int TileRadius = std::ceil(Radius/32.0f);
	int CenterX = TileRadius;
	int CenterY = TileRadius;
	int Width = 2*TileRadius+1;
	int Height = 2*TileRadius+1;
	char* pMap = new char[Width*Height];
	for(int j=0; j<Height; j++)
	{
		for(int i=0; i<Width; i++)
		{
			if(Collision()->CheckPoint(Pos1.x + 32.0f*(i-CenterX), Pos1.y + 32.0f*(j-CenterY)))
				pMap[j*Width+i] = 0x0; //This tile can be checked
			else
				pMap[j*Width+i] = 0x1;
		}
	}

	pMap[CenterY*Width+CenterX] = 0x2; //This tile is checked

	int Pos2X = clamp(CenterX + (int)round((Pos2.x - Pos1.x)/32.0f), 0, Width-1);
	int Pos2Y = clamp(CenterY + (int)round((Pos2.y - Pos1.y)/32.0f), 0, Height-1);

	bool Changes = true;
	while(Changes)
	{
		Changes = false;
		for(int j=0; j<Height; j++)
		{
			for(int i=0; i<Width; i++)
			{
				if(pMap[j*Width+i]&0x1 && !(pMap[j*Width+i]&0x2))
				{
					if(i>0 && (pMap[j*Width+(i-1)]&0x2))
					{
						pMap[j*Width+i] = 0x2;
						Changes = true;
					}
					if(j>0 && (pMap[(j-1)*Width+i]&0x2))
					{
						pMap[j*Width+i] = 0x2;
						Changes = true;
					}
					if(i<Width-1 && (pMap[j*Width+(i+1)]&0x2))
					{
						pMap[j*Width+i] = 0x2;
						Changes = true;
					}
					if(j<Height-1 && (pMap[(j+1)*Width+i]&0x2))
					{
						pMap[j*Width+i] = 0x2;
						Changes = true;
					}
				}
			}
		}

		if(pMap[Pos2Y*Width+Pos2X]&0x2)
		{
			delete[] pMap;
			return true;
		}
	}	

	delete[] pMap;
	return false;
}

void CInfClassGameContext::ConSetClass(IConsole::IResult *pResult, void *pUserData)
{
	CInfClassGameContext *pSelf = static_cast<CInfClassGameContext *>(pUserData);
	int PlayerID = pResult->GetInteger(0);
	const char *pClassName = pResult->GetString(1);

	CInfClassPlayer* pPlayer = pSelf->GetPlayer(PlayerID);
	
	if(!pPlayer)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "inf_set_class", "No such player");
		return;
	}

	for(const auto &idToName : pSelf->m_ClassIdToName)
	{
		if(str_comp(pClassName, idToName.second) != 0)
			continue;

		int ClassId = idToName.first;
		pPlayer->SetCharacterClass(ClassId);
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "The admin changed the class of %s to %s", pSelf->Server()->ClientName(PlayerID), pClassName);
		pSelf->SendChat(-1, CHAT_ALL, -1, aBuf);

		return;
	}

	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "inf_set_class", "Unknown class");
	return;
}

IGameServer *CreateModGameServer() { return new CInfClassGameContext; }
