#include "StdAfx.h"
#include "PythonItem.h"
#include <iostream>
#include "../gamelib/ItemManager.h"
#include "InstanceBase.h"
#include "AbstractApplication.h"
#include "Locale_inc.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;

PyObject * itemSetUseSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iUseSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iUseSound))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.SetUseSoundFileName(iUseSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemGtVnum(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetIndex());
}

PyObject * itemSetDropSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iDropSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iDropSound))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.SetDropSoundFileName(iDropSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemSelectItem(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BadArgument();

	if (!CItemManager::Instance().SelectItemData(iIndex))
	{
		//TraceError("Cannot find item by %d", iIndex);
		CItemManager::Instance().SelectItemData(60001);
	}

	return Py_BuildNone();
}

PyObject * itemGetItemName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetName());
}

PyObject * itemGetItemDescription(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetDescription());
}

PyObject * itemGetItemSummary(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetSummary());
}

PyObject * itemGetIconImage(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	//	if (CItemData::ITEM_TYPE_SKILLBOOK == pItemData->GetType())
	//	{
	//		char szItemName[64+1];
	//		_snprintf(szItemName, "d:/ymir work/ui/items/etc/book_%02d.sub", );
	//		CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szItemName);
	//	}

	return Py_BuildValue("i", pItemData->GetIconImage());
}

PyObject * itemGetIconImageFileName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildValue("s", "Noname");

	return Py_BuildValue("s", pImage->GetFileName());
}

PyObject * itemGetItemSize(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("(ii)", 1, pItemData->GetSize());
}

PyObject * itemGetItemType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetType());
}

PyObject * itemGetItemSubType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetSubType());
}

PyObject * itemGetIBuyItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetIBuyItemPrice());
}

PyObject * itemGetISellItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetISellItemPrice());
}


PyObject * itemIsAntiFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsAntiFlag(iFlag));
}

PyObject * itemIsFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(iFlag));
}

PyObject * itemIsWearableFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsWearableFlag(iFlag));
}

PyObject * itemIs1GoldItem(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD));
}

PyObject * itemGetLimit(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemLimit ItemLimit;
	if (!pItemData->GetLimit(iValueIndex, &ItemLimit))
		return Py_BuildException();

	return Py_BuildValue("ii", ItemLimit.bType, ItemLimit.lValue);
}

PyObject * itemGetAffect(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemApply ItemApply;
	if (!pItemData->GetApply(iValueIndex, &ItemApply))
		return Py_BuildException();

	if ((CItemData::APPLY_ATT_SPEED == ItemApply.bType) && (CItemData::ITEM_TYPE_WEAPON == pItemData->GetType()) && (CItemData::WEAPON_TWO_HANDED == pItemData->GetSubType()))
	{
		ItemApply.lValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
	}

	return Py_BuildValue("ii", ItemApply.bType, ItemApply.lValue);
}

PyObject * itemGetValue(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetValue(iValueIndex));
}

PyObject * itemGetSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetSocket(iValueIndex));
}

PyObject * itemGetIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildException("Cannot get icon image by %d", pItemData->GetIndex());

	CGraphicImageInstance * pImageInstance = CGraphicImageInstance::New();
	pImageInstance->SetImagePointer(pImage);

	return Py_BuildValue("i", pImageInstance);
}

PyObject * itemDeleteIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BadArgument();

	CGraphicImageInstance::Delete((CGraphicImageInstance *)iHandle);

	return Py_BuildNone();
}

PyObject * itemIsEquipmentVID(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->IsEquipment());
}

// 2005.05.20.myevan.통합 USE_TYPE 체크
PyObject* itemGetUseType(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	return Py_BuildValue("s", pItemData->GetUseTypeString());
}

