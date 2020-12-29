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

	void OnInit() override;

	void OnMessage(int MsgID, CUnpacker *pUnpacker, int ClientID) override;

	void OnClientConnected(int ClientID, bool AsSpec) override;

	void AnnounceSkinChange(int ClientID);

	CInfClassPlayerClass *CreateInfClass(int ClassId);

protected:
	using InfPlayerClassConstructor = CInfClassPlayerClass *();

	template <typename C>
	int RegisterInfClassClass();

	CGameControllerInfClass *m_pInfcGameController = nullptr;
	std::vector<InfPlayerClassConstructor*> m_ClassConstructors;
	std::map<int, const char *> m_ClassIdToName;
};

#endif // GAME_SERVER_INFCLASS_CONTEXT_H
