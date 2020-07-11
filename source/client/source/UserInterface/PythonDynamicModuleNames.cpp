#include "StdAfx.h"
#include "PythonDynamicModuleNames.h"
#include "../EterPack/EterPackManager.h"
/*
import net  >  import pythonApi\nnet = __import__(pythonApi.GetModuleName("net"))
import chr  >  import pythonApi\nchr = __import__(pythonApi.GetModuleName("chr"))
import chrmgr  >  import pythonApi\nchrmgr = __import__(pythonApi.GetModuleName("chrmgr"))
import app  >  import pythonApi\napp = __import__(pythonApi.GetModuleName("app"))
import pack  >  import pythonApi\npack = __import__(pythonApi.GetModuleName("pack"))
import player  >  import pythonApi\nplayer = __import__(pythonApi.GetModuleName("player"))
*/

//#define TEST_MODE _DEBUG
#pragma optimize("", off )
#ifdef TEST_MODE
std::string szModuleArrayOld[] = { "net", "chr", "chrmgr", "app", "pack", "player", "background", "changelook", "chat", "combatzone", "effect", "event", "exchange", "gameEvents", "guild", "ime", "item", "messenger", "minimap", "nonplayer", "privateShopSearch", "profiler", "quest", "safebox", "sash", "shop", "skill", "skillpet", "systemSettings", "textTail", "aura", "feature" };
#endif
std::string szModuleArray[MODULE_MAX] = {};


__forceinline const char* GetCallerReference_FileName() {
	PyThreadState* pThreadState = PyThreadState_Get();
	PyFrameObject* pFrameObject = pThreadState->frame;

	char const* c_szCallerFileName = PyString_AsString(pFrameObject->f_code->co_filename);
	return c_szCallerFileName;
}
#ifdef TEST_MODE
const char* GetCallerReference_FunctionName() {
	PyThreadState* pThreadState = PyThreadState_Get();
	PyFrameObject* pFrameObject = pThreadState->frame;

	char const* c_szCallerFunctionName = PyString_AsString(pFrameObject->f_code->co_name);
	return c_szCallerFunctionName;
}
#endif

inline std::string randomStrGen(int length) {
	CHAR __alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0x0 }; // abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890
	static std::string charset = __alphabet;
	std::string result;
	result.resize(length);

	for (int i = 0; i < length; i++)
		result[i] = charset[rand() % charset.length()];

	return result;
}

inline bool IsUsedName(std::string szName)
{
	for (int i = 0; i < MODULE_MAX; ++i)
		if (szModuleArray[i].empty() == false)
			if (szModuleArray[i] == szName)
				return true;
	return false;
}

__forceinline std::string CreateModuleName()
{
	std::string sztmpName;
	do { sztmpName = randomStrGen(32); } while (IsUsedName(sztmpName));
	return sztmpName;
}


__forceinline void CreateModuleNames()
{
	for (int i = 0; i < MODULE_MAX; ++i) {
		if (szModuleArray[i].empty()) {
			szModuleArray[i] = CreateModuleName();
#ifdef TEST_MODE
			TraceError("Dynamic module name created. %s -> %s", szModuleArrayOld[i].c_str(), szModuleArray[i].c_str());
#endif
		}
	}
}

std::string GetModuleName(int iModuleCode) {
	return szModuleArray[iModuleCode];
}
#pragma optimize("", on )

