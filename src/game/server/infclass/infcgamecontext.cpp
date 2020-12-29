#include "infcgamecontext.h"
#include "infcgamecontroller.h"
#include "infcplayer.h"

#include "classes/infected/hunter.h"
#include "classes/humans/hero.h"

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

CInfClassGameContext::CInfClassGameContext()
{
}

void CInfClassGameContext::RegisterTypes()
{
	RegisterInfClassClass<CInfClassHunter>();
	RegisterInfClassClass<CInfClassHero>();
}

void CInfClassGameContext::OnInit()
{
	RegisterTypes();

	// init everything
	m_pServer = Kernel()->RequestInterface<IServer>();
	m_pConfig = Kernel()->RequestInterface<IConfigManager>()->Values();
	m_pConsole = Kernel()->RequestInterface<IConsole>();
	m_World.SetGameServer(this);
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
