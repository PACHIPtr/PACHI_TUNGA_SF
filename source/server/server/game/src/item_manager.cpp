#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "desc_client.h"
#include "db.h"
#include "log.h"
#include "skill.h"
#include "text_file_loader.h"
#include "priv_manager.h"
#include "questmanager.h"
#include "unique_item.h"
#include "safebox.h"
#include "blend_item.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"

#include "../../common/VnumHelper.h"
#include "DragonSoul.h"
#include "cube.h"
#include <fstream>
#include "PetSystem.h"

ITEM_MANAGER::ITEM_MANAGER()
	: m_iTopOfTable(0), m_dwVIDCount(0), m_dwCurrentID(0)
{
	m_ItemIDRange.dwMin = m_ItemIDRange.dwMax = m_ItemIDRange.dwUsableItemIDMin = 0;
	m_ItemIDSpareRange.dwMin = m_ItemIDSpareRange.dwMax = m_ItemIDSpareRange.dwUsableItemIDMin = 0;
}

ITEM_MANAGER::~ITEM_MANAGER()
{
	Destroy();
}

void ITEM_MANAGER::Destroy()
{
	auto it = m_VIDMap.begin();
	for (; it != m_VIDMap.end(); ++it) {
		M2_DELETE(it->second);
	}
	m_VIDMap.clear();
}

void ITEM_MANAGER::GracefulShutdown()
{
	TR1_NS::unordered_set<LPITEM>::iterator it = m_set_pkItemForDelayedSave.begin();

	while (it != m_set_pkItemForDelayedSave.end())
		SaveSingleItem(*(it++));

	m_set_pkItemForDelayedSave.clear();
}

bool ITEM_MANAGER::InitializeSkillBookItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_skill_book.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_skill_book.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageSkillBook.push_back(iVnum);
	}

	return true;
}

bool ITEM_MANAGER::InitializeUpgradeItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_upgrade_item.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_upgrade_item.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageUpgradeItem.push_back(iVnum);
	}

	return true;
}

bool ITEM_MANAGER::InitializeStoneItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_stone_item.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_stone_item.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageStoneItem.push_back(iVnum);
	}

	return true;
}

bool ITEM_MANAGER::InitializeChestItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_chest_item.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_chest_item.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageChestItem.push_back(iVnum);
	}

	return true;	
}

bool ITEM_MANAGER::InitializeAttrItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_attr_item.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_attr_item.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageAttrItem.push_back(iVnum);
	}

	return true;
}

bool ITEM_MANAGER::InitializeFlowerItemTable()
{
	std::ifstream inf((LocaleService_GetBasePath() + "/special_storage_flower_item.txt").c_str());

	if (!inf.is_open())
	{
		sys_err("SYSTEM Cannot open 'special_storage_flower_item.txt'.");
		return false;
	}

	std::string ln;
	while (getline(inf, ln))
	{
		if (ln.empty())
			continue;

		DWORD iVnum = atoi(ln.c_str());
		m_listSpecialStorageFlowerItem.push_back(iVnum);
	}

	return true;
}	

bool ITEM_MANAGER::Initialize(TItemTable * table, int size)
{
	InitializeSkillBookItemTable();
	InitializeUpgradeItemTable();
	InitializeStoneItemTable();
	InitializeChestItemTable();
	InitializeAttrItemTable();
	InitializeFlowerItemTable();
	
	if (!m_vec_prototype.empty())
		m_vec_prototype.clear();

	int	i;

	m_vec_prototype.resize(size);
	thecore_memcpy(&m_vec_prototype[0], table, sizeof(TItemTable) * size);
	for (int i = 0; i < size; i++)
	{
		if (0 != m_vec_prototype[i].dwVnumRange)
		{
			m_vec_item_vnum_range_info.push_back(&m_vec_prototype[i]);
		}
	}

	m_map_ItemRefineFrom.clear();
	for (i = 0; i < size; ++i)
	{
		if (m_vec_prototype[i].dwRefinedVnum)
			m_map_ItemRefineFrom.insert(std::make_pair(m_vec_prototype[i].dwRefinedVnum, m_vec_prototype[i].dwVnum));

		// NOTE : QUEST_GIVE ÇÃ·¡±×´Â npc ÀÌº¥Æ®·Î ¹ß»ý.
		if (m_vec_prototype[i].bType == ITEM_QUEST || IS_SET(m_vec_prototype[i].dwFlags, ITEM_FLAG_QUEST_USE_MULTIPLE))
			quest::CQuestManager::instance().RegisterNPCVnum(m_vec_prototype[i].dwVnum);

		m_map_vid.insert(std::map<DWORD, TItemTable>::value_type(m_vec_prototype[i].dwVnum, m_vec_prototype[i]));
		if (test_server)
			sys_log(0, "ITEM_INFO %d %s ", m_vec_prototype[i].dwVnum, m_vec_prototype[i].szName);
	}

	int len = 0, len2;
	char buf[512];

	for (i = 0; i < size; ++i)
	{
		len2 = snprintf(buf + len, sizeof(buf) - len, "%5u %-16s", m_vec_prototype[i].dwVnum, m_vec_prototype[i].szLocaleName);

		if (len2 < 0 || len2 >= (int) sizeof(buf) - len)
			len += (sizeof(buf) - len) - 1;
		else
			len += len2;

		if (!((i + 1) % 4))
		{
			if (!test_server)
				sys_log(0, "%s", buf);
			len = 0;
		}
		else
		{
			buf[len++] = '\t';
			buf[len] = '\0';
		}
	}

	if ((i + 1) % 4)
	{
		if (!test_server)
			sys_log(0, "%s", buf);
	}

	ITEM_VID_MAP::iterator it = m_VIDMap.begin();

	sys_log(1, "ITEM_VID_MAP %d", m_VIDMap.size());

	while (it != m_VIDMap.end())
	{
		LPITEM item = it->second;
		++it;

		const TItemTable* tableInfo = GetTable(item->GetOriginalVnum());

		if (nullptr == tableInfo)
		{
			sys_err("cannot reset item table");
			item->SetProto(nullptr);
		}

		item->SetProto(tableInfo);
	}

	return true;
}

LPITEM ITEM_MANAGER::CreateItem(DWORD vnum, DWORD count, DWORD id, bool bTryMagic, int iRarePct, bool bSkipSave)
{
	if (0 == vnum)
		return nullptr;

	DWORD dwMaskVnum = 0;
	if (GetMaskVnum(vnum))
	{
		dwMaskVnum = GetMaskVnum(vnum);
	}

	const TItemTable* table = GetTable(vnum);

	if (nullptr == table)
		return nullptr;

	LPITEM item = nullptr;

	//id·Î °Ë»çÇØ¼­ Á¸ÀçÇÑ´Ù¸é -- ¸®ÅÏ!
	if (m_map_pkItemByID.find(id) != m_map_pkItemByID.end())
	{
		item = m_map_pkItemByID[id];
		LPCHARACTER owner = item->GetOwner();
		sys_err("ITEM_ID_DUP: %u %s owner %p", id, item->GetName(), get_pointer(owner));
		return nullptr;
	}

	item = M2_NEW CItem(vnum);

	bool bIsNewItem = (0 == id);

	//ÃÊ±âÈ­ ÇÏ°í. Å×ÀÌºí ¼ÂÇÏ°í
	item->Initialize();
	item->SetProto(table);
	item->SetMaskVnum(dwMaskVnum);

	if (item->GetType() == ITEM_ELK) // µ·Àº ID°¡ ÇÊ¿ä¾ø°í ÀúÀåµµ ÇÊ¿ä¾ø´Ù.
		item->SetSkipSave(true);

	// Unique ID¸¦ ¼¼ÆÃÇÏÀÚ
	else if (!bIsNewItem)
	{
		item->SetID(id);
		item->SetSkipSave(true);
	}
	else
	{
		item->SetID(GetNewID());
		item->SetSkipSave(bSkipSave); // mami

#ifdef ENABLE_TALISMAN_SYSTEM
		if (item->GetType() == ITEM_UNIQUE && !item->IsCharmItem())
#else
		if (item->GetType() == ITEM_UNIQUE)
#endif
		{
			if (item->GetValue(2) == 0)
				item->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, item->GetValue(0)); // °ÔÀÓ ½Ã°£ À¯´ÏÅ©
			else
			{
				//int globalTime = get_global_time();
				//int lastTime = item->GetValue(0);
				//int endTime = get_global_time() + item->GetValue(0);
				item->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, get_global_time() + item->GetValue(0)); // ½Ç½Ã°£ À¯´ÏÅ©
			}
		}
	}

	switch (item->GetVnum())
	{
	case ITEM_AUTO_HP_RECOVERY_S:
	case ITEM_AUTO_HP_RECOVERY_M:
	case ITEM_AUTO_HP_RECOVERY_L:
	case ITEM_AUTO_HP_RECOVERY_X:
	case ITEM_AUTO_SP_RECOVERY_S:
	case ITEM_AUTO_SP_RECOVERY_M:
	case ITEM_AUTO_SP_RECOVERY_L:
	case ITEM_AUTO_SP_RECOVERY_X:
	case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
	case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
	case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
	case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
	case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
	case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
		if (bIsNewItem)
			item->SetSocket(2, item->GetValue(0), true);
		else
			item->SetSocket(2, item->GetValue(0), false);
		break;
	}

	if (item->GetType() == ITEM_ELK) // µ·Àº ¾Æ¹« Ã³¸®°¡ ÇÊ¿äÇÏÁö ¾ÊÀ½
		;

	else if (item->IsStackable())  // ÇÕÄ¥ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ °æ¿ì
	{
		count = MINMAX(1, count, g_bItemCountLimit);

		if (bTryMagic && count <= 1 && IS_SET(item->GetFlag(), ITEM_FLAG_MAKECOUNT))
			count = item->GetValue(1);
	}
	else
		count = 1;

	item->SetVID(++m_dwVIDCount);

	if (bSkipSave == false)
		m_VIDMap.insert(ITEM_VID_MAP::value_type(item->GetVID(), item));

	if (item->GetID() != 0 && bSkipSave == false)
		m_map_pkItemByID.insert(std::map<DWORD, LPITEM>::value_type(item->GetID(), item));

	if (!item->SetCount(count))
		return nullptr;

	//item->SetSkipSave(false); // mami
	
