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

	m_pCharacter->Spawn(this, SpawnPos);
	GameServer()->CreatePlayerSpawn(SpawnPos);
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
}
