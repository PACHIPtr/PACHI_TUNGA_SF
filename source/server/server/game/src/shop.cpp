#include "stdafx.h"
#include "../../libgame/libgame/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "shop.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "questmanager.h"
#include "mob_manager.h"
#include "locale_service.h"
#include "db.h"
#include "desc_client.h"
#include "p2p.h"
#include "../../common/service.h"

extern int taxes;
extern bool trade_effect;
extern int trade_effect_shop_threshold;

CShop::CShop()
	: m_dwVnum(0), m_dwNPCVnum(0), m_pkPC(nullptr)
	#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	,m_IsLimitedItemShop(false)
	#endif
{
	m_pGrid = M2_NEW CGrid(5, 9);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SetLocked(false);
#endif
}

CShop::~CShop()
{
	TPacketGCShop pack;

	pack.header = HEADER_GC_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	GuestMapType::iterator it;

	it = m_map_guest.begin();

	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;
		ch->SetShop(nullptr);
		++it;
	}

	M2_DELETE(m_pGrid);
	
#ifdef ENABLE_SOLD_ITEM_SYSTEM
	m_SoldItems.clear();
#endif
#ifdef ENABLE_SHOP_SEEN
	bakanlar.clear();
#endif
}

bool CShop::Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable* pTable)
{
	sys_log(0, "SHOP #%d (Shopkeeper %d)", dwVnum, dwNPCVnum);

	m_dwVnum = dwVnum;
	m_dwNPCVnum = dwNPCVnum;
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (dwNPCVnum == 20451)
		m_IsLimitedItemShop = true;
#endif

	BYTE bItemCount;

	for (bItemCount = 0; bItemCount < SHOP_HOST_ITEM_MAX_NUM; ++bItemCount)
		if (0 == (pTable + bItemCount)->vnum)
			break;

	SetShopItems(pTable, bItemCount);
	return true;
}

