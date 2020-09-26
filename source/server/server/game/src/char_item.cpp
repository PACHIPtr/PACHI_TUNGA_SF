#include "stdafx.h"
#include <stack>
#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "item_manager.h"
#include "desc.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "packet.h"
#include "affect.h"
#include "skill.h"
#include "start_position.h"
#include "mob_manager.h"
#include "db.h"
#include "log.h"
#include "vector.h"
#include "buffer_manager.h"
#include "questmanager.h"
#include "fishing.h"
#include "party.h"
#include "dungeon.h"
#include "refine.h"
#include "unique_item.h"
#include "war_map.h"
#include "xmas_event.h"
#include "marriage.h"
#include "polymorph.h"
#include "blend_item.h"
#include "arena.h"
#include "config.h"
#include "questmanager.h"
#include "safebox.h"
#include "shop.h"
#include "pvp.h"
#include "battle.h"
#include "../../common/VnumHelper.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt.h"
#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "MeleyLair.h"
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
#include "PetSystem.h"
#endif
#include "inventory.h"
#include "guild.h"
#include "features.h"
const int ITEM_BROKEN_METIN_VNUM = 28960;
#define ERROR_MSG(exp, msg)  \
	if (true == (exp)) { \
			ChatPacket(CHAT_TYPE_INFO, msg); \
			return false; \
	}
#include "support_shaman.h"

const BYTE g_aBuffOnAttrPoints[] = { POINT_ENERGY, POINT_COSTUME_ATTR_BONUS };

struct FFindStone
{
	std::map<DWORD, LPCHARACTER> m_mapStone;

	void operator()(LPENTITY pEnt)
	{
		if (pEnt->IsType(ENTITY_CHARACTER) == true)
		{
			LPCHARACTER pChar = (LPCHARACTER)pEnt;

			if (pChar->IsStone() == true)
			{
				m_mapStone[(DWORD)pChar->GetVID()] = pChar;
			}
		}
	}
};

extern std::map<DWORD, DWORD> g_ShopIndexCount;
extern bool attr_always_add;
extern bool attr_always_5_add;
extern bool belt_allow_all_items;

//±ÍÈ¯ºÎ, ±ÍÈ¯±â¾ïºÎ, °áÈ¥¹ÝÁö
static bool IS_SUMMON_ITEM(int vnum)
{
	switch (vnum)
	{
	case 22000:
	case 22010:
	case 22011:
	case 22020:
	case ITEM_MARRIAGE_RING:
		return true;
	}

	return false;
}

static bool IS_MONKEY_DUNGEON(int map_index)
{
	switch (map_index)
	{
	case 5:
	case 25:
	case 45:
	case 108:
	case 109:
		return true;
	}

	return false;
}

bool IS_SUMMONABLE_ZONE(int map_index)
{
	// ¸ùÅ°´øÀü
	if (IS_MONKEY_DUNGEON(map_index))
		return false;
	// ¼º
	switch (map_index)
	{
	case 66: // »ç±ÍÅ¸¿ö
	case 71: // °Å¹Ì ´øÀü 2Ãþ
	case 72: // ÃµÀÇ µ¿±¼
	case 73: // ÃµÀÇ µ¿±¼ 2Ãþ
	case 193: // °Å¹Ì ´øÀü 2-1Ãþ
#if 0
	case 184: // ÃµÀÇ µ¿±¼(½Å¼ö)
	case 185: // ÃµÀÇ µ¿±¼ 2Ãþ(½Å¼ö)
	case 186: // ÃµÀÇ µ¿±¼(ÃµÁ¶)
	case 187: // ÃµÀÇ µ¿±¼ 2Ãþ(ÃµÁ¶)
	case 188: // ÃµÀÇ µ¿±¼(Áø³ë)
	case 189: // ÃµÀÇ µ¿±¼ 2Ãþ(Áø³ë)
#endif
//		case 206 : // ¾Æ±Íµ¿±¼
	case 216: // ¾Æ±Íµ¿±¼
	case 217: // °Å¹Ì ´øÀü 3Ãþ
	case 208: // ÃµÀÇ µ¿±¼ (¿ë¹æ)
	case 235:
	case 240:
	case 351:
	case 352:
	case 212:
	case 400:
		return false;
	}

	// ¸ðµç private ¸ÊÀ¸·Ð ¿öÇÁ ºÒ°¡´É
	if (map_index > 10000) return false;

	return true;
}

bool IS_BOTARYABLE_ZONE(int nMapIndex)
{
	if (LC_IsYMIR() == false && LC_IsKorea() == false) return true;

	switch (nMapIndex)
	{
	case 1:
	case 3:
	case 21:
	case 23:
	case 41:
	case 43:
		return true;
	}

	return false;
}

// item socket ÀÌ ÇÁ·ÎÅäÅ¸ÀÔ°ú °°ÀºÁö Ã¼Å© -- by mhh
static bool FN_check_item_socket(LPITEM item)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (item->GetSocket(i) != item->GetProto()->alSockets[i])
			return false;
	}

	return true;
}

