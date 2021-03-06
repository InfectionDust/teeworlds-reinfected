#include "mercenary.h"

#include <engine/shared/config.h>
#include <game/server/entities/projectile.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/entities/merc_bomb.h>
#include <game/server/infclass/infcgamecontext.h>

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

void CInfClassMercenary::SetupSkin()
{
	CInfClassHuman::SetupSkin(); // Reset to default

	// Hero (correct colors)
	str_copy(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING], "stripes", sizeof(m_SkinInfo.m_aaSkinPartNames[SKINPART_MARKING]));
	m_SkinInfo.m_aUseCustomColors[SKINPART_BODY] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_HANDS] = 1;
	m_SkinInfo.m_aUseCustomColors[SKINPART_FEET] = 1;
	m_SkinInfo.m_aSkinPartColors[SKINPART_BODY] = 10187898;
	m_SkinInfo.m_aSkinPartColors[SKINPART_MARKING] = -16711808;
	m_SkinInfo.m_aSkinPartColors[SKINPART_DECORATION] = 65408;
	m_SkinInfo.m_aSkinPartColors[SKINPART_HANDS] = 750848;
	m_SkinInfo.m_aSkinPartColors[SKINPART_FEET] = 1944919;
	m_SkinInfo.m_aSkinPartColors[SKINPART_EYES] = 65408;
}

void CInfClassMercenary::OnCharacterSpawned()
{
	CInfClassHuman::OnCharacterSpawned();

	m_pCharacter->GiveWeapon(WEAPON_HAMMER, -1);
	m_pCharacter->GiveWeapon(WEAPON_GUN, 10);

	m_pCharacter->SetLastWeapon(WEAPON_HAMMER);
	m_pCharacter->SetActiveWeapon(WEAPON_GUN);
}

void CInfClassMercenary::OnHammerFired()
{
	if (!Config()->m_InfMercLove)
		return;

	CMercenaryBomb* pCurrentBomb = nullptr;
	for(CMercenaryBomb *pBomb = (CMercenaryBomb*) GameWorld()->FindFirst(CMercenaryBomb::EntityId); pBomb; pBomb = (CMercenaryBomb*) pBomb->TypeNext())
	{
		if(pBomb->GetOwner() == GetCID())
		{
			pCurrentBomb = pBomb;
			break;
		}
	}

	if(pCurrentBomb)
	{
		if(pCurrentBomb->ReadyToExplode() || distance(pCurrentBomb->GetPos(), GetPos()) > 80.0f)
			pCurrentBomb->Explode();
		else
		{
			pCurrentBomb->IncreaseDamage(WEAPON_HAMMER);
			GameServer()->CreateSound(GetPos(), SOUND_PICKUP_ARMOR);
		}
	}
	else
	{
		new CMercenaryBomb(GameContext(), GetPos(), GetCID());
		GameServer()->CreateSound(GetPos(), SOUND_PICKUP_ARMOR);
	}

	m_pCharacter->SetReloadTimer(Server()->TickSpeed()/4);
}

void CInfClassMercenary::OnGunFired()
{
	vec2 ProjStartPos = GetPos()+GetDirection()*GetProximityRadius()*0.75f;
	vec2 Direction = GetDirection();

	CProjectile *pProj = new CProjectile(GameWorld(), WEAPON_GUN,
	                                     GetCID(),
	                                     ProjStartPos,
	                                     Direction,
	                                     (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime),
	                                     1, 0, 0, -1, WEAPON_GUN);

	// pack the Projectile and send it to the client Directly
	CNetObj_Projectile p;
	pProj->FillInfo(&p);

	CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
	Msg.AddInt(1);
	for(unsigned i = 0; i < sizeof(CNetObj_Projectile)/sizeof(int); i++)
		Msg.AddInt(((int *)&p)[i]);

	Server()->SendMsg(&Msg, MSGFLAG_VITAL, GetCID());

	float MaxSpeed = GameServer()->Tuning()->m_GroundControlSpeed*1.7f;
	vec2 Recoil = Direction*(-MaxSpeed/5.0f);
	m_pCharacter->SaturateVelocity(Recoil, MaxSpeed);

	GameServer()->CreateSound(GetPos(), SOUND_HOOK_LOOP);
}