void CShop::SetShopItemsPrivateShop(TShopItemTable * pTable, BYTE bItemCount)
{
	if (bItemCount > SHOP_HOST_ITEM_MAX_NUM)
		return;

	m_pGrid->Clear();

	m_itemVector.resize(SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(SHOP_ITEM) * m_itemVector.size());

	for (int i = 0; i < bItemCount; ++i)
	{
		LPITEM pkItem = nullptr;
		const TItemTable* item_table;

		if (m_pkPC)
		{
			pkItem = m_pkPC->GetItem(pTable->pos);

			if (!pkItem)
			{
				sys_err("cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, m_pkPC->GetName());
				continue;
			}

			item_table = pkItem->GetProto();
		}
		else
		{
			if (!pTable->vnum)
				continue;

			item_table = ITEM_MANAGER::instance().GetTable(pTable->vnum);
		}

		if (!item_table)
		{
			sys_err("Shop: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		int iPos;

		if (IsPCShop())
		{
			sys_log(0, "MyShop: use position %d", pTable->display_pos);
			iPos = pTable->display_pos;
		}
		else
			iPos = m_pGrid->FindBlank(1, item_table->bSize);

		if (iPos < 0)
		{
			sys_err("not enough shop window");
			continue;
		}

		if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
		{
			if (IsPCShop())
			{
				sys_err("not empty position for pc shop %s[%d]", m_pkPC->GetName(), m_pkPC->GetPlayerID());
			}
			else
			{
				sys_err("not empty position for npc shop");
			}
			continue;
		}

		m_pGrid->Put(iPos, 1, item_table->bSize);

		SHOP_ITEM& item = m_itemVector[iPos];

		item.pkItem = pkItem;
		item.itemid = 0;

		if (item.pkItem)
		{
			item.vnum = pkItem->GetVnum();
			item.count = pkItem->GetCount(); // PC ¼¥ÀÇ °æ¿ì ¾ÆÀÌÅÛ °³¼ö´Â ÁøÂ¥ ¾ÆÀÌÅÛÀÇ °³¼ö¿©¾ß ÇÑ´Ù.
			item.price = pTable->price; // °¡°Ýµµ »ç¿ëÀÚ°¡ Á¤ÇÑ´ë·Î..
#ifdef ENABLE_CHEQUE_SYSTEM
			item.price_cheque = pTable->price_cheque;
#endif
			item.itemid = pkItem->GetID();
		}
		else
		{
			item.vnum = pTable->vnum;
			item.count = pTable->count;
			item.price = pTable->price;
		}

		char name[36];
		snprintf(name, sizeof(name), "%-20s(#%-5d) (x %d)", item_table->szName, (int)item.vnum, item.count);

		sys_log(0, "SHOP_ITEM: %-36s PRICE %-5d", name, item.price);
		/*if (IsPCShop())
			InsertInMysql(m_pkPC, pkItem, iPos, item.price);*/
		++pTable;
	}
}

void CShop::SetShopItems(TShopItemTable * pTable, BYTE bItemCount)
{
	if (bItemCount > SHOP_HOST_ITEM_MAX_NUM)
		return;

	m_pGrid->Clear();

	m_itemVector.resize(SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(SHOP_ITEM) * m_itemVector.size());

	for (int i = 0; i < bItemCount; ++i)
	{
		LPITEM pkItem = nullptr;
		const TItemTable* item_table;

		if (m_pkPC)
		{
			pkItem = m_pkPC->GetItem(pTable->pos);

			if (!pkItem)
			{
				sys_err("cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, m_pkPC->GetName());
				continue;
			}

			item_table = pkItem->GetProto();
		}
		else
		{
			if (!pTable->vnum)
				continue;

			item_table = ITEM_MANAGER::instance().GetTable(pTable->vnum);
		}

		if (!item_table)
		{
			sys_err("Shop: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		int iPos;

		if (IsPCShop())
		{
			sys_log(0, "MyShop: use position %d", pTable->display_pos);
			iPos = pTable->display_pos;
		}
		else
			iPos = m_pGrid->FindBlank(1, item_table->bSize);

		if (iPos < 0)
		{
			sys_err("not enough shop window");
			continue;
		}

		if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
		{
			if (IsPCShop())
			{
				sys_err("not empty position for pc shop %s[%d]", m_pkPC->GetName(), m_pkPC->GetPlayerID());
			}
			else
			{
				sys_err("not empty position for npc shop");
			}
			continue;
		}

		m_pGrid->Put(iPos, 1, item_table->bSize);

		SHOP_ITEM& item = m_itemVector[iPos];

		item.pkItem = pkItem;
		item.itemid = 0;

		if (item.pkItem)
		{
			item.vnum = pkItem->GetVnum();
			item.count = pkItem->GetCount(); // PC ¼¥ÀÇ °æ¿ì ¾ÆÀÌÅÛ °³¼ö´Â ÁøÂ¥ ¾ÆÀÌÅÛÀÇ °³¼ö¿©¾ß ÇÑ´Ù.
			item.price = pTable->price; // °¡°Ýµµ »ç¿ëÀÚ°¡ Á¤ÇÑ´ë·Î..
#ifdef ENABLE_CHEQUE_SYSTEM
			item.price_cheque = pTable->price_cheque;
#endif
			item.itemid = pkItem->GetID();
		}
		else
		{
			item.vnum = pTable->vnum;
			item.count = pTable->count;
			if (IS_SET(item_table->dwFlags, ITEM_FLAG_COUNT_PER_1GOLD))
			{
				if (item_table->dwGold == 0)
					item.price = item.count;
				else
					item.price = item.count / item_table->dwGold;
			}
			else
				item.price = item_table->dwGold * item.count;
#ifdef ENABLE_BUY_WITH_ITEM
			item.witemVnum = pTable->witemVnum;
#endif
		}

		char name[36];
		snprintf(name, sizeof(name), "%-20s(#%-5d) (x %d)", item_table->szName, (int)item.vnum, item.count);

		sys_log(0, "SHOP_ITEM: %-36s PRICE %-5d", name, item.price);
		++pTable;
	}
}

int CShop::Buy(LPCHARACTER ch, BYTE pos, bool isSearchBuy /*= false*/)
{
	if (IsLocked())
		return SHOP_SUBHEADER_GC_END;

	if (pos >= m_itemVector.size())
	{
		sys_log(0, "Shop::Buy : invalid position %d : %s", pos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	sys_log(0, "Shop::Buy : name %s pos %d", ch->GetName(), pos);

	GuestMapType::iterator it = m_map_guest.find(ch);

	if (!isSearchBuy)
	{
		if (it == m_map_guest.end())
			return SHOP_SUBHEADER_GC_END;
	}

	SHOP_ITEM& r_item = m_itemVector[pos];

	if (r_item.price < 0)
	{
		LogManager::instance().HackLog("SHOP_BUY_GOLD_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}

	if (r_item.price_cheque < 0)
	{
		LogManager::instance().HackLog("SHOP_BUY_GOLD_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
	}

	//LPITEM pkSelectedItem = ITEM_MANAGER::instance().Find(r_item.itemid);
	
#ifdef FULL_YANG
	long long dwPrice = r_item.price;
#else
	DWORD dwPrice = r_item.price;
#endif
	DWORD dwPriceCheque = r_item.price_cheque;
#ifdef ENABLE_BUY_WITH_ITEM
	DWORD dwWItemVnum = r_item.witemVnum;
#endif

	if (dwWItemVnum > 0)
	{
		if (ch->CountSpecifyItem(dwWItemVnum) < dwPrice)
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM;
	}
	else
	{
		if (m_IsLimitedItemShop)
		{
			if (ch->CountZodiacItems(r_item.vnum) < (DWORD)r_item.count)
			{
				sys_log(1, "Shop::Buy : Not enough Zodiac Item : %s has %d, price %d", ch->GetName(), r_item.count, dwPrice);
				return SHOP_SUBHEADER_GC_NOT_ENOUGH_ZODIAC_ITEM;
			}
		}
		else
		{
			if (ch->GetGold() < (int)dwPrice)
			{
				sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %d", ch->GetName(), ch->GetGold(), dwPrice);
				return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
			}

#ifdef ENABLE_CHEQUE_SYSTEM
			if (ch->GetCheque() < (int)dwPriceCheque)
			{
				sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %d", ch->GetName(), ch->GetGold(), dwPrice);
				return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
			}
		}
#endif
	}

	LPITEM item;

	if (m_pkPC)
		item = r_item.pkItem;
	else {
		item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);
	}
	if (!item)
		return SHOP_SUBHEADER_GC_SOLD_OUT;

#ifdef ENABLE_AUTO_POTION_RENEWAL
	if (item->IsAutoPotionHP())
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = ch->GetInventoryItem(i);
			if (!item2)
				continue;
			if (item2->IsAutoPotionHP())
				return SHOP_SUBHEADER_GC_MAX_COUNT;
		}
	}
	
	if (item->IsAutoPotionSP())
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item2 = ch->GetInventoryItem(i);
			if (!item2)
				continue;
			if (item2->IsAutoPotionSP())
				return SHOP_SUBHEADER_GC_MAX_COUNT;
		}
	}
#endif

	if (IsPCShop() && m_pkPC->GetPrivShopOwner() == ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Kendi dukkanindan birsey satin alamazsin."));
		return SHOP_SUBHEADER_GC_OK;
	}

	int iEmptyPos;
	if (item->IsDragonSoul())
		iEmptyPos = ch->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsStone())
		iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsChest())
		iEmptyPos = ch->GetEmptyChestInventory(item->GetSize());
	else if (item->IsAttr())
		iEmptyPos = ch->GetEmptyAttrInventory(item->GetSize());
	else if (item->IsFlower())
		iEmptyPos = ch->GetEmptyFlowerInventory(item->GetSize());
#endif
	else
	{
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());
	}

	if (iEmptyPos < 0)
	{
		if (m_pkPC)
		{
			sys_log(1, "Shop::Buy at PC Shop : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
		else
		{
			sys_log(1, "Shop::Buy : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			M2_DESTROY_ITEM(item);
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
	}

	if (dwWItemVnum > 0)
		ch->RemoveSpecifyItem(dwWItemVnum, dwPrice);
	else
	{
		if (m_IsLimitedItemShop)
		{
			ch->SetZodiacItems(r_item.vnum, 0);
			ch->SetPurchaseZodiacItems(r_item.vnum, r_item.count);
			ch->PointChange(POINT_GOLD, -dwPrice, false);
		}
		else
		{
			if (dwPrice > 0)
				ch->PointChange(POINT_GOLD, -dwPrice, false);
#ifdef ENABLE_CHEQUE_SYSTEM
			if (dwPriceCheque > 0)
				ch->PointChange(POINT_CHEQUE, -dwPriceCheque, false);
#endif
		}
	}

	// ¡¾?AO ¢©?¢©??? : ??¡¾Y A¢§?o
	if (m_pkPC)
	{
		if (!m_pkPC->IsPrivShop())
		{
			m_pkPC->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 1000);
		}

		char buf[512];
		DWORD mpid = (m_pkPC->IsPrivShop() ? m_pkPC->GetPrivShopOwner() : m_pkPC->GetPlayerID());
		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			snprintf(buf, sizeof(buf), "%s FROM: %u TO: %u PRICE: %u", item->GetName(), ch->GetPlayerID(), mpid, dwPrice);
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), SHOP_BUY, buf);
			LogManager::instance().GoldBarLog(mpid, item->GetID(), SHOP_SELL, buf);
		}
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

		if (!item)
			return SHOP_SUBHEADER_GC_SOLD_OUT;

		if (pkNewItem)
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
			{
				pkNewItem->SetSocket(i, item->GetSocket(i));
			}
			// ¸ÅÁ÷ ¾ÆÀÌÅÛ ¼³Á¤
			item->CopyAttributeTo(pkNewItem);

			if (pkNewItem->IsDragonSoul())
				pkNewItem->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
			else if (pkNewItem->IsSkillBook())
				pkNewItem->AddToCharacter(ch, TItemPos(SKILL_BOOK_INVENTORY, iEmptyPos));
			else if (pkNewItem->IsStone())
				pkNewItem->AddToCharacter(ch, TItemPos(STONE_ITEMS_INVENTORY, iEmptyPos));
			else if (pkNewItem->IsUpgradeItem())
				pkNewItem->AddToCharacter(ch, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyPos));
			else if (pkNewItem->IsChest())
				pkNewItem->AddToCharacter(ch, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyPos));
			else if (pkNewItem->IsAttr())
				pkNewItem->AddToCharacter(ch, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyPos));
			else if (pkNewItem->IsFlower())
				pkNewItem->AddToCharacter(ch, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyPos));
#endif
			else
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			pkNewItem->SetTransmutation(item->GetTransmutation());
			if (pkNewItem->GetVnum() == 55002)
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item->GetID(), pkNewItem->GetID()));
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			std::string szBuy("SHOP_BUY");
			std::string szSell("SHOP_SELL");
			if (m_pkPC->IsPrivShop())
			{
				std::string temp("OFFLINE_");
				szBuy = temp + szBuy;
				szSell = temp + szSell;
			}
