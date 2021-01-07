/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include "growing_explosion.h"

#include <game/server/gamecontext.h>
#include <game/server/infclass/classes/infcplayerclass.h>
#include <game/server/infclass/entities/infccharacter.h>
#include <game/server/infclass/entities/slug_slime.h>
#include <game/server/infclass/infcgamecontext.h>

int CGrowingExplosion::EntityId = 0;

CGrowingExplosion::CGrowingExplosion(CInfClassGameContext *pGameContext, vec2 Pos, int Owner, vec2 Dir, int Radius, ExplosionEffect Effect)
		: CInfCEntity(pGameContext, EntityId, Pos, Owner),
		m_pGrowingMap(NULL),
		m_pGrowingMapVec(NULL)
{
	m_MaxGrowing = Radius;
	m_GrowingMap_Length = (2*m_MaxGrowing+1);
	m_GrowingMap_Size = (m_GrowingMap_Length*m_GrowingMap_Length);

	m_pGrowingMap = new int[m_GrowingMap_Size];
	m_pGrowingMapVec = new vec2[m_GrowingMap_Size];

	m_StartTick = Server()->Tick();
	m_ExplosionEffect = Effect;

	mem_zero(m_Hit, sizeof(m_Hit));

	GameWorld()->InsertEntity(this);

	vec2 explosionTile = vec2(16.0f, 16.0f) + vec2(
		static_cast<float>(static_cast<int>(round(m_Pos.x))/32)*32.0,
		static_cast<float>(static_cast<int>(round(m_Pos.y))/32)*32.0);

	//Check is the tile is occuped, and if the direction is valid
	if(GameServer()->Collision()->CheckPoint(explosionTile) && length(Dir) <= 1.1)
	{
		m_SeedPos = vec2(16.0f, 16.0f) + vec2(
		static_cast<float>(static_cast<int>(round(m_Pos.x + 32.0f*Dir.x))/32)*32.0,
		static_cast<float>(static_cast<int>(round(m_Pos.y + 32.0f*Dir.y))/32)*32.0);
	}
	else
	{
		m_SeedPos = explosionTile;
	}

	m_SeedX = static_cast<int>(round(m_SeedPos.x))/32;
	m_SeedY = static_cast<int>(round(m_SeedPos.y))/32;

	for(int j=0; j<m_GrowingMap_Length; j++)
	{
		for(int i=0; i<m_GrowingMap_Length; i++)
		{
			vec2 Tile = m_SeedPos + vec2(32.0f*(i-m_MaxGrowing), 32.0f*(j-m_MaxGrowing));
			if(GameServer()->Collision()->CheckPoint(Tile) || distance(Tile, m_SeedPos) > m_MaxGrowing*32.0f)
			{
				m_pGrowingMap[j*m_GrowingMap_Length+i] = -2;
			}
			else
			{
				m_pGrowingMap[j*m_GrowingMap_Length+i] = -1;
			}

			m_pGrowingMapVec[j*m_GrowingMap_Length+i] = vec2(0.0f, 0.0f);
		}
	}

	m_pGrowingMap[m_MaxGrowing*m_GrowingMap_Length+m_MaxGrowing] = Server()->Tick();

	switch(m_ExplosionEffect)
	{
		case ExplosionEffect::Invalid:
		case ExplosionEffect::BoomInfected:
			break;
	}
}

CGrowingExplosion::~CGrowingExplosion()
{
	if(m_pGrowingMap)
	{
		delete[] m_pGrowingMap;
		m_pGrowingMap = NULL;
	}

	if(m_pGrowingMapVec)
	{
		delete[] m_pGrowingMapVec;
		m_pGrowingMapVec = NULL;
	}
}

