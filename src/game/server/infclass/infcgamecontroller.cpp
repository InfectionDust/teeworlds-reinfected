
#include "infcgamecontroller.h"

#include "entities/infccharacter.h"
#include "mapconverter.h"
#include "infcplayer.h"
#include "infcserver.h"

#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>


#include <engine/shared/network.h>
#include <game/mapitems.h>


#include <engine/server.h>
#include <engine/shared/network.h>
#include <game/mapitems.h>
#include <time.h>
#include <iostream>
#include <map>

CGameControllerInfClass::CGameControllerInfClass(CGameContext *pGameServer) : IGameController(pGameServer)
{
	m_pGameType = "InfClassR7";
}

bool CGameControllerInfClass::PreSpawn(const CInfClassPlayer *pPlayer, vec2 *pPos)
{
	return CanSpawn(pPlayer->GetTeam(), pPos);
}

void CGameControllerInfClass::Tick()
{
	IGameController::Tick();
}

bool CGameControllerInfClass::DoWincheckMatch()
{
	// Humans win if there are humans.
	return true;
}

void CGameControllerInfClass::Snap(int SnappingClient)
{
	CNetObj_GameData *pGameData = static_cast<CNetObj_GameData *>(Server()->SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData)));
	if(!pGameData)
		return;

	pGameData->m_GameStartTick = m_GameStartTick;
	pGameData->m_GameStateFlags = 0;
	pGameData->m_GameStateEndTick = 0; // no timer/infinite = 0, on end = GameEndTick, otherwise = GameStateEndTick
	switch(m_GameState)
	{
	case IGS_WARMUP_GAME:
	case IGS_WARMUP_USER:
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_WARMUP;
		if(m_GameStateTimer != TIMER_INFINITE)
			pGameData->m_GameStateEndTick = Server()->Tick()+m_GameStateTimer;
		break;
	case IGS_START_COUNTDOWN:
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_STARTCOUNTDOWN|GAMESTATEFLAG_PAUSED;
		if(m_GameStateTimer != TIMER_INFINITE)
			pGameData->m_GameStateEndTick = Server()->Tick()+m_GameStateTimer;
		break;
	case IGS_GAME_PAUSED:
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_PAUSED;
		if(m_GameStateTimer != TIMER_INFINITE)
			pGameData->m_GameStateEndTick = Server()->Tick()+m_GameStateTimer;
		break;
	case IGS_END_ROUND:
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_ROUNDOVER;
		pGameData->m_GameStateEndTick = Server()->Tick()-m_GameStartTick-TIMER_END/2*Server()->TickSpeed()+m_GameStateTimer;
		break;
	case IGS_END_MATCH:
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_GAMEOVER;
		pGameData->m_GameStateEndTick = Server()->Tick()-m_GameStartTick-TIMER_END*Server()->TickSpeed()+m_GameStateTimer;
		break;
	case IGS_GAME_RUNNING:
		// not effected
		break;
	}
	if(m_SuddenDeath)
		pGameData->m_GameStateFlags |= GAMESTATEFLAG_SUDDENDEATH;

	if(IsTeamplay())
	{
		CNetObj_GameDataTeam *pGameDataTeam = static_cast<CNetObj_GameDataTeam *>(Server()->SnapNewItem(NETOBJTYPE_GAMEDATATEAM, 0, sizeof(CNetObj_GameDataTeam)));
		if(!pGameDataTeam)
			return;

		pGameDataTeam->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
		pGameDataTeam->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];
	}

	// demo recording
	if(SnappingClient == -1)
	{
		CNetObj_De_GameInfo *pGameInfo = static_cast<CNetObj_De_GameInfo *>(Server()->SnapNewItem(NETOBJTYPE_DE_GAMEINFO, 0, sizeof(CNetObj_De_GameInfo)));
		if(!pGameInfo)
			return;

		pGameInfo->m_GameFlags = m_GameFlags;
		pGameInfo->m_ScoreLimit = m_GameInfo.m_ScoreLimit;
		pGameInfo->m_TimeLimit = m_GameInfo.m_TimeLimit;
		pGameInfo->m_MatchNum = m_GameInfo.m_MatchNum;
		pGameInfo->m_MatchCurrent = m_GameInfo.m_MatchCurrent;
	}
	
//	CNetObj_GameData *pGameData = static_cast<CNetObj_GameData *>(Server()->SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData)));
//	if(!pGameData)
//		return;
	
	CInfClassServer *infcServer = static_cast<CInfClassServer*>(Server());
	int timeShiftUnit = infcServer->GetTimeShiftUnit();
	
	return;
//	pGameData->m_GameStartTick = m_GameStartTick;
//	pGameData->m_GameStateFlags = 0;
//	pGameData->m_GameStateEndTick = 0; // no timer/infinite = 0, on end = GameEndTick, otherwise = GameStateEndTick
	
//	CNetMsg_Sv_GameInfo *pSvGameInfo = static_cast<CNetMsg_Sv_GameInfo *>(Server()->SnapNewItem(NETMSGTYPE_SV_GAMEINFO, 0, sizeof(CNetMsg_Sv_GameInfo)));
//	if(!pSvGameInfo)
//		return;
	
//	int m_GameFlags;
//	int m_ScoreLimit;
//	int m_TimeLimit;
//	int m_MatchNum;
//	int m_MatchCurrent;
	
//	int ClassMask = 0;
//	if(GameServer()->m_apPlayers[SnappingClient])
//	{
//		int Page = -1;
		
////		if(GameServer()->m_apPlayers[SnappingClient]->MapMenu() == 1)
////		{
//			int Item = CMapConverter::MENUCLASS_RANDOM; // GameServer()->m_apPlayers[SnappingClient]->m_MapMenuItem;
//			Page = CMapConverter::TIMESHIFT_MENUCLASS + 3*((Item+1) + ClassMask*CMapConverter::TIMESHIFT_MENUCLASS_MASK) + 1;
////		}
		
//		if(Page >= 0)
//		{
//			double PageShift = static_cast<double>(Page * timeShiftUnit)/1000.0f;
//			double CycleShift = fmod(static_cast<double>(Server()->Tick() - pGameData->m_GameStartTick)/Server()->TickSpeed(), timeShiftUnit/1000.0);
//			int TimeShift = (PageShift + CycleShift)*Server()->TickSpeed();
			
//			pGameData->m_GameStartTick = Server()->Tick() - TimeShift;
//			pSvGameInfo->m_TimeLimit += (TimeShift/Server()->TickSpeed())/60;
//		}
//	}
}

void CGameControllerInfClass::OnCharacterSpawn(CCharacter *pChr)
{
	// We work only with InfClass characters
	CInfClassCharacter *pCharacter = static_cast<CInfClassCharacter *>(pChr);
	// Route the event method to the class itself
	pCharacter->OnCharacterSpawn();
}