#ifdef ENABLE_TALISMAN_SYSTEM
	if (item->GetType() == ITEM_UNIQUE && item->GetValue(2) != 0 && !item->IsCharmItem())
#else
	if (item->GetType() == ITEM_UNIQUE && item->GetValue(2) != 0)
#endif
		item->StartUniqueExpireEvent();

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		// ¾ÆÀÌÅÛ »ý¼º ½ÃÁ¡ºÎÅÍ »ç¿ëÇÏÁö ¾Ê¾Æµµ ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä
		if (LIMIT_REAL_TIME == item->GetLimitType(i))
		{
			if (item->GetLimitValue(i))
			{
				item->SetSocket(0, time(0) + item->GetLimitValue(i));
			}
			else
			{
				item->SetSocket(0, time(0) + 60 * 60 * 24 * 7);
			}

			item->StartRealTimeExpireEvent();
		}

		// ±âÁ¸ À¯´ÏÅ© ¾ÆÀÌÅÛÃ³·³ Âø¿ë½Ã¿¡¸¸ »ç¿ë°¡´É ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä
		else if (LIMIT_TIMER_BASED_ON_WEAR == item->GetLimitType(i))
		{
			// ÀÌ¹Ì Âø¿ëÁßÀÎ ¾ÆÀÌÅÛÀÌ¸é Å¸ÀÌ¸Ó¸¦ ½ÃÀÛÇÏ°í, »õ·Î ¸¸µå´Â ¾ÆÀÌÅÛÀº »ç¿ë °¡´É ½Ã°£À» ¼¼ÆÃÇØÁØ´Ù. (
			// ¾ÆÀÌÅÛ¸ô·Î Áö±ÞÇÏ´Â °æ¿ì¿¡´Â ÀÌ ·ÎÁ÷¿¡ µé¾î¿À±â Àü¿¡ Socket0 °ªÀÌ ¼¼ÆÃÀÌ µÇ¾î ÀÖ¾î¾ß ÇÑ´Ù.
			if (true == item->IsEquipped())
			{
				item->StartTimerBasedOnWearExpireEvent();
			}
			else if (0 == id)
			{
				long duration = item->GetSocket(0);
				if (0 == duration)
					duration = item->GetLimitValue(i);

				if (0 == duration)
					duration = 60 * 60 * 10;	// Á¤º¸°¡ ¾Æ¹«°Íµµ ¾øÀ¸¸é µðÆúÆ®·Î 10½Ã°£ ¼¼ÆÃ

				item->SetSocket(0, duration);
			}
		}
	}
#ifdef ENABLE_MINI_GAME_CATCH_KING
	if (item->GetVnum() == 79603 || item->GetVnum() == 79604)
	{
		if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event"))
		{
			int iEndTime = quest::CQuestManager::instance().GetEventFlag("catch_king_event_end_day");

			if (iEndTime)
			{
				item->SetSocket(0, iEndTime);
			}
		}
	}
#endif
	if (id == 0) // »õ·Î ¸¸µå´Â ¾ÆÀÌÅÛÀÏ ¶§¸¸ Ã³¸®
	{
		if (item->GetType() == ITEM_GACHA)
			item->SetSocket(0, item->GetLimitValue(1));
		// »õ·ÎÃß°¡µÇ´Â ¾àÃÊµéÀÏ°æ¿ì ¼º´ÉÀ» ´Ù¸£°ÔÃ³¸®
		if (ITEM_BLEND == item->GetType())
		{
			if (Blend_Item_find(item->GetVnum()))
			{
				Blend_Item_set_value(item);
				return item;
			}
		}

		if (table->sAddonType)
		{
			item->ApplyAddon(table->sAddonType);
		}

		if (bTryMagic)
		{
			if (iRarePct == -1)
				iRarePct = table->bAlterToMagicItemPct;

			if (number(1, 100) <= iRarePct)
				item->AlterToMagicItem();
		}

		if (table->bGainSocketPct)
			item->AlterToSocketItem(table->bGainSocketPct);

		if (vnum == 50300 || vnum == ITEM_SKILLFORGET_VNUM)
		{
			DWORD dwSkillVnum;
			do
			{
				if (number(1, 20) == 2)
				{
					dwSkillVnum = number(170, 175);
				}
				else
				{
					dwSkillVnum = number(1, 111);
				}

				if (nullptr != CSkillManager::instance().Get(dwSkillVnum))
					break;
			} while (true);
			item->SetSocket(0, dwSkillVnum);
		}
		else if (ITEM_SKILLFORGET2_VNUM == vnum)
		{
			DWORD dwSkillVnum;

			do
			{
				dwSkillVnum = number(112, 119);

				if (nullptr != CSkillManager::instance().Get(dwSkillVnum))
					break;
			} while (true);

			item->SetSocket(0, dwSkillVnum);
		}
	}
	else
	{
		// 100% È®·ü·Î ¼Ó¼ºÀÌ ºÙ¾î¾ß ÇÏ´Âµ¥ ¾È ºÙ¾îÀÖ´Ù¸é »õ·Î ºÙÈù´Ù. ...............
		if (100 == table->bAlterToMagicItemPct && 0 == item->GetAttributeCount())
		{
			item->AlterToMagicItem();
		}
	}

	if (item->GetType() == ITEM_QUEST)
	{
		for (auto it = m_map_pkQuestItemGroup.begin(); it != m_map_pkQuestItemGroup.end(); it++)
		{
			if (it->second->m_bType == CSpecialItemGroup::QUEST && it->second->Contains(vnum))
			{
				item->SetSIGVnum(it->first);
			}
		}
	}
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	else if (item->GetType() == ITEM_UNIQUE || item->GetSubType() == COSTUME_MOUNT)
#else
	else if (item->GetType() == ITEM_UNIQUE)
#endif
	{
		for (auto it = m_map_pkSpecialItemGroup.begin(); it != m_map_pkSpecialItemGroup.end(); it++)
		{
			if (it->second->m_bType == CSpecialItemGroup::SPECIAL && it->second->Contains(vnum))
			{
				item->SetSIGVnum(it->first);
			}
		}
	}

#ifdef ENABLE_ACCESSORY_BUG_FIX
	if (item->IsAccessoryForSocket())
	{
		item->SetSocket(0, 0);
		item->SetSocket(1, 0);
		item->SetSocket(2, 0);
	}
