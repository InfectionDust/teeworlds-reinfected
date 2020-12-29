#ifndef GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
#define GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H

#include <generated/protocol.h>

class CInfClassCharacter;

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

protected:
	explicit CInfClassPlayerClass();

	SkinInfo m_SkinInfo;
	CInfClassCharacter *m_pCharacter = nullptr;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
