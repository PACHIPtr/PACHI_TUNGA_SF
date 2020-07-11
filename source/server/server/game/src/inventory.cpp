#include "stdafx.h"

#include "config.h"
#include "char.h"
#include "item.h"
#include "questmanager.h"
#include "item_manager.h"
#include "log.h"
#include "inventory.h"

LPITEM CInventory::FindSameItem(LPITEM item, int fromPos, int untilPos, BYTE bWindowType) const
{
	for (int i = fromPos; i < untilPos; ++i)
	{
		LPITEM cItem = GetItemAtPos(i, bWindowType);
		if (!cItem)
			continue;

		if (cItem->GetVnum() == item->GetVnum())
		{
			//Vnum matches, verify sockets
			bool matched = true;
			for (int j = 0; j < ITEM_SOCKET_MAX_NUM && matched; ++j)
				matched = cItem->GetSocket(j) == item->GetSocket(j);

			if (matched)
				return cItem;
		}
	}

	return nullptr; // (We got here, nothing found)
}

void CInventory::TryStack(int fromInventoryPos, int untilInventoryPos, LPITEM item, LPITEM &match, short &bCount, BYTE bWindowType)
{
	if (!item || !bCount)
		return;

	//Keep trying to stack until we run out of things to stack or places to stack onto
	while (bCount > 0 && ((match = FindSameItem(item, fromInventoryPos, untilInventoryPos, bWindowType))))
	{
		fromInventoryPos = match->GetCell() + 1;
		BYTE mCount = (BYTE)match->GetCount();
		BYTE newCount = MIN(ITEM_MAX_COUNT, mCount + bCount);

		if (newCount > mCount)
		{
			bCount -= newCount - mCount;
			match->SetCount(newCount);
		}
	}
}

CInventory::EStatus CInventory::Add(LPITEM &item)
{
	if (!item)
		return EStatus::ERROR_UNDEFINED;

	// If the item is stackable, try to stack it
	if (item->GetFlag() & ITEM_FLAG_STACKABLE)
	{
		short bCount = (short)item->GetCount();
		LPITEM match;

		TryStack(0, 225, item, match, bCount, SKILL_BOOK_INVENTORY);
		TryStack(0, 225, item, match, bCount, UPGRADE_ITEMS_INVENTORY);
		TryStack(0, 225, item, match, bCount, STONE_ITEMS_INVENTORY);
		TryStack(0, 225, item, match, bCount, CHEST_ITEMS_INVENTORY);
		TryStack(0, 225, item, match, bCount, ATTR_ITEMS_INVENTORY);
		TryStack(0, 225, item, match, bCount, FLOWER_ITEMS_INVENTORY);
		
		if (bCount > 0)
		{
			//Try to stack on belt and on inventory afterwards
			TryStack(0, m_owner->GetExtendInvenMax(), item, match, bCount, INVENTORY);
		}
		
		if (bCount == 0)
		{
			// We're left with nothing more, we stacked the item in its
			// entirety. Delete the old one.
			M2_DESTROY_ITEM(item);
			item = nullptr;
			OUT item = match; // For all purposes, the item is now the last matched one
			return EStatus::SUCCESS;
		}
		else
		{
			// Otherwise, we need to update the old item count
			// (maybe we managed to stack a part of it)
			item->SetCount(bCount);
		}
	}

	// Either we couldn't stack it, or we only stacked a portion.
	// We need to place the item in the inventory!
	int iEmptyCell;
	if (item->IsDragonSoul())
	{
		if ((iEmptyCell = m_owner->GetEmptyDragonSoulInventory(item)) == -1)
		{
			sys_log(0, "No empty ds inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}
	}
	else if (item->IsSkillBook())
	{
		if ((iEmptyCell = m_owner->GetEmptySkillBookInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty book inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}			
	}
	else if (item->IsStone())
	{
		if ((iEmptyCell = m_owner->GetEmptyStoneInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty stone inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}		
	}
	else if (item->IsUpgradeItem())
	{
		if ((iEmptyCell = m_owner->GetEmptyUpgradeItemsInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty upgrade inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}		
	}
	else if (item->IsChest())
	{
		if ((iEmptyCell = m_owner->GetEmptyChestInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty chest inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}		
	}
	else if (item->IsAttr())
	{
		if ((iEmptyCell = m_owner->GetEmptyAttrInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty attr inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}
	}
	else if (item->IsFlower())
	{
		if ((iEmptyCell = m_owner->GetEmptyFlowerInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty flower inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}
	}
	else
	{
		if ((iEmptyCell = m_owner->GetEmptyInventory(item->GetSize())) == -1)
		{
			sys_log(0, "No empty inventory p#%lu size %d i#%lu", m_owner->GetPlayerID(), item->GetSize(), item->GetID());
			return EStatus::ERROR_NO_SPACE;
		}
	}

	// If the item is on the ground, we will remove it. Else, this won't do anything
	item->RemoveFromGround();

	// Bind the item to the character to the cell
	if (item->IsDragonSoul())
		item->AddToCharacter(m_owner, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
	else if (item->IsSkillBook())
		item->AddToCharacter(m_owner, TItemPos(SKILL_BOOK_INVENTORY, iEmptyCell));
	else if (item->IsStone())
		item->AddToCharacter(m_owner, TItemPos(STONE_ITEMS_INVENTORY, iEmptyCell));
	else if (item->IsUpgradeItem())
		item->AddToCharacter(m_owner, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyCell));
	else if (item->IsChest())
		item->AddToCharacter(m_owner, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyCell));
	else if (item->IsAttr())
		item->AddToCharacter(m_owner, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyCell));
	else if (item->IsFlower())
		item->AddToCharacter(m_owner, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyCell));
	else
		item->AddToCharacter(m_owner, TItemPos(INVENTORY, iEmptyCell));

	return EStatus::SUCCESS;
};

LPITEM CInventory::GetItemAtPos(const WORD& wCell, BYTE bWindowType) const
{
	if (bWindowType == INVENTORY)
	{
		if (wCell < m_owner->GetExtendInvenMax() || (wCell >= BELT_INVENTORY_SLOT_START && wCell < BELT_INVENTORY_SLOT_END))
			return m_owner->GetItem(TItemPos(INVENTORY, wCell));
	}
	else if (bWindowType == DRAGON_SOUL_INVENTORY)
	{
		if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
			return m_owner->GetItem(TItemPos(DRAGON_SOUL_INVENTORY, wCell));
	}
	else if (bWindowType == UPGRADE_ITEMS_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(UPGRADE_ITEMS_INVENTORY, wCell));
	}
	else if (bWindowType == SKILL_BOOK_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(SKILL_BOOK_INVENTORY, wCell));
	}
	else if (bWindowType == STONE_ITEMS_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(STONE_ITEMS_INVENTORY, wCell));
	}
	else if (bWindowType == CHEST_ITEMS_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(CHEST_ITEMS_INVENTORY, wCell));
	}
	else if (bWindowType == ATTR_ITEMS_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(ATTR_ITEMS_INVENTORY, wCell));
	}
	else if (bWindowType == FLOWER_ITEMS_INVENTORY)
	{
		if (wCell < 225)
			return m_owner->GetItem(TItemPos(FLOWER_ITEMS_INVENTORY, wCell));
	}
	return nullptr;
}