#endif

	// »õ·Î »ý¼ºµÇ´Â ¿ëÈ¥¼® Ã³¸®.
	if (item->IsDragonSoul() && 0 == id)
	{
		DSManager::instance().DragonSoulItemInitialize(item);
	}

	if (item->GetType() == ITEM_SOUL)
	{
		item->SetSocket(2, item->GetValue(2));
	}

	return item;
}

void ITEM_MANAGER::DelayedSave(LPITEM item)
{
	if (item->GetID() != 0)
		m_set_pkItemForDelayedSave.insert(item);
}

void ITEM_MANAGER::FlushDelayedSave(LPITEM item)
{
	TR1_NS::unordered_set<LPITEM>::iterator it = m_set_pkItemForDelayedSave.find(item);

	if (it == m_set_pkItemForDelayedSave.end())
	{
		return;
	}

	m_set_pkItemForDelayedSave.erase(it);
	SaveSingleItem(item);
}

void ITEM_MANAGER::SaveSingleItem(LPITEM item)
{
	if (!item->GetOwner())
	{
		DWORD dwID = item->GetID();
		DWORD dwOwnerID = item->GetLastOwnerPID();

		db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_DESTROY, 0, sizeof(DWORD) + sizeof(DWORD));
		db_clientdesc->Packet(&dwID, sizeof(DWORD));
		db_clientdesc->Packet(&dwOwnerID, sizeof(DWORD));

		sys_log(1, "ITEM_DELETE %s:%u", item->GetName(), dwID);
		return;
	}

	sys_log(1, "ITEM_SAVE %s:%d in %s window %d", item->GetName(), item->GetID(), item->GetWindow());

	TPlayerItem t;

	t.id = item->GetID();
	t.window = item->GetWindow();
	t.pos = t.window == EQUIPMENT ? item->GetCell() - INVENTORY_MAX_NUM : item->GetCell();
	t.count = item->GetCount();
	t.vnum = item->GetOriginalVnum();
#ifdef ENABLE_CHANGELOOK_SYSTEM
	t.transmutation = item->GetTransmutation();
#endif
	t.owner = (t.window == SAFEBOX || t.window == MALL) ? item->GetOwner()->GetDesc()->GetAccountTable().id : item->GetOwner()->GetPlayerID();
	thecore_memcpy(t.alSockets, item->GetSockets(), sizeof(t.alSockets));
	thecore_memcpy(t.aAttr, item->GetAttributes(), sizeof(t.aAttr));
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	t.sealbind = item->GetSealBindTime();
#endif
#ifdef ENABLE_BASIC_ITEM_SYSTEM
	t.is_basic = item->IsBasicItem();
#endif

	db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_SAVE, 0, sizeof(TPlayerItem));
	db_clientdesc->Packet(&t, sizeof(TPlayerItem));
}

void ITEM_MANAGER::Update()
{
	TR1_NS::unordered_set<LPITEM>::iterator it = m_set_pkItemForDelayedSave.begin();
	TR1_NS::unordered_set<LPITEM>::iterator this_it;

	while (it != m_set_pkItemForDelayedSave.end())
	{
		this_it = it++;
		LPITEM item = *this_it;
		
		if (!item)
			continue;
		
		auto ptr = reinterpret_cast<std::uintptr_t>(item);
		
		if (ptr == 0x5 || ptr == 0x1 || ptr == 0x2 || ptr == 0x3 || ptr == 0x4)
			continue;

		// SLOW_QUERY ÇÃ·¡±×°¡ ÀÖ´Â °ÍÀº Á¾·á½Ã¿¡¸¸ ÀúÀåÇÑ´Ù.
		if (item->GetOwner() && IS_SET(item->GetFlag(), ITEM_FLAG_SLOW_QUERY))
			continue;

		SaveSingleItem(item);

		m_set_pkItemForDelayedSave.erase(this_it);
	}
}

void ITEM_MANAGER::RemoveItem(LPITEM item, const char* c_pszReason)
{
	LPCHARACTER o;
	if ((o = item->GetOwner()))
	{
		char szHint[64];
		snprintf(szHint, sizeof(szHint), "%s %u ", item->GetName(), item->GetCount());

		LogManager::instance().ItemLog(o, item, c_pszReason ? c_pszReason : "REMOVE", szHint);

		// SAFEBOX_TIME_LIMIT_ITEM_BUG_FIX
		if (item->GetWindow() == MALL || item->GetWindow() == SAFEBOX)
		{
			// 20050613.ipkn.½Ã°£Á¦ ¾ÆÀÌÅÛÀÌ »óÁ¡¿¡ ÀÖÀ» °æ¿ì ½Ã°£¸¸·á½Ã ¼­¹ö°¡ ´Ù¿îµÈ´Ù.
			CSafebox* pSafebox = item->GetWindow() == MALL ? o->GetMall() : o->GetSafebox();
			if (pSafebox)
			{
				pSafebox->Remove(item->GetCell());
			}
		}
		// END_OF_SAFEBOX_TIME_LIMIT_ITEM_BUG_FIX
		else
		{
			if (!item->IsDragonSoul() && !item->IsSkillBook() && !item->IsUpgradeItem() && !item->IsStone() && !item->IsChest() && !item->IsAttr() && !item->IsFlower())
			{
				o->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 1000);
			}

			item->RemoveFromCharacter();
		}
	}

	M2_DESTROY_ITEM(item);
}

void ITEM_MANAGER::DestroyItem(LPITEM item)
{
	if (item->GetSectree())
		item->RemoveFromGround();

	if (item->GetOwner())
	{
		if (CHARACTER_MANAGER::instance().Find(item->GetOwner()->GetPlayerID()) != nullptr)
		{
			sys_err("DestroyItem: GetOwner %s %s!!", item->GetName(), item->GetOwner()->GetName());
			item->RemoveFromCharacter();
		}
		else
		{
			sys_err("WTH! Invalid item owner. owner pointer : %p", item->GetOwner());
		}
	}

	TR1_NS::unordered_set<LPITEM>::iterator it = m_set_pkItemForDelayedSave.find(item);

	if (it != m_set_pkItemForDelayedSave.end())
		m_set_pkItemForDelayedSave.erase(it);

	DWORD dwID = item->GetID();
	sys_log(2, "ITEM_DESTROY %s:%u", item->GetName(), dwID);

	if (!item->GetSkipSave() && dwID)
	{
		DWORD dwOwnerID = item->GetLastOwnerPID();

		db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_DESTROY, 0, sizeof(DWORD) + sizeof(DWORD));
		db_clientdesc->Packet(&dwID, sizeof(DWORD));
		db_clientdesc->Packet(&dwOwnerID, sizeof(DWORD));
	}
	else
	{
		sys_log(2, "ITEM_DESTROY_SKIP %s:%u (skip=%d)", item->GetName(), dwID, item->GetSkipSave());
	}

	if (dwID)
		m_map_pkItemByID.erase(dwID);

	m_VIDMap.erase(item->GetVID());
	M2_DELETE(item);
}

LPITEM ITEM_MANAGER::Find(DWORD id)
{
	auto it = m_map_pkItemByID.find(id);
	if (it == m_map_pkItemByID.end())
		return nullptr;
	return it->second;
}

LPITEM ITEM_MANAGER::FindByVID(DWORD vid)
{
	auto it = m_VIDMap.find(vid);

	if (it == m_VIDMap.end())
		return nullptr;

	return (it->second);
}

TItemTable* ITEM_MANAGER::GetTable(DWORD vnum)
{
	int rnum = RealNumber(vnum);

	if (rnum < 0)
	{
		for (unsigned int i = 0; i < m_vec_item_vnum_range_info.size(); i++)
		{
			TItemTable* p = m_vec_item_vnum_range_info[i];
			if ((p->dwVnum < vnum) &&
				vnum < (p->dwVnum + p->dwVnumRange))
			{
				return p;
			}
		}

		return nullptr;
	}

	return &m_vec_prototype[rnum];
}

