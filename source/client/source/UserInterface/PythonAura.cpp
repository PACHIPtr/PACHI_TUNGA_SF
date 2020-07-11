#include "StdAfx.h"
#include "Packet.h"

#include "PythonAura.h"
#include "PythonNetworkStream.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonAura::CPythonAura()
{
	Clear();
}

CPythonAura::~CPythonAura()
{
	Clear();
}

void CPythonAura::Clear()
{
	dwPrice = 0;
	
	ZeroMemory(&m_vAuraResult, sizeof(m_vAuraResult));
	
	m_vAuraMaterials.clear();
	m_vAuraMaterials.resize(AURA_WINDOW_MAX_MATERIALS);
	for (BYTE bPos = 0; bPos < m_vAuraMaterials.size(); ++bPos)
	{
		TAuraMaterial tMaterial;
		tMaterial.bHere = 0;
		tMaterial.wCell = 0;
		m_vAuraMaterials[bPos] = tMaterial;
	}
}

void CPythonAura::AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
		return;
	
	if (bPos == 0)
		dwPrice = dwRefPrice;
	
	TAuraMaterial tMaterial;
	tMaterial.bHere = 1;
	tMaterial.wCell = tPos.cell;
	m_vAuraMaterials[bPos] = tMaterial;
}

void CPythonAura::AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs)
{
	TAuraResult tResult;
	tResult.dwItemVnum = dwItemVnum;
	tResult.dwMinAbs = dwMinAbs;
	tResult.dwMaxAbs = dwMaxAbs;
	m_vAuraResult = tResult;
}

void CPythonAura::RemoveMaterial(DWORD dwRefPrice, BYTE bPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
		return;
	
	if (bPos == 1)
		dwPrice = dwRefPrice;
	
	TAuraMaterial tMaterial;
	tMaterial.bHere = 0;
	tMaterial.wCell = 0;
	m_vAuraMaterials[bPos] = tMaterial;
}

bool CPythonAura::GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
		return false;
	
	bHere = m_vAuraMaterials[bPos].bHere;
	wCell = m_vAuraMaterials[bPos].wCell;
	return true;
}

void CPythonAura::GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	dwItemVnum = m_vAuraResult.dwItemVnum;
	dwMinAbs = m_vAuraResult.dwMinAbs;
	dwMaxAbs = m_vAuraResult.dwMaxAbs;
}

PyObject * SendAuraCloseRequest(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAuraClosePacket();
	return Py_BuildNone();
}

PyObject * SendAuraAdd(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	TItemPos tPos;
	if (!PyTuple_GetInteger(poArgs, 0, &tPos.window_type))
		return Py_BuildException();
	else if (!PyTuple_GetInteger(poArgs, 1, &tPos.cell))
		return Py_BuildException();
	else if (!PyTuple_GetInteger(poArgs, 2, &bPos))
		return Py_BuildException();
	
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAuraAddPacket(tPos, bPos);
	return Py_BuildNone();
}

PyObject * SendAuraRemove(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAuraRemovePacket(bPos);
	return Py_BuildNone();
}

PyObject * GetAuraPrice(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonAura::Instance().GetPrice());
}

PyObject * GetAuraAttachedItem(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonAura::Instance().GetAttachedItem(bPos, bHere, wCell);
	if (!bGet)
	{
		bHere = 0;
		wCell = 0;
	}
	
	return Py_BuildValue("ii", bHere, wCell);
}

PyObject * GetAuraResultItem(PyObject * poSelf, PyObject * poArgs)
{
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	CPythonAura::Instance().GetResultItem(dwItemVnum, dwMinAbs, dwMaxAbs);
	return Py_BuildValue("iii", dwItemVnum, dwMinAbs, dwMaxAbs);
}

PyObject * SendAuraRefineRequest(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonAura::Instance().GetAttachedItem(1, bHere, wCell);
	if (bGet)
	{
		if (bHere)
		{
			CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
			rkNetStream.SendAuraRefinePacket();
		}
	}
	
	return Py_BuildNone();
}

void initAura()
{
	static PyMethodDef functions[] = {
										{"SendCloseRequest", SendAuraCloseRequest, METH_VARARGS},
										{"Add", SendAuraAdd, METH_VARARGS},
										{"Remove", SendAuraRemove, METH_VARARGS},
										{"GetPrice", GetAuraPrice, METH_VARARGS},
										{"GetAttachedItem", GetAuraAttachedItem, METH_VARARGS},
										{"GetResultItem", GetAuraResultItem, METH_VARARGS},
										{"SendRefineRequest", SendAuraRefineRequest, METH_VARARGS},
										{NULL, NULL, NULL},
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* pModule = Py_InitModule(GetModuleName(AURA_MODULE).c_str(), functions);
#else
	PyObject* pModule = Py_InitModule("aura", functions);
#endif

	PyModule_AddIntConstant(pModule, "ABSORPTION_SOCKET", AURA_ABSORPTION_SOCKET);
	PyModule_AddIntConstant(pModule, "ABSORBED_SOCKET", AURA_ABSORBED_SOCKET);
	PyModule_AddIntConstant(pModule, "LEVEL_SOCKET", AURA_LEVEL_SOCKET);
	PyModule_AddIntConstant(pModule, "WINDOW_MAX_MATERIALS", AURA_WINDOW_MAX_MATERIALS);
	PyModule_AddIntConstant(pModule, "ICE_RUNIC", AURA_ICE_RUNIC);
	PyModule_AddIntConstant(pModule, "LIMIT_RANGE", 1000);
}