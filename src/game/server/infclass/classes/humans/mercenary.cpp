#include "mercenary.h"

#include <game/server/entities/projectile.h>
#include <game/server/infclass/entities/infccharacter.h>

CInfClassMercenary::CInfClassMercenary()
	: CInfClassHuman()
{
}

const char *CInfClassMercenary::GetClassName()
{
	return ClassName();
}

const char *CInfClassMercenary::GetClassDisplayName()
{
	return "Mercenary";
}

void CInfClassMercenary::OnCharacterSpawned()
{
	CInfClassHuman::OnCharacterSpawned();

	m_pCharacter->GiveWeapon(WEAPON_HAMMER, -1);
	m_pCharacter->GiveWeapon(WEAPON_GUN, 10);

	m_pCharacter->SetLastWeapon(WEAPON_HAMMER);
	m_pCharacter->SetActiveWeapon(WEAPON_GUN);
}
