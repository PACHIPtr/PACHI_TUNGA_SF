#include "StdAfx.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

#include "PythonSkill.h"
#include "PythonNonPlayer.h"
#include "PythonPlayer.h"
#include "PythonCharacterManager.h"
#include "PythonGameEvents.h"

#include "../../Extern/include/cryptopp/aes.h"
#include "../../Extern/include/cryptopp/modes.h"
#include "../../Extern/include/cryptopp/filters.h"

void CPythonGameEvents::SetActivateEvent(bool isActivate, BYTE bEventID)
{
	if (bEventID > EVENT_MAX_NUM)
		return;
		
	m_pkActivateEvents[bEventID] = isActivate;
}

void CPythonGameEvents::SetEventTime(BYTE bEventID, DWORD event_time)
{
	if (bEventID > EVENT_MAX_NUM)
		return;
	
	m_dwEventEndTime[bEventID] = event_time;
}

bool CPythonGameEvents::IsActivateEvent(BYTE bEventID)
{
	return m_pkActivateEvents[bEventID];
}

DWORD CPythonGameEvents::GetEventTime(BYTE bEventID)
{
	return m_dwEventEndTime[bEventID];
}

CPythonGameEvents::CPythonGameEvents()
{
	for (int i = 0; i < EVENT_MAX_NUM; ++i)
	{
		m_pkActivateEvents[i] = false;
		m_dwEventEndTime[i] = 0;
	}
}


CPythonGameEvents::~CPythonGameEvents()
{
}

PyObject * gameEventsIsActivateEvent(PyObject* poSelf, PyObject* poArgs)
{
	int iEventIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iEventIndex))
		return Py_BuildNone();
	
	bool isActivate = CPythonGameEvents::instance().IsActivateEvent(iEventIndex);
	
	return Py_BuildValue("i", isActivate == true ? 1 : 0);
}

PyObject * gameEventsGetEventTime(PyObject* poSelf, PyObject* poArgs)
{
	int iEventIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iEventIndex))
		return Py_BuildNone();
	
	DWORD event_time = CPythonGameEvents::instance().GetEventTime(iEventIndex);
	
	return Py_BuildValue("i", event_time);
}

void initGameEvents()
{
	static PyMethodDef s_methods[] = 
	{
		{"IsActivateEvent",gameEventsIsActivateEvent,METH_VARARGS},
		{"GetEventTime",gameEventsGetEventTime,METH_VARARGS},
		{NULL, NULL, NULL},
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(GAME_EVENTS_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("gameEvents", s_methods);
#endif
	
	/*******************************************************************************************/
	/*** Events ***/
	/*******************************************************************************************/
	PyModule_AddIntConstant(poModule, "EVENT_MOON_DROP",			EVENT_MOON_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_HEXAGON_DROP",			EVENT_HEXAGON_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_CARVING_STONE_DROP",	EVENT_CARVING_STONE_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_RAMADAN",				EVENT_RAMADAN);
	PyModule_AddIntConstant(poModule, "EVENT_XMAS",					EVENT_XMAS);
	PyModule_AddIntConstant(poModule, "EVENT_EASTER",				EVENT_EASTER);
	PyModule_AddIntConstant(poModule, "EVENT_FOOTBALL_DROP",		EVENT_FOOTBALL_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_VALENTINE_DROP",		EVENT_VALENTINE_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_PET_DROP",				EVENT_PET_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_KIDS_DAY_DROP",		EVENT_KIDS_DAY_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_HALLOWEEN",			EVENT_HALLOWEEN);
	PyModule_AddIntConstant(poModule, "EVENT_OLYMPIC_BADGE",		EVENT_OLYMPIC_BADGE);
	PyModule_AddIntConstant(poModule, "EVENT_OKEY_CARD",			EVENT_OKEY_CARD);
	PyModule_AddIntConstant(poModule, "EVENT_MOUNT_DROP",			EVENT_MOUNT_DROP);
	PyModule_AddIntConstant(poModule, "EVENT_ALIGNMENT",			EVENT_ALIGNMENT);
	PyModule_AddIntConstant(poModule, "EVENT_YEAR_ROUND",			EVENT_YEAR_ROUND);
	PyModule_AddIntConstant(poModule, "EVENT_BOSS_HUNTERS",			EVENT_BOSS_HUNTERS);
	PyModule_AddIntConstant(poModule, "EVENT_NEW_FISH",				EVENT_NEW_FISH);
	PyModule_AddIntConstant(poModule, "EVENT_MINING",				EVENT_MINING);
	PyModule_AddIntConstant(poModule, "EVENT_HARVEST_FESTIVAL",		EVENT_HARVEST_FESTIVAL);
	PyModule_AddIntConstant(poModule, "EVENT_MAKE_WONSO",			EVENT_MAKE_WONSO);
	PyModule_AddIntConstant(poModule, "EVENT_CATCH_KING",			EVENT_CATCH_KING);
	PyModule_AddIntConstant(poModule, "EVENT_GOLDEN_RAIN",			EVENT_GOLDEN_RAIN);
	PyModule_AddIntConstant(poModule, "EVENT_SUPER_METIN",			EVENT_SUPER_METIN);
	PyModule_AddIntConstant(poModule, "EVENT_LUCKY_BOX",			EVENT_LUCKY_BOX);
	PyModule_AddIntConstant(poModule, "EVENT_MAX_NUM",				EVENT_MAX_NUM);
	/*******************************************************************************************/
	/*** END ***/
	/*******************************************************************************************/
}