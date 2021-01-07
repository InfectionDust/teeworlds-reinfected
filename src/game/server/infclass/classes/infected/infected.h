#ifndef GAME_SERVER_INFCLASS_CLASSES_INFECTED_H
#define GAME_SERVER_INFCLASS_CLASSES_INFECTED_H

#include "../infcplayerclass.h"

#include <game/server/alloc.h>

class CInfClassInfected : public CInfClassPlayerClass
{
public:
	CInfClassInfected();

	bool IsHuman() const final { return false; }
	int GetNormalEmote() const override;

	void OnCharacterSpawned() override;
	void OnHammerHitCharacter(CInfClassCharacter *pTarget, HammerFireContext *pHitContext) override;

	void OnSlimeEffect(int Owner) override;
	void OnExplosion(const CInfCExplosionContext &context, bool *pHit) override;

protected:
	int m_HealTick = 0;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_INFECTED_H
