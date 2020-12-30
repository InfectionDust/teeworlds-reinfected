#include "infcserver.h"

#include <engine/storage.h>

#include "mapconverter.h"

CInfClassServer::CInfClassServer()
{
}

int CInfClassServer::LoadMap(const char *pMapName)
{
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "maps/%s.map", pMapName);

	// check for valid standard map
	if(!m_MapChecker.ReadAndValidateMap(Storage(), aBuf, IStorage::TYPE_ALL))
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mapchecker", "invalid standard map");
		return 0;
	}

	if(!m_pMap->Load(aBuf))
		return 0;

	const char *pClientMapName = aBuf;

	/* INFECTION MODIFICATION START ***************************************/
	//The map format of InfectionClass is different from the vanilla format.
	//We need to convert the map to something that the client can use
	//First, try to find if the client map is already generated
	char aClientMapName[256];
	pClientMapName = aClientMapName;
	{
		CDataFileReader dfServerMap;
		dfServerMap.Open(Storage(), aBuf, IStorage::TYPE_ALL);
		unsigned ServerMapCrc = dfServerMap.Crc();
		dfServerMap.Close();

		str_format(aClientMapName, sizeof(aClientMapName), "clientmaps/%s_%08x/tw07-highres.map", pMapName, ServerMapCrc);

		CMapConverter MapConverter(Storage(), m_pMap, Console());
		if(!MapConverter.Load())
			return 0;

		m_TimeShiftUnit = MapConverter.GetTimeShiftUnit();

		CDataFileReader dfClientMap;
		//The map is already converted
		if(false && dfClientMap.Open(Storage(), aClientMapName, IStorage::TYPE_ALL))
		{
			m_CurrentMapCrc = dfClientMap.Crc();
			m_CurrentMapSha256 = dfClientMap.Sha256();
			dfClientMap.Close();
		}
		//The map must be converted
		else
		{
			char aClientMapDir[256];
			str_format(aClientMapDir, sizeof(aClientMapDir), "clientmaps/%s_%08x", pMapName, ServerMapCrc);

			char aFullPath[512];
			Storage()->GetCompletePath(IStorage::TYPE_SAVE, aClientMapDir, aFullPath, sizeof(aFullPath));
			if(fs_makedir(aFullPath) != 0)
			{
				dbg_msg("infclass", "Can't create the directory '%s'", aClientMapDir);
			}

			if(!MapConverter.CreateMap(aClientMapName))
				return 0;

			CDataFileReader dfGeneratedMap;
			dfGeneratedMap.Open(Storage(), aClientMapName, IStorage::TYPE_ALL);
			m_CurrentMapCrc = dfGeneratedMap.Crc();
			m_CurrentMapSha256 = dfGeneratedMap.Sha256();
			dfGeneratedMap.Close();
		}

		char aBufMsg[128];
		str_format(aBufMsg, sizeof(aBufMsg), "map crc is %08x, generated map crc is %08x", ServerMapCrc, m_CurrentMapCrc);
		Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", aBufMsg);
	}

	// stop recording when we change map
	m_DemoRecorder.Stop();

	// reinit snapshot ids
	m_IDPool.TimeoutIDs();

	// get the sha256 and crc of the map
	// m_CurrentMapSha256 = m_pMap->Sha256();
	// m_CurrentMapCrc = m_pMap->Crc();
	char aSha256[SHA256_MAXSTRSIZE];
	sha256_str(m_CurrentMapSha256, aSha256, sizeof(aSha256));
	char aBufMsg[256];
	str_format(aBufMsg, sizeof(aBufMsg), "%s sha256 is %s", aBuf, aSha256);
	Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", aBufMsg);
	str_format(aBufMsg, sizeof(aBufMsg), "%s crc is %08x", aBuf, m_CurrentMapCrc);
	Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "server", aBufMsg);

	str_copy(m_aCurrentMap, pMapName, sizeof(m_aCurrentMap));

	// load complete map into memory for download
	{
		IOHANDLE File = Storage()->OpenFile(pClientMapName, IOFLAG_READ, IStorage::TYPE_ALL);
		m_CurrentMapSize = (int)io_length(File);
		if(m_pCurrentMapData)
			mem_free(m_pCurrentMapData);
		m_pCurrentMapData = (unsigned char *)mem_alloc(m_CurrentMapSize, 1);
		io_read(File, m_pCurrentMapData, m_CurrentMapSize);
		io_close(File);
	}
	return 1;
}

#define CreateServer CreateInfcServer
extern CServer *CreateServer() { return new CInfClassServer(); }
