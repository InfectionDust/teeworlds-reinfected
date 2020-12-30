#ifndef GAME_SERVER_INFCLASS_CONTEXT_H
#define GAME_SERVER_INFCLASS_CONTEXT_H

#include "../gamecontext.h"

#include <vector>
#include <map>

class CGameControllerInfClass;
class CInfClassPlayer;
class CInfClassPlayerClass;

class CInfClassGameContext : public CGameContext
{
public:
	CInfClassGameContext();

	CGameControllerInfClass *GetController() const { return m_pInfcGameController; }
	CGameWorld *GameWorld();

	void RegisterTypes();
	void OnInit() override;
	void OnConsoleInit() override;

	void OnMessage(int MsgID, CUnpacker *pUnpacker, int ClientID) override;

	void OnClientConnected(int ClientID, bool AsSpec) override;

	void AnnounceSkinChange(int ClientID);

	CInfClassPlayer *GetPlayer(int ClientID);
	CInfClassPlayerClass *CreateInfClass(int ClassId);

protected:
	using InfPlayerClassConstructor = CInfClassPlayerClass *();

	template <typename C>
	int RegisterInfClassClass();

	static void ConSetClass(IConsole::IResult *pResult, void *pUserData);

	CGameControllerInfClass *m_pInfcGameController = nullptr;
	std::vector<InfPlayerClassConstructor*> m_ClassConstructors;
	std::map<int, const char *> m_ClassIdToName;
};

#endif // GAME_SERVER_INFCLASS_CONTEXT_H
