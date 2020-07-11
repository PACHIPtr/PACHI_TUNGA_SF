#include "stdafx.h"
#include "questmanager.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"
#include "log.h"
#include "refine.h"
#include "db.h"
#include "config.h"

#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

namespace quest
{
	//
	// "item" Lua functions
	//

	ALUA(item_get_cell)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			lua_pushnumber(L, q.GetCurrentItem()->GetCell());
		}
		else
			lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(item_select_cell)
	{
		lua_pushboolean(L, 0);
		if (!lua_isnumber(L, 1))
		{
			return 1;
		}
		DWORD cell = (DWORD)lua_tonumber(L, 1);

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		LPITEM item = ch ? ch->GetInventoryItem(cell) : nullptr;

		if (!item)
		{
			return 1;
		}

		CQuestManager::instance().SetCurrentItem(item);
		lua_pushboolean(L, 1);

		return 1;
	}

	ALUA(item_select_cell_ex)
	{
		lua_pushboolean(L, 0);
		if (!lua_isnumber(L, 1))
		{
			return 1;
		}
		DWORD cell = (DWORD)lua_tonumber(L, 1);

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		LPITEM item = ch ? ch->GetStoneItemsInventoryItem(cell) : nullptr;

		if (!item)
		{
			return 1;
		}

		CQuestManager::instance().SetCurrentItem(item);
		lua_pushboolean(L, 1);

		return 1;
	}

	ALUA(item_select)
	{
		lua_pushboolean(L, 0);
		if (!lua_isnumber(L, 1))
		{
			return 1;
		}
		DWORD id = (DWORD)lua_tonumber(L, 1);
		LPITEM item = ITEM_MANAGER::instance().Find(id);

		if (!item)
		{
			return 1;
		}

		CQuestManager::instance().SetCurrentItem(item);
		lua_pushboolean(L, 1);

		return 1;
	}

	ALUA(item_get_id)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			lua_pushnumber(L, q.GetCurrentItem()->GetID());
		}
		else
			lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(item_remove)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item != nullptr) {
			if (q.GetCurrentCharacterPtr() == item->GetOwner()) {
				ITEM_MANAGER::instance().RemoveItem(item);
			}
			else {
				sys_err("Tried to remove invalid item %p", get_pointer(item));
			}
			q.ClearCurrentItem();
		}

		return 0;
	}

	ALUA(item_get_socket)
	{
		CQuestManager& q = CQuestManager::instance();
		if (q.GetCurrentItem() && lua_isnumber(L, 1))
		{
			int idx = (int)lua_tonumber(L, 1);
			if (idx < 0 || idx >= ITEM_SOCKET_MAX_NUM)
				lua_pushnumber(L, 0);
			else
				lua_pushnumber(L, q.GetCurrentItem()->GetSocket(idx));
		}
		else
		{
			lua_pushnumber(L, 0);
		}
		return 1;
	}

	ALUA(item_set_socket)
	{
		CQuestManager& q = CQuestManager::instance();
		if (q.GetCurrentItem() && lua_isnumber(L, 1) && lua_isnumber(L, 2))
		{
			int idx = (int)lua_tonumber(L, 1);
			int value = (int)lua_tonumber(L, 2);
			if (idx >= 0 && idx < ITEM_SOCKET_MAX_NUM)
				q.GetCurrentItem()->SetSocket(idx, value);
		}
		return 0;
	}

	ALUA(item_get_vnum)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetVnum());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_has_flag)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (!lua_isnumber(L, 1))
		{
			sys_err("flag is not a number.");
			lua_pushboolean(L, 0);
			return 1;
		}

		if (!item)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		long lCheckFlag = (long)lua_tonumber(L, 1);
		lua_pushboolean(L, IS_SET(item->GetFlag(), lCheckFlag));

		return 1;
	}

	ALUA(item_get_value)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (!item)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (!lua_isnumber(L, 1))
		{
			sys_err("index is not a number");
			lua_pushnumber(L, 0);
			return 1;
		}

		int index = (int)lua_tonumber(L, 1);

		if (index < 0 || index >= ITEM_VALUES_MAX_NUM)
		{
			sys_err("index(%d) is out of range (0..%d)", index, ITEM_VALUES_MAX_NUM);
			lua_pushnumber(L, 0);
		}
		else
			lua_pushnumber(L, item->GetValue(index));

		return 1;
	}

	ALUA(item_set_value)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (!item)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (false == (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)))
		{
			sys_err("index is not a number");
			lua_pushnumber(L, 0);
			return 1;
		}

		item->SetForceAttribute(
			lua_tonumber(L, 1),		// index
			lua_tonumber(L, 2),		// apply type
			lua_tonumber(L, 3)		// apply value
		);

		return 0;
	}

	ALUA(item_get_name)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushstring(L, item->GetName());
		else
			lua_pushstring(L, "");

		return 1;
	}

	ALUA(item_get_size)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetSize());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_get_count)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetCount());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_get_type)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetType());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_get_sub_type)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetSubType());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_get_refine_vnum)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetRefinedVnum());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_next_refine_vnum)
	{
		DWORD vnum = 0;
		if (lua_isnumber(L, 1))
			vnum = (DWORD)lua_tonumber(L, 1);

		TItemTable* pTable = ITEM_MANAGER::instance().GetTable(vnum);
		if (pTable)
		{
			lua_pushnumber(L, pTable->dwRefinedVnum);
		}
		else
		{
			sys_err("Cannot find item table of vnum %u", vnum);
			lua_pushnumber(L, 0);
		}
		return 1;
	}

	ALUA(item_get_level)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();

		if (item)
			lua_pushnumber(L, item->GetRefineLevel());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(item_get_level_limit)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			if (q.GetCurrentItem()->GetType() != ITEM_WEAPON && q.GetCurrentItem()->GetType() != ITEM_ARMOR)
			{
				return 0;
			}
			lua_pushnumber(L, q.GetCurrentItem()->GetLevelLimit());
			return 1;
		}
		return 0;
	}

