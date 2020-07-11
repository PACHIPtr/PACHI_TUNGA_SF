#include "stdafx.h"
#include "../../libgame/libgame/grid.h"
#include "utils.h"
#include "desc.h"
#include "desc_client.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "locale_service.h"
#include "../../common/length.h"
#include "exchange.h"
#include "DragonSoul.h"
#include "config.h"
#include "questmanager.h"
#include "pvp.h"
#include "char_manager.h"
#include "utils.h"
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
#include "buffer_manager.h"
#include <ctime>
#include "config.h"
#endif
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
#include "messenger_manager.h"
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_BUFFI_SYSTEM
#include "support_shaman.h"
#endif

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
extern int passes_per_sec;
#endif

extern bool trade_effect;
extern int trade_effect_exchange_threshold;

void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void* pvData = nullptr);

// ±³È¯ ÆÐÅ¶
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void* pvData)
{
	if (!ch->GetDesc())
		return;

	struct packet_exchange pack_exchg;

	pack_exchg.header = HEADER_GC_EXCHANGE;
	pack_exchg.sub_header = sub_header;
	pack_exchg.is_me = is_me;
	pack_exchg.arg1 = arg1;
	pack_exchg.arg2 = arg2;
	pack_exchg.arg3 = arg3;

	if (sub_header == EXCHANGE_SUBHEADER_GC_ITEM_ADD && pvData)
	{
#ifdef WJ_TRADABLE_ICON
		pack_exchg.arg4 = TItemPos(((LPITEM)pvData)->GetWindow(), ((LPITEM)pvData)->GetCell());
#endif
		thecore_memcpy(&pack_exchg.alSockets, ((LPITEM)pvData)->GetSockets(), sizeof(pack_exchg.alSockets));
		thecore_memcpy(&pack_exchg.aAttr, ((LPITEM)pvData)->GetAttributes(), sizeof(pack_exchg.aAttr));
#ifdef ENABLE_CHANGELOOK_SYSTEM
		pack_exchg.dwTransmutation = ((LPITEM)pvData)->GetTransmutation();
#endif
	}
	else
	{
#ifdef WJ_TRADABLE_ICON
		pack_exchg.arg4 = TItemPos(RESERVED_WINDOW, 0);
#endif
		memset(&pack_exchg.alSockets, 0, sizeof(pack_exchg.alSockets));
		memset(&pack_exchg.aAttr, 0, sizeof(pack_exchg.aAttr));
#ifdef ENABLE_CHANGELOOK_SYSTEM
		pack_exchg.dwTransmutation = 0;
#endif
	}

	ch->GetDesc()->Packet(&pack_exchg, sizeof(pack_exchg));
}

// ±³È¯À» ½ÃÀÛ
bool CHARACTER::ExchangeStart(LPCHARACTER victim)
{
	if (this == victim)	// ÀÚ±â ÀÚ½Å°ú´Â ±³È¯À» ¸øÇÑ´Ù.
		return false;

	if (IsObserverMode())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°üÀü »óÅÂ¿¡¼­´Â ±³È¯À» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (victim->IsNPC())
		return false;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()) || CCombatZoneManager::Instance().IsCombatZoneMap(victim->GetMapIndex()))
		return false;
#endif

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	if (MessengerManager::instance().IsBlocked_Target(GetName(), victim->GetName()))
	{
		//ben blokladým hacý
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s blokkk"), victim->GetName());
		return false;
	}
	if (MessengerManager::instance().IsBlocked_Me(GetName(), victim->GetName()))
	{
		//o blokladý hacý
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s blokkk_me"), victim->GetName());
		return false;
	}
