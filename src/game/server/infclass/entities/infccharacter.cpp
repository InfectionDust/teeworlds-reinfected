#include "infccharacter.h"

#include "../classes/infcplayerclass.h"

CInfClassCharacter::CInfClassCharacter(CGameWorld *pWorld)
	: CCharacter(pWorld)
	, m_Grounded(false)
{
}

void CInfClassCharacter::Tick()
{
	CCharacter::Tick();
	if (m_Core.m_TriggeredEvents & COREEVENTFLAG_AIR_JUMP) {
		m_pClass->OnJumped(JumpType::Air);
	} else if (m_Core.m_TriggeredEvents & COREEVENTFLAG_GROUND_JUMP) {
		m_pClass->OnJumped(JumpType::Ground);
	}
	bool grounded = IsGrounded();
	if (m_Grounded != grounded) {
		if (grounded) {
			m_pClass->OnGrounded();
		}
		m_Grounded = grounded;
	}
}

void CInfClassCharacter::OnCharacterSpawn()
{
	// default health
	IncreaseHealth(10);
}

void CInfClassCharacter::EnableJump()
{
	m_Core.m_Jumped &= ~2;
}

void CInfClassCharacter::SetActiveWeapon(int Weapon)
{
	m_ActiveWeapon = Weapon;
}

void CInfClassCharacter::SetLastWeapon(int Weapon)
{
	m_LastWeapon = Weapon;
}

void CInfClassCharacter::TakeAllWeapons()
{
	for (WeaponStat &weapon : m_aWeapons)
	{
		weapon.m_Got = false;
	}
}

void CInfClassCharacter::SetClass(CInfClassPlayerClass *pClass)
{
	m_pClass = pClass;
}