PyObject * itemIsRefineScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (pItemData->GetType() != CItemData::ITEM_TYPE_USE)
		return Py_BuildValue("i", FALSE);

	switch (pItemData->GetSubType())
	{
	case CItemData::USE_TUNING:
		return Py_BuildValue("i", TRUE);
		break;
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsDetachScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	int iType = pItemData->GetType();
	int iSubType = pItemData->GetSubType();
	if (iType == CItemData::ITEM_TYPE_USE)
	if (iSubType == CItemData::USE_DETACHMENT)
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemCanAddToQuickSlotItem(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_USE == pItemData->GetType() || CItemData::ITEM_TYPE_QUEST == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsKey(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_TREASURE_KEY == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsMetin(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_METIN == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemRender(PyObject * poSelf, PyObject * poArgs)
{
	CPythonItem::Instance().Render();
	return Py_BuildNone();
}

PyObject * itemUpdate(PyObject * poSelf, PyObject * poArgs)
{
	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();

	POINT ptMouse;
	rkApp.GetMousePosition(&ptMouse);

	CPythonItem::Instance().Update(ptMouse);
	return Py_BuildNone();
}

PyObject * itemCreateItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();
	int iVirtualNumber;
	if (!PyTuple_GetInteger(poArgs, 1, &iVirtualNumber))
		return Py_BadArgument();
	int socket0;
	if (!PyTuple_GetInteger(poArgs, 2, &socket0))
		return Py_BadArgument();
	int socket1;
	if (!PyTuple_GetInteger(poArgs, 3, &socket1))
		return Py_BadArgument();
	int socket2;
	if (!PyTuple_GetInteger(poArgs, 4, &socket2))
		return Py_BadArgument();
	int count;
	if (!PyTuple_GetInteger(poArgs, 5, &count))
		return Py_BadArgument();

	float x;
	if (!PyTuple_GetFloat(poArgs, 6, &x))
		return Py_BadArgument();
	float y;
	if (!PyTuple_GetFloat(poArgs, 7, &y))
		return Py_BadArgument();
	float z;
	if (!PyTuple_GetFloat(poArgs, 8, &z))
		return Py_BadArgument();
	
	bool bDrop = true;
	PyTuple_GetBoolean(poArgs, 9, &bDrop);

	CPythonItem::Instance().CreateItem(iVirtualID, iVirtualNumber, socket0, socket1, socket2, count, x, y, z, bDrop);

	return Py_BuildNone();
}

PyObject * itemDeleteItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();

	CPythonItem::Instance().DeleteItem(iVirtualID);
	return Py_BuildNone();
}

PyObject * itemPick(PyObject * poSelf, PyObject * poArgs)
{
	DWORD dwItemID;
	if (CPythonItem::Instance().GetPickedItemID(&dwItemID))
		return Py_BuildValue("i", dwItemID);
	else
		return Py_BuildValue("i", -1);
}

PyObject* itemLoadItemTable(PyObject* poSelf, PyObject* poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BadArgument();

	CItemManager::Instance().LoadItemTable(szFileName);
	return Py_BuildNone();
}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
PyObject * itemIsSealScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	if (iItemIndex == CItemData::SEAL_ITEM_BINDING_VNUM )
		return Py_BuildValue("i", TRUE);

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsUnSealScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	if (iItemIndex == CItemData::SEAL_ITEM_UNBINDING_VNUM)
		return Py_BuildValue("i", TRUE);

	return Py_BuildValue("i", FALSE);
}
#endif

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
PyObject * itemIsTransferScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	
	if (iItemIndex == 70065)
		return Py_BuildValue("i", true);
	
	return Py_BuildValue("i", false);
}
#endif

#ifdef ENABLE_UPGRADE_SOCKET_SYSTEM
PyObject * itemIsUpgradeSocketScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	
	if (iItemIndex == 81100)
		return Py_BuildValue("i", 1);
	else if (iItemIndex == 81101)
		return Py_BuildValue("i", 2);
	else if (iItemIndex == 81102)
		return Py_BuildValue("i", 3);
	
	return Py_BuildValue("i", false);
}
#endif

PyObject* itemIsNewPetBooks(PyObject* poSelf, PyObject* poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	
	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");
	
	if (pItemData->GetIndex() >= 55010 && pItemData->GetIndex() <= 55035)
		return Py_BuildValue("i", 1);
	
	return Py_BuildValue("i", 0);
}

PyObject * itemGetApplyPoint(PyObject* poSelf, PyObject* poArgs)
{
	int applyType = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &applyType))
		return Py_BadArgument();

	return Py_BuildValue("i", ApplyTypeToPointType((BYTE)applyType));
}

PyObject* itemIsNewPotions(PyObject* poSelf, PyObject* poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	
	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");
	
	if (pItemData->GetValue(1) == 64 || pItemData->GetValue(1) == 65 || pItemData->GetValue(1) == 69 || pItemData->GetValue(1) == 70 || pItemData->GetValue(1) == 15 || pItemData->GetValue(1) == 16)
		return Py_BuildValue("i", 1);
	
	return Py_BuildValue("i", 0);
}

PyObject * itemIsBlend(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::TBlendInfo* pBlendInfo;
	return Py_BuildValue("b", CItemManager::Instance().GetBlendInfoPointer(iItemIndex, &pBlendInfo));
}

PyObject * itemGetBlendApplyType(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::TBlendInfo* pBlendInfo;
	if (!CItemManager::Instance().GetBlendInfoPointer(iItemIndex, &pBlendInfo))
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pBlendInfo->bApplyType);
}

PyObject * itemGetBlendDataCount(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::TBlendInfo* pBlendInfo;
	if (!CItemManager::Instance().GetBlendInfoPointer(iItemIndex, &pBlendInfo))
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pBlendInfo->vec_iApplyValue.size());
}

PyObject * itemGetBlendData(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();
	int iDataIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iDataIndex))
		return Py_BadArgument();

	CItemManager::TBlendInfo* pBlendInfo;
	if (!CItemManager::Instance().GetBlendInfoPointer(iItemIndex, &pBlendInfo))
		return Py_BuildValue("(ii)", 0, 0);

	if (iDataIndex >= pBlendInfo->vec_iApplyValue.size())
	{
		TraceError("invalid data index for blend item %u [%d]", iItemIndex, iDataIndex);
		return Py_BuildException();
	}

	return Py_BuildValue("(ii)", pBlendInfo->vec_iApplyValue[iDataIndex], pBlendInfo->vec_iApplyDuration[iDataIndex]);
}

PyObject * itemGetItemsByName(PyObject * poSelf, PyObject * poArgs)
{
	CItemManager::TItemMap m_ItemMap = CItemManager::Instance().GetItems();
	CItemManager::TItemMap::iterator f = m_ItemMap.begin();
	PyObject* dict = PyTuple_New(m_ItemMap.size());
	int i = 0;
	while (m_ItemMap.end() != f)
	{
		PyObject* item = PyDict_New();
		PyDict_SetItemString(item, "vnum", Py_BuildValue("i", f->second->GetIndex()));
		PyDict_SetItemString(item, "name", Py_BuildValue("s", f->second->GetName()));
		PyTuple_SetItem(dict, i++, item);
		f++;
	}
	return dict;
}

