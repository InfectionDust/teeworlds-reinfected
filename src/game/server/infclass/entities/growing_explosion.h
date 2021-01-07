/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include <game/server/infclass/entities/infcentity.h>
#include <game/server/infclass/infc_explosion.h>

#ifndef GAME_SERVER_ENTITIES_GROWINGEXP_H
#define GAME_SERVER_ENTITIES_GROWINGEXP_H

class CGrowingExplosion : public CInfCEntity
{
public:
	static int EntityId;

	CGrowingExplosion(CInfClassGameContext *pGameContext, vec2 Pos, int Owner,
	                  vec2 Dir, int Radius, ExplosionEffect Effect);
	~CGrowingExplosion();
	
	void Reset() override;
	void Tick() override;
	void TickPaused() override;

private:
	template<class E>
	bool IsInRadius(const E *pEntity);

	int m_MaxGrowing;
	int m_GrowingMap_Length;
	int m_GrowingMap_Size;
	
	vec2 m_SeedPos;
	int m_SeedX;
	int m_SeedY;
	int m_StartTick;
	int* m_pGrowingMap;
	vec2* m_pGrowingMapVec;
	ExplosionEffect m_ExplosionEffect;
	bool m_Hit[MAX_CLIENTS];
};

#endif