// item socket º¹»ç -- by mhh
static void FN_copy_item_socket(LPITEM dest, LPITEM src)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		dest->SetSocket(i, src->GetSocket(i));
	}
}
static bool FN_check_item_sex(LPCHARACTER ch, LPITEM item)
{
	// ³²ÀÚ ±ÝÁö
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_MALE))
	{
		if (SEX_MALE == GET_SEX(ch))
			return false;
	}
	// ¿©ÀÚ±ÝÁö
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE))
	{
		if (SEX_FEMALE == GET_SEX(ch))
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ITEM HANDLING
/////////////////////////////////////////////////////////////////////////////
bool CHARACTER::CanHandleItem(bool bSkipCheckRefine, bool bSkipObserver)
{
	if (!bSkipObserver)
		if (m_bIsObserver)
			return false;

	if (GetMyShop())
		return false;

	if (!bSkipCheckRefine)
		if (m_bUnderRefine)
			return false;

	if (IsCubeOpen() || DragonSoul_RefineWindow_GetOpener() != nullptr)
		return false;

#ifdef ENABLE_CHANGELOOK_SYSTEM
	if (m_bChangeLook)
		return false;
#endif

#ifdef ENABLE_ACCE_SYSTEM
	if ((m_bSashCombination) || (m_bSashAbsorption))
		return false;
#endif

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (IsCombOpen())
		return false;
#endif

#ifdef ENABLE_AURA_SYSTEM
	if ((m_bAuraRefine) || (m_bAuraAbsorption))
		return false;
#endif

	if (IsWarping())
		return false;

	return true;
}

LPITEM CHARACTER::GetInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}
LPITEM CHARACTER::GetDragonSoulInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(DRAGON_SOUL_INVENTORY, wCell));
}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
LPITEM CHARACTER::GetSkillBookInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(SKILL_BOOK_INVENTORY, wCell));
}
LPITEM CHARACTER::GetUpgradeItemsInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(UPGRADE_ITEMS_INVENTORY, wCell));
}
LPITEM CHARACTER::GetStoneItemsInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(STONE_ITEMS_INVENTORY, wCell));
}
LPITEM CHARACTER::GetChestItemsInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(CHEST_ITEMS_INVENTORY, wCell));
}
LPITEM CHARACTER::GetAttrItemsInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(ATTR_ITEMS_INVENTORY, wCell));
}
LPITEM CHARACTER::GetFlowerItemsInventoryItem(WORD wCell) const
{
	if (!features::IsFeatureEnabled(features::SPECIAL_INVENTORY))
		return GetItem(TItemPos(INVENTORY, wCell));
	else
		return GetItem(TItemPos(FLOWER_ITEMS_INVENTORY, wCell));
}
#endif
LPITEM CHARACTER::GetItem(TItemPos Cell) const
{
	if (!IsValidItemPosition(Cell))
		return nullptr;
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	switch (window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pItems[wCell];
	case DRAGON_SOUL_INVENTORY:
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid DS item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pDSItems[wCell];
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	case SKILL_BOOK_INVENTORY:
		if (wCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pSkillBookItems[wCell];
	case UPGRADE_ITEMS_INVENTORY:
		if (wCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pUpgradeItems[wCell];
	case STONE_ITEMS_INVENTORY:
		if (wCell >= STONE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pStoneItems[wCell];
	case CHEST_ITEMS_INVENTORY:
		if (wCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pChestItems[wCell];
	case ATTR_ITEMS_INVENTORY:
		if (wCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pAttrItems[wCell];
	case FLOWER_ITEMS_INVENTORY:
		if (wCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return nullptr;
		}
		return m_pointsInstant.pFlowerItems[wCell];
#endif
	default:
		return nullptr;
	}
	return nullptr;
}

LPITEM CHARACTER::GetItem_NEW(const TItemPos & Cell) const
{
	CItem* cellItem = GetItem(Cell);
	if (cellItem)
		return cellItem;

	//There's no item in this cell, but that does not mean there is not an item which currently uses up this cell.
	uint16_t bCell = Cell.cell;
	uint8_t bPage = bCell / (4);

	for (int j = -5; j < 0; ++j)
	{
		uint8_t p = bCell + (5 * j);

		if (p / (4) != bPage)
			continue;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		if (p >= GetExtendInvenMax())
			continue;
#else
		if (p >= INVENTORY_MAX_NUM) // Eeh just for the sake of...
			continue;
#endif

		CItem * curCellItem = GetItem(TItemPos(INVENTORY, p));
		if (!curCellItem)
			continue;

		if (p + (curCellItem->GetSize() - 1) * 5 < Cell.cell) //Doesn't reach Cell.cell
			continue;

		return curCellItem;
	}

	return nullptr;
}

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem, bool isHighLight)
#else
void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem)
#endif
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	if ((unsigned long)((CItem*)pItem) == 0xff || (unsigned long)((CItem*)pItem) == 0xffffffff)
	{
		sys_err("!!! FATAL ERROR !!! item == 0xff (char: %s cell: %u)", GetName(), wCell);
		core_dump();
		return;
	}

	if (pItem && pItem->GetOwner())
	{
		assert(!"GetOwner exist");
		return;
	}
	// ±âº» ÀÎº¥Åä¸®
	switch (window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
	{
		if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pItems[wCell];

		if (pOld)
		{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
			if (wCell < GetExtendInvenMax())
#else
			if (wCell < INVENTORY_MAX_NUM)
#endif
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
					if (p >= GetExtendInvenMax())
						continue;
#else
					if (p >= INVENTORY_MAX_NUM)
						continue;
#endif

					if (m_pointsInstant.pItems[p] && m_pointsInstant.pItems[p] != pOld)
						continue;

					m_pointsInstant.bItemGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.bItemGrid[wCell] = 0;
		}

		if (pItem)
		{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
			if (wCell < GetExtendInvenMax())
#else
			if (wCell < INVENTORY_MAX_NUM)
#endif
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
					if (p >= GetExtendInvenMax())
						continue;
#else
					if (p >= INVENTORY_MAX_NUM)
						continue;
#endif

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.bItemGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.bItemGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pItems[wCell] = pItem;
	}
	break;

	case DRAGON_SOUL_INVENTORY:
	{
		LPITEM pOld = m_pointsInstant.pDSItems[wCell];

		if (pOld)
		{
			if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pDSItems[p] && m_pointsInstant.pDSItems[p] != pOld)
						continue;

					m_pointsInstant.wDSItemGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.wDSItemGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::SetItem: invalid DS item cell %d", wCell);
				return;
			}

			if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.wDSItemGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.wDSItemGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pDSItems[wCell] = pItem;
	}
	break;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	case SKILL_BOOK_INVENTORY:
	{
		if (wCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pSkillBookItems[wCell];

		if (pOld)
		{
			if (wCell < SKILL_BOOK_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pSkillBookItems[p] && m_pointsInstant.pSkillBookItems[p] != pOld)
						continue;

					m_pointsInstant.wSkillBookItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.wSkillBookItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < SKILL_BOOK_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.wSkillBookItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.wSkillBookItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pSkillBookItems[wCell] = pItem;
	}
	break;
	case UPGRADE_ITEMS_INVENTORY:
	{
		if (wCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pUpgradeItems[wCell];

		if (pOld)
		{
			if (wCell < UPGRADE_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pUpgradeItems[p] && m_pointsInstant.pUpgradeItems[p] != pOld)
						continue;

					m_pointsInstant.pUpgradeItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.pUpgradeItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < UPGRADE_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.pUpgradeItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.pUpgradeItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pUpgradeItems[wCell] = pItem;
	}
	break;
	case STONE_ITEMS_INVENTORY:
	{
		if (wCell >= STONE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pStoneItems[wCell];

		if (pOld)
		{
			if (wCell < STONE_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= STONE_ITEMS_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pStoneItems[p] && m_pointsInstant.pStoneItems[p] != pOld)
						continue;

					m_pointsInstant.pStoneItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.pStoneItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < STONE_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= STONE_ITEMS_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.pStoneItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.pStoneItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pStoneItems[wCell] = pItem;
	}
	break;
	
	case CHEST_ITEMS_INVENTORY:
	{
		if (wCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pChestItems[wCell];

		if (pOld)
		{
			if (wCell < CHEST_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pChestItems[p] && m_pointsInstant.pChestItems[p] != pOld)
						continue;

					m_pointsInstant.pChestItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.pChestItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < CHEST_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.pChestItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.pChestItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pChestItems[wCell] = pItem;
	}
	break;
	
	case ATTR_ITEMS_INVENTORY:
	{
		if (wCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pAttrItems[wCell];

		if (pOld)
		{
			if (wCell < ATTR_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pAttrItems[p] && m_pointsInstant.pAttrItems[p] != pOld)
						continue;

					m_pointsInstant.pAttrItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.pAttrItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < ATTR_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.pAttrItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.pAttrItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pAttrItems[wCell] = pItem;
	}
	break;
	
	case FLOWER_ITEMS_INVENTORY:
	{
		if (wCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pFlowerItems[wCell];

		if (pOld)
		{
			if (wCell < FLOWER_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pFlowerItems[p] && m_pointsInstant.pFlowerItems[p] != pOld)
						continue;

					m_pointsInstant.pFlowerItemsGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.pFlowerItemsGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < FLOWER_ITEMS_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 ·Î ÇÏ´Â °ÍÀº ºó°÷À» Ã¼Å©ÇÒ ¶§ °°Àº
					// ¾ÆÀÌÅÛÀº ¿¹¿ÜÃ³¸®ÇÏ±â À§ÇÔ
					m_pointsInstant.pFlowerItemsGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.pFlowerItemsGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pFlowerItems[wCell] = pItem;
	}
	break;
#endif

	default:
		sys_err("Invalid Inventory type %d", window_type);
		return;
	}

	if (GetDesc())
	{
		// È®Àå ¾ÆÀÌÅÛ: ¼­¹ö¿¡¼­ ¾ÆÀÌÅÛ ÇÃ·¡±× Á¤º¸¸¦ º¸³½´Ù
		if (pItem)
		{
			TPacketGCItemSet pack;
			pack.header = HEADER_GC_ITEM_SET;
			pack.Cell = Cell;

			pack.count = pItem->GetCount();
#ifdef ENABLE_CHANGELOOK_SYSTEM
			pack.transmutation = pItem->GetTransmutation();
#endif
			pack.vnum = pItem->GetVnum();
			pack.flags = pItem->GetFlag();
			pack.anti_flags = pItem->GetAntiFlag();
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
			if (isHighLight)
				pack.highlight = true;
			else
				pack.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#else
			pack.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#endif
			thecore_memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			pack.sealbind = pItem->GetSealBindTime();
#endif
			pack.is_basic = pItem->IsBasicItem();
			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));
		}
		else
		{
			TPacketGCItemDelDeprecated pack;
			pack.header = HEADER_GC_ITEM_DEL;
			pack.Cell = Cell;
			pack.count = 0;
#ifdef ENABLE_CHANGELOOK_SYSTEM
			pack.transmutation = 0;
#endif
			pack.vnum = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
			memset(pack.aAttr, 0, sizeof(pack.aAttr));

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemDelDeprecated));
		}
	}

	if (pItem)
	{
		pItem->SetCell(this, wCell);
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			if ((wCell < INVENTORY_MAX_NUM) || (BELT_INVENTORY_SLOT_START <= wCell && BELT_INVENTORY_SLOT_END > wCell))
				pItem->SetWindow(INVENTORY);
			else
				pItem->SetWindow(EQUIPMENT);
			break;
		case DRAGON_SOUL_INVENTORY:
			pItem->SetWindow(DRAGON_SOUL_INVENTORY);
			break;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		case SKILL_BOOK_INVENTORY:
			pItem->SetWindow(SKILL_BOOK_INVENTORY);
			break;
		case UPGRADE_ITEMS_INVENTORY:
			pItem->SetWindow(UPGRADE_ITEMS_INVENTORY);
			break;
		case STONE_ITEMS_INVENTORY:
			pItem->SetWindow(STONE_ITEMS_INVENTORY);
			break;
		case CHEST_ITEMS_INVENTORY:
			pItem->SetWindow(CHEST_ITEMS_INVENTORY);
			break;
		case ATTR_ITEMS_INVENTORY:
			pItem->SetWindow(ATTR_ITEMS_INVENTORY);
			break;
		case FLOWER_ITEMS_INVENTORY:
			pItem->SetWindow(FLOWER_ITEMS_INVENTORY);
			break;
#endif
		}
	}
}

LPITEM CHARACTER::GetWear(UINT bCell) const
{
	// > WEAR_MAX_NUM : ¿ëÈ¥¼® ½½·Ôµé.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::GetWear: invalid wear cell %d", bCell);
		return nullptr;
	}

	return m_pointsInstant.pItems[INVENTORY_MAX_NUM + bCell];
}

void CHARACTER::SetWear(UINT bCell, LPITEM item)
{
	// > WEAR_MAX_NUM : ¿ëÈ¥¼® ½½·Ôµé.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::SetItem: invalid item cell %d", bCell);
		return;
	}

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
	SetItem(TItemPos(INVENTORY, INVENTORY_MAX_NUM + bCell), item, false);
#else
	SetItem(TItemPos(INVENTORY, INVENTORY_MAX_NUM + bCell), item);
#endif

	if (!item && bCell == WEAR_WEAPON)
	{
		// ±Í°Ë »ç¿ë ½Ã ¹þ´Â °ÍÀÌ¶ó¸é È¿°ú¸¦ ¾ø¾Ö¾ß ÇÑ´Ù.
		if (IsAffectFlag(AFF_GWIGUM))
			RemoveAffect(SKILL_GWIGEOM);

		if (IsAffectFlag(AFF_GEOMGYEONG))
			RemoveAffect(SKILL_GEOMKYUNG);
	}
}

void CHARACTER::ClearItem()
{
	int		i;
	LPITEM	item;

	for (i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
	{
		if ((item = GetInventoryItem(i)))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);

			SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, 1000);
		}
	}
	for (i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(DRAGON_SOUL_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	for (i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(SKILL_BOOK_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}

	for (i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(UPGRADE_ITEMS_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}

	for (i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(STONE_ITEMS_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}

	for (i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(CHEST_ITEMS_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	
	for (i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(ATTR_ITEMS_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	
	for (i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i) {
		if ((item = GetItem(TItemPos(FLOWER_ITEMS_INVENTORY, i)))) {
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif
}

bool CHARACTER::IsEmptyItemGrid(TItemPos Cell, BYTE bSize, int iExceptionCell) const
{
	switch (Cell.window_type)
	{
	case INVENTORY:
	{
		UINT bCell = Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (Cell.IsBeltInventoryPosition())
		{
			LPITEM beltItem = GetWear(WEAR_BELT);

			if (nullptr == beltItem)
				return false;

			if (false == CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
				return false;

			if (m_pointsInstant.bItemGrid[bCell])
			{
				if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
					return true;

				return false;
			}

			if (bSize == 1)
				return true;
		}
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		else if (bCell >= GetExtendInvenMax())
			return false;
#else
		else if (bCell >= INVENTORY_MAX_NUM)
			return false;
#endif
		if (m_pointsInstant.bItemGrid[bCell])
		{
			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (INVENTORY_MAX_NUM / 4);

				do
				{
					UINT p = bCell + (5 * j);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
					if (p >= GetExtendInvenMax())
						return false;
#else
					if (p >= INVENTORY_MAX_NUM)
						return false;
#endif

					if (p / (INVENTORY_MAX_NUM / 4) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			BYTE bPage = bCell / (INVENTORY_MAX_NUM / 4);

			do
			{
				UINT p = bCell + (5 * j);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
				if (p >= GetExtendInvenMax())
					return false;
#else
				if (p >= INVENTORY_MAX_NUM)
					return false;
#endif

				if (p / (INVENTORY_MAX_NUM / 4) != bPage)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	case DRAGON_SOUL_INVENTORY:
	{
		WORD wCell = Cell.cell;
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			return false;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		iExceptionCell++;

		if (m_pointsInstant.wDSItemGrid[wCell])
		{
			if (m_pointsInstant.wDSItemGrid[wCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.wDSItemGrid[p])
						if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

				if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	case SKILL_BOOK_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.wSkillBookItemsGrid[bCell])
		{
			if (m_pointsInstant.wSkillBookItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.wSkillBookItemsGrid[p])
						if (m_pointsInstant.wSkillBookItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.wSkillBookItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	case UPGRADE_ITEMS_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pUpgradeItemsGrid[bCell])
		{
			if (m_pointsInstant.pUpgradeItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.pUpgradeItemsGrid[p])
						if (m_pointsInstant.pUpgradeItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.pUpgradeItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	case STONE_ITEMS_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= STONE_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pStoneItemsGrid[bCell])
		{
			if (m_pointsInstant.pStoneItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= STONE_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.pStoneItemsGrid[p])
						if (m_pointsInstant.pStoneItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= STONE_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.pStoneItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	
	case CHEST_ITEMS_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pChestItemsGrid[bCell])
		{
			if (m_pointsInstant.pChestItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.pChestItemsGrid[p])
						if (m_pointsInstant.pChestItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.pChestItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	
	case ATTR_ITEMS_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pAttrItemsGrid[bCell])
		{
			if (m_pointsInstant.pAttrItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.pAttrItemsGrid[p])
						if (m_pointsInstant.pAttrItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.pAttrItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	
	case FLOWER_ITEMS_INVENTORY:
	{
		WORD bCell = Cell.cell;

		++iExceptionCell;

		if (bCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pFlowerItemsGrid[bCell])
		{
			if (m_pointsInstant.pFlowerItemsGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.pFlowerItemsGrid[p])
						if (m_pointsInstant.pFlowerItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.pFlowerItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
#endif
	}
	return false;
}

bool CHARACTER::IsEmptyItemGridSpecial(const TItemPos & Cell, BYTE bSize, int iExceptionCell, std::vector<WORD> & vec) const
{
	if (std::find(vec.begin(), vec.end(), Cell.cell) != vec.end()) {
		return false;
	}

	switch (Cell.window_type)
	{
	case INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (Cell.IsBeltInventoryPosition())
		{
			LPITEM beltItem = GetWear(WEAR_BELT);

			if (nullptr == beltItem)
				return false;

			if (false == CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
				return false;

			if (m_pointsInstant.bItemGrid[bCell])
			{
				if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
					return true;

				return false;
			}

			if (bSize == 1)
				return true;
		}
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		else if (bCell >= GetExtendInvenMax())
			return false;
#else
		else if (bCell >= INVENTORY_MAX_NUM)
			return false;
#endif

		if (m_pointsInstant.bItemGrid[bCell])
		{
			if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (45);

				do
				{
					WORD p = bCell + (5 * j);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
					if (p >= GetExtendInvenMax())
						return false;
#else
					if (p >= INVENTORY_MAX_NUM)
						return false;
#endif

					if (p / (45) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (45);

			do
			{
				WORD p = bCell + (5 * j);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
				if (p >= GetExtendInvenMax())
					return false;
#else
				if (p >= INVENTORY_MAX_NUM)
					return false;
#endif

				if (p / (45) != bPage)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	case DRAGON_SOUL_INVENTORY:
	{
		WORD wCell = Cell.cell;
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			return false;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		iExceptionCell++;

		if (m_pointsInstant.wDSItemGrid[wCell])
		{
			if (m_pointsInstant.wDSItemGrid[wCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.wDSItemGrid[p])
						if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

				if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	case SKILL_BOOK_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.wSkillBookItemsGrid[bCell])
		{
			if (m_pointsInstant.wSkillBookItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (3);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
						return false;

					if (p / (3) != bPage)
						return false;

					if (m_pointsInstant.wSkillBookItemsGrid[p])
						if (m_pointsInstant.wSkillBookItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (3);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= SKILL_BOOK_INVENTORY_MAX_NUM)
					return false;

				if (p / (3) != bPage)
					return false;

				if (m_pointsInstant.wSkillBookItemsGrid[p])
					if (m_pointsInstant.wSkillBookItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	case UPGRADE_ITEMS_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pUpgradeItemsGrid[bCell])
		{
			if (m_pointsInstant.pUpgradeItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (3);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (p / (3) != bPage)
						return false;

					if (m_pointsInstant.pUpgradeItemsGrid[p])
						if (m_pointsInstant.pUpgradeItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (5);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (p / (5) != bPage)
					return false;

				if (m_pointsInstant.pUpgradeItemsGrid[p])
					if (m_pointsInstant.pUpgradeItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	case STONE_ITEMS_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pStoneItemsGrid[bCell])
		{
			if (m_pointsInstant.pStoneItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (5);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (p / (5) != bPage)
						return false;

					if (m_pointsInstant.pStoneItemsGrid[p])
						if (m_pointsInstant.pStoneItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (5);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (p / (5) != bPage)
					return false;

				if (m_pointsInstant.pStoneItemsGrid[p])
					if (m_pointsInstant.pStoneItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	case CHEST_ITEMS_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pChestItemsGrid[bCell])
		{
			if (m_pointsInstant.pChestItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (5);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (p / (5) != bPage)
						return false;

					if (m_pointsInstant.pChestItemsGrid[p])
						if (m_pointsInstant.pChestItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (5);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= CHEST_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (p / (5) != bPage)
					return false;

				if (m_pointsInstant.pChestItemsGrid[p])
					if (m_pointsInstant.pChestItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	
	case ATTR_ITEMS_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pAttrItemsGrid[bCell])
		{
			if (m_pointsInstant.pAttrItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (5);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (p / (5) != bPage)
						return false;

					if (m_pointsInstant.pAttrItemsGrid[p])
						if (m_pointsInstant.pAttrItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (5);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= ATTR_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (p / (5) != bPage)
					return false;

				if (m_pointsInstant.pAttrItemsGrid[p])
					if (m_pointsInstant.pAttrItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
	
	case FLOWER_ITEMS_INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCellÀº 0ÀÌ falseÀÓÀ» ³ªÅ¸³»±â À§ÇØ + 1 ÇØ¼­ Ã³¸®ÇÑ´Ù.
		// µû¶ó¼­ iExceptionCell¿¡ 1À» ´õÇØ ºñ±³ÇÑ´Ù.
		++iExceptionCell;

		if (bCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.pFlowerItemsGrid[bCell])
		{
			if (m_pointsInstant.pFlowerItemsGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (5);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
						return false;

					if (p / (5) != bPage)
						return false;

					if (m_pointsInstant.pFlowerItemsGrid[p])
						if (m_pointsInstant.pFlowerItemsGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// Å©±â°¡ 1ÀÌ¸é ÇÑÄ­À» Â÷ÁöÇÏ´Â °ÍÀÌ¹Ç·Î ±×³É ¸®ÅÏ
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (5);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
					return false;

				if (p / (5) != bPage)
					return false;

				if (m_pointsInstant.pFlowerItemsGrid[p])
					if (m_pointsInstant.pFlowerItemsGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;
#endif
	}
	return false;
}

int CHARACTER::GetEmptyInventory(BYTE size) const
{
	// NOTE: ÇöÀç ÀÌ ÇÔ¼ö´Â ¾ÆÀÌÅÛ Áö±Þ, È¹µæ µîÀÇ ÇàÀ§¸¦ ÇÒ ¶§ ÀÎº¥Åä¸®ÀÇ ºó Ä­À» Ã£±â À§ÇØ »ç¿ëµÇ°í ÀÖ´Âµ¥,
	//		º§Æ® ÀÎº¥Åä¸®´Â Æ¯¼ö ÀÎº¥Åä¸®ÀÌ¹Ç·Î °Ë»çÇÏÁö ¾Êµµ·Ï ÇÑ´Ù. (±âº» ÀÎº¥Åä¸®: INVENTORY_MAX_NUM ±îÁö¸¸ °Ë»ç)
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	for (int i = 0; i < GetExtendInvenMax(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;
	return -1;
}

int CHARACTER::GetEmptyDragonSoulInventory(LPITEM pItem) const
{
	if (nullptr == pItem || !pItem->IsDragonSoul())
		return -1;
	if (!DragonSoul_IsQualified())
	{
		return -1;
	}
	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (0xffff == wBaseCell)
		return -1;

	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
		if (IsEmptyItemGrid(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize))
			return i + wBaseCell;

	return -1;
}

int CHARACTER::GetEmptyDragonSoulInventoryWithExceptions(LPITEM pItem, std::vector<WORD> & vec /*= -1*/) const
{
	if (nullptr == pItem || !pItem->IsDragonSoul())
		return -1;
	if (!DragonSoul_IsQualified())
	{
		return -1;
	}
	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (0xffff == wBaseCell)
		return -1;

	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
		if (IsEmptyItemGridSpecial(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize, -1, vec))
			return i + wBaseCell;

	return -1;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
int CHARACTER::GetEmptySkillBookInventory(BYTE size) const
{
	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(SKILL_BOOK_INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyStoneInventory(BYTE size) const
{
	for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(STONE_ITEMS_INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyUpgradeItemsInventory(BYTE size) const
{
	for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(UPGRADE_ITEMS_INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyChestInventory(BYTE size) const
{
	for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(CHEST_ITEMS_INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyAttrInventory(BYTE size) const
{
	for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(ATTR_ITEMS_INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyFlowerInventory(BYTE size) const
{
	for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(FLOWER_ITEMS_INVENTORY, i), size))
			return i;

	return -1;
}
#endif

void CHARACTER::CopyDragonSoulItemGrid(std::vector<WORD> & vDragonSoulItemGrid) const
{
	vDragonSoulItemGrid.resize(DRAGON_SOUL_INVENTORY_MAX_NUM);

	std::copy(m_pointsInstant.wDSItemGrid, m_pointsInstant.wDSItemGrid + DRAGON_SOUL_INVENTORY_MAX_NUM, vDragonSoulItemGrid.begin());
}

int CHARACTER::CountEmptyInventory() const
{
	int	count = 0;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	for (int i = 0; i < GetExtendInvenMax(); ++i)
#else
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
		if (GetInventoryItem(i))
			count += GetInventoryItem(i)->GetSize();

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	return (GetExtendInvenMax() - count);
#else
	return (INVENTORY_MAX_NUM - count);
#endif
}

void TransformRefineItem(LPITEM pkOldItem, LPITEM pkNewItem)
{
	// ACCESSORY_REFINE
	if (pkOldItem->IsAccessoryForSocket())
	{
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			pkNewItem->SetSocket(i, pkOldItem->GetSocket(i));
		}
		//pkNewItem->StartAccessorySocketExpireEvent();
	}
	// END_OF_ACCESSORY_REFINE
	else
	{
		// ¿©±â¼­ ±úÁø¼®ÀÌ ÀÚµ¿ÀûÀ¸·Î Ã»¼Ò µÊ
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			if (!pkOldItem->GetSocket(i))
				break;
			else
				pkNewItem->SetSocket(i, 1);
		}

		// ¼ÒÄÏ ¼³Á¤
		int slot = 0;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			long socket = pkOldItem->GetSocket(i);

			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				pkNewItem->SetSocket(slot++, socket);
		}
	}

	// ¸ÅÁ÷ ¾ÆÀÌÅÛ ¼³Á¤
	pkOldItem->CopyAttributeTo(pkNewItem);
	pkNewItem->SetTransmutation(pkOldItem->GetTransmutation());
}

void NotifyRefineSuccess(LPCHARACTER ch, LPITEM item, const char* way)
{
	if (nullptr != ch && item != nullptr)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineSuceeded");
		ch->EffectPacket(SE_FR_SUCCESS);

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), 1, way);
	}
}

void NotifyRefineFail(LPCHARACTER ch, LPITEM item, const char* way, int success = 0)
{
	if (nullptr != ch && nullptr != item)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailed");
		ch->EffectPacket(SE_FR_FAIL);

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), success, way);
	}
}

#ifdef ENABLE_REFINE_MSG_ADD
void NotifyRefineFailType(const LPCHARACTER pkChr, const LPITEM pkItem, const BYTE bType, const std::string stRefineType, const BYTE bSuccess = 0)
{
	if (pkChr && pkItem)
	{
		pkChr->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailedType %d", bType);
		pkChr->EffectPacket(SE_FR_FAIL);
		LogManager::instance().RefineLog(pkChr->GetPlayerID(), pkItem->GetName(), pkItem->GetID(), pkItem->GetRefineLevel(), bSuccess, stRefineType.c_str());
	}
}
#endif

void CHARACTER::SetRefineNPC(LPCHARACTER ch)
{
	if (ch != nullptr)
	{
		m_dwRefineNPCVID = ch->GetVID();
	}
	else
	{
		m_dwRefineNPCVID = 0;
	}
}

bool CHARACTER::DoRefine(LPITEM item, bool bMoneyOnly)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	//°³·® ½Ã°£Á¦ÇÑ : upgrade_refine_scroll.quest ¿¡¼­ °³·®ÈÄ 5ºÐÀÌ³»¿¡ ÀÏ¹Ý °³·®À»
	//ÁøÇàÇÒ¼ö ¾øÀ½
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const TRefineTable* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());

	if (!prt)
		return false;

	DWORD result_vnum = item->GetRefinedVnum();

	// REFINE_COST
	int cost = ComputeRefineFee(prt->cost);

	int RefineChance = GetQuestFlag("main_quest_lv7.refine_chance");

	if (RefineChance > 0)
	{
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹«·á °³·® ±âÈ¸´Â 20 ÀÌÇÏÀÇ ¹«±â¸¸ °¡´ÉÇÕ´Ï´Ù"));
			return false;
		}

		cost = 0;
		SetQuestFlag("main_quest_lv7.refine_chance", RefineChance - 1);
	}
	// END_OF_REFINE_COST

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
		return false;

	TItemTable * pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefine NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (GetGold() < cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³·®À» ÇÏ±â À§ÇÑ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
		return false;
	}

	if (!bMoneyOnly && !RefineChance)
	{
		for (int i = 0; i < prt->material_count; ++i)
		{
			if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
			{
				if (test_server)
				{
					ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
				}
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³·®À» ÇÏ±â À§ÇÑ Àç·á°¡ ºÎÁ·ÇÕ´Ï´Ù."));
				return false;
			}
		}

		for (int i = 0; i < prt->material_count; ++i)
		{
			RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);
		}
	}

	int prob = number(1, 100);

	if (IsRefineThroughGuild() || bMoneyOnly)
		prob -= 10;

	int success_prob = prt->prob;

#ifdef ENABLE_VALUE_PACK_SYSTEM
	if (IsAffectFlag(AFF_PREMIUM))
		success_prob += 5;
#endif

	// END_OF_REFINE_COST

	if (prob <= success_prob)
	{
		// ¼º°ø! ¸ðµç ¾ÆÀÌÅÛÀÌ »ç¶óÁö°í, °°Àº ¼Ó¼ºÀÇ ´Ù¸¥ ¾ÆÀÌÅÛ È¹µæ
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			// DETAIL_REFINE_LOG
			NotifyRefineSuccess(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");
			// END_OF_DETAIL_REFINE_LOG
#ifdef ENABLE_ANNOUNCEMENT_REFINE_SUCCES
			if (pkNewItem->GetRefineLevel() >= ENABLE_ANNOUNCEMENT_REFINE_SUCCES_MIN_LEVEL && pkNewItem->GetLevelLimit() >= ENABLE_ANNOUNCEMENT_REFINE_LEVEL_LIMIT)
			{
				char szUpgradeAnnouncement[QUERY_MAX_LEN];
				snprintf(szUpgradeAnnouncement, sizeof(szUpgradeAnnouncement), "<Demirci> [%s] isimli oyuncu [%s] elde etti !", GetName(), pkNewItem->GetName());
				BroadcastNotice(szUpgradeAnnouncement);
			}
#endif

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			sys_log(0, "Refine Success %d", cost);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -cost);
			sys_log(0, "PayPee %d", cost);
			PayRefineFee(cost);
			sys_log(0, "PayPee End %d", cost);
		}
		else
		{
			// DETAIL_REFINE_LOG
			// ¾ÆÀÌÅÛ »ý¼º¿¡ ½ÇÆÐ -> °³·® ½ÇÆÐ·Î °£ÁÖ
			sys_err("cannot create item %u", result_vnum);
#ifdef ENABLE_REFINE_MSG_ADD
			NotifyRefineFailType(this, item, REFINE_FAIL_DEL_ITEM, IsRefineThroughGuild() ? "GUILD" : "POWER");
#else
			NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
#endif
			// END_OF_DETAIL_REFINE_LOG
		}
	}
	else
	{
		// ½ÇÆÐ! ¸ðµç ¾ÆÀÌÅÛÀÌ »ç¶óÁü.
		DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
#ifdef ENABLE_REFINE_MSG_ADD
		NotifyRefineFailType(this, item, REFINE_FAIL_DEL_ITEM, IsRefineThroughGuild() ? "GUILD" : "POWER");
#else
		NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
#endif
		item->AttrLog();
		ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

		//PointChange(POINT_GOLD, -cost);
		PayRefineFee(cost);
	}

	return true;
}

enum ERefineScrolls
{
	CHUKBOK_SCROLL = 0,
	HYUNIRON_CHN = 1, // Áß±¹¿¡¼­¸¸ »ç¿ë
	YONGSIN_SCROLL = 2,
	MUSIN_SCROLL = 3,
	YAGONG_SCROLL = 4,
	MEMO_SCROLL = 5,
	BDRAGON_SCROLL = 6,
	RITUALS_SCROLL = 11,
	SOUL_SCROLL = 12,
};

bool CHARACTER::DoRefineWithScroll(LPITEM item)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	ClearRefineMode();

	//°³·® ½Ã°£Á¦ÇÑ : upgrade_refine_scroll.quest ¿¡¼­ °³·®ÈÄ 5ºÐÀÌ³»¿¡ ÀÏ¹Ý °³·®À»
	//ÁøÇàÇÒ¼ö ¾øÀ½
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const TRefineTable* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());

	if (!prt)
		return false;

	LPITEM pkItemScroll;

	// °³·®¼­ Ã¼Å©
	if (m_iRefineAdditionalCell < 0)
		return false;

	pkItemScroll = GetInventoryItem(m_iRefineAdditionalCell);

	if (!pkItemScroll)
		return false;

	if (!(pkItemScroll->GetType() == ITEM_USE && pkItemScroll->GetSubType() == USE_TUNING))
		return false;

	if (pkItemScroll->GetVnum() == item->GetVnum())
		return false;

	DWORD result_vnum = item->GetRefinedVnum();
	DWORD result_fail_vnum = item->GetRefineFromVnum();

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	// MUSIN_SCROLL
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL)
	{
		if (item->GetRefineLevel() >= 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ °³·®¼­·Î ´õ ÀÌ»ó °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}
	// END_OF_MUSIC_SCROLL

	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		if (item->GetRefineLevel() != pkItemScroll->GetValue(1))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ °³·®¼­·Î °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		if (item->GetType() != ITEM_METIN || item->GetRefineLevel() != 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀ¸·Î °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL)
	{
		if (item->GetLevelLimit() < pkItemScroll->GetValue(1))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("you can only upgrade items above level %d"), pkItemScroll->GetValue(1));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == SOUL_SCROLL)
	{
		if (item->GetType() != ITEM_SOUL)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("you can only upgrade soul items"));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == CHUKBOK_SCROLL)
	{
		if (item->GetType() != ITEM_UNIQUE && item->GetSubType() != USE_CHARM)
		{
			if (item->GetRefineLevel() == 9)
				return false;
		}
	}

	TItemTable* pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefineWithScroll NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (GetGold() < prt->cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³·®À» ÇÏ±â À§ÇÑ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
		return false;
	}

	for (int i = 0; i < prt->material_count; ++i)
	{
		if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
		{
			if (test_server)
			{
				ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
			}
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³·®À» ÇÏ±â À§ÇÑ Àç·á°¡ ºÎÁ·ÇÕ´Ï´Ù."));
			return false;
		}
	}

	for (int i = 0; i < prt->material_count; ++i)
		RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);

	int prob = number(1, 100);
	int success_prob = prt->prob;
	bool bDestroyWhenFail = false;

	const char* szRefineType = "SCROLL";

	if (pkItemScroll->GetValue(0) == HYUNIRON_CHN ||
		pkItemScroll->GetValue(0) == YONGSIN_SCROLL ||
		pkItemScroll->GetValue(0) == YAGONG_SCROLL ||
		pkItemScroll->GetValue(0) == RITUALS_SCROLL ||
		pkItemScroll->GetValue(0) == SOUL_SCROLL) // ÇöÃ¶, ¿ë½ÅÀÇ Ãàº¹¼­, ¾ß°øÀÇ ºñÀü¼­  Ã³¸®
	{
		const char hyuniron_prob[9] = { 100, 75, 65, 55, 45, 40, 35, 25, 20 };

		const char yagong_prob[9] = { 100, 100, 90, 80, 70, 60, 50, 30, 20 };

		if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			success_prob = hyuniron_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			success_prob = yagong_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL)
		{
			success_prob += pkItemScroll->GetValue(2); //increases the success probability by Value(2)%
		}
		else if (pkItemScroll->GetValue(0) == SOUL_SCROLL)
		{
			success_prob += pkItemScroll->GetValue(1); //increases the success probability by Value(2)%
		}
		else if (pkItemScroll->GetValue(0) == HYUNIRON_CHN) {} // @fixme121
		else
		{
			sys_err("REFINE : Unknown refine scroll item. Value0: %d", pkItemScroll->GetValue(0));
		}

		if (test_server)
		{
			ChatPacket(CHAT_TYPE_INFO, "[Only Test] Success_Prob %d, RefineLevel %d ", success_prob, item->GetRefineLevel());
		}

		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN || pkItemScroll->GetValue(0) == RITUALS_SCROLL) // ÇöÃ¶Àº ¾ÆÀÌÅÛÀÌ ºÎ¼­Á®¾ß ÇÑ´Ù.
			bDestroyWhenFail = true;

		// DETAIL_REFINE_LOG
		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN)
		{
			szRefineType = "HYUNIRON";
		}
		else if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			szRefineType = "GOD_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			szRefineType = "YAGONG_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL)
		{
			szRefineType = "RITUALS_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == SOUL_SCROLL)
		{
			szRefineType = "SOUL_SCROLL";
		}
		// END_OF_DETAIL_REFINE_LOG
	}

	// DETAIL_REFINE_LOG
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL) // ¹«½ÅÀÇ Ãàº¹¼­´Â 100% ¼º°ø (+4±îÁö¸¸)
	{
		success_prob = 100;

		szRefineType = "MUSIN_SCROLL";
	}
	// END_OF_DETAIL_REFINE_LOG
	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		success_prob = 100;
		szRefineType = "MEMO_SCROLL";
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		success_prob = 80;
		szRefineType = "BDRAGON_SCROLL";
	}

	pkItemScroll->SetCount(pkItemScroll->GetCount() - 1);

#ifdef ENABLE_VALUE_PACK_SYSTEM
	if (IsAffectFlag(AFF_PREMIUM))
		success_prob += 5;
#endif

	if (prob <= success_prob)
	{
		// ¼º°ø! ¸ðµç ¾ÆÀÌÅÛÀÌ »ç¶óÁö°í, °°Àº ¼Ó¼ºÀÇ ´Ù¸¥ ¾ÆÀÌÅÛ È¹µæ
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			if (pkNewItem->GetAttributeValue(5) > 9)
				pkNewItem->SetAttribute2(14, APPLY_NONE, 0);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			NotifyRefineSuccess(this, item, szRefineType);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");
#ifdef ENABLE_ANNOUNCEMENT_REFINE_SUCCES
			if (pkNewItem->GetRefineLevel() >= ENABLE_ANNOUNCEMENT_REFINE_SUCCES_MIN_LEVEL && pkNewItem->GetLevelLimit() >= ENABLE_ANNOUNCEMENT_REFINE_LEVEL_LIMIT)
			{
				char szUpgradeAnnouncement[QUERY_MAX_LEN];
				snprintf(szUpgradeAnnouncement, sizeof(szUpgradeAnnouncement), "<Demirci> [%s] isimli oyuncu [%s] elde etti !", GetName(), pkNewItem->GetName());
				BroadcastNotice(szUpgradeAnnouncement);
			}
#endif
			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));

			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// ¾ÆÀÌÅÛ »ý¼º¿¡ ½ÇÆÐ -> °³·® ½ÇÆÐ·Î °£ÁÖ
			sys_err("cannot create item %u", result_vnum);
#ifdef ENABLE_REFINE_MSG_ADD
			NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
			NotifyRefineFail(this, item, szRefineType);
#endif
		}
	}
	else if (!bDestroyWhenFail && result_fail_vnum)
	{
		// ½ÇÆÐ! ¸ðµç ¾ÆÀÌÅÛÀÌ »ç¶óÁö°í, °°Àº ¼Ó¼ºÀÇ ³·Àº µî±ÞÀÇ ¾ÆÀÌÅÛ È¹µæ
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_fail_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE FAIL", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
#ifdef ENABLE_REFINE_MSG_ADD
			NotifyRefineFailType(this, item, REFINE_FAIL_GRADE_DOWN, szRefineType, -1);
#else
			NotifyRefineFail(this, item, szRefineType, -1);
#endif
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			pkNewItem->AttrLog();

			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// ¾ÆÀÌÅÛ »ý¼º¿¡ ½ÇÆÐ -> °³·® ½ÇÆÐ·Î °£ÁÖ
			sys_err("cannot create item %u", result_fail_vnum);
#ifdef ENABLE_REFINE_MSG_ADD
			NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
			NotifyRefineFail(this, item, szRefineType);
#endif
		}
	}
	else
	{
#ifdef ENABLE_REFINE_MSG_ADD
		NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
		NotifyRefineFail(this, item, szRefineType);
#endif
		PayRefineFee(prt->cost);
	}

	return true;
}

bool CHARACTER::RefineInformation(UINT bCell, BYTE bType, int iAdditionalCell)
{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (bCell > GetExtendInvenMax())
		return false;
#else
	if (bCell > INVENTORY_MAX_NUM)
		return false;
#endif

	LPITEM item = GetInventoryItem(bCell);

	if (!item)
		return false;

	// REFINE_COST
	if (bType == REFINE_TYPE_MONEY_ONLY && !GetQuestFlag("deviltower_zone.can_refine"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç±Í Å¸¿ö ¿Ï·á º¸»óÀº ÇÑ¹ø±îÁö »ç¿ë°¡´ÉÇÕ´Ï´Ù."));
		return false;
	}
	// END_OF_REFINE_COST

	TPacketGCRefineInformation p;

	p.header = HEADER_GC_REFINE_INFORMATION;
	p.pos = bCell;
	p.src_vnum = item->GetVnum();
	p.result_vnum = item->GetRefinedVnum();
	p.type = bType;

	if (p.result_vnum == 0)
	{
		sys_err("RefineInformation p.result_vnum == 0");
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
	{
		if (bType == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº ÀÌ ¹æ½ÄÀ¸·Î´Â °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
		else
		{
			LPITEM itemScroll = GetInventoryItem(iAdditionalCell);
			if (!itemScroll || item->GetVnum() == itemScroll->GetVnum())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°°Àº °³·®¼­¸¦ ÇÕÄ¥ ¼ö´Â ¾ø½À´Ï´Ù."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãàº¹ÀÇ ¼­¿Í ÇöÃ¶À» ÇÕÄ¥ ¼ö ÀÖ½À´Ï´Ù."));
				return false;
			}
		}
	}

	CRefineManager& rm = CRefineManager::instance();

	const TRefineTable* prt = rm.GetRefineRecipe(item->GetRefineSet());

	if (!prt)
	{
		sys_err("RefineInformation NOT GET REFINE SET %d", item->GetRefineSet());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	// REFINE_COST

	//MAIN_QUEST_LV7
	if (GetQuestFlag("main_quest_lv7.refine_chance") > 0)
	{
		// ÀÏº»Àº Á¦¿Ü
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹«·á °³·® ±âÈ¸´Â 20 ÀÌÇÏÀÇ ¹«±â¸¸ °¡´ÉÇÕ´Ï´Ù"));
			return false;
		}
		p.cost = 0;
	}
	else
		p.cost = ComputeRefineFee(prt->cost);

	//END_MAIN_QUEST_LV7
	p.prob = prt->prob;
	if (bType == REFINE_TYPE_MONEY_ONLY)
	{
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
	else
	{
		p.material_count = prt->material_count;
		thecore_memcpy(&p.materials, prt->materials, sizeof(prt->materials));
	}
	// END_OF_REFINE_COST

	GetDesc()->Packet(&p, sizeof(TPacketGCRefineInformation));

	SetRefineMode(iAdditionalCell);
	return true;
}

bool CHARACTER::RefineItem(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;

	if (pkItem->GetSubType() == USE_TUNING)
	{
		// XXX ¼º´É, ¼ÒÄÏ °³·®¼­´Â »ç¶óÁ³½À´Ï´Ù...
		// XXX ¼º´É°³·®¼­´Â Ãàº¹ÀÇ ¼­°¡ µÇ¾ú´Ù!
		// MUSIN_SCROLL
		if (pkItem->GetValue(0) == MUSIN_SCROLL)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell());
		// END_OF_MUSIN_SCROLL
		else if (pkItem->GetValue(0) == HYUNIRON_CHN)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_HYUNIRON, pkItem->GetCell());
		else if (pkItem->GetValue(0) == BDRAGON_SCROLL)
		{
			if (pkTarget->GetRefineSet() != 702) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell());
		}
		else if (pkTarget->GetValue(0) == RITUALS_SCROLL)
		{
			if (pkTarget->GetLevelLimit() <= pkItem->GetValue(1)) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_RITUALS_SCROLL, pkItem->GetCell());
		}
		else if (pkTarget->GetValue(0) == SOUL_SCROLL)
		{
			if (pkTarget->GetType() != ITEM_SOUL) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SOUL_SCROLL, pkItem->GetCell());
		}
		else
		{
			if (pkTarget->GetRefineSet() == 501) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell());
		}
	}
	else if (pkItem->GetSubType() == USE_DETACHMENT && IS_SET(pkTarget->GetFlag(), ITEM_FLAG_REFINEABLE))
	{
		LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT", pkTarget->GetName());

		bool bHasMetinStone = false;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
		{
			long socket = pkTarget->GetSocket(i);
			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
			{
				bHasMetinStone = true;
				break;
			}
		}

		if (bHasMetinStone)
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				long socket = pkTarget->GetSocket(i);
				if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				{
					AutoGiveItem(socket);
					//TItemTable* pTable = ITEM_MANAGER::instance().GetTable(pkTarget->GetSocket(i));
					//pkTarget->SetSocket(i, pTable->alValues[2]);
					// ±úÁøµ¹·Î ´ëÃ¼ÇØÁØ´Ù
					pkTarget->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}
			}
			pkItem->SetCount(pkItem->GetCount() - 1);
			return true;
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»©³¾ ¼ö ÀÖ´Â ¸ÞÆ¾¼®ÀÌ ¾ø½À´Ï´Ù."));
			return false;
		}
	}

	return false;
}

EVENTFUNC(kill_campfire_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("kill_campfire_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}
	ch->m_pkMiningEvent = nullptr;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

bool CHARACTER::GiveRecallItem(LPITEM item)
{
	int idx = GetMapIndex();
	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±â¾ïÇØ µÑ ¼ö ¾ø´Â À§Ä¡ ÀÔ´Ï´Ù."));
		return false;
	}

	int pos;

	if (item->GetCount() == 1)	// ¾ÆÀÌÅÛÀÌ ÇÏ³ª¶ó¸é ±×³É ¼ÂÆÃ.
	{
		item->SetSocket(0, GetX());
		item->SetSocket(1, GetY());
	}
	else if ((pos = GetEmptyInventory(item->GetSize())) != -1) // ±×·¸Áö ¾Ê´Ù¸é ´Ù¸¥ ÀÎº¥Åä¸® ½½·ÔÀ» Ã£´Â´Ù.
	{
		LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);

		if (nullptr != item2)
		{
			item2->SetSocket(0, GetX());
			item2->SetSocket(1, GetY());
			item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

			item->SetCount(item->GetCount() - 1);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
		return false;
	}

	return true;
}

void CHARACTER::ProcessRecallItem(LPITEM item)
{
	int idx;

	if ((idx = SECTREE_MANAGER::instance().GetMapIndex(item->GetSocket(0), item->GetSocket(1))) == 0)
		return;

	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
		// ¾Ç·æ±ºµµ ÀÏ¶§
	case 301:
	case 302:
	case 303:
	case 304:
		if (GetLevel() < 90)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛÀÇ ·¹º§ Á¦ÇÑº¸´Ù ·¹º§ÀÌ ³·½À´Ï´Ù."));
			return;
		}
		else
			break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±â¾ïµÈ À§Ä¡°¡ Å¸Á¦±¹¿¡ ¼ÓÇØ ÀÖ¾î¼­ ±ÍÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		item->SetSocket(0, 0);
		item->SetSocket(1, 0);
	}
	else
	{
		sys_log(1, "Recall: %s %d %d -> %d %d", GetName(), GetX(), GetY(), item->GetSocket(0), item->GetSocket(1));
		WarpSet(item->GetSocket(0), item->GetSocket(1));
		item->SetCount(item->GetCount() - 1);
	}
}

void CHARACTER::__OpenPrivateShop()
{
	ChatPacket(CHAT_TYPE_COMMAND, "OpenPrivateShop");
}

// MYSHOP_PRICE_LIST
#ifdef ENABLE_CHEQUE_SYSTEM
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, TItemPriceType ItemPrice)
#else
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, DWORD dwItemPrice)
#endif
{
	char szLine[256];
#ifdef ENABLE_CHEQUE_SYSTEM
	snprintf(szLine, sizeof(szLine), "MyShopPriceListNew %u %u %u", dwItemVnum, ItemPrice.dwPrice, ItemPrice.byChequePrice);
#else
	snprintf(szLine, sizeof(szLine), "MyShopPriceList %u %u", dwItemVnum, dwItemPrice);
#endif
	ChatPacket(CHAT_TYPE_COMMAND, szLine);
	sys_log(0, szLine);
}

//
// DB Ä³½Ã·Î ºÎÅÍ ¹ÞÀº ¸®½ºÆ®¸¦ User ¿¡°Ô Àü¼ÛÇÏ°í »óÁ¡À» ¿­¶ó´Â Ä¿¸Çµå¸¦ º¸³½´Ù.
//
void CHARACTER::UseSilkBotaryReal(const TPacketMyshopPricelistHeader * p)
{
	const TItemPriceInfo* pInfo = (const TItemPriceInfo*)(p + 1);

	if (!p->byCount)
		// °¡°Ý ¸®½ºÆ®°¡ ¾ø´Ù. dummy µ¥ÀÌÅÍ¸¦ ³ÖÀº Ä¿¸Çµå¸¦ º¸³»ÁØ´Ù.
#ifdef ENABLE_CHEQUE_SYSTEM
		SendMyShopPriceListCmd(1, TItemPriceType());
#else
		SendMyShopPriceListCmd(1, 0);
#endif
	else {
		for (int idx = 0; idx < p->byCount; idx++)
#ifdef ENABLE_CHEQUE_SYSTEM
			SendMyShopPriceListCmd(pInfo[idx].dwVnum, TItemPriceType(pInfo[idx].price.dwPrice, pInfo[idx].price.byChequePrice));
#else
			SendMyShopPriceListCmd(pInfo[idx].dwVnum, pInfo[idx].dwPrice);
#endif
	}

	__OpenPrivateShop();
}

//
// ÀÌ¹ø Á¢¼Ó ÈÄ Ã³À½ »óÁ¡À» Open ÇÏ´Â °æ¿ì ¸®½ºÆ®¸¦ Load ÇÏ±â À§ÇØ DB Ä³½Ã¿¡ °¡°ÝÁ¤º¸ ¸®½ºÆ® ¿äÃ» ÆÐÅ¶À» º¸³½´Ù.
// ÀÌÈÄºÎÅÍ´Â ¹Ù·Î »óÁ¡À» ¿­¶ó´Â ÀÀ´äÀ» º¸³½´Ù.
//
void CHARACTER::UseSilkBotary(void)
{
	if (m_bNoOpenedShop) {
		DWORD dwPlayerID = GetPlayerID();
		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_REQ, GetDesc()->GetHandle(), &dwPlayerID, sizeof(DWORD));
		m_bNoOpenedShop = false;
	}
	else {
		__OpenPrivateShop();
	}
}
// END_OF_MYSHOP_PRICE_LIST

int CalculateConsume(LPCHARACTER ch)
{
	static const int WARP_NEED_LIFE_PERCENT = 30;
	static const int WARP_MIN_LIFE_PERCENT = 10;
	// CONSUME_LIFE_WHEN_USE_WARP_ITEM
	int consumeLife = 0;
	{
		// CheckNeedLifeForWarp
		const int curLife = ch->GetHP();
		const int needPercent = WARP_NEED_LIFE_PERCENT;
		const int needLife = ch->GetMaxHP() * needPercent / 100;
		if (curLife < needLife)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³²Àº »ý¸í·Â ¾çÀÌ ¸ðÀÚ¶ó »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return -1;
		}

		consumeLife = needLife;

		// CheckMinLifeForWarp: µ¶¿¡ ÀÇÇØ¼­ Á×À¸¸é ¾ÈµÇ¹Ç·Î »ý¸í·Â ÃÖ¼Ò·®´Â ³²°ÜÁØ´Ù
		const int minPercent = WARP_MIN_LIFE_PERCENT;
		const int minLife = ch->GetMaxHP() * minPercent / 100;
		if (curLife - needLife < minLife)
			consumeLife = curLife - minLife;

		if (consumeLife < 0)
			consumeLife = 0;
	}
	// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM
	return consumeLife;
}

int CalculateConsumeSP(LPCHARACTER lpChar)
{
	static const int NEED_WARP_SP_PERCENT = 30;

	const int curSP = lpChar->GetSP();
	const int needSP = lpChar->GetMaxSP() * NEED_WARP_SP_PERCENT / 100;

	if (curSP < needSP)
	{
		lpChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³²Àº Á¤½Å·Â ¾çÀÌ ¸ðÀÚ¶ó »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return -1;
	}

	return needSP;
}

#ifdef ENABLE_BLEND_ICON_SYSTEM
bool CHARACTER::SetAffectPotion(LPITEM item)
{
	if (!item)
		return false;

	int blend_get_affect[] = {AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9, AFFECT_POTION_10, AFFECT_POTION_11, AFFECT_POTION_12, AFFECT_POTION_13};
  
	int blend_null[] = {APPLY_NONE, AFF_NONE, 0, false};
  
	int blend_list[] = {50821, 50822, 50823, 50824, 50825, 50826, 50827, 50828, 50829, 50830, 50831, 50832, 50833};
  
   // const char* blend_succes = {"<Affect Potion> Set icon ingame for item: |cFFc9ff00|H|h[%s]"};
	//ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SEBNEM_KULLANILDI")); 
	int    blend_time    = item->GetSocket(2);
	if (blend_time <= 0)
		blend_time = 900;
  
	switch (item->GetVnum())
	{
		case 50821:
			AddAffect(AFFECT_POTION_1, blend_null[0], blend_null[2], AFF_POTION_1, blend_time, 0, true);
			break;
		case 50822:
			AddAffect(AFFECT_POTION_2, blend_null[0], blend_null[2], AFF_POTION_2, blend_time, 0, true);
			break;
		case 50823:
			AddAffect(AFFECT_POTION_3, blend_null[0], blend_null[2], AFF_POTION_3, blend_time, 0, true);
			break;
		case 50824:
			AddAffect(AFFECT_POTION_4, blend_null[0], blend_null[2], AFF_POTION_4, blend_time, 0, true);
			break; 
		case 50825:
			AddAffect(AFFECT_POTION_5, blend_null[0], blend_null[2], AFF_POTION_5, blend_time, 0, true);
			break; 
		case 50826:
			AddAffect(AFFECT_POTION_6, blend_null[0], blend_null[2], AFF_POTION_6, blend_time, 0, true);
			break; 
		case 50827:
			AddAffect(AFFECT_POTION_7, blend_null[0], blend_null[2], AFF_POTION_7, blend_time, 0, true);
			break; 
		case 50828:
			AddAffect(AFFECT_POTION_8, blend_null[0], blend_null[2], AFF_POTION_8, blend_time, 0, true);
			break; 
		case 50829:
			AddAffect(AFFECT_POTION_9, blend_null[0], blend_null[2], AFF_POTION_9, blend_time, 0, true);
			break; 
		case 50830:
			AddAffect(AFFECT_POTION_10, blend_null[0], blend_null[2], AFF_POTION_10, blend_time, 0, true);
			break; 
		case 50831:
			AddAffect(AFFECT_POTION_11, blend_null[0], blend_null[2], AFF_POTION_11, blend_time, 0, true);
			break; 
		case 50832:
			AddAffect(AFFECT_POTION_12, blend_null[0], blend_null[2], AFF_POTION_12, blend_time, 0, true);
			break; 
		case 50833:
			AddAffect(AFFECT_POTION_13, blend_null[0], blend_null[2], AFF_POTION_13, blend_time, 0, true);
			break; 
	}
		//ChatPacket(CHAT_TYPE_INFO, blend_succes, item->GetName()); 
}
#endif

bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell)
{
	int iLimitRealtimeStartFirstUseFlagIndex = -1;
	int iLimitTimerBasedOnWearFlagIndex = -1;

	WORD wDestCell = DestCell.cell;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limitValue = item->GetProto()->aLimits[i].lValue;

		switch (item->GetProto()->aLimits[i].bType)
		{
		case LIMIT_LEVEL:
			if (GetLevel() < limitValue)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛÀÇ ·¹º§ Á¦ÇÑº¸´Ù ·¹º§ÀÌ ³·½À´Ï´Ù."));
				return false;
			}
			break;

		case LIMIT_REAL_TIME_START_FIRST_USE:
			iLimitRealtimeStartFirstUseFlagIndex = i;
			break;

		case LIMIT_TIMER_BASED_ON_WEAR:
			iLimitTimerBasedOnWearFlagIndex = i;
			break;
		}
	}

#ifdef ENABLE_BLOCK_ITEMS_ON_WAR_MAP
	if ((item->GetVnum() == 71018 || item->GetVnum() == 71019 || item->GetVnum() == 71020) && (GetWarMap()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
		return false;
	}
#endif

	if (test_server)
	{
		//sys_log(0, "USE_ITEM %s, Inven %d, Cell %d, ItemType %d, SubType %d", item->GetName(), bDestInven, wDestCell, item->GetType(), item->GetSubType());
	}

	if (CArenaManager::instance().IsLimitedItem(GetMapIndex(), item->GetVnum()) == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
		return false;
	}

	if (!IsLoadedAffect())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Is Not Load Affect."));
		return false;
	}

	if (TItemPos(item->GetWindow(), item->GetCell()).IsBeltInventoryPosition())
	{
		LPITEM beltItem = GetWear(WEAR_BELT);

		if (nullptr == beltItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Belt> You can't use this item if you have no equipped belt."));
			return false;
		}

		if (false == CBeltInventoryHelper::IsAvailableCell(item->GetCell() - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Belt> You can't use this item if you don't upgrade your belt."));
			return false;
		}
	}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (TItemPos(item->GetWindow(), item->GetCell()).IsExtendInventoryPosition())
	{
		if (false == IsAvailableCell(item->GetCell() - EXTEND_INVENTORY_SLOT_START, m_points.extend_inventory))
		{
			ChatPacket(CHAT_TYPE_INFO, "<Extend Inventory> You can't use this item if you don't upgrade your inventory.");
			return false;
		}
	}
#endif

	// ¾ÆÀÌÅÛ ÃÖÃÊ »ç¿ë ÀÌÈÄºÎÅÍ´Â »ç¿ëÇÏÁö ¾Ê¾Æµµ ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä Ã³¸®.
	if (-1 != iLimitRealtimeStartFirstUseFlagIndex)
	{
		// ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇÑ ¾ÆÀÌÅÛÀÎÁö ¿©ºÎ´Â Socket1À» º¸°í ÆÇ´ÜÇÑ´Ù. (Socket1¿¡ »ç¿ëÈ½¼ö ±â·Ï)
		if (0 == item->GetSocket(1))
		{
			// »ç¿ë°¡´É½Ã°£Àº Default °ªÀ¸·Î Limit Value °ªÀ» »ç¿ëÇÏµÇ, Socket0¿¡ °ªÀÌ ÀÖÀ¸¸é ±× °ªÀ» »ç¿ëÇÏµµ·Ï ÇÑ´Ù. (´ÜÀ§´Â ÃÊ)
			long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[iLimitRealtimeStartFirstUseFlagIndex].lValue;

			if (0 == duration)
				duration = 60 * 60 * 24 * 7;

			item->SetSocket(0, time(0) + duration);
			item->StartRealTimeExpireEvent();
		}

		if (false == item->IsEquipped())
			item->SetSocket(1, item->GetSocket(1) + 1);
	}
#ifdef ENABLE_GROWTH_PET_SYSTEM

	if (item->GetVnum() == 55001) {
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetVnum() > 55710 || item2->GetVnum() < 55701)
			return false;

		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "SELECT duration,tduration FROM new_petsystem WHERE id = %u ", item2->GetID());
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
		if (pmsg2->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
			int suankiDuration = atoi(row[0]);

			if(suankiDuration >= get_global_time())
			{
				ChatPacket(CHAT_TYPE_INFO,LC_TEXT("PET_ALREADY_YOUNG"));
				return false;
			}

			int insertduration0 = time(0) + atoi(row[1]);
			int insertduration1 = time(0) + (atoi(row[2]) / 2);
			if (atoi(row[0]) > 0)
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %d", insertduration0, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D1"));
			}
			else
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %d", insertduration1, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D2"));
			}

			item->SetCount(item->GetCount() - 1);
			return true;
		}
		else
			return false;
	}

	if (item->GetVnum() >= 55701 && item->GetVnum() <= 55710) {
		LPITEM item2 = GetItem(DestCell);

		if (item2) {
			if (item2->GetVnum() == 55002) {
				if (item2->GetAttributeValue(0) > 0) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zaten iceride bir hayvan kutusu bulunuyor."));
				}
				else {
					if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
						return false;

					if (item2->IsExchanging())
						return false;

					if (GetNewPetSystem()->IsActivePet()) {
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once diger pet'i gonder."));
						return false;
					}

					for (int b = 0; b < 3; b++) {
						item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
					}

					item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
					item2->SetForceAttribute(6, 1, item->GetAttributeValue(6));
					item2->SetForceAttribute(7, 1, item->GetAttributeValue(7));
					item2->SetForceAttribute(8, 1, item->GetAttributeValue(8));
					item2->SetForceAttribute(9, 1, item->GetAttributeValue(9));
					item2->SetForceAttribute(10, 1, item->GetAttributeValue(10));
					item2->SetForceAttribute(11, 1, item->GetAttributeValue(11));
					item2->SetForceAttribute(12, 1, item->GetAttributeValue(12));
					item2->SetForceAttribute(13, 1, item->GetAttributeValue(4));
					DWORD vnum1 = item->GetVnum() - 55700;
					item2->SetSocket(0, vnum1);
					item2->SetSocket(2, item->GetSocket(2));
					item2->SetSocket(1, item->GetSocket(1));
					item2->SetSocket(3, item->GetSocket(3));
					//ChatPacket(CHAT_TYPE_INFO, "Pet %d %d %d //// %d %d %d",item->GetAttributeValue(0),item->GetAttributeValue(1),item->GetAttributeValue(2),item2->GetAttributeValue(0),item2->GetAttributeValue(1),item2->GetAttributeValue(2));
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item2->GetID(), item->GetID()));
					ITEM_MANAGER::instance().RemoveItem(item);
					return true;
				}
			}
		}
	}

	if (item->GetVnum() == 55002 && item->GetAttributeValue(0) > 0) {
		int pos = GetEmptyInventory(item->GetSize());
		if (pos == -1)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli alan yok!"));
			return false;
		}

		if (item->IsExchanging())
			return false;
		DWORD vnum2 = 55700 + item->GetSocket(0);
		LPITEM item2 = AutoGiveItem(vnum2, 1);
		for (int b = 0; b < 3; b++) {
			item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
		}
		item2->SetForceAttribute(3, 1, item->GetAttributeValue(3));
		item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
		item2->SetForceAttribute(6, 1, item->GetAttributeValue(6));
		item2->SetForceAttribute(7, 1, item->GetAttributeValue(7));
		item2->SetForceAttribute(8, 1, item->GetAttributeValue(8));
		item2->SetForceAttribute(9, 1, item->GetAttributeValue(9));
		item2->SetForceAttribute(10, 1, item->GetAttributeValue(10));
		item2->SetForceAttribute(11, 1, item->GetAttributeValue(11));
		item2->SetForceAttribute(12, 1, item->GetAttributeValue(12));
		item2->SetSocket(1, item->GetSocket(1));
		item2->SetSocket(2, item->GetSocket(2));
		item2->SetSocket(3, item->GetSocket(3));
		//ChatPacket(CHAT_TYPE_INFO, "Pet1 %d %d %d",item->GetAttributeValue(0),item->GetAttributeValue(1),item->GetAttributeValue(2));
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item2->GetID(), item->GetID()));
		ITEM_MANAGER::instance().RemoveItem(item);
		return true;
	}
#endif
	switch (item->GetType())
	{
	case ITEM_HAIR:
		return ItemProcess_Hair(item, wDestCell);

	case ITEM_POLYMORPH:
		return ItemProcess_Polymorph(item);

	case ITEM_QUEST:
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		if (GetWear(WEAR_COSTUME_MOUNT))
		{
			if (item->GetVnum() == 50051 || item->GetVnum() == 50052 || item->GetVnum() == 50053)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_CANNOT_USE_THIS_WHILE_RIDING"));
				return false;
			}
		}
#endif
		if (GetArena() != nullptr || IsObserverMode() == true)
		{
			if (item->GetVnum() == 50051 || item->GetVnum() == 50052 || item->GetVnum() == 50053)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
				return false;
			}
		}

		if (!IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE_MULTIPLE))
		{
			if (item->GetSIGVnum() == 0)
			{
				quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
			}
			else
			{
				quest::CQuestManager::instance().SIGUse(GetPlayerID(), item->GetSIGVnum(), item, false);
			}
		}
		break;

	case ITEM_CAMPFIRE:
	{
#ifdef ENABLE_CAMP_FIRE_FIX
		if (thecore_pulse() < LastCampFireUse + 60)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please wait a second."));
			return false;
		}
#endif

		float fx, fy;
		GetDeltaByDegree(GetRotation(), 100.0f, &fx, &fy);

		LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy));

		if (!tree)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ð´ÚºÒÀ» ÇÇ¿ï ¼ö ¾ø´Â ÁöÁ¡ÀÔ´Ï´Ù."));
			return false;
		}

		if (tree->IsAttr((long)(GetX() + fx), (long)(GetY() + fy), ATTR_WATER))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹° ¼Ó¿¡ ¸ð´ÚºÒÀ» ÇÇ¿ï ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}

		if (GetMapIndex() == 113)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OX_MAP_CANNOT_USE_THIS"));
			return false;
		}
		////ayar

		LPCHARACTER campfire = CHARACTER_MANAGER::instance().SpawnMob(fishing::CAMPFIRE_MOB, GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy), 0, false, number(0, 359));

		char_event_info * info = AllocEventInfo<char_event_info>();

		info->ch = campfire;

		campfire->m_pkMiningEvent = event_create(kill_campfire_event, info, PASSES_PER_SEC(40));

		item->SetCount(item->GetCount() - 1);

#ifdef ENABLE_CAMP_FIRE_FIX
		LastCampFireUse = thecore_pulse();
#endif
	}
	break;

	case ITEM_UNIQUE:
	{
		switch (item->GetSubType())
		{

#ifdef ENABLE_TALISMAN_SYSTEM
		case USE_CHARM:
		{
			if (!item->IsEquipped())
				EquipItem(item);
			else
				UnequipItem(item);
		}
		break;
#endif

#ifdef ENABLE_EXTENDED_PET_SYSTEM
		case USE_PET:
		{
			if (CWarMapManager::instance().IsWarMap(GetMapIndex()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LONCA_PET_BINEK_SAMAN"));
				return false;
			}

			if (item->GetSocket(1) == 0 && (GetPetSystem() && GetPetSystem()->CountSummoned() == 0))
			{
				if (GetPetSystem() && GetPetSystem()->CountSummoned() > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Already summoned pet!"));
					return false;
				}

				CPetSystem* petSystem = GetPetSystem();
				if (petSystem)
					petSystem->Summon(item->GetValue(0), item, 0, false);
			}
			else if (item->GetSocket(1) == 1 && (GetPetSystem() && GetPetSystem()->CountSummoned() == 0))
			{
				CPetSystem* petSystem = GetPetSystem();
				if (petSystem)
					petSystem->Summon(item->GetValue(0), item, 0, false);
			}
			else
			{
				CPetSystem* petSystem = GetPetSystem();
				if (petSystem)
					petSystem->Unsummon(item->GetValue(0));
				item->SetSocket(1, 0);
			}
		}
		break;
#endif

		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
				EffectPacket(SE_DXUP_PURPLE); //purple potion
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true, true);
				EffectPacket(SE_SPEEDUP_GREEN); //green potion
				break;

			case APPLY_STR:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_RESIST_MAGIC:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_RESIST_MAGIC, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		default:
		{
			if (item->GetSubType() == USE_SPECIAL)
			{
				sys_log(0, "ITEM_UNIQUE: USE_SPECIAL %u", item->GetVnum());

				switch (item->GetVnum())
				{
				case 71049: // ºñ´Üº¸µû¸®
					if (g_ShopIndexCount.count(GetMapIndex()) > 0)
					{
						int shop_max = g_ShopIndexCount[GetMapIndex()];
						bool block = false;

#ifdef SHOP_ONLY_ALLOWED_INDEX
						if (shop_max > 0)
						{
#else
						if (shop_max == 0)
							block = true;
						else {
#endif
							std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT map_index from player_shop WHERE channel=%d and status='OK' and map_index=%d", g_bChannel, GetMapIndex()));
							SQLResult* pRes = pkMsg->Get();
							if (pRes->uiNumRows >= shop_max)
								block = true;
						}
						if (block)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MAP_MAX"));
							return false;
						}
						}
#ifdef SHOP_ONLY_ALLOWED_INDEX
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_CANNOT_OPEN_HERE"));
						return false;
					}
#endif
					if (LC_IsYMIR() == true || LC_IsKorea() == true)
					{
						if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
						{
							UseSilkBotary();
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³ÀÎ »óÁ¡À» ¿­ ¼ö ¾ø´Â Áö¿ªÀÔ´Ï´Ù"));
						}
					}
					else
					{
						UseSilkBotary();
					}
					break;
					}
				}
			else
			{
				if (!item->IsEquipped())
					EquipItem(item);
				else
					UnequipItem(item);
			}
			}
		break;
		}
		}
	break;

	case ITEM_COSTUME:
	{
		if (item->GetSubType() == COSTUME_HAIR)
		{
			if (item->GetAttributeType(0) == 0 && item->IsQuestHair() && features::IsFeatureEnabled(features::HAIR_SELECT_EX))
				quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
			else
			{
				if (!item->IsEquipped())
					EquipItem(item);
				else
					UnequipItem(item);
			}
		}
		else
		{
			if (!item->IsEquipped())
				EquipItem(item);
			else
				UnequipItem(item);
		}
	}
	break;
	case ITEM_WEAPON:
	case ITEM_ARMOR:
	case ITEM_ROD:
	case ITEM_RING:		// ½Å±Ô ¹ÝÁö ¾ÆÀÌÅÛ
	case ITEM_BELT:		// ½Å±Ô º§Æ® ¾ÆÀÌÅÛ
		// MINING
	case ITEM_PICK:
		if (!item->IsEquipped())
		{//seri item cikarip takma fix
			if (GetQuestFlag("ARMOR.CHECKER") && get_global_time() < GetQuestFlag("ARMOR.CHECKER"))
			{
				return false;
			}
			EquipItem(item);
			SetQuestFlag("ARMOR.CHECKER", get_global_time() + 1);
		}
		else
			UnequipItem(item);
		break;
		// Âø¿ëÇÏÁö ¾ÊÀº ¿ëÈ¥¼®Àº »ç¿ëÇÒ ¼ö ¾ø´Ù.
		// Á¤»óÀûÀÎ Å¬¶ó¶ó¸é, ¿ëÈ¥¼®¿¡ °üÇÏ¿© item use ÆÐÅ¶À» º¸³¾ ¼ö ¾ø´Ù.
		// ¿ëÈ¥¼® Âø¿ëÀº item move ÆÐÅ¶À¸·Î ÇÑ´Ù.
		// Âø¿ëÇÑ ¿ëÈ¥¼®Àº ÃßÃâÇÑ´Ù.
	case ITEM_DS:
	{
		return ItemProccess_DS(item, DestCell);
		break;
	}
	case ITEM_SPECIAL_DS:
		if (!item->IsEquipped())
			EquipItem(item);
		else
			UnequipItem(item);
		break;

	case ITEM_FISH:
	{
		if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
			return false;
		}

		if (item->GetSubType() == FISH_ALIVE)
			fishing::UseFish(this, item);
	}
	break;

	case ITEM_TREASURE_BOX:
	{
		return false;
		//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¿­¼è·Î Àá°Ü ÀÖ¾î¼­ ¿­¸®Áö ¾Ê´Â°Í °°´Ù. ¿­¼è¸¦ ±¸ÇØº¸ÀÚ."));
	}
	break;

	case ITEM_TREASURE_KEY:
	{
		LPITEM item2;

		if (!GetItem(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetType() != ITEM_TREASURE_BOX)
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¿­¼è·Î ¿©´Â ¹°°ÇÀÌ ¾Æ´Ñ°Í °°´Ù."));
			return false;
		}

		if (item->GetValue(0) == item2->GetValue(0))
		{
			//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¿­¼è´Â ¸ÂÀ¸³ª ¾ÆÀÌÅÛ ÁÖ´Â ºÎºÐ ±¸ÇöÀÌ ¾ÈµÇ¾ú½À´Ï´Ù."));
			DWORD dwBoxVnum = item2->GetVnum();
			std::vector <DWORD> dwVnums;
			std::vector <DWORD> dwCounts;
			std::vector <LPITEM> item_gets(0);
			int count = 0;

			if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
			{
				item->SetCount(item->GetCount()-1);
				item2->SetCount(item2->GetCount()-1);

				for (int i = 0; i < count; i++) {
					switch (dwVnums[i])
					{
					case CSpecialItemGroup::GOLD:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ· %d ³ÉÀ» È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
						break;
					case CSpecialItemGroup::EXP:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ºÎÅÍ ½ÅºñÇÑ ºûÀÌ ³ª¿É´Ï´Ù."));
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÀÇ °æÇèÄ¡¸¦ È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
						break;
					case CSpecialItemGroup::MOB:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
						break;
					case CSpecialItemGroup::SLOW:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â »¡°£ ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ ¿òÁ÷ÀÌ´Â ¼Óµµ°¡ ´À·ÁÁ³½À´Ï´Ù!"));
						break;
					case CSpecialItemGroup::DRAIN_HP:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ°¡ °©ÀÚ±â Æø¹ßÇÏ¿´½À´Ï´Ù! »ý¸í·ÂÀÌ °¨¼ÒÇß½À´Ï´Ù."));
						break;
					case CSpecialItemGroup::POISON:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â ³ì»ö ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ µ¶ÀÌ ¿Â¸öÀ¸·Î ÆÛÁý´Ï´Ù!"));
						break;
					case CSpecialItemGroup::MOB_GROUP:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
						break;
					default:
						if (item_gets[i])
						{
							if (dwCounts[i] > 1)
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ %d °³ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName(), dwCounts[i]);
							else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName());
						}
					}
				}
			}
			else
			{
				ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¿­¼è°¡ ¸ÂÁö ¾Ê´Â °Í °°´Ù."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¿­¼è°¡ ¸ÂÁö ¾Ê´Â °Í °°´Ù."));
			return false;
		}
	}
	break;

	case ITEM_GIFTBOX:
	{
		DWORD dwBoxVnum = item->GetVnum();
		std::vector <DWORD> dwVnums;
		std::vector <DWORD> dwCounts;
		std::vector <LPITEM> item_gets(0);
		int count = 0;

		if (dwBoxVnum == 50033 && LC_IsYMIR()) // ¾Ë¼ö¾ø´Â »óÀÚ
		{
			if (GetLevel() < 15)
			{
				ChatPacket(CHAT_TYPE_INFO, "15·¹º§ ÀÌÇÏ¿¡¼­´Â »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.");
				return false;
			}
		}

		if ((dwBoxVnum > 51500 && dwBoxVnum < 52000) || (dwBoxVnum >= 50255 && dwBoxVnum <= 50260))	// ¿ëÈ¥¿ø¼®µé
		{
			if (!(this->DragonSoul_IsQualified()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ÕÀú ¿ëÈ¥¼® Äù½ºÆ®¸¦ ¿Ï·áÇÏ¼Å¾ß ÇÕ´Ï´Ù."));
				return false;
			}
		}

		if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
		{
			item->SetCount(item->GetCount() - 1);

			for (int i = 0; i < count; i++) {
				switch (dwVnums[i])
				{
				case CSpecialItemGroup::GOLD:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ· %d ³ÉÀ» È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
					break;
				case CSpecialItemGroup::EXP:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ºÎÅÍ ½ÅºñÇÑ ºûÀÌ ³ª¿É´Ï´Ù."));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÀÇ °æÇèÄ¡¸¦ È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
					break;
				case CSpecialItemGroup::MOB:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
					break;
				case CSpecialItemGroup::SLOW:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â »¡°£ ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ ¿òÁ÷ÀÌ´Â ¼Óµµ°¡ ´À·ÁÁ³½À´Ï´Ù!"));
					break;
				case CSpecialItemGroup::DRAIN_HP:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ°¡ °©ÀÚ±â Æø¹ßÇÏ¿´½À´Ï´Ù! »ý¸í·ÂÀÌ °¨¼ÒÇß½À´Ï´Ù."));
					break;
				case CSpecialItemGroup::POISON:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â ³ì»ö ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ µ¶ÀÌ ¿Â¸öÀ¸·Î ÆÛÁý´Ï´Ù!"));
					break;
				case CSpecialItemGroup::MOB_GROUP:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
					break;
				default:
					if (item_gets[i])
					{
						bool bMsg = dwBoxVnum >= 81550 && dwBoxVnum <= 81571;
						if (!bMsg)
						{
							if (dwCounts[i] > 1)
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ %d °³ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName(), dwCounts[i]);
							else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName());
						}
					}
				}
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¾Æ¹«°Íµµ ¾òÀ» ¼ö ¾ø¾ú½À´Ï´Ù."));
			return false;
		}
	}
	break;

	case ITEM_SKILLFORGET:
	{
		if (!item->GetSocket(0))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			return false;
		}

		DWORD dwVnum = item->GetSocket(0);

		if (SkillLevelDown(dwVnum))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("½ºÅ³ ·¹º§À» ³»¸®´Âµ¥ ¼º°øÇÏ¿´½À´Ï´Ù."));
		}
		else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("½ºÅ³ ·¹º§À» ³»¸± ¼ö ¾ø½À´Ï´Ù."));
	}
	break;

	case ITEM_SKILLBOOK:
	{
		if (item->GetVnum() == 50300)
		{
			ChatPacket(CHAT_TYPE_COMMAND, "bkekranac");
		}
		else
		{
			if (IsPolymorphed())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
				return false;
			}

			DWORD dwVnum = 0;

			if (item->GetVnum() == 50300)
			{
				dwVnum = item->GetSocket(0);
			}
			else
			{
				// »õ·Î¿î ¼ö·Ã¼­´Â value 0 ¿¡ ½ºÅ³ ¹øÈ£°¡ ÀÖÀ¸¹Ç·Î ±×°ÍÀ» »ç¿ë.
				dwVnum = item->GetValue(0);
			}

			if (0 == dwVnum)
			{
				ITEM_MANAGER::instance().RemoveItem(item);

				return false;
			}

			if (true == LearnSkillByBook(dwVnum))
			{
				item->SetCount(item->GetCount() - 1);
				int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
				SetSkillNextReadTime(dwVnum, get_global_time() + iReadDelay);
			}
		}
	}
	break;

	case ITEM_USE:
	{
		if (item->GetVnum() > 50800 && item->GetVnum() <= 50820)
		{
			if (test_server)
				sys_log(0, "ADD addtional effect : vnum(%d) subtype(%d)", item->GetOriginalVnum(), item->GetSubType());

			int affect_type = AFFECT_EXP_BONUS_EURO_FREE;
			int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
			int apply_value = item->GetValue(2);
			int apply_duration = item->GetValue(1);

			switch (item->GetSubType())
			{
			case USE_ABILITY_UP:
				if (FindAffect(affect_type, apply_type))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					return false;
				}

				{
					switch (item->GetValue(0))
					{
					case APPLY_MOV_SPEED:
						AddAffect(affect_type, apply_type, apply_value, AFF_MOV_SPEED_POTION, apply_duration, 0, true, true);
						break;

					case APPLY_ATT_SPEED:
						AddAffect(affect_type, apply_type, apply_value, AFF_ATT_SPEED_POTION, apply_duration, 0, true, true);
						break;

					case APPLY_STR:
					case APPLY_DEX:
					case APPLY_CON:
					case APPLY_INT:
					case APPLY_CAST_SPEED:
					case APPLY_RESIST_MAGIC:
					case APPLY_ATT_GRADE_BONUS:
					case APPLY_DEF_GRADE_BONUS:
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}
				}

				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				item->SetCount(item->GetCount() - 1);
				break;

			case USE_AFFECT:
			{
				if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
				}
				else
				{
					AddAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false, true);
					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case USE_POTION_NODELAY:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
						return false;
					}

					switch (item->GetVnum())
					{
					case 70020:
					case 71018:
					case 71019:
					case 71020:
						if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
						{
							if (m_nPotionLimit <= 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë Á¦ÇÑ·®À» ÃÊ°úÇÏ¿´½À´Ï´Ù."));
								return false;
							}
						}
						break;

					default:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
						return false;
						break;
					}
				}

				bool used = false;

				if (item->GetValue(0) != 0) // HP Àý´ë°ª È¸º¹
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(1) != 0)	// SP Àý´ë°ª È¸º¹
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (item->GetValue(3) != 0) // HP % È¸º¹
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(4) != 0) // SP % È¸º¹
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (used)
				{
					if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
					{
						if (test_server)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿ùº´ ¶Ç´Â Á¾ÀÚ ¸¦ »ç¿ëÇÏ¿´½À´Ï´Ù"));
						SetUseSeedOrMoonBottleTime();
					}
					if (GetDungeon())
						GetDungeon()->UsePotion(this);

					if (GetWarMap())
						GetWarMap()->UsePotion(this, item);

					m_nPotionLimit--;

					//RESTRICT_USE_SEED_OR_MOONBOTTLE
					item->SetCount(item->GetCount() - 1);
					//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
				}
			}
			break;
			}

			return true;
		}

		if (item->GetVnum() >= 27863 && item->GetVnum() <= 27883)
		{
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
				return false;
			}
		}

		if (test_server)
		{
			//sys_log (0, "USE_ITEM %s Type %d SubType %d vnum %d", item->GetName(), item->GetType(), item->GetSubType(), item->GetOriginalVnum());
		}

		switch (item->GetSubType())
		{
		case USE_TIME_CHARGE_PER:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (nullptr == pDestItem)
			{
				return false;
			}
			// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
			if (pDestItem->IsDragonSoul())
			{
				int ret;
				char buf[128];
				if (item->GetVnum() == DRAGON_HEART_VNUM)
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
				}
				else
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				}
				if (ret > 0)
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "Inc %ds by item{VN:%u SOC%d:%ld}", ret, item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "Inc %ds by item{VN:%u VAL%d:%ld}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÃÊ ¸¸Å­ ÃæÀüµÇ¾ú½À´Ï´Ù."), ret);
					item->SetCount(item->GetCount() - 1);
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					return true;
				}
				else
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "No change by item{VN:%u SOC%d:%ld}", item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "No change by item{VN:%u VAL%d:%ld}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÃæÀüÇÒ ¼ö ¾ø½À´Ï´Ù."));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
			}
			else
				return false;
		}
		break;
		case USE_TIME_CHARGE_FIX:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (nullptr == pDestItem)
			{
				return false;
			}
			// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
			if (pDestItem->IsDragonSoul())
			{
				int ret = pDestItem->GiveMoreTime_Fix(item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				char buf[128];
				if (ret)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÃÊ ¸¸Å­ ÃæÀüµÇ¾ú½À´Ï´Ù."), ret);
					sprintf(buf, "Increase %ds by item{VN:%u VAL%d:%ld}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÃæÀüÇÒ ¼ö ¾ø½À´Ï´Ù."));
					sprintf(buf, "No change by item{VN:%u VAL%d:%ld}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
			}
			else
				return false;
		}
		break;