int ITEM_MANAGER::RealNumber(DWORD vnum)
{
	int bot, top, mid;

	bot = 0;
	top = m_vec_prototype.size();

	TItemTable* pTable = &m_vec_prototype[0];

	while (1)
	{
		mid = (bot + top) >> 1;

		if ((pTable + mid)->dwVnum == vnum)
			return (mid);

		if (bot >= top)
			return (-1);

		if ((pTable + mid)->dwVnum > vnum)
			top = mid - 1;
		else
			bot = mid + 1;
	}
}

bool ITEM_MANAGER::GetVnum(const char* c_pszName, DWORD & r_dwVnum)
{
	int len = strlen(c_pszName);

	TItemTable* pTable = &m_vec_prototype[0];

	for (DWORD i = 0; i < m_vec_prototype.size(); ++i, ++pTable)
	{
		if (!strncasecmp(c_pszName, pTable->szLocaleName, len))
		{
			r_dwVnum = pTable->dwVnum;
			return true;
		}
	}

	return false;
}

bool ITEM_MANAGER::GetVnumByOriginalName(const char* c_pszName, DWORD & r_dwVnum)
{
	int len = strlen(c_pszName);

	TItemTable* pTable = &m_vec_prototype[0];

	for (DWORD i = 0; i < m_vec_prototype.size(); ++i, ++pTable)
	{
		if (!strncasecmp(c_pszName, pTable->szName, len))
		{
			r_dwVnum = pTable->dwVnum;
			return true;
		}
	}

	return false;
}

std::set<DWORD> g_set_lotto;

void load_lotto()
{
	static int bLoaded = false;

	if (bLoaded)
		return;

	bLoaded = true;
	FILE* fp = fopen("lotto.txt", "r");

	if (!fp)
		return;

	char buf[256];

	while (fgets(buf, 256, fp))
	{
		char* psz = strchr(buf, '\n');

		if (nullptr != psz)
			* psz = '\0';

		DWORD dw = 0;
		str_to_number(dw, buf);
		g_set_lotto.insert(dw);
	}

	fclose(fp);
}

DWORD lotto()
{
	load_lotto();

	char szBuf[6 + 1];

	do
	{
		for (int i = 0; i < 6; ++i)
			szBuf[i] = 48 + number(1, 9);

		szBuf[6] = '\0';

		DWORD dw = 0;
		str_to_number(dw, szBuf);

		if (g_set_lotto.end() == g_set_lotto.find(dw))
		{
			FILE* fp = fopen("lotto.txt", "a+");
			if (fp)
			{
				fprintf(fp, "%u\n", dw);
				fclose(fp);
			}
			return dw;
		}
	} while (1);
}

class CItemDropInfo
{
public:
	CItemDropInfo(int iLevelStart, int iLevelEnd, int iPercent, DWORD dwVnum) :
		m_iLevelStart(iLevelStart), m_iLevelEnd(iLevelEnd), m_iPercent(iPercent), m_dwVnum(dwVnum)
	{
	}

	int	m_iLevelStart;
	int	m_iLevelEnd;
	int	m_iPercent; // 1 ~ 1000
	DWORD	m_dwVnum;

	friend bool operator < (const CItemDropInfo& l, const CItemDropInfo& r)
	{
		return l.m_iLevelEnd < r.m_iLevelEnd;
	}
};

extern std::vector<CItemDropInfo> g_vec_pkCommonDropItem[MOB_RANK_MAX_NUM];

// 20050503.ipkn.
// iMinimum º¸´Ù ÀÛÀ¸¸é iDefault ¼¼ÆÃ (´Ü, iMinimumÀº 0º¸´Ù Ä¿¾ßÇÔ)
// 1, 0 ½ÄÀ¸·Î ON/OFF µÇ´Â ¹æ½ÄÀ» Áö¿øÇÏ±â À§ÇØ Á¸Àç
int GetDropPerKillPct(int iMinimum, int iDefault, int iDeltaPercent, const char* c_pszFlag)
{
	int iVal = 0;

	if ((iVal = quest::CQuestManager::instance().GetEventFlag(c_pszFlag)))
	{
		if (!test_server && !LC_IsJapan())
		{
			if (iVal < iMinimum)
				iVal = iDefault;

			if (iVal < 0)
				iVal = iDefault;
		}
	}

	if (iVal == 0)
		return 0;

	// ±âº» ¼¼ÆÃÀÏ¶§ (iDeltaPercent=100)
	// 40000 iVal ¸¶¸®´ç ÇÏ³ª ´À³¦À» ÁÖ±â À§ÇÑ »ó¼öÀÓ
	return (40000 * iDeltaPercent / iVal);
}

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
bool ITEM_MANAGER::CreateDropItemVector(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item)
{
	if (pkChr->IsPolymorphed() || pkChr->IsPC())
		return false;

	int iLevel = pkKiller->GetLevel();

	BYTE bRank = pkChr->GetMobRank();
	LPITEM item = nullptr;

	// Drop Item Group
	{
		auto it = m_map_pkDropItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkDropItemGroup.end())
		{
			decltype(it->second->GetVector()) v = it->second->GetVector();

			for (DWORD i = 0; i < v.size(); ++i)
			{
				item = CreateItem(v[i].dwVnum, v[i].iCount, 0, true);

				if (item)
				{
					if (item->GetType() == ITEM_POLYMORPH)
					{
						if (item->GetVnum() == pkChr->GetPolymorphItemVnum())
						{
							item->SetSocket(0, pkChr->GetRaceNum());
						}
					}

					vec_item.push_back(item);
				}
			}
		}
	}

	// MobDropItem Group
	{
		auto it = m_map_pkMobItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkMobItemGroup.end())
		{
			CMobItemGroup* pGroup = it->second;

			// MOB_DROP_ITEM_BUG_FIX
			// 20050805.myevan.MobDropItem ? ???? ?? ?? CMobItemGroup::GetOne() ??? ?? ?? ??
			if (pGroup && !pGroup->IsEmpty())
			{
				const CMobItemGroup::SMobItemGroupInfo& info = pGroup->GetOne();
				item = CreateItem(info.dwItemVnum, info.iCount, 0, true, info.iRarePct);

				if (item) vec_item.push_back(item);
			}
			// END_OF_MOB_DROP_ITEM_BUG_FIX
		}
	}

	// Level Item Group
	{
		auto it = m_map_pkLevelItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkLevelItemGroup.end())
		{
			if (it->second->GetLevelLimit() <= (DWORD)iLevel)
			{
				decltype(it->second->GetVector()) v = it->second->GetVector();

				for (DWORD i = 0; i < v.size(); i++)
				{
					DWORD dwVnum = v[i].dwVNum;
					item = CreateItem(dwVnum, v[i].iCount, 0, true);
					if (item) vec_item.push_back(item);
				}
			}
		}
	}

	// BuyerTheitGloves Item Group
	{
		// by mhh ?? ??? ??? ?? drop ? ???? ??
		if (pkKiller->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0 ||
			pkKiller->IsEquipUniqueGroup(UNIQUE_GROUP_DOUBLE_ITEM))
		{
			auto it = m_map_pkGloveItemGroup.find(pkChr->GetRaceNum());

			if (it != m_map_pkGloveItemGroup.end())
			{
				decltype(it->second->GetVector()) v = it->second->GetVector();

				for (DWORD i = 0; i < v.size(); ++i)
				{
					DWORD dwVnum = v[i].dwVnum;
					item = CreateItem(dwVnum, v[i].iCount, 0, true);
					if (item) vec_item.push_back(item);
				}
			}
		}
	}

	// ??
	if (pkChr->GetMobDropItemVnum())
	{
		auto it = m_map_dwEtcItemDropProb.find(pkChr->GetMobDropItemVnum());

		if (it != m_map_dwEtcItemDropProb.end())
		{
			item = CreateItem(pkChr->GetMobDropItemVnum(), 1, 0, true);
			if (item) vec_item.push_back(item);
		}
	}

	if (pkChr->IsStone())
	{
		if (pkChr->GetDropMetinStoneVnum())
		{
			item = CreateItem(pkChr->GetDropMetinStoneVnum(), 1, 0, true);
			if (item) vec_item.push_back(item);
		}
	}
	
	return vec_item.size();
}
#endif

