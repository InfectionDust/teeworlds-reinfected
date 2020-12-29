#ifndef GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
#define GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H

#include <game/server/entities/character.h>

class CInfClassPlayerClass;

class CInfClassCharacter : public CCharacter
{
	MACRO_ALLOC_HEAP()
public:
	CInfClassCharacter(CGameWorld *pWorld);

	void Tick() override;

	virtual void OnCharacterSpawn();

	void EnableJump();
	void SetActiveWeapon(int Weapon);
	void SetLastWeapon(int Weapon);
	void TakeAllWeapons();

	void SetClass(CInfClassPlayerClass *pClass);

protected:
	CInfClassPlayerClass *m_pClass;
	bool m_Grounded = false;
};

#endif // GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