#ifdef EXTEND_TIME_COSTUME
		case USE_EXTEND_TIME:
		{
			LPITEM item2;

			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsExchanging() || item2->IsEquipped())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this item when you exchange or when your object is equipped."));
				return false;
			}

			if ((item2->GetType() != ITEM_COSTUME || (item2->GetSubType() != COSTUME_BODY && item2->GetSubType() != COSTUME_HAIR)))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this item for this object."));
				return false;
			}

			item2->SetSocket(0, item2->GetSocket(0) + item->GetValue(0));
			item->SetCount(item->GetCount() - 1);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Extended time was added!"));
		}
		break;
#endif
		case USE_SPECIAL:

			switch (item->GetVnum())
			{
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			case 50263:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsEquipped() || item2->IsExchanging())
					return false;

				if (item2->IsSealed()) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item already sealed."));
					return false;
				}

				if (item2->IsBasicItem())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
					return false;
				}

				if (item2->GetType() != ITEM_WEAPON && item2->GetType() != ITEM_ARMOR && item2->GetType() != ITEM_COSTUME && item2->GetType() != ITEM_DS)
					return false;

				item2->SetSealBind();
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item sealbind success."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "SET_SEALBIND_SUCCESS", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50264:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->isLocked() || item2->IsEquipped() || item2->GetSealBindTime() >= 0)
					return false;

				if (item2->IsBasicItem())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
					return false;
				}

				if (item2->GetType() != ITEM_WEAPON && item2->GetType() != ITEM_ARMOR && item2->GetType() != ITEM_COSTUME && item2->GetType() != ITEM_DS)
					return false;

				long duration = 72 * 60 * 60;
				item2->SetSealBind(time(0) + duration);
				item2->StartUnSealBindTimerExpireEvent();
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item unsealbind success."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "REMOVE_SEALBIND_TIME_BEGIN", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;
#endif
			//Å©¸®½º¸¶½º ¶õÁÖ
			case ITEM_NOG_POCKET:
			{
				/*
				¶õÁÖ´É·ÂÄ¡ : item_proto value ÀÇ¹Ì
					ÀÌµ¿¼Óµµ  value 1
					°ø°Ý·Â	  value 2
					°æÇèÄ¡    value 3
					Áö¼Ó½Ã°£  value 0 (´ÜÀ§ ÃÊ)

				*/
				if (FindAffect(AFFECT_NOG_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					return false;
				}
				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				EffectPacket(SE_DXUP_PURPLE);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			//¶ó¸¶´Ü¿ë »çÅÁ
			case ITEM_RAMADAN_CANDY:
			{
				if (FindAffect(AFFECT_RAMADAN_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??©ö? ??¡Æ?¡Æ? ¡Æ?¡¤? ??¨ö?¢¥?¢¥?."));
					return false;
				}

				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;
			
			case ITEM_DRAGON_SILVER_POTION:
			{
				if (FindAffect(AFFECT_DG_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??©ö? ??¡Æ?¡Æ? ¡Æ?¡¤? ??¨ö?¢¥?¢¥?."));
					return false;
				}

				long time = item->GetValue(0);
				AddAffect(AFFECT_DG_ABILITY, POINT_MALL_EXPBONUS, 50, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_DG_ABILITY, POINT_MALL_ATTBONUS, 30, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_DG_ABILITY, POINT_CASTING_SPEED, 20, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_DG_ABILITY, POINT_MOV_SPEED, 30, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_DG_ABILITY, POINT_MAX_HP_PCT, 10, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_DG_ABILITY, POINT_MAX_SP_PCT, 10, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;
			
			case ITEM_MARRIAGE_RING:
			{
				marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
				if (pMarriage)
				{
					if (pMarriage->ch1 != nullptr)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch1->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
							break;
						}

						if (pMarriage->ch1->GetMapIndex() == 353 || pMarriage->ch1->GetMapIndex() == 354 || pMarriage->ch1->GetMapIndex() == 355 || pMarriage->ch1->GetMapIndex() == 212)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
							break;
						}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
						if (CCombatZoneManager::Instance().IsCombatZoneMap(pMarriage->ch1->GetMapIndex()))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_use_marriage_ring"));
							break;
						}
#endif
					}

					if (pMarriage->ch2 != nullptr)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch2->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
							break;
						}

						if (pMarriage->ch2->GetMapIndex() == 353 || pMarriage->ch2->GetMapIndex() == 354 || pMarriage->ch2->GetMapIndex() == 355 || pMarriage->ch2->GetMapIndex() == 212)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
							break;
						}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
						if (CCombatZoneManager::Instance().IsCombatZoneMap(pMarriage->ch2->GetMapIndex()))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_use_marriage_ring"));
							break;
						}
#endif
					}

					int consumeSP = CalculateConsumeSP(this);

					if (consumeSP < 0)
						return false;

					PointChange(POINT_SP, -consumeSP, false);

					WarpToPID(pMarriage->GetOther(GetPlayerID()));
				}
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°áÈ¥ »óÅÂ°¡ ¾Æ´Ï¸é °áÈ¥¹ÝÁö¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
			}
			break;

			//±âÁ¸ ¿ë±âÀÇ ¸ÁÅä
			case UNIQUE_ITEM_CAPE_OF_COURAGE:
				//¶ó¸¶´Ü º¸»ó¿ë ¿ë±âÀÇ ¸ÁÅä
			case 70057:
			case REWARD_BOX_UNIQUE_ITEM_CAPE_OF_COURAGE:
			case 70029:
				AggregateMonster();
#ifdef ENABLE_CAPE_EFFECT_FIX
				if (GetCapeEffectPulse() + 100 > thecore_pulse())
					break;
				SpecificEffectPacket("d:/ymir work/effect/etc/buff/bravery_cape.mse");
				SetCapeEffectPulse(thecore_pulse());
#endif
				//item->SetCount(item->GetCount()-1);
				break;

			case UNIQUE_ITEM_WHITE_FLAG:
#ifdef ENABLE_BLOCK_ITEMS_ON_WAR_MAP
				if (GetWarMap())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
					return false;
				}
#endif
				ForgetMyAttacker();
				item->SetCount(item->GetCount() - 1);
				break;

			case UNIQUE_ITEM_TREASURE_BOX:
				break;

			case 30093:
			case 30094:
			case 30095:
			case 30096:
				// º¹ÁÖ¸Ó´Ï
			{
				const int MAX_BAG_INFO = 26;
				static struct LuckyBagInfo
				{
					DWORD count;
					int prob;
					DWORD vnum;
				} b1[MAX_BAG_INFO] =
				{
					{ 1000,	302,	1 },
					{ 10,	150,	27002 },
					{ 10,	75,	27003 },
					{ 10,	100,	27005 },
					{ 10,	50,	27006 },
					{ 10,	80,	27001 },
					{ 10,	50,	27002 },
					{ 10,	80,	27004 },
					{ 10,	50,	27005 },
					{ 1,	10,	50300 },
					{ 1,	6,	92 },
					{ 1,	2,	132 },
					{ 1,	6,	1052 },
					{ 1,	2,	1092 },
					{ 1,	6,	2082 },
					{ 1,	2,	2122 },
					{ 1,	6,	3082 },
					{ 1,	2,	3122 },
					{ 1,	6,	5052 },
					{ 1,	2,	5082 },
					{ 1,	6,	7082 },
					{ 1,	2,	7122 },
					{ 1,	1,	11282 },
					{ 1,	1,	11482 },
					{ 1,	1,	11682 },
					{ 1,	1,	11882 },
				};

				struct LuckyBagInfo b2[MAX_BAG_INFO] =
				{
					{ 1000,	302,	1 },
					{ 10,	150,	27002 },
					{ 10,	75,	27002 },
					{ 10,	100,	27005 },
					{ 10,	50,	27005 },
					{ 10,	80,	27001 },
					{ 10,	50,	27002 },
					{ 10,	80,	27004 },
					{ 10,	50,	27005 },
					{ 1,	10,	50300 },
					{ 1,	6,	92 },
					{ 1,	2,	132 },
					{ 1,	6,	1052 },
					{ 1,	2,	1092 },
					{ 1,	6,	2082 },
					{ 1,	2,	2122 },
					{ 1,	6,	3082 },
					{ 1,	2,	3122 },
					{ 1,	6,	5052 },
					{ 1,	2,	5082 },
					{ 1,	6,	7082 },
					{ 1,	2,	7122 },
					{ 1,	1,	11282 },
					{ 1,	1,	11482 },
					{ 1,	1,	11682 },
					{ 1,	1,	11882 },
				};

				LuckyBagInfo* bi = nullptr;
				if (LC_IsHongKong())
					bi = b2;
				else
					bi = b1;

				int pct = number(1, 1000);

				int i;
				for (i = 0; i < MAX_BAG_INFO; i++)
				{
					if (pct <= bi[i].prob)
						break;
					pct -= bi[i].prob;
				}
				if (i >= MAX_BAG_INFO)
					return false;

				if (bi[i].vnum == 50300)
				{
					// ½ºÅ³¼ö·Ã¼­´Â Æ¯¼öÇÏ°Ô ÁØ´Ù.
					GiveRandomSkillBook();
				}
				else if (bi[i].vnum == 1)
				{
					PointChange(POINT_GOLD, 1000, true);
				}
				else
				{
					AutoGiveItem(bi[i].vnum, bi[i].count);
				}
				ITEM_MANAGER::instance().RemoveItem(item);
			}
			break;

			case 50004: // ÀÌº¥Æ®¿ë °¨Áö±â
			{
				if (item->GetSocket(0))
				{
					item->SetSocket(0, item->GetSocket(0) + 1);
				}
				else
				{
					// Ã³À½ »ç¿ë½Ã
					int iMapIndex = GetMapIndex();

					PIXEL_POSITION pos;

					if (SECTREE_MANAGER::instance().GetRandomLocation(iMapIndex, pos, 700))
					{
						item->SetSocket(0, 1);
						item->SetSocket(1, pos.x);
						item->SetSocket(2, pos.y);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ °÷¿¡¼± ÀÌº¥Æ®¿ë °¨Áö±â°¡ µ¿ÀÛÇÏÁö ¾Ê´Â°Í °°½À´Ï´Ù."));
						return false;
					}
				}

				int dist = 0;
				float distance = (DISTANCE_SQRT(GetX() - item->GetSocket(1), GetY() - item->GetSocket(2)));

				if (distance < 1000.0f)
				{
					// ¹ß°ß!
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌº¥Æ®¿ë °¨Áö±â°¡ ½Åºñ·Î¿î ºûÀ» ³»¸ç »ç¶óÁý´Ï´Ù."));

					// »ç¿ëÈ½¼ö¿¡ µû¶ó ÁÖ´Â ¾ÆÀÌÅÛÀ» ´Ù¸£°Ô ÇÑ´Ù.
					struct TEventStoneInfo
					{
						DWORD dwVnum;
						int count;
						int prob;
					};
					const int EVENT_STONE_MAX_INFO = 15;
					TEventStoneInfo info_10[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  8 },
						{ 27004, 10,  6 },
						{ 27002, 10, 12 },
						{ 27005, 10, 12 },
						{ 27100,  1,  9 },
						{ 27103,  1,  9 },
						{ 27101,  1, 10 },
						{ 27104,  1, 10 },
						{ 27999,  1, 12 },

						{ 25040,  1,  4 },

						{ 27410,  1,  0 },
						{ 27600,  1,  0 },
						{ 25100,  1,  0 },

						{ 50001,  1,  0 },
						{ 50003,  1,  1 },
					};
					TEventStoneInfo info_7[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  1 },
						{ 27004, 10,  1 },
						{ 27004, 10,  9 },
						{ 27005, 10,  9 },
						{ 27100,  1,  5 },
						{ 27103,  1,  5 },
						{ 27101,  1, 10 },
						{ 27104,  1, 10 },
						{ 27999,  1, 14 },

						{ 25040,  1,  5 },

						{ 27410,  1,  5 },
						{ 27600,  1,  5 },
						{ 25100,  1,  5 },

						{ 50001,  1,  0 },
						{ 50003,  1,  5 },
					};
					TEventStoneInfo info_4[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10,  0 },
						{ 27004, 10,  0 },
						{ 27002, 10,  0 },
						{ 27005, 10,  0 },
						{ 27100,  1,  0 },
						{ 27103,  1,  0 },
						{ 27101,  1,  0 },
						{ 27104,  1,  0 },
						{ 27999,  1, 25 },

						{ 25040,  1,  0 },

						{ 27410,  1,  0 },
						{ 27600,  1,  0 },
						{ 25100,  1, 15 },

						{ 50001,  1, 10 },
						{ 50003,  1, 50 },
					};

					{
						TEventStoneInfo* info;
						if (item->GetSocket(0) <= 4)
							info = info_4;
						else if (item->GetSocket(0) <= 7)
							info = info_7;
						else
							info = info_10;

						int prob = number(1, 100);

						for (int i = 0; i < EVENT_STONE_MAX_INFO; ++i)
						{
							if (!info[i].prob)
								continue;

							if (prob <= info[i].prob)
							{
								AutoGiveItem(info[i].dwVnum, info[i].count);

								break;
							}
							prob -= info[i].prob;
						}
					}

					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf), "StoneDetect %u 0 0", (DWORD)GetVID());

					if (len < 0 || len >= (int) sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len;  // \0 ¹®ÀÚ±îÁö º¸³»±â

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id	= vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());

					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 1");
					return true;
				}
				else if (distance < 20000)
					dist = 1;
				else if (distance < 70000)
					dist = 2;
				else
					dist = 3;

				// ¸¹ÀÌ »ç¿ëÇßÀ¸¸é »ç¶óÁø´Ù.
				const int STONE_DETECT_MAX_TRY = 10;
				if (item->GetSocket(0) >= STONE_DETECT_MAX_TRY)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌº¥Æ®¿ë °¨Áö±â°¡ ÈçÀûµµ ¾øÀÌ »ç¶óÁý´Ï´Ù."));
					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 0");
					AutoGiveItem(27002);
					return true;
				}

				if (dist)
				{
					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf),
						"StoneDetect %u %d %d",
						(DWORD)GetVID(), dist, (int)GetDegreeFromPositionXY(GetX(), item->GetSocket(2), item->GetSocket(1), GetY()));

					if (len < 0 || len >= (int) sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len;  // \0 ¹®ÀÚ±îÁö º¸³»±â

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id		= vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());
				}
			}
			break;

			case 27989: // ¿µ¼®°¨Áö±â
			case 76006: // ¼±¹°¿ë ¿µ¼®°¨Áö±â
			{
				LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

				if (pMap != nullptr)
				{
					FFindStone f;

					// <Factor> SECTREE::for_each -> SECTREE::for_each_entity
					pMap->for_each(f);

					if (f.m_mapStone.size() > 0)
					{
						std::map<DWORD, LPCHARACTER>::iterator stone = f.m_mapStone.begin();

						DWORD max = UINT_MAX;
						LPCHARACTER pTarget = stone->second;

						while (stone != f.m_mapStone.end())
						{
							DWORD dist = (DWORD)DISTANCE_SQRT(GetX() - stone->second->GetX(), GetY() - stone->second->GetY());

							if (dist != 0 && max > dist)
							{
								max = dist;
								pTarget = stone->second;
							}
							stone++;
						}

						if (pTarget != nullptr)
						{
							int val = 3;

							if (max < 10000) val = 2;
							else if (max < 70000) val = 1;

							ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u %d %d", (DWORD)GetVID(), val,
								(int)GetDegreeFromPositionXY(GetX(), pTarget->GetY(), pTarget->GetX(), GetY()));
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°¨Áö±â¸¦ ÀÛ¿ëÇÏ¿´À¸³ª °¨ÁöµÇ´Â ¿µ¼®ÀÌ ¾ø½À´Ï´Ù."));
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°¨Áö±â¸¦ ÀÛ¿ëÇÏ¿´À¸³ª °¨ÁöµÇ´Â ¿µ¼®ÀÌ ¾ø½À´Ï´Ù."));
					}
				}
				break;
			}
			break;

			case 50513:
				ChatPacket(CHAT_TYPE_COMMAND, "ruhtasiekranac");
				break;

			case 39023:
				if (int(GetQuestFlag("bio.sans")) == 1)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosanszatenaktif"));
				else if (GetQuestFlag("bio.durum") == 31 || GetQuestFlag("bio.durum") == 41 || GetQuestFlag("bio.durum") == 51 || GetQuestFlag("bio.durum") == 61 || GetQuestFlag("bio.durum") == 71 || GetQuestFlag("bio.durum") == 81 || GetQuestFlag("bio.durum") == 91 || GetQuestFlag("bio.durum") == 93 || GetQuestFlag("bio.durum") == 11)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0");
				}
				else if (GetQuestFlag("bio.ruhtasi") == 1)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
				else
				{
					item->SetCount(item->GetCount() - 1);
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
					SetQuestFlag("bio.sans", 1);
				}
				break;
			case 31029:
				if (int(GetQuestFlag("bio.sure")) == 1)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
				else if (GetQuestFlag("bio.durum") == 31 || GetQuestFlag("bio.durum") == 41 || GetQuestFlag("bio.durum") == 51 || GetQuestFlag("bio.durum") == 61 || GetQuestFlag("bio.durum") == 71 || GetQuestFlag("bio.durum") == 81 || GetQuestFlag("bio.durum") == 91 || GetQuestFlag("bio.durum") == 93 || GetQuestFlag("bio.durum") == 11)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0");
				}
				else if (GetQuestFlag("bio.ruhtasi") == 1)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
				else
				{
					item->SetCount(item->GetCount() - 1);
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
					SetQuestFlag("bio.sure", 1);
					SetQuestFlag("bio.kalan", 0);
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[GetQuestFlag("bio.durum")][0], GetQuestFlag("bio.verilen"), BiyologSistemi[GetQuestFlag("bio.durum")][1], GetQuestFlag("bio.kalan"));
				}
				break;
			case 31030:
				if (int(GetQuestFlag("bio.sure")) == 1)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
				else if (GetQuestFlag("bio.durum") == 31 || GetQuestFlag("bio.durum") == 41 || GetQuestFlag("bio.durum") == 51 || GetQuestFlag("bio.durum") == 61 || GetQuestFlag("bio.durum") == 71 || GetQuestFlag("bio.durum") == 81 || GetQuestFlag("bio.durum") == 91 || GetQuestFlag("bio.durum") == 93 || GetQuestFlag("bio.durum") == 11)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0");
				}
				else
				{
					item->SetCount(item->GetCount() - 1);
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
					SetQuestFlag("bio.sure", 1);
					SetQuestFlag("bio.sans", 1);
					SetQuestFlag("bio.kalan", 0);
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[GetQuestFlag("bio.durum")][0], GetQuestFlag("bio.verilen"), BiyologSistemi[GetQuestFlag("bio.durum")][1], GetQuestFlag("bio.kalan"));
				}
				break;

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
			case ITEM_PRIVATESHOPSEARCH_FIRST:
				ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 1");
				break;

			case ITEM_PRIVATESHOPSEARCH_SECOND:
				ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 2");
				break;

			case ITEM_PRIVATESHOPSEARCH_DIAMOND:
				ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 2");
				break;
#endif

#ifdef ENABLE_SHOP_DECORATION_SYSTEM
			case ITEM_SHOP_DECO_PACK:
				item->SetCount(item->GetCount() - 1);
				AddAffect(AFFECT_SHOP_DECO, POINT_NONE, 0, AFF_NONE, item->GetValue(0), 0, true);
				break;
#endif

#ifdef ENABLE_AUTO_HUNTING_SYSTEM
			case ITEM_AUTO_HUNTING:
			case ITEM_AUTO_HUNTING_2:
			case ITEM_AUTO_HUNTING_3:
			case ITEM_AUTO_HUNTING_4:
				item->SetCount(item->GetCount() - 1);
				AddAffect(AFFECT_AUTO_HUNT, POINT_NONE, 0, AFF_NONE, item->GetValue(0), 0, true);
				break;