#ifdef FULL_YANG
			snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", pkNewItem->GetName(), mpid, m_pkPC->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(ch, pkNewItem, szBuy.c_str(), buf);
			snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", pkNewItem->GetName(), ch->GetPlayerID(), ch->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(m_pkPC, pkNewItem, szSell.c_str(), buf);
#else
			snprintf(buf, sizeof(buf), "%s %u(%s) %u %u", pkNewItem->GetName(), mpid, m_pkPC->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(ch, pkNewItem, szBuy.c_str(), buf);
			snprintf(buf, sizeof(buf), "%s %u(%s) %u %u", pkNewItem->GetName(), ch->GetPlayerID(), ch->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(m_pkPC, pkNewItem, szSell.c_str(), buf);
#endif

			r_item.pkItem = nullptr;
			
#ifdef ENABLE_SOLD_ITEM_SYSTEM
			SShopSoldData lpSoldData;
			lpSoldData.lpShop = this;
			lpSoldData.vnum = pkNewItem->GetVnum();
			lpSoldData.Price = dwPrice;
			lpSoldData.price_cheque = dwPriceCheque;
			lpSoldData.Count = pkNewItem->GetCount();
#ifdef ENABLE_CHANGELOOK_SYSTEM
			lpSoldData.Transmutation = pkNewItem->GetTransmutation();
#endif
			thecore_memcpy(lpSoldData.alSockets, pkNewItem->GetSockets(), sizeof(lpSoldData.alSockets));
			thecore_memcpy(lpSoldData.aAttr, pkNewItem->GetAttributes(), sizeof(lpSoldData.aAttr));
			lpSoldData.newOwner = ch->GetName();
			m_SoldItems[pos] = lpSoldData;
#endif
			
			BroadcastUpdateItem(pos);
			if (m_pkPC->IsPrivShop())
			{
				LPCHARACTER owner = CHARACTER_MANAGER::instance().FindByPID(m_pkPC->GetPrivShopOwner());
				if (dwPrice > 0)
				{
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE player.player_shop SET gold=gold + '%u' WHERE id='%d';", dwPrice, m_pkPC->GetPrivShop()));
				}
#ifdef ENABLE_CHEQUE_SYSTEM
				if (dwPriceCheque > 0)
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE player.player_shop SET won=won + '%u' WHERE id='%d';", dwPriceCheque, m_pkPC->GetPrivShop()));
				
#endif

				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM player_shop_items WHERE shop_id=%d and id = %d", m_pkPC->GetPrivShop(), r_item.itemid));

				if (owner)
				{
					owner->LoadPrivShops();
					if (owner->GetGiftPages() > 0)
						owner->ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", owner->GetGiftPages());
				}
			}
			else
			{
				if (dwPrice > 0)
					m_pkPC->PointChange(POINT_GOLD, dwPrice, false);
#ifdef ENABLE_CHEQUE_SYSTEM
				if (dwPriceCheque > 0)
					m_pkPC->PointChange(POINT_CHEQUE, dwPriceCheque, false);
#endif
			}
			item = pkNewItem;
		}
		else {
			return SHOP_SUBHEADER_GC_INVALID_POS;
		}
	}
	else
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			item->AddToCharacter(ch, TItemPos(SKILL_BOOK_INVENTORY, iEmptyPos));
		else if (item->IsStone())
			item->AddToCharacter(ch, TItemPos(STONE_ITEMS_INVENTORY, iEmptyPos));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(ch, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyPos));
		else if (item->IsChest())
			item->AddToCharacter(ch, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyPos));
		else if (item->IsAttr())
			item->AddToCharacter(ch, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyPos));
		else if (item->IsFlower())
			item->AddToCharacter(ch, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyPos));