#endif
	//PREVENT_TRADE_WINDOW

	if (IsOpenSafebox() || GetShopOwner() || GetMyShop() || IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­·ÁÀÖÀ»°æ¿ì °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (IsCombOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­·ÁÀÖÀ»°æ¿ì °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}
#endif

	if (victim->IsOpenSafebox() || victim->GetShopOwner() || victim->GetMyShop() || victim->IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ´Ù¸¥ °Å·¡ÁßÀÌ¶ó °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (victim->IsCombOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ´Ù¸¥ °Å·¡ÁßÀÌ¶ó °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}
#endif

	//END_PREVENT_TRADE_WINDOW
	int iDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	// °Å¸® Ã¼Å©
	if (iDist >= EXCHANGE_MAX_DISTANCE)
		return false;

	if (GetExchange())
		return false;

	if (victim->GetExchange())
	{
		exchange_packet(this, EXCHANGE_SUBHEADER_GC_ALREADY, 0, 0, NPOS, 0);
		return false;
	}

	if (victim->IsBlockMode(BLOCK_EXCHANGE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ±³È¯ °ÅºÎ »óÅÂÀÔ´Ï´Ù."));
		return false;
	}

	SetExchange(M2_NEW CExchange(this));
	victim->SetExchange(M2_NEW CExchange(victim));

	victim->GetExchange()->SetCompany(GetExchange());
	GetExchange()->SetCompany(victim->GetExchange());

	//
	SetExchangeTime();
	victim->SetExchangeTime();

	exchange_packet(victim, EXCHANGE_SUBHEADER_GC_START, 0, GetVID(), NPOS, 0);
	exchange_packet(this, EXCHANGE_SUBHEADER_GC_START, 0, victim->GetVID(), NPOS, 0);

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	victim->GetExchange()->SendInfo(false, LC_TEXT("TRADE_STARTED"));
	GetExchange()->SendInfo(false, LC_TEXT("TRADE_STARTED"));
#endif

	return true;
}

CExchange::CExchange(LPCHARACTER pOwner)
{
	m_pCompany = nullptr;

	m_bAccept = false;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		m_apItems[i] = nullptr;
		m_aItemPos[i] = NPOS;
		m_abItemDisplayPos[i] = 0;
	}

	m_lGold = 0;

#ifdef ENABLE_CHEQUE_SYSTEM
	m_bCheque = 0;
#endif

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	m_lLastCriticalUpdatePulse = 0;
#endif

	m_pOwner = pOwner;
	pOwner->SetExchange(this);

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	m_pGrid = M2_NEW CGrid(5, 4);
#else
	m_pGrid = M2_NEW CGrid(4, 3);
#endif
}

CExchange::~CExchange()
{
	M2_DELETE(m_pGrid);
}

bool CExchange::AddItem(TItemPos item_pos, BYTE display_pos)
{
	assert(m_pOwner != nullptr && GetCompany());

	if (!item_pos.IsValidItemPosition())
		return false;

	// Àåºñ´Â ±³È¯ÇÒ ¼ö ¾øÀ½
	if (item_pos.IsEquipPosition())
		return false;

	LPITEM item;

	if (!(item = m_pOwner->GetItem(item_pos)))
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE))
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛÀ» °Ç³×ÁÙ ¼ö ¾ø½À´Ï´Ù."));
		return false;
	}

	if (true == item->isLocked())
	{
		return false;
	}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (item->IsSealed())
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't trade with sealed item."));
		return false;
	}
#endif

	if (item->IsBasicItem())
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return false;
	}

	// ÀÌ¹Ì ±³È¯Ã¢¿¡ Ãß°¡µÈ ¾ÆÀÌÅÛÀÎ°¡?
	if (item->IsExchanging())
	{
		sys_log(0, "EXCHANGE under exchanging");
		return false;
	}

	if (!m_pGrid->IsEmpty(display_pos, 1, item->GetSize()))
	{
		sys_log(0, "EXCHANGE not empty item_pos %d %d %d", display_pos, 1, item->GetSize());
		return false;
	}

	if (m_pOwner->GetQuestItemPtr() == item)
	{
		sys_log(0, "EXCHANGE %s trying to cheat by using a current quest item in trade", m_pOwner->GetName());
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			continue;

		m_apItems[i] = item;
		m_aItemPos[i] = item_pos;
		m_abItemDisplayPos[i] = display_pos;
		m_pGrid->Put(display_pos, 1, item->GetSize());

		item->SetExchanging(true);

		exchange_packet(m_pOwner,
			EXCHANGE_SUBHEADER_GC_ITEM_ADD,
			true,
			item->GetVnum(),
			TItemPos(RESERVED_WINDOW, display_pos),
			item->GetCount(),
			item);

		exchange_packet(GetCompany()->GetOwner(),
			EXCHANGE_SUBHEADER_GC_ITEM_ADD,
			false,
			item->GetVnum(),
			TItemPos(RESERVED_WINDOW, display_pos),
			item->GetCount(),
			item);

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
		std::string link = item->GetHyperlink();
		if (item->GetCount() > 1)
		{
			SendInfo(false, LC_TEXT("YOU_ADDED_%s_%d_OF_EXCHANGE_ITEM"), link.c_str(), item->GetCount());
			GetCompany()->SendInfo(false, LC_TEXT("%s_ADDED_%s_%d_OF_EXCHANGE_ITEM"), GetOwner()->GetName(), link.c_str(), item->GetCount());
		}
		else
		{
			SendInfo(false, LC_TEXT("YOU_ADDED_EXCHANGE_ITEM_%s"), link.c_str());
			GetCompany()->SendInfo(false, LC_TEXT("%s_ADDED_EXCHANGE_ITEM_%s"), GetOwner()->GetName(), link.c_str());
		}
#endif

		sys_log(0, "EXCHANGE AddItem success %s pos(%d, %d) %d", item->GetName(), item_pos.window_type, item_pos.cell, display_pos);

		return true;
	}

	// Ãß°¡ÇÒ °ø°£ÀÌ ¾øÀ½
	return false;
}

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
bool CExchange::RemoveItem(BYTE pos)
{
	return false;

	if (pos >= EXCHANGE_ITEM_MAX_NUM)
		return false;

	int k = -1;
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_abItemDisplayPos[i] == pos)
		{
			k = i;
			break;
		}
	}

	if (k == -1) // Not found
		return false;

	if (!m_apItems[k])
	{
		sys_err("No item on position %d when trying to remove it!", k);
		return false;
	}

	TItemPos PosOfInventory = m_aItemPos[k];
	m_apItems[k]->SetExchanging(false);

	m_pGrid->Get(m_abItemDisplayPos[k], 1, m_apItems[k]->GetSize());

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, true, pos, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, false, pos, PosOfInventory, 0);

	m_lLastCriticalUpdatePulse = thecore_pulse();
	Accept(false);
	GetCompany()->Accept(false);

	// Inform trade window
	std::string link = m_apItems[k]->GetHyperlink();

	if (m_apItems[k]->GetCount() > 1)
	{
		SendInfo(false, LC_TEXT("YOU_REMOVED_%s_%d_OF_EXCHANGE_ITEM"), link.c_str(), m_apItems[k]->GetCount());
		GetCompany()->SendInfo(false, LC_TEXT("%s_REMOVED_%s_%d_OF_EXCHANGE_ITEM"), GetOwner()->GetName(), link.c_str(), m_apItems[k]->GetCount());
	}
	else
	{
		SendInfo(false, LC_TEXT("YOU_REMOVED_EXCHANGE_ITEM_%s"), link.c_str());
		GetCompany()->SendInfo(false, LC_TEXT("%s_REMOVED_EXCHANGE_ITEM_%s"), GetOwner()->GetName(), link.c_str());
	}

	// Mark as removed
	m_apItems[k] = nullptr;
	m_aItemPos[k] = NPOS;
	m_abItemDisplayPos[k] = 0;

	return true;
}
#else
bool CExchange::RemoveItem(BYTE pos)
{
	if (pos >= EXCHANGE_ITEM_MAX_NUM)
		return false;

	if (!m_apItems[pos])
		return false;

	TItemPos PosOfInventory = m_aItemPos[pos];
	m_apItems[pos]->SetExchanging(false);

	m_pGrid->Get(m_abItemDisplayPos[pos], 1, m_apItems[pos]->GetSize());

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, true, pos, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, false, pos, PosOfInventory, 0);

	Accept(false);
	GetCompany()->Accept(false);

	m_apItems[pos] = nullptr;
	m_aItemPos[pos] = NPOS;
	m_abItemDisplayPos[pos] = 0;
	return true;
}
#endif

