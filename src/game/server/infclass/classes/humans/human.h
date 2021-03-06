#ifndef GAME_SERVER_INFCLASS_CLASSES_HUMAN_H
#define GAME_SERVER_INFCLASS_CLASSES_HUMAN_H

#include "../infcplayerclass.h"

#include <game/server/alloc.h>

class CInfClassHuman : public CInfClassPlayerClass
{
public:
	explicit CInfClassHuman();

	bool IsHuman() const final { return true; }

	bool IsFullAutoFireWeapon(int Weapon) const;

	void OnSlimeEffect(int Owner) override;
	void OnExplosion(const CInfCExplosionContext &context, bool *pHit) override;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_HUMAN_H
