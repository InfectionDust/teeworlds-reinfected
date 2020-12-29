#ifndef GAME_SERVER_INFCLASS_CLASSES_HUNTER_H
#define GAME_SERVER_INFCLASS_CLASSES_HUNTER_H

#include "infected.h"

class CInfClassHunter : public CInfClassInfected
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassHunter();

	static const char *ClassName() { return "hunter"; }
};

#endif // GAME_SERVER_INFCLASS_CLASSES_HUNTER_H
