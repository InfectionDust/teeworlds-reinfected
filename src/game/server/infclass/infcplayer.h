#ifndef GAME_SERVER_INFCLASS_PLAYER_H
#define GAME_SERVER_INFCLASS_PLAYER_H

#include <game/gamecore.h>

class CCharacter;
class CGameContext;
class CInfClassGameContext;
class IServer;

// We actually have to include player.h after all this stuff above.
#include "../player.h"

class CInfClassPlayer : public CPlayer
{
	MACRO_ALLOC_POOL_ID()

public:
	CInfClassPlayer(CInfClassGameContext *pGameServer, int ClientID, bool AsSpec = false);

protected:
	CInfClassGameContext *m_pInfcGameContext = nullptr;
};

#endif // GAME_SERVER_INFCLASS_PLAYER_H