void initItem()
{
	static PyMethodDef s_methods[] =
	{
		{ "SetUseSoundFileName", itemSetUseSoundFileName, METH_VARARGS },
		{ "SetDropSoundFileName", itemSetDropSoundFileName, METH_VARARGS },
		{ "SelectItem", itemSelectItem, METH_VARARGS },
		{ "GetItemName", itemGetItemName, METH_VARARGS },
		{ "GetVnum", itemGtVnum, METH_VARARGS },
		{ "GetItemDescription", itemGetItemDescription, METH_VARARGS },
		{ "GetItemSummary", itemGetItemSummary, METH_VARARGS },
		{ "GetIconImage", itemGetIconImage, METH_VARARGS },
		{ "GetIconImageFileName", itemGetIconImageFileName, METH_VARARGS },
		{ "GetItemSize", itemGetItemSize, METH_VARARGS },
		{ "GetItemType", itemGetItemType, METH_VARARGS },
		{ "GetItemSubType", itemGetItemSubType, METH_VARARGS },
		{ "GetIBuyItemPrice", itemGetIBuyItemPrice, METH_VARARGS },
		{ "GetISellItemPrice", itemGetISellItemPrice, METH_VARARGS },
		{ "IsAntiFlag", itemIsAntiFlag, METH_VARARGS },
		{ "IsFlag", itemIsFlag, METH_VARARGS },
		{ "IsWearableFlag", itemIsWearableFlag, METH_VARARGS },
		{ "Is1GoldItem", itemIs1GoldItem, METH_VARARGS },
		{ "GetLimit", itemGetLimit, METH_VARARGS },
		{ "GetAffect", itemGetAffect, METH_VARARGS },
		{ "GetValue", itemGetValue, METH_VARARGS },
		{ "GetSocket", itemGetSocket, METH_VARARGS },
		{ "GetIconInstance", itemGetIconInstance, METH_VARARGS },
		{ "GetUseType", itemGetUseType, METH_VARARGS },
		{ "DeleteIconInstance", itemDeleteIconInstance, METH_VARARGS },
		{ "IsEquipmentVID", itemIsEquipmentVID, METH_VARARGS },
		{ "IsRefineScroll", itemIsRefineScroll, METH_VARARGS },
		{ "IsDetachScroll", itemIsDetachScroll, METH_VARARGS },
		{ "IsKey", itemIsKey, METH_VARARGS },
		{ "IsMetin", itemIsMetin, METH_VARARGS },
		{ "CanAddToQuickSlotItem", itemCanAddToQuickSlotItem, METH_VARARGS },
		{ "Update", itemUpdate, METH_VARARGS },
		{ "Render", itemRender, METH_VARARGS },
		{ "CreateItem", itemCreateItem, METH_VARARGS },
		{ "DeleteItem", itemDeleteItem, METH_VARARGS },
		{ "Pick", itemPick, METH_VARARGS },
		{ "LoadItemTable", itemLoadItemTable, METH_VARARGS },
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
		{ "IsSealScroll",					itemIsSealScroll,						METH_VARARGS },
		{ "IsUnSealScroll",					itemIsUnSealScroll,						METH_VARARGS },
#endif
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
		{"IsTransferScroll", itemIsTransferScroll, METH_VARARGS},
#endif
#ifdef ENABLE_UPGRADE_SOCKET_SYSTEM
		{"IsUpgradeSocketScroll", itemIsUpgradeSocketScroll, METH_VARARGS},
#endif
		{"IsNewPetBooks", itemIsNewPetBooks, METH_VARARGS},
		{"GetApplyPoint", itemGetApplyPoint, METH_VARARGS},

		{ "IsNewPotions", itemIsNewPotions, METH_VARARGS},
		
		{ "IsBlend",						itemIsBlend,							METH_VARARGS },
		{ "GetBlendApplyType",				itemGetBlendApplyType,					METH_VARARGS },
		{ "GetBlendDataCount",				itemGetBlendDataCount,					METH_VARARGS },
		{ "GetBlendData",					itemGetBlendData,						METH_VARARGS },
		{ "GetItemsByName",					itemGetItemsByName,						METH_VARARGS },
		{ NULL, NULL, NULL },
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(ITEM_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("item", s_methods);
#endif
	
	PyModule_AddIntConstant(poModule, "USESOUND_ACCESSORY", CPythonItem::USESOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "USESOUND_ARMOR", CPythonItem::USESOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "USESOUND_BOW", CPythonItem::USESOUND_BOW);
	PyModule_AddIntConstant(poModule, "USESOUND_DEFAULT", CPythonItem::USESOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "USESOUND_WEAPON", CPythonItem::USESOUND_WEAPON);
	PyModule_AddIntConstant(poModule, "USESOUND_POTION", CPythonItem::USESOUND_POTION);
	PyModule_AddIntConstant(poModule, "USESOUND_PORTAL", CPythonItem::USESOUND_PORTAL);
	PyModule_AddIntConstant(poModule, "DROPSOUND_ACCESSORY", CPythonItem::DROPSOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "DROPSOUND_ARMOR", CPythonItem::DROPSOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "DROPSOUND_BOW", CPythonItem::DROPSOUND_BOW);
	PyModule_AddIntConstant(poModule, "DROPSOUND_DEFAULT", CPythonItem::DROPSOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "DROPSOUND_WEAPON", CPythonItem::DROPSOUND_WEAPON);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_COUNT", c_Equipment_Count);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_HEAD", c_Equipment_Head);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BODY", c_Equipment_Body);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WEAPON", c_Equipment_Weapon);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WRIST", c_Equipment_Wrist);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SHOES", c_Equipment_Shoes);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_NECK", c_Equipment_Neck);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_EAR", c_Equipment_Ear);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE1", c_Equipment_Unique1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE2", c_Equipment_Unique2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_ARROW", c_Equipment_Arrow);
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_RING1", c_Equipment_Ring1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_RING2", c_Equipment_Ring2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BELT", c_Equipment_Belt);
#ifdef ENABLE_TALISMAN_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_CHARM",				c_Equipment_Charm);
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PET",					c_Equipment_Pet);
#endif
#endif
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ELEC", CItemData::APPLY_ATTBONUS_ELEC);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_FIRE", CItemData::APPLY_ATTBONUS_FIRE);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ICE", CItemData::APPLY_ATTBONUS_ICE);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WIND", CItemData::APPLY_ATTBONUS_WIND);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_EARTH", CItemData::APPLY_ATTBONUS_EARTH);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DARK", CItemData::APPLY_ATTBONUS_DARK);
#endif
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_NONE", CItemData::ITEM_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_WEAPON", CItemData::ITEM_TYPE_WEAPON);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ARMOR", CItemData::ITEM_TYPE_ARMOR);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_USE", CItemData::ITEM_TYPE_USE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_AUTOUSE", CItemData::ITEM_TYPE_AUTOUSE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_MATERIAL", CItemData::ITEM_TYPE_MATERIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL", CItemData::ITEM_TYPE_SPECIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TOOL", CItemData::ITEM_TYPE_TOOL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_LOTTERY", CItemData::ITEM_TYPE_LOTTERY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ELK", CItemData::ITEM_TYPE_ELK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_METIN", CItemData::ITEM_TYPE_METIN);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CONTAINER", CItemData::ITEM_TYPE_CONTAINER);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_FISH", CItemData::ITEM_TYPE_FISH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ROD", CItemData::ITEM_TYPE_ROD);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RESOURCE", CItemData::ITEM_TYPE_RESOURCE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CAMPFIRE", CItemData::ITEM_TYPE_CAMPFIRE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_UNIQUE", CItemData::ITEM_TYPE_UNIQUE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLBOOK", CItemData::ITEM_TYPE_SKILLBOOK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_QUEST", CItemData::ITEM_TYPE_QUEST);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_POLYMORPH", CItemData::ITEM_TYPE_POLYMORPH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_BOX", CItemData::ITEM_TYPE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_KEY", CItemData::ITEM_TYPE_TREASURE_KEY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLFORGET",		CItemData::ITEM_TYPE_SKILLFORGET);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GIFTBOX",			CItemData::ITEM_TYPE_GIFTBOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PICK",				CItemData::ITEM_TYPE_PICK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BLEND", CItemData::ITEM_TYPE_BLEND);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_DS", CItemData::ITEM_TYPE_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL_DS", CItemData::ITEM_TYPE_SPECIAL_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RING", CItemData::ITEM_TYPE_RING);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BELT", CItemData::ITEM_TYPE_BELT);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GACHA",			CItemData::ITEM_TYPE_GACHA);
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_RESIST_MAGIC", CItemData::APPLY_ANTI_RESIST_MAGIC);
#endif
#ifdef ENABLE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_COSTUME", CItemData::ITEM_TYPE_COSTUME);
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_BODY", CItemData::COSTUME_BODY);
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_HAIR", CItemData::COSTUME_HAIR);
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_WEAPON", CItemData::COSTUME_WEAPON);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_WEAPON", c_Costume_Slot_Weapon);
#endif
#ifdef ENABLE_ACCE_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_SASH", CItemData::COSTUME_SASH);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_SASH", c_Costume_Slot_Sash);
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_MOUNT",	CItemData::COSTUME_MOUNT);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_MOUNT", c_Costume_Slot_Mount);
#endif
#ifdef ENABLE_AURA_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_AURA", CItemData::COSTUME_AURA);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_AURA", c_Costume_Slot_Aura);
#endif

	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_START", c_Costume_Slot_Start);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_COUNT", c_Costume_Slot_Count);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_BODY", c_Costume_Slot_Body);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_HAIR", c_Costume_Slot_Hair);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_END", c_Costume_Slot_End);
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_START", c_Belt_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_COUNT", c_Belt_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_END", c_Belt_Inventory_Slot_End);
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	PyModule_AddIntConstant(poModule, "SKILL_BOOK_INVENTORY_SLOT_START",	c_Skill_Book_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "UPGRADE_ITEMS_INVENTORY_SLOT_START",	c_Upgrade_Items_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "STONE_INVENTORY_SLOT_START",			c_Stone_Items_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "CHEST_INVENTORY_SLOT_START",			c_Chest_Items_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "ATTR_INVENTORY_SLOT_START",			c_Attr_Items_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "FLOWER_INVENTORY_SLOT_START",		c_Flower_Items_Inventory_Slot_Start);
#endif
	PyModule_AddIntConstant(poModule, "WEAPON_SWORD", CItemData::WEAPON_SWORD);
	PyModule_AddIntConstant(poModule, "WEAPON_DAGGER", CItemData::WEAPON_DAGGER);
	PyModule_AddIntConstant(poModule, "WEAPON_BOW", CItemData::WEAPON_BOW);
	PyModule_AddIntConstant(poModule, "WEAPON_TWO_HANDED", CItemData::WEAPON_TWO_HANDED);
	PyModule_AddIntConstant(poModule, "WEAPON_BELL", CItemData::WEAPON_BELL);
	PyModule_AddIntConstant(poModule, "WEAPON_FAN", CItemData::WEAPON_FAN);
	PyModule_AddIntConstant(poModule, "WEAPON_ARROW", CItemData::WEAPON_ARROW);
	PyModule_AddIntConstant(poModule, "WEAPON_MOUNT_SPEAR", CItemData::WEAPON_MOUNT_SPEAR);
	PyModule_AddIntConstant(poModule, "WEAPON_CLAW", CItemData::WEAPON_CLAW);
