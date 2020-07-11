/*********************************************************************
* title_name		: Combat Zone (Official Webzen 16.4)
* date_created		: 2017.05.21
* filename			: PythonCombatZone.cpp
* author			: VegaS
* version_actual	: Version 0.2.0
*/
#include "StdAfx.h"
#include "PythonApplication.h"
#include "Packet.h"
#include "PythonNetworkStream.h"
#include "PythonCombatZone.h"
#include "InstanceBase.h"
#include "AbstractPlayer.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonCombatZone* CPythonCombatZone::curInstance = nullptr; 
CPythonCombatZone::CPythonCombatZone(void)
{}
CPythonCombatZone::~CPythonCombatZone(void)
{}

DWORD m_pDataDays[7][8];
bool bIsRunning = false;

CPythonCombatZone* CPythonCombatZone::instance()
{
	if (!curInstance)
		curInstance = new CPythonCombatZone();
	return curInstance;
}

void CPythonCombatZone::SendDataDays(DWORD m_pSendedData[7][8], bool bIsOnline)
{
	bIsRunning = bIsOnline;
	memcpy(&m_pDataDays, m_pSendedData, sizeof(m_pDataDays));
}

void CPythonCombatZone::Initialize(TPacketGCCombatZoneRankingData p)
{
	m_vecRankingData.clear();	

	for(int i = 0; i < COMBAT_ZONE_MAX_ROWS_RANKING; i++)
	{
		TPacketGCCombatZoneRanking vecs = p.rankingData[i];
		m_vecRankingData.push_back(vecs);
	}
}

TPacketGCCombatZoneRanking CPythonCombatZone::Request(int pid)
{
	if (m_vecRankingData.size() < pid)
	{
		TPacketGCCombatZoneRanking p;
		p.rank = COMBAT_ZONE_EMPTY_DATA;	
		strncpy(p.name, "", sizeof(p.name));
		p.empire = COMBAT_ZONE_EMPTY_DATA;	
		p.points = COMBAT_ZONE_EMPTY_DATA;	
		return p;
	}
	return m_vecRankingData[pid];
}

PyObject * pyObjectGetRankings(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* row = PyTuple_New(4);
	int pid;

	if (!PyTuple_GetInteger(poArgs, 0, &pid))
		return Py_BuildException();
	
	TPacketGCCombatZoneRanking p = CPythonCombatZone::instance()->Request(pid);

	PyTuple_SetItem(row, 0, Py_BuildValue("i", p.rank));
	PyTuple_SetItem(row, 1, Py_BuildValue("s", p.name));
	PyTuple_SetItem(row, 2, Py_BuildValue("i", p.empire));
	PyTuple_SetItem(row, 3, Py_BuildValue("i", p.points));
	return row;
}

PyObject* pyObjectSendCombatZoneRequestActionPacket(PyObject* poSelf, PyObject* poArgs)
{
	int subHeader;
	int value;

	if (!PyTuple_GetInteger(poArgs, 0, &subHeader))
		return Py_BuildException();
	
	if (!PyTuple_GetInteger(poArgs, 1, &value))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCombatZoneRequestActionPacket(subHeader, value);
	return Py_BuildNone();
}

PyObject * pyObjectGetDay(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* row = PyTuple_New(4);
	int iDay;

	if (!PyTuple_GetInteger(poArgs, 0, &iDay))
		return Py_BuildException();

	PyTuple_SetItem(row, 0, Py_BuildValue("i", m_pDataDays[iDay][0]));
	PyTuple_SetItem(row, 1, Py_BuildValue("i", m_pDataDays[iDay][1]));
	PyTuple_SetItem(row, 2, Py_BuildValue("i", m_pDataDays[iDay][2]));
	PyTuple_SetItem(row, 3, Py_BuildValue("i", m_pDataDays[iDay][3]));
	return row;
}

PyObject * pyObjectIsRunning(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("b", bIsRunning);
}

void initCombatZoneSystem()
{
	static PyMethodDef s_methods[] = {
		{ "IsRunning",								pyObjectIsRunning,								METH_VARARGS },
		{ "GetRankings",							pyObjectGetRankings,							METH_VARARGS },
		{ "GetDay",									pyObjectGetDay,									METH_VARARGS },
		{ "SendCombatZoneRequestActionPacket",		pyObjectSendCombatZoneRequestActionPacket,		METH_VARARGS },
		{ NULL,						NULL,							NULL }
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(COMBATZONE_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("combatzone", s_methods);
#endif

	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_MAX_ROWS_RANKING", COMBAT_ZONE_MAX_ROWS_RANKING);
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_EMPTY_DATA",	COMBAT_ZONE_EMPTY_DATA);
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ACTION_LEAVE",	COMBAT_ZONE_ACTION_LEAVE);	
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ACTION_OPEN_RANKING",	COMBAT_ZONE_ACTION_OPEN_RANKING);
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ACTION_CHANGE_PAGE_RANKING",	COMBAT_ZONE_ACTION_CHANGE_PAGE_RANKING);
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_TYPE_RANKING_WEEKLY",	COMBAT_ZONE_TYPE_RANKING_WEEKLY);
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_TYPE_RANKING_ALL",	COMBAT_ZONE_TYPE_RANKING_ALL);	
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ACTION_PARTICIPATE",	COMBAT_ZONE_ACTION_PARTICIPATE);	
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ACTION_REQUEST_POTION",	COMBAT_ZONE_ACTION_REQUEST_POTION);			
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ICON_SLOT",	27125);	
	PyModule_AddIntConstant(poModule, "COMBAT_ZONE_ICON_SLOT_COUNT",	3);	
}