#include "mercenary.h"

#include <game/server/entities/projectile.h>
#include <game/server/infclass/entities/infccharacter.h>
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

void CInfClassMercenary::OnCharacterSpawned()
{
	CInfClassHuman::OnCharacterSpawned();

	m_pCharacter->GiveWeapon(WEAPON_HAMMER, -1);
	m_pCharacter->GiveWeapon(WEAPON_GUN, 10);

	m_pCharacter->SetLastWeapon(WEAPON_HAMMER);
	m_pCharacter->SetActiveWeapon(WEAPON_GUN);
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