bool CExchange::AddGold(long gold)
{
	if (gold <= 0)
		return false;

	if (GetOwner()->GetGold() < gold)
	{
		// °¡Áö°í ÀÖ´Â µ·ÀÌ ºÎÁ·.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_GOLD, 0, 0, NPOS, 0);
		return false;
	}

	if (m_lGold > 0)
	{
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	// Inform trade window
	if (m_lGold < gold) // Before < new value: increased
	{
		SendInfo(false, LC_TEXT("YOU_INCREASED_EXCHANGE_GOLD_TO_%d"), gold);
		GetCompany()->SendInfo(false, LC_TEXT("%s_INCREASED_EXCHANGE_GOLD_TO_%d"), GetOwner()->GetName(), gold);
	}
	else
	{
		m_lLastCriticalUpdatePulse = thecore_pulse();
		SendInfo(false, LC_TEXT("YOU_DECREASED_EXCHANGE_GOLD_TO_%d"), gold);
		GetCompany()->SendInfo(false, LC_TEXT("%s_DECREASED_EXCHANGE_GOLD_TO_%d"), GetOwner()->GetName(), gold);
	}
#endif

	m_lGold = gold;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, true, m_lGold, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, false, m_lGold, NPOS, 0);
	return true;
}

#ifdef ENABLE_CHEQUE_SYSTEM
bool CExchange::AddCheque(long Cheque)
{
	if (Cheque <= 0)
		return false;

	if (GetOwner()->GetCheque() < Cheque)
	{
		// °¡Áö°í ÀÖ´Â µ·ÀÌ ºÎÁ·.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
		return false;
	}

	if (m_bCheque > 0)
		return false;

	Accept(false);
	GetCompany()->Accept(false);

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	//Inform trade window
	if (m_bCheque < Cheque) { //Before < new value: increased
		SendInfo(false, LC_TEXT("YOU_INCREASED_EXCHANGE_CHEQUE_TO_%d"), Cheque);
		GetCompany()->SendInfo(false, LC_TEXT("%s_INCREASED_EXCHANGE_CHEQUE_TO_%d"), GetOwner()->GetName(), Cheque);
	}
	else {
		m_lLastCriticalUpdatePulse = thecore_pulse();
		SendInfo(false, LC_TEXT("YOU_DECREASED_EXCHANGE_CHEQUE_TO_%d"), Cheque);
		GetCompany()->SendInfo(false, LC_TEXT("%s_DECREASED_EXCHANGE_CHEQUE_TO_%d"), GetOwner()->GetName(), Cheque);
	}
#endif

	m_bCheque = Cheque;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, true, m_bCheque, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, false, m_bCheque, NPOS, 0);
	return true;
}
#endif
// µ·ÀÌ ÃæºÐÈ÷ ÀÖ´ÂÁö, ±³È¯ÇÏ·Á´Â ¾ÆÀÌÅÛÀÌ ½ÇÁ¦·Î ÀÖ´ÂÁö È®ÀÎ ÇÑ´Ù.
bool CExchange::CheckSpace()
{
	LPCHARACTER	me = GetOwner();

	static CGrid s_grid1(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 1   9 Rows a 5 Columns
	static CGrid s_grid2(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 2   9 Rows a 5 Columns
	static CGrid s_grid3(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 3   9 Rows a 5 Columns
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	CGrid s_grid4(5, me->CheckExtendAvailableSlots3());
#endif

	s_grid1.Clear();
	s_grid2.Clear();
	s_grid3.Clear();
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	s_grid4.Clear();
#endif
	LPITEM item;

	int i;

	const int perPageSlotCount = 45;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	for (i = 0; i < me->GetExtendInvenMax(); ++i)
#else
	for (i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		if (!(item = me->GetInventoryItem(i)))
			continue;

		BYTE itemSize = item->GetSize();

		if (i < perPageSlotCount)
			s_grid1.Put(i, 1, itemSize);
		else if (i < perPageSlotCount * 2)
			s_grid2.Put(i - perPageSlotCount, 1, itemSize);
		else if (i < perPageSlotCount * 3)
			s_grid3.Put(i - perPageSlotCount * 2, 1, itemSize);
		else
			s_grid4.Put(i - perPageSlotCount * 3, 1, itemSize);
	}

	static std::vector <WORD> s_vDSGrid(DRAGON_SOUL_INVENTORY_MAX_NUM);
	std::vector<WORD> exceptDSCells;
	exceptDSCells.clear();

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!((item = GetCompany()->GetItemByPosition(i))))
			continue;

		BYTE itemSize = item->GetSize();

		if (item->IsDragonSoul())
		{
			WORD foundCell = me->GetEmptyDragonSoulInventoryWithExceptions(item, exceptDSCells);
			if (static_cast<int>(foundCell) != -1)
			{
				exceptDSCells.push_back(foundCell);
				continue;
			}

			return false;
		}
		else
		{
			int iPos = s_grid1.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid1.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid2.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid2.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid3.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid3.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid4.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid4.Put(iPos, 1, itemSize);
				continue;
			}

			return false;  // No space left in inventory
		}
	}

	return true;
}

