#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>

#include "entities/infccharacter.h"

#include "infcgamecontroller.h"
#include "infcplayer.h"

CGameControllerInfClass::CGameControllerInfClass(CGameContext *pGameServer) : IGameController(pGameServer)
{
	m_pGameType = "InfClassR7";
}

bool CGameControllerInfClass::PreSpawn(const CInfClassPlayer *pPlayer, vec2 *pOutPos)
{
	SPAWN_POINT_TYPE SpawnType = (pPlayer->IsZombie() ? POINT_TYPE_INFECTED : POINT_TYPE_HUMAN);

	const array<vec2> &SpawnPoints = m_aSpawnPoints[SpawnType];

	if(SpawnPoints.size() == 0)
		return false;

	// get spawn point
	int RandomShift = random_int() % SpawnPoints.size();
	for(int i = 0; i < SpawnPoints.size(); i++)
	{
		int I = (i + RandomShift)%SpawnPoints.size();
		*pOutPos = SpawnPoints[I];
		return true;
	}

	return false;
}

bool CGameControllerInfClass::OnEntity(int, vec2)
{
	return false;
}

bool CGameControllerInfClass::OnModEntity(const char *pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv)
{
	vec2 Pos = (P0 + P1 + P2 + P3)/4.0f;

	if(str_comp(pName, "icInfected") == 0)
		m_aSpawnPoints[POINT_TYPE_INFECTED].add(Pos);
	else if(str_comp(pName, "icHuman") == 0)
		m_aSpawnPoints[POINT_TYPE_HUMAN].add(Pos);

	return false;
}

void CGameControllerInfClass::OnCharacterSpawn(CCharacter *pChr)
{
	// We work only with InfClass characters
	CInfClassCharacter *pCharacter = static_cast<CInfClassCharacter *>(pChr);
	// Route the event method to the class itself
	pCharacter->OnCharacterSpawn();
}