#endif
		else
			item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(0) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = ch->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionHP())
				{
					int addvalue = item->GetValue(0) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d HP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
		else if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(1) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = ch->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionSP())
				{
					int addvalue = item->GetValue(1) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d SP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
#endif
		
		ITEM_MANAGER::instance().FlushDelayedSave(item);
		LogManager::instance().ItemLog(ch, item, "BUY", item->GetName());
		
		if (m_IsLimitedItemShop)
		{
			BroadcastUpdateItemCh(pos, ch);
		}

		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), PERSONAL_SHOP_BUY, "");
		}
	}

	if (item)
		sys_log(0, "SHOP: BUY: name %s %s(x %d):%u price %u", ch->GetName(), item->GetName(), item->GetCount(), item->GetID(), dwPrice);
	ch->Save();
#ifdef ENABLE_OFFLINESHOP_MESSAGE_SYSTEM
	if (m_pkPC && m_pkPC->IsPrivShop())
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindByPID(m_pkPC->GetPrivShopOwner());

		if (tch && item)
		{
			char msg[CHAT_MAX_LEN + 1];
			snprintf(msg, sizeof(msg), LC_TEXT("Your item %s sold, buyer is: %s."), item->GetName(), ch->GetName());

			LPDESC pkVictimDesc = tch->GetDesc();
			
			if (pkVictimDesc)
			{
				TPacketGCWhisper pack;

				int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, "[Dükkan]", sizeof(pack.szNameFrom));

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg, len);

				pkVictimDesc->Packet(buf.read_peek(), buf.size());
			}

			//tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Market Tezgâhýndan nesne satýldý: %s."), item->GetName());
		}
		else if (item)
		{
			// P2P_FIND

			TPacketGGOfflineShopMessage p;
			p.bHeader = HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE;
			p.dwTargetPID = m_pkPC->GetPrivShopOwner();
			strlcpy(p.szItemName, item->GetName(), sizeof(p.szItemName));
			strlcpy(p.szName, ch->GetName(), sizeof(p.szName));
			P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGOfflineShopMessage));
		}
	}
