/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_SLUG_SLIME_H
#define GAME_SERVER_ENTITIES_SLUG_SLIME_H

#include <game/server/infclass/entities/infcentity.h>

class CInfClassGameContext;

class CSlugSlime : public CInfCEntity
{
public:
	static int EntityId;
public:
	CSlugSlime(CInfClassGameContext *pGameContext, vec2 Pos, int Owner);

	void Tick() override;

protected:
	int m_LifeSpan;
};

#endif
