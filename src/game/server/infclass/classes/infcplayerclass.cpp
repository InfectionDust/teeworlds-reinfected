#include "infcplayerclass.h"

#include <base/system.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/infcgamecontext.h>
#include <game/server/infclass/infcplayer.h>

CInfClassPlayerClass::CInfClassPlayerClass()
{
}

CInfClassGameContext *CInfClassPlayerClass::GameContext() const
{
	if(m_pCharacter)
		return m_pCharacter->GameContext();

	return nullptr;
}

// A lot of code use GameServer() as CGameContext* getter, so let it be.
CGameContext *CInfClassPlayerClass::GameServer() const
{
	return GameContext();
}

CGameWorld *CInfClassPlayerClass::GameWorld() const
{
	if(m_pCharacter)
		return m_pCharacter->GameWorld();

	return nullptr;
}

IServer *CInfClassPlayerClass::Server()
{
	if(m_pCharacter)
		return m_pCharacter->Server();

	return nullptr;
}

CInfClassPlayer *CInfClassPlayerClass::GetPlayer()
{
	if(m_pCharacter)
		return static_cast<CInfClassPlayer*>(m_pCharacter->GetPlayer());

	return nullptr;
}

int CInfClassPlayerClass::GetCID()
{
	const CInfClassPlayer *pPlayer = GetPlayer();
	if(pPlayer)
		return pPlayer->GetCID();

	return -1;
}

vec2 CInfClassPlayerClass::GetPos() const
{
	if(m_pCharacter)
		return m_pCharacter->GetPos();

	return vec2(0, 0);
}

vec2 CInfClassPlayerClass::GetDirection() const
{
	if(m_pCharacter)
		return m_pCharacter->GetDirection();

	return vec2(0, 0);
}

float CInfClassPlayerClass::GetProximityRadius() const
{
	if(m_pCharacter)
		return m_pCharacter->GetProximityRadius();

	return 0;
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

void CInfClassPlayerClass::OnJumped(JumpType)
{
}