bool ITEM_MANAGER::GetDropPct(LPCHARACTER pkChr, LPCHARACTER pkKiller, OUT int& iDeltaPercent, OUT int& iRandRange)
{
	if (nullptr == pkChr || nullptr == pkKiller)
		return false;

	int iLevel = pkKiller->GetLevel();
	iDeltaPercent = 100;

	if (!pkChr->IsStone() && pkChr->GetMobRank() >= MOB_RANK_BOSS)
		iDeltaPercent = PERCENT_LVDELTA_BOSS(pkKiller->GetLevel(), pkChr->GetLevel());
	else
		iDeltaPercent = PERCENT_LVDELTA(pkKiller->GetLevel(), pkChr->GetLevel());

	BYTE bRank = pkChr->GetMobRank();

	if (1 == number(1, 50000))
		iDeltaPercent += 1000;
	else if (1 == number(1, 10000))
		iDeltaPercent += 500;

	sys_log(3, "CreateDropItem for level: %d rank: %u pct: %d", iLevel, bRank, iDeltaPercent);
	iDeltaPercent = iDeltaPercent * CHARACTER_MANAGER::instance().GetMobItemRate(pkKiller) / 100;

	int32_t bonus = 0;
	if (pkKiller->IsEquipUniqueItem(UNIQUE_ITEM_DOUBLE_ITEM) && pkKiller->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0) {
		//irremovable gloves + mall
		bonus = 100;
	}
	else if (pkKiller->IsEquipUniqueItem(UNIQUE_ITEM_DOUBLE_ITEM) || (pkKiller->IsEquipUniqueGroup(UNIQUE_GROUP_DOUBLE_ITEM) && pkKiller->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0)) { 
		//irremovable gloves OR removable gloves + mall
		bonus = 50;
	}
	
#ifdef ENABLE_DROP_PCT_EXTRA
	bonus += 100;
#endif

	int32_t itemDropBonus = std::min(100, pkKiller->GetPoint(POINT_ITEM_DROP_BONUS));

	iRandRange = 4000000;
	iRandRange = iRandRange * 100 / (100 + CPrivManager::instance().GetPriv(pkKiller, PRIV_ITEM_DROP) + bonus + itemDropBonus);

	return true;
}

bool ITEM_MANAGER::CreateDropItem(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item)
{
	int iLevel = pkKiller->GetLevel();

	int iDeltaPercent, iRandRange;
	if (!GetDropPct(pkChr, pkKiller, iDeltaPercent, iRandRange))
		return false;

	BYTE bRank = pkChr->GetMobRank();
	LPITEM item = nullptr;

	// Common Drop Items
	std::vector<CItemDropInfo>::iterator it = g_vec_pkCommonDropItem[bRank].begin();

	while (it != g_vec_pkCommonDropItem[bRank].end())
	{
		const CItemDropInfo& c_rInfo = *(it++);

		if (iLevel < c_rInfo.m_iLevelStart || iLevel > c_rInfo.m_iLevelEnd)
			continue;
#ifdef ENABLE_SUPER_METIN_EVENT
		if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
		{
			for (int x = 0; x < 3; ++x)
			{
				int iPercent = (c_rInfo.m_iPercent * iDeltaPercent) / 100;
				sys_log(3, "CreateDropItem %d ~ %d %d(%d)", c_rInfo.m_iLevelStart, c_rInfo.m_iLevelEnd, c_rInfo.m_dwVnum, iPercent, c_rInfo.m_iPercent);

				if (iPercent >= number(1, iRandRange))
				{
					TItemTable* table = GetTable(c_rInfo.m_dwVnum);

					if (!table)
						continue;

					item = nullptr;

					if (table->bType == ITEM_POLYMORPH)
					{
						if (c_rInfo.m_dwVnum == pkChr->GetPolymorphItemVnum())
						{
							item = CreateItem(c_rInfo.m_dwVnum, 1, 0, true);

							if (item)
								item->SetSocket(0, pkChr->GetRaceNum());
						}
					}
					else
						item = CreateItem(c_rInfo.m_dwVnum, 1, 0, true);

					if (item) vec_item.push_back(item);
				}
			}
		}
		else
#endif
		{
			int iPercent = (c_rInfo.m_iPercent * iDeltaPercent) / 100;
			sys_log(3, "CreateDropItem %d ~ %d %d(%d)", c_rInfo.m_iLevelStart, c_rInfo.m_iLevelEnd, c_rInfo.m_dwVnum, iPercent, c_rInfo.m_iPercent);

			if (iPercent >= number(1, iRandRange))
			{
				TItemTable* table = GetTable(c_rInfo.m_dwVnum);

				if (!table)
					continue;

				item = nullptr;

				if (table->bType == ITEM_POLYMORPH)
				{
					if (c_rInfo.m_dwVnum == pkChr->GetPolymorphItemVnum())
					{
						item = CreateItem(c_rInfo.m_dwVnum, 1, 0, true);

						if (item)
							item->SetSocket(0, pkChr->GetRaceNum());
					}
				}
				else
					item = CreateItem(c_rInfo.m_dwVnum, 1, 0, true);

				if (item) vec_item.push_back(item);
			}
		}
	}

	// Drop Item Group
	{
		auto it = m_map_pkDropItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkDropItemGroup.end())
		{
			decltype(it->second->GetVector()) v = it->second->GetVector();

			for (DWORD i = 0; i < v.size(); ++i)
			{
				int iPercent = (v[i].dwPct * iDeltaPercent) / 100;

#ifdef ENABLE_SUPER_METIN_EVENT
				if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
				{
					for (int x = 0; x < 3; ++x)
					{
						if (iPercent >= number(1, iRandRange))
						{
							item = CreateItem(v[i].dwVnum, v[i].iCount, 0, true);

							if (item)
							{
								if (item->GetType() == ITEM_POLYMORPH)
								{
									if (item->GetVnum() == pkChr->GetPolymorphItemVnum())
									{
										item->SetSocket(0, pkChr->GetRaceNum());
									}
								}

								vec_item.push_back(item);
							}
						}
					}
				}
				else
#endif
				{
					if (iPercent >= number(1, iRandRange))
					{
						item = CreateItem(v[i].dwVnum, v[i].iCount, 0, true);

						if (item)
						{
							if (item->GetType() == ITEM_POLYMORPH)
							{
								if (item->GetVnum() == pkChr->GetPolymorphItemVnum())
								{
									item->SetSocket(0, pkChr->GetRaceNum());
								}
							}

							vec_item.push_back(item);
						}
					}
				}
			}
		}
	}

	// MobDropItem Group
	{
		auto it = m_map_pkMobItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkMobItemGroup.end())
		{
			CMobItemGroup* pGroup = it->second;

			// MOB_DROP_ITEM_BUG_FIX
			// 20050805.myevan.MobDropItem ¿¡ ¾ÆÀÌÅÛÀÌ ¾øÀ» °æ¿ì CMobItemGroup::GetOne() Á¢±Ù½Ã ¹®Á¦ ¹ß»ý ¼öÁ¤
			if (pGroup && !pGroup->IsEmpty())
			{
#ifdef ENABLE_SUPER_METIN_EVENT
				if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
				{
					for (int x = 0; x < 3; ++x)
					{
						int iPercent = 40000 * iDeltaPercent / pGroup->GetKillPerDrop();
						if (iPercent >= number(1, iRandRange))
						{
							const CMobItemGroup::SMobItemGroupInfo& info = pGroup->GetOne();
							item = CreateItem(info.dwItemVnum, info.iCount, 0, true, info.iRarePct);

							if (item) vec_item.push_back(item);
						}
					}
				}
				else
#endif
				{
					int iPercent = 40000 * iDeltaPercent / pGroup->GetKillPerDrop();
					if (iPercent >= number(1, iRandRange))
					{
						const CMobItemGroup::SMobItemGroupInfo& info = pGroup->GetOne();
						item = CreateItem(info.dwItemVnum, info.iCount, 0, true, info.iRarePct);

						if (item) vec_item.push_back(item);
					}
				}
			}
			// END_OF_MOB_DROP_ITEM_BUG_FIX
		}
	}

	// Level Item Group
	{
		auto it = m_map_pkLevelItemGroup.find(pkChr->GetRaceNum());

		if (it != m_map_pkLevelItemGroup.end())
		{
			if (it->second->GetLevelLimit() <= (DWORD)iLevel)
			{
				decltype(it->second->GetVector()) v = it->second->GetVector();
				for (DWORD i = 0; i < v.size(); i++)
				{
#ifdef ENABLE_SUPER_METIN_EVENT
					if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
					{
						for (int x = 0; x < 3; ++x)
						{
							if (v[i].dwPct >= (DWORD)number(1, 1000000/*iRandRange*/))
							{
								DWORD dwVnum = v[i].dwVNum;
								item = CreateItem(dwVnum, v[i].iCount, 0, true);
								if (item) vec_item.push_back(item);
							}
						}
					}
					else
#endif
					{
						if (v[i].dwPct >= (DWORD)number(1, 1000000/*iRandRange*/))
						{
							DWORD dwVnum = v[i].dwVNum;
							item = CreateItem(dwVnum, v[i].iCount, 0, true);
							if (item) vec_item.push_back(item);
						}
					}
				}
			}
		}
	}

	// BuyerTheitGloves Item Group
	{
		if (pkKiller->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0 ||
			pkKiller->IsEquipUniqueGroup(UNIQUE_GROUP_DOUBLE_ITEM))
		{
			auto it = m_map_pkGloveItemGroup.find(pkChr->GetRaceNum());

			if (it != m_map_pkGloveItemGroup.end())
			{
				decltype(it->second->GetVector()) v = it->second->GetVector();

				for (DWORD i = 0; i < v.size(); ++i)
				{
#ifdef ENABLE_SUPER_METIN_EVENT
					if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
					{
						for (int x = 0; x < 3; ++x)
						{
							int iPercent = (v[i].dwPct * iDeltaPercent) / 100;

							if (iPercent >= number(1, iRandRange))
							{
								DWORD dwVnum = v[i].dwVnum;
								item = CreateItem(dwVnum, v[i].iCount, 0, true);
								if (item) vec_item.push_back(item);
							}
						}
					}
					else
#endif
					{
						int iPercent = (v[i].dwPct * iDeltaPercent) / 100;

						if (iPercent >= number(1, iRandRange))
						{
							DWORD dwVnum = v[i].dwVnum;
							item = CreateItem(dwVnum, v[i].iCount, 0, true);
							if (item) vec_item.push_back(item);
						}
					}
				}
			}
		}
	}

	// ÀâÅÛ
	if (pkChr->GetMobDropItemVnum())
	{
		auto it = m_map_dwEtcItemDropProb.find(pkChr->GetMobDropItemVnum());

		if (it != m_map_dwEtcItemDropProb.end())
		{
#ifdef ENABLE_SUPER_METIN_EVENT
			if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
			{
				for (int x = 0; x < 3; ++x)
				{
					int iPercent = (it->second * iDeltaPercent) / 100;

					if (iPercent >= number(1, iRandRange))
					{
						item = CreateItem(pkChr->GetMobDropItemVnum(), 1, 0, true);
						if (item) vec_item.push_back(item);
					}
				}
			}
			else
#endif
			{
				int iPercent = (it->second * iDeltaPercent) / 100;

				if (iPercent >= number(1, iRandRange))
				{
					item = CreateItem(pkChr->GetMobDropItemVnum(), 1, 0, true);
					if (item) vec_item.push_back(item);
				}
			}
		}
	}

	if (pkChr->IsStone())
	{
		if (pkChr->GetDropMetinStoneVnum())
		{
#ifdef ENABLE_SUPER_METIN_EVENT
			if (quest::CQuestManager::Instance().GetEventFlag("super_metin_event") == 1 && pkChr->IsStone())
			{
				for (int x = 0; x < 3; ++x)
				{
					int iPercent = (pkChr->GetDropMetinStonePct() * iDeltaPercent) * 400;

					if (iPercent >= number(1, iRandRange))
					{
						item = CreateItem(pkChr->GetDropMetinStoneVnum(), 1, 0, true);
						if (item) vec_item.push_back(item);
					}
				}
			}
			else
#endif
			{
				int iPercent = (pkChr->GetDropMetinStonePct() * iDeltaPercent) * 400;

				if (iPercent >= number(1, iRandRange))
				{
					item = CreateItem(pkChr->GetDropMetinStoneVnum(), 1, 0, true);
					if (item) vec_item.push_back(item);
				}
			}
		}
	}

	if (pkKiller->IsHorseRiding() &&
		GetDropPerKillPct(1000, 1000000, iDeltaPercent, "horse_skill_book_drop") >= number(1, iRandRange))
	{
		sys_log(0, "EVENT HORSE_SKILL_BOOK_DROP");

		if ((item = CreateItem(ITEM_HORSE_SKILL_TRAIN_BOOK, 1, 0, true)))
			vec_item.push_back(item);
	}

	//
	// ½ºÆä¼È µå·Ó ¾ÆÀÌÅÛ
	//
	CreateQuestDropItem(pkChr, pkKiller, vec_item, iDeltaPercent, iRandRange);

	for (auto it = vec_item.begin(); it != vec_item.end(); ++it)
	{
		LPITEM item = *it;
		DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, item->GetVnum(), item->GetCount());
	}

	return vec_item.size();
}

