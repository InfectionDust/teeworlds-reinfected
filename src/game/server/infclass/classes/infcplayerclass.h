#ifndef GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
#define GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H

#include <base/vmath.h>
#include <generated/protocol.h>

class CConfig;
class CGameContext;
class CGameWorld;
class CInfCExplosionContext;
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
	virtual const char *GetClassName() = 0;
	virtual const char *GetClassDisplayName() = 0;

	virtual bool IsHuman() const = 0;
	bool IsZombie() const;

	virtual void SetupSkin();

	void Poison(int Count, int From);

	// Events
	virtual void Tick();
	virtual void OnCharacterSpawned();
	virtual void OnGrounded() { }
	virtual void OnJumped(JumpType jumpType);
	virtual bool IsFullAutoFireWeapon(int Weapon) const;
	virtual void OnWeaponFired(int Weapon);

	virtual void OnHammerFired();
	virtual void OnGunFired();
	virtual void OnShotgunFired();
	virtual void OnGrenadeFired();
	virtual void OnLaserFired();
	virtual void OnNinjaFired();

	virtual void OnSlimeEffect(int Owner) = 0;
	virtual void OnExplosion(const CInfCExplosionContext &context, bool *pHit) = 0;

	struct HammerFireContext
	{
		vec2 ProjStartPos;
		vec2 Direction;
		float Radius = 0;
	};

	virtual void OnHammerHitCharacter(CInfClassCharacter *pTarget, HammerFireContext *pHitContext);

	CInfClassGameContext *GameContext() const;
	CGameContext *GameServer() const;
	CGameWorld *GameWorld() const;
	CConfig *Config();
	IServer *Server();
	CInfClassPlayer *GetPlayer();
	int GetCID();
	vec2 GetPos() const;
	vec2 GetDirection() const;
	float GetProximityRadius() const;

protected:
	explicit CInfClassPlayerClass();
	SkinInfo m_SkinInfo;
	CInfClassCharacter *m_pCharacter = nullptr;

	int m_Poison = 0;
	int m_PoisonTick = 0;
	int m_PoisonFrom = 0;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
