#include "infcserver.h"

CInfClassServer::CInfClassServer()
{
}

#define CreateServer CreateInfcServer
extern CServer *CreateServer() { return new CInfClassServer(); }