// ADD_GRANDMASTER_SKILL
int GetThreeSkillLevelAdjust(int level)
{
	if (level < 40)
		return 32;
	if (level < 45)
		return 16;
	if (level < 50)
		return 8;
	if (level < 55)
		return 4;
	if (level < 60)
		return 2;
	return 1;
}
// END_OF_ADD_GRANDMASTER_SKILL

// DROPEVENT_CHARSTONE
// drop_char_stone 1
// drop_char_stone.percent_lv01_10 5
// drop_char_stone.percent_lv11_30 10
// drop_char_stone.percent_lv31_MX 15
// drop_char_stone.level_range	   10
static struct DropEvent_CharStone
{
	int percent_lv01_10;
	int percent_lv11_30;
	int percent_lv31_MX;
	int level_range;
	bool alive;

	DropEvent_CharStone()
	{
		percent_lv01_10 = 100;
		percent_lv11_30 = 200;
		percent_lv31_MX = 300;
		level_range = 10;
		alive = false;
	}
} gs_dropEvent_charStone;

static int __DropEvent_CharStone_GetDropPercent(int killer_level)
{
	int killer_levelStep = (killer_level - 1) / 10;

	switch (killer_levelStep)
	{
	case 0:
		return gs_dropEvent_charStone.percent_lv01_10;

	case 1:
	case 2:
		return gs_dropEvent_charStone.percent_lv11_30;
	}

	return gs_dropEvent_charStone.percent_lv31_MX;
}

static void __DropEvent_CharStone_DropItem(CHARACTER & killer, CHARACTER & victim, ITEM_MANAGER & itemMgr, std::vector<LPITEM> & vec_item)
{
	if (!gs_dropEvent_charStone.alive)
		return;

	int killer_level = killer.GetLevel();
	int dropPercent = __DropEvent_CharStone_GetDropPercent(killer_level);

	int MaxRange = 10000;

	if (LC_IsCanada() == true)
		MaxRange = 20000;

	if (number(1, MaxRange) <= dropPercent)
	{
		int log_level = (test_server || killer.GetGMLevel() >= GM_LOW_WIZARD) ? 0 : 1;
		int victim_level = victim.GetLevel();
		int level_diff = victim_level - killer_level;

		if (level_diff >= +gs_dropEvent_charStone.level_range || level_diff <= -gs_dropEvent_charStone.level_range)
		{
			sys_log(log_level,
				"dropevent.drop_char_stone.level_range_over: killer(%s: lv%d), victim(%s: lv:%d), level_diff(%d)",
				killer.GetName(), killer.GetLevel(), victim.GetName(), victim.GetLevel(), level_diff);
			return;
		}

		static const int Stones[] = { 30210, 30211, 30212, 30213, 30214, 30215, 30216, 30217, 30218, 30219, 30258, 30259, 30260, 30261, 30262, 30263 };
		int item_vnum = Stones[number(0, _countof(Stones))];

		LPITEM p_item = nullptr;

		if ((p_item = itemMgr.CreateItem(item_vnum, 1, 0, true)))
		{
			vec_item.push_back(p_item);

			sys_log(log_level,
				"dropevent.drop_char_stone.item_drop: killer(%s: lv%d), victim(%s: lv:%d), item_name(%s)",
				killer.GetName(), killer.GetLevel(), victim.GetName(), victim.GetLevel(), p_item->GetName());
		}
	}
}