#endif
	if(trade_effect)
	{
		if (m_pkPC) {
			for (int i = 1; i < r_item.price+1; i=i+trade_effect_shop_threshold)
				ch->CreateFly(FLY_SP_SMALL,m_pkPC); // 1 Fly pro 100kk , sonst laggt es O:
			m_pkPC->CreateFly(FLY_SP_SMALL,ch); // 1 Item
			m_pkPC->CreateFly(FLY_SP_SMALL,ch); // Allgemeines anzeigen
		}
	}
#if defined(SHOP_AUTO_CLOSE) && defined(ENABLE_OFFLINE_SHOP_SYSTEM)
	if (m_pkPC && m_pkPC->IsPrivShop())
	{
		if (GetItemCount() <= 0)
			m_pkPC->DeleteMyShop();
	}
#endif
	return (SHOP_SUBHEADER_GC_OK);
}

void CShop::RemoveGuests(LPCHARACTER except)
{
	GuestMapType::iterator it = m_map_guest.begin();
	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;
		if (except && ch == except)
		{
			it++;
			continue;
		}
		if (ch)
		{
			ch->SetShop(nullptr);

			TPacketGCShop pack;

			pack.header = HEADER_GC_SHOP;
			pack.subheader = SHOP_SUBHEADER_GC_END;
			pack.size = sizeof(TPacketGCShop);

			ch->GetDesc()->Packet(&pack, sizeof(pack));
		}
		m_map_guest.erase(it++);
	}
}
void CShop::ClearItems()
{
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		m_itemVector[i].pkItem = nullptr;
		BroadcastUpdateItem(i);
	}
}
int CShop::GetItemCount()
{
	int count = 0;
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if (m_itemVector[i].pkItem)
			count++;
	}
	return count;
}
bool CShop::GetItems()
{
	if (!m_pkPC)
		return false;
	if (!m_pkPC)
		return false;
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if (!m_itemVector[i].pkItem)
			continue;
		char query[8192];

		sprintf(query, "INSERT INTO player_gift SET \
		owner_id=%d,vnum=%d,count=%d,transmutation=%d,reason='%s',`from`=replace(\"%s\",' ','_'),status='WAIT',date_add=NOW()",
			m_pkPC->GetPrivShopOwner(), m_itemVector[i].vnum, m_itemVector[i].count, m_itemVector[i].pkItem->GetTransmutation(), LC_TEXT("SHOP_REASON"), m_pkPC->GetName());

		for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
		{
			sprintf(query, "%s, socket%d='%ld'", query, s, m_itemVector[i].pkItem->GetSocket(s));
		}
		for (int ia = 0; ia < ITEM_ATTRIBUTE_MAX_NUM; ia++)
		{
			const TPlayerItemAttribute& attr = m_itemVector[i].pkItem->GetAttribute(ia);
			if (ia < 7)
			{
				sprintf(query, "%s, attrtype%d='%u'", query, ia, attr.bType);
				sprintf(query, "%s, attrvalue%d='%d'", query, ia, attr.sValue);
			}
			else
			{
				sprintf(query, "%s, applytype%d='%u'", query, ia - 7, attr.bType);
				sprintf(query, "%s, applyvalue%d='%d'", query, ia - 7, attr.sValue);
			}
		}
		std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery(query));

		std::unique_ptr<SQLMsg> psMsg(DBManager::instance().DirectQuery("delete from player_shop_items where id='%d'", m_itemVector[i].itemid));
		m_itemVector[i].pkItem->RemoveFromCharacter();
		m_itemVector[i].pkItem = nullptr;
		BroadcastUpdateItem(i);
	}

	return true;
}
void CShop::SetPrivShopItems(std::vector<TShopItemTable*> map_shop)
{
	m_pGrid->Clear();

	m_itemVector.resize(SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(SHOP_ITEM) * m_itemVector.size());

	for (DWORD count = 0; count < map_shop.size(); count++)
	{
		TShopItemTable* pTable = map_shop[count];
		LPITEM pkItem = nullptr;
		const TItemTable* item_table;

		if (!m_pkPC)
			return;
		pkItem = m_pkPC->GetItem(pTable->pos);

		if (!pkItem)
		{
			sys_err("cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, m_pkPC->GetName());
			continue;
		}

		item_table = pkItem->GetProto();

		if (!item_table)
		{
			sys_err("Shop: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		WORD iPos = pTable->display_pos;
		sys_log(0, "MyShop: use position %d", iPos);

		if (iPos < 0)
		{
			sys_err("not enough shop window");
			continue;
		}

		if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
		{
			sys_err("not empty position for pc shop %s[%d] fixing", m_pkPC->GetName(), m_pkPC->GetPlayerID());
			iPos = m_pGrid->FindBlank(1, item_table->bSize);
			if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
			{
				sys_err("not empty position for pc shop %s[%d]", m_pkPC->GetName(), m_pkPC->GetPlayerID());
				continue;
			}
		}

		m_pGrid->Put(iPos, 1, item_table->bSize);

		SHOP_ITEM& item = m_itemVector[iPos];

		item.pkItem = pkItem;
		item.itemid = 0;

		if (item.pkItem)
		{
			item.pos = iPos;
			item.vnum = pkItem->GetVnum();
			item.count = pkItem->GetCount(); // PC ?? ?? ??? ??? ?? ???? ???? ??.
			item.price = pTable->price; // ??? ???? ????..
#ifdef ENABLE_CHEQUE_SYSTEM
			item.price_cheque = pTable->price_cheque;
#endif
			item.itemid = pkItem->GetRealID();
		}

		char name[36];
		snprintf(name, sizeof(name), "%-20s(#%-5d) (x %d)", item_table->szName, (int)item.vnum, item.count);

		sys_log(0, "PRIV_SHOP_ITEM: %-36s PRICE %-5d", name, item.price);
		//InsertInMysql(m_pkPC, pkItem, iPos, item.price);
	}
}

bool CShop::AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire)
{
	if (!ch)
		return false;

	if (ch->GetExchange())
		return false;

	if (ch->GetShop())
		return false;

	ch->SetShop(this);

	m_map_guest.insert(GuestMapType::value_type(ch, bOtherEmpire));

	TPacketGCShop pack;

	pack.header = HEADER_GC_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_START;

	TPacketGCShopStart pack2;

	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = owner_vid;
	
#ifdef ENABLE_SHOP_SEEN
	LPCHARACTER owner = CHARACTER_MANAGER::instance().Find(owner_vid);
	if (owner)
	{
		if (ch != owner && owner->IsPrivShop())
		{
			/*int pid = ch->GetPlayerID();
			if (find(bakanlar.begin(), bakanlar.end(), pid) == bakanlar.end())
			{
				bakanlar.push_back(pid);
			}*/

			bakanlar.push_back(bakanlar.size()+1);
			pack2.bakanlar = bakanlar.size();
		}
		else
		{
			pack2.bakanlar = bakanlar.size();
		}
	}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	pack2.islimiteditemshop = m_IsLimitedItemShop;
#endif


	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const SHOP_ITEM& item = m_itemVector[i];

		if (m_pkPC && !item.pkItem)
		{
#ifdef ENABLE_SOLD_ITEM_SYSTEM
			DWORD dwVnum = GetSoldItemVnum(i);
			if (dwVnum)
			{
				pack2.items[i].vnum = dwVnum;
				long lGetPriceFromTempData = GetSoldPrice(i, 1);
				pack2.items[i].price = lGetPriceFromTempData;
				pack2.items[i].price_cheque = GetSoldPrice(i, 2);
				pack2.items[i].count = GetSoldCount(i);
#ifdef ENABLE_CHANGELOOK_SYSTEM
				pack2.items[i].transmutation = GetSoldTransmutation(i);
#endif
				thecore_memcpy(pack2.items[i].alSockets, GetSoldSockets(i), sizeof(pack2.items[i].alSockets));
				thecore_memcpy(pack2.items[i].aAttr, GetSoldAttributes(i), sizeof(pack2.items[i].aAttr));
				pack2.items[i].isSold = 1;
				strlcpy(pack2.items[i].newOwner, GetSoldNewOwner(i), sizeof(pack2.items[i].newOwner));
			}
			else
#endif
				continue;
		}
		else
		{
			pack2.items[i].vnum = item.vnum;
			pack2.items[i].price = item.price;
#ifdef ENABLE_CHEQUE_SYSTEM
			pack2.items[i].price_cheque = item.price_cheque;
#endif
			pack2.items[i].count = item.count;
#ifdef ENABLE_BUY_WITH_ITEM
			pack2.items[i].witemVnum = item.witemVnum;
#endif

			if (item.pkItem)
			{
				thecore_memcpy(pack2.items[i].alSockets, item.pkItem->GetSockets(), sizeof(pack2.items[i].alSockets));
				thecore_memcpy(pack2.items[i].aAttr, item.pkItem->GetAttributes(), sizeof(pack2.items[i].aAttr));
#ifdef ENABLE_CHANGELOOK_SYSTEM
				pack2.items[i].transmutation = item.pkItem->GetTransmutation();
#endif
			}
			else
			{
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
				pack2.items[i].getLimitedCount = item.count;
				pack2.items[i].getLimitedPurchaseCount = ch->PurchaseCountZodiacItems(item.vnum);
#endif
			}
		}
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
	ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCShopStart));
	return true;
}

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
void CShop::BroadcastUpdateItemCh(BYTE pos, LPCHARACTER ch)
{
	if (!ch || !ch->IsPC())
	{
		sys_err("CH::YOK!");
		return;
	}

	TPacketGCShop pack;
	TPacketGCShopUpdateItem pack2;

	TEMP_BUFFER	buf;

	pack.header		= HEADER_GC_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_UPDATE_ITEM;
	pack.size		= sizeof(pack) + sizeof(pack2);

	pack2.pos		= pos;

	if (m_pkPC && !m_itemVector[pos].pkItem)
	{
#ifdef ENABLE_SOLD_ITEM_SYSTEM
		DWORD dwVnum = GetSoldItemVnum(pos);
		if (dwVnum)
		{
			pack2.item.vnum = dwVnum;
			pack2.item.price = GetSoldPrice(pos, 1);
			pack2.item.price_cheque = GetSoldPrice(pos, 2);
			pack2.item.count = GetSoldCount(pos);
			pack2.item.transmutation = GetSoldTransmutation(pos);

			thecore_memcpy(pack2.item.alSockets, GetSoldSockets(pos), sizeof(pack2.item.alSockets));
			thecore_memcpy(pack2.item.aAttr, GetSoldAttributes(pos), sizeof(pack2.item.aAttr));
			pack2.item.isSold = 1;
			strlcpy(pack2.item.newOwner, GetSoldNewOwner(pos), sizeof(pack2.item.newOwner));
		}
		else
#endif
		{
			pack2.item.vnum = 0;
		}
	}
	else
	{
		pack2.item.vnum = m_itemVector[pos].vnum;
		if (m_itemVector[pos].pkItem)
		{
			thecore_memcpy(pack2.item.alSockets, m_itemVector[pos].pkItem->GetSockets(), sizeof(pack2.item.alSockets));
			thecore_memcpy(pack2.item.aAttr, m_itemVector[pos].pkItem->GetAttributes(), sizeof(pack2.item.aAttr));
		}
		else if (!m_pkPC && !IsPCShop())
		{
			memset(pack2.item.alSockets, 0, sizeof(pack2.item.alSockets));
			memset(pack2.item.aAttr, 0, sizeof(pack2.item.aAttr));

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
			pack2.item.getLimitedCount = m_itemVector[pos].count;
			pack2.item.getLimitedPurchaseCount = ch->PurchaseCountZodiacItems(m_itemVector[pos].vnum);
#endif
		}
	}

	pack2.item.price	= m_itemVector[pos].price;
	pack2.item.price_cheque = m_itemVector[pos].price_cheque;

	pack2.item.count	= m_itemVector[pos].count;
#ifdef ENABLE_BUY_WITH_ITEM
	pack2.item.witemVnum = m_itemVector[pos].witemVnum;
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	pack2.item.getLimitedCount = m_itemVector[pos].count;
	pack2.item.getLimitedPurchaseCount = ch->PurchaseCountZodiacItems(m_itemVector[pos].vnum);
#endif

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));

	Broadcast(buf.read_peek(), buf.size());
}
#endif

