#include "infcentity.h"

#include <game/server/infclass/infcgamecontext.h>
#include <game/server/infclass/infcserver.h>

CInfCEntity::CInfCEntity(CInfClassGameContext *pGameContext, int ObjectType, vec2 Pos, int Owner,
                         int ProximityRadius)
	: CEntity(pGameContext->GameWorld(), ObjectType, Pos, ProximityRadius)
	, m_pContext(pGameContext)
	, m_Owner(Owner)
{
}

CInfClassServer *CInfCEntity::ModServer() const
{
	return static_cast<CInfClassServer *>(m_pContext->Server());
}

void CInfCEntity::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CInfCEntity::OnExplosion(const CInfCExplosionContext &)
{
}