#ifdef ENABLE_GROWTH_PET_SYSTEM
	ALUA(item_pet_death)
	{
		CQuestManager& q = CQuestManager::instance();
		DWORD itemid = 0;
		if (q.GetCurrentItem())
		{
			itemid = q.GetCurrentItem()->GetID();
			char szQuery1[1024];
			snprintf(szQuery1, sizeof(szQuery1), "SELECT duration FROM new_petsystem WHERE id = %u LIMIT 1", itemid);
			std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
			if (pmsg2->Get()->uiNumRows > 0) {
				MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
				lua_pushboolean(L, atoi(row[0]) <= 0);
				return 0;
			}
			else {
				lua_pushboolean(L, false);
				sys_err("[NewPetSystem]Error no item founded!On item.pet.death");
				return 0;
			}
		}
		return 0;
	}

	ALUA(item_pet_revive)
	{
		CQuestManager& q = CQuestManager::instance();
		DWORD itemid = 0;
		if (q.GetCurrentItem())
		{
			itemid = q.GetCurrentItem()->GetID();
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =(tduration/2) WHERE id = %d", itemid));
		}
		return 0;
	}
#endif

	ALUA(item_start_realtime_expire)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM pItem = q.GetCurrentItem();

		if (pItem)
		{
			pItem->StartRealTimeExpireEvent();
			return 1;
		}

		return 0;
	}

	ALUA(item_copy_and_give_before_remove)
	{
		lua_pushboolean(L, 0);
		if (!lua_isnumber(L, 1))
			return 1;

		DWORD vnum = (DWORD)lua_tonumber(L, 1);

		CQuestManager& q = CQuestManager::instance();
		LPITEM pItem = q.GetCurrentItem();
		LPCHARACTER pChar = q.GetCurrentCharacterPtr();

		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(pItem, pkNewItem);
			LogManager::instance().ItemLog(pChar, pkNewItem, "COPY SUCCESS", pkNewItem->GetName());

			UINT bCell = pItem->GetCell();

			ITEM_MANAGER::instance().RemoveItem(pItem, "REMOVE (COPY SUCCESS)");

			pkNewItem->AddToCharacter(pChar, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			pkNewItem->AttrLog();

			// ¼º°ø!
			lua_pushboolean(L, 1);
		}

		return 1;
	}

	ALUA(item_get_flag)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		lua_pushnumber(L, item ? item->GetFlag() : 0);
		return 1;
	}

	ALUA(item_get_wearflag)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		lua_pushnumber(L, item ? item->GetWearFlag() : 0);
		return 1;
	}

	ALUA(item_get_antiflag)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		lua_pushnumber(L, item ? item->GetAntiFlag() : 0);
		return 1;
	}

	ALUA(item_has_antiflag)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!lua_isnumber(L, 1))
		{
			sys_err("Invalid argument.");
			return 0;
		}

		if (!item) return 0;

		long lAntiCheck = (long)lua_tonumber(L, 1);
		lua_pushboolean(L, IS_SET(item->GetAntiFlag(), lAntiCheck));
		return 1;
	}

	ALUA(item_get_refine_set)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		lua_pushnumber(L, item ? item->GetRefineSet() : 0);
		return 1;
	}

	ALUA(item_get_limit)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument, need a number from range(0..%d)!", ITEM_LIMIT_MAX_NUM);
			lua_pushnumber(L, 0);
			return 1;
		}

		int byLimitIndex = (int)lua_tonumber(L, 1);
		if (byLimitIndex < 0 || byLimitIndex >= ITEM_LIMIT_MAX_NUM)
		{
			sys_err("Invalid limit type(%d). Out of range(0..%d)", byLimitIndex, ITEM_LIMIT_MAX_NUM);
			lua_pushnumber(L, 0);
			return 1;
		}

		lua_newtable(L);
		{
			lua_pushnumber(L, item->GetLimitType(byLimitIndex));
			lua_rawseti(L, -2, 1);

			lua_pushnumber(L, item->GetLimitValue(byLimitIndex));
			lua_rawseti(L, -2, 2);
		}
		return 1;
	}

	ALUA(item_get_apply)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item)
		{
			sys_err("No current item selected!");
			return 0;
		}

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument, need a number from range(0..%d)!", ITEM_APPLY_MAX_NUM);
			lua_pushnumber(L, 0);
			return 1;
		}

		int bApplyIndex = (int)lua_tonumber(L, 1);
		if (bApplyIndex < 0 || bApplyIndex >= ITEM_APPLY_MAX_NUM)
		{
			sys_err("Invalid apply index(%d). Out of range(0..%d)", bApplyIndex, ITEM_APPLY_MAX_NUM);
			lua_pushnumber(L, 0);
			return 1;
		}

		const TItemTable* itemTable = item->GetProto();

		lua_newtable(L);
		{
			lua_pushnumber(L, itemTable->aLimits[bApplyIndex].bType);
			lua_rawseti(L, -2, 1);

			lua_pushnumber(L, itemTable->aLimits[bApplyIndex].lValue);
			lua_rawseti(L, -2, 2);
		}
		return 1;
	}

	ALUA(item_get_applies)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();
		if (!item)
		{
			sys_err("No current item selected!");
			return 0;
		}

		const TItemTable* itemTable = item->GetProto();
		lua_newtable(L);
		{
			for (BYTE i = 0; i < ITEM_APPLY_MAX_NUM; i++)
			{
				//char Key1[64] = "", Key2[64] = "";

				lua_newtable(L);
				lua_pushnumber(L, itemTable->aLimits[i].bType);
				lua_rawseti(L, -2, 1);

				lua_pushnumber(L, itemTable->aLimits[i].lValue);
				lua_rawseti(L, -2, 2);
				lua_rawseti(L, -2, i);
			}
		}

		return 1;
	}

	ALUA(item_get_refine_materials)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();
		if (!item)
		{
			sys_err("No current item selected!");
			return 0;
		}

		const TRefineTable* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
		if (!prt)
		{
			sys_err("Failed to get refine materials!");
			return 0;
		}

		if (prt->cost == 0 && prt->material_count == 0)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		lua_newtable(L);
		{
			lua_pushstring(L, "cost");
			lua_pushnumber(L, prt->cost);
			lua_rawset(L, -3);

			lua_pushstring(L, "material_count");
			lua_pushnumber(L, prt->material_count);
			lua_rawset(L, -3);

			lua_pushstring(L, "materials");
			lua_newtable(L);
			{
				for (BYTE i = 0; i < prt->material_count; i++)
				{
					lua_newtable(L);
					lua_pushnumber(L, prt->materials[i].vnum);
					lua_rawseti(L, -2, 1);

					lua_pushnumber(L, prt->materials[i].count);
					lua_rawseti(L, -2, 2);
					lua_rawseti(L, -2, i + 1);
				}
			}
			lua_rawset(L, -3);
		}

		return 1;
	}

	ALUA(item_get_addon_type)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;

		const TItemTable* itemTable = item->GetProto();
		lua_pushnumber(L, itemTable ? itemTable->sAddonType : 0);
		return 1;
	}

	ALUA(item_add_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		bool bRet = TRUE;
		if (item->GetAttributeCount() < 5)
			item->AddAttribute();
		else
			bRet = FALSE;

		lua_pushboolean(L, bRet);
		return 1;
	}

	ALUA(item_get_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument, need a number from range(0..%d)!", ITEM_ATTRIBUTE_MAX_NUM - 2);
			lua_pushnumber(L, 0);
			return 1;
		}

		int iAttrIndex = (int)lua_tonumber(L, 1);
		if (iAttrIndex < 0 || iAttrIndex >= ITEM_ATTRIBUTE_MAX_NUM - 2)
		{
			sys_err("Invalid index %d. Index out of range(0..%d)", iAttrIndex, ITEM_ATTRIBUTE_MAX_NUM - 2);
			lua_pushnumber(L, 0);
			return 1;
		}

		const TPlayerItemAttribute& AttrItem = item->GetAttribute(iAttrIndex);

		lua_newtable(L);

		lua_pushnumber(L, AttrItem.bType);
		lua_rawseti(L, -2, 1);

		lua_pushnumber(L, AttrItem.sValue);
		lua_rawseti(L, -2, 2);
		return 1;
	}

	ALUA(item_set_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument[AttrIdx] #1.");
			lua_pushboolean(L, false);
			return 1;
		}
		else if (!lua_isnumber(L, 2))
		{
			sys_err("Wrong argument[AttrType] #2.");
			lua_pushboolean(L, false);
			return 1;
		}
		else if (!lua_isnumber(L, 3))
		{
			sys_err("Wrong argument[AttrValue] #3.");
			lua_pushboolean(L, false);
			return 1;
		}

		int bAttrIndex = (int)lua_tonumber(L, 1);
		if (bAttrIndex < 0 || bAttrIndex >= ITEM_ATTRIBUTE_MAX_NUM - 2)
		{
			sys_err("Invalid AttrIndex %d. AttrIndex out of range(0..4)", bAttrIndex);
			lua_pushboolean(L, false);
			return 1;
		}

		int bAttrType = (int)lua_tonumber(L, 2);
		if (bAttrType < 1 || bAttrType >= MAX_APPLY_NUM)
		{
			sys_err("Invalid AttrType %d. AttrType out of range(1..%d)", MAX_APPLY_NUM);
			lua_pushboolean(L, false);
			return 1;
		}

		if (item->HasAttr(bAttrType) && (item->GetAttribute(bAttrIndex).bType != bAttrType))
		{
			sys_err("AttrType[%d] multiplicated.", bAttrType);
			lua_pushboolean(L, false);
			return 1;
		}

		int bAttrValue = (int)lua_tonumber(L, 3);
		if (bAttrValue < 1 || bAttrValue >= 32768)
		{
			sys_err("Invalid AttrValue %d. AttrValue should be between 1 and 32767!", bAttrValue);
			lua_pushboolean(L, false);
			return 1;
		}

		bool bRet = TRUE;
		int bAttrCount = item->GetAttributeCount();
		if (bAttrCount <= 4 && bAttrCount >= 0)
		{
			if (bAttrCount < bAttrIndex)
				bAttrIndex = bAttrCount;

			item->SetForceAttribute(bAttrIndex, bAttrType, bAttrValue);
		}
		else
			bRet = FALSE;

		lua_pushboolean(L, bRet);
		return 1;
	}

	ALUA(item_change_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		bool bRet = TRUE;
		if (item->GetAttributeCount() > 0)
			item->ChangeAttribute();
		else
			bRet = FALSE;

		lua_pushboolean(L, bRet);
		return 1;
	}

	ALUA(item_add_rare_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		lua_pushboolean(L, item->AddRareAttribute() ? TRUE : FALSE);
		return 1;
	}

	ALUA(item_get_rare_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument, need a number from range(0..1)!");
			lua_pushnumber(L, 0);
			return 1;
		}

		int iRareAttrIndex = (int)lua_tonumber(L, 1);
		if (iRareAttrIndex < 0 || iRareAttrIndex > 1)
		{
			sys_err("Invalid index %d. Index out of range(0..1)", iRareAttrIndex);
			lua_pushboolean(L, false);
			return 1;
		}

		const TPlayerItemAttribute& RareAttrItem = item->GetAttribute(iRareAttrIndex + 5);

		lua_newtable(L);

		lua_pushnumber(L, RareAttrItem.bType);
		lua_rawseti(L, -2, 1);

		lua_pushnumber(L, RareAttrItem.sValue);
		lua_rawseti(L, -2, 2);
		return 1;
	}

	ALUA(item_set_rare_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		if (!lua_isnumber(L, 1))
		{
			sys_err("Wrong argument[AttrIdx], not number!");
			lua_pushboolean(L, false);
			return 1;
		}
		else if (!lua_isnumber(L, 2))
		{
			sys_err("Wrong argument[AttrType], not number!");
			lua_pushboolean(L, false);
			return 1;
		}
		else if (!lua_isnumber(L, 3))
		{
			sys_err("Wrong argument[AttrValue], not number!");
			lua_pushboolean(L, false);
			return 1;
		}

		int iRareAttrIndex = (int)lua_tonumber(L, 1);
		if (iRareAttrIndex < 0 || iRareAttrIndex > 1)
		{
			sys_err("Invalid index %d. Index out of range(0..1)", iRareAttrIndex);
			lua_pushboolean(L, false);
			return 1;
		}

		int iRareAttrType = (int)lua_tonumber(L, 2);
		if (iRareAttrType < 1 || iRareAttrType >= MAX_APPLY_NUM)
		{
			sys_err("Invalid apply %d. Apply out of range(1..%d)", MAX_APPLY_NUM);
			lua_pushboolean(L, false);
			return 1;
		}

		if (item->HasAttr(iRareAttrType) && (item->GetAttribute(iRareAttrIndex).bType != iRareAttrType))
		{
			sys_err("Apply %d muliplicated.", iRareAttrType);
			lua_pushboolean(L, false);
			return 1;
		}

		int iRareAttrValue = (int)lua_tonumber(L, 3);
		if (iRareAttrValue < 1 || iRareAttrValue >= 32768)
		{
			sys_err("Invalid value %d. The value should be between 1 and 32767!", iRareAttrValue);
			lua_pushboolean(L, false);
			return 1;
		}

		bool bRet = TRUE;
		int iRareAttrCount = item->GetRareAttrCount();
		if (iRareAttrCount <= 1 && iRareAttrCount >= 0)
		{
			if (iRareAttrCount < iRareAttrIndex)
				iRareAttrIndex = iRareAttrCount;

			item->SetForceAttribute(iRareAttrIndex + 5, iRareAttrType, iRareAttrValue);
		}
		else
			bRet = FALSE;

		lua_pushboolean(L, bRet);
		return 1;
	}

	ALUA(item_change_rare_attribute)
	{
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item) return 0;
		if (item->GetType() == ITEM_COSTUME)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		lua_pushboolean(L, item->ChangeRareAttribute());
		return 1;
	}

	ALUA(item_equip_selected)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (!lua_isnumber(L, 1))
		{
			sys_err("Argument error.");
			lua_pushboolean(L, false);
			return 1;
		}

		int bCell = (int)lua_tonumber(L, 1);
		if (bCell < 0 || bCell >= WEAR_MAX_NUM)
		{
			sys_err("Invalid wear position %d. Index out of range(0..%d)", bCell, WEAR_MAX_NUM);
			lua_pushboolean(L, false);
			return 1;
		}

		LPITEM item = CQuestManager::instance().GetCurrentItem();       //current item in used
		LPITEM equipped = ch->GetWear(bCell);                            //current equipped item on target slot

		//check the pointers
		if (!ch || !item) return 0;

		//remove the equipped item
		if ((equipped->GetVnum() != 0) || item->IsEquipped())
			ch->UnequipItem(equipped);

		//equipping the item to the given slot
		item->EquipTo(ch, bCell);
		lua_pushboolean(L, true);
		return 1;
	}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	ALUA(item_is_sealed)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			lua_pushnumber(L, q.GetCurrentItem()->IsSealed());
		}
		else
			lua_pushnumber(L, 0);
		return 1;
	}
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
	ALUA(item_is_transmulated)
	{
		CQuestManager& qMgr = CQuestManager::instance();
		LPITEM pkItem = qMgr.GetCurrentItem();
		if (pkItem)
		{
			if (pkItem->GetTransmutation() > 0)
				lua_pushboolean(L, true);
			else
				lua_pushboolean(L, false);
		}
		else
			lua_pushboolean(L, false);

		return 1;
	}

	ALUA(item_set_transmutation)
	{
		CQuestManager& qMgr = CQuestManager::instance();
		LPITEM pkItem = qMgr.GetCurrentItem();
		if ((pkItem) && (lua_isnumber(L, 1)))
		{
			DWORD dwTransmutation = (DWORD)lua_tonumber(L, 1);
			pkItem->SetTransmutation(dwTransmutation);
		}

		return 0;
	}

	ALUA(item_get_transmutation)
	{
		CQuestManager& qMgr = CQuestManager::instance();
		LPITEM pkItem = qMgr.GetCurrentItem();
		if (pkItem)
			lua_pushnumber(L, pkItem->GetTransmutation());
		else
			lua_pushnumber(L, 0);

		return 1;
	}