#endif

			case ITEM_BLEED_BANDAGE: // New Item Bandage for Bleed
				RemoveBleeding();
				break;

			case 27987: // Á¶°³
				// 50  µ¹Á¶°¢ 47990
				// 30  ²Î
				// 10  ¹éÁøÁÖ 47992
				// 7   Ã»ÁøÁÖ 47993
				// 3   ÇÇÁøÁÖ 47994
			{
				item->SetCount(item->GetCount() - 1);

				int r = number(1, 100);

				if (r <= 55)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¶°³¿¡¼­ µ¹Á¶°¢ÀÌ ³ª¿Ô½À´Ï´Ù."));
					AutoGiveItem(27990);
				}
				else
				{
					const int prob_table[] =
					{
						70, 83, 91
					};

					if (r <= prob_table[0])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¶°³°¡ ÈçÀûµµ ¾øÀÌ »ç¶óÁý´Ï´Ù."));
					}
					else if (r <= prob_table[1])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¶°³¿¡¼­ ¹éÁøÁÖ°¡ ³ª¿Ô½À´Ï´Ù."));
						AutoGiveItem(27992);
					}
					else if (r <= prob_table[2])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¶°³¿¡¼­ Ã»ÁøÁÖ°¡ ³ª¿Ô½À´Ï´Ù."));
						AutoGiveItem(27993);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¶°³¿¡¼­ ÇÇÁøÁÖ°¡ ³ª¿Ô½À´Ï´Ù."));
						AutoGiveItem(27994);
					}
				}
			}
			break;

			case 71013: // ÃàÁ¦¿ëÆøÁ×
				CreateFly(number(FLY_FIREWORK1, FLY_FIREWORK6), this);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50100: // ÆøÁ×
			case 50101:
			case 50102:
			case 50103:
			case 50104:
			case 50105:
			case 50106:
				CreateFly(item->GetVnum() - 50100 + FLY_FIREWORK1, this);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50200: // º¸µû¸®
				if (g_ShopIndexCount.count(GetMapIndex()) > 0)
				{
					int shop_max = g_ShopIndexCount[GetMapIndex()];
					bool block = false;

#ifdef SHOP_ONLY_ALLOWED_INDEX
					if (shop_max > 0)
					{
#else
					if (shop_max == 0)
						block = true;
					else {
#endif
						std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT map_index from player_shop WHERE channel=%d and status='OK' and map_index=%d", g_bChannel, GetMapIndex()));
						SQLResult* pRes = pkMsg->Get();
						if (pRes->uiNumRows >= shop_max)
							block = true;
					}
					if (block)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MAP_MAX"));
						return false;
					}
					}
#ifdef SHOP_ONLY_ALLOWED_INDEX
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_CANNOT_OPEN_HERE"));
					return false;
				}
#endif
				UseSilkBotary();
				break;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
			case ITEM_EXTEND_INVENTORY_FIRST:
			case ITEM_EXTEND_INVENTORY_SECOND:
				ExtendInventoryItemUse(this);
				break;
#endif

			case ITEM_GUILD_RING_NEW:
				{
					if (GetGuild())
					{
						TGuildMember* gm = GetGuild()->GetMember(GetPlayerID());
						if (gm->grade == GUILD_LEADER_GRADE)
						{
							for (int i = GUILD_SKILL_START; i < GUILD_SKILL_END; ++i)
								GetGuild()->SetSkillLevel(i, 0, 0);
							
							GetGuild()->SetSkillPoint(GetGuild()->GetLevel()-1);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_LEADER_KULLANIR"));
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BIR_LONCA_UYESI_DEGILSIN"));
					}
				}
				break;
				
			case fishing::FISH_MIND_PILL_VNUM:
				AddAffect(AFFECT_FISH_MIND_PILL, POINT_NONE, 0, AFF_FISH_MIND, 20 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50301: // Åë¼Ö·Â ¼ö·Ã¼­
			case 50302:
			case 50303:
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				int lv = GetSkillLevel(SKILL_LEADERSHIP);

				if (lv < item->GetValue(0))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ã¥Àº ³Ê¹« ¾î·Á¿ö ÀÌÇØÇÏ±â°¡ Èûµì´Ï´Ù."));
					return false;
				}

				if (lv >= item->GetValue(1))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ã¥Àº ¾Æ¹«¸® ºÁµµ µµ¿òÀÌ µÉ °Í °°Áö ¾Ê½À´Ï´Ù."));
					return false;
				}

				if (LearnSkillByBook(SKILL_LEADERSHIP))
				{
					item->SetCount(item->GetCount() - 1);
					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					SetSkillNextReadTime(SKILL_LEADERSHIP, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50304: // ¿¬°è±â ¼ö·Ã¼­
			case 50305:
			case 50306:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				if (GetSkillLevel(SKILL_COMBO) == 0 && GetLevel() < 30)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·¹º§ 30ÀÌ µÇ±â Àü¿¡´Â ½ÀµæÇÒ ¼ö ÀÖÀ» °Í °°Áö ¾Ê½À´Ï´Ù."));
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) == 1 && GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·¹º§ 50ÀÌ µÇ±â Àü¿¡´Â ½ÀµæÇÒ ¼ö ÀÖÀ» °Í °°Áö ¾Ê½À´Ï´Ù."));
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) >= 2)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿¬°è±â´Â ´õÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				int iPct = item->GetValue(0);

				if (LearnSkillByBook(SKILL_COMBO, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(SKILL_COMBO, get_global_time() + iReadDelay);
				}
			}
			break;
			case 50311: // ¾ð¾î ¼ö·Ã¼­
			case 50312:
			case 50313:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);
				if (GetSkillLevel(dwSkillVnum) >= 20 || dwSkillVnum - SKILL_LANGUAGE1 + 1 == GetEmpire())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ¿Ïº®ÇÏ°Ô ¾Ë¾ÆµéÀ» ¼ö ÀÖ´Â ¾ð¾îÀÌ´Ù."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50061: // ÀÏº» ¸» ¼ÒÈ¯ ½ºÅ³ ¼ö·Ã¼­
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 10)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					ITEM_MANAGER::instance().RemoveItem(item);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50314: case 50315: case 50316: // º¯½Å ¼ö·Ã¼­
			case 50325: case 50326: // ÁõÇ÷ ¼ö·Ã¼­
			case 50327: case 50328: // Ã¶Åë ¼ö·Ã¼­
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				int iSkillLevelLowLimit = item->GetValue(0);
				int iSkillLevelHighLimit = item->GetValue(1);
				int iPct = MINMAX(0, item->GetValue(2), 100);
				int iLevelLimit = item->GetValue(3);
				DWORD dwSkillVnum = 0;

				switch (item->GetVnum())
				{
				case 50314: case 50315: case 50316:
					dwSkillVnum = SKILL_POLYMORPH;
					break;

				case 50325: case 50326:
					dwSkillVnum = SKILL_ADD_HP;
					break;

				case 50327: case 50328:
					dwSkillVnum = SKILL_RESIST_PENETRATE;
					break;

				default:
					return false;
				}

				if (0 == dwSkillVnum)
					return false;

				if (GetLevel() < iLevelLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ã¥À» ÀÐÀ¸·Á¸é ·¹º§À» ´õ ¿Ã·Á¾ß ÇÕ´Ï´Ù."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) < iSkillLevelLowLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ã¥Àº ³Ê¹« ¾î·Á¿ö ÀÌÇØÇÏ±â°¡ Èûµì´Ï´Ù."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= iSkillLevelHighLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ã¥À¸·Î´Â ´õ ÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50902:
			case 50903:
			case 50904:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				DWORD dwSkillVnum = SKILL_CREATE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);

					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Success to learn skill ");
					}
				}
				else
				{
					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Failed to learn skill ");
					}
				}
			}
			break;

			// MINING
			case ITEM_MINING_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				DWORD dwSkillVnum = SKILL_MINING;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ¼ö·ÃÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;
			// END_OF_MINING

			case ITEM_HORSE_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯½ÅÁß¿¡´Â Ã¥À» ÀÐÀ»¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
				DWORD dwSkillVnum = SKILL_HORSE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ ½Â¸¶ ½ºÅ³À» ¼ö·ÃÇÒ ¼ö ÀÖ´Â ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù."));
					return false;
				}

				if (!test_server && get_global_time() < GetSkillNextReadTime(dwSkillVnum))
				{
					if (FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
					{
						// ÁÖ¾È¼ú¼­ »ç¿ëÁß¿¡´Â ½Ã°£ Á¦ÇÑ ¹«½Ã
						RemoveAffect(AFFECT_SKILL_NO_BOOK_DELAY);
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÁÖ¾È¼ú¼­¸¦ ÅëÇØ ÁÖÈ­ÀÔ¸¶¿¡¼­ ºüÁ®³ª¿Ô½À´Ï´Ù."));
					}
					else
					{
						SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(dwSkillVnum) - get_global_time());
						return false;
					}
				}

				if (GetPoint(POINT_HORSE_SKILL) >= 20 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK_RANGE) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ½Â¸¶ ¼ö·Ã¼­¸¦ ÀÐÀ» ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (number(1, 100) <= iPct)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("½Â¸¶ ¼ö·Ã¼­¸¦ ÀÐ¾î ½Â¸¶ ½ºÅ³ Æ÷ÀÎÆ®¸¦ ¾ò¾ú½À´Ï´Ù."));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾òÀº Æ÷ÀÎÆ®·Î´Â ½Â¸¶ ½ºÅ³ÀÇ ·¹º§À» ¿Ã¸± ¼ö ÀÖ½À´Ï´Ù."));
					PointChange(POINT_HORSE_SKILL, 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					if (!test_server)
						SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("½Â¸¶ ¼ö·Ã¼­ ÀÌÇØ¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
				}

				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 70102: // ¼±µÎ
			case 70103: // ¼±µÎ
			{
				if (GetAlignment() >= 0)
					return false;

				int delta = MIN(-GetAlignment(), item->GetValue(0));

				sys_log(0, "%s ALIGNMENT ITEM %d", GetName(), delta);

#ifdef ENABLE_ALIGNMENT_SYSTEM
				UpdateAlignment(delta, true);
#else
				UpdateAlignment(delta);
#endif
				item->SetCount(item->GetCount() - 1);

				if (delta / 10 > 0)
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¸¶À½ÀÌ ¸¼¾ÆÁö´Â±º. °¡½¿À» Áþ´©¸£´ø ¹«¾ð°¡°¡ Á» °¡º­¿öÁø ´À³¦ÀÌ¾ß."));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼±¾ÇÄ¡°¡ %d Áõ°¡ÇÏ¿´½À´Ï´Ù."), delta / 10);
				}
			}
			break;

#ifdef ENABLE_ALIGNMENT_SYSTEM
			case 71107:
			case 81602:
			case 81603:
			case 81604:
			{
				if (GetAlignment() < 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("DERECENI_ZEN_FASULYESIYLE_YUKSELTEBILIRSIN"));
					return false;
				}
				long long val = item->GetValue(0);
				if (10000000 - GetAlignment() < val * 10)
				{
					val = (10000000 - GetAlignment()) / 10;
				}
				long long old_alignment = GetAlignment() / 10;
#ifdef ENABLE_ALIGNMENT_SYSTEM
				UpdateAlignment(val * 10, true);
#else
				UpdateAlignment(val * 10);
#endif
				
				item->SetCount(item->GetCount() - 1);

				ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¸¶À½ÀÌ ¸¼¾ÆÁö´Â±º. °¡½¿À» Áþ´©¸£´ø ¹«¾ð°¡°¡ Á» °¡º­¿öÁø ´À³¦ÀÌ¾ß."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼±¾ÇÄ¡°¡ %d Áõ°¡ÇÏ¿´½À´Ï´Ù."), val);

				char buf[256 + 1];
				snprintf(buf, sizeof(buf), "%lld %lld", old_alignment, GetAlignment() / 10);
				LogManager::instance().CharLog(this, val, "MYTHICAL_PEACH", buf);
			}
			break;
#else
			case 71107: // Ãµµµº¹¼þ¾Æ
			case 39032:
			{
				int val = item->GetValue(0);
				int interval = item->GetValue(1);
				quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());
				if (!pPC)
					return false;
				int last_use_time = pPC->GetFlag("mythical_peach.last_use_time");

				if (get_global_time() - last_use_time < interval * 60 * 60)
				{
					if (test_server == false)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
						return false;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Å×½ºÆ® ¼­¹ö ½Ã°£Á¦ÇÑ Åë°ú"));
					}
				}

				if (GetAlignment() == 200000)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼±¾ÇÄ¡¸¦ ´õ ÀÌ»ó ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (200000 - GetAlignment() < val * 10)
				{
					val = (200000 - GetAlignment()) / 10;
				}

				int old_alignment = GetAlignment() / 10;

				UpdateAlignment(val * 10);

				item->SetCount(item->GetCount() - 1);
				pPC->SetFlag("mythical_peach.last_use_time", get_global_time());

				ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¸¶À½ÀÌ ¸¼¾ÆÁö´Â±º. °¡½¿À» Áþ´©¸£´ø ¹«¾ð°¡°¡ Á» °¡º­¿öÁø ´À³¦ÀÌ¾ß."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼±¾ÇÄ¡°¡ %d Áõ°¡ÇÏ¿´½À´Ï´Ù."), val);

				char buf[256 + 1];
				snprintf(buf, sizeof(buf), "%d %d", old_alignment, GetAlignment() / 10);
				LogManager::instance().CharLog(this, val, "MYTHICAL_PEACH", buf);
			}
			break;
#endif

			case 71109: // Å»¼®¼­
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->GetSocketCount() == 0)
					return false;

				if (item2->IsEquipped())
					return false;

				switch (item2->GetType())
				{
				case ITEM_WEAPON:
					break;
				case ITEM_ARMOR:
					switch (item2->GetSubType())
					{
					case ARMOR_EAR:
					case ARMOR_WRIST:
					case ARMOR_NECK:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»©³¾ ¿µ¼®ÀÌ ¾ø½À´Ï´Ù"));
						return false;
					}
					break;

				default:
					return false;
				}

				std::stack<long> socket;

				for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
					socket.push(item2->GetSocket(i));

				int idx = ITEM_SOCKET_MAX_NUM - 1;

				while (socket.size() > 0)
				{
					if (socket.top() > 2 && socket.top() != ITEM_BROKEN_METIN_VNUM)
						break;

					idx--;
					socket.pop();
				}

				if (socket.size() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»©³¾ ¿µ¼®ÀÌ ¾ø½À´Ï´Ù"));
					return false;
				}

				LPITEM pItemReward = AutoGiveItem(socket.top());

				if (pItemReward != nullptr)
				{
					item2->SetSocket(idx, 1);

					char buf[256 + 1];
					snprintf(buf, sizeof(buf), "%s(%u) %s(%u)",
						item2->GetName(), item2->GetID(), pItemReward->GetName(), pItemReward->GetID());
					LogManager::instance().ItemLog(this, item, "USE_DETACHMENT_ONE", buf);

					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case 70201:   // Å»»öÁ¦
			case 70202:   // ¿°»ö¾à(Èò»ö)
			case 70203:   // ¿°»ö¾à(±Ý»ö)
			case 70204:   // ¿°»ö¾à(»¡°£»ö)
			case 70205:   // ¿°»ö¾à(°¥»ö)
			case 70206:   // ¿°»ö¾à(°ËÀº»ö)
			{
				// NEW_HAIR_STYLE_ADD
				if (GetPart(PART_HAIR) >= 1001)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÇöÀç Çì¾î½ºÅ¸ÀÏ¿¡¼­´Â ¿°»ö°ú Å»»öÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù."));
				}
				// END_NEW_HAIR_STYLE_ADD
				else
				{
					quest::CQuestManager& q = quest::CQuestManager::instance();
					quest::PC* pPC = q.GetPC(GetPlayerID());

					if (pPC)
					{
						int last_dye_level = pPC->GetFlag("dyeing_hair.last_dye_level");

						if (last_dye_level == 0 ||
							last_dye_level + 3 <= GetLevel() ||
							item->GetVnum() == 70201)
						{
							SetPart(PART_HAIR, item->GetVnum() - 70201);

							if (item->GetVnum() == 70201)
								pPC->SetFlag("dyeing_hair.last_dye_level", 0);
							else
								pPC->SetFlag("dyeing_hair.last_dye_level", GetLevel());

							item->SetCount(item->GetCount() - 1);
							UpdatePacket();
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ·¹º§ÀÌ µÇ¾î¾ß ´Ù½Ã ¿°»öÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù."), last_dye_level + 3);
						}
					}
				}
			}
			break;

			case ITEM_NEW_YEAR_GREETING_VNUM:
			{
				DWORD dwBoxVnum = ITEM_NEW_YEAR_GREETING_VNUM;
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets;
				int count = 0;

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
				{
					for (int i = 0; i < count; i++)
					{
						if (dwVnums[i] == CSpecialItemGroup::GOLD)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ· %d ³ÉÀ» È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
					}

					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case ITEM_VALENTINE_ROSE:
			case ITEM_VALENTINE_CHOCOLATE:
			{
				DWORD dwBoxVnum = item->GetVnum();
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;

				if ((item->GetVnum() == ITEM_VALENTINE_ROSE && SEX_MALE == GET_SEX(this)) ||
					(item->GetVnum() == ITEM_VALENTINE_CHOCOLATE && SEX_FEMALE == GET_SEX(this)))
				{
					// ¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ¾µ ¼ö ¾ø´Ù.
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ÀÌ ¾ÆÀÌÅÛÀ» ¿­ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_WHITEDAY_CANDY:
			case ITEM_WHITEDAY_ROSE:
			{
				DWORD dwBoxVnum = item->GetVnum();
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;

				if ((item->GetVnum() == ITEM_WHITEDAY_CANDY && SEX_MALE == GET_SEX(this)) ||
					(item->GetVnum() == ITEM_WHITEDAY_ROSE && SEX_FEMALE == GET_SEX(this)))
				{
					// ¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ¾µ ¼ö ¾ø´Ù.
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ÀÌ ¾ÆÀÌÅÛÀ» ¿­ ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50011: // ¿ù±¤º¸ÇÕ
			{
				DWORD dwBoxVnum = 50011;
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;

				if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
				{
					for (int i = 0; i < count; i++)
					{
						char buf[50 + 1];
						snprintf(buf, sizeof(buf), "%u %u", dwVnums[i], dwCounts[i]);
						LogManager::instance().ItemLog(this, item, "MOONLIGHT_GET", buf);

						//ITEM_MANAGER::instance().RemoveItem(item);
						item->SetCount(item->GetCount() - 1);

						switch (dwVnums[i])
						{
						case CSpecialItemGroup::GOLD:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ· %d ³ÉÀ» È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
							break;

						case CSpecialItemGroup::EXP:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ºÎÅÍ ½ÅºñÇÑ ºûÀÌ ³ª¿É´Ï´Ù."));
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÀÇ °æÇèÄ¡¸¦ È¹µæÇß½À´Ï´Ù."), dwCounts[i]);
							break;

						case CSpecialItemGroup::MOB:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
							break;

						case CSpecialItemGroup::SLOW:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â »¡°£ ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ ¿òÁ÷ÀÌ´Â ¼Óµµ°¡ ´À·ÁÁ³½À´Ï´Ù!"));
							break;

						case CSpecialItemGroup::DRAIN_HP:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ°¡ °©ÀÚ±â Æø¹ßÇÏ¿´½À´Ï´Ù! »ý¸í·ÂÀÌ °¨¼ÒÇß½À´Ï´Ù."));
							break;

						case CSpecialItemGroup::POISON:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ³ª¿Â ³ì»ö ¿¬±â¸¦ µéÀÌ¸¶½ÃÀÚ µ¶ÀÌ ¿Â¸öÀ¸·Î ÆÛÁý´Ï´Ù!"));
							break;

						case CSpecialItemGroup::MOB_GROUP:
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ ¸ó½ºÅÍ°¡ ³ªÅ¸³µ½À´Ï´Ù!"));
							break;

						default:
							if (item_gets[i])
							{
								if (dwCounts[i] > 1)
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ %d °³ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName(), dwCounts[i]);
								else
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÀÚ¿¡¼­ %s °¡ ³ª¿Ô½À´Ï´Ù."), item_gets[i]->GetName());
							}
							break;
						}
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("¾Æ¹«°Íµµ ¾òÀ» ¼ö ¾ø¾ú½À´Ï´Ù."));
					return false;
				}
			}
			break;

			case ITEM_GIVE_STAT_RESET_COUNT_VNUM:
			{
				//PointChange(POINT_GOLD, -iCost);
				PointChange(POINT_STAT_RESET_COUNT, 1);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50107:
			{
				EffectPacket(SE_CHINA_FIREWORK);
				// ½ºÅÏ °ø°ÝÀ» ¿Ã·ÁÁØ´Ù
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50108:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
					return false;
				}

				EffectPacket(SE_SPIN_TOP);
				// ½ºÅÏ °ø°ÝÀ» ¿Ã·ÁÁØ´Ù
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_WONSO_BEAN_VNUM:
				PointChange(POINT_HP, GetMaxHP() - GetHP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_SUGAR_VNUM:
				PointChange(POINT_SP, GetMaxSP() - GetSP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_FRUIT_VNUM:
				PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_ELK_VNUM: // µ·²Ù·¯¹Ì
			{
				int iGold = item->GetSocket(0);
				ITEM_MANAGER::instance().RemoveItem(item);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ· %d ³ÉÀ» È¹µæÇß½À´Ï´Ù."), iGold);
				PointChange(POINT_GOLD, iGold);
			}
			break;

			case ITEM_CHEQUE_COUPON:
			{
				int iCheque = item->GetSocket(0);
#ifdef ENABLE_CHEQUE_COUPON_SYSTEM
				if (iCheque == 0)
				{
					char buf[128];
					snprintf(buf, sizeof(buf), "OpenChequeTicket %d", item->GetCell());
					ChatPacket(CHAT_TYPE_COMMAND, buf);
				}
				else
				{
#endif
					if (GetCheque() + iCheque > CHEQUE_MAX)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Envanterinde fazla won tasiyorsun."));
						return false;
					}
					ITEM_MANAGER::instance().RemoveItem(item);
					PointChange(POINT_CHEQUE, iCheque, true);
#ifdef ENABLE_CHEQUE_COUPON_SYSTEM
				}
#endif
			}
			break;

			//±ºÁÖÀÇ ÁõÇ¥

			case 27995:
			{
			}
			break;

			case 71092: // º¯½Å ÇØÃ¼ºÎ ÀÓ½Ã
			{
				if (m_pkChrTarget != nullptr)
				{
					if (m_pkChrTarget->IsPolymorphed())
					{
						m_pkChrTarget->SetPolymorph(0);
						m_pkChrTarget->RemoveAffect(AFFECT_POLYMORPH);
					}
				}
				else
				{
					if (IsPolymorphed())
					{
						SetPolymorph(0);
						RemoveAffect(AFFECT_POLYMORPH);
					}
				}
			}
			break;
			
#ifdef ENABLE_PERMA_BLEND_SYSTEM
			case NEW_MOVE_SPEED_POTION:
			case NEW_ATTACK_SPEED_POTION:
			case NEW_DRAGON_POTION_18385:
			case NEW_DRAGON_POTION_18386:
			case NEW_DRAGON_POTION_18387:
			case NEW_DRAGON_POTION_18388:
			case NEW_CRITIC_POTION_18389:
			case NEW_PENETRE_POTION_18390:
			case NEW_AFFECT_POTION_18391:
			case NEW_AFFECT_POTION_18392:
			case NEW_AFFECT_POTION_18393:
			case NEW_AFFECT_POTION_18394:
			case NEW_AFFECT_POTION_18395:
			case NEW_AFFECT_POTION_13131:
			{
				EAffectTypes type = AFFECT_NONE;

				if (item->GetVnum() == NEW_MOVE_SPEED_POTION)
					type = AFFECT_MOV_SPEED;
				else if (item->GetVnum() == NEW_ATTACK_SPEED_POTION)
					type = AFFECT_ATT_SPEED;
				else if (item->GetVnum() == NEW_DRAGON_POTION_18385)
					type = AFFECT_18385;
				else if (item->GetVnum() == NEW_DRAGON_POTION_18386)
					type = AFFECT_18386;
				else if (item->GetVnum() == NEW_DRAGON_POTION_18387)
					type = AFFECT_18387;
				else if (item->GetVnum() == NEW_DRAGON_POTION_18388)
					type = AFFECT_18388;
				else if (item->GetVnum() == NEW_CRITIC_POTION_18389)
					type = AFFECT_18389;
				else if (item->GetVnum() == NEW_PENETRE_POTION_18390)
					type = AFFECT_18390;
/*****************************************/
				else if (item->GetVnum() == NEW_AFFECT_POTION_18391)
					type = AFFECT_18391;
				else if (item->GetVnum() == NEW_AFFECT_POTION_18392)
					type = AFFECT_18392;
				else if (item->GetVnum() == NEW_AFFECT_POTION_18393)
					type = AFFECT_18393;
				else if (item->GetVnum() == NEW_AFFECT_POTION_18394)
					type = AFFECT_18394;
				else if (item->GetVnum() == NEW_AFFECT_POTION_18395)
					type = AFFECT_18395;
				else if (item->GetVnum() == NEW_AFFECT_POTION_13131)
					type = AFFECT_13131;
/*****************************************/
				if (AFFECT_NONE == type)
					break;

				CAffect * pAffect = FindAffect(type);
				
				// if (!pAffect)
					// break;

				if (NULL == pAffect)
				{
					EPointTypes bonus = POINT_NONE;

					// Green and purple potion.
					if (item->GetVnum() == NEW_MOVE_SPEED_POTION)
					{
						bonus = POINT_MOV_SPEED;
					}
					else if (item->GetVnum() == NEW_ATTACK_SPEED_POTION)
					{
						bonus = POINT_ATT_SPEED;
					}
					else if (item->GetVnum() == NEW_DRAGON_POTION_18385)
					{
						bonus = POINT_MAX_HP_PCT;
					}
					else if (item->GetVnum() == NEW_DRAGON_POTION_18386)
					{
						bonus = POINT_MAX_SP_PCT;
					}
					else if (item->GetVnum() == NEW_DRAGON_POTION_18387)
					{
						bonus = POINT_MALL_DEFBONUS;
					}
					else if (item->GetVnum() == NEW_DRAGON_POTION_18388)
					{
						bonus = POINT_MALL_ATTBONUS;
					}
					else if (item->GetVnum() == NEW_CRITIC_POTION_18389)
					{
						bonus = POINT_CRITICAL_PCT;
					}
					else if (item->GetVnum() == NEW_PENETRE_POTION_18390)
					{
						bonus = POINT_PENETRATE_PCT;
					}
/*****************************************/
					else if (item->GetVnum() == NEW_AFFECT_POTION_18391)
					{
						bonus = POINT_CRITICAL_PCT;
					}
					else if (item->GetVnum() == NEW_AFFECT_POTION_18392)
					{
						bonus = POINT_PENETRATE_PCT;
					}
					else if (item->GetVnum() == NEW_AFFECT_POTION_18393)
					{
						bonus = POINT_ATT_SPEED;
					}
					else if (item->GetVnum() == NEW_AFFECT_POTION_18394)
					{
						bonus = POINT_RESIST_MAGIC;
					}
					else if (item->GetVnum() == NEW_AFFECT_POTION_18395)
					{
						bonus = POINT_ATT_GRADE_BONUS;
					}
					else if (item->GetVnum() == NEW_AFFECT_POTION_13131)
					{
						bonus = POINT_DEF_GRADE_BONUS;
					}
/*****************************************/
					if (item->GetVnum() == NEW_AFFECT_POTION_18391 && FindAffect(AFFECT_BLEND, POINT_CRITICAL_PCT))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == NEW_AFFECT_POTION_18392 && FindAffect(AFFECT_BLEND, POINT_PENETRATE_PCT))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == NEW_AFFECT_POTION_18393 && FindAffect(AFFECT_BLEND, POINT_ATT_SPEED))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == NEW_AFFECT_POTION_18394 && FindAffect(AFFECT_BLEND, POINT_RESIST_MAGIC))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == NEW_AFFECT_POTION_18395 && FindAffect(AFFECT_BLEND, POINT_ATT_GRADE_BONUS))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == NEW_AFFECT_POTION_13131 && FindAffect(AFFECT_BLEND, POINT_DEF_GRADE_BONUS))
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else
					{
						AddAffect(type, bonus, item->GetValue(2), AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
						item->Lock(true);
						item->SetSocket(0, true);
					}
				}
				else
				{
					RemoveAffect(pAffect);
					item->Lock(false);
					item->SetSocket(0, false);
				}
			}
			break;
#endif

#ifdef ENABLE_UPGRADE_SOCKET_SYSTEM
			case ITEM_UPGRADE_SOCKET_WEAPON:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped())
					return false;

				if (item2->GetType() != ITEM_WEAPON)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_ONLY_WEAPON"));
					return false;
				}

				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_SEALED_CANNOT_MODIFY"));
					return false;
				}

				if (item2->IsBasicItem())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
					return false;
				}

				if (item2->GetLevelLimit() < 55)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_LV65"));
					return false;
				}

				if (item2->GetSocket(3) > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_SOCKET3"));
					return false;
				}

				item2->SetSocket(3, 1);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SUCCESS_OPEN_SOCKET"));
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_UPGRADE_SOCKET_ARMOR:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped())
					return false;

				if (item2->GetType() != ITEM_ARMOR)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_ONLY_ARMOR"));
					return false;
				}

				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_SEALED_CANNOT_MODIFY"));
					return false;
				}

				if (item2->IsBasicItem())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
					return false;
				}

				if (item2->GetLevelLimit() < 55)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_LV65"));
					return false;
				}

				if (item2->GetSocket(3) > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_SOCKET3"));
					return false;
				}

				item2->SetSocket(3, 1);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SUCCESS_OPEN_SOCKET"));
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_UPGRADE_SOCKET_MIX:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped())
					return false;

				if (item2->GetType() != ITEM_ARMOR && item2->GetType() != ITEM_WEAPON)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_ONLY_ARMOR_AND_WEAPON"));
					return false;
				}

				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_SEALED_CANNOT_MODIFY"));
					return false;
				}

				if (item2->IsBasicItem())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
					return false;
				}

				if (item2->GetLevelLimit() < 55)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_LV65"));
					return false;
				}

				if (item2->GetSocket(3) > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("USE_OPEN_SOCKET_WARNING_SOCKET3"));
					return false;
				}

				item2->SetSocket(3, 1);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SUCCESS_OPEN_SOCKET"));
				item->SetCount(item->GetCount() - 1);
			}
			break;
#endif

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
			case ITEM_RARITY_BOX_1:
			case ITEM_RARITY_BOX_2:
			case ITEM_RARITY_BOX_3:
			case ITEM_RARITY_BOX_4:
			{
				if (item->IsExchanging() || item->IsEquipped())
					return false;

				int iValue = item->GetValue(0);
				if (iValue != 0)
				{
					if (GetWear(WEAR_WEAPON) && IS_SET(GetWear(WEAR_WEAPON)->GetFlag(), ITEM_FLAG_RARE_ABILITY))
					{
						if (GetWear(WEAR_WEAPON)->IsSealed() == true)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CANNOT_USE_PACKAGE_RARE_POINTS_BINDED_ITEM"));
							return false;
						}
						if (GetWear(WEAR_WEAPON)->IsBasicItem())
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
							return false;
						}
						if ((GetWear(WEAR_WEAPON)->GetRarePoints() + iValue) > ITEM_RARITY_MAX)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARE_POINTS_MAX"));
							return false;
						}
						GetWear(WEAR_WEAPON)->UpdateRarePoints(iValue);
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GIVE_RARITY_POINTS_BY_BOX%d"), iValue);
						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("THIS_WEAPON_NOT_RARITYABLE"));
						return false;
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARITY_ERROR_SEND_GAME_MASTER"));
					return false;
				}
			}
			break;
