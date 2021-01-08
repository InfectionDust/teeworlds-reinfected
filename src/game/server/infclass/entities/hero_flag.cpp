/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "hero_flag.h"

#include <game/server/infclass/classes/humans/human.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/infcgamecontext.h>
#include <game/server/infclass/infcgamecontroller.h>
#include <game/server/infclass/infcserver.h>

int CHeroFlag::EntityId = 0;

CHeroFlag::CHeroFlag(CInfClassGameContext *pGameContext, int Owner)
	: CInfCEntity(pGameContext, EntityId, vec2(0, 0), Owner, ms_PhysSize)
{
	m_CoolDownTick = 0;
	for(int i=0; i<CHeroFlag::SHIELD_COUNT; i++)
	{
		m_IDs[i] = Server()->SnapNewID();
	}

	FindPosition();
	GameWorld()->InsertEntity(this);
}

CHeroFlag::~CHeroFlag()
{
	for(int i=0; i<CHeroFlag::SHIELD_COUNT; i++)
	{
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CHeroFlag::FindPosition()
{
	GetController()->FindHeroFlagPosition(this);
}

void CHeroFlag::GiveGift(CCharacter* pHero)
{
	pHero->IncreaseHealth(10);
	pHero->IncreaseArmor(10);

	// Increase only your *own* character health when on cooldown
	if(GetController()->GetGlobalHeroGiftCoolDown() > 0)
	{
		CInfClassCharacter *pChar = static_cast<CInfClassCharacter*>(pHero);
		CInfClassHuman *pHuman = static_cast<CInfClassHuman*>(pChar->GetClass());
		pHuman->GiveGift();
		GameContext()->CreateSound(GetPos(), SOUND_CTF_CAPTURE, CmaskOne(pHuman->GetCID()));
	}
	else
	{
		GetController()->OnFlagCollected(this);
		m_CoolDownTick = GetController()->GetHeroGiftCoolDown();

		// Find other players
		for(CInfClassCharacter *p = (CInfClassCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CInfClassCharacter *)p->TypeNext())
		{
			if(!p->GetClass()->IsHuman())
				continue;

			CInfClassHuman *pHuman = static_cast<CInfClassHuman*>(p->GetClass());
			pHuman->GiveGift();
		}
	}
}

void CHeroFlag::Tick()
{
	if(m_CoolDownTick > 0)
	{
		--m_CoolDownTick;
		return;
	}

	CCharacter *pOwner = GameContext()->GetPlayerChar(GetOwner());
	if(!pOwner)
		return;

	float Len = distance(pOwner->GetPos(), GetPos());
	if(Len < GetProximityRadius() + pOwner->GetProximityRadius())
	{
		FindPosition();
		GiveGift(pOwner);
	}
}

void CHeroFlag::Snap(int SnappingClient)
{
	if(m_CoolDownTick > 0)
		return;
	
	if(NetworkClipped(SnappingClient))
		return;
	
	if(SnappingClient != GetOwner())
		return;
	
//	CPlayer* pClient = GameServer()->m_apPlayers[SnappingClient];
//	if(pClient->GetClass() != PLAYERCLASS_HERO)
//		return;

//	if (GameServer()->GetHeroGiftCoolDown() <= 0)
//	{
//		float AngleStart = (2.0f * pi * Server()->Tick()/static_cast<float>(Server()->TickSpeed()))/CHeroFlag::SPEED;
//		float AngleStep = 2.0f * pi / CHeroFlag::SHIELD_COUNT;
		
//		for(int i=0; i<CHeroFlag::SHIELD_COUNT; i++)
//		{
//			CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDs[i], sizeof(CNetObj_Pickup)));
//			if(!pObj)
//				return;

//			vec2 PosStart = m_Pos + vec2(CHeroFlag::RADIUS * cos(AngleStart + AngleStep*i), CHeroFlag::RADIUS * sin(AngleStart + AngleStep*i));

//			pObj->m_X = (int)PosStart.x;
//			pObj->m_Y = (int)PosStart.y;
//			pObj->m_Type = i % 2 == 0 ? PICKUP_ARMOR : PICKUP_HEALTH;
//		}
//	}

	CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, TEAM_BLUE, sizeof(CNetObj_Flag));
	if(!pFlag)
		return;

	pFlag->m_X = (int)m_Pos.x;
	pFlag->m_Y = (int)m_Pos.y+16.0f;
	pFlag->m_Team = TEAM_BLUE;
}
