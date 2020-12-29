#ifndef GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
#define GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H

#include <game/server/entities/character.h>

class CInfClassCharacter : public CCharacter
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassCharacter(CGameWorld *pWorld);

	virtual void OnCharacterSpawn();

	void SetActiveWeapon(int Weapon);
	void SetLastWeapon(int Weapon);
	void TakeAllWeapons();
};

#endif // GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
