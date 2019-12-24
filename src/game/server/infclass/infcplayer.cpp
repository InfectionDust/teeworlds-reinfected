#include "infcplayer.h"

#include "infcgamecontext.h"

static const bool NotADummy = false;

MACRO_ALLOC_POOL_ID_IMPL(CInfClassPlayer, MAX_CLIENTS)

CInfClassPlayer::CInfClassPlayer(CInfClassGameContext *pGameServer, int ClientID, bool AsSpec)
	: CPlayer(pGameServer, ClientID, NotADummy, AsSpec)
{
}