#endif
			
			case ITEM_ZODIAC_BOX_FIRST:
			case ITEM_ZODIAC_BOX_SECONDARY:
			{
				int count = 30;
				if (GetZodiacPoint() + count >= ZODIAC_POINT_MAX)
					return false;
				PointChange(POINT_ZODIAC, count);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_AUTO_HP_RECOVERY_S:
			case ITEM_AUTO_HP_RECOVERY_M:
			case ITEM_AUTO_HP_RECOVERY_L:
			case ITEM_AUTO_HP_RECOVERY_X:
			case ITEM_AUTO_SP_RECOVERY_S:
			case ITEM_AUTO_SP_RECOVERY_M:
			case ITEM_AUTO_SP_RECOVERY_L:
			case ITEM_AUTO_SP_RECOVERY_X:
				// ¹«½Ã¹«½ÃÇÏÁö¸¸ ÀÌÀü¿¡ ÇÏ´ø °É °íÄ¡±â´Â ¹«¼·°í...
				// ±×·¡¼­ ±×³É ÇÏµå ÄÚµù. ¼±¹° »óÀÚ¿ë ÀÚµ¿¹°¾à ¾ÆÀÌÅÛµé.
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				EAffectTypes type = AFFECT_NONE;
				bool isSpecialPotion = false;

				switch (item->GetVnum())
				{
				case ITEM_AUTO_HP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_HP_RECOVERY_S:
				case ITEM_AUTO_HP_RECOVERY_M:
				case ITEM_AUTO_HP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
					type = AFFECT_AUTO_HP_RECOVERY;
					break;

				case ITEM_AUTO_SP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_SP_RECOVERY_S:
				case ITEM_AUTO_SP_RECOVERY_M:
				case ITEM_AUTO_SP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
					type = AFFECT_AUTO_SP_RECOVERY;
					break;
				}

				if (AFFECT_NONE == type)
					break;

				if (item->GetCount() > 1)
				{
					int pos = GetEmptyInventory(item->GetSize());

					if (-1 == pos)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
						break;
					}

					item->SetCount(item->GetCount() - 1);

					LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);
					item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

					if (item->GetSocket(1) != 0)
					{
						item2->SetSocket(1, item->GetSocket(1));
					}

					item = item2;
				}

				CAffect* pAffect = FindAffect(type);

				if (nullptr == pAffect)
				{
					EPointTypes bonus = POINT_NONE;

					if (true == isSpecialPotion)
					{
						if (type == AFFECT_AUTO_HP_RECOVERY)
						{
							bonus = POINT_MAX_HP_PCT;
						}
						else if (type == AFFECT_AUTO_SP_RECOVERY)
						{
							bonus = POINT_MAX_SP_PCT;
						}
					}

					AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

					item->Lock(true);
					item->SetSocket(0, true);

					AutoRecoveryItemProcess(type);
				}
				else
				{
					if (item->GetID() == pAffect->dwFlag)
					{
						RemoveAffect(pAffect);

						item->Lock(false);
						item->SetSocket(0, false);
					}
					else
					{
						LPITEM old = FindItemByID(pAffect->dwFlag);

						if (nullptr != old)
						{
							old->Lock(false);
							old->SetSocket(0, false);
						}

						RemoveAffect(pAffect);

						EPointTypes bonus = POINT_NONE;

						if (true == isSpecialPotion)
						{
							if (type == AFFECT_AUTO_HP_RECOVERY)
							{
								bonus = POINT_MAX_HP_PCT;
							}
							else if (type == AFFECT_AUTO_SP_RECOVERY)
							{
								bonus = POINT_MAX_SP_PCT;
							}
						}

						AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

						item->Lock(true);
						item->SetSocket(0, true);

						AutoRecoveryItemProcess(type);
					}
				}
			}
			break;
				}
			break;

		case USE_CLEAR:
		{
			RemoveBadAffect();
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_INVISIBILITY:
		{
			if (item->GetVnum() == 70026)
			{
				quest::CQuestManager& q = quest::CQuestManager::instance();
				quest::PC* pPC = q.GetPC(GetPlayerID());

				if (pPC != nullptr)
				{
					int last_use_time = pPC->GetFlag("mirror_of_disapper.last_use_time");

					if (get_global_time() - last_use_time < 10 * 60)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
						return false;
					}

					pPC->SetFlag("mirror_of_disapper.last_use_time", get_global_time());
				}
			}

			AddAffect(AFFECT_INVISIBILITY, POINT_NONE, 0, AFF_INVISIBILITY, 300, 0, true);
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_POTION_NODELAY:
		{
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				switch (item->GetVnum())
				{
				case 70020:
				case 71018:
				case 71019:
				case 71020:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë Á¦ÇÑ·®À» ÃÊ°úÇÏ¿´½À´Ï´Ù."));
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
			}

			bool used = false;

			if (item->GetValue(0) != 0) // HP Àý´ë°ª È¸º¹
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(1) != 0)	// SP Àý´ë°ª È¸º¹
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (item->GetValue(3) != 0) // HP % È¸º¹
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(4) != 0) // SP % È¸º¹
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (used)
			{
				if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿ùº´ ¶Ç´Â Á¾ÀÚ ¸¦ »ç¿ëÇÏ¿´½À´Ï´Ù"));
					SetUseSeedOrMoonBottleTime();
				}
				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				m_nPotionLimit--;

				//RESTRICT_USE_SEED_OR_MOONBOTTLE
				item->SetCount(item->GetCount() - 1);
				//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
			}
		}
		break;

		case USE_POTION:
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}

				switch (item->GetVnum())
				{
				case 27001:
				case 27002:
				case 27003:
				case 27004:
				case 27005:
				case 27006:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë Á¦ÇÑ·®À» ÃÊ°úÇÏ¿´½À´Ï´Ù."));
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
					return false;
				}
			}

			if (item->GetValue(1) != 0)
			{
				if (GetPoint(POINT_SP_RECOVERY) + GetSP() >= GetMaxSP())
				{
					return false;
				}

				PointChange(POINT_SP_RECOVERY, item->GetValue(1) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_SPUP_BLUE);
			}

			if (item->GetValue(0) != 0)
			{
				if (GetPoint(POINT_HP_RECOVERY) + GetHP() >= GetMaxHP())
				{
					return false;
				}

				PointChange(POINT_HP_RECOVERY, item->GetValue(0) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_HPUP_RED);
			}

			if (GetDungeon())
				GetDungeon()->UsePotion(this);

			if (GetWarMap())
				GetWarMap()->UsePotion(this, item);

			item->SetCount(item->GetCount() - 1);
			m_nPotionLimit--;
			break;

		case USE_POTION_CONTINUE:
		{
			if (item->GetValue(0) != 0)
			{
				AddAffect(AFFECT_HP_RECOVER_CONTINUE, POINT_HP_RECOVER_CONTINUE, item->GetValue(0), 0, item->GetValue(2), 0, true);
			}
			else if (item->GetValue(1) != 0)
			{
				AddAffect(AFFECT_SP_RECOVER_CONTINUE, POINT_SP_RECOVER_CONTINUE, item->GetValue(1), 0, item->GetValue(2), 0, true);
			}
			else
				return false;
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
				if (FindAffect(AFFECT_MOV_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					return false;
				}
				EffectPacket(SE_DXUP_PURPLE); //purple potion
				AddAffect(AFFECT_MOV_SPEED, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true);
				break;

			case APPLY_ATT_SPEED:
				if (FindAffect(AFFECT_ATT_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					return false;
				}
				EffectPacket(SE_SPEEDUP_GREEN); //green potion
				AddAffect(AFFECT_ATT_SPEED, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true);
				break;

			case APPLY_STR:
				AddAffect(AFFECT_STR, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_DEX, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_CON, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_INT, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_CAST_SPEED, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_DEF_GRADE, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_TALISMAN:
		{
			const int TOWN_PORTAL = 1;
			const int MEMORY_PORTAL = 2;

			// gm_guild_build, oxevent ¸Ê¿¡¼­ ±ÍÈ¯ºÎ ±ÍÈ¯±â¾ïºÎ ¸¦ »ç¿ë¸øÇÏ°Ô ¸·À½
			if (GetMapIndex() == 200 || GetMapIndex() == 113)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÇöÀç À§Ä¡¿¡¼­ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·Ã Áß¿¡´Â ÀÌ¿ëÇÒ ¼ö ¾ø´Â ¹°Ç°ÀÔ´Ï´Ù."));
				return false;
			}

			if (m_pkWarpEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌµ¿ÇÒ ÁØºñ°¡ µÇ¾îÀÖÀ½À¸·Î ±ÍÈ¯ºÎ¸¦ »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù"));
				return false;
			}

			// CONSUME_LIFE_WHEN_USE_WARP_ITEM
			int consumeLife = CalculateConsume(this);

			if (consumeLife < 0)
				return false;
			// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

			if (item->GetValue(0) == TOWN_PORTAL) // ±ÍÈ¯ºÎ
			{
				if (item->GetSocket(0) == 0)
				{
					if (!GetDungeon())
						if (!GiveRecallItem(item))
							return false;

					PIXEL_POSITION posWarp;

					if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp))
					{
						// CONSUME_LIFE_WHEN_USE_WARP_ITEM
						PointChange(POINT_HP, -consumeLife, false);
						// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

						WarpSet(posWarp.x, posWarp.y);
					}
					else
					{
						sys_err("CHARACTER::UseItem : cannot find spawn position (name %s, %d x %d)", GetName(), GetX(), GetY());
					}
				}
				else
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿ø·¡ À§Ä¡·Î º¹±Í"));

					ProcessRecallItem(item);
				}
			}
			else if (item->GetValue(0) == MEMORY_PORTAL) // ±ÍÈ¯±â¾ïºÎ
			{
				if (item->GetSocket(0) == 0)
				{
					if (GetDungeon())
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´øÀü ¾È¿¡¼­´Â %s%s »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), item->GetName(), "");
						return false;
					}

					if (!GiveRecallItem(item))
						return false;
				}
				else
				{
					// CONSUME_LIFE_WHEN_USE_WARP_ITEM
					PointChange(POINT_HP, -consumeLife, false);
					// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

					ProcessRecallItem(item);
				}
			}
		}
		break;

		case USE_TUNING:
		case USE_DETACHMENT:
		{
			LPITEM item2;

			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsExchanging())
				return false;

			if (item2->IsEquipped())
				return false;

			if (item2->GetVnum() >= 28330 && item2->GetVnum() <= 28343) // ¿µ¼®+3
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("+3 ¿µ¼®Àº ÀÌ ¾ÆÀÌÅÛÀ¸·Î °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù"));
				return false;
			}

			TItemTable* table = ITEM_MANAGER::instance().GetTable(item2->GetVnum());

			for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
			{
				if (LIMIT_REAL_TIME == table->aLimits->bType && item2->GetType() == ITEM_WEAPON)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cannot use detachment time item."));
					return false;
				}
			}
			
			if (item2->GetType() == ITEM_SOUL)
			{
				if (item->GetValue(0) != SOUL_SCROLL)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_RUH_PARSOMENI_ILE_YUKSELTEBILIRSIN"));
					return false;
				}
			}

#ifdef ENABLE_CHANGELOOK_SYSTEM
			if (item->GetValue(0) == CL_CLEAN_ATTR_VALUE0)
			{
				if (!CleanTransmutation(item, item2))
					return false;

				return true;
			}
#endif

#ifdef ENABLE_ACCE_SYSTEM
			if (item->GetValue(0) == SASH_CLEAN_ATTR_VALUE0)
			{
				if (!CleanSashAttr(item, item2))
					return false;

				return true;
			}
#endif

			if (item->GetValue(0) == RITUALS_SCROLL)
			{
				if (item2->GetLevelLimit() < item->GetValue(1))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("you can only upgrade items above level %d"), item->GetValue(1));
					return false;
				}
				else
					RefineItem(item, item2);
			}

			if (item2->GetVnum() >= 28430 && item2->GetVnum() <= 28443)  // ¿µ¼®+4
			{
				if (item->GetVnum() == 71056) // Ã»·æÀÇ¼û°á
				{
					RefineItem(item, item2);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿µ¼®Àº ÀÌ ¾ÆÀÌÅÛÀ¸·Î °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù"));
				}
			}
			else
			{
				RefineItem(item, item2);
			}
		}
		break;

#ifdef ENABLE_COSTUME_ENCHANT_SYSTEM
		case USE_CHANGE_COSTUME_ATTR:
		case USE_RESET_COSTUME_ATTR:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
				return false;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			if (item2->IsSealed())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_SEALED_CANNOT_MODIFY"));
				return false;
			}
#endif

			if (item2->IsBasicItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
				return false;
			}

			if (item2->GetType() != ITEM_COSTUME)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ç×¸ñ°ú Ç×¸ñÀÇ ¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}

			if (item2->GetSubType() == COSTUME_SASH)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ Ç×¸ñ°ú Ç×¸ñÀÇ ¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}

			if (item2->IsQuestHair() == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_CANNOT_USE_THIS_ITEM_QUEST_HAIR"));
				return false;
			}

			if (item2->GetAttributeSetIndex() == -1)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
				return false;
			}

			switch (item->GetSubType())
			{
			case USE_CHANGE_COSTUME_ATTR:
			{
				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯°æÇÒ ¼Ó¼ºÀÌ ¾ø½À´Ï´Ù."));
					return false;
				}

				item2->ChangeAttribute(aiCostumeAttributeLevelPercent);

				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÏ¿´½À´Ï´Ù."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_COSTUME_ATTR", buf);
				}

				item->SetCount(item->GetCount() - 1);
			}
			break;

			case USE_RESET_COSTUME_ATTR:
			{
				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯°æÇÒ ¼Ó¼ºÀÌ ¾ø½À´Ï´Ù."));
					return false;
				}

				item2->ClearAttribute();

				char buf[21];
				snprintf(buf, sizeof(buf), "%u", item2->GetID());

				BYTE i;
				for (i = 0; i < COSTUME_ATTRIBUTE_MAX_NUM; i++)
				{
					char result[64];
					if (number(1, 100) <= aiCostumeAttributeAddPercent[item2->GetAttributeCount()])
					{
						snprintf(result, sizeof(result), "RESET_COSTUME_ATTR_SUCCESS%d", i);
						item2->AddAttribute();
						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							result,
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum());
					}
					else
					{
						snprintf(result, sizeof(result), "RESET_COSTUME_ATTR_FAIL%d", i);
						LogManager::instance().ItemLog(this, item, result, buf);
						break;
					}
				}

				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÏ¿´½À´Ï´Ù."));
				item->SetCount(item->GetCount() - 1);
			}
			}
		}
		break;
#endif

		//  ACCESSORY_REFINE & ADD/CHANGE_ATTRIBUTES
		case USE_PUT_INTO_BELT_SOCKET:
		case USE_PUT_INTO_RING_SOCKET:
		case USE_PUT_INTO_ACCESSORY_SOCKET:
		case USE_ADD_ACCESSORY_SOCKET:
		case USE_CLEAN_SOCKET:
		case USE_CHANGE_ATTRIBUTE:
		case USE_CHANGE_ATTRIBUTE2:
		case USE_ADD_ATTRIBUTE:
		case USE_ADD_ATTRIBUTE2:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}

			// [NOTE] ÄÚ½ºÆ¬ ¾ÆÀÌÅÛ¿¡´Â ¾ÆÀÌÅÛ ÃÖÃÊ »ý¼º½Ã ·£´ý ¼Ó¼ºÀ» ºÎ¿©ÇÏµÇ, Àç°æÀç°¡ µîµîÀº ¸·¾Æ´Þ¶ó´Â ¿äÃ»ÀÌ ÀÖ¾úÀ½.
			// ¿ø·¡ ANTI_CHANGE_ATTRIBUTE °°Àº ¾ÆÀÌÅÛ Flag¸¦ Ãß°¡ÇÏ¿© ±âÈ¹ ·¹º§¿¡¼­ À¯¿¬ÇÏ°Ô ÄÁÆ®·Ñ ÇÒ ¼ö ÀÖµµ·Ï ÇÒ ¿¹Á¤ÀÌ¾úÀ¸³ª
			// ±×µý°Å ÇÊ¿ä¾øÀ¸´Ï ´ÚÄ¡°í »¡¸® ÇØ´Þ·¡¼­ ±×³É ¿©±â¼­ ¸·À½... -_-
			if (ITEM_COSTUME == item2->GetType())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
				return false;
			}

			if (item2->IsExchanging())
				return false;

			if (item2->IsEquipped())
				return false;

			switch (item->GetSubType())
			{
			case USE_CLEAN_SOCKET:
			{
				int i;
				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				{
					if (item2->GetSocket(i) == ITEM_BROKEN_METIN_VNUM)
						break;
				}

				if (i == ITEM_SOCKET_MAX_NUM)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã»¼ÒÇÒ ¼®ÀÌ ¹ÚÇôÀÖÁö ¾Ê½À´Ï´Ù."));
					return false;
				}

				int j = 0;

				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				{
					if (item2->GetSocket(i) != ITEM_BROKEN_METIN_VNUM && item2->GetSocket(i) != 0)
						item2->SetSocket(j++, item2->GetSocket(i));
				}

				for (; j < ITEM_SOCKET_MAX_NUM; ++j)
				{
					if (item2->GetSocket(j) > 0)
						item2->SetSocket(j, 1);
				}

				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CLEAN_SOCKET", buf);
				}

				item->SetCount(item->GetCount() - 1);
			}
			break;

			case USE_CHANGE_ATTRIBUTE:

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
					return false;
				}
#endif
				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
					return false;
				}

				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("º¯°æÇÒ ¼Ó¼ºÀÌ ¾ø½À´Ï´Ù."));
					return false;
				}

				if (item->GetSubType() == USE_CHANGE_ATTRIBUTE2)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 0, 30, 40, 3
					};

					item2->ChangeAttribute(aiChangeProb);
				}
				else if (item->GetVnum() == 76014)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 10, 50, 39, 1
					};

					item2->ChangeAttribute(aiChangeProb);
				}
#ifdef ENABLE_TALISMAN_ATTR
				else if (item->GetVnum() == 19983)
				{
					if (item2->GetType() == ITEM_UNIQUE && item2->GetSubType() == USE_CHARM)
					{
						item2->ChangeAttribute();
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_TILSIMLARDA_KULLANILIR"));
						return false;
					}
				}
				else if (item2->GetType() == ITEM_UNIQUE && item2->GetSubType() == USE_CHARM)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_TILSIMLARDA_KULLANILIR"));
					return false;
				}
#endif
				else
				{
					// ¿¬Àç°æ Æ¯¼öÃ³¸®
					// Àý´ë·Î ¿¬Àç°¡ Ãß°¡ ¾ÈµÉ°Å¶ó ÇÏ¿© ÇÏµå ÄÚµùÇÔ.
					if (item->GetVnum() == 71151 || item->GetVnum() == 76023)
					{
						if ((item2->GetType() == ITEM_WEAPON)
							|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àû¿ë ·¹º§º¸´Ù ³ô¾Æ »ç¿ëÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù."));
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹«±â¿Í °©¿Ê¿¡¸¸ »ç¿ë °¡´ÉÇÕ´Ï´Ù."));
							break;
						}
					}
					item2->ChangeAttribute();
				}
				


				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÏ¿´½À´Ï´Ù."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
				}

				item->SetCount(item->GetCount() - 1);
				break;

			case USE_ADD_ATTRIBUTE:
				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
					return false;
				}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
					return false;
				}
#endif

				if (item2->GetAttributeCount() < 4)
				{
					// ¿¬Àç°¡ Æ¯¼öÃ³¸®
					// Àý´ë·Î ¿¬Àç°¡ Ãß°¡ ¾ÈµÉ°Å¶ó ÇÏ¿© ÇÏµå ÄÚµùÇÔ.
					if (item->GetVnum() == 71152 || item->GetVnum() == 76024)
					{
						if ((item2->GetType() == ITEM_WEAPON)
							|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àû¿ë ·¹º§º¸´Ù ³ô¾Æ »ç¿ëÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù."));
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹«±â¿Í °©¿Ê¿¡¸¸ »ç¿ë °¡´ÉÇÕ´Ï´Ù."));
							break;
						}
					}
					
#ifdef ENABLE_TALISMAN_ATTR
					if (item->GetVnum() == 19984)
					{
						if (item2->GetType() == ITEM_UNIQUE && item2->GetSubType() == USE_CHARM)
						{
							item2->AddAttribute();
							item->SetCount(item->GetCount() - 1);
							return true;
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_TILSIMLARDA_KULLANILIR"));
							return false;
						}
					}
					else if (item2->GetType() == ITEM_UNIQUE && item2->GetSubType() == USE_CHARM)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SADECE_TILSIMLARDA_KULLANILIR"));
						return false;
					}
#endif
					
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					if(attr_always_add == true)
					{
						aiItemAttributeAddPercent[0] = 100;
						aiItemAttributeAddPercent[1] = 100;
						aiItemAttributeAddPercent[2] = 100;
						aiItemAttributeAddPercent[3] = 100;
					}
					if (attr_always_5_add == true) {
						aiItemAttributeAddPercent[4] = 100;
					}
					if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
					{
#ifdef ENABLE_ATTR_RENEWAL
						while (item2->GetAttributeCount() < 5)
						{
							item2->AddAttribute();
						}
#else
						item2->AddAttribute();
#endif
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼º Ãß°¡¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));

						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							"ADD_ATTRIBUTE_SUCCESS",
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum());
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼º Ãß°¡¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
						LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE_FAIL", buf);
					}

					item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õÀÌ»ó ÀÌ ¾ÆÀÌÅÛÀ» ÀÌ¿ëÇÏ¿© ¼Ó¼ºÀ» Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				}
				break;

			case USE_ADD_ATTRIBUTE2:
				// Ãàº¹ÀÇ ±¸½½
				// Àç°¡ºñ¼­¸¦ ÅëÇØ ¼Ó¼ºÀ» 4°³ Ãß°¡ ½ÃÅ² ¾ÆÀÌÅÛ¿¡ ´ëÇØ¼­ ÇÏ³ªÀÇ ¼Ó¼ºÀ» ´õ ºÙ¿©ÁØ´Ù.
				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼ºÀ» º¯°æÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
					return false;
				}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
					return false;
				}
#endif

				// ¼Ó¼ºÀÌ ÀÌ¹Ì 4°³ Ãß°¡ µÇ¾úÀ» ¶§¸¸ ¼Ó¼ºÀ» Ãß°¡ °¡´ÉÇÏ´Ù.
				if (item2->GetAttributeCount() == 4)
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
					{
						item2->AddAttribute();
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼º Ãß°¡¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));

						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							"ADD_ATTRIBUTE2_SUCCESS",
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum());
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼Ó¼º Ãß°¡¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
						LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE2_FAIL", buf);
					}

					item->SetCount(item->GetCount() - 1);
				}
				else if (item2->GetAttributeCount() == 5)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´õ ÀÌ»ó ÀÌ ¾ÆÀÌÅÛÀ» ÀÌ¿ëÇÏ¿© ¼Ó¼ºÀ» Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				}
				else if (item2->GetAttributeCount() < 4)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ÕÀú Àç°¡ºñ¼­¸¦ ÀÌ¿ëÇÏ¿© ¼Ó¼ºÀ» Ãß°¡½ÃÄÑ ÁÖ¼¼¿ä."));
				}
				else
				{
					// wtf ?!
					sys_err("ADD_ATTRIBUTE2 : Item has wrong AttributeCount(%d)", item2->GetAttributeCount());
				}
				break;

			case USE_ADD_ACCESSORY_SOCKET:
			{
				char buf[21];
				snprintf(buf, sizeof(buf), "%u", item2->GetID());

				if (item2->IsAccessoryForSocket())
				{
					if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
					{
						if (number(1, 100) <= 100)
						{
							item2->SetAccessorySocketMaxGrade(item2->GetAccessorySocketMaxGrade() + 1);
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÄÏÀÌ ¼º°øÀûÀ¸·Î Ãß°¡µÇ¾ú½À´Ï´Ù."));
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÄÏ Ãß°¡¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_FAIL", buf);
						}

						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾×¼¼¼­¸®¿¡´Â ´õÀÌ»ó ¼ÒÄÏÀ» Ãß°¡ÇÒ °ø°£ÀÌ ¾ø½À´Ï´Ù."));
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀ¸·Î ¼ÒÄÏÀ» Ãß°¡ÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
				}
			}
			break;

			case USE_PUT_INTO_BELT_SOCKET:
			case USE_PUT_INTO_ACCESSORY_SOCKET:
#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
				if ( item2->IsAccessoryForSocket() && item->CanPutInto(item2) || item->CanPutIntoNew(item2))
#else
				if (item2->IsAccessoryForSocket() && item->CanPutInto(item2))
#endif
				{
#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
					if (item2->GetSocket(item2->GetAccessorySocketGrade()) == 31)
					{
						if (item->GetVnum() >= 50621 && item->GetVnum() <= 50640)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PERMA_CEVHER_VARKEN_BASAMAZSIN"));
							return false;
						}
					}
#endif
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (item2->GetAccessorySocketGrade() < item2->GetAccessorySocketMaxGrade())
					{
						if (number(1, 100) <= aiAccessorySocketPutPct[item2->GetAccessorySocketGrade()])
						{
#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
							if (item2->GetAccessorySocketGrade() == 0)
							{
								if (item->GetVnum() >= 50641 && item->GetVnum() <= 50647)
									item2->SetSocket(2,31);
								
								if (item->GetVnum() == 18901)
									item2->SetSocket(2,31);
							}
							if (item2->GetSocket(2) == 31 && item->GetVnum() >= 50641 && item->GetVnum() <= 50647)
							{
								item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1, true);
								item->SetCount(item->GetCount() - 1);
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
								return true;
							}
							else if (item2->GetSocket(2) == 31 && item->GetVnum() == 18901)
							{
								item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1, true);
								item->SetCount(item->GetCount() - 1);
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
								return true;
							}
							else if (item2->GetSocket(2) != 31 && item->GetVnum() >= 50634 && item->GetVnum() <= 50640)
							{
								item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1, false);
								item->SetCount(item->GetCount() - 1);
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
								return true;
							}
							else if (item2->GetSocket(2) != 31 && item->GetVnum() == 18901)
							{
								// boþ devam edecek
							}
							else if (item2->GetSocket(2) != 31 && !(item->GetVnum() >= 50641 && item->GetVnum() <= 50647))
							{
								item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1, false);
								item->SetCount(item->GetCount() - 1);
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
								return true;
							}
#else
							item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1);
#endif
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀåÂø¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_FAIL", buf);
						}

						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						if (item2->GetAccessorySocketMaxGrade() == 0)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ÕÀú ´ÙÀÌ¾Æ¸óµå·Î ¾Ç¼¼¼­¸®¿¡ ¼ÒÄÏÀ» Ãß°¡ÇØ¾ßÇÕ´Ï´Ù."));
						else if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾×¼¼¼­¸®¿¡´Â ´õÀÌ»ó ÀåÂøÇÒ ¼ÒÄÏÀÌ ¾ø½À´Ï´Ù."));
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ÙÀÌ¾Æ¸óµå·Î ¼ÒÄÏÀ» Ãß°¡ÇØ¾ßÇÕ´Ï´Ù."));
						}
						else
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾×¼¼¼­¸®¿¡´Â ´õÀÌ»ó º¸¼®À» ÀåÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀ» ÀåÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
				}
				break;
			}
			if (item2->IsEquipped())
			{
				BuffOnAttr_AddBuffsFromItem(item2);
			}
		}
		break;
		//  END_OF_ACCESSORY_REFINE & END_OF_ADD_ATTRIBUTES & END_OF_CHANGE_ATTRIBUTES

		case USE_BAIT:
		{
			if (m_pkFishingEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³¬½Ã Áß¿¡ ¹Ì³¢¸¦ °¥¾Æ³¢¿ï ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}

			LPITEM weapon = GetWear(WEAR_WEAPON);

			if (!weapon || weapon->GetType() != ITEM_ROD)
				return false;

			if (weapon->GetSocket(2))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ²ÈÇôÀÖ´ø ¹Ì³¢¸¦ »©°í %s¸¦ ³¢¿ó´Ï´Ù."), item->GetName());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³¬½Ã´ë¿¡ %s¸¦ ¹Ì³¢·Î ³¢¿ó´Ï´Ù."), item->GetName());
			}

			weapon->SetSocket(2, item->GetValue(0));
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_MOVE:
		case USE_TREASURE_BOX:
		case USE_MONEYBAG:
			break;

		case USE_AFFECT:
		{
			if (item->GetValue(1) == 64 || item->GetValue(1) == 65 || item->GetValue(1) == 69 || item->GetValue(1) == 70 || item->GetValue(1) == 15 || item->GetValue(1) == 16)
			{
				CAffect* pAffect = FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType);
				
				if (pAffect && item->GetSocket(1) == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LUTFEN_DIGER_TANRIYI_KAPAT"));
					return true;
				}
				else if (pAffect && item->GetSocket(1) != 0)
				{
					if (pAffect)
						RemoveAffect(pAffect);
					item->SetSocket(1, 0);
				}
				else
				{
					if (CheckPotionItem(item->GetValue(1)) == true)
						return true;

					AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1, 1);
				}
				return true;
			}
			else
			{
				if (FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
				}
				else
				{
					AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false);
					item->SetCount(item->GetCount() - 1);
				}
			}
		}
		break;

		case USE_CREATE_STONE:
			AutoGiveItem(number(28030, 28045));
			item->SetCount(item->GetCount() - 1);
			break;

			// ¹°¾à Á¦Á¶ ½ºÅ³¿ë ·¹½ÃÇÇ Ã³¸®
		case USE_RECIPE:
		{
			LPITEM pSource1 = FindSpecifyItem(item->GetValue(1));
			DWORD dwSourceCount1 = item->GetValue(2);

			LPITEM pSource2 = FindSpecifyItem(item->GetValue(3));
			DWORD dwSourceCount2 = item->GetValue(4);

			if (dwSourceCount1 != 0)
			{
				if (pSource1 == nullptr)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹°¾à Á¶ÇÕÀ» À§ÇÑ Àç·á°¡ ºÎÁ·ÇÕ´Ï´Ù."));
					return false;
				}
			}

			if (dwSourceCount2 != 0)
			{
				if (pSource2 == nullptr)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹°¾à Á¶ÇÕÀ» À§ÇÑ Àç·á°¡ ºÎÁ·ÇÕ´Ï´Ù."));
					return false;
				}
			}

			if (pSource1 != nullptr)
			{
				if (pSource1->GetCount() < dwSourceCount1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àç·á(%s)°¡ ºÎÁ·ÇÕ´Ï´Ù."), pSource1->GetName());
					return false;
				}

				pSource1->SetCount(pSource1->GetCount() - dwSourceCount1);
			}

			if (pSource2 != nullptr)
			{
				if (pSource2->GetCount() < dwSourceCount2)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àç·á(%s)°¡ ºÎÁ·ÇÕ´Ï´Ù."), pSource2->GetName());
					return false;
				}

				pSource2->SetCount(pSource2->GetCount() - dwSourceCount2);
			}

			LPITEM pBottle = FindSpecifyItem(50901);

			if (!pBottle || pBottle->GetCount() < 1)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ºó º´ÀÌ ¸ðÀÚ¸¨´Ï´Ù."));
				return false;
			}

			pBottle->SetCount(pBottle->GetCount() - 1);

			if (number(1, 100) > item->GetValue(5))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹°¾à Á¦Á¶¿¡ ½ÇÆÐÇß½À´Ï´Ù."));
				return false;
			}

			AutoGiveItem(item->GetValue(0));
		}
		break;
			}
		}
	break;

	case ITEM_METIN:
	{
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		TItemTable* table = ITEM_MANAGER::instance().GetTable(item2->GetVnum());

		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
		{
			if (LIMIT_REAL_TIME == table->aLimits->bType)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cannot add metin stones time item."));
				return false;
			}
		}

		if (item2->IsEquipped())
			return false;

		if (item2->GetType() == ITEM_PICK) return false;
		if (item2->GetType() == ITEM_ROD) return false;

		int i;

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			DWORD dwVnum;

			if ((dwVnum = item2->GetSocket(i)) <= 2)
				continue;

			TItemTable * p = ITEM_MANAGER::instance().GetTable(dwVnum);

			if (!p)
				continue;

			if (item->GetValue(5) == p->alValues[5])
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°°Àº Á¾·ùÀÇ ¸ÞÆ¾¼®Àº ¿©·¯°³ ºÎÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
		}

		if (item2->GetType() == ITEM_ARMOR)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_BODY) || !IS_SET(item2->GetWearFlag(), WEARABLE_BODY))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¸ÞÆ¾¼®Àº Àåºñ¿¡ ºÎÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
		}
		else if (item2->GetType() == ITEM_WEAPON)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_WEAPON))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¸ÞÆ¾¼®Àº ¹«±â¿¡ ºÎÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ºÎÂøÇÒ ¼ö ÀÖ´Â ½½·ÔÀÌ ¾ø½À´Ï´Ù."));
			return false;
		}

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			if (item2->GetSocket(i) >= 1 && item2->GetSocket(i) <= 2 && item2->GetSocket(i) >= item->GetValue(2))
			{
				int prob = stone_always_add == true ? 100 : 50;
				if (number(1, 100) <= prob)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ÞÆ¾¼® ºÎÂø¿¡ ¼º°øÇÏ¿´½À´Ï´Ù."));
					item2->SetSocket(i, item->GetVnum());
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸ÞÆ¾¼® ºÎÂø¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
					item2->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}

				LogManager::instance().ItemLog(this, item2, "SOCKET", item->GetName());
				item->SetCount(item->GetCount() - 1);
				break;
			}

		if (i == ITEM_SOCKET_MAX_NUM)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ºÎÂøÇÒ ¼ö ÀÖ´Â ½½·ÔÀÌ ¾ø½À´Ï´Ù."));
	}
	break;

	case ITEM_AUTOUSE:
	case ITEM_MATERIAL:
	case ITEM_SPECIAL:
	case ITEM_TOOL:
	case ITEM_LOTTERY:
		break;

	case ITEM_TOTEM:
	{
		if (!item->IsEquipped())
			EquipItem(item);
	}
	break;

	case ITEM_BLEND:
		// »õ·Î¿î ¾àÃÊµé
		sys_log(0, "ITEM_BLEND!!");
		if (Blend_Item_find(item->GetVnum()))
		{
			int		affect_type = AFFECT_BLEND;
			if (item->GetSocket(0) >= _countof(aApplyInfo))
			{
				sys_err("INVALID BLEND ITEM(id : %d, vnum : %d). APPLY TYPE IS %d.", item->GetID(), item->GetVnum(), item->GetSocket(0));
				return false;
			}
			int		apply_type = aApplyInfo[item->GetSocket(0)].bPointType;
			int		apply_value = item->GetSocket(1);
			int		apply_duration = item->GetSocket(2);

			if (FindAffect(affect_type, apply_type))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
			}
			else
			{
				if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, POINT_RESIST_MAGIC))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
				}
				else
				{
#ifdef ENABLE_PERMA_BLEND_SYSTEM
					if (item->GetVnum() == 50821 && FindAffect(AFFECT_18391, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == 50822 && FindAffect(AFFECT_18392, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == 50823 && FindAffect(AFFECT_18393, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == 50824 && FindAffect(AFFECT_18394, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == 50825 && FindAffect(AFFECT_18395, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else if (item->GetVnum() == 50826 && FindAffect(AFFECT_13131, apply_type) != nullptr)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì È¿°ú°¡ °É·Á ÀÖ½À´Ï´Ù."));
					else
#endif
					{
#ifdef ENABLE_BLEND_ICON_SYSTEM
						SetAffectPotion(item);
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, false);
						item->SetCount(item->GetCount() - 1);
					}
				}
			}
		}
		break;

	case ITEM_EXTRACT:
	{
		LPITEM pDestItem = GetItem(DestCell);
		if (nullptr == pDestItem)
		{
			return false;
		}
		switch (item->GetSubType())
		{
		case EXTRACT_DRAGON_SOUL:
			if (pDestItem->IsDragonSoul())
			{
				return DSManager::instance().PullOut(this, NPOS, pDestItem, item);
			}
			return false;
		case EXTRACT_DRAGON_HEART:
			if (item->GetVnum() == 100700 || item->GetVnum() == 100701)
			{
				if (pDestItem->IsDragonSoul())
				{
					if (DSManager::instance().IsActiveDragonSoul(pDestItem) == true)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_GIYILIYOR"));
						return false;
					}

					pDestItem->SetForceAttribute(0, 0, 0);
					pDestItem->SetForceAttribute(1, 0, 0);
					pDestItem->SetForceAttribute(2, 0, 0);
					pDestItem->SetForceAttribute(3, 0, 0);
					pDestItem->SetForceAttribute(4, 0, 0);
					pDestItem->SetForceAttribute(5, 0, 0);
					pDestItem->SetForceAttribute(6, 0, 0);

					bool ret = DSManager::instance().PutAttributes(pDestItem);
					if (ret == true)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_DEGISTIRME_BASARILI!"));
						item->SetCount(item->GetCount() - 1);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_DEGISTIRME_BASARISIZ!"));
					}
				}
			}
			else
			{
				if (pDestItem->IsDragonSoul())
				{
					return DSManager::instance().ExtractDragonHeart(this, pDestItem, item);
				}
			}
			return false;
		default:
			return false;
		}
	}
	break;
#ifdef ENABLE_ITEM_GACHA_SYSTEM
	case ITEM_GACHA:
	{
		DWORD dwBoxVnum = item->GetVnum();
		std::vector <DWORD> dwVnums;
		std::vector <DWORD> dwCounts;
		std::vector <LPITEM> item_gets(0);
		int count = 0;

		if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
		{
			if (item->GetSocket(0) > 1)
				item->SetSocket(0, item->GetSocket(0) - 1);
			else
				item->SetCount(item->GetCount() - 1);
		}
	}
	break;
#endif

	case ITEM_SOUL:
	{
		switch (item->GetSubType())
		{
		case RED_SOUL:
		{
			if (item->GetSocket(1) != 0) // tiklanmis
			{
				if (!IsAffectFlag(AFF_RED_SOUL))
				{
					item->SetSocket(1, 0);
					item->Lock(false);
				}
				
				if (IsAffectFlag(AFF_MIX_SOUL))
				{
					if (CheckSoulItem(true) == true)
					{
						RemoveAffect(AFFECT_SOUL);
						AddAffect(AFFECT_SOUL, 0, 0, AFF_BLUE_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 0);
						item->Lock(false);
						return true;
					}
					else if (CheckSoulItem(false) == true)
					{
						RemoveAffect(AFFECT_SOUL);
						AddAffect(AFFECT_SOUL, 0, 0, AFF_RED_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 1);
						item->Lock(true);
						return true;
					}
					else
					{
						RemoveAffect(AFFECT_SOUL);
						item->SetSocket(1, 0);
						item->Lock(false);
						return true;
					}
				}
				else if (IsAffectFlag(AFF_BLUE_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					if (IsAffectFlag(AFF_RED_SOUL))
					{
						AddAffect(AFFECT_SOUL, 0, 0, AFF_RED_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 1);
						item->Lock(true);
						return true;
					}
					item->SetSocket(1,0);
					item->Lock(false);
					return true;
				}
				else if (IsAffectFlag(AFF_RED_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					item->SetSocket(1,0);
					item->Lock(false);
					return true;
				}
			}
			else // tiklanmamis
			{
				if (IsAffectFlag(AFF_RED_SOUL))
					return true;
				
				if (IsAffectFlag(AFF_BLUE_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					AddAffect(AFFECT_SOUL, 0, 0, AFF_MIX_SOUL, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1,1);
					item->Lock(true);
					return true;
				}
				else if (!IsAffectFlag(AFF_MIX_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					AddAffect(AFFECT_SOUL, 0, 0, AFF_RED_SOUL, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1,1);
					item->Lock(true);
					return true;
				}
			}
		}
		break;
		case BLUE_SOUL:
		{
			if (item->GetSocket(1) != 0) // tiklanmis
			{
				if (IsAffectFlag(AFF_MIX_SOUL))
				{
					if (CheckSoulItem(false) == true)
					{
						RemoveAffect(AFFECT_SOUL);
						AddAffect(AFFECT_SOUL, 0, 0, AFF_RED_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 0);
						item->Lock(false);
						return true;
					}
					else if (CheckSoulItem(true) == true)
					{
						RemoveAffect(AFFECT_SOUL);
						AddAffect(AFFECT_SOUL, 0, 0, AFF_BLUE_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 1);
						item->Lock(true);
						return true;
					}
					else
					{
						RemoveAffect(AFFECT_SOUL);
						item->SetSocket(1, 0);
						item->Lock(false);
						return true;
					}
				}
				else if (IsAffectFlag(AFF_RED_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					if (IsAffectFlag(AFF_BLUE_SOUL))
					{
						AddAffect(AFFECT_SOUL, 0, 0, AFF_BLUE_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						item->SetSocket(1, 1);
						item->Lock(true);
						return true;
					}
					item->SetSocket(1,0);
					item->Lock(false);
					return true;
				}
				else if (IsAffectFlag(AFF_BLUE_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					item->SetSocket(1,0);
					item->Lock(false);
					return true;
				}
			}
			else // tiklanmamis
			{
				if (IsAffectFlag(AFF_BLUE_SOUL))
					return true;
				
				if (IsAffectFlag(AFF_RED_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					AddAffect(AFFECT_SOUL, 0, 0, AFF_MIX_SOUL, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1,1);
					item->Lock(true);
					return true;
				}
				else if (!IsAffectFlag(AFF_MIX_SOUL))
				{
					RemoveAffect(AFFECT_SOUL);
					AddAffect(AFFECT_SOUL, 0, 0, AFF_BLUE_SOUL, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1,1);
					item->Lock(true);
					return true;
				}
			}
		}
		break;
		}
	}
	break;
	
#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
	case ITEM_SPECIAL_GACHA:
		{
			return true; // daha sonra acilacak.
			SetSpecialGachaItemVnum(item->GetVnum());
			char buf[256];
			snprintf(buf, sizeof(buf), "%d_special_gacha", item->GetVnum());
			if (int(GetQuestFlag(buf)) == 0)
			{
				std::vector<TChestDropInfoTable> vec_ItemList;
				ITEM_MANAGER::instance().GetChestItemList(GetSpecialGachaItemVnum(), vec_ItemList);
				
				if (vec_ItemList.size() == 0)
					return false;
				
				DWORD dwSendItemVnum = 0;
				DWORD dwSendItemCount = 0;
				int col = 0;
				do
				{
					TChestDropInfoTable pkmem = vec_ItemList.at(col);
					if (pkmem.dwItemVnum != 0)
					{
						int iProb = number(1,100);
						if (iProb < 35);
						{
							dwSendItemVnum = pkmem.dwItemVnum;
							dwSendItemCount = pkmem.bItemCount;
						}
					}
					col++;
				} while(dwSendItemVnum != 0);
				
				SetSpecialGachaItemVnumGive(dwSendItemVnum);
				SetSpecialGachaItemCountGive(dwSendItemCount);
				ChatPacket(CHAT_TYPE_COMMAND, "Binary_LuckyBoxAppend %d %d %d", dwSendItemVnum, dwSendItemCount, 1000000);
			}
			else
				ChatPacket(CHAT_TYPE_COMMAND, "Binary_LuckyBoxAppend %d %d %d", GetSpecialGachaItemVnumGive(), GetSpecialGachaItemCountGive(), GetQuestFlag(buf));
		}
		break;
#endif

	case ITEM_NONE:
		sys_err("Item type NONE %s", item->GetName());
		break;

	default:
		sys_log(0, "UseItemEx: Unknown type %s %d", item->GetName(), item->GetType());
		return false;
	}

	return true;
	}

int g_nPortalLimitTime = 10;

bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell)
{
	//WORD wCell = Cell.cell;
	//WORD wDestCell = DestCell.cell;
	//BYTE bDestInven = DestCell.window_type;
	LPITEM item = nullptr;

	if (!CanHandleItem())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;
	
	//We don't want to use it if we are dragging it over another item of the same type...
	CItem * destItem = GetItem(DestCell);
	if (destItem && item != destItem && destItem->IsStackable() && !IS_SET(destItem->GetAntiFlag(), ITEM_ANTIFLAG_STACK) && destItem->GetVnum() == item->GetVnum())
	{
		if (MoveItem(Cell, DestCell, 0))
			return false;
	}

	if (ITEM_BELT == item->GetType())
		ERROR_MSG(CBeltInventoryHelper::IsExistItemInBeltInventory(this), LC_TEXT("Error text Belt1"));
	//sys_log(0, "%s: USE_ITEM %s (inven %d, cell: %d)", GetName(), item->GetName(), window_type, wCell);

	if (item->IsExchanging())
		return false;

	if (!item->CanUsedBy(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁ÷ÀÌ ¸ÂÁö¾Ê¾Æ ÀÌ ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (IsStun())
		return false;

	if (false == FN_check_item_sex(this, item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ÀÌ ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (!CCombatZoneManager::instance().CanUseItem(this, item))
		return false;
#endif

	//PREVENT_TRADE_WINDOW
	if (IS_SUMMON_ITEM(item->GetVnum()))
	{
		if (false == IS_SUMMONABLE_ZONE(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}

		// °æÈ¥¹ÝÁö »ç¿ëÁö »ó´ë¹æÀÌ SUMMONABLE_ZONE¿¡ ÀÖ´Â°¡´Â WarpToPC()¿¡¼­ Ã¼Å©

		int iPulse = thecore_pulse();

		//Ã¢°í ¿¬ÈÄ Ã¼Å©
		if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í¸¦ ¿¬ÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);

			if (test_server)
				ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
			return false;
		}

		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || IsCombOpen() || (m_bSashCombination) || (m_bSashAbsorption) || (m_bChangeLook) || (m_bAuraRefine) || (m_bAuraAbsorption))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡Ã¢,Ã¢°í µîÀ» ¿¬ »óÅÂ¿¡¼­´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ ¸¦ »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}

		//PREVENT_REFINE_HACK
		//°³·®ÈÄ ½Ã°£Ã¼Å©
		{
			if (iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ °³·®ÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
				return false;
			}
		}
		//END_PREVENT_REFINE_HACK

		//PREVENT_ITEM_COPY
		{
			if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³ÀÎ»óÁ¡ »ç¿ëÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
				return false;
			}
		}
		//END_PREVENT_ITEM_COPY

		//±ÍÈ¯ºÎ °Å¸®Ã¼Å©
		if (item->GetVnum() != ITEM_MARRIAGE_RING)
		{
			PIXEL_POSITION posWarp;

			int x = 0;
			int y = 0;

			double nDist = 0;
			const double nDistant = 5000.0;
			//±ÍÈ¯±â¾ïºÎ
			if (item->GetVnum() == 22010)
			{
				x = item->GetSocket(0) - GetX();
				y = item->GetSocket(1) - GetY();
			}
			//±ÍÈ¯ºÎ
			else if (item->GetVnum() == 22000)
			{
				SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp);

				if (item->GetSocket(0) == 0)
				{
					x = posWarp.x - GetX();
					y = posWarp.y - GetY();
				}
				else
				{
					x = item->GetSocket(0) - GetX();
					y = item->GetSocket(1) - GetY();
				}
			}

			nDist = sqrt(pow((float)x, 2) + pow((float)y, 2));

			if (nDistant > nDist)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌµ¿ µÇ¾îÁú À§Ä¡¿Í ³Ê¹« °¡±î¿ö ±ÍÈ¯ºÎ¸¦ »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù."));
				if (test_server)
					ChatPacket(CHAT_TYPE_INFO, "PossibleDistant %f nNowDist %f", nDistant, nDist);
				return false;
			}
		}

		//PREVENT_PORTAL_AFTER_EXCHANGE
		//±³È¯ ÈÄ ½Ã°£Ã¼Å©
		if (iPulse - GetExchangeTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎµîÀ» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
			return false;
		}
		//END_PREVENT_PORTAL_AFTER_EXCHANGE
	}

	//º¸µû¸® ºñ´Ü »ç¿ë½Ã °Å·¡Ã¢ Á¦ÇÑ Ã¼Å©
	if ((item->GetVnum() == 50200) || (item->GetVnum() == 50203) || (item->GetVnum() == 71049) || item->GetVnum() == 71221 || item->GetVnum() == 71260)
	{
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || IsCombOpen())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡Ã¢,Ã¢°í µîÀ» ¿¬ »óÅÂ¿¡¼­´Â º¸µû¸®,ºñ´Üº¸µû¸®¸¦ »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}
	//END_PREVENT_TRADE_WINDOW
	return UseItemEx(item, DestCell);
}

#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
bool CHARACTER::DropItem(TItemPos Cell, BYTE bCount)
{
	LPITEM item = nullptr;

	if (!CanHandleItem())
	{
		if (nullptr != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°­È­Ã¢À» ¿¬ »óÅÂ¿¡¼­´Â ¾ÆÀÌÅÛÀ» ¿Å±æ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (true == item->isLocked())
		return false;
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (item->IsSealed()) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't drop sealbind item."));
		return false;
	}
#endif

	if (item->IsBasicItem())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return false;
	}

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹ö¸± ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
		return false;
	}

	if (bCount == 0 || bCount > item->GetCount())
		bCount = item->GetCount();

	SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, 1000);	// Quickslot ¿¡¼­ Áö¿ò
	LPITEM pkItemToDrop;

	if (bCount == item->GetCount())
	{
		item->RemoveFromCharacter();
		pkItemToDrop = item;
	}
	else
	{
		if (bCount == 0)
		{
			if (test_server)
				sys_log(0, "[DROP_ITEM] drop item count == 0");
			return false;
		}

		// check non-split items for china
		//if (LC_IsNewCIBN())
		//	if (item->GetVnum() == 71095 || item->GetVnum() == 71050 || item->GetVnum() == 70038)
		//		return false;

		item->SetCount(item->GetCount() - bCount);
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		pkItemToDrop = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), bCount);

		// copy item socket -- by mhh
		FN_copy_item_socket(pkItemToDrop, item);

		char szBuf[51 + 1];
		snprintf(szBuf, sizeof(szBuf), "%u %u", pkItemToDrop->GetID(), pkItemToDrop->GetCount());
		LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
	}

