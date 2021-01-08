#ifndef GAME_SERVER_INFCLASS_CONTROLLER_H
#define GAME_SERVER_INFCLASS_CONTROLLER_H

#include <game/server/gamecontroller.h>

class CCharacter;
class CGameContext;
class CInfCEntity;
class CInfClassGameContext;
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
	CGameControllerInfClass(CInfClassGameContext *pContext);

	CInfClassServer *ModServer() const;
	CInfClassGameContext *GameContext() const { return m_pGameContext; }

	bool PreSpawn(const CInfClassPlayer *pPlayer, vec2 *pOutPos);

	bool OnEntity(int Index, vec2 Pos) override;
	bool OnModEntity(const char* pName, vec2 Pivot, vec2 P0, vec2 P1, vec2 P2, vec2 P3, int PosEnv);

	void Tick() override;

	void OnCharacterSpawn(CCharacter *pChr) override;
	void FindHeroFlagPosition(CInfCEntity *pFlag);
	int GetHeroGiftCoolDown() const;
	int GetGlobalHeroGiftCoolDown() const;
	void OnFlagCollected(CInfCEntity *pFlag);

protected:
	CInfClassGameContext *m_pGameContext = nullptr;
	array<vec2> m_aSpawnPoints[POINT_TYPES_COUNT];
	array<vec2> m_HeroFlagPositions;
	int m_GlobalHeroGiftCoolDown = 0;
};

extern IGameController *CreateInfCGameServer();

#endif // GAME_SERVER_INFCLASS_CONTROLLER_H