void CGrowingExplosion::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CGrowingExplosion::Tick()
{
	if(IsMarkedForDestroy())
		return;

	int tick = Server()->Tick();
	if((tick - m_StartTick) > m_MaxGrowing)
	{
		GameServer()->m_World.DestroyEntity(this);
		return;
	}

	bool NewTile = false;

	for(int j=0; j<m_GrowingMap_Length; j++)
	{
		for(int i=0; i<m_GrowingMap_Length; i++)
		{
			if(m_pGrowingMap[j*m_GrowingMap_Length+i] == -1)
			{
				bool FromLeft = (i > 0 && m_pGrowingMap[j*m_GrowingMap_Length+i-1] < tick && m_pGrowingMap[j*m_GrowingMap_Length+i-1] >= 0);
				bool FromRight = (i < m_GrowingMap_Length-1 && m_pGrowingMap[j*m_GrowingMap_Length+i+1] < tick && m_pGrowingMap[j*m_GrowingMap_Length+i+1] >= 0);
				bool FromTop = (j > 0 && m_pGrowingMap[(j-1)*m_GrowingMap_Length+i] < tick && m_pGrowingMap[(j-1)*m_GrowingMap_Length+i] >= 0);
				bool FromBottom = (j < m_GrowingMap_Length-1 && m_pGrowingMap[(j+1)*m_GrowingMap_Length+i] < tick && m_pGrowingMap[(j+1)*m_GrowingMap_Length+i] >= 0);

				if(FromLeft || FromRight || FromTop || FromBottom)
				{
					m_pGrowingMap[j*m_GrowingMap_Length+i] = tick;
					NewTile = true;
					vec2 TileCenter = m_SeedPos + vec2(32.0f*(i-m_MaxGrowing) - 16.0f + random_float()*32.0f, 32.0f*(j-m_MaxGrowing) - 16.0f + random_float()*32.0f);
					switch(m_ExplosionEffect)
					{
						case ExplosionEffect::Invalid:
							break;
						case ExplosionEffect::BoomInfected:
							if (random_prob(0.2f))
							{
								GameServer()->CreateExplosion(TileCenter, m_Owner, WEAPON_HAMMER, 6);
							}
							break;
					}
				}
			}
		}
	}

	if(NewTile)
	{
		switch(m_ExplosionEffect)
		{
			case ExplosionEffect::Invalid:
				break;
			case ExplosionEffect::BoomInfected:
				if(random_prob(0.1f))
				{
					GameServer()->CreateSound(m_Pos, SOUND_PLAYER_DIE);
				}
				break;
		}
	}

	// Find other players
	CInfCExplosionContext context;
	context.Effect = m_ExplosionEffect;
	context.From = GetOwner();
	context.MaxGrowing = m_MaxGrowing;
	context.SeedPos = m_SeedPos;
	context.StartTick = m_StartTick;
	for(CInfClassCharacter *p = (CInfClassCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CInfClassCharacter *)p->TypeNext())
	{
		if (!IsInRadius(p))
			continue;

		CInfClassPlayerClass *pClass = p->GetClass();
		if(m_Hit[pClass->GetCID()])
			continue;

		bool Hit = false;
		pClass->OnExplosion(context, &Hit);
		if(Hit)
		{
			m_Hit[pClass->GetCID()] = true;
		}
	}
}

void CGrowingExplosion::TickPaused()
{
	++m_StartTick;
}

template<class E>
bool CGrowingExplosion::IsInRadius(const E *pEntity)
{
	int tileX = m_MaxGrowing + static_cast<int>(round(pEntity->GetPos().x))/32 - m_SeedX;
	int tileY = m_MaxGrowing + static_cast<int>(round(pEntity->GetPos().y))/32 - m_SeedY;

	if(tileX < 0 || tileX >= m_GrowingMap_Length || tileY < 0 || tileY >= m_GrowingMap_Length)
		return false;

	int k = tileY*m_GrowingMap_Length+tileX;
	if(m_pGrowingMap[k] >= 0)
	{
		if(Server()->Tick() - m_pGrowingMap[k] < Server()->TickSpeed()/4)
		{
			return true;
		}
	}

	return false;
}