bool DropEvent_CharStone_SetValue(const std::string & name, int value)
{
	if (name == "drop_char_stone")
	{
		gs_dropEvent_charStone.alive = value;

		if (value)
			sys_log(0, "dropevent.drop_char_stone = on");
		else
			sys_log(0, "dropevent.drop_char_stone = off");
	}
	else if (name == "drop_char_stone.percent_lv01_10")
		gs_dropEvent_charStone.percent_lv01_10 = value;
	else if (name == "drop_char_stone.percent_lv11_30")
		gs_dropEvent_charStone.percent_lv11_30 = value;
	else if (name == "drop_char_stone.percent_lv31_MX")
		gs_dropEvent_charStone.percent_lv31_MX = value;
	else if (name == "drop_char_stone.level_range")
		gs_dropEvent_charStone.level_range = value;
	else
		return false;

	sys_log(0, "dropevent.drop_char_stone: %d", gs_dropEvent_charStone.alive ? true : false);
	sys_log(0, "dropevent.drop_char_stone.percent_lv01_10: %f", gs_dropEvent_charStone.percent_lv01_10 / 100.0f);
	sys_log(0, "dropevent.drop_char_stone.percent_lv11_30: %f", gs_dropEvent_charStone.percent_lv11_30 / 100.0f);
	sys_log(0, "dropevent.drop_char_stone.percent_lv31_MX: %f", gs_dropEvent_charStone.percent_lv31_MX / 100.0f);
	sys_log(0, "dropevent.drop_char_stone.level_range: %d", gs_dropEvent_charStone.level_range);

	return true;
}

// END_OF_DROPEVENT_CHARSTONE

// fixme
// À§ÀÇ °Í°ú ÇÔ²² quest·Î »¬°Í »©º¸ÀÚ.
// ÀÌ°Å ³Ê¹« ´õ·´ÀÝ¾Æ...
// ”?. ÇÏµåÄÚµù ½È´Ù ¤Ì¤Ð
// °è·® ¾ÆÀÌÅÛ º¸»ó ½ÃÀÛ.
// by rtsummit °íÄ¡ÀÚ ÁøÂ¥
static struct DropEvent_RefineBox
{
	int percent_low;
	int low;
	int percent_mid;
	int mid;
	int percent_high;
	//int level_range;
	bool alive;

	DropEvent_RefineBox()
	{
		percent_low = 100;
		low = 20;
		percent_mid = 100;
		mid = 45;
		percent_high = 100;
		//level_range = 10;
		alive = false;
	}
} gs_dropEvent_refineBox;

static LPITEM __DropEvent_RefineBox_GetDropItem(CHARACTER & killer, CHARACTER & victim, ITEM_MANAGER & itemMgr)
{
	static const int lowerBox[] = { 50197, 50198, 50199 };
	static const int lowerBox_range = 3;
	static const int midderBox[] = { 50203, 50204, 50205, 50206 };
	static const int midderBox_range = 4;
	static const int higherBox[] = { 50207, 50208, 50209, 50210, 50211 };
	static const int higherBox_range = 5;

	if (victim.GetMobRank() < MOB_RANK_KNIGHT)
		return nullptr;

	int killer_level = killer.GetLevel();
	//int level_diff = victim_level - killer_level;

	//if (level_diff >= +gs_dropEvent_refineBox.level_range || level_diff <= -gs_dropEvent_refineBox.level_range)
	//{
	//	sys_log(log_level,
	//		"dropevent.drop_refine_box.level_range_over: killer(%s: lv%d), victim(%s: lv:%d), level_diff(%d)",
	//		killer.GetName(), killer.GetLevel(), victim.GetName(), victim.GetLevel(), level_diff);
	//	return nullptr;
	//}

	if (killer_level <= gs_dropEvent_refineBox.low)
	{
		if (number(1, gs_dropEvent_refineBox.percent_low) == 1)
		{
			return itemMgr.CreateItem(lowerBox[number(1, lowerBox_range) - 1], 1, 0, true);
		}
	}
	else if (killer_level <= gs_dropEvent_refineBox.mid)
	{
		if (number(1, gs_dropEvent_refineBox.percent_mid) == 1)
		{
			return itemMgr.CreateItem(midderBox[number(1, midderBox_range) - 1], 1, 0, true);
		}
	}
	else
	{
		if (number(1, gs_dropEvent_refineBox.percent_high) == 1)
		{
			return itemMgr.CreateItem(higherBox[number(1, higherBox_range) - 1], 1, 0, true);
		}
	}
	return nullptr;
}

static void __DropEvent_RefineBox_DropItem(CHARACTER & killer, CHARACTER & victim, ITEM_MANAGER & itemMgr, std::vector<LPITEM> & vec_item)
{
	if (!gs_dropEvent_refineBox.alive)
		return;

	int log_level = (test_server || killer.GetGMLevel() >= GM_LOW_WIZARD) ? 0 : 1;

	LPITEM p_item = __DropEvent_RefineBox_GetDropItem(killer, victim, itemMgr);

	if (p_item)
	{
		vec_item.push_back(p_item);

		sys_log(log_level,
			"dropevent.drop_refine_box.item_drop: killer(%s: lv%d), victim(%s: lv:%d), item_name(%s)",
			killer.GetName(), killer.GetLevel(), victim.GetName(), victim.GetLevel(), p_item->GetName());
	}
}

bool DropEvent_RefineBox_SetValue(const std::string & name, int value)
{
	if (name == "refine_box_drop")
	{
		gs_dropEvent_refineBox.alive = value;

		if (value)
			sys_log(0, "refine_box_drop = on");
		else
			sys_log(0, "refine_box_drop = off");
	}
	else if (name == "refine_box_low")
		gs_dropEvent_refineBox.percent_low = value < 100 ? 100 : value;
	else if (name == "refine_box_mid")
		gs_dropEvent_refineBox.percent_mid = value < 100 ? 100 : value;
	else if (name == "refine_box_high")
		gs_dropEvent_refineBox.percent_high = value < 100 ? 100 : value;
	//else if (name == "refine_box_level_range")
	//	gs_dropEvent_refineBox.level_range = value;
	else
		return false;

	sys_log(0, "refine_box_drop: %d", gs_dropEvent_refineBox.alive ? true : false);
	sys_log(0, "refine_box_low: %d", gs_dropEvent_refineBox.percent_low);
	sys_log(0, "refine_box_mid: %d", gs_dropEvent_refineBox.percent_mid);
	sys_log(0, "refine_box_high: %d", gs_dropEvent_refineBox.percent_high);
	//sys_log(0, "refine_box_low_level_range: %d", gs_dropEvent_refineBox.level_range);

	return true;
}
// °³·® ¾ÆÀÌÅÛ º¸»ó ³¡.

