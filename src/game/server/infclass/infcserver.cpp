#include "infcserver.h"

CInfClassServer::CInfClassServer()
{
}

int CInfClassServer::GetActivePlayerCount()
{
	int PlayerCount = 0;
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_aClients[i].m_State == CClient::STATE_INGAME)
		{
			if (!GameServer()->IsClientSpectator(i))
				PlayerCount++;
		}
	}
	return PlayerCount;
}

#define CreateServer CreateInfcServer
extern CServer *CreateServer() { return new CInfClassServer(); }