#endif

	ALUA(item_set_count)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		LPITEM item = CQuestManager::instance().GetCurrentItem();

		if (!item || !ch)
		{
			sys_err("No item selected or no character instance wtf?!");
			return 0;
		}

		if (!lua_isnumber(L, 1))
		{
			sys_err("Invalid argument.");
			return 0;
		}

		int count = (int)lua_tonumber(L, 1);
		if (count > g_bItemCountLimit)
		{
			sys_err("Item count overflowing.. (%d)", count);
			return 0;
		}

		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK) || !item->IsStackable())
			return 0;

		if (count > 0)
			item->SetCount(count);
		else
		{
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
			//ITEM_MANAGER::instance().RemoveItem(item);
		}

		return 0;
	}

#ifdef ENABLE_BASIC_ITEM_SYSTEM
	ALUA(item_is_basic)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			lua_pushnumber(L, q.GetCurrentItem()->IsBasicItem());
		}
		else
			lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(item_set_basic)
	{
		CQuestManager& q = CQuestManager::instance();
		LPITEM item = q.GetCurrentItem();
		bool is_basic = lua_toboolean(L, 1);
		if (item)
			item->SetBasic(is_basic);
		return 0;
	}
#endif

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
	ALUA(item_get_rare_points)
	{
		CQuestManager& q = CQuestManager::instance();

		if (q.GetCurrentItem())
		{
			lua_pushnumber(L, q.GetCurrentItem()->GetRarePoints());
		}
		else
			lua_pushnumber(L, 0);
		return 1;
	}
