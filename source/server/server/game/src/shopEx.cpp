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
#include "desc_client.h"
#include "shopEx.h"
#include "group_text_parse_tree.h"
#include "unique_item.h"
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif

extern int taxes;

bool CShopEx::Create(DWORD dwVnum, DWORD dwNPCVnum)
{
	m_dwVnum = dwVnum;
	m_dwNPCVnum = dwNPCVnum;
	return true;
}

bool CShopEx::AddShopTable(TShopTableEx& shopTable)
{
	for (auto it = m_vec_shopTabs.begin(); it != m_vec_shopTabs.end(); it++)
	{
		const TShopTableEx& _shopTable = *it;
		if (0 != _shopTable.dwVnum && _shopTable.dwVnum == shopTable.dwVnum)
			return false;
		if (0 != _shopTable.dwNPCVnum && _shopTable.dwNPCVnum == shopTable.dwVnum)
			return false;
	}
	m_vec_shopTabs.push_back(shopTable);
	return true;
}

bool CShopEx::AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire)
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
	pack.subheader = SHOP_SUBHEADER_GC_START_EX;

	TPacketGCShopStartEx pack2;

	memset(&pack2, 0, sizeof(pack2));

	pack2.owner_vid = owner_vid;
	pack2.shop_tab_count = m_vec_shopTabs.size();
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	DWORD dwGetFirstDayHour = CCombatZoneManager::instance().GetFirstDayHour();
	int dwLastBuyTime = ch->GetQuestFlag(COMBAT_ZONE_FLAG_BUY_LAST_TIME);

	if (dwGetFirstDayHour > static_cast<DWORD>(dwLastBuyTime))
		ch->SetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS, 0);

	pack2.points = ch->GetRealCombatZonePoints();
	pack2.curLimit = ch->GetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS);
	pack2.maxLimit = COMBAT_ZONE_SHOP_MAX_LIMIT_POINTS;
#endif
	char temp[8096]; // √÷¥Î 1728 * 3
	char* buf = &temp[0];
	size_t size = 0;
	for (auto it = m_vec_shopTabs.begin(); it != m_vec_shopTabs.end(); it++)
	{
		const TShopTableEx& shop_tab = *it;
		TPacketGCShopStartEx::TSubPacketShopTab pack_tab;
		pack_tab.coin_type = shop_tab.coinType;
		memcpy(pack_tab.name, shop_tab.name.c_str(), SHOP_TAB_NAME_MAX);

		for (BYTE i = 0; i < SHOP_HOST_ITEM_MAX_NUM; i++)
		{
			pack_tab.items[i].vnum = shop_tab.items[i].vnum;
			pack_tab.items[i].count = shop_tab.items[i].count;
			switch (shop_tab.coinType)
			{
			case SHOP_COIN_TYPE_GOLD:
			case SHOP_COIN_TYPE_SECONDARY_COIN:
#ifdef ENABLE_EVENT_SYSTEM
			case SHOP_COIN_TYPE_YEAR_ROUND_COIN:
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
			case SHOP_COIN_TYPE_COMBAT_ZONE:
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
			case SHOP_COIN_TYPE_CHEQUE:
#endif
#ifdef ENABLE_GEM_SYSTEM
			case SHOP_COIN_TYPE_GAYA:
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
			case SHOP_COIN_TYPE_ZODIAC:
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
			case SHOP_COIN_TYPE_WORSHIP:
#endif
			case SHOP_COIN_TYPE_EXP:
			case SHOP_COIN_TYPE_DRAGON_COIN:
			case SHOP_COIN_TYPE_DRAGON_MARK:
				pack_tab.items[i].price = shop_tab.items[i].price;
				break;
			}
			memset(pack_tab.items[i].aAttr, 0, sizeof(pack_tab.items[i].aAttr));
			memset(pack_tab.items[i].alSockets, 0, sizeof(pack_tab.items[i].alSockets));
			pack_tab.items[i].isSold = 0;
			memset(pack_tab.items[i].newOwner, 0, sizeof(pack_tab.items[i].newOwner));
			pack_tab.items[i].getLimitedCount = 0;
			pack_tab.items[i].getLimitedPurchaseCount = 0;
#ifdef ENABLE_CHANGELOOK_SYSTEM
			pack_tab.items[i].transmutation = 0;
#endif
#ifdef ENABLE_BUY_WITH_ITEM
			pack_tab.items[i].witemVnum = 0;
#endif
		}

		memcpy(buf, &pack_tab, sizeof(pack_tab));
		buf += sizeof(pack_tab);
		size += sizeof(pack_tab);
	}

	pack.size = sizeof(pack) + sizeof(pack2) + size;

	ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
	ch->GetDesc()->BufferedPacket(&pack2, sizeof(TPacketGCShopStartEx));
	ch->GetDesc()->Packet(temp, size);

	return true;
}

