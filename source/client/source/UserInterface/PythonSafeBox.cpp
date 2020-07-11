#include "StdAfx.h"
#include "PythonSafeBox.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

void CPythonSafeBox::OpenSafeBox(int iSize)
{
	m_llMoney = 0;
	m_wCheque = 0;
	m_ItemInstanceVector.clear();
#ifdef ENABLE_CHECK_IF_SAFEBOX_OPEN
	m_isOpenSafebox = true;
#endif
	m_ItemInstanceVector.resize(SAFEBOX_SLOT_X_COUNT * iSize);

	for (DWORD i = 0; i < m_ItemInstanceVector.size(); ++i)
	{
		TItemData & rInstance = m_ItemInstanceVector[i];
		ZeroMemory(&rInstance, sizeof(rInstance));
	}
}

void CPythonSafeBox::SetItemData(DWORD dwSlotIndex, const TItemData & rItemInstance)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::SetItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	m_ItemInstanceVector[dwSlotIndex] = rItemInstance;
}

void CPythonSafeBox::DelItemData(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::DelItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	TItemData & rInstance = m_ItemInstanceVector[dwSlotIndex];
	ZeroMemory(&rInstance, sizeof(rInstance));
}

void CPythonSafeBox::SetMoney(long long llMoney)
{
	m_llMoney = llMoney;
}

long long CPythonSafeBox::GetMoney()
{
	return m_llMoney;
}

void CPythonSafeBox::SetCheque(WORD wCheque)
{
	m_wCheque = wCheque;
}

WORD CPythonSafeBox::GetCheque()
{
	return m_wCheque;
}

int CPythonSafeBox::GetCurrentSafeBoxSize()
{
	return m_ItemInstanceVector.size();
}

BOOL CPythonSafeBox::GetSlotItemID(DWORD dwSlotIndex, DWORD* pdwItemID)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*pdwItemID=m_ItemInstanceVector[dwSlotIndex].vnum;

	return TRUE;
}

BOOL CPythonSafeBox::GetItemDataPtr(DWORD dwSlotIndex, TItemData ** ppInstance)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*ppInstance = &m_ItemInstanceVector[dwSlotIndex];

	return TRUE;
}

void CPythonSafeBox::OpenMall(int iSize)
{
	m_MallItemInstanceVector.clear();
	m_MallItemInstanceVector.resize(SAFEBOX_SLOT_X_COUNT * iSize);

	for (DWORD i = 0; i < m_MallItemInstanceVector.size(); ++i)
	{
		TItemData & rInstance = m_MallItemInstanceVector[i];
		ZeroMemory(&rInstance, sizeof(rInstance));
	}
}

void CPythonSafeBox::SetMallItemData(DWORD dwSlotIndex, const TItemData & rItemData)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::SetMallItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	m_MallItemInstanceVector[dwSlotIndex] = rItemData;
}

void CPythonSafeBox::DelMallItemData(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::DelMallItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	TItemData & rInstance = m_MallItemInstanceVector[dwSlotIndex];
	ZeroMemory(&rInstance, sizeof(rInstance));
}

BOOL CPythonSafeBox::GetMallItemDataPtr(DWORD dwSlotIndex, TItemData ** ppInstance)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetMallSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*ppInstance = &m_MallItemInstanceVector[dwSlotIndex];

	return TRUE;
}

BOOL CPythonSafeBox::GetSlotMallItemID(DWORD dwSlotIndex, DWORD * pdwItemID)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetMallSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*pdwItemID = m_MallItemInstanceVector[dwSlotIndex].vnum;

	return TRUE;
}

DWORD CPythonSafeBox::GetMallSize()
{
	return m_MallItemInstanceVector.size();
}

#ifdef ENABLE_CHECK_IF_SAFEBOX_OPEN
bool CPythonSafeBox::isOpen()
{
	return m_isOpenSafebox;
}

void CPythonSafeBox::Close()
{
	m_isOpenSafebox = false;
}
#endif

CPythonSafeBox::CPythonSafeBox()
{
	m_llMoney = 0;
	m_wCheque = 0;
#ifdef ENABLE_CHECK_IF_SAFEBOX_OPEN
	m_isOpenSafebox = false;
#endif
}

CPythonSafeBox::~CPythonSafeBox()
{
}

PyObject * safeboxGetCurrentSafeboxSize(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSafeBox::Instance().GetCurrentSafeBoxSize());
}

PyObject * safeboxGetItemID(PyObject * poSelf, PyObject * poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
		return Py_BadArgument();

	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pInstance->vnum);
}

PyObject * safeboxGetItemCount(PyObject * poSelf, PyObject * poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
		return Py_BadArgument();

	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pInstance->count);
}

PyObject * safeboxGetItemFlags(PyObject * poSelf, PyObject * poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
		return Py_BadArgument();

	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pInstance->flags);
}

PyObject * safeboxGetItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BadArgument();
	int iSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSocketIndex))
		return Py_BadArgument();

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return Py_BuildException();

	TItemData * pItemData;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(iSlotIndex, &pItemData))
		return Py_BuildException();

	return Py_BuildValue("i", pItemData->alSockets[iSocketIndex]);
}

