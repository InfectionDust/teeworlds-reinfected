#ifndef GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
#define GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H

#include <generated/protocol.h>

struct SkinInfo
{
	char m_aaSkinPartNames[NUM_SKINPARTS][24];
	int m_aUseCustomColors[NUM_SKINPARTS];
	int m_aSkinPartColors[NUM_SKINPARTS];
};

class CInfClassPlayerClass
{
public:
	virtual ~CInfClassPlayerClass() = default;

	const SkinInfo &GetSkinInfo() const { return m_SkinInfo; }
	virtual void SetupSkin();

	// Events
	virtual void OnCharacterSpawned();

protected:
	explicit CInfClassPlayerClass();

	SkinInfo m_SkinInfo;
};

#endif // GAME_SERVER_INFCLASS_CLASSES_PLAYER_CLASS_H
