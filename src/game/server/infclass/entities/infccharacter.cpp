#include "infccharacter.h"

CInfClassCharacter::CInfClassCharacter(CGameWorld *pWorld)
	: CCharacter(pWorld)
{
}

void CInfClassCharacter::OnCharacterSpawn()
{
	// default health
	IncreaseHealth(10);
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