#ifdef ENABLE_DROP_HACK_FIX
	// Clear the variable, it looks the player does not dropped any item in the past second.
	if (thecore_pulse() > LastDropTime + 25)
		CountDrops = 0;

	// It looks the player dropped min. 4 items in the past 1 second
	if (thecore_pulse() < LastDropTime + 25 && CountDrops >= 4)
	{
		// Set it to 0
		CountDrops = 0;
		sys_err("%s[%d] has been disconnected because of drophack using", GetName(), GetPlayerID());
		// Disconnect the player
		GetDesc()->SetPhase(PHASE_CLOSE);
		return false;
	}
#endif

	PIXEL_POSITION pxPos = GetXYZ();
	if (pkItemToDrop->AddToGround(GetMapIndex(), pxPos))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¶³¾îÁø ¾ÆÀÌÅÛÀº 3ºÐ ÈÄ »ç¶óÁý´Ï´Ù."));
		pkItemToDrop->StartDestroyEvent(3);

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemToDrop);

		char szHint[32 + 1];
		snprintf(szHint, sizeof(szHint), "%s %u %u", pkItemToDrop->GetName(), pkItemToDrop->GetCount(), pkItemToDrop->GetOriginalVnum());
		LogManager::instance().ItemLog(this, pkItemToDrop, "DROP", szHint);
		LastDropTime = thecore_pulse();
		CountDrops++;
		//Motion(MOTION_PICKUP);
	}

	return true;
}
#endif

#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
bool CHARACTER::DeleteItem(TItemPos Cell)
{
	LPITEM item = nullptr;

	if (!CanHandleItem())
		return false;

	if (IsDead() || IsStun())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (true == item->isLocked() || item->IsExchanging())
		return false;
	
	if (true == item->IsEquipped())
		return false;

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (item->IsSealed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't do this because item is sealed!"));
		return false;
	}
#endif

	// EXTRA_CHECK
	int iPulse = thecore_pulse();

	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please wait a second."));
		return false;
	}

	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return false;
	}
	// EXTRA_CHECK

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (item->GetVnum() >= 55701 && item->GetVnum() <= 55706)
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM new_petsystem WHERE id = %d", item->GetID()));
#endif
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has been deleted successfully."), item->GetName());
	LogManager::instance().ItemDestroyLog(this, item);
	ITEM_MANAGER::instance().RemoveItem(item);

	return true;
}

bool CHARACTER::SellItem(TItemPos Cell)
{
	LPITEM item = nullptr;

	if (!CanHandleItem())
		return false;

	if (IsDead() || IsStun())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (true == item->isLocked() || item->IsExchanging())
		return false;
	
	if (true == item->IsEquipped())
		return false;

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_SELL))
		return false;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (item->IsSealed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't do this because item is sealed!"));
		return false;
	}
#endif

	if (item->IsBasicItem())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return false;
	}

	// EXTRA_CHECK
	int iPulse = thecore_pulse();

	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please wait a second."));
		return false;
	}

	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || IsCombOpen() || isSashOpened(true) || isSashOpened(false) || isAuraOpened(true) || isAuraOpened(false))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return false;
	}
	// EXTRA_CHECK

	DWORD dwPrice;
	BYTE bCount;

	bCount = item->GetCount();
	dwPrice = item->GetShopBuyPrice();

	if (IS_SET(item->GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD))
	{
		if (dwPrice == 0)
			dwPrice = bCount;
		else
			dwPrice = bCount / dwPrice;
	}
	else
	{
		dwPrice *= bCount;
	}

	DWORD dwTax = 0;
	int iVal = 3;

	dwTax = dwPrice * iVal / 100;
	dwPrice -= dwTax;

	const int64_t nTotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(dwPrice);

	if (GOLD_MAX <= nTotalMoney)
	{
		sys_err("[OVERFLOW_GOLD] id %u name %s gold %u", GetPlayerID(), GetName(), GetGold());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20¾ï³ÉÀÌ ÃÊ°úÇÏ¿© ¹°Ç°À» ÆÈ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (iVal > 0)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÆÇ¸Å±Ý¾×ÀÇ %d %% °¡ ¼¼±ÝÀ¸·Î ³ª°¡°ÔµË´Ï´Ù"), iVal);

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (item->GetVnum() >= 55701 && item->GetVnum() <= 55706)
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM new_petsystem WHERE id = %d", item->GetID()));
#endif
	ITEM_MANAGER::instance().RemoveItem(item);
	LogManager::instance().ItemDestroyLog(this, item);
	PointChange(POINT_GOLD, dwPrice, false);

	return true;
}
#endif

bool CHARACTER::DropGold(int gold)
{
	if (gold <= 0 || gold > GetGold())
		return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_GoldDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastGoldDropTime + g_GoldDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ °ñµå¸¦ ¹ö¸± ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}

	m_dwLastGoldDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(1, gold);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			//Motion(MOTION_PICKUP);
			PointChange(POINT_GOLD, -gold, true);
#ifdef ENABLE_USELESS_LOGS
			if (gold > 1000) // Ãµ¿ø ÀÌ»ó¸¸ ±â·ÏÇÑ´Ù.
				LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");
#endif

			item->StartDestroyEvent(0);
		}

		Save();
		return true;
	}

	return false;
}

bool CHARACTER::MoveItem(TItemPos Cell, TItemPos DestCell, BYTE count)
{
	LPITEM item = nullptr;

	if (!IsValidItemPosition(Cell))
		return false;

	if (!(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (item->GetCount() < count)
		return false;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (INVENTORY == Cell.window_type && Cell.cell >= GetExtendInvenMax() && IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;
#else
	if (INVENTORY == Cell.window_type && Cell.cell >= INVENTORY_MAX_NUM && IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;
#endif

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (!item->IsDragonSoul() && !DestCell.IsBeltInventoryPosition() && INVENTORY == DestCell.window_type && DestCell.cell >= GetExtendInvenMax())
		return false;
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	if (item->IsSkillBook() && DestCell.window_type != SKILL_BOOK_INVENTORY)
		return false;
	
	if (item->IsSkillBook() && DestCell.cell >= SKILL_BOOK_INVENTORY_MAX_NUM)
		return false;
	
	if (item->IsUpgradeItem() && DestCell.window_type != UPGRADE_ITEMS_INVENTORY)
		return false;
	
	if (item->IsUpgradeItem() && DestCell.cell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
		return false;
	
	if (item->IsStone() && DestCell.window_type != STONE_ITEMS_INVENTORY)
		return false;

	if (item->IsStone() && DestCell.cell >= STONE_ITEMS_INVENTORY_MAX_NUM)
		return false;
	
	if (item->IsChest() && DestCell.window_type != CHEST_ITEMS_INVENTORY)
		return false;
	
	if (item->IsChest() && DestCell.cell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
		return false;
	
	if (item->IsAttr() && DestCell.window_type != ATTR_ITEMS_INVENTORY)
		return false;
	
	if (item->IsAttr() && DestCell.cell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
		return false;
	
	if (item->IsFlower() && DestCell.window_type != FLOWER_ITEMS_INVENTORY)
		return false;
	
	if (item->IsFlower() && DestCell.cell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
		return false;
#endif

	if (true == item->isLocked())
		return false;

	if (!IsValidItemPosition(DestCell))
	{
		return false;
	}

	if (!CanHandleItem())
	{
		if (nullptr != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°­È­Ã¢À» ¿¬ »óÅÂ¿¡¼­´Â ¾ÆÀÌÅÛÀ» ¿Å±æ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	// ±âÈ¹ÀÚÀÇ ¿äÃ»À¸·Î º§Æ® ÀÎº¥Åä¸®¿¡´Â Æ¯Á¤ Å¸ÀÔÀÇ ¾ÆÀÌÅÛ¸¸ ³ÖÀ» ¼ö ÀÖ´Ù.
	if (DestCell.IsBeltInventoryPosition() && false == CBeltInventoryHelper::CanMoveIntoBeltInventory(item) && !belt_allow_all_items)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº º§Æ® ÀÎº¥Åä¸®·Î ¿Å±æ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	// ÀÌ¹Ì Âø¿ëÁßÀÎ ¾ÆÀÌÅÛÀ» ´Ù¸¥ °÷À¸·Î ¿Å±â´Â °æ¿ì, 'ÀåÃ¥ ÇØÁ¦' °¡´ÉÇÑ Áö È®ÀÎÇÏ°í ¿Å±è
	if (Cell.IsEquipPosition() && !CanUnequipNow(item))
		return false;

	if (DestCell.IsEquipPosition())
	{
		if (GetItem(DestCell))	// ÀåºñÀÏ °æ¿ì ÇÑ °÷¸¸ °Ë»çÇØµµ µÈ´Ù.
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Àåºñ¸¦ Âø¿ëÇÏ°í ÀÖ½À´Ï´Ù."));

			return false;
		}

		if ((item->GetAttributeType(0) == 0 && item->IsQuestHair()) && features::IsFeatureEnabled(features::HAIR_SELECT_EX))
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		else
			EquipItem(item, DestCell.cell - INVENTORY_MAX_NUM);
	}
	else
	{
		if (item->IsDragonSoul())
		{
			if (item->IsEquipped())
			{
				if (DestCell.window_type == INVENTORY)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EQUIPED_DS_CANT_MOVE_INTO_THIS_INVENTORY"));
					return false;
				}

				return DSManager::instance().PullOut(this, DestCell, item);
			}
			else
			{
				if (DestCell.window_type != DRAGON_SOUL_INVENTORY)
				{
					return false;
				}

				if (!DSManager::instance().IsValidCellForThisItem(item, DestCell))
					return false;
			}
		}
		// ¿ëÈ¥¼®ÀÌ ¾Æ´Ñ ¾ÆÀÌÅÛÀº ¿ëÈ¥¼® ÀÎº¥¿¡ µé¾î°¥ ¼ö ¾ø´Ù.
		else if (DRAGON_SOUL_INVENTORY == DestCell.window_type)
			return false;

		LPITEM item2;

		if ((item2 = GetItem(DestCell)) && item != item2 && item2->IsStackable() &&
			!IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_STACK) &&
			item2->GetVnum() == item->GetVnum()) // ÇÕÄ¥ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ °æ¿ì
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				if (item2->GetSocket(i) != item->GetSocket(i))
					return false;

			if (count == 0)
				count = item->GetCount();

			sys_log(0, "%s: ITEM_STACK %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			count = MIN(g_bItemCountLimit - item2->GetCount(), count);

			item->SetCount(item->GetCount() - count);
			item2->SetCount(item2->GetCount() + count);
			return true;
		}

		if (!IsEmptyItemGrid(DestCell, item->GetSize(), Cell.cell)) //It's not empty - Let's try swapping.
		{
			if (count != 0 && count != item->GetCount()) //Can't swap if not the item as a whole is being moved
				return false;

			if (!DestCell.IsDefaultInventoryPosition() || !Cell.IsDefaultInventoryPosition()) //Only this kind of swapping on inventory
				return false;

			CItem * targetItem = GetItem_NEW(DestCell);
			if (!targetItem)
				return false;

			if (targetItem->GetVID() == item->GetVID()) //Can't swap over my own slots
				return false;

			if (targetItem) {
				DestCell = TItemPos(INVENTORY, targetItem->GetCell());
			}

			if (item->IsExchanging() || (targetItem && targetItem->IsExchanging()))
				return false;

			if (targetItem->isLocked() == true)
				return false;

			uint8_t basePage = DestCell.cell / (4);
			std::map<uint16_t, CItem*> moveItemMap;
			uint8_t sizeLeft = item->GetSize();

			for (uint16_t i = 0; i < item->GetSize(); ++i)
			{
				uint16_t cellNumber = DestCell.cell + i * 5;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
				if (cellNumber >= GetExtendInvenMax())
					return false;
#endif

				uint8_t cPage = cellNumber / (4);
				if (basePage != cPage)
					return false;

				CItem * mvItem = GetItem(TItemPos(INVENTORY, cellNumber));
				if (mvItem) {
					if (mvItem->GetSize() > item->GetSize())
						return false;

					if (mvItem->IsExchanging())
						return false;

					moveItemMap.insert({ Cell.cell + i * 5, mvItem });
					sizeLeft -= mvItem->GetSize();

					if (mvItem->GetSize() > 1)
						i += mvItem->GetSize() - 1; //Skip checking the obviously used cells
				}
				else {
					sizeLeft -= 1; //Empty slot
				}
			}

			if (sizeLeft != 0)
				return false;

			//This map will hold cell positions for syncing the quickslots afterwards
			std::map<uint8_t, uint16_t> syncCells; //Quickslot pos -> Target cell.

			//Let's remove the original item
			syncCells.insert({ GetQuickslotPosition(QUICKSLOT_TYPE_ITEM, item->GetCell()), DestCell.cell });
			item->RemoveFromCharacter();

			for (auto& it : moveItemMap)
			{
				uint16_t toCellNumber = it.first;
				CItem* mvItem = it.second;

				syncCells.insert({ GetQuickslotPosition(QUICKSLOT_TYPE_ITEM, mvItem->GetCell()), toCellNumber });
				mvItem->RemoveFromCharacter();

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
				SetItem(TItemPos(INVENTORY, toCellNumber), mvItem, false);
#else
				SetItem(TItemPos(INVENTORY, toCellNumber), mvItem);
#endif
			}

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
			SetItem(DestCell, item, false);
#else
			SetItem(DestCell, item);
#endif

			//Sync quickslots only after all is set
			for (auto& sCell : syncCells)
			{
				TQuickslot qs;
				qs.type = QUICKSLOT_TYPE_ITEM;
				qs.pos = sCell.second;

				SetQuickslot(sCell.first, qs);
			}

			return true;
		}

		if (count == 0 || count >= item->GetCount() || !item->IsStackable() || IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			sys_log(0, "%s: ITEM_MOVE %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			item->RemoveFromCharacter();
			SetItem(DestCell, item);

			if (INVENTORY == Cell.window_type && INVENTORY == DestCell.window_type)
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, DestCell.cell);
		}
		else if (count < item->GetCount())
		{
			//check non-split items
			//if (LC_IsNewCIBN())
			//{
			//	if (item->GetVnum() == 71095 || item->GetVnum() == 71050 || item->GetVnum() == 70038)
			//	{
			//		return false;
			//	}
			//}

			sys_log(0, "%s: ITEM_SPLIT %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			item->SetCount(item->GetCount() - count);
			LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), count);

			// copy socket -- by mhh
			FN_copy_item_socket(item2, item);
			item2->SetBasic(item->IsBasicItem());

			item2->AddToCharacter(this, DestCell);

			char szBuf[51 + 1];
			snprintf(szBuf, sizeof(szBuf), "%u %u %u %u ", item2->GetID(), item2->GetCount(), item->GetCount(), item->GetCount() + item2->GetCount());
			LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
		}
	}

	return true;
}

namespace NPartyPickupDistribute
{
	struct FFindOwnership
	{
		LPITEM item;
		LPCHARACTER owner;

		FFindOwnership(LPITEM item)
			: item(item), owner(nullptr)
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (item->IsOwnership(ch))
				owner = ch;
		}
	};

	struct FCountNearMember
	{
		int		total;
		int		x, y;

		FCountNearMember(LPCHARACTER center)
			: total(0), x(center->GetX()), y(center->GetY())
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				total += 1;
		}
	};

	struct FMoneyDistributor
	{
		int		total;
		LPCHARACTER	c;
		int		x, y;
		int		iMoney;

		FMoneyDistributor(LPCHARACTER center, int iMoney)
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iMoney(iMoney)
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch != c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_GOLD, iMoney, true);

					if (iMoney > 100000) // Dont log below 100.000
						LogManager::instance().CharLog(ch, iMoney, "GET_GOLD", "");
				}
		}
	};
}

void CHARACTER::GiveGold(int iAmount)
{
	if (iAmount <= 0)
		return;

	sys_log(0, "GIVE_GOLD: %s %d", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// ÆÄÆ¼°¡ ÀÖ´Â °æ¿ì ³ª´©¾î °¡Áø´Ù.
		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FMoneyDistributor funcMoneyDist(this, dwShare);

			pParty->ForEachOnlineMember(funcMoneyDist);
		}

		PointChange(POINT_GOLD, dwMyAmount, true);

		if (dwMyAmount > 100000) // Dont log below 100.000
			LogManager::instance().CharLog(this, dwMyAmount, "GET_GOLD", "");
	}
	else
	{
		PointChange(POINT_GOLD, iAmount, true);

		// ºê¶óÁú¿¡ µ·ÀÌ ¾ø¾îÁø´Ù´Â ¹ö±×°¡ ÀÖ´Âµ¥,
		// °¡´ÉÇÑ ½Ã³ª¸®¿À Áß¿¡ ÇÏ³ª´Â,
		// ¸ÞÅ©·Î³ª, ÇÙÀ» ½á¼­ 1000¿ø ÀÌÇÏÀÇ µ·À» °è¼Ó ¹ö·Á °ñµå¸¦ 0À¸·Î ¸¸µé°í,
		// µ·ÀÌ ¾ø¾îÁ³´Ù°í º¹±¸ ½ÅÃ»ÇÏ´Â °ÍÀÏ ¼öµµ ÀÖ´Ù.
		// µû¶ó¼­ ±×·± °æ¿ì¸¦ Àâ±â À§ÇØ ³·Àº ¼öÄ¡ÀÇ °ñµå¿¡ ´ëÇØ¼­µµ ·Î±×¸¦ ³²±è.

		if (iAmount > 100000) // Dont log below 100.000
			LogManager::instance().CharLog(this, iAmount, "GET_GOLD", "");
	}
}

bool CHARACTER::PickupItem(DWORD dwVID)
{	
	LPITEM item = ITEM_MANAGER::instance().FindByVID(dwVID);

	if (IsObserverMode())
		return false;

	if (!item || !item->GetSectree())
		return false;
	
	if (item->DistanceValid(this))
	{
		if (item->IsOwnership(this))
		{
			// ?? ??? ?? ???? ????
			if (item->GetType() == ITEM_ELK)
			{
				GiveGold(item->GetCount());
				item->RemoveFromGround();

				M2_DESTROY_ITEM(item);

				Save();
			}
			// ??? ??????
			else
			{
				if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{
							int j;

							for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
								if (item2->GetSocket(j) != item->GetSocket(j))
									break;

							if (j != ITEM_SOCKET_MAX_NUM)
								continue;
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								if (item2->GetType() == ITEM_QUEST)
									quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
				// @fixpch002 begin
				if (item->IsUpgradeItem() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetUpgradeItemsInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}
				else if (item->IsSkillBook() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetSkillBookInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							int j;
							for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
								if (item2->GetSocket(j) != item->GetSocket(j))
									break;
							if (j != ITEM_SOCKET_MAX_NUM)
								continue;
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}
				else if (item->IsStone() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetStoneItemsInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}

				else if (item->IsFlower() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetFlowerItemsInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}

				else if (item->IsAttr() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetAttrItemsInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}

				else if (item->IsChest() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					BYTE bCount = item->GetCount();
					for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetChestItemsInventoryItem(i);
						if (!item2)
							continue;
						if (item2->GetVnum() == item->GetVnum())
						{
							BYTE bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
							bCount -= bCount2;
							if (bCount2 > 0)
								item2->SetCount(item2->GetCount() + bCount2);
							if (bCount == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item2->GetName());
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
					item->SetCount(bCount);
				}
				// @fixpch002 end
				
				int iEmptyCell;
				if (item->IsDragonSoul())
				{
					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						sys_log(0, "No empty ds inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsUpgradeItem())
				{
					if ((iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsSkillBook())
				{
					if ((iEmptyCell = GetEmptySkillBookInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsStone())
				{
					if ((iEmptyCell = GetEmptyStoneInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsChest())
				{
					if ((iEmptyCell = GetEmptyChestInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsAttr())
				{
					if ((iEmptyCell = GetEmptyAttrInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else if (item->IsFlower())
				{
					if ((iEmptyCell = GetEmptyFlowerInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
				else
				{
					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}

				item->RemoveFromGround();

				if (item->IsDragonSoul())
					item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
				else if (item->IsUpgradeItem())
					item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyCell));
				else if (item->IsSkillBook())
					item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, iEmptyCell));
				else if (item->IsStone())
					item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, iEmptyCell));
				else if (item->IsChest())
					item->AddToCharacter(this, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyCell));
				else if (item->IsAttr())
					item->AddToCharacter(this, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyCell));
				else if (item->IsFlower())
					item->AddToCharacter(this, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyCell));
				else
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));

				char szHint[32+1];
				snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
				LogManager::instance().ItemLog(this, item, "GET", szHint);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ESYA_TOPLANDI: %s"), item->GetName()); // @fixpch001
				
				//SetLastPickupTime(get_dword_time());

				if (item->GetType() == ITEM_QUEST)
					quest::CQuestManager::instance().PickupItem (GetPlayerID(), item);
			}

			//Motion(MOTION_PICKUP);
			return true;
		}
		else if (!IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_DROP) && GetParty())
		{
			// ?? ??? ??? ???? ???? ???
			NPartyPickupDistribute::FFindOwnership funcFindOwnership(item);

			GetParty()->ForEachOnlineMember(funcFindOwnership);

			LPCHARACTER owner = funcFindOwnership.owner;
			// @fixme115
			if (!owner)
				return false;

			int iEmptyCell;

			if (item->IsDragonSoul())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyDragonSoulInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsUpgradeItem())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyUpgradeItemsInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsSkillBook())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptySkillBookInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptySkillBookInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsStone())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyStoneInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyStoneInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsChest())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyChestInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyChestInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsAttr())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyAttrInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyAttrInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else if (item->IsFlower())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyFlowerInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyFlowerInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}
			else
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ³Ê¹« ¸¹½À´Ï´Ù."));
						return false;
					}
				}
			}

			item->RemoveFromGround();

			if (item->IsDragonSoul())
				item->AddToCharacter(owner, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
			else if (item->IsUpgradeItem())
				item->AddToCharacter(owner, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyCell));
			else if (item->IsSkillBook())
				item->AddToCharacter(owner, TItemPos(SKILL_BOOK_INVENTORY, iEmptyCell));
			else if (item->IsStone())
				item->AddToCharacter(owner, TItemPos(STONE_ITEMS_INVENTORY, iEmptyCell));
			else if (item->IsChest())
				item->AddToCharacter(owner, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyCell));
			else if (item->IsAttr())
				item->AddToCharacter(owner, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyCell));
			else if (item->IsFlower())
				item->AddToCharacter(owner, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyCell));
#endif			
			else
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));

			char szHint[32+1];
			snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
			LogManager::instance().ItemLog(owner, item, "GET", szHint);

			if (owner == this)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item->GetName());
			else
			{
				owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s ´ÔÀ¸·ÎºÎÅÍ %s"), GetName(), item->GetName());
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ Àü´Þ: %s ´Ô¿¡°Ô %s"), owner->GetName(), item->GetName());
			}
			
			//SetLastPickupTime(get_dword_time());

			if (item->GetType() == ITEM_QUEST)
				quest::CQuestManager::instance().PickupItem (owner->GetPlayerID(), item);

			return true;
		}
	}

	return false;
}

bool CHARACTER::SwapItem(UINT bCell, UINT bDestCell)
{
	if (!CanHandleItem())
		return false;

	TItemPos srcCell(INVENTORY, bCell), destCell(INVENTORY, bDestCell);

	// ¿Ã¹Ù¸¥ Cell ÀÎÁö °Ë»ç
	// ¿ëÈ¥¼®Àº SwapÇÒ ¼ö ¾øÀ¸¹Ç·Î, ¿©±â¼­ °É¸².
	//if (bCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM || bDestCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM)
	if (srcCell.IsDragonSoulEquipPosition() || destCell.IsDragonSoulEquipPosition())
		return false;

	// °°Àº CELL ÀÎÁö °Ë»ç
	if (bCell == bDestCell)
		return false;

	// µÑ ´Ù ÀåºñÃ¢ À§Ä¡¸é Swap ÇÒ ¼ö ¾ø´Ù.
	if (srcCell.IsEquipPosition() && destCell.IsEquipPosition())
		return false;

	LPITEM item1, item2;

	// item2°¡ ÀåºñÃ¢¿¡ ÀÖ´Â °ÍÀÌ µÇµµ·Ï.
	if (srcCell.IsEquipPosition())
	{
		item1 = GetInventoryItem(bDestCell);
		item2 = GetInventoryItem(bCell);
	}
	else
	{
		item1 = GetInventoryItem(bCell);
		item2 = GetInventoryItem(bDestCell);
	}

	if (!item1 || !item2)
		return false;

	if (item1 == item2)
	{
		sys_log(0, "[WARNING][WARNING][HACK USER!] : %s %d %d", m_stName.c_str(), bCell, bDestCell);
		return false;
	}

	// item2°¡ bCellÀ§Ä¡¿¡ µé¾î°¥ ¼ö ÀÖ´ÂÁö È®ÀÎÇÑ´Ù.
	if (!IsEmptyItemGrid(TItemPos(INVENTORY, item1->GetCell()), item2->GetSize(), item1->GetCell()))
		return false;

	// ¹Ù²Ü ¾ÆÀÌÅÛÀÌ ÀåºñÃ¢¿¡ ÀÖÀ¸¸é
	if (TItemPos(EQUIPMENT, item2->GetCell()).IsEquipPosition())
	{
		INT bEquipCell = item2->GetCell() - INVENTORY_MAX_NUM;
		UINT bInvenCell = item1->GetCell();

		// Âø¿ëÁßÀÎ ¾ÆÀÌÅÛÀ» ¹þÀ» ¼ö ÀÖ°í, Âø¿ë ¿¹Á¤ ¾ÆÀÌÅÛÀÌ Âø¿ë °¡´ÉÇÑ »óÅÂ¿©¾ß¸¸ ÁøÇà
		//if (false == CanUnequipNow(item2) || false == CanEquipNow(item1))
		if (false == CanEquipNow(item1))
			return false;
		if (item2->IsDragonSoul() && false == CanUnequipNow(item2))
			return false;

		if (bEquipCell != item1->FindEquipCell(this)) // °°Àº À§Ä¡ÀÏ¶§¸¸ Çã¿ë
			return false;

#ifdef ENABLE_GUILD_HIGHLIGHT_EQUIP_ITEM
		if (GetGuild() && CWarMapManager::instance().IsWarMap(GetMapIndex()))
		{
			char item1link[256];
			int len;
			int i;

			len = snprintf(item1link, sizeof(item1link), "item:%x:%lx", item1->GetVnum(), item1->GetFlag());

			for (i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
				len += snprintf(item1link + len, sizeof(item1link) - len, ":%ld", item1->GetSocket(i));

			for (i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++) {
				if (i >= item1->GetAttributeCount())
					len += snprintf(item1link + len, sizeof(item1link) - len, ":0:0");
				else
					len += snprintf(item1link + len, sizeof(item1link) - len, ":%x:%hd", item1->GetAttributeType(i), item1->GetAttributeValue(i));
			}

			char item2link[256];
			len = 0;
			i = 0;

			len = snprintf(item2link, sizeof(item2link), "item:%x:%lx", item2->GetVnum(), item2->GetFlag());

			for (i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
				len += snprintf(item2link + len, sizeof(item2link) - len, ":%ld", item2->GetSocket(i));

			for (i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++) {
				if (i >= item2->GetAttributeCount())
					len += snprintf(item2link + len, sizeof(item2link) - len, ":0:0");
				else
					len += snprintf(item2link + len, sizeof(item2link) - len, ":%x:%hd", item2->GetAttributeType(i), item2->GetAttributeValue(i));
			}

			char buf[2048] = { 0 };
			snprintf(buf, sizeof(buf), "%s: |cffffc700|H%s|h[%s]|h|r cikarip |cffffc700|H%s|h[%s]|h|r takti.",
				GetName(), item1link, item1->GetName(), item2link, item2->GetName());

			GetGuild()->Chat(buf);
		}
#endif

		item2->RemoveFromCharacter();

		if (item1->EquipTo(this, bEquipCell))

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
			item2->AddToCharacter(this, TItemPos(INVENTORY, bInvenCell), false);
#else
			item2->AddToCharacter(this, TItemPos(INVENTORY, bInvenCell));
#endif

		else
			sys_err("SwapItem cannot equip %s! item1 %s", item2->GetName(), item1->GetName());
	}
	else
	{
		UINT bCell1 = item1->GetCell();
		UINT bCell2 = item2->GetCell();

		item1->RemoveFromCharacter();
		item2->RemoveFromCharacter();

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item1->AddToCharacter(this, TItemPos(INVENTORY, bCell2), false);
		item2->AddToCharacter(this, TItemPos(INVENTORY, bCell1), false);
#else
		item1->AddToCharacter(this, TItemPos(INVENTORY, bCell2));
		item2->AddToCharacter(this, TItemPos(INVENTORY, bCell1));
#endif
	}

	return true;
}

bool CHARACTER::UnequipItem(LPITEM item)
{
	int pos;

	if (false == CanUnequipNow(item))
		return false;

	if (item->IsDragonSoul())
		pos = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		pos = GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		pos = GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsStone())
		pos = GetEmptyStoneInventory(item->GetSize());
#endif
	else
		pos = GetEmptyInventory(item->GetSize());

	// HARD CODING
	if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		ShowAlignment(true);

#ifdef ENABLE_GUILD_HIGHLIGHT_EQUIP_ITEM
	if (GetGuild() && CWarMapManager::instance().IsWarMap(GetMapIndex()) && item->IsEquipped())
	{
		char buf[1024] = { 0 };
		char itemlink[256];
		int len;
		int i;

		len = snprintf(itemlink, sizeof(itemlink), "item:%x:%lx", item->GetVnum(), item->GetFlag());

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
			len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%ld", item->GetSocket(i));

		for (i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++) {
			if (i >= item->GetAttributeCount())
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":0:0");
			else
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%hd", item->GetAttributeType(i), item->GetAttributeValue(i));
		}

		snprintf(buf, sizeof(buf), "%s: |cffffc700|H%s|h[%s]|h|r cikardi.", GetName(), itemlink, item->GetName());
		GetGuild()->Chat(buf);
	}
#endif

	item->RemoveFromCharacter();
	if (item->IsDragonSoul())
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos));
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, pos));
#endif
	else if (item->IsUpgradeItem())
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, pos));
#endif
	else if (item->IsStone())
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, pos));
#endif
	else