#ifdef ENABLE_QUIVER_SYSTEM
	PyModule_AddIntConstant(poModule, "WEAPON_QUIVER", CItemData::WEAPON_QUIVER);
#endif
	PyModule_AddIntConstant(poModule, "WEAPON_NUM_TYPES", CItemData::WEAPON_NUM_TYPES);
	PyModule_AddIntConstant(poModule, "USE_POTION",					CItemData::USE_POTION);
	PyModule_AddIntConstant(poModule, "USE_TALISMAN",				CItemData::USE_TALISMAN);
	PyModule_AddIntConstant(poModule, "USE_TUNING",					CItemData::USE_TUNING);
	PyModule_AddIntConstant(poModule, "USE_MOVE",					CItemData::USE_MOVE);
	PyModule_AddIntConstant(poModule, "USE_TREASURE_BOX",			CItemData::USE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "USE_MONEYBAG",				CItemData::USE_MONEYBAG);
	PyModule_AddIntConstant(poModule, "USE_BAIT",					CItemData::USE_BAIT);
	PyModule_AddIntConstant(poModule, "USE_ABILITY_UP",				CItemData::USE_ABILITY_UP);
	PyModule_AddIntConstant(poModule, "USE_AFFECT",					CItemData::USE_AFFECT);
	PyModule_AddIntConstant(poModule, "USE_CREATE_STONE",			CItemData::USE_CREATE_STONE);
	PyModule_AddIntConstant(poModule, "USE_SPECIAL",				CItemData::USE_SPECIAL);
	PyModule_AddIntConstant(poModule, "USE_POTION_NODELAY",			CItemData::USE_POTION_NODELAY);
	PyModule_AddIntConstant(poModule, "USE_CLEAR",					CItemData::USE_CLEAR);
	PyModule_AddIntConstant(poModule, "USE_INVISIBILITY",			CItemData::USE_INVISIBILITY);
	PyModule_AddIntConstant(poModule, "USE_DETACHMENT",				CItemData::USE_DETACHMENT);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_PER",		CItemData::USE_TIME_CHARGE_PER);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_FIX",		CItemData::USE_TIME_CHARGE_FIX);
	PyModule_AddIntConstant(poModule, "USE_ADD_ACCESSORY_SOCKET",	CItemData::USE_ADD_ACCESSORY_SOCKET);
	PyModule_AddIntConstant(poModule, "USE_PUT_INTO_BELT_SOCKET",	CItemData::USE_PUT_INTO_BELT_SOCKET);
	PyModule_AddIntConstant(poModule, "USE_CHANGE_COSTUME_ATTR",	CItemData::USE_CHANGE_COSTUME_ATTR);
	PyModule_AddIntConstant(poModule, "USE_RESET_COSTUME_ATTR",		CItemData::USE_RESET_COSTUME_ATTR);