#endif

	void RegisterITEMFunctionTable()
	{
		luaL_reg item_functions[] =
		{
			{"get_id", item_get_id},
			{"get_cell", item_get_cell},
			{"select", item_select},
			{"select_cell", item_select_cell},
			{"select_cell_ex", item_select_cell_ex},
			{"remove", item_remove},
			{"get_socket", item_get_socket},
			{"set_socket", item_set_socket},
			{"get_vnum", item_get_vnum},
			{"has_flag", item_has_flag},
			{"get_value", item_get_value},
			{"set_value", item_set_value},
			{"get_name", item_get_name},
			{"get_size", item_get_size},
			{"get_count", item_get_count},
			{"get_type", item_get_type},
			{"get_sub_type", item_get_sub_type},
			{"get_refine_vnum", item_get_refine_vnum},
			{"get_level", item_get_level},
			{"next_refine_vnum", item_next_refine_vnum},
			{"get_level_limit", item_get_level_limit},
			{"start_realtime_expire", item_start_realtime_expire},
			{"copy_and_give_before_remove", item_copy_and_give_before_remove},
#ifdef ENABLE_GROWTH_PET_SYSTEM
			{"petdeath", item_pet_death},
			{"petrevive", item_pet_revive},
#endif
			{"get_flag", item_get_flag},
			{"get_wearflag", item_get_wearflag},
			{"get_antiflag", item_get_antiflag},
			{"has_antiflag", item_has_antiflag},
			{"get_refine_set", item_get_refine_set},
			{"get_limit", item_get_limit},
			{"get_apply", item_get_apply},
			{"get_applies", item_get_applies,},
			{"get_refine_materials", item_get_refine_materials},
			{"get_addon_type", item_get_addon_type},
			{"add_attribute", item_add_attribute},
			{"get_attribute", item_get_attribute},
			{"set_attribute", item_set_attribute},
			{"change_attribute", item_change_attribute},
			{"add_rare_attribute", item_add_rare_attribute},
			{"get_rare_attribute", item_get_rare_attribute},
			{"set_rare_attribute", item_set_rare_attribute},
			{"change_rare_attribute", item_change_rare_attribute},
			{"equip", item_equip_selected},
#ifdef ENABLE_CHANGELOOK_SYSTEM
			{"is_transmulated", item_is_transmulated},
			{"set_transmutation", item_set_transmutation},
			{"get_transmutation", item_get_transmutation},
#endif
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			{"is_sealed", item_is_sealed},
#endif
			{"set_count", item_set_count},
#ifdef ENABLE_BASIC_ITEM_SYSTEM
			{"is_basic", item_is_basic},
			{"set_basic", item_set_basic},
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
			{"get_rare_points", item_get_rare_points},
#endif
			{nullptr, nullptr}
		};
		CQuestManager::instance().AddLuaFunctionTable("item", item_functions);
	}
}