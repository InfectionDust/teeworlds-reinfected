/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "slug_slime.h"

#include <engine/shared/config.h>
#include <game/server/infclass/classes/infcplayerclass.h>
#include <game/server/infclass/entities/growing_explosion.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/infcgamecontext.h>

int CSlugSlime::EntityId = 0;

CSlugSlime::CSlugSlime(CInfClassGameContext *pGameContext, vec2 Pos, int Owner)
	: CInfCEntity(pGameContext, EntityId, Pos, Owner)
{
	m_LifeSpan = Server()->TickSpeed()*Config()->m_InfSlimeDuration;
	GameWorld()->InsertEntity(this);
}

void CSlugSlime::Tick()
{
	if(IsMarkedForDestroy())
		return;

	if(m_LifeSpan <= 0)
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	// Find other players
	for(CInfClassCharacter *p = (CInfClassCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CInfClassCharacter *)p->TypeNext())
	{
		if(!GameContext()->AreConnected(p->GetPos(), GetPos(), 84.0f))
			continue; // not in reach

		p->GetClass()->OnSlimeEffect(m_Owner);
	}

	if(random_prob(0.2f))
	{
		GameServer()->CreateDeath(m_Pos, m_Owner);
	}

	m_LifeSpan--;
}

void CSlugSlime::OnExplosion(const CInfCExplosionContext &context)
{
	if(context.Effect == ExplosionEffect::FreezeInfected)
	{
		Reset();
	}
}
