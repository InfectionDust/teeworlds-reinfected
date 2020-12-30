#ifndef GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
#define GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H

#include <base/vmath.h>
#include <generated/protocol.h>

class CGameContext;
class CGameWorld;
class CInfClassCharacter;
class CInfClassGameContext;
class CInfClassPlayer;
class IServer;

struct SkinInfo
{
	char m_aaSkinPartNames[NUM_SKINPARTS][24];
	int m_aUseCustomColors[NUM_SKINPARTS];
	int m_aSkinPartColors[NUM_SKINPARTS];
};

enum class JumpType {
	Air,
	Ground,
};

class CInfClassPlayerClass
{
public:
	virtual ~CInfClassPlayerClass() = default;

	void SetCharacter(CInfClassCharacter *character);

	const SkinInfo &GetSkinInfo() const { return m_SkinInfo; }
	virtual void SetupSkin();

	// Events
	virtual void OnCharacterSpawned();
	virtual void OnGrounded() { }
	virtual void OnJumped(JumpType jumpType);
	virtual void OnWeaponFired(int Weapon);

	virtual void OnHammerFired();
	virtual void OnGunFired();
	virtual void OnShotgunFired();
	virtual void OnGrenadeFired();
	virtual void OnLaserFired();
	virtual void OnNinjaFired();

protected:
	explicit CInfClassPlayerClass();
	CInfClassGameContext *GameContext() const;
	CGameContext *GameServer() const;
	CGameWorld *GameWorld() const;
	IServer *Server();
	CInfClassPlayer *GetPlayer();
	int GetCID();
	vec2 GetPos() const;
	vec2 GetDirection() const;
	float GetProximityRadius() const;

	SkinInfo m_SkinInfo;
	CInfClassCharacter *m_pCharacter = nullptr;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
