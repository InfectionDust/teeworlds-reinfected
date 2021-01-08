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

bool CGameControllerInfClass::PreSpawn(const CInfClassPlayer *pPlayer, vec2 *pPos)
{
	return CanSpawn(pPlayer->GetTeam(), pPos);
}

bool CGameControllerInfClass::OnEntity(int, vec2)
{
	return false;
}

bool CGameControllerInfClass::OnModEntity(const char *pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv)
{
	return false;
}

void CGameControllerInfClass::OnCharacterSpawn(CCharacter *pChr)
{
	// We work only with InfClass characters
	CInfClassCharacter *pCharacter = static_cast<CInfClassCharacter *>(pChr);
	// Route the event method to the class itself
	pCharacter->OnCharacterSpawn();
}
