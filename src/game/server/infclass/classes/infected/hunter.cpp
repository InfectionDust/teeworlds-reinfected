#include "hunter.h"

#include <game/server/infclass/entities/infccharacter.h>

CInfClassHunter::CInfClassHunter()
	: CInfClassInfected()
	, m_AirJumpCounter(0)
{
}

void CInfClassHunter::SetupSkin()
{
	CInfClassInfected::SetupSkin(); // Reset to default

	// Hunter (rough colors)
	str_copy(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING], "warpaint", sizeof(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING]));
	m_SkinInfo.m_aUseCustomColors[SKINPART_BODY] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_MARKING] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_HANDS] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_FEET] = 1;
	m_SkinInfo.m_aSkinPartColors[SKINPART_BODY] = 3931970;
	m_SkinInfo.m_aSkinPartColors[SKINPART_MARKING] = -11993216;
	m_SkinInfo.m_aSkinPartColors[SKINPART_DECORATION] = 65408;
	m_SkinInfo.m_aSkinPartColors[SKINPART_HANDS] = 750337;
	m_SkinInfo.m_aSkinPartColors[SKINPART_FEET] = 750337;
	m_SkinInfo.m_aSkinPartColors[SKINPART_EYES] = 65408;
}

void CInfClassHunter::OnJumped(JumpType jumpType)
{
	if (jumpType != JumpType::Air)
	{
		return;
	}
	
	++m_AirJumpCounter;
	
	if (m_AirJumpCounter < 2) {
		m_pCharacter->EnableJump();
	}
}

void CInfClassHunter::OnGrounded()
{
	m_AirJumpCounter = 0;
}
