#include "slug.h"

#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/entities/slug_slime.h>
#include <game/server/infclass/infcgamecontext.h>

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

bool CInfClassSlug::IsFullAutoFireWeapon(int Weapon) const
{
	switch (Weapon) {
		case WEAPON_HAMMER:
			return true;
		default:
			return false;
	}
}

void CInfClassSlug::OnHammerFired()
{
	CInfClassInfected::OnHammerFired();
	m_pCharacter->SetAttackAnimationEnabled(false);

	vec2 CheckPos = GetPos() + GetDirection() * 64.0f;
	if(GameServer()->Collision()->IntersectLine(GetPos(), CheckPos, 0x0, &CheckPos))
	{
		float Distance = 99999999.0f;
		for(CSlugSlime* pSlime = (CSlugSlime*) GameWorld()->FindFirst(CSlugSlime::EntityId); pSlime; pSlime = (CSlugSlime*) pSlime->TypeNext())
		{
			if(distance(pSlime->GetPos(), GetPos()) < Distance)
			{
				Distance = distance(pSlime->GetPos(), GetPos());
			}
		}

		if(Distance > 84.0f)
		{
			new CSlugSlime(GameContext(), CheckPos, GetCID());
			m_pCharacter->SetAttackAnimationEnabled(true);
		}
	}
}

void CInfClassSlug::OnSlimeEffect(int)
{
	// Slime has no effect on Slug characters
}
