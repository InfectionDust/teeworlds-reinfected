#include "infected.h"

#include <engine/shared/config.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/infcgamecontext.h>
#include <generated/server_data.h>

CInfClassInfected::CInfClassInfected()
	: CInfClassPlayerClass()
{
}

void CInfClassInfected::OnCharacterSpawned()
{
	CInfClassPlayerClass::OnCharacterSpawned();

	m_pCharacter->SetActiveWeapon(WEAPON_HAMMER);
	m_pCharacter->SetLastWeapon(WEAPON_HAMMER);
	m_pCharacter->GiveWeapon(WEAPON_HAMMER, -1);
}

void CInfClassInfected::OnHammerHitCharacter(CInfClassCharacter *pTarget, CInfClassPlayerClass::HammerFireContext *pHitContext)
{
	// set the target velocity to fast upward (for now)
	if(length(pTarget->GetPos() - pHitContext->ProjStartPos) > 0.0f)
	{
		const vec2 HitEffectDirection = normalize(pTarget->GetPos() - pHitContext->ProjStartPos);
		GameServer()->CreateHammerHit(pTarget->GetPos()-HitEffectDirection * pHitContext->Radius);
	}
	else
		GameServer()->CreateHammerHit(pHitContext->ProjStartPos);

	vec2 Dir;
	if (length(pTarget->GetPos() - GetPos()) > 0.0f)
		Dir = normalize(pTarget->GetPos() - GetPos());
	else
		Dir = vec2(0.f, -1.f);

	const vec2 HitVelocity = vec2(0.f, -1.f) + normalize(Dir + vec2(0.f, -1.1f)) * 10.0f;
	CInfClassPlayerClass *pTargetClass = pTarget->GetClass();
	if(pTargetClass->IsZombie())
	{
		// Heal
		if(pTarget->IncreaseOverallHp(4))
		{
			pTarget->SetEmote(EMOTE_HAPPY, Server()->Tick() + Server()->TickSpeed());
			m_pCharacter->IncreaseOverallHp(1);
		}
		pTarget->GiveVelocity(HitVelocity);
	}
	else
	{
		pTarget->TakeDamage(HitVelocity, Dir*-1, g_pData->m_Weapons.m_Hammer.m_pBase->m_Damage, GetCID(), WEAPON_HAMMER);
	}
}

void CInfClassInfected::OnSlimeEffect(int)
{
	m_pCharacter->SetEmote(EMOTE_HAPPY, Server()->Tick());
	if(Server()->Tick() >= m_HealTick + (Server()->TickSpeed() / Config()->m_InfSlimeHealRate))
	{
		m_HealTick = Server()->Tick();
		m_pCharacter->IncreaseHealth(1);
	}
}
