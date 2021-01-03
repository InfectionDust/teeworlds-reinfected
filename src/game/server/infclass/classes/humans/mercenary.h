#ifndef GAME_SERVER_INFCLASS_CLASSES_MERCENARY_H
#define GAME_SERVER_INFCLASS_CLASSES_MERCENARY_H

#include "human.h"

class CInfClassMercenary : public CInfClassHuman
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassMercenary();

	static const char *ClassName() { return "mercenary"; }
	const char *GetClassName() override;
	const char *GetClassDisplayName() override;

	void OnCharacterSpawned() override;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_MERCENARY_H