#define SPECIAL_INVENTORY_MAX_NUM	225

bool CExchange::CheckSpecialStorageSpace(BYTE bStorageType)
{
	static CGrid s_grid1(5, SPECIAL_INVENTORY_MAX_NUM/5 / 2);
	static CGrid s_grid2(5, SPECIAL_INVENTORY_MAX_NUM/5 / 2);
	static CGrid s_grid3(5, SPECIAL_INVENTORY_MAX_NUM/5 / 2);
	static CGrid s_grid4(5, SPECIAL_INVENTORY_MAX_NUM/5 / 2);
	static CGrid s_grid5(5, SPECIAL_INVENTORY_MAX_NUM/5 / 2);

	s_grid1.Clear();
	s_grid2.Clear();
	s_grid3.Clear();
	s_grid4.Clear();
	s_grid5.Clear();

	LPCHARACTER	victim = GetCompany()->GetOwner();
	LPITEM item;

	int i;
	
	const int perPageSlotCount = 45;

	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if(bStorageType < 0 || bStorageType > 5)
			continue;
		
		if(bStorageType == 0)
			item = victim->GetSkillBookInventoryItem(i);
		else if(bStorageType == 1)
			item = victim->GetUpgradeItemsInventoryItem(i);
		else if(bStorageType == 2)
			item = victim->GetStoneItemsInventoryItem(i);
		else if(bStorageType == 3)
			item = victim->GetChestItemsInventoryItem(i);
		else if(bStorageType == 4)
			item = victim->GetAttrItemsInventoryItem(i);
		else if(bStorageType == 5)
			item = victim->GetFlowerItemsInventoryItem(i);

		if (!item)
			continue;

		BYTE itemSize = item->GetSize();

		if (i < perPageSlotCount)
			s_grid1.Put(i, 1, itemSize);
		else if (i < perPageSlotCount * 2)
			s_grid2.Put(i - perPageSlotCount, 1, itemSize);
		else if (i < perPageSlotCount * 3)
			s_grid3.Put(i - perPageSlotCount * 2, 1, itemSize);
		else if (i < perPageSlotCount * 3)
			s_grid4.Put(i - perPageSlotCount * 3, 1, itemSize);
		else
			s_grid5.Put(i - perPageSlotCount * 4, 1, itemSize);
	}

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!((item = GetCompany()->GetItemByPosition(i))))
			continue;
		
		if(bStorageType < 0 || bStorageType > 5)
			continue;
		
		if((bStorageType == 0 && (!item->IsSkillBook())) || (bStorageType == 1 && (!item->IsUpgradeItem())) || (bStorageType == 2 && (!item->IsStone())) || (bStorageType == 3 && (!item->IsChest())) || (bStorageType == 4 && (!item->IsAttr())) || (bStorageType == 5 && (item->IsFlower())))
			continue;
		
		BYTE itemSize = item->GetSize();

		int iPos = s_grid1.FindBlank(1, itemSize);
		if (iPos >= 0) {
			s_grid1.Put(iPos, 1, itemSize);
			continue;
		}

		iPos = s_grid2.FindBlank(1, itemSize);
		if (iPos >= 0) {
			s_grid2.Put(iPos, 1, itemSize);
			continue;
		}

		iPos = s_grid3.FindBlank(1, itemSize);
		if (iPos >= 0) {
			s_grid3.Put(iPos, 1, itemSize);
			continue;
		}

		iPos = s_grid4.FindBlank(1, itemSize);
		if (iPos >= 0) {
			s_grid4.Put(iPos, 1, itemSize);
			continue;
		}
		
		iPos = s_grid5.FindBlank(1, itemSize);
		if (iPos >= 0) {
			s_grid5.Put(iPos, 1, itemSize);
			continue;
		}

		return false;  // No space left in inventory
	}

	return true;
}

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
void CExchange::Cancel()
{
	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_END, 0, 0, NPOS, 0);
	GetOwner()->SetExchange(nullptr);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			m_apItems[i]->SetExchanging(false);
	}

	if (GetCompany())
	{
		GetCompany()->SetCompany(nullptr);
		GetCompany()->Cancel();
	}

	M2_DELETE(this);
}

