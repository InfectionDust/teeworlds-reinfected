#include "infected.h"

#include <game/server/infclass/entities/infccharacter.h>

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
