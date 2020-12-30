#include "infcplayerclass.h"

#include <base/system.h>
#include <game/gamecore.h>
#include <generated/server_data.h>
#include <game/server/entities/laser.h>
#include <game/server/entities/projectile.h>
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

CConfig *CInfClassPlayerClass::Config()
{
	if(m_pCharacter)
		return m_pCharacter->Config();

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

void CInfClassPlayerClass::OnWeaponFired(int Weapon)
{
	switch(Weapon)
	{
		case WEAPON_HAMMER:
			OnHammerFired();
			break;
		case WEAPON_GUN:
			OnGunFired();
			break;
		case WEAPON_SHOTGUN:
			OnShotgunFired();
			break;
		case WEAPON_GRENADE:
			OnGrenadeFired();
			break;
		case WEAPON_LASER:
			OnLaserFired();
			break;
		case WEAPON_NINJA:
			OnNinjaFired();
			break;
		default:
			break;
	}
}

void CInfClassPlayerClass::OnHammerFired()
{
	GameServer()->CreateSound(GetPos(), SOUND_HAMMER_FIRE);

	vec2 ProjStartPos = GetPos()+GetDirection()*GetProximityRadius()*0.75f;

	CInfClassCharacter *apEnts[MAX_CLIENTS];
	int Hits = 0;
	float Radius = GetProximityRadius()*0.5f;
	int Num = GameWorld()->FindEntities(ProjStartPos, Radius, (CEntity**)apEnts,
										MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);

	for (int i = 0; i < Num; ++i)
	{
		CInfClassCharacter *pTarget = apEnts[i];

		if ((pTarget == m_pCharacter) || GameServer()->Collision()->IntersectLine(ProjStartPos, pTarget->GetPos(), nullptr, nullptr))
			continue;

		// set his velocity to fast upward (for now)
		if(length(pTarget->GetPos()-ProjStartPos) > 0.0f)
			GameServer()->CreateHammerHit(pTarget->GetPos()-normalize(pTarget->GetPos()-ProjStartPos)*Radius);
		else
			GameServer()->CreateHammerHit(ProjStartPos);

		vec2 Dir;
		if (length(pTarget->GetPos() - GetPos()) > 0.0f)
			Dir = normalize(pTarget->GetPos() - GetPos());
		else
			Dir = vec2(0.f, -1.f);

		vec2 Force = vec2(0.f, -1.f) + normalize(Dir + vec2(0.f, -1.1f)) * 10.0f;
		pTarget->TakeDamage(Force, Dir*-1, g_pData->m_Weapons.m_Hammer.m_pBase->m_Damage,
			GetCID(), WEAPON_HAMMER);
		Hits++;
	}

	// if we Hit anything, we have to wait for the reload
	if(Hits)
		m_pCharacter->SetReloadTimer(Server()->TickSpeed()/3);
}

void CInfClassPlayerClass::OnGunFired()
{
	vec2 Direction = GetDirection();
	vec2 ProjStartPos = GetPos()+Direction*GetProximityRadius()*0.75f;

	new CProjectile(GameWorld(), WEAPON_GUN,
		GetCID(),
		ProjStartPos,
		Direction,
		(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime),
		g_pData->m_Weapons.m_Gun.m_pBase->m_Damage, false, 0, -1, WEAPON_GUN);

	GameServer()->CreateSound(GetPos(), SOUND_GUN_FIRE);
}

void CInfClassPlayerClass::OnShotgunFired()
{
	vec2 Direction = GetDirection();
	vec2 ProjStartPos = GetPos()+Direction*GetProximityRadius()*0.75f;
	int ShotSpread = 2;

	for(int i = -ShotSpread; i <= ShotSpread; ++i)
	{
		float Spreading[] = {-0.185f, -0.070f, 0, 0.070f, 0.185f};
		float a = angle(Direction);
		a += Spreading[i+2];
		float v = 1-(absolute(i)/(float)ShotSpread);
		float Speed = mix((float)GameServer()->Tuning()->m_ShotgunSpeeddiff, 1.0f, v);
		new CProjectile(GameWorld(), WEAPON_SHOTGUN,
			GetCID(),
			ProjStartPos,
			vec2(cosf(a), sinf(a))*Speed,
			(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_ShotgunLifetime),
			g_pData->m_Weapons.m_Shotgun.m_pBase->m_Damage, false, 0, -1, WEAPON_SHOTGUN);
	}

	GameServer()->CreateSound(GetPos(), SOUND_SHOTGUN_FIRE);
}

void CInfClassPlayerClass::OnGrenadeFired()
{
	vec2 Direction = GetDirection();
	vec2 ProjStartPos = GetPos()+Direction*GetProximityRadius()*0.75f;

	new CProjectile(GameWorld(), WEAPON_GRENADE,
		GetCID(),
		ProjStartPos,
		Direction,
		(int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GrenadeLifetime),
		g_pData->m_Weapons.m_Grenade.m_pBase->m_Damage, true, 0, SOUND_GRENADE_EXPLODE, WEAPON_GRENADE);

	GameServer()->CreateSound(GetPos(), SOUND_GRENADE_FIRE);
}

void CInfClassPlayerClass::OnLaserFired()
{
	new CLaser(GameWorld(), GetPos(), GetDirection(), GameServer()->Tuning()->m_LaserReach, GetCID());
	GameServer()->CreateSound(GetPos(), SOUND_LASER_FIRE);
}

void CInfClassPlayerClass::OnNinjaFired()
{
}
