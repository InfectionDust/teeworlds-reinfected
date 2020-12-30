#ifndef GAME_SERVER_INFCLASS_SERVER_H
#define GAME_SERVER_INFCLASS_SERVER_H

#include <engine/console.h>
#include <engine/map.h>
#include <engine/masterserver.h>
#include <engine/server/register.h>
#include <engine/shared/demo.h>
#include <engine/shared/econ.h>
#include <engine/shared/mapchecker.h>
#include <engine/shared/netban.h>
#include <engine/shared/network.h>
#include <engine/shared/protocol.h>
#include <engine/shared/snapshot.h>

#include <engine/server/server.h>

class CInfClassServer : public CServer
{
public:
	CInfClassServer();
	int LoadMap(const char *pMapName) override;

	int GetTimeShiftUnit() const { return m_TimeShiftUnit; }

protected:
	int m_TimeShiftUnit;
};

#endif // GAME_SERVER_INFCLASS_MAP_H
