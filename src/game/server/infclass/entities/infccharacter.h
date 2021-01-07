#ifndef GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
#define GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H

#include <game/server/entities/character.h>

class CInfClassGameContext;
class CInfClassPlayerClass;

class CInfClassCharacter : public CCharacter
{
	MACRO_ALLOC_HEAP()
public:
	static int EntityId;

	CInfClassCharacter(CInfClassGameContext *pContext);

	void Tick() override;
	void Snap(int SnappingClient) override;
	void FireWeapon() override;

	virtual void OnCharacterSpawn();

	vec2 GetDirection() const;

	bool IncreaseOverallHp(int Amount);
	void GiveVelocity(const vec2 &ExtraVelocity);
	void SaturateVelocity(vec2 Force, float MaxSpeed);

	void EnableJump();
	void SetActiveWeapon(int Weapon);
	void SetLastWeapon(int Weapon);
	void TakeAllWeapons();
	void SetReloadTimer(int ReloadTimer);
	void SetAttackAnimationEnabled(bool Enabled);

	CInfClassPlayerClass *GetClass() { return m_pClass; }
	void SetClass(CInfClassPlayerClass *pClass);

	CInfClassGameContext *GameContext() const { return m_pContext; }

protected:
	CInfClassGameContext *m_pContext = nullptr;
	CInfClassPlayerClass *m_pClass = nullptr;
	bool m_Grounded = false;
	bool m_AttackAnimationEnabled = false;
};

#endif // GAME_SERVER_INFCLASS_ENTITIES_CHARACTER_H
