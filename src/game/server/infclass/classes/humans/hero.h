#ifndef GAME_SERVER_INFCLASS_CLASSES_HERO_H
#define GAME_SERVER_INFCLASS_CLASSES_HERO_H

#include "human.h"

class CHeroFlag;

class CInfClassHero : public CInfClassHuman
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassHero();

	static const char *ClassName() { return "hero"; }
	const char *GetClassName() override;
	const char *GetClassDisplayName() override;
	void SetupSkin() override;

	void Tick() override;
	void OnCharacterSpawned() override;

	void GiveGift() override;

protected:
	CHeroFlag* m_pHeroFlag = nullptr;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_HERO_H