#ifdef ENABLE_TALISMAN_SYSTEM
	PyModule_AddIntConstant(poModule, "USE_CHARM", CItemData::USE_CHARM);
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "USE_PET", CItemData::USE_PET);
#endif
	PyModule_AddIntConstant(poModule, "USE_EXTEND_TIME",				CItemData::USE_EXTEND_TIME);
	
	PyModule_AddIntConstant(poModule, "FISH_ALIVE",					0);
	PyModule_AddIntConstant(poModule, "FISH_DEAD",					1);
	
	PyModule_AddIntConstant(poModule, "RESOURCE_FISHBONE", 0);
	PyModule_AddIntConstant(poModule, "RESOURCE_WATERSTONEPIECE", 1);
	PyModule_AddIntConstant(poModule, "RESOURCE_WATERSTONE", 2);
	PyModule_AddIntConstant(poModule, "RESOURCE_BLOOD_PEARL", 3);
	PyModule_AddIntConstant(poModule, "RESOURCE_BLUE_PEARL", 4);
	PyModule_AddIntConstant(poModule, "RESOURCE_WHITE_PEARL", 5);
	PyModule_AddIntConstant(poModule, "RESOURCE_BUCKET", 6);
	PyModule_AddIntConstant(poModule, "RESOURCE_CRYSTAL", 7);
	PyModule_AddIntConstant(poModule, "RESOURCE_GEM", 8);
	PyModule_AddIntConstant(poModule, "RESOURCE_STONE", 9);
	PyModule_AddIntConstant(poModule, "RESOURCE_METIN", 10);
	PyModule_AddIntConstant(poModule, "RESOURCE_ORE", 11);
	PyModule_AddIntConstant(poModule, "RESOURCE_GREEN_PEARL", 12);
	PyModule_AddIntConstant(poModule, "RESOURCE_PINK_PEARL", 13);
	PyModule_AddIntConstant(poModule, "RESOURCE_GOLD_PEARL", 14);
	
	PyModule_AddIntConstant(poModule, "DS_WHITE", 0);
	PyModule_AddIntConstant(poModule, "DS_RED", 1);
	PyModule_AddIntConstant(poModule, "DS_GREEN", 2);
	PyModule_AddIntConstant(poModule, "DS_BLUE", 3);
	PyModule_AddIntConstant(poModule, "DS_YELLOW", 4);
	PyModule_AddIntConstant(poModule, "DS_BLACK", 5);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_NORMAL", CItemData::MATERIAL_DS_REFINE_NORMAL);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_BLESSED", CItemData::MATERIAL_DS_REFINE_BLESSED);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_HOLLY", CItemData::MATERIAL_DS_REFINE_HOLLY);
	PyModule_AddIntConstant(poModule, "METIN_NORMAL", CItemData::METIN_NORMAL);
	PyModule_AddIntConstant(poModule, "METIN_GOLD", CItemData::METIN_GOLD);
	PyModule_AddIntConstant(poModule, "LIMIT_NONE", CItemData::LIMIT_NONE);
	PyModule_AddIntConstant(poModule, "LIMIT_LEVEL", CItemData::LIMIT_LEVEL);
	PyModule_AddIntConstant(poModule, "LIMIT_STR", CItemData::LIMIT_STR);
	PyModule_AddIntConstant(poModule, "LIMIT_DEX", CItemData::LIMIT_DEX);
	PyModule_AddIntConstant(poModule, "LIMIT_INT", CItemData::LIMIT_INT);
	PyModule_AddIntConstant(poModule, "LIMIT_CON", CItemData::LIMIT_CON);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME", CItemData::LIMIT_REAL_TIME);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME_START_FIRST_USE", CItemData::LIMIT_REAL_TIME_START_FIRST_USE);
	PyModule_AddIntConstant(poModule, "LIMIT_TIMER_BASED_ON_WEAR", CItemData::LIMIT_TIMER_BASED_ON_WEAR);
	PyModule_AddIntConstant(poModule, "LIMIT_TYPE_MAX_NUM", CItemData::LIMIT_MAX_NUM);
	PyModule_AddIntConstant(poModule, "LIMIT_MAX_NUM", CItemData::ITEM_LIMIT_MAX_NUM);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_FEMALE", CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MALE", CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WARRIOR", CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ASSASSIN", CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SURA", CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SHAMAN", CItemData::ITEM_ANTIFLAG_SHAMAN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WOLFMAN", CItemData::ITEM_ANTIFLAG_WOLFMAN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GET", CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_DROP", CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SELL", CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_A", CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_B", CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_R", CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAVE", CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GIVE", CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_PKDROP", CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_STACK", CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MYSHOP", CItemData::ITEM_ANTIFLAG_MYSHOP);

	PyModule_AddIntConstant(poModule, "ITEM_FLAG_CONFIRM_WHEN_USE", CItemData::ITEM_FLAG_CONFIRM_WHEN_USE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_FEMALE", CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MALE", CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WARRIOR", CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_ASSASSIN", CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SURA", CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SHAMAN", CItemData::ITEM_ANTIFLAG_SHAMAN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WOLFMAN", CItemData::ITEM_ANTIFLAG_WOLFMAN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GET", CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_DROP", CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SELL", CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_A", CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_B", CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_R", CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAVE", CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GIVE", CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_PKDROP", CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_STACK", CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MYSHOP", CItemData::ITEM_ANTIFLAG_MYSHOP);
	PyModule_AddIntConstant(poModule, "WEARABLE_BODY", CItemData::WEARABLE_BODY);
	PyModule_AddIntConstant(poModule, "WEARABLE_HEAD", CItemData::WEARABLE_HEAD);
	PyModule_AddIntConstant(poModule, "WEARABLE_FOOTS", CItemData::WEARABLE_FOOTS);
	PyModule_AddIntConstant(poModule, "WEARABLE_WRIST", CItemData::WEARABLE_WRIST);
	PyModule_AddIntConstant(poModule, "WEARABLE_WEAPON", CItemData::WEARABLE_WEAPON);
	PyModule_AddIntConstant(poModule, "WEARABLE_NECK", CItemData::WEARABLE_NECK);
	PyModule_AddIntConstant(poModule, "WEARABLE_EAR", CItemData::WEARABLE_EAR);
	PyModule_AddIntConstant(poModule, "WEARABLE_UNIQUE", CItemData::WEARABLE_UNIQUE);
	PyModule_AddIntConstant(poModule, "WEARABLE_SHIELD", CItemData::WEARABLE_SHIELD);
	PyModule_AddIntConstant(poModule, "WEARABLE_ARROW", CItemData::WEARABLE_ARROW);
	PyModule_AddIntConstant(poModule, "ARMOR_BODY", CItemData::ARMOR_BODY);
	PyModule_AddIntConstant(poModule, "ARMOR_HEAD", CItemData::ARMOR_HEAD);
	PyModule_AddIntConstant(poModule, "ARMOR_SHIELD", CItemData::ARMOR_SHIELD);
	PyModule_AddIntConstant(poModule, "ARMOR_WRIST", CItemData::ARMOR_WRIST);
	PyModule_AddIntConstant(poModule, "ARMOR_FOOTS", CItemData::ARMOR_FOOTS);
	PyModule_AddIntConstant(poModule, "ARMOR_NECK", CItemData::ARMOR_NECK);
	PyModule_AddIntConstant(poModule, "ARMOR_EAR", CItemData::ARMOR_EAR);
	PyModule_AddIntConstant(poModule, "ITEM_APPLY_MAX_NUM", CItemData::ITEM_APPLY_MAX_NUM);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_MAX_NUM", CItemData::ITEM_SOCKET_MAX_NUM);
	PyModule_AddIntConstant(poModule, "APPLY_NONE", CItemData::APPLY_NONE);
	PyModule_AddIntConstant(poModule, "APPLY_STR", CItemData::APPLY_STR);
	PyModule_AddIntConstant(poModule, "APPLY_DEX", CItemData::APPLY_DEX);
	PyModule_AddIntConstant(poModule, "APPLY_CON", CItemData::APPLY_CON);
	PyModule_AddIntConstant(poModule, "APPLY_INT", CItemData::APPLY_INT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP", CItemData::APPLY_MAX_HP);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP", CItemData::APPLY_MAX_SP);
	PyModule_AddIntConstant(poModule, "APPLY_HP_REGEN", CItemData::APPLY_HP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_SP_REGEN", CItemData::APPLY_SP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_DEF_GRADE_BONUS", CItemData::APPLY_DEF_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_GRADE_BONUS", CItemData::APPLY_ATT_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_SPEED", CItemData::APPLY_ATT_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MOV_SPEED", CItemData::APPLY_MOV_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_CAST_SPEED", CItemData::APPLY_CAST_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATT_GRADE", CItemData::APPLY_MAGIC_ATT_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_DEF_GRADE", CItemData::APPLY_MAGIC_DEF_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL", CItemData::APPLY_SKILL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ANIMAL", CItemData::APPLY_ATTBONUS_ANIMAL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD", CItemData::APPLY_ATTBONUS_UNDEAD);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", CItemData::APPLY_ATTBONUS_DEVIL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_HUMAN", CItemData::APPLY_ATTBONUS_HUMAN);
	PyModule_AddIntConstant(poModule, "APPLY_BOW_DISTANCE", CItemData::APPLY_BOW_DISTANCE);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_BOW", CItemData::APPLY_RESIST_BOW);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_FIRE", CItemData::APPLY_RESIST_FIRE);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ELEC", CItemData::APPLY_RESIST_ELEC);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC", CItemData::APPLY_RESIST_MAGIC);
	PyModule_AddIntConstant(poModule, "APPLY_POISON_PCT", CItemData::APPLY_POISON_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_SLOW_PCT", CItemData::APPLY_SLOW_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_STUN_PCT", CItemData::APPLY_STUN_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_CRITICAL_PCT", CItemData::APPLY_CRITICAL_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_PENETRATE_PCT", CItemData::APPLY_PENETRATE_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ORC", CItemData::APPLY_ATTBONUS_ORC);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MILGYO", CItemData::APPLY_ATTBONUS_MILGYO);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD", CItemData::APPLY_ATTBONUS_UNDEAD);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", CItemData::APPLY_ATTBONUS_DEVIL);
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_HP", CItemData::APPLY_STEAL_HP);
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_SP", CItemData::APPLY_STEAL_SP);
	PyModule_AddIntConstant(poModule, "APPLY_MANA_BURN_PCT", CItemData::APPLY_MANA_BURN_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_DAMAGE_SP_RECOVER", CItemData::APPLY_DAMAGE_SP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_BLOCK", CItemData::APPLY_BLOCK);
	PyModule_AddIntConstant(poModule, "APPLY_DODGE", CItemData::APPLY_DODGE);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SWORD", CItemData::APPLY_RESIST_SWORD);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_TWOHAND", CItemData::APPLY_RESIST_TWOHAND);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DAGGER", CItemData::APPLY_RESIST_DAGGER);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_BELL", CItemData::APPLY_RESIST_BELL);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_FAN", CItemData::APPLY_RESIST_FAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WIND", CItemData::APPLY_RESIST_WIND);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_CLAW", CItemData::APPLY_RESIST_CLAW);
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_MELEE", CItemData::APPLY_REFLECT_MELEE);
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_CURSE", CItemData::APPLY_REFLECT_CURSE);
	PyModule_AddIntConstant(poModule, "APPLY_POISON_REDUCE", CItemData::APPLY_POISON_REDUCE);
	PyModule_AddIntConstant(poModule, "APPLY_KILL_SP_RECOVER", CItemData::APPLY_KILL_SP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_EXP_DOUBLE_BONUS", CItemData::APPLY_EXP_DOUBLE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_GOLD_DOUBLE_BONUS", CItemData::APPLY_GOLD_DOUBLE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ITEM_DROP_BONUS", CItemData::APPLY_ITEM_DROP_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_POTION_BONUS", CItemData::APPLY_POTION_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_KILL_HP_RECOVER", CItemData::APPLY_KILL_HP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_STUN", CItemData::APPLY_IMMUNE_STUN);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_SLOW", CItemData::APPLY_IMMUNE_SLOW);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_FALL", CItemData::APPLY_IMMUNE_FALL);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_STAMINA", CItemData::APPLY_MAX_STAMINA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WARRIOR", CItemData::APPLY_ATT_BONUS_TO_WARRIOR);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ASSASSIN", CItemData::APPLY_ATT_BONUS_TO_ASSASSIN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SURA", CItemData::APPLY_ATT_BONUS_TO_SURA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SHAMAN", CItemData::APPLY_ATT_BONUS_TO_SHAMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WOLFMAN", CItemData::APPLY_ATT_BONUS_TO_WOLFMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MONSTER", CItemData::APPLY_ATT_BONUS_TO_MONSTER);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ATTBONUS", CItemData::APPLY_MALL_ATTBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_DEFBONUS", CItemData::APPLY_MALL_DEFBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_EXPBONUS", CItemData::APPLY_MALL_EXPBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ITEMBONUS", CItemData::APPLY_MALL_ITEMBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_GOLDBONUS", CItemData::APPLY_MALL_GOLDBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP_PCT", CItemData::APPLY_MAX_HP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP_PCT", CItemData::APPLY_MAX_SP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DAMAGE_BONUS", CItemData::APPLY_SKILL_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DAMAGE_BONUS", CItemData::APPLY_NORMAL_HIT_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DEFEND_BONUS", CItemData::APPLY_SKILL_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DEFEND_BONUS", CItemData::APPLY_NORMAL_HIT_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WARRIOR", CItemData::APPLY_RESIST_WARRIOR);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ASSASSIN", CItemData::APPLY_RESIST_ASSASSIN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SURA", CItemData::APPLY_RESIST_SURA);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SHAMAN", CItemData::APPLY_RESIST_SHAMAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WOLFMAN", CItemData::APPLY_RESIST_WOLFMAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_CLAW", CItemData::APPLY_RESIST_CLAW);
	PyModule_AddIntConstant(poModule, "APPLY_ENERGY", CItemData::APPLY_ENERGY);
	PyModule_AddIntConstant(poModule, "APPLY_COSTUME_ATTR_BONUS", CItemData::APPLY_COSTUME_ATTR_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATTBONUS_PER", CItemData::APPLY_MAGIC_ATTBONUS_PER);
	PyModule_AddIntConstant(poModule, "APPLY_MELEE_MAGIC_ATTBONUS_PER", CItemData::APPLY_MELEE_MAGIC_ATTBONUS_PER);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ICE", CItemData::APPLY_RESIST_ICE);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_EARTH", CItemData::APPLY_RESIST_EARTH);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DARK", CItemData::APPLY_RESIST_DARK);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_CRITICAL_PCT", CItemData::APPLY_ANTI_CRITICAL_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_PENETRATE_PCT", CItemData::APPLY_ANTI_PENETRATE_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_BLEEDING_PCT", CItemData::APPLY_BLEEDING_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_BLEEDING_REDUCE", CItemData::APPLY_BLEEDING_REDUCE);
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	PyModule_AddIntConstant(poModule, "E_SEAL_DATE_DEFAULT_TIMESTAMP",		CItemData::SEAL_DATE_DEFAULT_TIMESTAMP);
	PyModule_AddIntConstant(poModule, "E_SEAL_DATE_UNLIMITED_TIMESTAMP",	CItemData::SEAL_DATE_UNLIMITED_TIMESTAMP);
