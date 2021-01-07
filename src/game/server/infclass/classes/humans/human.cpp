#include "human.h"

#include <engine/shared/config.h>

CInfClassHuman::CInfClassHuman()
	: CInfClassPlayerClass()
{
}

bool CInfClassHuman::IsFullAutoFireWeapon(int Weapon) const
{
	switch (Weapon) {
		case WEAPON_GUN:
		case WEAPON_GRENADE:
		case WEAPON_SHOTGUN:
		case WEAPON_LASER:
			return true;
		default:
			return false;
	}
}

void CInfClassHuman::OnSlimeEffect(int Owner)
{
	int Count = Config()->m_InfSlimePoisonDuration;
	Poison(Count, Owner);
}

void CInfClassHuman::OnExplosion(const CInfCExplosionContext &context, bool *pHit)
{
}