void CShop::RemoveGuest(LPCHARACTER ch)
{
	if (ch->GetShop() != this)
		return;

	m_map_guest.erase(ch);
	ch->SetShop(nullptr);

	TPacketGCShop pack;

	pack.header = HEADER_GC_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	ch->GetDesc()->Packet(&pack, sizeof(pack));
}

void CShop::Broadcast(const void* data, int bytes)
{
	sys_log(1, "Shop::Broadcast %p %d", data, bytes);

	GuestMapType::iterator it;

	it = m_map_guest.begin();

	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;

		if (ch && m_pkPC) {
			if (ch->GetMapIndex() != m_pkPC->GetMapIndex())
				continue;
		}

		if (ch && ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);

		++it;
	}
}

void CShop::BroadcastUpdateItem(BYTE pos)
{
	TPacketGCShop pack;
	TPacketGCShopUpdateItem pack2;

	TEMP_BUFFER	buf;

	pack.header = HEADER_GC_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM;
	pack.size = sizeof(pack) + sizeof(pack2);

	pack2.pos = pos;

	if (m_pkPC && !m_itemVector[pos].pkItem)
	{
#ifdef ENABLE_SOLD_ITEM_SYSTEM
		DWORD dwVnum = GetSoldItemVnum(pos);
		if (dwVnum)
		{
			pack2.item.vnum = dwVnum;
			pack2.item.price = GetSoldPrice(pos, 1);
			pack2.item.price_cheque = GetSoldPrice(pos, 2);
			pack2.item.count = GetSoldCount(pos);

#ifdef ENABLE_CHANGELOOK_SYSTEM
			pack2.item.transmutation = GetSoldTransmutation(pos);
#endif

			thecore_memcpy(pack2.item.alSockets, GetSoldSockets(pos), sizeof(pack2.item.alSockets));
			thecore_memcpy(pack2.item.aAttr, GetSoldAttributes(pos), sizeof(pack2.item.aAttr));
			pack2.item.isSold = 1;
			strlcpy(pack2.item.newOwner, GetSoldNewOwner(pos), sizeof(pack2.item.newOwner));
		}
		else
#endif
			pack2.item.vnum = 0;
	}
	else
	{
		pack2.item.vnum = m_itemVector[pos].vnum;
		if (m_itemVector[pos].pkItem)
		{
			thecore_memcpy(pack2.item.alSockets, m_itemVector[pos].pkItem->GetSockets(), sizeof(pack2.item.alSockets));
			thecore_memcpy(pack2.item.aAttr, m_itemVector[pos].pkItem->GetAttributes(), sizeof(pack2.item.aAttr));
		}
		else
		{
			memset(pack2.item.alSockets, 0, sizeof(pack2.item.alSockets));
			memset(pack2.item.aAttr, 0, sizeof(pack2.item.aAttr));
		}
	}

	pack2.item.price = m_itemVector[pos].price;
#ifdef ENABLE_CHEQUE_SYSTEM
	pack2.item.price_cheque = m_itemVector[pos].price_cheque;
#endif

	pack2.item.count = m_itemVector[pos].count;

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));

	Broadcast(buf.read_peek(), buf.size());
}