#endif
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAFEBOX",		CItemData::ITEM_ANTIFLAG_SAFEBOX);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAFEBOX",			CItemData::ITEM_ANTIFLAG_SAFEBOX);
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SWORD", CItemData::APPLY_ATTBONUS_SWORD);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_TWOHANDED", CItemData::APPLY_ATTBONUS_TWOHANDED);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DAGGER", CItemData::APPLY_ATTBONUS_DAGGER);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BELL", CItemData::APPLY_ATTBONUS_BELL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_FAN", CItemData::APPLY_ATTBONUS_FAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BOW", CItemData::APPLY_ATTBONUS_BOW);
#endif
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_HUMAN", CItemData::APPLY_RESIST_HUMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_STONE", CItemData::APPLY_ATTBONUS_STONE);
	PyModule_AddIntConstant (poModule, "PET_HATCHING_MONEY", PET_HATCHING_MONEY);
	PyModule_AddIntConstant (poModule, "PET_NAME_MIN_SIZE", PET_NAME_MIN_SIZE);
	PyModule_AddIntConstant (poModule, "PET_NAME_MAX_SIZE", PET_NAME_MAX_SIZE);
	
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_RARE_ABILITY", CItemData::ITEM_FLAG_RARE_ABILITY);
#endif

	/*** NEWS ***/
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC_REDUCTION", CItemData::APPLY_ANTI_RESIST_MAGIC);
	
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOUL",	CItemData::ITEM_TYPE_SOUL);
	PyModule_AddIntConstant(poModule, "RED_SOUL", 0);
	PyModule_AddIntConstant(poModule, "BLUE_SOUL", 1);
	
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DESERT", CItemData::APPLY_ATTBONUS_DESERT);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_CZ", CItemData::APPLY_ATTBONUS_CZ);
	