#endif
#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif

	CheckMaximumPoints();

	return true;
}

//
// @version	05/07/05 Bang2ni - Skill »ç¿ëÈÄ 1.5 ÃÊ ÀÌ³»¿¡ Àåºñ Âø¿ë ±ÝÁö
//
bool CHARACTER::EquipItem(LPITEM item, int iCandidateCell)
{
	if (ITEM_BELT == item->GetType() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Kemer envanterini bosaltin!"));
		return false;//kemer envanter fix
	}

	if (item->IsExchanging())
		return false;

	if (false == item->IsEquipable())
		return false;

	if (false == CanEquipNow(item))
		return false;

	int iWearCell = item->FindEquipCell(this, iCandidateCell);

	if (iWearCell < 0)
		return false;

	if (iWearCell == WEAR_BODY && IsRiding() && (item->GetVnum() >= 11901 && item->GetVnum() <= 11904))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» Åº »óÅÂ¿¡¼­ ¿¹º¹À» ÀÔÀ» ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (iWearCell != WEAR_ARROW && IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â Âø¿ëÁßÀÎ Àåºñ¸¦ º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (FN_check_item_sex(this, item) == false)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºº°ÀÌ ¸ÂÁö¾Ê¾Æ ÀÌ ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	//½Å±Ô Å»°Í »ç¿ë½Ã ±âÁ¸ ¸» »ç¿ë¿©ºÎ Ã¼Å©
	if (item->IsRideItem() && IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Å»°ÍÀ» ÀÌ¿ëÁßÀÔ´Ï´Ù."));
		return false;
	}

	DWORD dwCurTime = get_dword_time();

	if (iWearCell != WEAR_ARROW && (dwCurTime - GetLastAttackTime() <= 1500 || dwCurTime - m_dwLastSkillTime <= 1500))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°¡¸¸È÷ ÀÖÀ» ¶§¸¸ Âø¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù."));
		return false;
	}

	if (item->IsDragonSoul())
	{
		// °°Àº Å¸ÀÔÀÇ ¿ëÈ¥¼®ÀÌ ÀÌ¹Ì µé¾î°¡ ÀÖ´Ù¸é Âø¿ëÇÒ ¼ö ¾ø´Ù.
		// ¿ëÈ¥¼®Àº swapÀ» Áö¿øÇÏ¸é ¾ÈµÊ.
		if (GetInventoryItem(INVENTORY_MAX_NUM + iWearCell))
		{
			ChatPacket(CHAT_TYPE_INFO, "ÀÌ¹Ì °°Àº Á¾·ùÀÇ ¿ëÈ¥¼®À» Âø¿ëÇÏ°í ÀÖ½À´Ï´Ù.");
			return false;
		}

		if (!item->EquipTo(this, iWearCell))
		{
			return false;
		}
	}
	// ¿ëÈ¥¼®ÀÌ ¾Æ´Ô.
	else
	{
		// Âø¿ëÇÒ °÷¿¡ ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é,
		if (GetWear(iWearCell) && !IS_SET(GetWear(iWearCell)->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		{
			// ÀÌ ¾ÆÀÌÅÛÀº ÇÑ¹ø ¹ÚÈ÷¸é º¯°æ ºÒ°¡. swap ¿ª½Ã ¿ÏÀü ºÒ°¡
			if (item->GetWearFlag() == WEARABLE_ABILITY)
				return false;

			if (false == SwapItem(item->GetCell(), INVENTORY_MAX_NUM + iWearCell))
			{
				return false;
			}
		}
		else
		{
			UINT bOldCell = item->GetCell();

			if (item->EquipTo(this, iWearCell))
			{
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, bOldCell, iWearCell);
			}
		}
	}

	if (true == item->IsEquipped())
	{
		// ¾ÆÀÌÅÛ ÃÖÃÊ »ç¿ë ÀÌÈÄºÎÅÍ´Â »ç¿ëÇÏÁö ¾Ê¾Æµµ ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä Ã³¸®.
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			// ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇÑ ¾ÆÀÌÅÛÀÎÁö ¿©ºÎ´Â Socket1À» º¸°í ÆÇ´ÜÇÑ´Ù. (Socket1¿¡ »ç¿ëÈ½¼ö ±â·Ï)
			if (0 == item->GetSocket(1))
			{
				// »ç¿ë°¡´É½Ã°£Àº Default °ªÀ¸·Î Limit Value °ªÀ» »ç¿ëÇÏµÇ, Socket0¿¡ °ªÀÌ ÀÖÀ¸¸é ±× °ªÀ» »ç¿ëÇÏµµ·Ï ÇÑ´Ù. (´ÜÀ§´Â ÃÊ)
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[(unsigned char)(item->GetProto()->cLimitRealTimeFirstUseIndex)].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		const DWORD & dwVnum = item->GetVnum();

		// ¶ó¸¶´Ü ÀÌº¥Æ® ÃÊ½Â´ÞÀÇ ¹ÝÁö(71135) Âø¿ë½Ã ÀÌÆåÆ® ¹ßµ¿
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// ÇÒ·ÎÀ© »çÅÁ(71136) Âø¿ë½Ã ÀÌÆåÆ® ¹ßµ¿
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// Çàº¹ÀÇ ¹ÝÁö(71143) Âø¿ë½Ã ÀÌÆåÆ® ¹ßµ¿
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// »ç¶ûÀÇ ÆÒ´øÆ®(71145) Âø¿ë½Ã ÀÌÆåÆ® ¹ßµ¿
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		else if (item->GetVnum() == DBONE_VNUM_1) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_tigerknochenarmband.mse");
		}
		else if (item->GetVnum() == DBONE_VNUM_2) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_drachenknochenarmband.mse");
		}
		else if (item->GetVnum() == UNIQUE_ITEM_DOUBLE_EXP) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_expring.mse");
		}
		else if (item->GetVnum() == UNIQUE_ITEM_DOUBLE_ITEM) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_handschuh.mse");
		}
		else if (item->GetVnum() == 72054) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_item14.mse");
		}
		else if (item->GetVnum() == 71199) {
			SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_item10.mse");
		}
		// ITEM_UNIQUEÀÇ °æ¿ì, SpecialItemGroup¿¡ Á¤ÀÇµÇ¾î ÀÖ°í, (item->GetSIGVnum() != nullptr)
		//
		else if (ITEM_UNIQUE == item->GetType() && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (nullptr != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (nullptr != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}
#ifdef ENABLE_ACCE_SYSTEM
		else if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_SASH))
			this->EffectPacket(SE_EFFECT_SASH_EQUIP);
#endif
		if (
			(ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_RIDE == item->GetSubType())
			|| (ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == item->GetSubType()))
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}
	}

#ifdef ENABLE_GUILD_HIGHLIGHT_EQUIP_ITEM
	if (GetGuild() && CWarMapManager::instance().IsWarMap(GetMapIndex()) && item->IsEquipped())
	{
		char buf[1024] = { 0 };
		char itemlink[256];
		int len;
		int i;

		len = snprintf(itemlink, sizeof(itemlink), "item:%x:%lx", item->GetVnum(), item->GetFlag());

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
			len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%ld", item->GetSocket(i));

		for (i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++) {
			if (i >= item->GetAttributeCount())
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":0:0");
			else
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%hd", item->GetAttributeType(i), item->GetAttributeValue(i));
		}

		snprintf(buf, sizeof(buf), "%s: |cffffc700|H%s|h[%s]|h|r takti.", GetName(), itemlink, item->GetName());
		GetGuild()->Chat(buf);
	}
#endif

	return true;
}

void CHARACTER::BuffOnAttr_AddBuffsFromItem(LPITEM pItem)
{
	for (unsigned int i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->AddBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem)
{
	for (unsigned int i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->RemoveBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_ClearAll()
{
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		CBuffOnAttributes* pBuff = it->second;
		if (pBuff)
		{
			pBuff->Initialize();
		}
	}
}

void CHARACTER::BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue)
{
	TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(bType);

	if (0 == bNewValue)
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->Off();
	}
	else if (0 == bOldValue)
	{
		CBuffOnAttributes* pBuff;
		if (m_map_buff_on_attrs.end() == it)
		{
			switch (bType)
			{
			case POINT_ENERGY:
			{
				static BYTE abSlot[] = { WEAR_BODY, WEAR_HEAD, WEAR_FOOTS, WEAR_WRIST, WEAR_WEAPON, WEAR_NECK, WEAR_EAR, WEAR_SHIELD };
				static std::vector <BYTE> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;
			case POINT_COSTUME_ATTR_BONUS:
			{
				static BYTE abSlot[] = { WEAR_COSTUME_BODY, WEAR_COSTUME_HAIR,
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
										WEAR_COSTUME_WEAPON,
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
										WEAR_COSTUME_MOUNT,
#endif
				};
				static std::vector <BYTE> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;
			default:
				break;
			}
			m_map_buff_on_attrs.insert(TMapBuffOnAttrs::value_type(bType, pBuff));
		}
		else
			pBuff = it->second;

		pBuff->On(bNewValue);
	}
	else
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->ChangeBuffValue(bNewValue);
	}
}

LPITEM CHARACTER::FindSpecifyItem(DWORD vnum) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (GetInventoryItem(i) && GetInventoryItem(i)->GetVnum() == vnum)
			return GetInventoryItem(i);

	return nullptr;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
LPITEM CHARACTER::FindSkillBookInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
		if (GetSkillBookInventoryItem(i) && GetSkillBookInventoryItem(i)->GetVnum() == vnum)
			return GetSkillBookInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindUpgradeItemsInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (GetUpgradeItemsInventoryItem(i) && GetUpgradeItemsInventoryItem(i)->GetVnum() == vnum)
			return GetUpgradeItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindStoneItemsInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (GetStoneItemsInventoryItem(i) && GetStoneItemsInventoryItem(i)->GetVnum() == vnum)
			return GetStoneItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindChestItemsInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (GetChestItemsInventoryItem(i) && GetChestItemsInventoryItem(i)->GetVnum() == vnum)
			return GetChestItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindAttrItemsInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (GetAttrItemsInventoryItem(i) && GetAttrItemsInventoryItem(i)->GetVnum() == vnum)
			return GetAttrItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindFlowerItemsInventoryItem(DWORD vnum) const
{
	for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (GetFlowerItemsInventoryItem(i) && GetFlowerItemsInventoryItem(i)->GetVnum() == vnum)
			return GetFlowerItemsInventoryItem(i);

	return nullptr;
}
#endif

LPITEM CHARACTER::FindItemByID(DWORD id) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	for (int i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
	{
		if (nullptr != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	return nullptr;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
LPITEM CHARACTER::FindSkillBookInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetSkillBookInventoryItem(i) && GetSkillBookInventoryItem(i)->GetVnum() == vnum)
			return GetSkillBookInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindUpgradeItemsInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetUpgradeItemsInventoryItem(i) && GetUpgradeItemsInventoryItem(i)->GetVnum() == vnum)
			return GetUpgradeItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindStoneItemsInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetStoneItemsInventoryItem(i) && GetStoneItemsInventoryItem(i)->GetVnum() == vnum)
			return GetStoneItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindChestItemsInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetChestItemsInventoryItem(i) && GetChestItemsInventoryItem(i)->GetVnum() == vnum)
			return GetChestItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindAttrItemsInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetAttrItemsInventoryItem(i) && GetAttrItemsInventoryItem(i)->GetVnum() == vnum)
			return GetAttrItemsInventoryItem(i);

	return nullptr;
}

LPITEM CHARACTER::FindFlowerItemsInventoryItemByID(DWORD vnum) const
{
	for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
		if (nullptr != GetFlowerItemsInventoryItem(i) && GetFlowerItemsInventoryItem(i)->GetVnum() == vnum)
			return GetFlowerItemsInventoryItem(i);

	return nullptr;
}
#endif

int CHARACTER::CountSpecifyItem(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		item = GetInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	for (int i = 0; i < 225; ++i)
	{
		item = GetUpgradeItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < 225; ++i)
	{
		item = GetSkillBookInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < 225; ++i)
	{
		item = GetStoneItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < 225; ++i)
	{
		item = GetChestItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	
	for (int i = 0; i < 225; ++i)
	{
		item = GetAttrItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	
	for (int i = 0; i < 225; ++i)
	{
		item = GetFlowerItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}

	return count;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
int CHARACTER::CountSpecifyItemBySkillBookInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
	{
		item = GetSkillBookInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}

int CHARACTER::CountSpecifyItemByUpgradeItemsInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		item = GetUpgradeItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}

int CHARACTER::CountSpecifyItemByStoneItemsInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		item = GetStoneItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}

int CHARACTER::CountSpecifyItemByChestItemsInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		item = GetChestItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}

int CHARACTER::CountSpecifyItemByAttrItemsInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		item = GetAttrItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}

int CHARACTER::CountSpecifyItemByFlowerItemsInventory(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		item = GetFlowerItemsInventoryItem(i);
		if (nullptr != item && item->GetVnum() == vnum)
		{
			// °³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	return count;
}
#endif

void CHARACTER::RemoveSpecifyItem(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetUpgradeItemsInventoryItem(i))
			continue;

		if (GetUpgradeItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetUpgradeItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetUpgradeItemsInventoryItem(i)->GetCount())
		{
			count -= GetUpgradeItemsInventoryItem(i)->GetCount();
			GetUpgradeItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetUpgradeItemsInventoryItem(i)->SetCount(GetUpgradeItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetSkillBookInventoryItem(i))
			continue;

		if (GetSkillBookInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetSkillBookInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetSkillBookInventoryItem(i)->GetCount())
		{
			count -= GetSkillBookInventoryItem(i)->GetCount();
			GetSkillBookInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetSkillBookInventoryItem(i)->SetCount(GetSkillBookInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetStoneItemsInventoryItem(i))
			continue;

		if (GetStoneItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetStoneItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetStoneItemsInventoryItem(i)->GetCount())
		{
			count -= GetStoneItemsInventoryItem(i)->GetCount();
			GetStoneItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetStoneItemsInventoryItem(i)->SetCount(GetStoneItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetChestItemsInventoryItem(i))
			continue;

		if (GetChestItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetChestItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetChestItemsInventoryItem(i)->GetCount())
		{
			count -= GetChestItemsInventoryItem(i)->GetCount();
			GetChestItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChestItemsInventoryItem(i)->SetCount(GetChestItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetAttrItemsInventoryItem(i))
			continue;

		if (GetAttrItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetAttrItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetAttrItemsInventoryItem(i)->GetCount())
		{
			count -= GetAttrItemsInventoryItem(i)->GetCount();
			GetAttrItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetAttrItemsInventoryItem(i)->SetCount(GetAttrItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	
	for (UINT i = 0; i < 225; ++i)
	{
		if (nullptr == GetFlowerItemsInventoryItem(i))
			continue;

		if (GetFlowerItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetFlowerItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetFlowerItemsInventoryItem(i)->GetCount())
		{
			count -= GetFlowerItemsInventoryItem(i)->GetCount();
			GetFlowerItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetFlowerItemsInventoryItem(i)->SetCount(GetFlowerItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#endif

	// ¿¹¿ÜÃ³¸®°¡ ¾àÇÏ´Ù.
	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItem cannot remove enough item vnum %u, still remain %d", vnum, count);
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
void CHARACTER::RemoveSpecifyItemBySkillBookInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetSkillBookInventoryItem(i))
			continue;

		if (GetSkillBookInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetSkillBookInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetSkillBookInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetSkillBookInventoryItem(i)->GetCount())
		{
			count -= GetSkillBookInventoryItem(i)->GetCount();
			GetSkillBookInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetSkillBookInventoryItem(i)->SetCount(GetSkillBookInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemBySkillBookInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}

void CHARACTER::RemoveSpecifyItemByUpgradeItemsInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetUpgradeItemsInventoryItem(i))
			continue;

		if (GetUpgradeItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetUpgradeItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetUpgradeItemsInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetUpgradeItemsInventoryItem(i)->GetCount())
		{
			count -= GetUpgradeItemsInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetUpgradeItemsInventoryItem(i)->SetCount(GetUpgradeItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemByUpgradeItemsInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}

void CHARACTER::RemoveSpecifyItemByStoneItemsInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetStoneItemsInventoryItem(i))
			continue;

		if (GetStoneItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetStoneItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetStoneItemsInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetStoneItemsInventoryItem(i)->GetCount())
		{
			count -= GetStoneItemsInventoryItem(i)->GetCount();
			GetStoneItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetStoneItemsInventoryItem(i)->SetCount(GetStoneItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemByStoneItemsInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}

void CHARACTER::RemoveSpecifyItemByChestItemsInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetChestItemsInventoryItem(i))
			continue;

		if (GetChestItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetChestItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetChestItemsInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetChestItemsInventoryItem(i)->GetCount())
		{
			count -= GetChestItemsInventoryItem(i)->GetCount();
			GetChestItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChestItemsInventoryItem(i)->SetCount(GetChestItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemByChestItemsInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}

void CHARACTER::RemoveSpecifyItemByAttrItemsInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetAttrItemsInventoryItem(i))
			continue;

		if (GetAttrItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetAttrItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetAttrItemsInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetAttrItemsInventoryItem(i)->GetCount())
		{
			count -= GetAttrItemsInventoryItem(i)->GetCount();
			GetAttrItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetAttrItemsInventoryItem(i)->SetCount(GetAttrItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemByChestItemsInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}

void CHARACTER::RemoveSpecifyItemByFlowerItemsInventory(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetFlowerItemsInventoryItem(i))
			continue;

		if (GetFlowerItemsInventoryItem(i)->GetVnum() != vnum)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetFlowerItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetFlowerItemsInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetFlowerItemsInventoryItem(i)->GetCount())
		{
			count -= GetFlowerItemsInventoryItem(i)->GetCount();
			GetFlowerItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetFlowerItemsInventoryItem(i)->SetCount(GetFlowerItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItemByChestItemsInventory cannot remove enough item vnum %u, still remain %d", vnum, count);
}
#endif

int CHARACTER::CountSpecifyTypeItem(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
int CHARACTER::CountSpecifyTypeItemBySkillBookInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetSkillBookInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

int CHARACTER::CountSpecifyTypeItemByUpgradeItemsInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetUpgradeItemsInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

int CHARACTER::CountSpecifyTypeItemByStoneItemsInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetStoneItemsInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

int CHARACTER::CountSpecifyTypeItemByChestItemsInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetChestItemsInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

int CHARACTER::CountSpecifyTypeItemByAttrItemsInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetAttrItemsInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

int CHARACTER::CountSpecifyTypeItemByFlowerItemsInventory(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetFlowerItemsInventoryItem(i);
		if (pItem != nullptr && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}
#endif

void CHARACTER::RemoveSpecifyTypeItem(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
void CHARACTER::RemoveSpecifyTypeItemBySkillBookInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetSkillBookInventoryItem(i))
			continue;

		if (GetSkillBookInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetSkillBookInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetSkillBookInventoryItem(i)->GetCount())
		{
			count -= GetSkillBookInventoryItem(i)->GetCount();
			GetSkillBookInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetSkillBookInventoryItem(i)->SetCount(GetSkillBookInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::RemoveSpecifyTypeItemByUpgradeItemsInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetUpgradeItemsInventoryItem(i))
			continue;

		if (GetUpgradeItemsInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetUpgradeItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetUpgradeItemsInventoryItem(i)->GetCount())
		{
			count -= GetUpgradeItemsInventoryItem(i)->GetCount();
			GetUpgradeItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetUpgradeItemsInventoryItem(i)->SetCount(GetUpgradeItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::RemoveSpecifyTypeItemByStoneItemsInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < STONE_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetStoneItemsInventoryItem(i))
			continue;

		if (GetStoneItemsInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetStoneItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetStoneItemsInventoryItem(i)->GetCount())
		{
			count -= GetStoneItemsInventoryItem(i)->GetCount();
			GetStoneItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetStoneItemsInventoryItem(i)->SetCount(GetStoneItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::RemoveSpecifyTypeItemByChestItemsInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < CHEST_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetChestItemsInventoryItem(i))
			continue;

		if (GetChestItemsInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetChestItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetChestItemsInventoryItem(i)->GetCount())
		{
			count -= GetChestItemsInventoryItem(i)->GetCount();
			GetChestItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChestItemsInventoryItem(i)->SetCount(GetChestItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::RemoveSpecifyTypeItemByAttrItemsInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < ATTR_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetAttrItemsInventoryItem(i))
			continue;

		if (GetAttrItemsInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetAttrItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetAttrItemsInventoryItem(i)->GetCount())
		{
			count -= GetAttrItemsInventoryItem(i)->GetCount();
			GetAttrItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetAttrItemsInventoryItem(i)->SetCount(GetAttrItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

void CHARACTER::RemoveSpecifyTypeItemByFlowerItemsInventory(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < FLOWER_ITEMS_INVENTORY_MAX_NUM; ++i)
	{
		if (nullptr == GetFlowerItemsInventoryItem(i))
			continue;

		if (GetFlowerItemsInventoryItem(i)->GetType() != type)
			continue;

		//°³ÀÎ »óÁ¡¿¡ µî·ÏµÈ ¹°°ÇÀÌ¸é ³Ñ¾î°£´Ù. (°³ÀÎ »óÁ¡¿¡¼­ ÆÇ¸ÅµÉ¶§ ÀÌ ºÎºÐÀ¸·Î µé¾î¿Ã °æ¿ì ¹®Á¦!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetFlowerItemsInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetFlowerItemsInventoryItem(i)->GetCount())
		{
			count -= GetFlowerItemsInventoryItem(i)->GetCount();
			GetFlowerItemsInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetFlowerItemsInventoryItem(i)->SetCount(GetFlowerItemsInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}
#endif

void CHARACTER::AutoGiveItem(LPITEM item, bool longOwnerShip)
{
	if (nullptr == item)
	{
		sys_err("nullptr point.");
		return;
	}
	if (item->GetOwner())
	{
		sys_err("item %d 's owner exists!", item->GetID());
		return;
	}

	int cell;
	if (item->IsDragonSoul())
		cell = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		cell = GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		cell = GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsStone())
		cell = GetEmptyStoneInventory(item->GetSize());
	else if (item->IsChest())
		cell = GetEmptyChestInventory(item->GetSize());
	else if (item->IsAttr())
		cell = GetEmptyAttrInventory(item->GetSize());
	else if (item->IsFlower())
		cell = GetEmptyFlowerInventory(item->GetSize());
#endif
	else
		cell = GetEmptyInventory(item->GetSize());

	if (cell != -1)
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, cell));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, cell));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, cell));
		else if (item->IsStone())
			item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, cell));
		else if (item->IsChest())
			item->AddToCharacter(this, TItemPos(CHEST_ITEMS_INVENTORY, cell));
		else if (item->IsAttr())
			item->AddToCharacter(this, TItemPos(ATTR_ITEMS_INVENTORY, cell));
		else if (item->IsFlower())
			item->AddToCharacter(this, TItemPos(FLOWER_ITEMS_INVENTORY, cell));
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
		
#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(0) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = this->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionHP())
				{
					int addvalue = item->GetValue(0) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					this->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d HP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
		else if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(1) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = this->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionSP())
				{
					int addvalue = item->GetValue(1) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					this->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d SP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
#endif

		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && (item->GetValue(0) > 0 || item->GetValue(1) > 0))
#else
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
#endif
		{
			TQuickslot* pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = cell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();

		if (longOwnerShip)
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}
}

static bool IsUpgradeItem(DWORD dwVnum)
{
	std::vector<int> upgradeVector = ITEM_MANAGER::instance().GetUpgradeItemTableVector();
	if (upgradeVector.empty())
	{
		return false;
	}
	
	return std::find(upgradeVector.begin(), upgradeVector.end(), dwVnum) != upgradeVector.end();
}

static bool IsStoneSpecial(DWORD dwVnum, BYTE type)
{
	std::vector<int> stoneVector = ITEM_MANAGER::instance().GetStoneItemTableVector();
	if (stoneVector.empty())
	{
		if (type == ITEM_METIN)
			return true;

		return false;
	}
	
	if (type == ITEM_METIN)
		return true;
	
	return std::find(stoneVector.begin(), stoneVector.end(), dwVnum) != stoneVector.end();
}

static bool IsChest(DWORD dwVnum, BYTE type)
{
	std::vector<int> chestVector = ITEM_MANAGER::instance().GetChestItemTableVector();
	if (chestVector.empty())
	{
		if (type == ITEM_GIFTBOX)
			return true;
		
		if (type == ITEM_GACHA)
			return true;

		return false;
	}
	
	if (type == ITEM_GIFTBOX)
		return true;
	
	if (type == ITEM_GACHA)
		return true;
	
	return std::find(chestVector.begin(), chestVector.end(), dwVnum) != chestVector.end();
}

LPITEM CHARACTER::AutoGiveItem(DWORD dwItemVnum, BYTE bCount, int iRarePct, bool bMsg)
{
	TItemTable* p = ITEM_MANAGER::instance().GetTable(dwItemVnum);

	if (!p)
		return nullptr;

	DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, dwItemVnum, bCount);

	if (p->dwFlags & ITEM_FLAG_STACKABLE && p->bType != ITEM_BLEND)
	{
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		if (IsUpgradeItem(dwItemVnum)) //upgrade item
		{
			for (int i = 0; i < 225; ++i)
			{
				LPITEM item = GetUpgradeItemsInventoryItem(i);
				if (!item)
					continue;
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					BYTE bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
					bCount -= bCount2;
					if (bCount2 > 0)
						item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), item->GetName());
						return item;
					}
				}
			}
		}
		else if (IsStoneSpecial(dwItemVnum, p->bType)) //stone item
		{
			for (int i = 0; i < 225; ++i)
			{
				LPITEM item = GetStoneItemsInventoryItem(i);
				if (!item)
					continue;
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}
					BYTE bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
					bCount -= bCount2;
					if (bCount2 > 0)
						item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), item->GetName());
						return item;
					}
				}
			}
		}
		else if (IsChest(dwItemVnum, p->bType))
		{
			for (int i = 0; i < 225; ++i)
			{
				LPITEM item = GetChestItemsInventoryItem(i);
				if (!item)
					continue;
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}
					BYTE bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
					bCount -= bCount2;
					if (bCount2 > 0)
						item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), item->GetName());
						return item;
					}
				}
			}
		}
		else
		{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
			for (int i = 0; i < GetExtendInvenMax(); ++i)
#else
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
			{
				LPITEM item = GetInventoryItem(i);
				if (!item)
					continue;
				
#ifdef ENABLE_AUTO_POTION_RENEWAL
				if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(0) > 0)
					continue;
#endif
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}
					BYTE bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
					bCount -= bCount2;
					if (bCount2 > 0)
						item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item->GetName());
						return item;
					}
				}
			}
		}
#else
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item = GetInventoryItem(i);
			if (!item)
				continue;
			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}
				BYTE bCount2 = MIN(g_bItemCountLimit - item->GetCount(), bCount);
				bCount -= bCount2;
				if (bCount2 > 0)
					item->SetCount(item->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item->GetName());
					return item;
				}
			}
		}
#endif
	}

	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwItemVnum, bCount, 0, true);

	if (!item)
	{
		sys_err("cannot create item by vnum %u (name: %s)", dwItemVnum, GetName());
		return nullptr;
	}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	if (item->IsSkillBook())
	{
		for (int i = 0; i < 225; ++i)
		{
			LPITEM book = GetSkillBookInventoryItem(i);

			if (!book)
				continue;

			if (book->GetVnum() == dwItemVnum && book->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				BYTE bCount2 = MIN(g_bItemCountLimit - book->GetCount(), bCount);
				bCount -= bCount2;

				if (bCount2 > 0)
					book->SetCount(book->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), book->GetName());

					M2_DESTROY_ITEM(item);
					return book;
				}
			}
		}
	}
	
	if (item->IsAttr())
	{
		for (int i = 0; i < 225; ++i)
		{
			LPITEM attr = GetAttrItemsInventoryItem(i);

			if (!attr)
				continue;
			
			if (attr->GetCount() == g_bItemCountLimit)
				continue;
			
			if (attr->GetVnum() == dwItemVnum && attr->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				BYTE bCount2 = MIN(g_bItemCountLimit - attr->GetCount(), bCount);
				bCount -= bCount2;
				
				if (bCount2 > 0)
					attr->SetCount(attr->GetCount() + bCount2);
				
				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), attr->GetName());

					M2_DESTROY_ITEM(item);
					return attr;
				}
			}
		}
	}
	
	if (item->IsFlower())
	{
		for (int i = 0; i < 225; ++i)
		{
			LPITEM flower = GetFlowerItemsInventoryItem(i);

			if (!flower)
				continue;

			if (flower->GetVnum() == dwItemVnum && flower->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				BYTE bCount2 = MIN(g_bItemCountLimit - flower->GetCount(), bCount);
				bCount -= bCount2;

				if (bCount2 > 0)
					flower->SetCount(flower->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lCRELU Caµc: %s"), flower->GetName());

					M2_DESTROY_ITEM(item);
					return flower;
				}
			}
		}
	}
#endif

	if (item->GetType() == ITEM_BLEND)
	{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		for (int i = 0; i < GetExtendInvenMax(); i++)
#else
		for (int i = 0; i < INVENTORY_MAX_NUM; i++)
#endif
		{
			LPITEM inv_item = GetInventoryItem(i);

			if (inv_item == nullptr) continue;

			if (inv_item->GetType() == ITEM_BLEND)
			{
				if (inv_item->GetVnum() == item->GetVnum())
				{
					if (inv_item->GetSocket(0) == item->GetSocket(0) &&
						inv_item->GetSocket(1) == item->GetSocket(1) &&
						inv_item->GetSocket(2) == item->GetSocket(2) &&
						inv_item->GetCount() + item->GetCount() <= ITEM_MAX_COUNT)
					{
						inv_item->SetCount(inv_item->GetCount() + item->GetCount());
						M2_DESTROY_ITEM(item);
						return inv_item;
					}
				}
			}
		}
	}

	int iEmptyCell;
	if (item->IsDragonSoul())
		iEmptyCell = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		iEmptyCell = GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsStone())
		iEmptyCell = GetEmptyStoneInventory(item->GetSize());
	else if (item->IsChest())
		iEmptyCell = GetEmptyChestInventory(item->GetSize());
	else if (item->IsAttr())
		iEmptyCell = GetEmptyAttrInventory(item->GetSize());
	else if (item->IsFlower())
		iEmptyCell = GetEmptyFlowerInventory(item->GetSize());
#endif
	else
		iEmptyCell = GetEmptyInventory(item->GetSize());

	if (iEmptyCell != -1)
	{
		if (bMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ È¹µæ: %s"), item->GetName());

		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, iEmptyCell));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyCell));
		else if (item->IsStone())
			item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, iEmptyCell));
		else if (item->IsChest())
			item->AddToCharacter(this, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyCell));
		else if (item->IsAttr())
			item->AddToCharacter(this, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyCell));
		else if (item->IsFlower())
			item->AddToCharacter(this, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyCell));
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
		
#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(0) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = this->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionHP())
				{
					int addvalue = item->GetValue(0) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					this->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d HP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
		else if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(1) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = this->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionSP())
				{
					int addvalue = item->GetValue(1) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					this->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d SP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
#endif
		
		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && (item->GetValue(0) > 0 || item->GetValue(1) > 0))
#else
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
#endif
		{
			TQuickslot* pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = iEmptyCell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();
		// ¾ÈÆ¼ µå¶ø flag°¡ °É·ÁÀÖ´Â ¾ÆÀÌÅÛÀÇ °æ¿ì,
		// ÀÎº¥¿¡ ºó °ø°£ÀÌ ¾ø¾î¼­ ¾îÂ¿ ¼ö ¾øÀÌ ¶³¾îÆ®¸®°Ô µÇ¸é,
		// ownershipÀ» ¾ÆÀÌÅÛÀÌ »ç¶óÁú ¶§±îÁö(300ÃÊ) À¯ÁöÇÑ´Ù.
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP))
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}

	sys_log(0,
		"7: %d %d", dwItemVnum, bCount);
	return item;
}

bool CHARACTER::GiveItem(LPCHARACTER victim, TItemPos Cell)
{
	if (!CanHandleItem())
		return false;

	LPITEM item = GetItem(Cell);

	if (item && !item->IsExchanging())
	{
		if (victim->CanReceiveItem(this, item))
		{
			victim->ReceiveItem(this, item);
			return true;
		}
	}

	return false;
}

bool CHARACTER::CanReceiveItem(LPCHARACTER from, LPITEM item) const
{
	if (IsPC())
		return false;

	// TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
	if (DISTANCE_APPROX(GetX() - from->GetX(), GetY() - from->GetY()) > 2000)
		return false;
	// END_OF_TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (item->IsSealed()) {
		from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't upgrade sealbind item."));
		return false;
	}
