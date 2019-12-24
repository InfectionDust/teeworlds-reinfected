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
