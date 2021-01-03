#include "slug.h"

#include <game/server/infclass/entities/infccharacter.h>

CInfClassSlug::CInfClassSlug()
	: CInfClassInfected()
{
}

const char *CInfClassSlug::GetClassName()
{
	return ClassName();
}

const char *CInfClassSlug::GetClassDisplayName()
{
	return "Slug";
}

void CInfClassSlug::SetupSkin()
{
	CInfClassInfected::SetupSkin(); // Reset to default

	// Slug (rough colors)
	str_copy(m_SkinInfo.m_aaSkinPartNames[SKINPART_BODY], "bear", sizeof(m_SkinInfo.m_aaSkinPartNames[SKINPART_BODY]));
	//str_copy(m_SkinInfo.m_aaSkinPartNames[SKINPART_BODY], "koala", sizeof(m_SkinInfo.m_aaSkinPartNames[SKINPART_BODY]));
	// str_copy(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING], "twinbelly", sizeof(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING]));
	m_SkinInfo.m_aUseCustomColors[SKINPART_BODY] = 1;
//	m_SkinInfo.m_aUseCustomColors[SKINPART_MARKING] = 1;
//	m_SkinInfo.m_aUseCustomColors[SKINPART_HANDS] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_FEET] = 1;
	
	// Wanted slug body:
	// - rgb(120, 192, 0) 
	// - rgb: 0x85, 0xc0, 0x00
	// - H; 83, S:100, L: 37.6
	// - H:0x3a, 0xff, 0x60
	
	// Body: #79c000 RGB == 120% hue, 100 sat, 61.2 value
	// Body: #0x3aff60 (HSL) = 3866368 (dec)
	m_SkinInfo.m_aSkinPartColors[SKINPART_BODY] = 3866368;
//	m_SkinInfo.m_aSkinPartColors[SKINPART_MARKING] = -11993216; // Some color
//	m_SkinInfo.m_aSkinPartColors[SKINPART_DECORATION] = 65408; // Some color
//	m_SkinInfo.m_aSkinPartColors[SKINPART_HANDS] = 65414; // Guess from 'feet'
	m_SkinInfo.m_aSkinPartColors[SKINPART_FEET] = 65414; // Correct
	//	m_SkinInfo.m_aSkinPartColors[SKINPART_EYES] = 65408; // Some color
}
