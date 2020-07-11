#include "StdAfx.h"

#ifdef ENABLE_ACCE_SYSTEM
#include "PythonSash.h"
#include "PythonNetworkStream.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonSash::CPythonSash()
{
	Clear();
}

CPythonSash::~CPythonSash()
{
	Clear();
}

void CPythonSash::Clear()
{
	dwPrice = 0;
	
	ZeroMemory(&m_vSashResult, sizeof(m_vSashResult));
	
	m_vSashMaterials.clear();
	m_vSashMaterials.resize(SASH_WINDOW_MAX_MATERIALS);
	for (BYTE bPos = 0; bPos < m_vSashMaterials.size(); ++bPos)
	{
		TSashMaterial tMaterial;
		tMaterial.bHere = 0;
		tMaterial.wCell = 0;
		m_vSashMaterials[bPos] = tMaterial;
	}
}

void CPythonSash::AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
		return;
	
	if (bPos == 0)
		dwPrice = dwRefPrice;
	
	TSashMaterial tMaterial;
	tMaterial.bHere = 1;
	tMaterial.wCell = tPos.cell;
	m_vSashMaterials[bPos] = tMaterial;
}

void CPythonSash::AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs)
{
	TSashResult tResult;
	tResult.dwItemVnum = dwItemVnum;
	tResult.dwMinAbs = dwMinAbs;
	tResult.dwMaxAbs = dwMaxAbs;
	m_vSashResult = tResult;
}

void CPythonSash::RemoveMaterial(DWORD dwRefPrice, BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
		return;
	
	if (bPos == 1)
		dwPrice = dwRefPrice;
	
	TSashMaterial tMaterial;
	tMaterial.bHere = 0;
	tMaterial.wCell = 0;
	m_vSashMaterials[bPos] = tMaterial;
}

bool CPythonSash::GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
		return false;
	
	bHere = m_vSashMaterials[bPos].bHere;
	wCell = m_vSashMaterials[bPos].wCell;
	return true;
}

void CPythonSash::GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	dwItemVnum = m_vSashResult.dwItemVnum;
	dwMinAbs = m_vSashResult.dwMinAbs;
	dwMaxAbs = m_vSashResult.dwMaxAbs;
}

PyObject * SendSashCloseRequest(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSashClosePacket();
	return Py_BuildNone();
}

PyObject * SendSashAdd(PyObject * poSelf, PyObject * poArgs)
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
	rkNetStream.SendSashAddPacket(tPos, bPos);
	return Py_BuildNone();
}

PyObject * SendSashRemove(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSashRemovePacket(bPos);
	return Py_BuildNone();
}

PyObject * GetSashPrice(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSash::Instance().GetPrice());
}

PyObject * GetSashAttachedItem(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonSash::Instance().GetAttachedItem(bPos, bHere, wCell);
	if (!bGet)
	{
		bHere = 0;
		wCell = 0;
	}
	
	return Py_BuildValue("ii", bHere, wCell);
}

PyObject * GetSashResultItem(PyObject * poSelf, PyObject * poArgs)
{
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	CPythonSash::Instance().GetResultItem(dwItemVnum, dwMinAbs, dwMaxAbs);
	return Py_BuildValue("iii", dwItemVnum, dwMinAbs, dwMaxAbs);
}

PyObject * SendSashRefineRequest(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonSash::Instance().GetAttachedItem(1, bHere, wCell);
	if (bGet)
	{
		if (bHere)
		{
			CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
			rkNetStream.SendSashRefinePacket();
		}
	}
	
	return Py_BuildNone();
}

void initSash()
{
	static PyMethodDef functions[] = {
										{"SendCloseRequest", SendSashCloseRequest, METH_VARARGS},
										{"Add", SendSashAdd, METH_VARARGS},
										{"Remove", SendSashRemove, METH_VARARGS},
										{"GetPrice", GetSashPrice, METH_VARARGS},
										{"GetAttachedItem", GetSashAttachedItem, METH_VARARGS},
										{"GetResultItem", GetSashResultItem, METH_VARARGS},
										{"SendRefineRequest", SendSashRefineRequest, METH_VARARGS},
										{NULL, NULL, NULL},
	};
		
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* pModule = Py_InitModule(GetModuleName(SASH_MODULE).c_str(), functions);
#else
	PyObject* pModule = Py_InitModule("sash", functions);
#endif

	PyModule_AddIntConstant(pModule, "ABSORPTION_SOCKET", SASH_ABSORPTION_SOCKET);
	PyModule_AddIntConstant(pModule, "ABSORBED_SOCKET", SASH_ABSORBED_SOCKET);
	PyModule_AddIntConstant(pModule, "CLEAN_ATTR_VALUE0", SASH_CLEAN_ATTR_VALUE0);
	PyModule_AddIntConstant(pModule, "WINDOW_MAX_MATERIALS", SASH_WINDOW_MAX_MATERIALS);
	PyModule_AddIntConstant(pModule, "CLEAN_ATTR_VALUE_FIELD", 0);
	PyModule_AddIntConstant(pModule, "LIMIT_RANGE", 1000);
}
#endif
