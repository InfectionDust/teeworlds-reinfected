#ifndef GAME_SERVER_INFCLASS_CONTEXT_H
#define GAME_SERVER_INFCLASS_CONTEXT_H

#include "../gamecontext.h"

class CGameControllerInfClass;

class CInfClassGameContext : public CGameContext
{
public:
	CInfClassGameContext();

	CGameControllerInfClass *GetController() const { return m_pInfcGameController; }

	void OnInit() override;

	void OnClientConnected(int ClientID, bool AsSpec) override;

	void AnnounceSkinChange(int ClientID);

protected:
	CGameControllerInfClass *m_pInfcGameController = nullptr;
};

#endif // GAME_SERVER_INFCLASS_CONTEXT_H
