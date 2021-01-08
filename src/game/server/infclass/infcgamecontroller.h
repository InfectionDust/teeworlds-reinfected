#ifndef GAME_SERVER_INFCLASS_CONTROLLER_H
#define GAME_SERVER_INFCLASS_CONTROLLER_H

#include <game/server/gamecontroller.h>

class CCharacter;
class CGameContext;
class CInfClassPlayer;
class CInfClassServer;

enum SPAWN_POINT_TYPE
{
	POINT_TYPE_INFECTED,
	POINT_TYPE_HUMAN,
	POINT_TYPES_COUNT,
};

class CGameControllerInfClass : public IGameController
{
public:
	CGameControllerInfClass(CGameContext *pGameServer);

	CInfClassServer *ModServer() const;

	bool PreSpawn(const CInfClassPlayer *pPlayer, vec2 *pOutPos);

	bool OnEntity(int Index, vec2 Pos) override;
	bool OnModEntity(const char* pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv);

	void OnCharacterSpawn(CCharacter *pChr) override;

protected:
	array<vec2> m_aSpawnPoints[POINT_TYPES_COUNT];
};

extern IGameController *CreateInfCGameServer();

#endif // GAME_SERVER_INFCLASS_CONTROLLER_H