int CShop::GetNumberByVnum(DWORD dwVnum)
{
	int itemNumber = 0;

	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const SHOP_ITEM& item = m_itemVector[i];

		if (item.vnum == dwVnum)
		{
			itemNumber += item.count;
		}
	}

	return itemNumber;
}

bool CShop::IsSellingItem(DWORD itemID)
{
	bool isSelling = false;

	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if (m_itemVector[i].itemid == itemID)
		{
			isSelling = true;
			break;
		}
	}

	return isSelling;
}

void CShop::Refresh()
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(m_pkPC->GetPrivShopOwner());
	if (!ch)
		return;
	for (int i = 0; i < SHOP_HOST_ITEM_MAX_NUM; i++)
	{
		const SHOP_ITEM& item = m_itemVector[i];

		if (item.vnum == 0)
			continue;
		char attrs[1024] = { '\0' };
		char sockets[1024] = { '\0' };
		for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
			sprintf(sockets, "%s%ld%s", sockets, item.pkItem->GetSocket(x), (x < ITEM_SOCKET_MAX_NUM - 1 ? "|" : ""));
		//col--;

		for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
			sprintf(attrs, "%s%hhu,%hd%s", attrs, item.pkItem->GetAttributeType(x), item.pkItem->GetAttributeValue(x), (x < ITEM_ATTRIBUTE_MAX_NUM - 1 ? "|" : ""));

		ch->ChatPacket(CHAT_TYPE_COMMAND, "shop_item %s#%s#%s#%s#%s#%s#%s#%s#%s", item.pkItem->GetID(), item.pkItem->GetVnum(), item.count, i, item.price, item.price_cheque, sockets, attrs);
	}
}

