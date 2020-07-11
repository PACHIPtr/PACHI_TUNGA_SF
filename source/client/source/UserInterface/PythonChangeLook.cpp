#include "StdAfx.h"

#ifdef ENABLE_CHANGELOOK_SYSTEM
#include "PythonChangeLook.h"
#include "PythonNetworkStream.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonChangeLook::CPythonChangeLook()
{
	Clear();
}

CPythonChangeLook::~CPythonChangeLook()
{
	Clear();
}

void CPythonChangeLook::Clear()
{
	dwCost = 0;
	
	m_vMaterials.clear();
	m_vMaterials.resize(CL_WINDOW_MAX_MATERIALS);
	for (BYTE bPos = 0; bPos < m_vMaterials.size(); ++bPos)
	{
		TChangeLookMaterial tMaterial;
		tMaterial.bHere = 0;
		tMaterial.wCell = 0;
		m_vMaterials[bPos] = tMaterial;
	}
}

void CPythonChangeLook::AddMaterial(BYTE bPos, TItemPos tPos)
{
	if (bPos >= CL_WINDOW_MAX_MATERIALS)
		return;
	
	TChangeLookMaterial tMaterial;
	tMaterial.bHere = 1;
	tMaterial.wCell = tPos.cell;
	
	m_vMaterials[bPos] = tMaterial;
}

void CPythonChangeLook::RemoveMaterial(BYTE bPos)
{
	if (bPos >= CL_WINDOW_MAX_MATERIALS)
		return;
	
	TChangeLookMaterial tMaterial;
	tMaterial.bHere = 0;
	tMaterial.wCell = 0;
	
	m_vMaterials[bPos] = tMaterial;
}

void CPythonChangeLook::RemoveAllMaterials()
{
	for (BYTE bPos = 0; bPos < CL_WINDOW_MAX_MATERIALS; ++bPos)
	{
		TChangeLookMaterial tMaterial;
		tMaterial.bHere = 0;
		tMaterial.wCell = 0;
		
		m_vMaterials[bPos] = tMaterial;
	}
}

bool CPythonChangeLook::GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell)
{
	if (bPos >= CL_WINDOW_MAX_MATERIALS)
		return false;
	
	bHere = m_vMaterials[bPos].bHere;
	wCell = m_vMaterials[bPos].wCell;
	return true;
}

PyObject * SendClCloseRequest(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendClClosePacket();
	return Py_BuildNone();
}

PyObject * SendClAdd(PyObject * poSelf, PyObject * poArgs)
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
	rkNetStream.SendClAddPacket(tPos, bPos);
	return Py_BuildNone();
}

PyObject * SendClRemove(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendClRemovePacket(bPos);
	return Py_BuildNone();
}

PyObject * GetClCost(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonChangeLook::Instance().GetCost());
}

PyObject * GetClAttachedItem(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();
	
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonChangeLook::Instance().GetAttachedItem(bPos, bHere, wCell);
	if (!bGet)
	{
		bHere = 0;
		wCell = 0;
	}
	
	return Py_BuildValue("ii", bHere, wCell);
}

PyObject * SendClRefineRequest(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonChangeLook::Instance().GetAttachedItem(1, bHere, wCell);
	if (bGet)
	{
		if (bHere)
		{
			CPythonNetworkStream & rkNetStream = CPythonNetworkStream::Instance();
			rkNetStream.SendClRefinePacket();
		}
	}
	
	return Py_BuildNone();
}

void initChangeLook()
{
	static PyMethodDef functions[] = {
										{"SendCloseRequest", SendClCloseRequest, METH_VARARGS},
										{"Add", SendClAdd, METH_VARARGS},
										{"Remove", SendClRemove, METH_VARARGS},
										{"GetCost", GetClCost, METH_VARARGS},
										{"GetAttachedItem", GetClAttachedItem, METH_VARARGS},
										{"SendRefineRequest", SendClRefineRequest, METH_VARARGS},
										{NULL, NULL, NULL},
	};
		
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* pModule = Py_InitModule(GetModuleName(CHANGELOOK_MODULE).c_str(), functions);
#else
	PyObject* pModule = Py_InitModule("changelook", functions);
#endif
	
	PyModule_AddIntConstant(pModule, "CLEAN_ATTR_VALUE0", CL_CLEAN_ATTR_VALUE0);
	PyModule_AddIntConstant(pModule, "WINDOW_MAX_MATERIALS", CL_WINDOW_MAX_MATERIALS);
	PyModule_AddIntConstant(pModule, "SCROLL_VNUM", CL_SCROLL_VNUM);
	PyModule_AddIntConstant(pModule, "CLEAN_ATTR_VALUE_FIELD", 0);
	PyModule_AddIntConstant(pModule, "LIMIT_RANGE", 1000);
}
#endif
