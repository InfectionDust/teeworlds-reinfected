#include "infcplayer.h"

#include <game/server/infclass/entities/infccharacter.h>

#include "classes/infcplayerclass.h"

#include "infcgamecontext.h"
#include "infcgamecontroller.h"

static const bool NotADummy = false;

MACRO_ALLOC_POOL_ID_IMPL(CInfClassPlayer, MAX_CLIENTS)

CInfClassPlayer::CInfClassPlayer(CInfClassGameContext *pGameServer, int ClientID, bool AsSpec)
	: CPlayer(pGameServer, ClientID, NotADummy, AsSpec)
	, m_pInfcGameContext(pGameServer)
{
}

void CInfClassPlayer::TryRespawn()
{
	vec2 SpawnPos;

	if(!m_pInfcGameContext->GetController()->PreSpawn(this, &SpawnPos))
		return;

	m_Spawning = false;
	m_pInfcCharacter = new CInfClassCharacter(&GameServer()->m_World);
	m_pCharacter = m_pInfcCharacter;

	SetCharacterClass(0);

	m_pCharacter->Spawn(this, SpawnPos);
	GameServer()->CreatePlayerSpawn(SpawnPos);
}

void CInfClassPlayer::SyncSkin()
{
	m_pInfcPlayerClass->SetupSkin();
	const SkinInfo &skin = m_pInfcPlayerClass->GetSkinInfo();

	for(int p = 0; p < NUM_SKINPARTS; p++)
	{
		str_copy(m_TeeInfos.m_aaSkinPartNames[p], skin.m_aaSkinPartNames[p], 24);
		m_TeeInfos.m_aUseCustomColors[p] = skin.m_aUseCustomColors[p];
		m_TeeInfos.m_aSkinPartColors[p] = skin.m_aSkinPartColors[p];
	}
	m_pInfcGameContext->AnnounceSkinChange(GetCID());
}

void CInfClassPlayer::SetCharacterClass(int ClassId)
{
	CInfClassPlayerClass *pClass = m_pInfcGameContext->CreateInfClass(ClassId);
	SetCharacterClass(pClass);
}

void CInfClassPlayer::SetCharacterClass(CInfClassPlayerClass *pClass)
{
	if(m_pInfcPlayerClass)
		delete m_pInfcPlayerClass;

	m_pInfcPlayerClass = pClass;
	SyncSkin();
}