bool CExchange::Done(bool firstPlayer)
{
	int		empty_pos, i;
	LPITEM	item;

	LPCHARACTER	victim = GetCompany()->GetOwner();

	if (m_lGold)
	{
		GetOwner()->PointChange(POINT_GOLD, -m_lGold);
		victim->PointChange(POINT_GOLD, m_lGold);
	}
	
	if (trade_effect)
		GetOwner()->CreateFly(6,victim);

	if (m_bCheque)
	{
		GetOwner()->PointChange(POINT_CHEQUE, -m_bCheque);
		victim->PointChange(POINT_CHEQUE, m_bCheque);
	}

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
			empty_pos = victim->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			empty_pos = victim->GetEmptySkillBookInventory(item->GetSize());
		else if (item->IsUpgradeItem())
			empty_pos = victim->GetEmptyUpgradeItemsInventory(item->GetSize());
		else if (item->IsStone())
			empty_pos = victim->GetEmptyStoneInventory(item->GetSize());
		else if (item->IsChest())
			empty_pos = victim->GetEmptyChestInventory(item->GetSize());
		else if (item->IsAttr())
			empty_pos = victim->GetEmptyAttrInventory(item->GetSize());
		else if (item->IsFlower())
			empty_pos = victim->GetEmptyFlowerInventory(item->GetSize());
#endif
		else
			empty_pos = victim->GetEmptyInventory(item->GetSize());

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s", m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		assert(empty_pos >= 0);

		m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 1000);
		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(victim, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			item->AddToCharacter(victim, TItemPos(SKILL_BOOK_INVENTORY, empty_pos));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(victim, TItemPos(UPGRADE_ITEMS_INVENTORY, empty_pos));
		else if (item->IsStone())
			item->AddToCharacter(victim, TItemPos(STONE_ITEMS_INVENTORY, empty_pos));
		else if (item->IsChest())
			item->AddToCharacter(victim, TItemPos(CHEST_ITEMS_INVENTORY, empty_pos));
		else if (item->IsAttr())
			item->AddToCharacter(victim, TItemPos(ATTR_ITEMS_INVENTORY, empty_pos));
		else if (item->IsFlower())
			item->AddToCharacter(victim, TItemPos(FLOWER_ITEMS_INVENTORY, empty_pos));
#endif
		else
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);
		if(trade_effect)
			GetOwner()->CreateFly(FLY_SP_MEDIUM,victim);
		
		if(trade_effect)
			for (int i = 1; i < m_lGold+1; i=i+trade_effect_exchange_threshold) 
				GetOwner()->CreateFly(FLY_SP_SMALL,victim);  // 1 Fly pro 500k . dh max 20 auf einmal (TradeLimit: 10kkk)

		item->SetExchanging(false);

		DWORD itemVnum = item->GetVnum();
		if ((itemVnum >= 80003 && itemVnum <= 80007) || (itemVnum >= 80018 && itemVnum <= 80020))
		{
			LogManager::instance().GoldBarLog(victim->GetPlayerID(), item->GetID(), EXCHANGE_TAKE, "");
			LogManager::instance().GoldBarLog(GetOwner()->GetPlayerID(), item->GetID(), EXCHANGE_GIVE, "");
		}

		m_apItems[i] = nullptr;
	}

	m_pGrid->Clear();
	return true;
}

