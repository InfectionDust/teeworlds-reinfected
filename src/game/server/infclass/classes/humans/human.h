#ifndef GAME_SERVER_INFCLASS_CLASSES_HUMAN_H
#define GAME_SERVER_INFCLASS_CLASSES_HUMAN_H

#include "../infcplayerclass.h"

#include <game/server/alloc.h>

class CInfClassHuman : public CInfClassPlayerClass
{
public:
	explicit CInfClassHuman();

	bool IsHuman() const final { return true; }
};

#endif // GAME_SERVER_INFCLASS_CLASSES_HUMAN_H
