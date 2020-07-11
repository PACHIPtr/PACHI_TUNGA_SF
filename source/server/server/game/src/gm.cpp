#include "stdafx.h"
#include "constants.h"
#include "gm.h"
#include "locale_service.h"

extern int test_server;

//ADMIN_MANAGER
typedef struct
{
	tAdminInfo Info;
	std::set<std::string>* pset_Host;
} tGM;

std::set<std::string> g_set_Host;
std::map<std::string, tGM> g_map_GM;

void gm_new_clear()
{
	g_set_Host.clear();
	g_map_GM.clear();
}

void gm_new_insert(const tAdminInfo& rAdminInfo)
{
	tGM t;

	if (strlen(rAdminInfo.m_szContactIP) == 0)
	{
		t.pset_Host = &g_set_Host;
		sys_log(0, "GM Use ContactIP");
	}
	else
	{
		t.pset_Host = nullptr;
		sys_log(0, "GM Use Default Host List");
	}

	memcpy(&t.Info, &rAdminInfo, sizeof(rAdminInfo));

	g_map_GM[rAdminInfo.m_szName] = t;
}

void gm_new_host_inert(const char* host)
{
	g_set_Host.insert(host);
	sys_log(0, "InsertGMHost(ip:%s)", host);
}

BYTE gm_new_get_level(const char* name, const char* host, const char* account)
{
	if (test_server) return GM_IMPLEMENTOR;

	std::map<std::string, tGM >::iterator it = g_map_GM.find(name);

	if (g_map_GM.end() == it)
		return GM_PLAYER;

	// GERMAN_GM_NOT_CHECK_HOST
	if (account)
	{
		if (strcmp(it->second.Info.m_szAccount, account) != 0)
		{
			sys_log(0, "GM_NEW_GET_LEVEL : BAD ACCOUNT [ACCOUNT:%s/%s", it->second.Info.m_szAccount, account);
			return GM_PLAYER;
		}

		return it->second.Info.m_Authority;
	}
	return GM_PLAYER;
}

//END_ADMIN_MANAGER
BYTE gm_get_level(const char* name, const char* host, const char* account)
{
	return gm_new_get_level(name, host, account);
}