#ifdef ENABLE_ADVANCED_RELOAD_SYSTEM
void CShop::RemoveAllGuests()
{
	if (m_map_guest.empty())
		return;
	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); it++)
	{
		LPCHARACTER ch = it->first;
		if (ch)
		{
			if (ch->GetDesc() && ch->GetShop() == this)
			{
				ch->SetShop(nullptr);

				TPacketGCShop pack;

				pack.header = HEADER_GC_SHOP;
				pack.subheader = SHOP_SUBHEADER_GC_END;
				pack.size = sizeof(TPacketGCShop);

				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
	}
	m_map_guest.clear();
}
#endif

#ifdef ENABLE_SOLD_ITEM_SYSTEM
DWORD CShop::GetSoldItemVnum(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return 0;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		if (it->second.vnum == 0)
			return 0;

		return it->second.vnum;
	}

	return 0;
}

long CShop::GetSoldPrice(BYTE pos, BYTE type)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return 0;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		if (type == 1 && it->second.Price == 0)
			return 0;
		if (type == 2 && it->second.price_cheque == 0)
			return 0;

		if (type == 2)
			return it->second.price_cheque;
		return it->second.Price;
	}

	return 0;
}

BYTE CShop::GetSoldCount(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return 0;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		if (it->second.Count == 0)
			return 0;

		return it->second.Count;
	}

	return 0;
}

#ifdef ENABLE_CHANGELOOK_SYSTEM
BYTE CShop::GetSoldTransmutation(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return 0;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		if (it->second.Transmutation == 0)
			return 0;

		return it->second.Transmutation;
	}

	return 0;
}
#endif

const long* CShop::GetSoldSockets(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return 0;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		return it->second.alSockets;
	}

	return nullptr;
}

const TPlayerItemAttribute* CShop::GetSoldAttributes(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return nullptr;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		return it->second.aAttr;
	}

	return nullptr;
}

const char* CShop::GetSoldNewOwner(BYTE pos)
{
	if (m_SoldItems.empty() || m_SoldItems.begin() == m_SoldItems.end())
		return nullptr;

	std::map<BYTE, SShopSoldData>::iterator it;
	for (it = m_SoldItems.begin(); it != m_SoldItems.end(); ++it)
	{
		if (it->first != pos)
			continue;

		if (it->second.lpShop != this)
			continue;

		if (it->second.newOwner.c_str() == nullptr)
			return nullptr;

		return it->second.newOwner.c_str();
	}

	return nullptr;
}
#endif