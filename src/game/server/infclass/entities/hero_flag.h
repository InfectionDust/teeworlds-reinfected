/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_HEROFLAG_H
#define GAME_SERVER_ENTITIES_HEROFLAG_H

#include <game/server/infclass/entities/infcentity.h>

class CInfClassCharacter;

class CHeroFlag : public CInfCEntity
{
public:
	static int EntityId;

	enum
	{
		RADIUS = 50,
		SHIELD_COUNT = 4,
		SPEED = 15, // higher = slower
	};

public:
	static const int ms_PhysSize = 14;

	CHeroFlag(CInfClassGameContext *pGameContext, int Owner);
	~CHeroFlag();

	inline int GetCoolDown() { return m_CoolDownTick; }

	void Tick() override;
	void FindPosition();
	void Snap(int SnappingClient) override;
	void GiveGift(CCharacter *pHero);

protected:
	int m_CoolDownTick = 0;
	int m_IDs[SHIELD_COUNT];
};

#endif
