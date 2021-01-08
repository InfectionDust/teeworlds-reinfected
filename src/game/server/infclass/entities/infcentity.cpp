#include "infcentity.h"

#include <game/server/infclass/infcgamecontext.h>

CInfCEntity::CInfCEntity(CInfClassGameContext *pGameContext, int ObjectType, vec2 Pos, int Owner,
                         int ProximityRadius)
	: CEntity(pGameContext->GameWorld(), ObjectType, Pos, ProximityRadius)
	, m_pContext(pGameContext)
	, m_Owner(Owner)
{
}

void CInfCEntity::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CInfCEntity::OnExplosion(const CInfCExplosionContext &)
{
}
