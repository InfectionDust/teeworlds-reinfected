#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>

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
