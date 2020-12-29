#ifndef GAME_SERVER_INFCLASS_CLASSES_HUNTER_H
#define GAME_SERVER_INFCLASS_CLASSES_HUNTER_H

#include "infected.h"

class CInfClassHunter : public CInfClassInfected
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassHunter();

	static const char *ClassName() { return "hunter"; }
	void SetupSkin() override;

	void OnJumped(JumpType jumpType) override;
	void OnGrounded() override;

protected:
	int m_AirJumpCounter = 0;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_HUNTER_H
