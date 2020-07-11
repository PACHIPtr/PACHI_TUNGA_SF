#include "stdafx.h"
#include "PythonPrivateShopSearch.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonPrivateShopSearch::CPythonPrivateShopSearch()
{
}


CPythonPrivateShopSearch::~CPythonPrivateShopSearch()
{
}

void CPythonPrivateShopSearch::AddItemData (DWORD vid, DWORD price, DWORD price2, const TSearchItemData& rItemData)
{

	m_ItemInstanceVector.push_back (rItemData);
}

void CPythonPrivateShopSearch::ClearItemData()
{
	m_ItemInstanceVector.clear();
}

DWORD CPythonPrivateShopSearch::GetItemDataPtr (DWORD index, TSearchItemData** ppInstance)
{
	*ppInstance = &m_ItemInstanceVector.at (index);
	return 1;
}


PyObject* privateShopSearchGetSearchItemMetinSocket (PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 0, &iSlotIndex))
	{
		return Py_BadArgument();
	}
	int iSocketIndex;
	if (!PyTuple_GetInteger (poArgs, 1, &iSocketIndex))
	{
		return Py_BadArgument();
	}

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
	{
		return Py_BuildException();
	}

	CPythonPrivateShopSearch::TSearchItemData* pItemData;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (iSlotIndex, &pItemData))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pItemData->alSockets[iSocketIndex]);
}

PyObject* privateShopSearchGetSearchItemAttribute (PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 0, &iSlotIndex))
	{
		return Py_BuildException();
	}
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger (poArgs, 1, &iAttrSlotIndex))
	{
		return Py_BuildException();
	}

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		CPythonPrivateShopSearch::TSearchItemData* pItemData;
		if (CPythonPrivateShopSearch::Instance().GetItemDataPtr (iSlotIndex, &pItemData))
		{
			return Py_BuildValue ("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
		}
	}

	return Py_BuildValue ("ii", 0, 0);
}

PyObject* privateShopSearchGetItemCount (PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue ("i", CPythonPrivateShopSearch::Instance().GetItemDataCount());
}

PyObject* privateShopSearchGetSearchItemCount (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->count);
}

PyObject* privateShopSearchGetSearchItemPrice (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->price);
}

PyObject* privateShopSearchGetSearchItemPrice2 (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->price2);	
}

PyObject* privateShopSearchGetSearchItemShopVID (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->vid);
}

PyObject* privateShopSearchGetSearchItemPos (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->Cell);
}


PyObject* privateShopSearchGetSearchItemVnum (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->vnum);
}

#ifdef ENABLE_CHANGELOOK_SYSTEM
PyObject* privateShopSearchGetSearchItemTransmutation (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("i", pInstance->transmutation);
}
#endif

PyObject* privateShopSearchGetSellerName (PyObject* poSelf, PyObject* poArgs)
{
	int ipos;
	if (!PyTuple_GetInteger (poArgs, 0, &ipos))
	{
		return Py_BadArgument();
	}

	CPythonPrivateShopSearch::TSearchItemData* pInstance;
	if (!CPythonPrivateShopSearch::Instance().GetItemDataPtr (ipos, &pInstance))
	{
		return Py_BuildException();
	}

	return Py_BuildValue ("s", pInstance->name);
}

PyObject* privateShopSearchClearSearchItems (PyObject* poSelf, PyObject* poArgs)
{
	CPythonPrivateShopSearch::Instance().ClearItemData();
	return Py_BuildNone();
}

void initprivateShopSearch()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetItemCount",		privateShopSearchGetItemCount,			METH_VARARGS },

		{ "GetSearchItemCount",			privateShopSearchGetSearchItemCount,				METH_VARARGS },
		{ "GetSearchItemPrice",			privateShopSearchGetSearchItemPrice,				METH_VARARGS },
		{ "GetSearchItemPrice2",			privateShopSearchGetSearchItemPrice2,				METH_VARARGS },
		{ "GetSearchItemVnum",			privateShopSearchGetSearchItemVnum,				METH_VARARGS },

		{ "GetSearchItemShopVID",			privateShopSearchGetSearchItemShopVID,				METH_VARARGS },

		{ "ClearSearchItems", privateShopSearchClearSearchItems, METH_VARARGS },

		{ "GetSearchItemMetinSocket",		privateShopSearchGetSearchItemMetinSocket,			METH_VARARGS },
		{ "GetSearchItemAttribute",		privateShopSearchGetSearchItemAttribute,			METH_VARARGS },
#ifdef ENABLE_CHANGELOOK_SYSTEM
		{ "GetSearchItemTransmutation",	privateShopSearchGetSearchItemTransmutation,		METH_VARARGS },
#endif
		{ "GetSearchItemSellerName",	privateShopSearchGetSellerName,						METH_VARARGS },
		{ "GetSearchItemPos", privateShopSearchGetSearchItemPos, METH_VARARGS	},
		{ NULL,							NULL,									NULL },
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(SHOP_SEARCH_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("privateShopSearch", s_methods);
#endif
}