void CExchange::Accept(bool bIsAccept /*= true*/)
{
	if (!GetCompany())
	{
		sys_err("Invalid company");
		return;
	}

	// Player can't update 5 seconds after a trade decrease.
	if (bIsAccept && GetCompany()->GetLastCriticalUpdatePulse() != 0 && thecore_pulse() < GetCompany()->GetLastCriticalUpdatePulse() + PASSES_PER_SEC(5))
	{
		SendInfo(true, LC_TEXT("YOU_CANT_READY_UP_EXCHANGE"));
		return;
	}

	m_bAccept = !m_bAccept;

	if (!bIsAccept)
		m_bAccept = false;

	// Inform both players
	if (m_bAccept && bIsAccept) // No message on forced removal
	{
		SendInfo(false, LC_TEXT("YOU_READIED_UP_EXCHANGE"));
		GetCompany()->SendInfo(false, LC_TEXT("%s_READIED_UP_EXCHANGE"), GetOwner()->GetName());
	}

	if (!m_bAccept && bIsAccept) // No message on forced removal
	{
		SendInfo(false, LC_TEXT("YOU_REMOVED_READY_EXCHANGE"));
		GetCompany()->SendInfo(false, LC_TEXT("%s_REMOVED_READY_EXCHANGE"), GetOwner()->GetName());
	}

	if (!GetAcceptStatus() || !GetCompany()->GetAcceptStatus())
	{
		// Update the 'accepted' information
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, true, m_bAccept, NPOS, 0);
		exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, false, m_bAccept, NPOS, 0);
		return;
	}

	// Both accepted, run the trade!
	if (!PerformTrade())
	{
		// Something went wrong, unready both sides
		Accept(false);
		GetCompany()->Accept(false);
	}
	else
		Cancel(); // All ok, end the trade.
}

bool CExchange::PerformTrade()
{
	LPCHARACTER otherPlayer = GetCompany()->GetOwner();

	// PREVENT_PORTAL_AFTER_EXCHANGE
	GetOwner()->SetExchangeTime();
	otherPlayer->SetExchangeTime();
	// END_PREVENT_PORTAL_AFTER_EXCHANGE

	// Check the player's gold to make sure that the sums are correct
	if (GetOwner()->GetGold() < GetExchangingGold())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_DONT_HAVE_ENOUGH_GOLD_FOR_EXCHANGE"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s_DOESNT_HAVE_ENOUGH_GOLD_FOR_EXCHANGE"), GetOwner()->GetName());
		return false;
	}

	if (otherPlayer->GetGold() < GetCompany()->GetExchangingGold())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s_DOESNT_HAVE_ENOUGH_GOLD_FOR_EXCHANGE"), otherPlayer->GetName());
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_DONT_HAVE_ENOUGH_GOLD_FOR_EXCHANGE"));
		return false;
	}

	if (GetOwner()->GetCheque() < GetExchangingCheque())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_DONT_HAVE_ENOUGH_CHEQUE_FOR_EXCHANGE"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s_DOESNT_HAVE_ENOUGH_CHEQUE_FOR_EXCHANGE"), GetOwner()->GetName());
		return false;
	}

	if (otherPlayer->GetCheque() < GetCompany()->GetExchangingCheque())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s_DOESNT_HAVE_ENOUGH_CHEQUE_FOR_EXCHANGE"), otherPlayer->GetName());
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_DONT_HAVE_ENOUGH_CHEQUE_FOR_EXCHANGE"));
		return false;
	}

	// Run a sanity check for items that no longer exist,
	// invalid positions, etc.
	if (!SanityCheck())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_SANITY_CHECK_FAILED"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OTHER_EXCHANGE_SANITY_CHECK_FAILED"));
		return false;
	}

	if (!GetCompany()->SanityCheck())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OTHER_EXCHANGE_SANITY_CHECK_FAILED"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_SANITY_CHECK_FAILED"));
		return false;
	}

	// Revise that each player can fit all the items from the trade
	// in their inventory.
	if (!CheckSpace())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_IN_INVENTORY"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_NO_SPACE_LEFT_OTHER_PERSON"));
		return false;
	}

	if (!GetCompany()->CheckSpace())
	{
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_NO_SPACE_LEFT_OTHER_PERSON"));
		GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_IN_INVENTORY"));
		return false;
	}
	
	for(int i = 0; i < 6; i++)
	{
		if (!CheckSpecialStorageSpace(i))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_IN_INVENTORY"));
			GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_NO_SPACE_LEFT_OTHER_PERSON"));
			return false;
		}
		
		if (!GetCompany()->CheckSpecialStorageSpace(i))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("EXCHANGE_NO_SPACE_LEFT_OTHER_PERSON"));
			GetCompany()->GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_IN_INVENTORY"));
			return false;
		}
	}

	if (db_clientdesc->GetSocket() == INVALID_SOCKET)
	{
		sys_err("Cannot use exchange while DB cache connection is dead.");
		otherPlayer->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("UNKNOWN_TRADE_ERROR"));
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("UNKNOWN_TRADE_ERROR"));
		Cancel();
		return false;
	}

	// If nothing was traded, cancel things out
	if (CountExchangingItems() + GetCompany()->CountExchangingItems() == 0 && GetExchangingGold() + GetCompany()->GetExchangingGold() == 0)
	{
		otherPlayer->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("TRADE_COMPLETED_WITHOUT_GOODS_EXCHANGE"));
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("TRADE_COMPLETED_WITHOUT_GOODS_EXCHANGE"));
		Cancel();
		return false;
	}

	if (Done(true))
	{
		if (m_lGold) // µ·ÀÌ ÀÖÀ» ¸¸ ÀúÀå
			GetOwner()->Save();

		if (GetCompany()->Done(false))
		{
			if (GetCompany()->m_lGold) // µ·ÀÌ ÀÖÀ» ¶§¸¸ ÀúÀå
				otherPlayer->Save();

			// INTERNATIONAL_VERSION
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´Ô°úÀÇ ±³È¯ÀÌ ¼º»ç µÇ¾ú½À´Ï´Ù."), otherPlayer->GetName());
			otherPlayer->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´Ô°úÀÇ ±³È¯ÀÌ ¼º»ç µÇ¾ú½À´Ï´Ù."), GetOwner()->GetName());
			// END_OF_INTERNATIONAL_VERSION
		}
	}

	return true;
}

