#include "infcplayerclass.h"

#include <base/system.h>
#include <game/server/infclass/entities/infccharacter.h>

CInfClassPlayerClass::CInfClassPlayerClass()
{
}

void CInfClassPlayerClass::SetCharacter(CInfClassCharacter *character)
{
	m_pCharacter = character;
	m_pCharacter->SetClass(this);
}

void CInfClassPlayerClass::SetupSkin()
{
	for(int part = 0; part < NUM_SKINPARTS; part++)
	{
		if ((part == SKINPART_MARKING) || (part == SKINPART_DECORATION))
		{
			str_copy(m_SkinInfo.m_aaSkinPartNames[part], "", sizeof(m_SkinInfo.m_aaSkinPartNames[part]));
		}
		else
		{
			str_copy(m_SkinInfo.m_aaSkinPartNames[part], "standard", sizeof(m_SkinInfo.m_aaSkinPartNames[part]));
		}

		m_SkinInfo.m_aUseCustomColors[part] = 0;
		m_SkinInfo.m_aSkinPartColors[part] = 0;
	}
}

void CInfClassPlayerClass::OnCharacterSpawned()
{
	m_pCharacter->TakeAllWeapons();
}