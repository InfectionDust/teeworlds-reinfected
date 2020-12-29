#ifndef GAME_SERVER_INFCLASS_PLAYER_H
#define GAME_SERVER_INFCLASS_PLAYER_H

#include <game/gamecore.h>

class CCharacter;
class CGameContext;
class CInfClassCharacter;
class CInfClassGameContext;
class CInfClassPlayerClass;
class IServer;

// We actually have to include player.h after all this stuff above.
#include "../player.h"

class CInfClassPlayer : public CPlayer
{
	MACRO_ALLOC_POOL_ID()

public:
	CInfClassPlayer(CInfClassGameContext *pGameServer, int ClientID, bool AsSpec = false);

	void TryRespawn() override;
	void SetCharacterClass(int ClassId);
	void SetCharacterClass(CInfClassPlayerClass *pClass);

protected:
	CInfClassGameContext *m_pInfcGameContext = nullptr;
	CInfClassCharacter *m_pInfcCharacter = nullptr;
	CInfClassPlayerClass *m_pInfcPlayerClass = nullptr;
};

#endif // GAME_SERVER_INFCLASS_PLAYER_H
