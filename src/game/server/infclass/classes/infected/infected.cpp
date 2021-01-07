#include "infected.h"

#include <engine/shared/config.h>
#include <game/server/infclass/entities/growing_explosion.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/infc_explosion.h>
#include <game/server/infclass/infcgamecontext.h>
#include <generated/server_data.h>

CInfClassInfected::CInfClassInfected()
	: CInfClassPlayerClass()
{
}

int CInfClassInfected::GetNormalEmote() const
{
	return EMOTE_ANGRY;
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

void CInfClassInfected::OnExplosion(const CInfCExplosionContext &context, bool *pHit)
{
	switch(context.Effect)
	{
		case ExplosionEffect::FreezeInfected:
			// TODO: m_pCharacter->Freeze(3.0f, m_Owner, FREEZEREASON_FLASH);
			GameServer()->SendEmoticon(GetCID(), EMOTICON_QUESTION);
			break;
		case ExplosionEffect::PoisonInfected:
			m_pCharacter->GetClass()->Poison(Config()->m_InfPoisonDamage, context.From);
			GameServer()->SendEmoticon(GetCID(), EMOTICON_DROP);
			break;
		case ExplosionEffect::BoomInfected:
		{
			break;
		}
		case ExplosionEffect::ElectricInfected:
		{
			const int MaxGrowing = context.MaxGrowing;
			int tick = Server()->Tick();
			int Damage = 5+20*((float)(MaxGrowing - min(tick - context.StartTick, MaxGrowing)))/(context.MaxGrowing);
			vec2 Source = vec2(0, 0);
			m_pCharacter->TakeDamage(normalize(GetPos() - context.SeedPos)*10.0f, Source, Damage, context.From, WEAPON_HAMMER);
			break;
		}
		default:
			return;
	}

	*pHit = true;
}