#endif

	if (item->IsBasicItem())
	{
		from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return false;
	}

	switch (GetRaceNum())
	{
	case fishing::CAMPFIRE_MOB:
		if (item->GetType() == ITEM_FISH &&
			(item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			return true;
		break;

	case fishing::FISHER_MOB:
		if (item->GetType() == ITEM_ROD)
			return true;
		break;

		// BUILDING_NPC
	case BLACKSMITH_WEAPON_MOB:
	case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
		if (item->GetType() == ITEM_WEAPON &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;

	case BLACKSMITH_ARMOR_MOB:
	case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
		if (item->GetType() == ITEM_ARMOR &&
			(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;

	case BLACKSMITH_ACCESSORY_MOB:
	case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
		if (item->GetType() == ITEM_ARMOR &&
			!(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;
		// END_OF_BUILDING_NPC

	case BLACKSMITH_MOB:
		if (item->GetRefinedVnum() && (item->GetRefineSet() < 500 || item->GetRefineSet() >= 10000))
		{
			return true;
		}
		else
		{
			return false;
		}

	case BLACKSMITH2_MOB:
		if (item->GetRefineSet() >= 500)
		{
			return true;
		}
		else
		{
			return false;
		}

	case ALCHEMIST_MOB:
		if (item->GetRefinedVnum())
			return true;
		break;

	case 20101:
	case 20102:
	case 20103:
		// ÃÊ±Þ ¸»
		if (item->GetVnum() == ITEM_REVIVE_HORSE_1)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Áö ¾ÊÀº ¸»¿¡°Ô ¼±ÃÊ¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Àº ¸»¿¡°Ô »ç·á¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_2 || item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			return false;
		}
		break;
	case 20104:
	case 20105:
	case 20106:
		// Áß±Þ ¸»
		if (item->GetVnum() == ITEM_REVIVE_HORSE_2)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Áö ¾ÊÀº ¸»¿¡°Ô ¼±ÃÊ¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_2)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Àº ¸»¿¡°Ô »ç·á¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			return false;
		}
		break;
	case 20107:
	case 20108:
	case 20109:
		// °í±Þ ¸»
		if (item->GetVnum() == ITEM_REVIVE_HORSE_3)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Áö ¾ÊÀº ¸»¿¡°Ô ¼±ÃÊ¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á×Àº ¸»¿¡°Ô »ç·á¸¦ ¸ÔÀÏ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_2)
		{
			return false;
		}
		break;
	}

	//if (IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_GIVE))
	{
		return true;
	}

	return false;
}

void CHARACTER::ReceiveItem(LPCHARACTER from, LPITEM item)
{
	if (IsPC())
		return;

	switch (GetRaceNum())
	{
	case fishing::CAMPFIRE_MOB:
		if (item->GetType() == ITEM_FISH && (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			fishing::Grill(from, item);
		else
		{
			// TAKE_ITEM_BUG_FIX
			from->SetQuestNPCID(GetVID());
			// END_OF_TAKE_ITEM_BUG_FIX
			quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		}
		break;

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	case MeleyLair::STATUE_VNUM:
	{
		if (MeleyLair::CMgr::instance().IsMeleyMap(from->GetMapIndex()))
			MeleyLair::CMgr::instance().OnKillStatue(item, from, this, from->GetGuild());
	}
	break;
#endif

	// DEVILTOWER_NPC
	case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
	case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
	case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
		if (item->GetRefinedVnum() != 0 && item->GetRefineSet() != 0 && item->GetRefineSet() < 500)
		{
			from->SetRefineNPC(this);
			from->RefineInformation(item->GetCell(), REFINE_TYPE_MONEY_ONLY);
		}
		else
		{
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		break;
		// END_OF_DEVILTOWER_NPC

	case BLACKSMITH_MOB:
	case BLACKSMITH2_MOB:
	case BLACKSMITH_WEAPON_MOB:
	case BLACKSMITH_ARMOR_MOB:
	case BLACKSMITH_ACCESSORY_MOB:
		if (item->GetRefinedVnum())
		{
			from->SetRefineNPC(this);
			from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL);
		}
		else
		{
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ ¾ÆÀÌÅÛÀº °³·®ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		break;

	case 20101:
	case 20102:
	case 20103:
	case 20104:
	case 20105:
	case 20106:
	case 20107:
	case 20108:
	case 20109:
		if (item->GetVnum() == ITEM_REVIVE_HORSE_1 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_2 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_3)
		{
			from->ReviveHorse();
			item->SetCount(item->GetCount() - 1);
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡°Ô ¼±ÃÊ¸¦ ÁÖ¾ú½À´Ï´Ù."));
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 ||
			item->GetVnum() == ITEM_HORSE_FOOD_2 ||
			item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			from->FeedHorse();
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡°Ô »ç·á¸¦ ÁÖ¾ú½À´Ï´Ù."));
			item->SetCount(item->GetCount() - 1);
			EffectPacket(SE_HPUP_RED);
		}
		break;

	default:
		sys_log(0, "TakeItem %s %d %s", from->GetName(), GetRaceNum(), item->GetName());
		from->SetQuestNPCID(GetVID());
		quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		break;
	}
}

bool CHARACTER::IsEquipUniqueItem(DWORD dwItemVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}
	
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	{
		LPITEM u = GetWear(WEAR_COSTUME_MOUNT);
		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}
#endif

	// ¾ð¾î¹ÝÁöÀÎ °æ¿ì ¾ð¾î¹ÝÁö(°ßº») ÀÎÁöµµ Ã¼Å©ÇÑ´Ù.
	if (dwItemVnum == UNIQUE_ITEM_RING_OF_LANGUAGE)
		return IsEquipUniqueItem(UNIQUE_ITEM_RING_OF_LANGUAGE_SAMPLE);

	return false;
}

// CHECK_UNIQUE_GROUP
bool CHARACTER::IsEquipUniqueGroup(DWORD dwGroupVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}
	
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	{
		LPITEM u = GetWear(WEAR_COSTUME_MOUNT);
		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}
#endif

	return false;
}
// END_OF_CHECK_UNIQUE_GROUP

void CHARACTER::SetRefineMode(int iAdditionalCell)
{
	m_iRefineAdditionalCell = iAdditionalCell;
	m_bUnderRefine = true;
}

void CHARACTER::ClearRefineMode()
{
	m_bUnderRefine = false;
	SetRefineNPC(nullptr);
}

bool CHARACTER::GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector<DWORD> & dwItemVnums,
	std::vector<DWORD> & dwItemCounts, std::vector <LPITEM> & item_gets, int& count)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);

	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}

	std::vector <int> idxes;
	int n = pGroup->GetMultiIndex(idxes);

	bool bSuccess;

	for (int i = 0; i < n; i++)
	{
		bSuccess = false;
		int idx = idxes[i];
		DWORD dwVnum = pGroup->GetVnum(idx);
		DWORD dwCount = pGroup->GetCount(idx);
		int	iRarePct = pGroup->GetRarePct(idx);
		LPITEM item_get = nullptr;
		switch (dwVnum)
		{
		case CSpecialItemGroup::GOLD:
			PointChange(POINT_GOLD, dwCount);
			LogManager::instance().CharLog(this, dwCount, "TREASURE_GOLD", "");

			bSuccess = true;
			break;
		case CSpecialItemGroup::EXP:
		{
			PointChange(POINT_EXP, dwCount);
			LogManager::instance().CharLog(this, dwCount, "TREASURE_EXP", "");

			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::MOB:
		{
			sys_log(0, "CSpecialItemGroup::MOB %d", dwCount);
			int x = GetX() + number(-500, 500);
			int y = GetY() + number(-500, 500);

			LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwCount, GetMapIndex(), x, y, 0, true, -1);
			if (ch)
				ch->SetAggressive();
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::SLOW:
		{
			sys_log(0, "CSpecialItemGroup::SLOW %d", -(int)dwCount);
			AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)dwCount, AFF_SLOW, 300, 0, true);
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::DRAIN_HP:
		{
			int iDropHP = GetMaxHP() * dwCount / 100;
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			iDropHP = MIN(iDropHP, GetHP() - 1);
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			PointChange(POINT_HP, -iDropHP);
			bSuccess = true;
		}
		break;
		case CSpecialItemGroup::POISON:
		{
			AttackedByPoison(nullptr);
			bSuccess = true;
		}
		break;

		// case CSpecialItemGroup::BLEED:
			// {
				// AttackedByBleeding(nullptr);
				// bSuccess = true;
			// }
			// break;

		case CSpecialItemGroup::MOB_GROUP:
		{
			int sx = GetX() - number(300, 500);
			int sy = GetY() - number(300, 500);
			int ex = GetX() + number(300, 500);
			int ey = GetY() + number(300, 500);
			CHARACTER_MANAGER::instance().SpawnGroup(dwCount, GetMapIndex(), sx, sy, ex, ey, nullptr, true);

			bSuccess = true;
		}
		break;
		default:
		{
			if (dwGroupNum >= 81550 && dwGroupNum <= 81571) // giftbox crash fix
				item_get = AutoGiveItem(dwVnum, dwCount, iRarePct, false);
			else
				item_get = AutoGiveItem(dwVnum, dwCount, iRarePct);

			if (item_get)
			{
				bSuccess = true;
			}
		}
		break;
		}

		if (bSuccess)
		{
			dwItemVnums.push_back(dwVnum);
			dwItemCounts.push_back(dwCount);
			item_gets.push_back(item_get);
			count++;
		}
		else
		{
			return false;
		}
	}
	return bSuccess;
}

// NEW_HAIR_STYLE_ADD
bool CHARACTER::ItemProcess_Hair(LPITEM item, int iDestCell)
{
	if (item->CheckItemUseLevel(GetLevel()) == false)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ ÀÌ ¸Ó¸®¸¦ »ç¿ëÇÒ ¼ö ¾ø´Â ·¹º§ÀÔ´Ï´Ù."));
		return false;
	}

	DWORD hair = item->GetVnum();
	switch (GetJob())
	{
	case JOB_WARRIOR:
		hair -= 72000;
		break;

	case JOB_ASSASSIN:
		hair -= 71250;
		break;

	case JOB_SURA:
		hair -= 70500;
		break;

	case JOB_SHAMAN:
		hair -= 69750;
		break;
#ifdef ENABLE_WOLFMAN_CHARACTER
	case JOB_WOLFMAN:
		/* (delete "return false" and add "hair -= vnum" just if you have wolf hair on your server) */
		// hair -= 70750
		return false;
		break;
#endif
	default:
		return false;
		break;
	}

	if (hair == GetPart(PART_HAIR))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ¿ÀÏÇÑ ¸Ó¸® ½ºÅ¸ÀÏ·Î´Â ±³Ã¼ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return true;
	}

	item->SetCount(item->GetCount() - 1);

	SetPart(PART_HAIR, static_cast<WORD>(hair));
	UpdatePacket();

	return true;
}

// END_NEW_HAIR_STYLE_ADD

bool CHARACTER::ItemProcess_Polymorph(LPITEM item)
{
	if (IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì µÐ°©ÁßÀÎ »óÅÂÀÔ´Ï´Ù."));
		return false;
	}
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_use_polymorph_item"));
		return false;
	}
#endif

	if (true == IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°©ÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù."));
		return false;
	}

	DWORD dwVnum = item->GetSocket(0);

	if (dwVnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àß¸øµÈ µÐ°© ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	const CMob* pMob = CMobManager::instance().Get(dwVnum);

	if (pMob == nullptr)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Àß¸øµÈ µÐ°© ¾ÆÀÌÅÛÀÔ´Ï´Ù."));
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	switch (item->GetVnum())
	{
	case 70104:
	case 70105:
	case 70106:
	case 70107:
	case 71093:
	{
		// µÐ°©±¸ Ã³¸®
		sys_log(0, "USE_POLYMORPH_BALL PID(%d) vnum(%d)", GetPlayerID(), dwVnum);

		// ·¹º§ Á¦ÇÑ Ã¼Å©
		int iPolymorphLevelLimit = MAX(0, 20 - GetLevel() * 3 / 10);
		if (pMob->m_table.bLevel >= GetLevel() + iPolymorphLevelLimit)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³ªº¸´Ù ³Ê¹« ³ôÀº ·¹º§ÀÇ ¸ó½ºÅÍ·Î´Â º¯½Å ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}

		int iDuration = GetSkillLevel(POLYMORPH_SKILL_ID) == 0 ? 5 : (5 + (5 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40 * 25));
		iDuration *= 60;
		DWORD dwBonus = (2 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40) * 100;

		AddAffect(AFFECT_POLYMORPH, POINT_POLYMORPH, dwVnum, AFF_POLYMORPH, iDuration, 0, true);
		AddAffect(AFFECT_POLYMORPH, POINT_ATT_BONUS, dwBonus, AFF_POLYMORPH, iDuration, 0, false);

		item->SetCount(item->GetCount() - 1);
	}
	break;

	case 50322:
	{
		// º¸·ù

		// µÐ°©¼­ Ã³¸®
		// ¼ÒÄÏ0                ¼ÒÄÏ1           ¼ÒÄÏ2
		// µÐ°©ÇÒ ¸ó½ºÅÍ ¹øÈ£   ¼ö·ÃÁ¤µµ        µÐ°©¼­ ·¹º§
		sys_log(0, "USE_POLYMORPH_BOOK: %s(%u) vnum(%u)", GetName(), GetPlayerID(), dwVnum);

		if (CPolymorphUtils::instance().PolymorphCharacter(this, item, pMob) == true)
		{
			CPolymorphUtils::instance().UpdateBookPracticeGrade(this, item);
		}
	}
	break;

	default:
		sys_err("POLYMORPH invalid item passed PID(%d) vnum(%d)", GetPlayerID(), item->GetOriginalVnum());
		return false;
	}

	return true;
}

bool CHARACTER::ItemProccess_DS(CItem * item, const TItemPos& DestCell)
{
	// Fix
	// Disappears stone when drag a stone dragon of alchemy that is the circle of
	// alchemy to another alchemical stone that is also in the circle
	if (DestCell.IsEquipPosition())
	{
		if (GetItem(DestCell))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Àåºñ¸¦ Âø¿ëÇÏ°í ÀÖ½À´Ï´Ù."));
			return false;
		}
	}

	if (!item->IsEquipped())
		return false;

	return DSManager::instance().PullOut(this, NPOS, item);
}

bool CHARACTER::CanDoCube() const
{
	if (m_bIsObserver)	return false;
	if (GetShop())		return false;
	if (GetMyShop())	return false;
	if (m_bUnderRefine)	return false;
	if (IsWarping())	return false;
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (IsCombOpen()) return false;
#endif
	return true;
}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
bool CHARACTER::CanDoComb() const
{
	if (m_bIsObserver)		return false;
	if (GetShop())			return false;
	if (GetMyShop())		return false;
	if (m_bUnderRefine)		return false;
	if (IsWarping())		return false;
	if (IsCubeOpen())		return false;

	return true;
}
#endif

bool CHARACTER::UnEquipSpecialRideUniqueItem()
{
	LPITEM unique1 = GetWear(WEAR_UNIQUE1);
	if (unique1 != nullptr)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == unique1->GetSpecialGroup())
			return UnequipItem(unique1);
	}

	LPITEM unique2 = GetWear(WEAR_UNIQUE2);
	if (unique2 != nullptr)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == unique2->GetSpecialGroup())
			return UnequipItem(unique2);
	}

	return true;
}

void CHARACTER::AutoRecoveryItemProcess(const EAffectTypes type)
{
	if (true == IsDead() || true == IsStun())
		return;

	if (false == IsPC())
		return;

	if (AFFECT_AUTO_HP_RECOVERY != type && AFFECT_AUTO_SP_RECOVERY != type)
		return;
	if (nullptr != FindAffect(AFFECT_STUN))
		return;

	{
		const DWORD stunSkills[] = { SKILL_TANHWAN, SKILL_GEOMPUNG, SKILL_BYEURAK, SKILL_GIGUNG };

		for (size_t i = 0; i < sizeof(stunSkills) / sizeof(DWORD); ++i)
		{
			const CAffect* p = FindAffect(stunSkills[i]);

			if (nullptr != p && AFF_STUN == p->dwFlag)
				return;
		}
	}

	const CAffect* pAffect = FindAffect(type);
	const size_t idx_of_amount_of_used = 1;
	const size_t idx_of_amount_of_full = 2;

	if (nullptr != pAffect)
	{
		LPITEM pItem = FindItemByID(pAffect->dwFlag);

		if (nullptr != pItem && true == pItem->GetSocket(0))
		{
			if (false == CArenaManager::instance().IsArenaMap(GetMapIndex()))
			{
				const long amount_of_used = pItem->GetSocket(idx_of_amount_of_used);
				const long amount_of_full = pItem->GetSocket(idx_of_amount_of_full);

				const int32_t avail = amount_of_full - amount_of_used;

				int32_t amount = 0;

				if (AFFECT_AUTO_HP_RECOVERY == type)
				{
					amount = GetMaxHP() - (GetHP() + GetPoint(POINT_HP_RECOVERY));
				}
				else if (AFFECT_AUTO_SP_RECOVERY == type)
				{
					amount = GetMaxSP() - (GetSP() + GetPoint(POINT_SP_RECOVERY));
				}

				if (amount > 0)
				{
					if (avail > amount)
					{
						const int pct_of_used = amount_of_used * 100 / amount_of_full;
						const int pct_of_will_used = (amount_of_used + amount) * 100 / amount_of_full;

						bool bLog = false;
						// »ç¿ë·®ÀÇ 10% ´ÜÀ§·Î ·Î±×¸¦ ³²±è
						// (»ç¿ë·®ÀÇ %¿¡¼­, ½ÊÀÇ ÀÚ¸®°¡ ¹Ù²ð ¶§¸¶´Ù ·Î±×¸¦ ³²±è.)
						if ((pct_of_will_used / 10) - (pct_of_used / 10) >= 1)
							bLog = true;
						pItem->SetSocket(idx_of_amount_of_used, amount_of_used + amount, bLog);
					}
					else
					{
						amount = avail;

#ifndef ENABLE_AUTO_POTION_RENEWAL
						ITEM_MANAGER::instance().RemoveItem(pItem);
#endif
					}

					if (AFFECT_AUTO_HP_RECOVERY == type)
					{
						PointChange(POINT_HP_RECOVERY, amount);
						EffectPacket(SE_AUTO_HPUP);
					}
					else if (AFFECT_AUTO_SP_RECOVERY == type)
					{
						PointChange(POINT_SP_RECOVERY, amount);
						EffectPacket(SE_AUTO_SPUP);
					}
				}
			}
			else
			{
				pItem->Lock(false);
				pItem->SetSocket(0, false);
				RemoveAffect(const_cast<CAffect*>(pAffect));
			}
		}
		else
		{
			RemoveAffect(const_cast<CAffect*>(pAffect));
		}
	}
}

bool CHARACTER::IsValidItemPosition(TItemPos Pos) const
{
	BYTE window_type = Pos.window_type;
	WORD cell = Pos.cell;

	switch (window_type)
	{
	case RESERVED_WINDOW:
		return false;

	case INVENTORY:
	case EQUIPMENT:
		return cell < (INVENTORY_AND_EQUIP_SLOT_MAX);

	case DRAGON_SOUL_INVENTORY:
		return cell < (DRAGON_SOUL_INVENTORY_MAX_NUM);

	case SAFEBOX:
		if (nullptr != m_pkSafebox)
			return m_pkSafebox->IsValidPosition(cell);
		else
			return false;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	case SKILL_BOOK_INVENTORY:
		return cell < (SKILL_BOOK_INVENTORY_MAX_NUM);

	case UPGRADE_ITEMS_INVENTORY:
		return cell < (UPGRADE_ITEMS_INVENTORY_MAX_NUM);

	case STONE_ITEMS_INVENTORY:
		return cell < (STONE_ITEMS_INVENTORY_MAX_NUM);

	case CHEST_ITEMS_INVENTORY:
		return cell < (CHEST_ITEMS_INVENTORY_MAX_NUM);
	
	case ATTR_ITEMS_INVENTORY:
		return cell < (ATTR_ITEMS_INVENTORY_MAX_NUM);
		
	case FLOWER_ITEMS_INVENTORY:
		return cell < (FLOWER_ITEMS_INVENTORY_MAX_NUM);
#endif

	case MALL:
		if (nullptr != m_pkMall)
			return m_pkMall->IsValidPosition(cell);
		else
			return false;
	default:
		return false;
	}
}

// ±ÍÂú¾Æ¼­ ¸¸µç ¸ÅÅ©·Î.. exp°¡ true¸é msg¸¦ Ãâ·ÂÇÏ°í return false ÇÏ´Â ¸ÅÅ©·Î (ÀÏ¹ÝÀûÀÎ verify ¿ëµµ¶ûÀº return ¶§¹®¿¡ ¾à°£ ¹Ý´ë¶ó ÀÌ¸§¶§¹®¿¡ Çò°¥¸± ¼öµµ ÀÖ°Ú´Ù..)
#define VERIFY_MSG(exp, msg)  \
	if (true == (exp)) { \
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT(msg)); \
			return false; \
	}

/// ÇöÀç Ä³¸¯ÅÍÀÇ »óÅÂ¸¦ ¹ÙÅÁÀ¸·Î ÁÖ¾îÁø itemÀ» Âø¿ëÇÒ ¼ö ÀÖ´Â Áö È®ÀÎÇÏ°í, ºÒ°¡´É ÇÏ´Ù¸é Ä³¸¯ÅÍ¿¡°Ô ÀÌÀ¯¸¦ ¾Ë·ÁÁÖ´Â ÇÔ¼ö
bool CHARACTER::CanEquipNow(const LPITEM item, const TItemPos & srcCell, const TItemPos & destCell)
{
	const TItemTable* itemTable = item->GetProto();
	//BYTE itemType = item->GetType();
	//BYTE itemSubType = item->GetSubType();
	switch (GetJob())
	{
	case JOB_WARRIOR:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
			return false;
		break;

	case JOB_ASSASSIN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
			return false;
		break;

	case JOB_SHAMAN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		break;

	case JOB_SURA:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
			return false;
		break;

	case JOB_WOLFMAN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
			return false;
		break;
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limit = itemTable->aLimits[i].lValue;
		switch (itemTable->aLimits[i].bType)
		{
		case LIMIT_LEVEL:
			if (GetLevel() < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·¹º§ÀÌ ³·¾Æ Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			break;

		case LIMIT_STR:
			if (GetPoint(POINT_ST) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±Ù·ÂÀÌ ³·¾Æ Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			break;

		case LIMIT_INT:
			if (GetPoint(POINT_IQ) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Áö´ÉÀÌ ³·¾Æ Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			break;

		case LIMIT_DEX:
			if (GetPoint(POINT_DX) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¹ÎÃ¸ÀÌ ³·¾Æ Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			break;

		case LIMIT_CON:
			if (GetPoint(POINT_HT) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¼·ÂÀÌ ³·¾Æ Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return false;
			}
			break;
		}
	}

	if (item->GetWearFlag() & WEARABLE_UNIQUE)
	{
		if ((GetWear(WEAR_UNIQUE1) && GetWear(WEAR_UNIQUE1)->IsSameSpecialGroup(item)) || (GetWear(WEAR_UNIQUE2) && GetWear(WEAR_UNIQUE2)->IsSameSpecialGroup(item)))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°°Àº Á¾·ùÀÇ À¯´ÏÅ© ¾ÆÀÌÅÛ µÎ °³¸¦ µ¿½Ã¿¡ ÀåÂøÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}

		if (marriage::CManager::instance().IsMarriageUniqueItem(item->GetVnum()) &&
			!marriage::CManager::instance().IsMarried(GetPlayerID()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°áÈ¥ÇÏÁö ¾ÊÀº »óÅÂ¿¡¼­ ¿¹¹°À» Âø¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return false;
		}
	}

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
#ifdef __NEW_ARROW_SYSTEM__
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW && item->GetSubType() != WEAPON_UNLIMITED_ARROW)
#else
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW)
#endif
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Silah takiliyken silah kostumunu degistiremezsin."));
			return false;
		}
	}
	else if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_WEAPON)
	{
		LPITEM pkItem = GetWear(WEAR_WEAPON);
		if (!pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Silah takiliyken kostum silahini giyemem."));
			return false;
		}
		else if (item->GetValue(3) != pkItem->GetSubType())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Farkli turde bir kostum silah giyemem."));
			return false;
		}
		else if (pkItem->GetType() == ITEM_ROD || pkItem->GetType() == ITEM_PICK)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Kazma veya olta takiliyken kostum silah giyemem."));
			return false;
		}
	}

	if (item->GetType() == ITEM_ROD || item->GetType() == ITEM_PICK)
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Silah takiliyken silah kostumunu degistiremezsin."));
			return false;
		}
	}
#endif

	if (item->GetType() == ITEM_RING) // ring check for two same rings
	{
		LPITEM ringItems[2] = { GetWear(WEAR_RING1), GetWear(WEAR_RING2) };
		for (int i = 0; i < 2; i++)
		{
			if (ringItems[i]) // if that item is equipped
			{
				if (ringItems[i]->GetVnum() == item->GetVnum())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("iki kez bu objeyi takamazsin!"));
					return false;
				}
			}
		}
	}

#ifdef ENABLE_COSTUME_UNEQUIP_MARRIAGE_ITEMS
	if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_BODY)
	{
		LPITEM item = GetWear(WEAR_BODY);
		if (item && (item->GetVnum() >= 11901 && item->GetVnum() <= 11914))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Uzerinde gelinlik veya smokin varken kostum giyemezsin."));
			return false;
		}
	}

	if (item->GetVnum() >= 11901 && item->GetVnum() <= 11914)
	{
		LPITEM item = GetWear(WEAR_COSTUME_BODY);
		if (item && (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_BODY))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Uzerinde kostum varken smokin veya gelinlik giyemezsin."));
			return false;
		}
	}
#endif

	return true;
}

/// ÇöÀç Ä³¸¯ÅÍÀÇ »óÅÂ¸¦ ¹ÙÅÁÀ¸·Î Âø¿ë ÁßÀÎ itemÀ» ¹þÀ» ¼ö ÀÖ´Â Áö È®ÀÎÇÏ°í, ºÒ°¡´É ÇÏ´Ù¸é Ä³¸¯ÅÍ¿¡°Ô ÀÌÀ¯¸¦ ¾Ë·ÁÁÖ´Â ÇÔ¼ö
bool CHARACTER::CanUnequipNow(const LPITEM item, const TItemPos & srcCell, const TItemPos & destCell)
{
	if (ITEM_BELT == item->GetType())
		VERIFY_MSG(CBeltInventoryHelper::IsExistItemInBeltInventory(this), "º§Æ® ÀÎº¥Åä¸®¿¡ ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ¸é ÇØÁ¦ÇÒ ¼ö ¾ø½À´Ï´Ù.");

	// ¿µ¿øÈ÷ ÇØÁ¦ÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛ
	if (IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	// ¾ÆÀÌÅÛ unequip½Ã ÀÎº¥Åä¸®·Î ¿Å±æ ¶§ ºó ÀÚ¸®°¡ ÀÖ´Â Áö È®ÀÎ
	{
		int pos = -1;
		if (item->IsDragonSoul())
			pos = GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			pos = GetEmptySkillBookInventory(item->GetSize());
		else if (item->IsUpgradeItem())
			pos = GetEmptyUpgradeItemsInventory(item->GetSize());
		else if (item->IsStone())
			pos = GetEmptyStoneInventory(item->GetSize());
#endif
		else
			pos = GetEmptyInventory(item->GetSize());

		VERIFY_MSG(-1 == pos, "¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù.");
	}

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
#ifdef ENABLE_QUIVER_SYSTEM
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW && item->GetSubType() != WEAPON_QUIVER)
#else
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW)
#endif
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Silah takiliyken silah kostumunu degistiremezsin!."));
			return false;
		}
	}
#endif

	return true;
}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
void CHARACTER::ExtendInventoryItemUse(CHARACTER * ch)
{
	const short CurrentStage = ch->GetExtendInvenStage();
	if (CurrentStage >= 27)
	{
		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("IS_FULL_MAX_EXTEND_INVENTORY_SLOTS: Current Stage %d"), CurrentStage);
	}

	const short ExItemNeedCount = ch->GetExtendItemNeedCount();
	const short ItemNeedLeft = ExItemNeedCount - (CountSpecifyItem(72319) + CountSpecifyItem(72320));

	if (CountSpecifyItem(72319) + CountSpecifyItem(72320) < ExItemNeedCount)
	{
		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_NEED_%d_FOR_EXTEND"), ItemNeedLeft);
	}

	LPDESC d = ch->GetDesc();

	TPacketGCExtendInventory p1;

	p1.header = HEADER_GC_EXTEND_INVENTORY;
	p1.id = CurrentStage;
	p1.id1 = ItemNeedLeft;
	p1.id2 = ExItemNeedCount;
	p1.subheader = NOTIFY_EXTEND_INVEN_ITEM_USE;
	d->Packet(&p1, sizeof(p1));
}
#endif

void CHARACTER::CheckPotions()
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->GetValue(1) == 64 || item->GetValue(1) == 65 || item->GetValue(1) == 69 || item->GetValue(1) == 70 || item->GetValue(1) == 15 || item->GetValue(1) == 16)
		{
			const CAffect* pAffect = FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType);
			if (item->GetSocket(1) == 1)
			{
				if (pAffect == nullptr)
				{
					AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, INFINITE_AFFECT_DURATION, 0, false);
					item->SetSocket(1, 1);
				}
			}
			else
			{
				if (pAffect)
				{
					RemoveAffect(const_cast<CAffect*>(pAffect));
					item->SetSocket(1, 0);
				}
			}
		}
	}
}

void CHARACTER::CheckTeleportItems()
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->IsPetItem())
		{
			if (item->GetSocket(1) > 0)
			{
				CPetSystem* petSystem = GetPetSystem();
				if (petSystem)
					petSystem->Summon(item->GetValue(0), item, 0, false);
			}
		}
		else if (item->GetVnum() <= 55710 && item->GetVnum() >= 55701)
		{
			if (item->GetSocket(0) > 0)
			{
				CNewPetSystem* petNewSystem = GetNewPetSystem();
				if (petNewSystem)
					petNewSystem->Summon(item->GetValue(0), item, 0, false);
			}
		}
		else if (item->GetVnum() >= 81001 && item->GetVnum() <= 81005)
		{
			if (item->GetSocket(1) > 0)
			{
				CSupportShaman* shaman = GetSupportShaman();
				if (shaman)
					shaman->Summon(34077, item, 0, false);
			}
		}
	}
}

void CHARACTER::CheckSoul()
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->GetType() == ITEM_SOUL)
		{
			if (item->GetSocket(1) > 0)
			{
				UseItemEx(item, TItemPos(item->GetWindow(), item->GetCell()));
			}
		}
	}
}

float CHARACTER::GetSoulDamVal(bool bMelee)
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->GetType() == ITEM_SOUL)
		{
			if (bMelee)
			{
				if (item->GetSocket(2) == 0)
				{
					if (item->GetSubType() == RED_SOUL)
					{
						if (IsAffectFlag(AFF_MIX_SOUL))
						{
							RemoveAffect(AFF_MIX_SOUL);
							AddAffect(AFFECT_SOUL, 0, 0, AFF_BLUE_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						}
						else if (IsAffectFlag(AFF_RED_SOUL))
						{
							RemoveAffect(AFFECT_SOUL);
						}
					}
					item->SetCount(item->GetCount()-1);
				}
				if (item->GetSubType() == RED_SOUL && item->GetSocket(1) > 0)
				{
					item->SetSocket(2, item->GetSocket(2) - 1);
					if (item->GetSocket(1) > 0)
						return float(item->GetValue(3)/10.0f);

					return 1.0f;
				}
			}
			else
			{
				if (item->GetSocket(2) == 0)
				{
					if (item->GetSubType() == BLUE_SOUL)
					{
						if (IsAffectFlag(AFF_MIX_SOUL))
						{
							RemoveAffect(AFFECT_SOUL);
							AddAffect(AFFECT_SOUL, 0, 0, AFF_RED_SOUL, INFINITE_AFFECT_DURATION, 0, false);
						}
						else if (IsAffectFlag(AFF_BLUE_SOUL))
						{
							RemoveAffect(AFFECT_SOUL);
						}
					}
					item->SetCount(item->GetCount()-1);
				}
				if (item->GetSubType() == BLUE_SOUL && item->GetSocket(1) > 0)
				{
					item->SetSocket(2, item->GetSocket(2) - 1);
					if (item->GetSocket(1) > 0)
						return float(item->GetValue(3)/10.0f);

					return 1.0f;
				}
			}
		}
	}

	return 1.0f;
}

bool CHARACTER::CheckSoulItem(bool bBlue)
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->GetType() == ITEM_SOUL)
		{
			if (bBlue)
			{
				if (item->GetSubType() == BLUE_SOUL)
				{
					if (item->GetSocket(1) > 0)
					{
						return true;
					}

					return false;
				}
			}
			else
			{
				if (item->GetSubType() == RED_SOUL)
				{
					if (item->GetSocket(1) > 0)
					{
						return true;
					}

					return false;
				}
			}
		}
	}

	return false;
}

bool CHARACTER::CheckPotionItem(int affectID)
{
	for (int i = 0; i < GetExtendInvenMax(); ++i)
	{
		LPITEM item = GetInventoryItem(i);
		if (!item)
			continue;
		if (item->GetValue(1) == affectID)
		{
			if (item->GetSocket(1) == 1)
				return true;

			return false;
		}
	}

	return false;
}

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
bool CHARACTER::DoRefine_Rarity(LPITEM item, LPITEM second_item, LPITEM third_item)
{
	if (item->GetType() == ITEM_WEAPON)
	{
		if (!CanHandleItem(true))
		{
			return false;
		}
		
		if (item->IsSealed())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_SEALED_CANNOT_MODIFY"));
			return false;
		}
		
		if (second_item->GetVnum() != 81705 && second_item->GetVnum() != 81706 && second_item->GetVnum() != 81707 && second_item->GetVnum() != 81708 && second_item->GetVnum() != 81709 && second_item->GetVnum() != 81710 && second_item->GetVnum() != 81711)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARITY_REFINE_UNKNOWN_ITEM_SECOND_ITEM"));
			return false;
		}
		
		if (third_item != nullptr)
		{
			if (third_item->GetVnum() != 81712)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARITY_REFINE_UNKNOWN_ITEM_THIRD_ITEM"));
				return false;
			}
		}
		
		if (item->GetRarePoints() >= ITEM_RARITY_MAX)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARITY_REFINE_ITEM_MAX_RARITY"));
			return false;
		}
		
		DWORD itemVnum = item->GetVnum();
		DWORD itemEvo = item->GetRareLevel();
		DWORD artanEvo = 0;
		if (second_item->GetVnum() == 81705)
			artanEvo = 1;
		else if (second_item->GetVnum() == 81706)
			artanEvo = 3;
		else if (second_item->GetVnum() == 81707)
			artanEvo = 5;
		else if (second_item->GetVnum() == 81708)
			artanEvo = 7;
		else if (second_item->GetVnum() == 81709)
			artanEvo = 9;
		else if (second_item->GetVnum() == 81710)
			artanEvo = 12;
		else if (second_item->GetVnum() == 81711)
			artanEvo = 15;
		
		if (item->GetRarePoints() + artanEvo >= ITEM_RARITY_MAX)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("RARITY_REFINE_ITEM_MAX_RARITY"));
			return false;
		}
		
		int rrPercent = 35;
		
		if (third_item != nullptr)
			rrPercent = 100;

		if (GetGold() < 25000000)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°³·®À» ÇÏ±â À§ÇÑ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
			return false;
		}

		int prob = number(1, 100);

		if (prob <= rrPercent)
		{
			// ¼º°ø! ¸ðµç ¾ÆÀÌÅÛÀÌ »ç¶óÁö°í, °°Àº ¼Ó¼ºÀÇ ´Ù¸¥ ¾ÆÀÌÅÛ È¹µæ
			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(itemVnum, 1, 0, false);

			if (pkNewItem)
			{
				ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
				if ((pkNewItem->GetRarePoints() + artanEvo) <= ITEM_RARITY_MAX)
					pkNewItem->UpdateRarePoints(artanEvo);
				LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

				UINT bCell = item->GetCell();

				DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -25000000);
				item->SetCount(item->GetCount()-1);
				second_item->SetCount(second_item->GetCount()-1);
				EffectPacket(SE_FR_SUCCESS);
				pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
				ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

				pkNewItem->AttrLog();
				PayRefineFee(25000000);
				ChatPacket(CHAT_TYPE_COMMAND, "RarityRefine_Success");
			}
		}
		else
		{
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -25000000);
			item->AttrLog();
			if (item->GetRarePoints() >= 50)
				item->SetCount(item->GetCount()-1);
			else
			{
				if ((item->GetRarePoints() - artanEvo) <= 0)
					item->NegativeRarePoints(artanEvo);
			}
			second_item->SetCount(second_item->GetCount()-1);
			EffectPacket(SE_FR_FAIL);
			ChatPacket(CHAT_TYPE_COMMAND, "RarityRefine_Fail");
			PayRefineFee(25000000);
		}
		if (third_item)
			ITEM_MANAGER::instance().RemoveItem(third_item, "REMOVE (REFINE SUCCESS)");
	}

	return true;
}
#endif