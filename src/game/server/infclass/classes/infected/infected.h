#ifndef GAME_SERVER_INFCLASS_CLASSES_INFECTED_H
#define GAME_SERVER_INFCLASS_CLASSES_INFECTED_H

#include "../infcplayerclass.h"

#include <game/server/alloc.h>

class CInfClassInfected : public CInfClassPlayerClass
{
public:
	CInfClassInfected();

	void OnCharacterSpawned() override;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_INFECTED_H