PyObject * safeboxGetItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		TItemData * pItemData;
		if (CPythonSafeBox::Instance().GetItemDataPtr(iSlotIndex, &pItemData))
			return Py_BuildValue("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}

PyObject * safeboxGetMoney(PyObject * poSelf, PyObject * poArgs)
{
	return PyLong_FromLongLong(CPythonSafeBox::Instance().GetMoney());
}

PyObject * safeboxGetCheque(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSafeBox::Instance().GetCheque());
}

PyObject * safeboxGetMallItemID(PyObject * poSelf, PyObject * poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
		return Py_BadArgument();

	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(ipos, &pInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pInstance->vnum);
}

PyObject * safeboxGetMallItemCount(PyObject * poSelf, PyObject * poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger(poArgs, 0, &ipos))
		return Py_BadArgument();

	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(ipos, &pInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pInstance->count);
}

PyObject * safeboxGetMallItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BadArgument();
	int iSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSocketIndex))
		return Py_BadArgument();

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return Py_BuildException();

	TItemData * pItemData;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(iSlotIndex, &pItemData))
		return Py_BuildException();

	return Py_BuildValue("i", pItemData->alSockets[iSocketIndex]);
}

PyObject * safeboxGetMallItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		TItemData * pItemData;
		if (CPythonSafeBox::Instance().GetMallItemDataPtr(iSlotIndex, &pItemData))
			return Py_BuildValue("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}

PyObject * safeboxGetMallSize(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSafeBox::Instance().GetMallSize());
}

#ifdef ENABLE_CHECK_IF_SAFEBOX_OPEN
PyObject * safeboxIsOpen(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSafeBox::Instance().isOpen());
}

PyObject * safeboxClose(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSafeBox::Instance().Close();
	return Py_BuildNone();
}
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
PyObject * safeboxGetItemTransmutation(PyObject * poSelf, PyObject * poArgs)
{
	int iPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iPos))
		return Py_BadArgument();
	
	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(iPos, &pInstance))
		return Py_BuildException();
	
	return Py_BuildValue("i", pInstance->transmutation);
}

PyObject * safeboxGetMallItemTransmutation(PyObject * poSelf, PyObject * poArgs)
{
	int iPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iPos))
		return Py_BadArgument();
	
	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(iPos, &pInstance))
		return Py_BuildException();
	
	return Py_BuildValue("i", pInstance->transmutation);
}
#endif

PyObject * safeboxGetItemUnbindTime(PyObject * poSelf, PyObject * poArgs)
{
	int iPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iPos))
		return Py_BadArgument();
	
	TItemData * pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(iPos, &pInstance))
		return Py_BuildException();
	
	return Py_BuildValue("i", pInstance->sealbind);
}

void initsafebox()
{
	static PyMethodDef s_methods[] =
	{
		// SafeBox
		{ "GetCurrentSafeboxSize",		safeboxGetCurrentSafeboxSize,			METH_VARARGS },
		{ "GetItemID",					safeboxGetItemID,						METH_VARARGS },
		{ "GetItemCount",				safeboxGetItemCount,					METH_VARARGS },
		{ "GetItemFlags",				safeboxGetItemFlags,					METH_VARARGS },
		{ "GetItemMetinSocket",			safeboxGetItemMetinSocket,				METH_VARARGS },
		{ "GetItemAttribute",			safeboxGetItemAttribute,				METH_VARARGS },
		{"GetMoney", safeboxGetMoney, METH_VARARGS},
		{"GetCheque", safeboxGetCheque, METH_VARARGS},
#ifdef ENABLE_CHECK_IF_SAFEBOX_OPEN
		{ "isOpen", safeboxIsOpen, METH_VARARGS },
		{ "Close", safeboxClose, METH_VARARGS },
#endif
		// Mall
		{ "GetMallItemID",				safeboxGetMallItemID,					METH_VARARGS },
		{ "GetMallItemCount",			safeboxGetMallItemCount,				METH_VARARGS },
		{ "GetMallItemMetinSocket",		safeboxGetMallItemMetinSocket,			METH_VARARGS },
		{ "GetMallItemAttribute",		safeboxGetMallItemAttribute,			METH_VARARGS },
		{ "GetMallSize",				safeboxGetMallSize,						METH_VARARGS },
		
#ifdef ENABLE_CHANGELOOK_SYSTEM
		{"GetItemTransmutation", safeboxGetItemTransmutation, METH_VARARGS},
		{"GetMallItemTransmutation", safeboxGetMallItemTransmutation, METH_VARARGS},
#endif

		{"GetItemUnBindTime",	safeboxGetItemUnbindTime, METH_VARARGS},
		{ NULL,							NULL,									NULL },
	};

#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(SAFEBOX_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("safebox", s_methods);
#endif
	
	PyModule_AddIntConstant(poModule, "SAFEBOX_SLOT_X_COUNT", CPythonSafeBox::SAFEBOX_SLOT_X_COUNT);
	PyModule_AddIntConstant(poModule, "SAFEBOX_SLOT_Y_COUNT", CPythonSafeBox::SAFEBOX_SLOT_Y_COUNT);
	PyModule_AddIntConstant(poModule, "SAFEBOX_PAGE_SIZE", CPythonSafeBox::SAFEBOX_PAGE_SIZE);
}
