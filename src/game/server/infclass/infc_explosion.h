#ifndef GAME_SERVER_INFCLASS_EXPLOSION_H
#define GAME_SERVER_INFCLASS_EXPLOSION_H

#include <base/vmath.h>
#include <game/gamecore.h>

enum class ExplosionEffect
{
	Invalid,
	FreezeInfected,
	PoisonInfected,
	ElectricInfected,
	BoomInfected,
	HealHumans,
};

class CInfCExplosionContext
{
public:
	ExplosionEffect Effect = ExplosionEffect::Invalid;
	int From = 0;
	int MaxGrowing = 0;
	vec2 SeedPos;
	int StartTick = 0;
};

#endif // GAME_SERVER_INFCLASS_EXPLOSION_H