bool CExchange::SanityCheck()
{
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;
	}

	return true;
}
#else

bool CExchange::Check(int* piItemCount)
{
	if (GetOwner()->GetGold() < m_lGold)
		return false;
#ifdef ENABLE_CHEQUE_SYSTEM
	if (GetOwner()->GetCheque() < m_bCheque)
		return false;
#endif

	int item_count = 0;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;

		++item_count;
	}

	*piItemCount = item_count;
	return true;
}
// ±³È¯ ³¡ (¾ÆÀÌÅÛ°ú µ· µîÀ» ½ÇÁ¦·Î ¿Å±ä´Ù)
bool CExchange::Done()
{
	int		empty_pos, i;
	LPITEM	item;

	LPCHARACTER	victim = GetCompany()->GetOwner();

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
			empty_pos = victim->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			empty_pos = victim->GetEmptySkillBookInventory(item->GetSize());
		else if (item->IsUpgradeItem())
			empty_pos = victim->GetEmptyUpgradeItemsInventory(item->GetSize());
#endif
		else
			empty_pos = victim->GetEmptyInventory(item->GetSize());

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s",
				m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		assert(empty_pos >= 0);

		m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 1000);

		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(victim, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		else if (item->IsSkillBook())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
#endif
		else
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		item->SetExchanging(false);
		{
			char exchange_buf[51];

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), GetOwner()->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(victim, item, "EXCHANGE_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), victim->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(GetOwner(), item, "EXCHANGE_GIVE", exchange_buf);

			if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
			{
				LogManager::instance().GoldBarLog(victim->GetPlayerID(), item->GetID(), EXCHANGE_TAKE, "");
				LogManager::instance().GoldBarLog(GetOwner()->GetPlayerID(), item->GetID(), EXCHANGE_GIVE, "");
			}
		}

		m_apItems[i] = nullptr;
	}

	if (m_lGold)
	{
		GetOwner()->PointChange(POINT_GOLD, -m_lGold, true);
		victim->PointChange(POINT_GOLD, m_lGold, true);

		if (m_lGold > 1000)
		{
			char exchange_buf[51];
			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lGold, "EXCHANGE_GOLD_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lGold, "EXCHANGE_GOLD_GIVE", exchange_buf);
		}
	}
#ifdef ENABLE_CHEQUE_SYSTEM
	if (m_bCheque)
	{
		GetOwner()->PointChange(POINT_CHEQUE, -m_bCheque, true);
		victim->PointChange(POINT_CHEQUE, m_bCheque, true);
	}
#endif

	m_pGrid->Clear();
	return true;
}

