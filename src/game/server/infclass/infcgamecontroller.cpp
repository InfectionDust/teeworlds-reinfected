#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "infcgamecontroller.h"

CGameControllerInfClass::CGameControllerInfClass(CGameContext *pGameServer) : IGameController(pGameServer)
{
	m_pGameType = "InfClassR7";
}
