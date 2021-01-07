#ifndef GAME_SERVER_INFCLASS_CLASSES_SLUG_H
#define GAME_SERVER_INFCLASS_CLASSES_SLUG_H

#include "infected.h"

class CInfClassSlug : public CInfClassInfected
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassSlug();

	static const char *ClassName() { return "slug"; }
	const char *GetClassName() override;
	const char *GetClassDisplayName() override;
	void SetupSkin() override;

	bool IsFullAutoFireWeapon(int Weapon) const;

	void OnHammerFired() override;

	void OnSlimeEffect(int Owner) override;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_SLUG_H