void ITEM_MANAGER::CreateQuestDropItem(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item, int iDeltaPercent, int iRandRange)
{
	LPITEM item = nullptr;

	if (!pkChr)
		return;

	if (!pkKiller)
		return;

	sys_log(1, "CreateQuestDropItem victim(%s), killer(%s)", pkChr->GetName(), pkKiller->GetName());

	// DROPEVENT_CHARSTONE
	__DropEvent_CharStone_DropItem(*pkKiller, *pkChr, *this, vec_item);
	// END_OF_DROPEVENT_CHARSTONE
	__DropEvent_RefineBox_DropItem(*pkKiller, *pkChr, *this, vec_item);
	
#ifdef ENABLE_EVENT_SYSTEM
	#ifdef ENABLE_MOON_DROP_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_moon_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1450;
		int prob = 10;
		
		const DWORD ex_moon_drop_vnum = 50011;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_moon_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_HEXAGON_DROP_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_hexagon_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 450;
		int prob = 10;
		
		const DWORD ex_hexagon_drop_vnum = 50037;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_hexagon_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_CARVING_STONE_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_carving_stone_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 550;
		int prob = 10;
		
		const DWORD ex_carving_stone_drop_vnum = 30178;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_carving_stone_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_RAMADAN_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_ramadan_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1450;
		int prob = 10;
		
		const DWORD ex_ramadan_drop_vnum = 30315;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_ramadan_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_XMAS_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_xmas_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1450;
		int prob = 10;
		
		const DWORD ex_xmas_drop_vnum = 50010;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			CPetSystem* petSystem = pkKiller->GetPetSystem();
			if (petSystem)
			{
				CPetActor * petActor = petSystem->GetByVnum(53002);
				if (petActor)
				{
					prob += 10;
				}
			}
			
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_xmas_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_FOOTBALL_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_football_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1450;
		int prob = 10;
		
		const DWORD ex_football_drop_vnum = 50096;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_football_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_VALENTINE_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_valentine_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 200;
		int prob = 10;
				
		if (pkChr->IsStone())
		{
			if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
			{
				if (number(1, randomnumberlimit) <= prob)
				{
					if (GET_SEX(pkKiller) == 0)
					{
						if ((item = CreateItem(71147, 1, 0, true)))
							vec_item.push_back(item);
					}
					else
					{
						if ((item = CreateItem(71146, 1, 0, true)))
							vec_item.push_back(item);
					}
				}
			}
		}
	}
	#endif
	
	#ifdef ENABLE_PET_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_pet_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 600;
		int prob = 10;
				
		if (pkChr->IsStone())
		{
			if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
			{
				if (number(1, randomnumberlimit) <= prob)
				{
					if ((item = CreateItem(53001, 1, 0, true)))
						vec_item.push_back(item);
				}
			}
		}
	}
	#endif
	
	#ifdef ENABLE_KIDS_DAY_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_kids_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1400;
		int prob = 10;
		
		const DWORD ex_kids_drop_vnum = 50034;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_kids_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_MOUNT_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_mount_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 600;
		int prob = 10;
		
		int mount_vnums[] = {52701,52702,52703,52704,52705,52706,52707,52708};
				
		if (pkChr->IsStone())
		{
			if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
			{
				if (number(1, randomnumberlimit) <= prob)
				{
					int i = number(0,7);
					if ((item = CreateItem(mount_vnums[i], 1, 0, true)))
						vec_item.push_back(item);
				}
			}
		}
	}
	#endif

	#ifdef ENABLE_HALLOWEEN_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_halloween_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 1450;
		int prob = 10;
		
		const DWORD ex_halloween_drop_vnum = 30336;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_halloween_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_OLYMPIC_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("ex_olympic_drop"))
	{
		int levelrange = 10;
		int randomnumberlimit = 3700;
		int prob = 10;
		
		int olympic_vnums[] = {30265,30266,30267,30268,30269};

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				int i = number(0,4);
		
				if ((item = CreateItem(olympic_vnums[i], 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_MINIGAME_RUMI_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("cards_event"))
	{
		int levelrange = 10;
		int randomnumberlimit = 800;
		int prob = 10;
		
		const DWORD ex_cards_drop_vnum = 79505;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_cards_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_MINI_GAME_CATCH_KING
	if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event"))
	{
		int levelrange = 10;
		int randomnumberlimit = 800;
		int prob = 10;
		
		const DWORD ex_cking_drop_vnum = 79603;

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if ((item = CreateItem(ex_cking_drop_vnum, 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_MAKE_WONSO_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("newyear_wonso"))
	{
		int levelrange = 10;
		int randomnumberlimit = 900;
		int prob = 10;
		
		int wonso_vnums[] = {50016,50019};

		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				int i = number(0,1);
		
				if ((item = CreateItem(wonso_vnums[i], 1, 0, true)))
					vec_item.push_back(item);
			}
		}
	}
	#endif
	
	#ifdef ENABLE_LUCKYBOX_EVENT
	if (quest::CQuestManager::instance().GetEventFlag("lucky_box_event"))
	{
		int levelrange = 10;
		int randomnumberlimit = 100;
		int prob = 25;
		
		int luckybox_vnum = 0;
				
		if (pkChr->GetRaceNum() == 1093)
			luckybox_vnum = 72734;
		else if (pkChr->GetRaceNum() == 2598)
			luckybox_vnum = 72734;
		else if (pkChr->GetRaceNum() == 6091)
			luckybox_vnum = 72734;
		else if (pkChr->GetRaceNum() == 6191)
			luckybox_vnum = 72734;
		else if (pkChr->GetRaceNum() == 2493)
			luckybox_vnum = 72734;
			
		if (pkChr->GetLevel() >= (pkKiller->GetLevel() - levelrange))
		{
			if (number(1, randomnumberlimit) <= prob)
			{
				if (luckybox_vnum != 0)
				{
					if ((item = CreateItem(luckybox_vnum, 1, 0, true)))
						vec_item.push_back(item);
				}
			}
		}
	}
	#endif
#endif
}

DWORD ITEM_MANAGER::GetRefineFromVnum(DWORD dwVnum)
{
	auto it = m_map_ItemRefineFrom.find(dwVnum);
	if (it != m_map_ItemRefineFrom.end())
		return it->second;
	return 0;
}

const CSpecialItemGroup* ITEM_MANAGER::GetSpecialItemGroup(DWORD dwVnum)
{
	auto it = m_map_pkSpecialItemGroup.find(dwVnum);
	if (it != m_map_pkSpecialItemGroup.end())
	{
		return it->second;
	}
	return nullptr;
}

const CSpecialAttrGroup* ITEM_MANAGER::GetSpecialAttrGroup(DWORD dwVnum)
{
	auto it = m_map_pkSpecialAttrGroup.find(dwVnum);
	if (it != m_map_pkSpecialAttrGroup.end())
	{
		return it->second;
	}
	return nullptr;
}

DWORD ITEM_MANAGER::GetMaskVnum(DWORD dwVnum)
{
	TMapDW2DW::iterator it = m_map_new_to_ori.find(dwVnum);
	if (it != m_map_new_to_ori.end())
	{
		return it->second;
	}
	else
		return 0;
}

// pkNewItemÀ¸·Î ¸ðµç ¼Ó¼º°ú ¼ÒÄÏ °ªµéÀ» ¸ñ»çÇÏ´Â ÇÔ¼ö.
// ±âÁ¸¿¡ char_item.cpp ÆÄÀÏ¿¡ ÀÖ´ø ·ÎÄÃÇÔ¼öÀÎ TransformRefineItem ±×´ë·Î º¹»çÇÔ
void ITEM_MANAGER::CopyAllAttrTo(LPITEM pkOldItem, LPITEM pkNewItem)
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
			const int ITEM_BROKEN_METIN_VNUM = 28960; // ÀÌ°Ç ¹¹ ¶È°°Àº »ó¼ö°¡ 3±ºµ¥³ª ÀÖ³Ä... ÇÏ³ª·Î ÇØ³õÁö¤Ð¤Ð¤Ð ³ª´Â ÆÐ½º È«ÀÌ ÇÒ²¨ÀÓ
			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				pkNewItem->SetSocket(slot++, socket);
		}
	}

	// ¸ÅÁ÷ ¾ÆÀÌÅÛ ¼³Á¤
	pkOldItem->CopyAttributeTo(pkNewItem);
	pkNewItem->SetTransmutation(pkOldItem->GetTransmutation(), false);
}

#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
#include "../../libgame/libgame/grid.h"
void ITEM_MANAGER::GetChestItemList(DWORD dwChestVnum, std::vector<TChestDropInfoTable> & vec_item)
{
	TChestDropInfoTable kTempTab;

	BYTE dwChestDropPageCount = 5;

	CGrid* pGrids[dwChestDropPageCount];

	for (int i = 0; i < dwChestDropPageCount; ++i)
	{
		pGrids[i] = new CGrid(5, 8);
		pGrids[i]->Clear();
	}

	const CSpecialItemGroup* pGroup = GetSpecialItemGroup(dwChestVnum);

	if (pGroup)
	{
		for (int i = 0; i < pGroup->GetGroupSize(); i++)
		{
			const TItemTable* itemTable = GetTable(pGroup->GetVnum(i));

			if (itemTable != nullptr)
			{
				for (int iPage = 0; iPage < dwChestDropPageCount; ++iPage)
				{
					int iPos = pGrids[iPage]->FindBlank(1, itemTable->bSize);

					if (iPos >= 0)
					{
						pGrids[iPage]->Put(iPos, 1, itemTable->bSize);

						kTempTab.bPageIndex = iPage + 1;
						kTempTab.bSlotIndex = iPos;

						kTempTab.bItemCount = pGroup->GetCount(i);
						kTempTab.dwItemVnum = pGroup->GetVnum(i);

						vec_item.push_back(kTempTab);

						break;
					}
				}
			}
		}
	}
}
#endif