// Python api
PyObject * pythonApiGetModuleName(PyObject * poSelf, PyObject * poArgs)
{
	CHAR __net[] = { 'n', 'e', 't', 0x0 }; // net
	CHAR __chr[] = { 'c', 'h', 'r', 0x0 }; // chr
	CHAR __chrmgr[] = { 'c', 'h', 'r', 'm', 'g', 'r', 0x0 }; // chrmgr
	CHAR __app[] = { 'a', 'p', 'p', 0x0 }; // app
	CHAR __pack[] = { 'p', 'a', 'c', 'k', 0x0 }; // pack
	CHAR __player[] = { 'p', 'l', 'a', 'y', 'e', 'r', 0x0 }; // player
	CHAR __background[] = { 'b', 'a', 'c', 'k', 'g', 'r', 'o', 'u', 'n', 'd', 0x0 }; // background
	CHAR __changelook[] = { 'c', 'h', 'a', 'n', 'g', 'e', 'l', 'o', 'o', 'k', 0x0 }; // changelook
	CHAR __chat[] = { 'c', 'h', 'a', 't', 0x0 }; // chat
	CHAR __combatzone[] = { 'c', 'o', 'm', 'b', 'a', 't', 'z', 'o', 'n', 'e', 0x0 }; // combatzone
	CHAR __effect[] = { 'e', 'f', 'f', 'e', 'c', 't', 0x0 }; // effect
	CHAR __eventmanager[] = { 'e', 'v', 'e', 'n', 't', 0x0 }; // event
	CHAR __exchange[] = { 'e', 'x', 'c', 'h', 'a', 'n', 'g', 'e', 0x0 }; // exchange
	CHAR __gamevents[] = { 'g', 'a', 'm', 'e', 'E', 'v', 'e', 'n', 't', 's', 0x0 }; // gameevents
	CHAR __guild[] = { 'g', 'u', 'i', 'l', 'd', 0x0 }; // guild
	CHAR __ime[] = { 'i', 'm', 'e', 0x0 }; // ime
	CHAR __item[] = { 'i', 't', 'e', 'm', 0x0 }; // item
	CHAR __messenger[] = { 'm', 'e', 's', 's', 'e', 'n', 'g', 'e', 'r', 0x0 }; // messenger
	CHAR __minimap[] = { 'm', 'i', 'n', 'i', 'M', 'a', 'p', 0x0 }; // minimap
	CHAR __nonplayer[] = { 'n', 'o', 'n', 'p', 'l', 'a', 'y', 'e', 'r', 0x0 };
	CHAR __shopsearch[] = { 'p', 'r', 'i', 'v', 'a', 't', 'e', 'S', 'h', 'o', 'p', 'S', 'e', 'a', 'r', 'c', 'h', 0x0};
	CHAR __profiler[] = { 'p', 'r', 'o', 'f', 'i', 'l', 'e', 'r', 0x0};
	CHAR __quest[] = { 'q', 'u', 'e', 's', 't', 0x0};
	CHAR __safebox[] = { 's', 'a', 'f', 'e', 'b', 'o', 'x', 0x0};
	CHAR __sash[] = { 's', 'a', 's', 'h', 0x0};
	CHAR __shop[] = { 's', 'h', 'o', 'p', 0x0};
	CHAR __skill[] = { 's', 'k', 'i', 'l', 'l', 0x0};
	CHAR __skillpet[] = { 's', 'k', 'i', 'l', 'l', 'p', 'e', 't', 0x0};
	CHAR __systemModule[] = { 's', 'y', 's', 't', 'e', 'm', 'S', 'e', 't', 't', 'i', 'n', 'g', 0x0};
	CHAR __textTail[] = { 't', 'e', 'x', 't', 'T', 'a', 'i', 'l', 0x0};
	CHAR __aura[] = { 'a', 'u', 'r', 'a', 0x0};
	CHAR __features[] = { 'f', 'e', 'a', 't', 'u', 'r', 'e', 's', 0x0};
	CHAR __s[] = { 's', 0x0 }; // s

	char * szOldName;
	if (!PyTuple_GetString(poArgs, 0, &szOldName))
		return Py_BuildException();

#ifndef PYTHON_DYNAMIC_MODULE_NAME
	return Py_BuildValue(__s, szOldName);
#else
	CHAR __locale[] = { 'l', 'o', 'c', 'a', 'l', 'e', '/', 0x0 }; // locale/
	CHAR __uiscript[] = { 'u', 'i', 's', 'c', 'r', 'i', 'p', 't', '/', 0x0 }; // uiscript/
	const char* c_szCallerReference = GetCallerReference_FileName();
	if (!CEterPackManager::Instance().isExistInPack(c_szCallerReference) &&
		!strstr(c_szCallerReference, __locale) &&
		!strstr(c_szCallerReference, __uiscript))
	{
#ifdef TEST_MODE
		TraceError("Error! Python module import request from unknown target! %s:%s", GetCallerReference_FileName(), GetCallerReference_FunctionName());
#endif
		return Py_BuildValue(__s, szOldName);
	}

	if (!strcmp(szOldName, __net))
		return Py_BuildValue(__s, szModuleArray[NET_MODULE].c_str());
	else if (!strcmp(szOldName, __chr))
		return Py_BuildValue(__s, szModuleArray[CHR_MODULE].c_str());
	else if (!strcmp(szOldName, __chrmgr))
		return Py_BuildValue(__s, szModuleArray[CHRMGR_MODULE].c_str());
	else if (!strcmp(szOldName, __app))
		return Py_BuildValue(__s, szModuleArray[APP_MODULE].c_str());
	else if (!strcmp(szOldName, __pack))
		return Py_BuildValue(__s, szModuleArray[PACK_MODULE].c_str());
	else if (!strcmp(szOldName, __player))
		return Py_BuildValue(__s, szModuleArray[PLAYER_MODULE].c_str());
	else if (!strcmp(szOldName, __background))
		return Py_BuildValue(__s, szModuleArray[BACKGROUND_MODULE].c_str());
	else if (!strcmp(szOldName, __changelook))
		return Py_BuildValue(__s, szModuleArray[CHANGELOOK_MODULE].c_str());
	else if (!strcmp(szOldName, __chat))
		return Py_BuildValue(__s, szModuleArray[CHAT_MODULE].c_str());
	else if (!strcmp(szOldName, __combatzone))
		return Py_BuildValue(__s, szModuleArray[COMBATZONE_MODULE].c_str());
	else if (!strcmp(szOldName, __effect))
		return Py_BuildValue(__s, szModuleArray[EFFECT_MODULE].c_str());
	else if (!strcmp(szOldName, __eventmanager))
		return Py_BuildValue(__s, szModuleArray[EVENT_MANAGER_MODULE].c_str());
	else if (!strcmp(szOldName, __exchange))
		return Py_BuildValue(__s, szModuleArray[EXCHANGE_MODULE].c_str());
	else if (!strcmp(szOldName, __gamevents))
		return Py_BuildValue(__s, szModuleArray[GAME_EVENTS_MODULE].c_str());
	else if (!strcmp(szOldName, __guild))
		return Py_BuildValue(__s, szModuleArray[GUILD_MODULE].c_str());
	else if (!strcmp(szOldName, __ime))
		return Py_BuildValue(__s, szModuleArray[IME_MODULE].c_str());
	else if (!strcmp(szOldName, __item))
		return Py_BuildValue(__s, szModuleArray[ITEM_MODULE].c_str());
	else if (!strcmp(szOldName, __messenger))
		return Py_BuildValue(__s, szModuleArray[MESSENGER_MODULE].c_str());
	else if (!strcmp(szOldName, __minimap))
		return Py_BuildValue(__s, szModuleArray[MINIMAP_MODULE].c_str());
	else if (!strcmp(szOldName, __nonplayer))
		return Py_BuildValue(__s, szModuleArray[NON_PLAYER_MODULE].c_str());
	else if (!strcmp(szOldName, __shopsearch))
		return Py_BuildValue(__s, szModuleArray[SHOP_SEARCH_MODULE].c_str());
	else if (!strcmp(szOldName, __profiler))
		return Py_BuildValue(__s, szModuleArray[PROFILER_MODULE].c_str());
	else if (!strcmp(szOldName, __quest))
		return Py_BuildValue(__s, szModuleArray[QUEST_MODULE].c_str());
	else if (!strcmp(szOldName, __safebox))
		return Py_BuildValue(__s, szModuleArray[SAFEBOX_MODULE].c_str());
	else if (!strcmp(szOldName, __sash))
		return Py_BuildValue(__s, szModuleArray[SASH_MODULE].c_str());
	else if (!strcmp(szOldName, __shop))
		return Py_BuildValue(__s, szModuleArray[SHOP_MODULE].c_str());
	else if (!strcmp(szOldName, __skill))
		return Py_BuildValue(__s, szModuleArray[SKILL_MODULE].c_str());
	else if (!strcmp(szOldName, __skillpet))
		return Py_BuildValue(__s, szModuleArray[SKILL_PET_MODULE].c_str());
	else if (!strcmp(szOldName, __systemModule))
		return Py_BuildValue(__s, szModuleArray[SYSTEM_MODULE].c_str());
	else if (!strcmp(szOldName, __textTail))
		return Py_BuildValue(__s, szModuleArray[TEXT_TAIL_MODULE].c_str());
	else if (!strcmp(szOldName, __aura))
		return Py_BuildValue(__s, szModuleArray[AURA_MODULE].c_str());
	else if (!strcmp(szOldName, __features))
		return Py_BuildValue(__s, szModuleArray[FEATURES_MODULE].c_str());
#ifdef TEST_MODE
	else
		TraceError("Error! Unknown module name request as dynamic! (%s) - From-> %s:%s", szOldName, GetCallerReference_FileName(), GetCallerReference_FunctionName());
#endif

#endif // PYTHON_DYNAMIC_MODULE_NAME

	return Py_BuildNone();
}

// Initialize Python API
void initPythonApi()
{
	CHAR __GetModuleName[] = { 'G', 'e', 't', 'M', 'o', 'd', 'u', 'l', 'e', 'N', 'a', 'm', 'e', 0x0 }; // GetModuleName
	//CHAR __pythonApi[] = { 'p', 'y', 't', 'h', 'o', 'n', 'A', 'p', 'i', 0x0 }; // pythonApi
	
	CHAR __pythonApi[] = { 'R', '3', 'G', 'h', 'C', 'Q', 'R', 'x', '2', 'v', 'r', '6', 'g', 'T', 'K', 'X', 0x0 };
		
	//uvvY2sBNvVcbJz4jvDr6fSWYdmqv9NPQ

	static PyMethodDef s_methods[] =
	{
		{ __GetModuleName,				pythonApiGetModuleName,				METH_VARARGS },
		{ NULL,							NULL,								NULL },
	};

	PyObject * poModule = Py_InitModule(__pythonApi, s_methods);

	CreateModuleNames();
}