// ±³È¯À» µ¿ÀÇ
bool CExchange::Accept(bool bAccept)
{
	if (m_bAccept == bAccept)
		return true;

	m_bAccept = bAccept;

	// µÑ ´Ù µ¿ÀÇ ÇßÀ¸¹Ç·Î ±³È¯ ¼º¸³
	if (m_bAccept && GetCompany()->m_bAccept)
	{
		int	iItemCount;

		LPCHARACTER victim = GetCompany()->GetOwner();

		//PREVENT_PORTAL_AFTER_EXCHANGE
		GetOwner()->SetExchangeTime();
		victim->SetExchangeTime();
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

		// exchange_check ¿¡¼­´Â ±³È¯ÇÒ ¾ÆÀÌÅÛµéÀÌ Á¦ÀÚ¸®¿¡ ÀÖ³ª È®ÀÎÇÏ°í,
		// ¿¤Å©µµ ÃæºÐÈ÷ ÀÖ³ª È®ÀÎÇÑ´Ù, µÎ¹øÂ° ÀÎÀÚ·Î ±³È¯ÇÒ ¾ÆÀÌÅÛ °³¼ö
		// ¸¦ ¸®ÅÏÇÑ´Ù.
		if (!Check(&iItemCount))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ·ÀÌ ºÎÁ·ÇÏ°Å³ª ¾ÆÀÌÅÛÀÌ Á¦ÀÚ¸®¿¡ ¾ø½À´Ï´Ù."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ µ·ÀÌ ºÎÁ·ÇÏ°Å³ª ¾ÆÀÌÅÛÀÌ Á¦ÀÚ¸®¿¡ ¾ø½À´Ï´Ù."));
			goto EXCHANGE_END;
		}

		// ¸®ÅÏ ¹ÞÀº ¾ÆÀÌÅÛ °³¼ö·Î »ó´ë¹æÀÇ ¼ÒÁöÇ°¿¡ ³²Àº ÀÚ¸®°¡ ÀÖ³ª È®ÀÎÇÑ´Ù.
		if (!CheckSpace())
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ ¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
			goto EXCHANGE_END;
		}

		// »ó´ë¹æµµ ¸¶Âù°¡Áö·Î..
		if (!GetCompany()->Check(&iItemCount))
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µ·ÀÌ ºÎÁ·ÇÏ°Å³ª ¾ÆÀÌÅÛÀÌ Á¦ÀÚ¸®¿¡ ¾ø½À´Ï´Ù."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ µ·ÀÌ ºÎÁ·ÇÏ°Å³ª ¾ÆÀÌÅÛÀÌ Á¦ÀÚ¸®¿¡ ¾ø½À´Ï´Ù."));
			goto EXCHANGE_END;
		}

		if (!GetCompany()->CheckSpace())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ ¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÁöÇ°¿¡ ºó °ø°£ÀÌ ¾ø½À´Ï´Ù."));
			goto EXCHANGE_END;
		}

		if (db_clientdesc->GetSocket() == INVALID_SOCKET)
		{
			sys_err("Cannot use exchange feature while DB cache connection is dead.");
			victim->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			goto EXCHANGE_END;
		}

		if (Done())
		{
#ifdef ENABLE_CHEQUE_SYSTEM
			if (m_lGold || m_bCheque)
#else
			if (m_lGold) // µ·ÀÌ ÀÖÀ» ‹š¸¸ ÀúÀå
#endif
				GetOwner()->Save();

			if (GetCompany()->Done())
			{
#ifdef ENABLE_CHEQUE_SYSTEM
				if (GetCompany()->m_lGold || GetCompany()->m_bCheque)
#else
				if (GetCompany()->m_lGold) // µ·ÀÌ ÀÖÀ» ¶§¸¸ ÀúÀå
#endif
					victim->Save();

				// INTERNATIONAL_VERSION
				GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´Ô°úÀÇ ±³È¯ÀÌ ¼º»ç µÇ¾ú½À´Ï´Ù."), victim->GetName());
				victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´Ô°úÀÇ ±³È¯ÀÌ ¼º»ç µÇ¾ú½À´Ï´Ù."), GetOwner()->GetName());
				// END_OF_INTERNATIONAL_VERSION
			}
		}

	EXCHANGE_END:
		Cancel();
		return false;
	}
	else
	{
		// ¾Æ´Ï¸é accept¿¡ ´ëÇÑ ÆÐÅ¶À» º¸³»ÀÚ.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, true, m_bAccept, NPOS, 0);
		exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, false, m_bAccept, NPOS, 0);
		return true;
	}
}

// ±³È¯ Ãë¼Ò
void CExchange::Cancel()
{
	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_END, 0, 0, NPOS, 0);
	GetOwner()->SetExchange(nullptr);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			m_apItems[i]->SetExchanging(false);
	}

	if (GetCompany())
	{
		GetCompany()->SetCompany(nullptr);
		GetCompany()->Cancel();
	}

	M2_DELETE(this);
}
#endif

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
void CExchange::SendInfo(bool isError, const char* format, ...)
{
	if (!GetOwner())
		return;

	LPDESC d = GetOwner()->GetDesc();
	if (!d)
		return;

	char rawbuf[1024 + 1];
	va_list args;

	va_start(args, format);
	int len = vsnprintf(rawbuf, sizeof(rawbuf), format, args);
	va_end(args);

	TPacketGCExchageInfo pack;

	pack.bHeader = HEADER_GC_EXCHANGE_INFO;
	pack.wSize = sizeof(TPacketGCExchageInfo) + len;
	pack.bError = isError;
	pack.iUnixTime = std::time(nullptr);

	TEMP_BUFFER tmpbuf;
	tmpbuf.write(&pack, sizeof(pack));
	tmpbuf.write(rawbuf, len);

	d->Packet(tmpbuf.read_peek(), tmpbuf.size());
}

int CExchange::CountExchangingItems()
{
	int count = 0;
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		++count;
	}

	return count;
}

int CExchange::GetPositionByItem(LPITEM item) const
{
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (m_apItems[i] == item)
			return i;
	}

	return -1;
}
#endif