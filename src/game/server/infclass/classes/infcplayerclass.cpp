#include "infcplayerclass.h"

#include <base/system.h>

CInfClassPlayerClass::CInfClassPlayerClass()
{
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
