/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_MERCENARY_BOMB_H
#define GAME_SERVER_ENTITIES_MERCENARY_BOMB_H

#include <game/server/infclass/entities/infcentity.h>

class CMercenaryBomb : public CInfCEntity
{
public:
	static int EntityId;

	enum
	{
		NUM_SIDE = 12,
		NUM_HINT = 12,
		NUM_IDS = NUM_SIDE + NUM_HINT,
	};

public:
	CMercenaryBomb(CInfClassGameContext *pGameContext, vec2 Pos, int Owner);
	~CMercenaryBomb();

	virtual void Snap(int SnappingClient);
	virtual void Tick();
	virtual void Reset();
	void Explode();
	void IncreaseDamage(int weapon);
	bool ReadyToExplode();

private:
	int m_IDs[NUM_IDS];

	int m_LoadingTick;
	int m_Owner;
	float m_Damage;
};

#endif