int CShopEx::Buy(LPCHARACTER ch, BYTE pos, bool unused)
{
	BYTE tabIdx = pos / SHOP_HOST_ITEM_MAX_NUM;
	BYTE slotPos = pos % SHOP_HOST_ITEM_MAX_NUM;
	if (tabIdx >= GetTabCount())
	{
		sys_log(0, "ShopEx::Buy : invalid position %d : %s", pos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	sys_log(0, "ShopEx::Buy : name %s pos %d", ch->GetName(), pos);

	GuestMapType::iterator it = m_map_guest.find(ch);

	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;

	TShopTableEx & shopTab = m_vec_shopTabs[tabIdx];
	TShopItemTable & r_item = shopTab.items[slotPos];

	if (r_item.price <= 0)
	{
		LogManager::instance().HackLog("SHOP_BUY_GOLD_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}

	DWORD dwPrice = r_item.price;

	switch (shopTab.coinType)
	{
	case SHOP_COIN_TYPE_GOLD:
		if (it->second)	// if other empire, price is triple
			dwPrice *= 3;

		if (ch->GetGold() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough money : %s has %d, price %d", ch->GetName(), ch->GetGold(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
		}
		break;
	case SHOP_COIN_TYPE_SECONDARY_COIN:
	{
		int count = ch->CountSpecifyItem(U_ITEM_SECONDARY_COIN);
		if (count < dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough myeongdojun : %s has %d, price %d", ch->GetName(), count, dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX;
		}
	}
	break;
#ifdef ENABLE_EVENT_SYSTEM
	case SHOP_COIN_TYPE_YEAR_ROUND_COIN:
	{
		int count = ch->CountSpecifyItem(25105);
		if (count < dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough year_round_coin : %s has %d, price %d", ch->GetName(), count, dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_YEAR_ROUND_COIN;
		}
	}
	break;
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	case SHOP_COIN_TYPE_COMBAT_ZONE:
	{
		int iCurrentLimitPoints = ch->GetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS);

		if (iCurrentLimitPoints == COMBAT_ZONE_SHOP_MAX_LIMIT_POINTS)
			return SHOP_SUBHEADER_GC_MAX_LIMIT_POINTS;

		if (ch->GetRealCombatZonePoints() < dwPrice)
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_POINTS;

		if ((iCurrentLimitPoints + dwPrice) > COMBAT_ZONE_SHOP_MAX_LIMIT_POINTS)
			return SHOP_SUBHEADER_GC_OVERFLOW_LIMIT_POINTS;
	}
	break;
#endif

	case SHOP_COIN_TYPE_CHEQUE:
	{
		if (ch->GetCheque() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough cheque : %s has %d, price %d", ch->GetName(), ch->GetCheque(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
		}
	}
	break;

	case SHOP_COIN_TYPE_GAYA:
	{
		if (ch->GetGem() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough gaya : %s has %d, price %d", ch->GetName(), ch->GetGem(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_GAYA_POINT;
		}
	}
	break;

	case SHOP_COIN_TYPE_ZODIAC:
	{
		if (ch->GetZodiacPoint() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough zodiac : %s has %d, price %d", ch->GetName(), ch->GetZodiacPoint(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_ZODIAC_POINT;
		}
	}
	break;
	
#ifdef ENABLE_WORSHIP_SYSTEM
	case SHOP_COIN_TYPE_WORSHIP:
	{
		if (ch->GetWorshipPoint() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough worship : %s has %d, price %d", ch->GetName(), ch->GetWorshipPoint(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_WORSHIP_POINT;
		}
	}
	break;
#endif

	case SHOP_COIN_TYPE_EXP:
	{
		if (ch->GetExp() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough exp : %s has %d, price %d", ch->GetName(), ch->GetExp(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_EXP;
		}
	}
	break;

	case SHOP_COIN_TYPE_DRAGON_COIN:
	{
		if (ch->GetDragonCoin() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough dc : %s has %d, price %d", ch->GetName(), ch->GetDragonCoin(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_DRAGON_COIN;
		}
	}
	break;

	case SHOP_COIN_TYPE_DRAGON_MARK:
	{
		if (ch->GetDragonMark() < (int)dwPrice)
		{
			sys_log(1, "ShopEx::Buy : Not enough dc : %s has %d, price %d", ch->GetName(), ch->GetDragonMark(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_DRAGON_MARK;
		}
	}
	break;
	}

	LPITEM item;

	item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

	if (!item)
		return SHOP_SUBHEADER_GC_SOLD_OUT;

	int iEmptyPos;
	if (item->IsDragonSoul())
	{
		iEmptyPos = ch->GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
	{
		iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
	}
	else if (item->IsStone())
	{
		iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
	}
	else if (item->IsUpgradeItem())
	{
		iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
	}
	else if (item->IsChest())
	{
		iEmptyPos = ch->GetEmptyChestInventory(item->GetSize());
	}
	else if (item->IsAttr())
	{
		iEmptyPos = ch->GetEmptyAttrInventory(item->GetSize());
	}
	else if (item->IsFlower())
	{
		iEmptyPos = ch->GetEmptyFlowerInventory(item->GetSize());
	}
#endif
	else
	{
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());
	}

	if (iEmptyPos < 0)
	{
		sys_log(1, "ShopEx::Buy : Inventory full : %s size %d", ch->GetName(), item->GetSize());
		M2_DESTROY_ITEM(item);
		return SHOP_SUBHEADER_GC_INVENTORY_FULL;
	}

	switch (shopTab.coinType)
	{
	case SHOP_COIN_TYPE_GOLD:
		ch->PointChange(POINT_GOLD, -dwPrice, false);
		break;
	case SHOP_COIN_TYPE_SECONDARY_COIN:
		ch->RemoveSpecifyItem(U_ITEM_SECONDARY_COIN, dwPrice);
		break;
#ifdef ENABLE_EVENT_SYSTEM
	case SHOP_COIN_TYPE_YEAR_ROUND_COIN:
		ch->RemoveSpecifyItem((int)25105, (unsigned int)dwPrice);
		break;
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	case SHOP_COIN_TYPE_COMBAT_ZONE:
	{
		ch->SetRealCombatZonePoints(ch->GetRealCombatZonePoints() - dwPrice);
		ch->SetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS, ch->GetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS) + dwPrice);
		ch->SetQuestFlag(COMBAT_ZONE_FLAG_BUY_LAST_TIME, get_global_time());

		std::vector<DWORD> m_vec_refreshData;
		m_vec_refreshData.push_back(ch->GetRealCombatZonePoints() - dwPrice);
		m_vec_refreshData.push_back(ch->GetQuestFlag(COMBAT_ZONE_FLAG_LIMIT_POINTS));
		m_vec_refreshData.push_back(COMBAT_ZONE_SHOP_MAX_LIMIT_POINTS);
		m_vec_refreshData.push_back(0);
		CCombatZoneManager::instance().SendCombatZoneInfoPacket(ch, COMBAT_ZONE_SUB_HEADER_REFRESH_SHOP, m_vec_refreshData);
	}
	break;
#endif
	case SHOP_COIN_TYPE_CHEQUE:
		ch->PointChange(POINT_CHEQUE, -dwPrice, false);
		break;
	case SHOP_COIN_TYPE_GAYA:
		ch->PointChange(POINT_GEM, -dwPrice, false);
		break;
	case SHOP_COIN_TYPE_ZODIAC:
		ch->PointChange(POINT_ZODIAC, -dwPrice, false);
		break;
#ifdef ENABLE_WORSHIP_SYSTEM
	case SHOP_COIN_TYPE_WORSHIP:
		ch->PointChange(POINT_WORSHIP, -dwPrice, false);
#endif
	case SHOP_COIN_TYPE_EXP:
		ch->PointChange(POINT_EXP, -dwPrice, false);
		break;
	case SHOP_COIN_TYPE_DRAGON_COIN:
		ch->SetDragonCoin(ch->GetDragonCoin() - dwPrice);
		break;
	case SHOP_COIN_TYPE_DRAGON_MARK:
		ch->SetDragonMark(ch->GetDragonMark() - dwPrice);
		break;
	}

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

	ITEM_MANAGER::instance().FlushDelayedSave(item);
	LogManager::instance().ItemLog(ch, item, "BUY", item->GetName());

	if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
	{
		LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), PERSONAL_SHOP_BUY, "");
	}

	DBManager::instance().SendMoneyLog(MONEY_LOG_SHOP, item->GetVnum(), -dwPrice);

	if (item)
		sys_log(0, "ShopEx: BUY: name %s %s(x %d):%u price %u", ch->GetName(), item->GetName(), item->GetCount(), item->GetID(), dwPrice);

#ifdef ENABLE_FLUSH_CACHE_FEATURE // @warme006
	{
		ch->SaveReal();
		db_clientdesc->DBPacketHeader(HEADER_GD_FLUSH_CACHE, 0, sizeof(DWORD));
		DWORD pid = ch->GetPlayerID();
		db_clientdesc->Packet(&pid, sizeof(DWORD));
	}
#else
	{
		ch->Save();
	}
#endif

	return (SHOP_SUBHEADER_GC_OK);
}