#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_RAZADOR",	 CItemData::APPLY_ATTBONUS_RAZADOR);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_NEMERE",	CItemData::APPLY_ATTBONUS_NEMERE);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_HYDRA",	CItemData::APPLY_ATTBONUS_HYDRA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_JOTUN_THRYM",	CItemData::APPLY_ATTBONUS_JOTUN_THRYM);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_LUSIFER",	CItemData::APPLY_ATTBONUS_LUSIFER);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BERAN_SETAOU",	CItemData::APPLY_ATTBONUS_BERAN_SETAOU);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_AZRAEL",	CItemData::APPLY_ATTBONUS_AZRAEL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MELEY",	 CItemData::APPLY_ATTBONUS_MELEY);
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "APPLY_MOUNT",	CItemData::APPLY_MOUNT);
#endif

#ifdef ENABLE_MUSHROOM_DUNGEON
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MUSHROOM",	CItemData::APPLY_ATTBONUS_MUSHROOM);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MUSHROOM",		CItemData::APPLY_RESIST_MUSHROOM);
#endif

#ifdef ENABLE_WATER_DUNGEON
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WATER",		CItemData::APPLY_ATTBONUS_WATER);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WATER",			CItemData::APPLY_RESIST_WATER);
#endif

#ifdef ENABLE_BONUS_TO_BOSS
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BOSS",		CItemData::APPLY_ATTBONUS_BOSS);
#endif
}
