#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "utils.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "protocol.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "cmd.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "battle.h"
#include "exchange.h"
#include "questmanager.h"
#include "profiler.h"
#include "messenger_manager.h"
#include "party.h"
#include "p2p.h"
#include "affect.h"
#include "guild.h"
#include "guild_manager.h"
#include "log.h"
#include "banword.h"
#include "unique_item.h"
#include "building.h"
#include "locale_service.h"
#include "gm.h"
#include "ani.h"
#include "motion.h"
#include "OXEvent.h"
#include "locale_service.h"
#include "DragonSoul.h"
#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#include "../../common/VnumHelper.h"
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
#include "minigame.h"
#endif
#include "features.h"
#include "war_map.h"
#include "refine.h"

extern void SendShout(const char* szText, BYTE bEmpire);
extern int g_nPortalLimitTime;
extern bool glass_enable;
extern bool glass_needed;
extern int g_iShoutLimitLevel;
extern int g_iShoutLimitValue;

static int __deposit_limit()
{
	return (1000 * 10000); // 1Ãµ¸¸
}

#ifdef ENABLE_CHAT_LOGGING
static char	__escape_string[1024];
//static char	__escape_string2[1024];
#endif

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
#include "entity.h"
#include <unordered_map>
bool CompareItemVnumAcPriceAC(CShop::SHOP_ITEM i, CShop::SHOP_ITEM j)
{
	return (i.vnum < j.vnum) && (i.price < j.price);
}
#include "entity.h"
void CInputMain::ShopSearch(LPCHARACTER ch, const char* data, bool bName)
{
	TPacketCGShopSearch* pinfo = (TPacketCGShopSearch*)data;

	if (!ch)
		return;

	if (!data)
		return;
	
	if (!features::IsFeatureEnabled(features::PRIVATESHOP_SEARCH))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;		
	}

	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CANT_DO_THIS_BECAUSE_OTHER_WINDOW_OPEN"));
		return;
	}

	if (0 == quest::CQuestManager::instance().GetEventFlag("enable_shop_search"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;
	}

#ifndef DISABLE_PRIVATE_SHOP_SEARCH_NEED_ITEM
	if (!ch->FindSpecifyItem(60004) && !ch->FindSpecifyItem(60005) && !ch->FindSpecifyItem(87009))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_NEED_ITEM"));
		return;
	}
#endif

	int32_t Race = pinfo->Race;
	int32_t Cat = pinfo->ItemCat;
	int32_t SubCat = pinfo->SubCat;
	int32_t MinLevel = pinfo->MinLevel;
	int32_t MaxLevel = pinfo->MaxLevel;
	int32_t MinRefine = pinfo->MinRefine;
	int32_t MaxRefine = pinfo->MaxRefine;
	uint64_t MinGold = pinfo->MinGold;
	uint64_t MaxGold = pinfo->MaxGold;
	uint64_t MinCheque = pinfo->MinCheque;
	uint64_t MaxCheque = pinfo->MaxCheque;
	std::string itemName = "";

	//Checks
	if (Race < JOB_WARRIOR || Race > JOB_WOLFMAN)
		return;
	if (Cat < ITEM_NONE || Cat > ITEM_SOUL)
		return;
	if (SubCat < USE_POTION || SubCat > USE_RESET_COSTUME_ATTR)
		return;
	if (MinLevel < 0 || MinLevel > PLAYER_MAX_LEVEL_CONST)
		return;
	if (MaxLevel < 0 || MaxLevel > PLAYER_MAX_LEVEL_CONST)
		return;
	if (MinRefine < 0 || MinRefine > 9)
		return;
	if (MaxRefine < 0 || MaxRefine > 9)
		return;
	if (MinGold < 0 || MinGold > GOLD_MAX)
		return;
	if (MaxGold < 0 || MaxGold > GOLD_MAX)
		return;
	if (MinCheque < 0 || MinCheque > 999)
		return;
	if (MaxCheque < 0 || MaxCheque > 999)
		return;
	if (MinLevel > MaxLevel)
		return;
	if (MinRefine > MaxRefine)
		return;
	if (MinGold > MaxGold)
		return;
	if (MinCheque > MaxCheque)
		return;

	if (bName)
	{
		itemName = pinfo->ItemName;
		std::replace(itemName.begin(), itemName.end(), '_', ' ');
	}

	quest::PC* pPC = quest::CQuestManager::instance().GetPC(ch->GetPlayerID());

	if (!pPC)
		return;

	DWORD dwShopSearchSecCycle = 2; // 1 sec
	DWORD dwNowSec = get_global_time();
	DWORD dwLastShopSearchAttrSec = pPC->GetFlag("ShopSearch.LastShopSearchSecAttr");

	if (dwLastShopSearchAttrSec + dwShopSearchSecCycle > dwNowSec)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_NEED_WAIT_%d_LEFT_(%d)"),
			dwShopSearchSecCycle, dwShopSearchSecCycle - (dwNowSec - dwLastShopSearchAttrSec));
		return;
	}

	pPC->SetFlag("ShopSearch.LastShopSearchSecAttr", dwNowSec);

	if (bName)
		sys_log(0, "SHOP_SEARCH: CharName: %s Search: Race: %d Cat: %d SubCat: %d MinLevel: %d MaxLevel: %d MinRefine: %d MaxRefine: %d MinGold: %llu MaxGold: %llu itemName: %s", ch->GetName(), Race, Cat, SubCat, MinLevel, MaxLevel, MinRefine, MaxRefine, MinGold, MaxGold, itemName.c_str());
	else
		sys_log(0, "SHOP_SEARCH: CharName: %s Search: Race: %d Cat: %d SubCat: %d MinLevel: %d MaxLevel: %d MinRefine: %d MaxRefine: %d MinGold: %llu MaxGold: %llu", ch->GetName(), Race, Cat, SubCat, MinLevel, MaxLevel, MinRefine, MaxRefine, MinGold, MaxGold);

	//CEntity::ENTITY_MAP viewmap = ch->GetViewMap();
	//CEntity::ENTITY_MAP::iterator it = viewmap.begin();
	CShopManager::TShopMap viewmap = CShopManager::instance().GetShopMap();
	CShopManager::TShopMap::iterator it = viewmap.begin();
	std::vector<CShop::SHOP_ITEM> sendItems;

	while (it != viewmap.end())
	{
		if (it->second->GetPC() == nullptr) {
			++it;
			continue;
		}
		
		LPCHARACTER tch = static_cast<LPCHARACTER>((it++)->second->GetPC());

		if (!tch)
			continue;

		if (!tch->GetMyShop() || tch == ch)
			continue;

		std::vector<CShop::SHOP_ITEM> shop_items = tch->GetMyShop()->GetItemVector();
		std::vector<CShop::SHOP_ITEM>::iterator item;

		for (item = shop_items.begin(); item != shop_items.end(); ++item)
		{
			if (bName)
			{
				LPITEM curItem = item->pkItem;

				if (!curItem)
					continue;

				if (curItem && itemName.length() > 2)
				{
					if (!((curItem->GetLevelLimit() >= MinLevel && curItem->GetLevelLimit() <= MaxLevel) || curItem->GetLevelLimit() == 0))
						continue;

					if (!(item->price >= MinGold && item->price <= MaxGold))
						continue;

					if (!(item->price_cheque >= MinCheque && item->price_cheque <= MaxCheque))
						continue;

					bool pushback = false;

					std::string foundName = curItem->GetNameString();
					if (foundName.find(itemName) != std::string::npos) {
						pushback = true;
					}

					if (pushback)
					{
						sendItems.push_back(*item);
					}
				}
			}
			else
			{
				LPITEM curItem = item->pkItem;
				if (!curItem)
					continue;

				if (curItem && curItem->GetType() == Cat && curItem->GetSubType() == SubCat)
				{
					if ((curItem->GetType() == ITEM_WEAPON || curItem->GetType() == ITEM_ARMOR) && !(curItem->GetRefineLevel() >= MinRefine && curItem->GetRefineLevel() <= MaxRefine))
						continue;

					if (!((curItem->GetLevelLimit() >= MinLevel && curItem->GetLevelLimit() <= MaxLevel) || curItem->GetLevelLimit() == 0))
						continue;

					if (!(item->price >= MinGold && item->price <= MaxGold))
						continue;

					if (!(item->price_cheque >= MinCheque && item->price_cheque <= MaxCheque))
						continue;

					bool cont = false;
					switch (Race)
					{
					case JOB_WARRIOR:
						if (curItem->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
							cont = true;
						break;

					case JOB_ASSASSIN:
						if (curItem->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
							cont = true;
						break;

					case JOB_SHAMAN:
						if (curItem->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
							cont = true;
						break;

					case JOB_SURA:
						if (curItem->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
							cont = true;
						break;

					case JOB_WOLFMAN:
						if (curItem->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
							cont = true;
						break;
					}

					if (cont)
						continue;

					sendItems.push_back(*item);
				}
			}
		}
	}

	std::stable_sort(sendItems.begin(), sendItems.end(), CompareItemVnumAcPriceAC);
	std::vector<CShop::SHOP_ITEM>::iterator item;

	for (item = sendItems.begin(); item != sendItems.end(); ++item)
	{
		LPITEM curItem = item->pkItem;

		if (curItem)
		{
			LPDESC d = ch->GetDesc();

			if (!d)
				return;

			TPacketGCShopSearchItemSet pack;
			pack.header = HEADER_GC_SHOPSEARCH_SET;

			pack.count = static_cast<BYTE>(item->count);
			pack.vnum = curItem->GetVnum();
			pack.flags = curItem->GetFlag();
			pack.anti_flags = curItem->GetAntiFlag();
			pack.price = item->price;
#ifdef ENABLE_CHEQUE_SYSTEM
			pack.price2 = item->price_cheque;
#endif
			pack.vid = curItem->GetOwner()->GetVID();
#ifdef ENABLE_CHANGELOOK_SYSTEM
			pack.transmutation = curItem->GetTransmutation();
#endif
			pack.Cell = item->pos;
			strlcpy(pack.name, curItem->GetOwner()->GetName(), sizeof(pack.name));

			thecore_memcpy(pack.alSockets, curItem->GetSockets(), sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, curItem->GetAttributes(), sizeof(pack.aAttr));

			d->LargePacket(&pack, sizeof(TPacketGCShopSearchItemSet));
		}
	}
}

void CInputMain::ShopSearchBuy(LPCHARACTER ch, const char* data)
{
	TPacketCGShopSearchBuy* pinfo = (TPacketCGShopSearchBuy*)data;

	int32_t shopVid = pinfo->shopVid;
	int32_t shopItemPos = pinfo->shopItemPos;

	if (!ch)
		return;

	if (!features::IsFeatureEnabled(features::PRIVATESHOP_SEARCH))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;		
	}

	if (0 == quest::CQuestManager::instance().GetEventFlag("enable_shop_search"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;
	}

	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CANT_DO_THIS_BECAUSE_OTHER_WINDOW_OPEN"));
		return;
	}

#ifndef DISABLE_PRIVATE_SHOP_SEARCH_NEED_ITEM
	if (!ch->FindSpecifyItem(60005) && !ch->FindSpecifyItem(60006) && !ch->FindSpecifyItem(87009))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_NEED_ITEM"));
		return;
	}
#endif

	LPCHARACTER pkChrShop = CHARACTER_MANAGER::instance().Find(shopVid);

	if (!pkChrShop)
		return;

	LPSHOP pkShop = pkChrShop->GetMyShop();

	if (!pkShop)
		return;
	if (pkShop->IsLocked())
	{
		sys_err("Player %lu trying to buy from closed shop.", ch->GetPlayerID());
		return;
	}

	// if (DISTANCE_APPROX(ch->GetX() - pkChrShop->GetX(), ch->GetY() - pkChrShop->GetY()) > VIEW_RANGE)
	// {
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÁ¡°úÀÇ °Å¸®°¡ ³Ê¹« ¸Ö¾î ¹°°ÇÀ» »ì ¼ö ¾ø½À´Ï´Ù."));
		// return;
	// }

	// if (ch->GetMapIndex() != pkChrShop->GetMapIndex())
	// {
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_CANNOT_BUY_THIS_BECAUSE_NOT_IN_SAME_MAP"));
		// return;
	// }

	int32_t returnHeader = pkShop->Buy(ch, shopItemPos, true);

	if (SHOP_SUBHEADER_GC_OK == returnHeader)
	{
		// being lazy starting in 3 2 ..
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShopSearchBuy");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShopSearchError %d", returnHeader);
	}

	//If, after buying, the shop is closed, destroy it (through its owner char)
	/*if (pkShop->IsClosed() && pkShop->IsPCShop())
	{
		M2_DESTROY_CHARACTER(pkShop->GetOwner());
	}*/
}
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
void CInputMain::ChangeLook(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC* pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;

	if (!features::IsFeatureEnabled(features::CHANGELOOK))
	{
		LogManager::instance().HackLog("CHANGELOOKDISABLED", pkChar);
		return;
	}

	TPacketChangeLook* sPacket = (TPacketChangeLook*)c_pData;
	switch (sPacket->subheader)
	{
	case CL_SUBHEADER_CLOSE:
	{
		pkChar->ChangeLookWindow(false);
	}
	break;
	case CL_SUBHEADER_ADD:
	{
		pkChar->AddClMaterial(sPacket->tPos, sPacket->bPos);
	}
	break;
	case CL_SUBHEADER_REMOVE:
	{
		pkChar->RemoveClMaterial(sPacket->bPos);
	}
	break;
	case CL_SUBHEADER_REFINE:
	{
		pkChar->RefineClMaterials();
	}
	break;
	default:
		break;
	}
}
#endif

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
void CInputMain::TargetInfoLoad(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGTargetInfoLoad* p = (TPacketCGTargetInfoLoad*)c_pData;
	TPacketGCTargetInfo pInfo;
	pInfo.header = HEADER_GC_TARGET_INFO;
	static std::vector<LPITEM> s_vec_item;
	s_vec_item.clear();
	LPITEM pkInfoItem;
	LPCHARACTER m_pkChrTarget = CHARACTER_MANAGER::instance().Find(p->dwVID);

	if (!ch || !m_pkChrTarget)
		return;

	if (!ch->GetDesc())
		return;

	if (ITEM_MANAGER::instance().CreateDropItemVector(m_pkChrTarget, ch, s_vec_item) && (m_pkChrTarget->IsMonster() || m_pkChrTarget->IsStone()))
	{
		if (s_vec_item.size() == 0);
		else if (s_vec_item.size() == 1)
		{
			pkInfoItem = s_vec_item[0];
			pInfo.dwVID = m_pkChrTarget->GetVID();
			pInfo.race = m_pkChrTarget->GetRaceNum();
			pInfo.dwVnum = pkInfoItem->GetVnum();
			pInfo.count = pkInfoItem->GetCount();
			ch->GetDesc()->Packet(&pInfo, sizeof(TPacketGCTargetInfo));
		}
		else
		{
			int iItemIdx = s_vec_item.size() - 1;
			while (iItemIdx >= 0)
			{
				pkInfoItem = s_vec_item[iItemIdx--];

				if (!pkInfoItem)
				{
					sys_err("pkInfoItem nullptr in vector idx %d", iItemIdx + 1);
					continue;
				}

				pInfo.dwVID = m_pkChrTarget->GetVID();
				pInfo.race = m_pkChrTarget->GetRaceNum();
				pInfo.dwVnum = pkInfoItem->GetVnum();
				pInfo.count = pkInfoItem->GetCount();
				ch->GetDesc()->Packet(&pInfo, sizeof(TPacketGCTargetInfo));
			}
		}
	}
	// }
}
#endif

void SendBlockChatInfo(LPCHARACTER ch, int sec)
{
	if (sec <= 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¤ÆÃ ±ÝÁö »óÅÂÀÔ´Ï´Ù."));
		return;
	}

	long hour = sec / 3600;
	sec -= hour * 3600;

	long min = (sec / 60);
	sec -= min * 60;

	char buf[128 + 1];

	if (hour > 0 && min > 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ½Ã°£ %d ºÐ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), hour, min, sec);
	else if (hour > 0 && min == 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ½Ã°£ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), hour, sec);
	else if (hour == 0 && min > 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ºÐ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), min, sec);
	else
		snprintf(buf, sizeof(buf), LC_TEXT("%d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), sec);

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

enum ETextTag
{
	TEXT_TAG_PLAIN,
	TEXT_TAG_TAG, // ||
	TEXT_TAG_COLOR, // |cffffffff
	TEXT_TAG_HYPERLINK_START, // |H
	TEXT_TAG_HYPERLINK_END, // |h ex) |Hitem:1234:1:1:1|h
};

int GetTextTag(const char* src, int maxLen, int& tagLen, std::string & extraInfo)
{
	tagLen = 1;

	if (maxLen < 2 || *src != '|')
		return TEXT_TAG_PLAIN;

	const char* cur = ++src;

	if (*cur == '|') // ||´Â |·Î Ç¥½ÃÇÑ´Ù.
	{
		tagLen = 2;
		return TEXT_TAG_TAG;
	}
	else if (*cur == 'c') // color |cffffffffblahblah|r
	{
		tagLen = 2;
		return TEXT_TAG_COLOR;
	}
	else if (*cur == 'H') // hyperlink |Hitem:10000:0:0:0:0|h[ÀÌ¸§]|h
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_START;
	}
	else if (*cur == 'h') // end of hyperlink
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_END;
	}

	return TEXT_TAG_PLAIN;
}

void GetTextTagInfo(const char* src, int src_len, int& hyperlinks, bool& colored)
{
	colored = false;
	hyperlinks = 0;

	int len;
	std::string extraInfo;

	for (int i = 0; i < src_len;)
	{
		int tag = GetTextTag(&src[i], src_len - i, len, extraInfo);

		if (tag == TEXT_TAG_HYPERLINK_START)
			++hyperlinks;

		if (tag == TEXT_TAG_COLOR)
			colored = true;

		i += len;
	}
}

int ProcessTextTag(LPCHARACTER ch, const char* c_pszText, size_t len)
{
	int hyperlinks;
	bool colored;

	GetTextTagInfo(c_pszText, len, hyperlinks, colored);

	if (colored == true && hyperlinks == 0)
		return 4;

	if (ch->GetExchange())
	{
		if (hyperlinks == 0)
			return 0;
		else
			return 3;
	}
	
	if(!glass_enable)
		return 0;

	int nPrismCount = ch->CountSpecifyItem(ITEM_PRISM);

	if (nPrismCount < hyperlinks && glass_needed)
		return 1;

	if (!ch->GetMyShop())
	{
		if(glass_needed)
			ch->RemoveSpecifyItem(ITEM_PRISM, hyperlinks);
		return 0;
	}
	else
	{
		int sellingNumber = ch->GetMyShop()->GetNumberByVnum(ITEM_PRISM);
		if (nPrismCount - sellingNumber < hyperlinks)
		{
			return 2;
		}
		else
		{
			if(glass_needed)
				ch->RemoveSpecifyItem(ITEM_PRISM, hyperlinks);
			return 0;
		}
	}

	return 4;
}

int CInputMain::Whisper(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	const TPacketCGWhisper* pinfo = reinterpret_cast<const TPacketCGWhisper*>(data);

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGWhisper);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	if (ch->FindAffect(AFFECT_BLOCK_CHAT))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¤ÆÃ ±ÝÁö »óÅÂÀÔ´Ï´Ù."));
		return (iExtraLen);
	}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM) && defined(ENABLE_BATTLE_ZONE_SYSTEM_HIDE_INFO_USER)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()) && (ch->GetGMLevel() < GM_IMPLEMENTOR))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_use_whisper"));
		return (iExtraLen);
	}
#endif

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(pinfo->szNameTo);

	if (pkChr == ch)
		return (iExtraLen);

	LPDESC pkDesc = nullptr;

	BYTE bOpponentEmpire = 0;

	if (test_server)
	{
		if (!pkChr)
			sys_log(0, "Whisper to %s(%s) from %s", "nullptr", pinfo->szNameTo, ch->GetName());
		else
			sys_log(0, "Whisper to %s(%s) from %s", pkChr->GetName(), pinfo->szNameTo, ch->GetName());
	}

	if (ch->IsBlockMode(BLOCK_WHISPER))
	{
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;
			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
			pack.bLevel = 0;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
			ch->GetDesc()->Packet(&pack, sizeof(pack));
		}
		return iExtraLen;
	}

	DWORD dwTargetPID; //Target player id
	if (!pkChr)
	{
		CCI* pkCCI = P2P_MANAGER::instance().Find(pinfo->szNameTo);

		if (pkCCI)
		{
			pkDesc = pkCCI->pkDesc;
			pkDesc->SetRelay(pinfo->szNameTo);
			bOpponentEmpire = pkCCI->bEmpire;
			dwTargetPID = pkCCI->dwPID;

			if (test_server)
			{
				sys_log(0, "Whisper out to %s from %s (Channel %d Mapindex %d)", pkCCI->szName, ch->GetName(), pkCCI->bChannel, pkCCI->lMapIndex);
			}
		}
	}
	else
	{
		pkDesc = pkChr->GetDesc();
		bOpponentEmpire = pkChr->GetEmpire();
		dwTargetPID = pkChr->GetPlayerID();
	}

	if (!pkDesc)
	{
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;

			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_NOT_EXIST;
			pack.bLevel = 0;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
#ifdef ENABLE_WHISPER_RENEWAL
			char buf[CHAT_MAX_LEN + 1];
			strlcpy(buf, data + sizeof(TPacketCGWhisper), MIN(iExtraLen + 1, sizeof(buf)));
			if (!(std::string(buf).find("|?whisper_renewal>|") != std::string::npos || std::string(buf).find("|?whisper_renewal<|") != std::string::npos)) {
				ch->GetDesc()->Packet(&pack, sizeof(TPacketGCWhisper));
				sys_log(0, "WHISPER: no player");
			}
#else
			ch->GetDesc()->Packet(&pack, sizeof(TPacketGCWhisper));
			sys_log(0, "WHISPER: no player");
#endif
		}
	}
	else
	{
		if (ch->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				pack.bLevel = 0;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
		else if (pkChr && pkChr->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_TARGET_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				pack.bLevel = 0;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
		else if (pkChr && MessengerManager::instance().IsBlocked_Target(ch->GetName(), pkChr->GetName()))
		{
			//ben
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg_2[CHAT_MAX_LEN + 1];
				snprintf(msg_2, sizeof(msg_2), LC_TEXT("%s'yi blokladim"), pkChr->GetName());
				int len = MIN(CHAT_MAX_LEN, strlen(msg_2) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				pack.bLevel = 0;

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg_2, len);
				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
		else if (pkChr && MessengerManager::instance().IsBlocked_Me(ch->GetName(), pkChr->GetName()))
		{
			//beni
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg_1[CHAT_MAX_LEN + 1];
				snprintf(msg_1, sizeof(msg_1), LC_TEXT("%s beni blokladi."), pkChr->GetName());
				int len = MIN(CHAT_MAX_LEN, strlen(msg_1) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				pack.bLevel = 0;

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg_1, len);

				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
		else if (pkChr && MessengerManager::instance().IsBlocked_Target(ch->GetName(), pkChr->GetName()) && MessengerManager::instance().IsBlocked_Me(ch->GetName(), pkChr->GetName()))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg_3[CHAT_MAX_LEN + 1];
				snprintf(msg_3, sizeof(msg_3), LC_TEXT("%s birbirimiz."), pkChr->GetName());
				int len = MIN(CHAT_MAX_LEN, strlen(msg_3) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				pack.bLevel = 0;

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg_3, len);

				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
#endif
		else
		{
			BYTE bType = WHISPER_TYPE_NORMAL;

			char buf[CHAT_MAX_LEN + 1];
			strlcpy(buf, data + sizeof(TPacketCGWhisper), MIN(iExtraLen + 1, sizeof(buf)));
			const size_t buflen = strlen(buf);
			CBanwordManager::instance().ConvertString(buf, buflen);

			int processReturn = ProcessTextTag(ch, buf, buflen);
			if (processReturn != 0)
			{
				if (ch->GetDesc())
				{
					TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

					if (pTable)
					{
						char buf[128];
						int len;
						if (3 == processReturn) //±³È¯Áß
							len = snprintf(buf, sizeof(buf), LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), pTable->szLocaleName);
						else
							len = snprintf(buf, sizeof(buf), LC_TEXT("%sÀÌ ÇÊ¿äÇÕ´Ï´Ù."), pTable->szLocaleName);

						if (len < 0 || len >= (int) sizeof(buf))
							len = sizeof(buf) - 1;

						++len;  // \0 ¹®ÀÚ Æ÷ÇÔ

						TPacketGCWhisper pack;

						pack.bHeader = HEADER_GC_WHISPER;
						pack.bType = WHISPER_TYPE_ERROR;
						pack.wSize = sizeof(TPacketGCWhisper) + len;
						strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
						pack.bLevel = 0;

						ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
						ch->GetDesc()->Packet(buf, len);

						sys_log(0, "WHISPER: not enough %s: char: %s", pTable->szLocaleName, ch->GetName());
					}
				}

				// ¸±·¡ÀÌ »óÅÂÀÏ ¼ö ÀÖÀ¸¹Ç·Î ¸±·¡ÀÌ¸¦ Ç®¾îÁØ´Ù.
				pkDesc->SetRelay("");
				return (iExtraLen);
			}

			if (ch->IsGM())
				bType = (bType & 0xF0) | WHISPER_TYPE_GM;

			if (buflen > 0)
			{
				TPacketGCWhisper pack;

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + buflen;
				pack.bType = bType;
				strlcpy(pack.szNameFrom, ch->GetName(), sizeof(pack.szNameFrom));
				pack.bLevel = ch->GetLevel();

				// desc->BufferedPacketÀ» ÇÏÁö ¾Ê°í ¹öÆÛ¿¡ ½á¾ßÇÏ´Â ÀÌÀ¯´Â
				// P2P relayµÇ¾î ÆÐÅ¶ÀÌ Ä¸½¶È­ µÉ ¼ö ÀÖ±â ¶§¹®ÀÌ´Ù.
				TEMP_BUFFER tmpbuf;

				tmpbuf.write(&pack, sizeof(pack));
				tmpbuf.write(buf, buflen);

				pkDesc->Packet(tmpbuf.read_peek(), tmpbuf.size());

				LogManager::instance().WhisperLog(ch->GetPlayerID(), dwTargetPID, buf);

				// LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), buf, buflen);
				// LogManager::instance().EscapeString(__escape_string2, sizeof(__escape_string2), pinfo->szNameTo, sizeof(pack.szNameFrom));
				// LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), 0, __escape_string2, "WHISPER", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
			}
		}
	}
	if (pkDesc)
		pkDesc->SetRelay("");

	return (iExtraLen);
}

struct RawPacketToCharacterFunc
{
	const void* m_buf;
	int	m_buf_len;

	RawPacketToCharacterFunc(const void* buf, int buf_len) : m_buf(buf), m_buf_len(buf_len)
	{
	}

	void operator () (LPCHARACTER c)
	{
		if (!c->GetDesc())
			return;

		c->GetDesc()->Packet(m_buf, m_buf_len);
	}
};

struct FEmpireChatPacket
{
	packet_chat& p;
	const char* orig_msg;
	int orig_len;
	char converted_msg[CHAT_MAX_LEN + 1];

	BYTE bEmpire;
	int iMapIndex;
	int namelen;

	FEmpireChatPacket(packet_chat& p, const char* chat_msg, int len, BYTE bEmpire, int iMapIndex, int iNameLen)
		: p(p), orig_msg(chat_msg), orig_len(len), bEmpire(bEmpire), iMapIndex(iMapIndex), namelen(iNameLen)
	{
		memset(converted_msg, 0, sizeof(converted_msg));
	}

	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		if (d->GetCharacter()->GetMapIndex() != iMapIndex)
			return;

		d->BufferedPacket(&p, sizeof(packet_chat));
		d->Packet(orig_msg, orig_len);
	}
};

#ifdef ENABLE_GROWTH_PET_SYSTEM
void CInputMain::BraveRequestPetName(LPCHARACTER ch, const char* c_pData)
{
	if (!ch->GetDesc()) { return; }
	int vid = ch->GetEggVid();
	if (vid == 0) { return; }
	
	if (!features::IsFeatureEnabled(features::PET))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;		
	}

	TPacketCGRequestPetName* p = (TPacketCGRequestPetName*)c_pData;

	if (ch->GetGold() < 100000) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] 100.000 Yang gerekir"));
		return;
	}

	if (!check_name(p->petname))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] Hatali isim girdiniz"));
		return;
	}

	if (ch->CountSpecifyItem(vid) > 0) {
		DBManager::instance().SendMoneyLog(MONEY_LOG_QUEST, ch->GetPlayerID(), -100000);
		ch->PointChange(POINT_GOLD, -100000, true);
		ch->RemoveSpecifyItem(vid, 1);
		LPITEM item = ch->AutoGiveItem(vid + 300, 1);
		if (!item)
			return;
		int tmpslot = number(3, 3);
		int tmpskill[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i)
		{
			if (i > tmpslot - 1)
				tmpskill[i] = -1;
		}
		
		int pet_type = number(0,5);
		int totaldur = number(1, 14) * 24 * 60 * 60;
		int tmpdur = time(0) + totaldur;
		int tmpagedur = time(0) - 86400;
		char szQuery1[1024];
		/*TPickupItems m_pickupItems[58];
		memset(m_pickupItems, 0, sizeof(m_pickupItems));*/
		snprintf(szQuery1, sizeof(szQuery1), "INSERT INTO new_petsystem VALUES(%u,'%s', 1, 0, 0, 0, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", item->GetID(), p->petname, number(1, 30), number(1, 30), number(1, 30), tmpskill[0], 0, tmpskill[1], 0, tmpskill[2], 0, tmpdur, totaldur, tmpagedur, pet_type);
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
	}
}
#endif

int CInputMain::Chat(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	const TPacketCGChat* pinfo = reinterpret_cast<const TPacketCGChat*>(data);

	if (uiBytes < pinfo->size)
		return -1;

	const int iExtraLen = pinfo->size - sizeof(TPacketCGChat);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->size, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char buf[CHAT_MAX_LEN - (CHARACTER_NAME_MAX_LEN + 3) + 1];
	strlcpy(buf, data + sizeof(TPacketCGChat), MIN(iExtraLen + 1, sizeof(buf)));
	const size_t buflen = strlen(buf);

	if (buflen > 1 && *buf == '/')
	{
		interpret_command(ch, buf + 1, buflen - 1);
		return iExtraLen;
	}

	if (ch->IncreaseChatCounter() >= 10)
	{
		if (ch->GetChatCounter() == 10)
		{
			sys_log(0, "CHAT_HACK: %s", ch->GetName());
			ch->GetDesc()->DelayedDisconnect(5);
		}

		return iExtraLen;
	}

	// Ã¤ÆÃ ±ÝÁö Affect Ã³¸®
	const CAffect* pAffect = ch->FindAffect(AFFECT_BLOCK_CHAT);

	if (pAffect != nullptr)
	{
		SendBlockChatInfo(ch, pAffect->lDuration);
		return iExtraLen;
	}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM) && defined(ENABLE_BATTLE_ZONE_SYSTEM_HIDE_INFO_USER)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()) && (ch->GetGMLevel() < GM_IMPLEMENTOR))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_use_chat"));
		return iExtraLen;
	}
#endif

	if (CHAT_TYPE_SHOUT != pinfo->type)
	{
		if (ch->GetMapIndex() == OXEVENT_MAP_INDEX)
		{
			if (quest::CQuestManager::Instance().GetEventFlag("ox_chat") == 1 && ch->GetGMLevel() == GM_PLAYER)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OX_CHAT_WARNING"));
				return iExtraLen;
			}
		}
	}

	char chatbuf[CHAT_MAX_LEN + 1];
	int len = snprintf(chatbuf, sizeof(chatbuf), "%s : %s", ch->GetName(), buf);

	if (CHAT_TYPE_SHOUT == pinfo->type)
	{
		const char* kingdoms[3] = { "[1]   ","[2]   ","[3]   " };
		len = snprintf(chatbuf, sizeof(chatbuf), "%s%s |H%s%s|h(#)|h|r - Lv.%d|h|r : %s", kingdoms[ch->GetEmpire() - 1], ch->GetName(), "whisper:", ch->GetName(), ch->GetLevel(), buf);
		LogManager::instance().ShoutLog(g_bChannel, ch->GetEmpire(), chatbuf);
	}

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	int processReturn = ProcessTextTag(ch, chatbuf, len);
	if (0 != processReturn)
	{
		const TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

		if (nullptr != pTable)
		{
			if (3 == processReturn) //±³È¯Áß
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), pTable->szLocaleName);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%sÀÌ ÇÊ¿äÇÕ´Ï´Ù."), pTable->szLocaleName);
		}

		return iExtraLen;
	}

	if (pinfo->type == CHAT_TYPE_SHOUT)
	{
		if (ch->GetLevel() < g_iShoutLimitLevel)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿ÜÄ¡±â´Â ·¹º§ %d ÀÌ»ó¸¸ »ç¿ë °¡´É ÇÕ´Ï´Ù."), g_iShoutLimitLevel);
			return (iExtraLen);
		}

		if (thecore_heart->pulse - (int)ch->GetLastShoutPulse() < passes_per_sec * g_iShoutLimitValue)
			return (iExtraLen);

		ch->SetLastShoutPulse(thecore_heart->pulse);

		TPacketGGShout p;

		p.bHeader = HEADER_GG_SHOUT;
		p.bEmpire = ch->GetEmpire();
		strlcpy(p.szText, chatbuf, sizeof(p.szText));

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShout));

		SendShout(chatbuf, ch->GetEmpire());

		return (iExtraLen);
	}

	TPacketGCChat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(TPacketGCChat) + len;
	pack_chat.type = pinfo->type;
	pack_chat.id = ch->GetVID();

	switch (pinfo->type)
	{
	case CHAT_TYPE_TALKING:
	{
		const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();

		std::for_each(c_ref_set.begin(), c_ref_set.end(), FEmpireChatPacket(pack_chat, chatbuf, len, (ch->GetGMLevel() > GM_PLAYER || ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)) ? 0 : ch->GetEmpire(), ch->GetMapIndex(), strlen(ch->GetName())));
#ifdef ENABLE_CHAT_LOGGING
		LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
		LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), 0, "", "NORMAL", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
#endif
	}
	break;

	case CHAT_TYPE_PARTY:
	{
		if (!ch->GetParty())
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÆÄÆ¼ ÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		else
		{
			TEMP_BUFFER tbuf;

			tbuf.write(&pack_chat, sizeof(pack_chat));
			tbuf.write(chatbuf, len);

			RawPacketToCharacterFunc f(tbuf.read_peek(), tbuf.size());
			ch->GetParty()->ForEachOnlineMember(f);

#ifdef ENABLE_CHAT_LOGGING
			LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
			LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetParty()->GetLeaderPID(), "", "PARTY", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
#endif
		}
	}
	break;

	case CHAT_TYPE_GUILD:
	{
		if (!ch->GetGuild())
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±æµå¿¡ °¡ÀÔÇÏÁö ¾Ê¾Ò½À´Ï´Ù."));
		else
			ch->GetGuild()->Chat(chatbuf);

#ifdef ENABLE_CHAT_LOGGING
		if (ch->GetGuild())
		{
			LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
			LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetGuild()->GetID(), ch->GetGuild()->GetName(), "GUILD", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
		}
#endif
	}
	break;

	default:
		sys_err("Unknown chat type %d", pinfo->type);
		break;
	}

	return (iExtraLen);
}

void CInputMain::ItemUse(LPCHARACTER ch, const char* data)
{
	ch->UseItem(((struct command_item_use*) data)->Cell);
}

void CInputMain::ItemToItem(LPCHARACTER ch, const char* pcData)
{
	TPacketCGItemUseToItem* p = (TPacketCGItemUseToItem*)pcData;
	if (ch)
		ch->UseItem(p->Cell, p->TargetCell);
}

#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
void CInputMain::ItemDrop(LPCHARACTER ch, const char* data)
{
	struct command_item_drop* pinfo = (struct command_item_drop*) data;

	if (!ch)
		return;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
	else
		ch->DropItem(pinfo->Cell);
}

void CInputMain::ItemDrop2(LPCHARACTER ch, const char* data)
{
	TPacketCGItemDrop2* pinfo = (TPacketCGItemDrop2*)data;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.

	if (!ch)
		return;
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
	else
		ch->DropItem(pinfo->Cell, pinfo->count);
}
#else
void CInputMain::ItemDelete(LPCHARACTER ch, const char* data)
{
	struct command_item_delete* pinfo = (struct command_item_delete*) data;

	if (ch)
		ch->DeleteItem(pinfo->Cell);
}

void CInputMain::ItemSell(LPCHARACTER ch, const char* data)
{
	struct command_item_sell* pinfo = (struct command_item_sell*) data;

	if (ch)
		ch->SellItem(pinfo->Cell);
}
#endif

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
void CInputMain::ExInventory(CHARACTER * ch, const char* data)
{
	struct command_extend_inventory* pinfo = (struct command_extend_inventory*) data;

	if (!ch->CanHandleItem())
		return;

	switch (pinfo->sub_header)
	{
	case 0:	// upgrade Extend Inventory
	{
		sys_log(0, "CInputMain()::ExInventory() ==> UpgradeExInventory");
		ch->UpgradeExInventory(ch);
	}
	break;

	case 1:	// Button Click Extend Inventory
	{
		sys_log(0, "CInputMain()::ExInventory() ==> ButtonClick");
		ch->ExtendInventoryItemUse(ch);
	}
	break;
	}
}
#endif

void CInputMain::ItemMove(LPCHARACTER ch, const char* data)
{
	struct command_item_move* pinfo = (struct command_item_move*) data;

	if (ch)
		ch->MoveItem(pinfo->Cell, pinfo->CellTo, pinfo->count);
}

void CInputMain::ItemPickup(LPCHARACTER ch, const char* data)
{
	struct command_item_pickup* pinfo = (struct command_item_pickup*) data;
	if (ch)
	{
		ch->PickupItem(pinfo->vid);
	}
}

void CInputMain::QuickslotAdd(LPCHARACTER ch, const char* data)
{
	struct command_quickslot_add* pinfo = (struct command_quickslot_add*) data;

	if (!ch)
		return;

    if(pinfo->slot.type == QUICKSLOT_TYPE_ITEM)
    {
        LPITEM item = NULL;
      
        TItemPos srcCell(INVENTORY, pinfo->slot.pos);
      
        if (!(item = ch->GetItem(srcCell)))
            return;
      
        if (item->GetType() != ITEM_USE && item->GetType() != ITEM_QUEST)
            return;
    }
	
	ch->SetQuickslot(pinfo->pos, pinfo->slot);
}

void CInputMain::QuickslotDelete(LPCHARACTER ch, const char* data)
{
	struct command_quickslot_del* pinfo = (struct command_quickslot_del*) data;
	ch->DelQuickslot(pinfo->pos);
}

void CInputMain::QuickslotSwap(LPCHARACTER ch, const char* data)
{
	struct command_quickslot_swap* pinfo = (struct command_quickslot_swap*) data;
	ch->SwapQuickslot(pinfo->pos, pinfo->change_pos);
}
int CInputMain::Messenger(LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	TPacketCGMessenger* p = (TPacketCGMessenger*)c_pData;

	if (uiBytes < sizeof(TPacketCGMessenger))
		return -1;

	c_pData += sizeof(TPacketCGMessenger);
	uiBytes -= sizeof(TPacketCGMessenger);

	switch (p->subheader)
	{
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	case MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_VID:
	{
		if (uiBytes < sizeof(TPacketCGMessengerAddBlockByVID))
			return -1;

		TPacketCGMessengerAddBlockByVID* p2 = (TPacketCGMessengerAddBlockByVID*)c_pData;
		LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

		if (!ch_companion)
			return sizeof(TPacketCGMessengerAddBlockByVID);

		if (ch->IsObserverMode())
			return sizeof(TPacketCGMessengerAddBlockByVID);

		LPDESC d = ch_companion->GetDesc();

		if (!d)
			return sizeof(TPacketCGMessengerAddByVID);

		if (ch_companion->GetGuild() == ch->GetGuild() && ch->GetGuild() != nullptr && ch_companion->GetGuild() != nullptr)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lonca olmaz"));
			return sizeof(TPacketCGMessengerAddBlockByVID);
		}

		if (MessengerManager::instance().IsFriend(ch->GetName(), ch_companion->GetName()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("arkadas ekleyemezsin."));
			return sizeof(TPacketCGMessengerAddBlockByVID);
		}

		if (MessengerManager::instance().IsBlocked_Target(ch->GetName(), ch_companion->GetName()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("zaten bloklu"));
			return sizeof(TPacketCGMessengerAddBlockByVID);
		}

		if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("gm'ye bu yanlis yapilmaz."));
			return sizeof(TPacketCGMessengerAddByVID);
		}

		if (ch->GetDesc() == d) // ??? ??? ? ??.
			return sizeof(TPacketCGMessengerAddBlockByVID);

		MessengerManager::instance().AddToBlockList(ch->GetName(), ch_companion->GetName());
	}
	return sizeof(TPacketCGMessengerAddBlockByVID);

	case MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_NAME:
	{
		if (uiBytes < CHARACTER_NAME_MAX_LEN)
			return -1;

		char name[CHARACTER_NAME_MAX_LEN + 1];
		strlcpy(name, c_pData, sizeof(name));

		if (gm_get_level(name) != GM_PLAYER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("gm'ye bu yanlis yapilmaz."));
			return CHARACTER_NAME_MAX_LEN;
		}

		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);

		if (!tch)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s yok ki."), name);
		else
		{
			if (tch == ch) // ??? ??? ? ??.
				return CHARACTER_NAME_MAX_LEN;

			if (tch->GetGuild() == ch->GetGuild() && ch->GetGuild() != nullptr && tch->GetGuild() != nullptr)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lonca olmaz"));
				return CHARACTER_NAME_MAX_LEN;
			}

			if (MessengerManager::instance().IsFriend(ch->GetName(), tch->GetName()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("arkadas ekleyemezsin."));
				return CHARACTER_NAME_MAX_LEN;
			}

			if (MessengerManager::instance().IsBlocked_Target(ch->GetName(), tch->GetName()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("zaten bloklu"));
				return CHARACTER_NAME_MAX_LEN;
			}

			MessengerManager::instance().AddToBlockList(ch->GetName(), tch->GetName());
		}
	}
	return CHARACTER_NAME_MAX_LEN;

	case MESSENGER_SUBHEADER_CG_REMOVE_BLOCK:
	{
		if (uiBytes < CHARACTER_NAME_MAX_LEN)
			return -1;

		char char_name[CHARACTER_NAME_MAX_LEN + 1];
		strlcpy(char_name, c_pData, sizeof(char_name));

		if (!MessengerManager::instance().IsBlocked_Target(ch->GetName(), char_name))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bloklu degil"));
			return CHARACTER_NAME_MAX_LEN;
		}
		MessengerManager::instance().RemoveFromBlockList(ch->GetName(), char_name);
	}
	return CHARACTER_NAME_MAX_LEN;
#endif

	case MESSENGER_SUBHEADER_CG_ADD_BY_VID:
	{
		if (uiBytes < sizeof(TPacketCGMessengerAddByVID))
			return -1;

		TPacketCGMessengerAddByVID* p2 = (TPacketCGMessengerAddByVID*)c_pData;
		LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

		if (!ch_companion)
			return sizeof(TPacketCGMessengerAddByVID);

		if (ch->IsObserverMode())
			return sizeof(TPacketCGMessengerAddByVID);

		if (ch_companion->IsBlockMode(BLOCK_MESSENGER_INVITE))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ¸Þ½ÅÁ® Ãß°¡ °ÅºÎ »óÅÂÀÔ´Ï´Ù."));
			return sizeof(TPacketCGMessengerAddByVID);
		}

		LPDESC d = ch_companion->GetDesc();

		if (!d)
			return sizeof(TPacketCGMessengerAddByVID);

		if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<¸Þ½ÅÁ®> ¿î¿µÀÚ´Â ¸Þ½ÅÁ®¿¡ Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return sizeof(TPacketCGMessengerAddByVID);
		}

		if (ch->GetDesc() == d) // ??? ??? ? ??.
			return sizeof(TPacketCGMessengerAddByVID);

		if (MessengerManager::instance().IsFriend(ch->GetName(), ch_companion->GetName()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("zaten arkadas."));
			return sizeof(TPacketCGMessengerAddByVID);
		}

		if (MessengerManager::instance().IsBlocked(ch->GetName(), ch_companion->GetName()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("blok ekleyemezsin"));
			return sizeof(TPacketCGMessengerAddByVID);
		}

		MessengerManager::instance().RequestToAdd(ch, ch_companion);
		//MessengerManager::instance().AddToList(ch->GetName(), ch_companion->GetName());
	}
	return sizeof(TPacketCGMessengerAddByVID);

	case MESSENGER_SUBHEADER_CG_ADD_BY_NAME:
	{
		if (uiBytes < CHARACTER_NAME_MAX_LEN)
			return -1;

		char name[CHARACTER_NAME_MAX_LEN + 1];
		strlcpy(name, c_pData, sizeof(name));

		if (ch->GetGMLevel() == GM_PLAYER && gm_get_level(name) != GM_PLAYER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<¸Þ½ÅÁ®> ¿î¿µÀÚ´Â ¸Þ½ÅÁ®¿¡ Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return CHARACTER_NAME_MAX_LEN;
		}

		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);

		if (!tch)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀº Á¢¼ÓµÇ ÀÖÁö ¾Ê½À´Ï´Ù."), name);
		else
		{
			if (tch == ch) // ??? ??? ? ??.
				return CHARACTER_NAME_MAX_LEN;

			if (tch->IsBlockMode(BLOCK_MESSENGER_INVITE) == true)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ¸Þ½ÅÁ® Ãß°¡ °ÅºÎ »óÅÂÀÔ´Ï´Ù."));
			}
			else
			{
				if (MessengerManager::instance().IsFriend(ch->GetName(), tch->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("zaten arkadas."));
					return CHARACTER_NAME_MAX_LEN;
				}

				if (MessengerManager::instance().IsBlocked(ch->GetName(), tch->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("blok ekleyemezsin"));
					return CHARACTER_NAME_MAX_LEN;
				}
				// ???? ?????? ??? ??
				MessengerManager::instance().RequestToAdd(ch, tch);
				//MessengerManager::instance().AddToList(ch->GetName(), tch->GetName());
			}
		}
	}
	return CHARACTER_NAME_MAX_LEN;

	case MESSENGER_SUBHEADER_CG_REMOVE:
	{
		if (uiBytes < CHARACTER_NAME_MAX_LEN)
			return -1;

		char char_name[CHARACTER_NAME_MAX_LEN + 1];
		strlcpy(char_name, c_pData, sizeof(char_name));
		if (!MessengerManager::instance().IsFriend(ch->GetName(), char_name))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("arkadas degil"));
			return CHARACTER_NAME_MAX_LEN;
		}
		else
		{
			MessengerManager::instance().RemoveFromList(ch->GetName(), char_name)
			MessengerManager::instance().RemoveFromList(char_name, ch->GetName());
		}
	}
	return CHARACTER_NAME_MAX_LEN;

	default:
		sys_err("CInputMain::Messenger : Unknown subheader %d : %s", p->subheader, ch->GetName());
		break;
	}

	return 0;
}

int CInputMain::Shop(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	TPacketCGShop* p = (TPacketCGShop*)data;

	if (uiBytes < sizeof(TPacketCGShop))
		return -1;

	if (test_server)
		sys_log(0, "CInputMain::Shop() ==> SubHeader %d", p->subheader);

	if (!ch)
		return -1;

	if (!ch->GetDesc())
		return -1;

	const char* c_pData = data + sizeof(TPacketCGShop);
	uiBytes -= sizeof(TPacketCGShop);

	switch (p->subheader)
	{
	case SHOP_SUBHEADER_CG_END:
		sys_log(1, "INPUT: %s SHOP: END", ch->GetName());
		CShopManager::instance().StopShopping(ch);
		return 0;

	case SHOP_SUBHEADER_CG_BUY:
	{
		if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
			return -1;

		BYTE bPos = *(c_pData + 1);
		sys_log(1, "INPUT: %s SHOP: BUY %d", ch->GetName(), bPos);
		CShopManager::instance().Buy(ch, bPos);
		return (sizeof(BYTE) + sizeof(BYTE));
	}

	case SHOP_SUBHEADER_CG_SELL:
	{
		if (uiBytes < sizeof(BYTE))
			return -1;

		BYTE pos = *c_pData;

		sys_log(0, "INPUT: %s SHOP: SELL", ch->GetName());
		CShopManager::instance().Sell(ch, pos);
		return sizeof(BYTE);
	}

	case SHOP_SUBHEADER_CG_SELL2:
	{
		if (uiBytes < sizeof(UINT) + sizeof(BYTE))
			return -1;

		const UINT wPos = *reinterpret_cast<const UINT*>(c_pData);
		const BYTE bCount = *(c_pData + sizeof(UINT));
		const BYTE bType = *(c_pData + sizeof(UINT) + sizeof(BYTE));

		sys_log(0, "INPUT: %s SHOP: SELL2", ch->GetName());
		CShopManager::instance().Sell(ch, wPos, bCount, bType);
		return sizeof(UINT) + sizeof(BYTE) + sizeof(BYTE);
	}

	default:
		sys_err("CInputMain::Shop : Unknown subheader %d : %s", p->subheader, ch->GetName());
		break;
	}

	return 0;
}

void CInputMain::OnClick(LPCHARACTER ch, const char* data)
{
	struct command_on_click* pinfo = (struct command_on_click*) data;
	LPCHARACTER			victim;

	if ((victim = CHARACTER_MANAGER::instance().Find(pinfo->vid)))
		victim->OnClick(ch);
	else if (test_server)
	{
		sys_err("CInputMain::OnClick %s.Click.NOT_EXIST_VID[%d]", ch->GetName(), pinfo->vid);
	}
}

void CInputMain::Exchange(LPCHARACTER ch, const char* data)
{
	struct command_exchange* pinfo = (struct command_exchange*) data;
	LPCHARACTER	to_ch = nullptr;

	if (!ch->CanHandleItem())
		return;

	int iPulse = thecore_pulse();

	if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
	{
		if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			to_ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡ Ã¢°í¸¦ ¿­¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
			return;
		}

		if (true == to_ch->IsDead())
		{
			return;
		}
	}

	sys_log(0, "CInputMain()::Exchange()  SubHeader %d ", pinfo->sub_header);

	if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡ Ã¢°í¸¦ ¿­¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
		return;
	}

	switch (pinfo->sub_header)
	{
	case EXCHANGE_SUBHEADER_CG_START:	// arg1 == vid of target character
		if (!ch->GetExchange())
		{
			if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
			{
				if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í¸¦ ¿¬ÈÄ %dÃÊ ÀÌ³»¿¡´Â °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);

					if (test_server)
						ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
					return;
				}

				if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
				{
					to_ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í¸¦ ¿¬ÈÄ %dÃÊ ÀÌ³»¿¡´Â °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);

					if (test_server)
						to_ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, to_ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
					return;
				}

				if (ch->GetGold() >= GOLD_MAX)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾×¼ö°¡ 20¾ï ³ÉÀ» ÃÊ°úÇÏ¿© °Å·¡¸¦ ÇÒ¼ö°¡ ¾ø½À´Ï´Ù.."));

					sys_err("[OVERFLOG_GOLD] START (%u) id %u name %s ", ch->GetGold(), ch->GetPlayerID(), ch->GetName());
					return;
				}

				if (to_ch->IsPC())
				{
					if (quest::CQuestManager::instance().GiveItemToPC(ch->GetPlayerID(), to_ch))
					{
						sys_log(0, "Exchange canceled by quest %s %s", ch->GetName(), to_ch->GetName());
						return;
					}
				}

				if (ch->GetMyShop() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
					return;
				}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
				if (ch->IsCombOpen())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
					return;
				}
#endif

				ch->ExchangeStart(to_ch);
			}
		}
		break;

	case EXCHANGE_SUBHEADER_CG_ITEM_ADD:	// arg1 == position of item, arg2 == position in exchange window
		if (ch->GetExchange())
		{
#ifndef ENABLE_EXCHANGE_WINDOW_SYSTEM
			if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
#endif
				ch->GetExchange()->AddItem(pinfo->Pos, pinfo->arg2);
		}
		break;

	case EXCHANGE_SUBHEADER_CG_ITEM_DEL:	// arg1 == position of item
		if (ch->GetExchange())
		{
#ifndef ENABLE_EXCHANGE_WINDOW_SYSTEM
			if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
#endif
				ch->GetExchange()->RemoveItem(pinfo->arg1);
		}
		break;

	case EXCHANGE_SUBHEADER_CG_ELK_ADD:	// arg1 == amount of gold
		if (ch->GetExchange())
		{
			const int64_t nTotalGold = static_cast<int64_t>(ch->GetExchange()->GetCompany()->GetOwner()->GetGold()) + static_cast<int64_t>(pinfo->arg1);

			if (GOLD_MAX <= nTotalGold)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ ÃÑ±Ý¾×ÀÌ 20¾ï ³ÉÀ» ÃÊ°úÇÏ¿© °Å·¡¸¦ ÇÒ¼ö°¡ ¾ø½À´Ï´Ù.."));

				sys_err("[OVERFLOW_GOLD] ELK_ADD (%u) id %u name %s ",
					ch->GetExchange()->GetCompany()->GetOwner()->GetGold(),
					ch->GetExchange()->GetCompany()->GetOwner()->GetPlayerID(),
					ch->GetExchange()->GetCompany()->GetOwner()->GetName());

				return;
			}

			if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
				ch->GetExchange()->AddGold(pinfo->arg1);
		}
		break;
#ifdef ENABLE_CHEQUE_SYSTEM
	case EXCHANGE_SUBHEADER_CG_CHEQUE_ADD:	// arg1 == amount of cheque
		if (ch->GetExchange())
		{
			const int16_t nTotalCheque = static_cast<int16_t>(ch->GetExchange()->GetCompany()->GetOwner()->GetCheque()) + static_cast<int16_t>(pinfo->arg1);

			if (CHEQUE_MAX <= nTotalCheque)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The other player has cheque over the limite."));

				sys_err("[OVERFLOW_CHEQUE] CHEQUE_ADD (%u) id %u name %s ",
					ch->GetExchange()->GetCompany()->GetOwner()->GetCheque(),
					ch->GetExchange()->GetCompany()->GetOwner()->GetPlayerID(),
					ch->GetExchange()->GetCompany()->GetOwner()->GetName());

				return;
			}

			if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
				ch->GetExchange()->AddCheque(pinfo->arg1);
		}
		break;
#endif

	case EXCHANGE_SUBHEADER_CG_ACCEPT:	// arg1 == not used
		if (ch->GetExchange())
		{
			sys_log(0, "CInputMain()::Exchange() ==> ACCEPT ");
			ch->GetExchange()->Accept(true);
		}

		break;

	case EXCHANGE_SUBHEADER_CG_CANCEL:	// arg1 == not used
		if (ch->GetExchange())
			ch->GetExchange()->Cancel();
		break;
	}
}

void CInputMain::Position(LPCHARACTER ch, const char* data)
{
	struct command_position* pinfo = (struct command_position*) data;

	switch (pinfo->position)
	{
	case POSITION_GENERAL:
		ch->Standup();
		break;

	case POSITION_SITTING_CHAIR:
		ch->Sitdown(0);
		break;

	case POSITION_SITTING_GROUND:
		ch->Sitdown(1);
		break;
	}
}

void CInputMain::Move(LPCHARACTER ch, const char* data)
{
	if (!ch->CanMove())
		return;

	struct command_move* pinfo = (struct command_move*) data;

	if (pinfo->bFunc >= FUNC_MAX_NUM && !(pinfo->bFunc & 0x80))
	{
		sys_err("invalid move type: %s", ch->GetName());
		return;
	}

	// const float fDist = DISTANCE_SQRT((ch->GetX() - pinfo->lX) / 100, (ch->GetY() - pinfo->lY) / 100);
	// if (((false == ch->IsRiding() && fDist > 40) || (true == ch->IsRidingMount() && fDist > 999) || fDist > 60) && OXEVENT_MAP_INDEX != ch->GetMapIndex())
	// {
		// sys_log(0, "MOVE: %s trying to move too far (dist: %.1fm) Riding(%d)", ch->GetName(), fDist, ch->IsRiding());
		// ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
		// ch->Stop();
		// return;
	// }

#ifdef ENABLE_GHOSTMODE_FIX
	if (ch->IsPC() && ch->IsDead())
	{
		sys_log(0, "MOVE: %s trying to move as dead", ch->GetName());
		ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
		ch->Stop();
		return;
	}
#endif

	if (pinfo->bFunc == FUNC_MOVE)
	{
		if (ch->GetLimitPoint(POINT_MOV_SPEED) == 0)
			return;

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Goto(pinfo->lX, pinfo->lY);

#ifdef ENABLE_AFK_MODE_SYSTEM
		if (ch->IsAway())
		{
			ch->SetAway(false);
			if (ch->IsAffectFlag(AFF_AFK))
				ch->RemoveAffect(AFFECT_AFK);
		}
#endif
	}
	else
	{
		if (pinfo->bFunc == FUNC_ATTACK || pinfo->bFunc == FUNC_COMBO)
		{
			ch->OnMove(true);
#ifdef ENABLE_ABUSE_SYSTEM
			ch->GetAbuseController()->ReceiveMoveAttackPacket();
#endif
		}
		else if (pinfo->bFunc & FUNC_SKILL)
		{
			const int MASK_SKILL_MOTION = 0x7F;
			unsigned int motion = pinfo->bFunc & MASK_SKILL_MOTION;

			if (!ch->IsUsableSkillMotion(motion))
			{
				const char* name = ch->GetName();
				unsigned int job = ch->GetJob();
				unsigned int group = ch->GetSkillGroup();

				char szBuf[256];
				snprintf(szBuf, sizeof(szBuf), "SKILL_HACK: name=%s, job=%d, group=%d, motion=%d", name, job, group, motion);
				LogManager::instance().HackLog(szBuf, ch->GetDesc()->GetAccountTable().login, ch->GetName(), ch->GetDesc()->GetHostName());
				sys_log(0, "%s", szBuf);

				if (test_server)
				{
					ch->GetDesc()->DelayedDisconnect(number(2, 8));
					ch->ChatPacket(CHAT_TYPE_INFO, szBuf);
				}
				else
				{
					ch->GetDesc()->DelayedDisconnect(number(150, 500));
				}
			}

			ch->OnMove();
		}

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Move(pinfo->lX, pinfo->lY);
		ch->Stop();
		ch->StopStaminaConsume();
	}

	TPacketGCMove pack;

	pack.bHeader = HEADER_GC_MOVE;
	pack.bFunc = pinfo->bFunc;
	pack.bArg = pinfo->bArg;
	pack.bRot = pinfo->bRot;
	pack.dwVID = ch->GetVID();
	pack.lX = pinfo->lX;
	pack.lY = pinfo->lY;
	pack.dwTime = pinfo->dwTime;
	pack.dwDuration = (pinfo->bFunc == FUNC_MOVE) ? ch->GetCurrentMoveDuration() : 0;

	ch->PacketAround(&pack, sizeof(TPacketGCMove), ch);
}

void CInputMain::Attack(LPCHARACTER ch, const BYTE header, const char* data)
{
	if (nullptr == ch)
		return;

	struct type_identifier
	{
		BYTE header;
		BYTE type;
	};

	const struct type_identifier* const type = reinterpret_cast<const struct type_identifier*>(data);

	if (type->type > 0)
	{
		if (false == ch->CanUseSkill(type->type))
		{
			return;
		}

		switch (type->type)
		{
		case SKILL_GEOMPUNG:
		case SKILL_SANGONG:
		case SKILL_YEONSA:
		case SKILL_KWANKYEOK:
		case SKILL_HWAJO:
		case SKILL_GIGUNG:
		case SKILL_PABEOB:
		case SKILL_MARYUNG:
		case SKILL_TUSOK:
		case SKILL_MAHWAN:
		case SKILL_BIPABU:
		case SKILL_NOEJEON:
		case SKILL_CHAIN:
		case SKILL_HORSE_WILDATTACK_RANGE:
			if (HEADER_CG_SHOOT != type->header)
			{
				if (test_server)
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Attack :name[%s] Vnum[%d] can't use skill by attack(warning)"), type->type);
				return;
			}
			break;
		}
	}

	switch (header)
	{
	case HEADER_CG_ATTACK:
	{
		if (nullptr == ch->GetDesc())
			return;

		const TPacketCGAttack * const packMelee = reinterpret_cast<const TPacketCGAttack*>(data);

		LPCHARACTER	victim = CHARACTER_MANAGER::instance().Find(packMelee->dwVID);

		if (nullptr == victim || ch == victim)
			return;

		switch (victim->GetCharType())
		{
		case CHAR_TYPE_NPC:
		case CHAR_TYPE_WARP:
		case CHAR_TYPE_GOTO:
			return;
		}

		if (packMelee->bType > 0)
		{
			if (false == ch->CheckSkillHitCount(packMelee->bType, victim->GetVID()))
			{
				return;
			}
		}

		ch->Attack(victim, packMelee->bType);
	}
	break;

	case HEADER_CG_SHOOT:
	{
		const TPacketCGShoot* const packShoot = reinterpret_cast<const TPacketCGShoot*>(data);

		ch->Shoot(packShoot->bType);
	}
	break;
	}
}

int CInputMain::SyncPosition(LPCHARACTER ch, const char* c_pcData, size_t uiBytes)
{
	const TPacketCGSyncPosition* pinfo = reinterpret_cast<const TPacketCGSyncPosition*>(c_pcData);

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGSyncPosition);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	if (0 != (iExtraLen % sizeof(TPacketCGSyncPositionElement)))
	{
		sys_err("invalid packet length %d (name: %s)", pinfo->wSize, ch->GetName());
		return iExtraLen;
	}

	int iCount = iExtraLen / sizeof(TPacketCGSyncPositionElement);

	if (iCount <= 0)
		return iExtraLen;

	TEMP_BUFFER tbuf;
	LPBUFFER lpBuf = tbuf.getptr();

	TPacketGCSyncPosition * pHeader = (TPacketGCSyncPosition*)buffer_write_peek(lpBuf);
	buffer_write_proceed(lpBuf, sizeof(TPacketGCSyncPosition));

	const TPacketCGSyncPositionElement * e =
		reinterpret_cast<const TPacketCGSyncPositionElement*>(c_pcData + sizeof(TPacketCGSyncPosition));

	timeval tvCurTime;
	gettimeofday(&tvCurTime, nullptr);

	for (int i = 0; i < iCount; ++i, ++e)
	{
		LPCHARACTER victim = CHARACTER_MANAGER::instance().Find(e->dwVID);

		if (!victim)
			continue;

		switch (victim->GetCharType())
		{
		case CHAR_TYPE_NPC:
		case CHAR_TYPE_WARP:
		case CHAR_TYPE_GOTO:
			continue;
		}

		// ¼ÒÀ¯±Ç °Ë»ç
		if (!victim->SetSyncOwner(ch))
			continue;

		victim->SetLastSyncTime(tvCurTime);
		victim->Sync(e->lX, e->lY);
		buffer_write(lpBuf, e, sizeof(TPacketCGSyncPositionElement));
	}

	if (buffer_size(lpBuf) != sizeof(TPacketGCSyncPosition))
	{
		pHeader->bHeader = HEADER_GC_SYNC_POSITION;
		pHeader->wSize = buffer_size(lpBuf);

		ch->PacketAround(buffer_read_peek(lpBuf), buffer_size(lpBuf), ch);
	}

	return iExtraLen;
}

void CInputMain::FlyTarget(LPCHARACTER ch, const char* pcData, BYTE bHeader)
{
	TPacketCGFlyTargeting* p = (TPacketCGFlyTargeting*)pcData;
	if (!ch)
		return;
	ch->FlyTarget(p->dwTargetVID, p->x, p->y, bHeader);
}

void CInputMain::UseSkill(LPCHARACTER ch, const char* pcData)
{
	TPacketCGUseSkill* p = (TPacketCGUseSkill*)pcData;
	if (false == ch->CanUseSkill(p->dwVnum))
		return;
	ch->UseSkill(p->dwVnum, CHARACTER_MANAGER::instance().Find(p->dwVID));
}

void CInputMain::ScriptButton(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptButton* p = (TPacketCGScriptButton*)c_pData;
	sys_log(0, "QUEST ScriptButton pid %d idx %u", ch->GetPlayerID(), p->idx);

	quest::PC* pc = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());
	if (pc && pc->IsConfirmWait())
	{
		quest::CQuestManager::instance().Confirm(ch->GetPlayerID(), quest::CONFIRM_TIMEOUT);
	}
	else if (p->idx & 0x80000000)
	{
		quest::CQuestManager::Instance().QuestInfo(ch->GetPlayerID(), p->idx & 0x7fffffff);
	}
	else
	{
		quest::CQuestManager::Instance().QuestButton(ch->GetPlayerID(), p->idx);
	}
}

void CInputMain::ScriptAnswer(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptAnswer* p = (TPacketCGScriptAnswer*)c_pData;
	sys_log(0, "QUEST ScriptAnswer pid %d answer %d", ch->GetPlayerID(), p->answer);

	if (p->answer > 250) // ´ÙÀ½ ¹öÆ°¿¡ ´ëÇÑ ÀÀ´äÀ¸·Î ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Resume(ch->GetPlayerID());
	}
	else // ¼±ÅÃ ¹öÆ°À» °ñ¶ó¼­ ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Select(ch->GetPlayerID(), p->answer);
	}
}

// SCRIPT_SELECT_ITEM
void CInputMain::ScriptSelectItem(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptSelectItem* p = (TPacketCGScriptSelectItem*)c_pData;
	sys_log(0, "QUEST ScriptSelectItem pid %d answer %d", ch->GetPlayerID(), p->selection);
	quest::CQuestManager::Instance().SelectItem(ch->GetPlayerID(), p->selection.cell);
}
// END_OF_SCRIPT_SELECT_ITEM

void CInputMain::QuestInputString(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestInputString* p = (TPacketCGQuestInputString*)c_pData;

	char msg[65];
	strlcpy(msg, p->msg, sizeof(msg));
	sys_log(0, "QUEST InputString pid %u msg %s", ch->GetPlayerID(), msg);

	quest::CQuestManager::Instance().Input(ch->GetPlayerID(), msg);
}

#ifdef ENABLE_OX_RENEWAL
void CInputMain::QuestInputLongString(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestInputLongString* p = (TPacketCGQuestInputLongString*)c_pData;

	char msg[129];
	strlcpy(msg, p->msg, sizeof(msg));
	sys_log(0, "QUEST InputLongString pid %u msg %s", ch->GetPlayerID(), msg);

	quest::CQuestManager::Instance().Input(ch->GetPlayerID(), msg);
}
#endif

void CInputMain::QuestConfirm(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestConfirm* p = (TPacketCGQuestConfirm*)c_pData;
	LPCHARACTER ch_wait = CHARACTER_MANAGER::instance().FindByPID(p->requestPID);
	if (p->answer)
		p->answer = quest::CONFIRM_YES;
	sys_log(0, "QuestConfirm from %s pid %u name %s answer %d", ch->GetName(), p->requestPID, (ch_wait) ? ch_wait->GetName() : "", p->answer);
	if (ch_wait)
	{
		quest::CQuestManager::Instance().Confirm(ch_wait->GetPlayerID(), (quest::EQuestConfirmType) p->answer, ch->GetPlayerID());
	}
}

void CInputMain::Target(LPCHARACTER ch, const char* pcData)
{
	TPacketCGTarget* p = (TPacketCGTarget*)pcData;

	building::LPOBJECT pkObj = building::CManager::instance().FindObjectByVID(p->dwVID);

	if (pkObj)
	{
		TPacketGCTarget pckTarget;
		pckTarget.header = HEADER_GC_TARGET;
		pckTarget.dwVID = p->dwVID;
		ch->GetDesc()->Packet(&pckTarget, sizeof(TPacketGCTarget));
	}
	else
		ch->SetTarget(CHARACTER_MANAGER::instance().Find(p->dwVID));
}

void CInputMain::Warp(LPCHARACTER ch, const char* pcData)
{
	ch->WarpEnd();
}

void CInputMain::SafeboxCheckin(LPCHARACTER ch, const char* c_pData)
{
	if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning() == true)
		return;

	TPacketCGSafeboxCheckin * p = (TPacketCGSafeboxCheckin*)c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox * pkSafebox = ch->GetSafebox();
	LPITEM pkItem = ch->GetItem(p->ItemPos);

	if (!pkSafebox || !pkItem)
		return;

	if (pkItem->GetType() == ITEM_BELT && pkItem->IsEquipped())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PLEASE_BE_EMPTY_BELT_INVENTORY_WINDOW"));
		return;
	}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (pkItem->GetCell() >= ch->GetExtendInvenMax() && IS_SET(pkItem->GetFlag(), ITEM_FLAG_IRREMOVABLE))
#else
	if (pkItem->GetCell() >= INVENTORY_MAX_NUM && IS_SET(pkItem->GetFlag(), ITEM_FLAG_IRREMOVABLE))
#endif
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í·Î ¿Å±æ ¼ö ¾ø´Â ¾ÆÀÌÅÛ ÀÔ´Ï´Ù."));
		return;
	}

	if (true == pkItem->IsEquipped())
		return;

	if (!pkSafebox->IsEmpty(p->bSafePos, pkItem->GetSize()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
		return;
	}

	if (pkItem->GetVnum() == UNIQUE_ITEM_SAFEBOX_EXPAND)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_SAFEBOX))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (true == pkItem->isLocked())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (pkItem->IsBasicItem())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return;
	}

	pkItem->RemoveFromCharacter();
	if (!pkItem->IsDragonSoul() && !pkItem->IsSkillBook() && !pkItem->IsUpgradeItem() && !pkItem->IsStone() && !pkItem->IsChest() && !pkItem->IsAttr() && !pkItem->IsFlower())
		ch->SyncQuickslot(QUICKSLOT_TYPE_ITEM, p->ItemPos.cell, 1000);

	pkSafebox->Add(p->bSafePos, pkItem);

	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX PUT", szHint);
}

void CInputMain::SafeboxCheckout(LPCHARACTER ch, const char* c_pData, bool bMall)
{
	TPacketCGSafeboxCheckout* p = (TPacketCGSafeboxCheckout*)c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox* pkSafebox;

	if (bMall)
		pkSafebox = ch->GetMall();
	else
		pkSafebox = ch->GetSafebox();

	if (!pkSafebox)
		return;

	LPITEM pkItem = pkSafebox->Get(p->bSafePos);

	if (!pkItem)
		return;

	if (!ch->IsEmptyItemGrid(p->ItemPos, pkItem->GetSize()))
		return;

	for (WORD belt_index = BELT_INVENTORY_SLOT_START; belt_index < BELT_INVENTORY_SLOT_END; ++belt_index)
	{
		if (pkItem->GetType() != 3 && p->ItemPos.cell == belt_index)
		{
			if (pkItem->GetSubType() != 0 || pkItem->GetSubType() != 11 || pkItem->GetSubType() != 7)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Check> Depodan Kemer Envanterine item yerlestiremezsin !"));
				return;
			}
		}
	}

	// ¾ÆÀÌÅÛ ¸ô¿¡¼­ ÀÎº¥À¸·Î ¿Å±â´Â ºÎºÐ¿¡¼­ ¿ëÈ¥¼® Æ¯¼ö Ã³¸®
	// (¸ô¿¡¼­ ¸¸µå´Â ¾ÆÀÌÅÛÀº item_proto¿¡ Á¤ÀÇµÈ´ë·Î ¼Ó¼ºÀÌ ºÙ±â ¶§¹®¿¡,
	//  ¿ëÈ¥¼®ÀÇ °æ¿ì, ÀÌ Ã³¸®¸¦ ÇÏÁö ¾ÊÀ¸¸é ¼Ó¼ºÀÌ ÇÏ³ªµµ ºÙÁö ¾Ê°Ô µÈ´Ù.)
	if (pkItem->IsDragonSoul())
	{
		if (bMall)
		{
			DSManager::instance().DragonSoulItemInitialize(pkItem);
		}

		if (DRAGON_SOUL_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;
		if (!DSManager::instance().IsValidCellForThisItem(pkItem, DestPos))
		{
			int iCell = ch->GetEmptyDragonSoulInventory(pkItem);
			if (iCell < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
				return;
			}
			DestPos = TItemPos(DRAGON_SOUL_INVENTORY, iCell);
		}

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos, false);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsUpgradeItem())
	{
		if (UPGRADE_ITEMS_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyUpgradeItemsInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(UPGRADE_ITEMS_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsSkillBook())
	{
		if (SKILL_BOOK_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptySkillBookInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(SKILL_BOOK_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsStone())
	{
		if (STONE_ITEMS_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyStoneInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(STONE_ITEMS_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsChest())
	{
		if (CHEST_ITEMS_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyChestInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(CHEST_ITEMS_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsAttr())
	{
		if (ATTR_ITEMS_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyAttrInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(ATTR_ITEMS_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsFlower())
	{
		if (FLOWER_ITEMS_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyFlowerInventory(pkItem->GetSize());
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		DestPos = TItemPos(FLOWER_ITEMS_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else
	{
		if (DRAGON_SOUL_INVENTORY == p->ItemPos.window_type ||
			UPGRADE_ITEMS_INVENTORY == p->ItemPos.window_type ||
			SKILL_BOOK_INVENTORY == p->ItemPos.window_type ||
			STONE_ITEMS_INVENTORY == p->ItemPos.window_type ||
			CHEST_ITEMS_INVENTORY == p->ItemPos.window_type ||
			ATTR_ITEMS_INVENTORY == p->ItemPos.window_type ||
			FLOWER_ITEMS_INVENTORY == p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		pkSafebox->Remove(p->bSafePos);
		if (bMall)
		{
			if (nullptr == pkItem->GetProto())
			{
				sys_err("pkItem->GetProto() == nullptr (id : %d)", pkItem->GetID());
				return;
			}
			// 100% È®·ü·Î ¼Ó¼ºÀÌ ºÙ¾î¾ß ÇÏ´Âµ¥ ¾È ºÙ¾îÀÖ´Ù¸é »õ·Î ºÙÈù´Ù. ...............
			if (100 == pkItem->GetProto()->bAlterToMagicItemPct && 0 == pkItem->GetAttributeCount())
			{
				pkItem->AlterToMagicItem();
			}
		}
		pkItem->AddToCharacter(ch, p->ItemPos, false);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}

	DWORD dwID = pkItem->GetID();
	db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_FLUSH, 0, sizeof(DWORD));
	db_clientdesc->Packet(&dwID, sizeof(DWORD));

	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	if (bMall)
		LogManager::instance().ItemLog(ch, pkItem, "MALL GET", szHint);
	else
		LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX GET", szHint);
}

void CInputMain::SafeboxItemMove(LPCHARACTER ch, const char* data)
{
	struct command_item_move* pinfo = (struct command_item_move*) data;

	if (!ch->CanHandleItem())
		return;

	if (!ch->GetSafebox())
		return;

	ch->GetSafebox()->MoveItem(pinfo->Cell.cell, pinfo->CellTo.cell, pinfo->count);
}

// PARTY_JOIN_BUG_FIX
void CInputMain::PartyInvite(LPCHARACTER ch, const char* c_pData)
{
	if (!ch)
		return;
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartyInvite* p = (TPacketCGPartyInvite*)c_pData;

	LPCHARACTER pInvitee = CHARACTER_MANAGER::instance().Find(p->vid);

	if (!pInvitee || !ch->GetDesc() || !pInvitee->GetDesc() || !pInvitee->IsPC() || !ch->IsPC())
	{
		sys_err("PARTY Cannot find invited character");
		return;
	}

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	if (MessengerManager::instance().IsBlocked_Target(ch->GetName(), pInvitee->GetName()))
	{
		return;
	}
	if (MessengerManager::instance().IsBlocked_Me(ch->GetName(), pInvitee->GetName()))
	{
		return;
	}
#endif

	ch->PartyInvite(pInvitee);
}

void CInputMain::PartyInviteAnswer(LPCHARACTER ch, const char* c_pData)
{
	if (!ch)
		return;
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartyInviteAnswer* p = (TPacketCGPartyInviteAnswer*)c_pData;

	LPCHARACTER pInviter = CHARACTER_MANAGER::instance().Find(p->leader_vid);

	// pInviter °¡ ch ¿¡°Ô ÆÄÆ¼ ¿äÃ»À» Çß¾ú´Ù.

	if (!pInviter || !pInviter->IsPC())
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿äÃ»À» ÇÑ Ä³¸¯ÅÍ¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù."));
	else if (!p->accept)
		pInviter->PartyInviteDeny(ch->GetPlayerID());
	else
		pInviter->PartyInviteAccept(ch);
}
// END_OF_PARTY_JOIN_BUG_FIX

void CInputMain::PartySetState(LPCHARACTER ch, const char* c_pData)
{
	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartySetState* p = (TPacketCGPartySetState*)c_pData;

	if (!ch->GetParty())
		return;

	if (ch->GetParty()->GetLeaderPID() != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¸®´õ¸¸ º¯°æÇÒ ¼ö ÀÖ½À´Ï´Ù."));
		return;
	}

	if (!ch->GetParty()->IsMember(p->pid))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> »óÅÂ¸¦ º¯°æÇÏ·Á´Â »ç¶÷ÀÌ ÆÄÆ¼¿øÀÌ ¾Æ´Õ´Ï´Ù."));
		return;
	}

	DWORD pid = p->pid;
	sys_log(0, "PARTY SetRole pid %d to role %d state %s", pid, p->byRole, p->flag ? "on" : "off");

	switch (p->byRole)
	{
	case PARTY_ROLE_NORMAL:
		break;

	case PARTY_ROLE_ATTACKER:
	case PARTY_ROLE_TANKER:
	case PARTY_ROLE_BUFFER:
	case PARTY_ROLE_SKILL_MASTER:
	case PARTY_ROLE_HASTE:
	case PARTY_ROLE_DEFENDER:
		if (ch->GetParty()->SetRole(pid, p->byRole, p->flag))
		{
			TPacketPartyStateChange pack;
			pack.dwLeaderPID = ch->GetPlayerID();
			pack.dwPID = p->pid;
			pack.bRole = p->byRole;
			pack.bFlag = p->flag;
			db_clientdesc->DBPacket(HEADER_GD_PARTY_STATE_CHANGE, 0, &pack, sizeof(pack));
		}
		/* else
		   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¾îÅÂÄ¿ ¼³Á¤¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù.")); */
		break;

	default:
		sys_err("wrong byRole in PartySetState Packet name %s state %d", ch->GetName(), p->byRole);
		break;
	}
}

void CInputMain::PartyRemove(LPCHARACTER ch, const char* c_pData)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÀü ¾È¿¡¼­´Â ÆÄÆ¼¿¡¼­ Ãß¹æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (ch->GetZodiac() || (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zodyak tapinaginda grubu bozamazsin."));
		return;
	}
#endif

	TPacketCGPartyRemove* p = (TPacketCGPartyRemove*)c_pData;

	if (!ch->GetParty())
		return;

	LPPARTY pParty = ch->GetParty();
	if (pParty->GetLeaderPID() == ch->GetPlayerID())
	{
		if (ch->GetDungeon())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÁ¯³»¿¡¼­´Â ÆÄÆ¼¿øÀ» Ãß¹æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		else
		{
			// leader can remove any member
			if (p->pid == ch->GetPlayerID() || pParty->GetMemberCount() == 2)
			{
				// party disband
				CPartyManager::instance().DeleteParty(pParty);
			}
			else
			{
				LPCHARACTER B = CHARACTER_MANAGER::instance().FindByPID(p->pid);
				if (B)
				{
					//pParty->SendPartyRemoveOneToAll(B);
					B->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ Ãß¹æ´çÇÏ¼Ì½À´Ï´Ù."));
					//pParty->Unlink(B);
					//CPartyManager::instance().SetPartyMember(B->GetPlayerID(), nullptr);
				}
				pParty->Quit(p->pid);
			}
		}
	}
	else
	{
		// otherwise, only remove itself
		if (p->pid == ch->GetPlayerID())
		{
			if (ch->GetDungeon())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÁ¯³»¿¡¼­´Â ÆÄÆ¼¸¦ ³ª°¥ ¼ö ¾ø½À´Ï´Ù."));
			}
			else
			{
				if (pParty->GetMemberCount() == 2)
				{
					// party disband
					CPartyManager::instance().DeleteParty(pParty);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
					//pParty->SendPartyRemoveOneToAll(ch);
					pParty->Quit(ch->GetPlayerID());
					//pParty->SendPartyRemoveAllToOne(ch);
					//CPartyManager::instance().SetPartyMember(ch->GetPlayerID(), nullptr);
				}
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´Ù¸¥ ÆÄÆ¼¿øÀ» Å»Åð½ÃÅ³ ¼ö ¾ø½À´Ï´Ù."));
		}
	}
}

void CInputMain::AnswerMakeGuild(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGAnswerMakeGuild* p = (TPacketCGAnswerMakeGuild*)c_pData;

	if (ch->GetGold() < 200000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Hata> Yetersiz yang!"));
		return;
	}

	if (ch->GetLevel() < 40)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Hata> Yetersiz seviye!"));
		return;
	}

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_disband_time") <
		CGuildManager::instance().GetDisbandDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÇØ»êÇÑ ÈÄ %dÀÏ ÀÌ³»¿¡´Â ±æµå¸¦ ¸¸µé ¼ö ¾ø½À´Ï´Ù."),
			quest::CQuestManager::instance().GetEventFlag("guild_disband_delay"));
		return;
	}

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_withdraw_time") <
		CGuildManager::instance().GetWithdrawDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Å»ÅðÇÑ ÈÄ %dÀÏ ÀÌ³»¿¡´Â ±æµå¸¦ ¸¸µé ¼ö ¾ø½À´Ï´Ù."),
			quest::CQuestManager::instance().GetEventFlag("guild_withdraw_delay"));
		return;
	}

	if (ch->GetGuild())
		return;

	CGuildManager& gm = CGuildManager::instance();

	TGuildCreateParameter cp;
	memset(&cp, 0, sizeof(cp));

	cp.master = ch;
	strlcpy(cp.name, p->guild_name, sizeof(cp.name));

	if (cp.name[0] == 0 || !check_name(cp.name))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀûÇÕÇÏÁö ¾ÊÀº ±æµå ÀÌ¸§ ÀÔ´Ï´Ù."));
		return;
	}

	DWORD dwGuildID = gm.CreateGuild(cp);

	if (dwGuildID)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> [%s] ±æµå°¡ »ý¼ºµÇ¾ú½À´Ï´Ù."), cp.name);

		int GuildCreateFee = 200000;
		ch->PointChange(POINT_GOLD, -GuildCreateFee);
		DBManager::instance().SendMoneyLog(MONEY_LOG_GUILD, ch->GetPlayerID(), -GuildCreateFee);

		char Log[128];
		snprintf(Log, sizeof(Log), "GUILD_NAME %s MASTER %s", cp.name, ch->GetName());
		LogManager::instance().CharLog(ch, 0, "MAKE_GUILD", Log);
		DBManager::instance().Query("INSERT INTO lonca_gecmis%s (name, guild) values('%s', '%s')", get_table_postfix(), ch->GetName(), cp.name);

		ch->RemoveSpecifyItem(GUILD_CREATE_ITEM_VNUM, 1);
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå »ý¼º¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
}

void CInputMain::PartyUseSkill(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGPartyUseSkill* p = (TPacketCGPartyUseSkill*)c_pData;
	if (!ch->GetParty())
		return;

	if (ch->GetPlayerID() != ch->GetParty()->GetLeaderPID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼ ±â¼úÀº ÆÄÆ¼Àå¸¸ »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù."));
		return;
	}

	switch (p->bySkillIndex)
	{
	case PARTY_SKILL_HEAL:
		ch->GetParty()->HealParty();
		break;
	case PARTY_SKILL_WARP:
	{
		LPCHARACTER pch = CHARACTER_MANAGER::instance().Find(p->vid);
		if (pch)
		{
			if (pch->GetMapIndex() != ch->GetMapIndex())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yanina getirmek istedigin oyuncu ile ayni haritada olmalisin!"));
				return;
			}
			if (pch->IsPC())
				ch->GetParty()->SummonToLeader(pch->GetPlayerID());
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼ÒÈ¯ÇÏ·Á´Â ´ë»óÀ» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
	}
	break;
	}
}

void CInputMain::PartyParameter(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGPartyParameter* p = (TPacketCGPartyParameter*)c_pData;

	if (ch->GetParty())
		ch->GetParty()->SetParameter(p->bDistributeMode);
}

size_t GetSubPacketSize(const GUILD_SUBHEADER_CG & header)
{
	switch (header)
	{
	case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:				return sizeof(int);
	case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:				return sizeof(int);
	case GUILD_SUBHEADER_CG_ADD_MEMBER:					return sizeof(DWORD);
	case GUILD_SUBHEADER_CG_REMOVE_MEMBER:				return sizeof(DWORD);
	case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:			return 10;
	case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:		return sizeof(BYTE) + sizeof(BYTE);
	case GUILD_SUBHEADER_CG_OFFER:						return sizeof(DWORD);
	case GUILD_SUBHEADER_CG_CHARGE_GSP:					return sizeof(int);
	case GUILD_SUBHEADER_CG_POST_COMMENT:				return 1;
	case GUILD_SUBHEADER_CG_DELETE_COMMENT:				return sizeof(DWORD);
	case GUILD_SUBHEADER_CG_REFRESH_COMMENT:			return 0;
	case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:		return sizeof(DWORD) + sizeof(BYTE);
	case GUILD_SUBHEADER_CG_USE_SKILL:					return sizeof(TPacketCGGuildUseSkill);
	case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:		return sizeof(DWORD) + sizeof(BYTE);
	case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:		return sizeof(DWORD) + sizeof(BYTE);
	}

	return 0;
}

int CInputMain::Guild(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGGuild))
		return -1;

	const TPacketCGGuild* p = reinterpret_cast<const TPacketCGGuild*>(data);
	const char* c_pData = data + sizeof(TPacketCGGuild);

	uiBytes -= sizeof(TPacketCGGuild);

	const GUILD_SUBHEADER_CG SubHeader = static_cast<GUILD_SUBHEADER_CG>(p->subheader);
	const size_t SubPacketLen = GetSubPacketSize(SubHeader);

	if (uiBytes < SubPacketLen)
	{
		return -1;
	}

	CGuild* pGuild = ch->GetGuild();

	if (nullptr == pGuild)
	{
		if (SubHeader != GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿¡ ¼ÓÇØÀÖÁö ¾Ê½À´Ï´Ù."));
			return SubPacketLen;
		}
	}

	switch (SubHeader)
	{
	case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:
	{
		// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ ³ÖÀ» ¼ö ¾ø´Ù.
		return SubPacketLen;

		const int gold = MIN(*reinterpret_cast<const int*>(c_pData), __deposit_limit());

		if (gold < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àß¸øµÈ ±Ý¾×ÀÔ´Ï´Ù."));
			return SubPacketLen;
		}

		if (ch->GetGold() < gold)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °¡Áö°í ÀÖ´Â µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
			return SubPacketLen;
		}

		pGuild->RequestDepositMoney(ch, gold);
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:
	{
		// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ »¬ ¼ö ¾ø´Ù.
		return SubPacketLen;

		const int gold = MIN(*reinterpret_cast<const int*>(c_pData), 500000);

		if (gold < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àß¸øµÈ ±Ý¾×ÀÔ´Ï´Ù."));
			return SubPacketLen;
		}

		pGuild->RequestWithdrawMoney(ch, gold);
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_ADD_MEMBER:
	{
		const DWORD vid = *reinterpret_cast<const DWORD*>(c_pData);
		LPCHARACTER newmember = CHARACTER_MANAGER::instance().Find(vid);

		if (!newmember)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·¯ÇÑ »ç¶÷À» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
			return SubPacketLen;
		}

		if (!newmember->IsPC())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yapilan islem engellendi ve kayit altina alindi"));
			return SubPacketLen;
		}
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
		if (MessengerManager::instance().IsBlocked_Target(ch->GetName(), newmember->GetName()))
		{
			return SubPacketLen;
		}
		if (MessengerManager::instance().IsBlocked_Me(ch->GetName(), newmember->GetName()))
		{
			return SubPacketLen;
		}
#endif

		pGuild->Invite(ch, newmember);
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_REMOVE_MEMBER:
	{
		if (pGuild->UnderAnyWar() != 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü Áß¿¡´Â ±æµå¿øÀ» Å»Åð½ÃÅ³ ¼ö ¾ø½À´Ï´Ù."));
			return SubPacketLen;
		}

		const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
		const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		LPCHARACTER member = CHARACTER_MANAGER::instance().FindByPID(pid);

		if (member)
		{
			if (member->GetGuild() != pGuild)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æÀÌ °°Àº ±æµå°¡ ¾Æ´Õ´Ï´Ù."));
				return SubPacketLen;
			}

			if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÅ³ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				return SubPacketLen;
			}

			member->SetQuestFlag("guild_manage.new_withdraw_time", get_global_time());
			pGuild->RequestRemoveMember(member->GetPlayerID());
		}
		else
		{
			if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÅ³ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				return SubPacketLen;
			}

			if (pGuild->RequestRemoveMember(pid))
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÄ×½À´Ï´Ù."));
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·¯ÇÑ »ç¶÷À» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:
	{
		char gradename[GUILD_GRADE_NAME_MAX_LEN + 1];
		strlcpy(gradename, c_pData + 1, sizeof(gradename));

		const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (m->grade != GUILD_LEADER_GRADE)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§ ÀÌ¸§À» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		}
		else if (*c_pData == GUILD_LEADER_GRADE)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ Á÷À§ ÀÌ¸§Àº º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		else if (!check_name(gradename))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀûÇÕÇÏÁö ¾ÊÀº Á÷À§ ÀÌ¸§ ÀÔ´Ï´Ù."));
		}
		else
		{
			pGuild->ChangeGradeName(*c_pData, gradename);
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:
	{
		const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (m->grade != GUILD_LEADER_GRADE)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§ ±ÇÇÑÀ» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		}
		else if (*c_pData == GUILD_LEADER_GRADE)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ ±ÇÇÑÀº º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		else
		{
			pGuild->ChangeGradeAuth(*c_pData, *(c_pData + 1));
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_OFFER:
	{
		DWORD offer = *reinterpret_cast<const DWORD*>(c_pData);

		if (pGuild->GetLevel() >= GUILD_MAX_LEVEL)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå°¡ ÀÌ¹Ì ÃÖ°í ·¹º§ÀÔ´Ï´Ù."));
		}
		else
		{
			offer /= 100;
			offer *= 100;

			if (pGuild->OfferExp(ch, offer))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> %uÀÇ °æÇèÄ¡¸¦ ÅõÀÚÇÏ¿´½À´Ï´Ù."), offer);
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °æÇèÄ¡ ÅõÀÚ¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
			}
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_CHARGE_GSP:
	{
		const int offer = *reinterpret_cast<const int*>(c_pData);
		const int gold = offer * 100;

		if (offer < 0 || gold < offer || gold < 0 || ch->GetGold() < gold)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
			return SubPacketLen;
		}

		if (!pGuild->ChargeSP(ch, offer))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ¿ë½Å·Â È¸º¹¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_POST_COMMENT:
	{
		const size_t length = *c_pData;

		if (length > GUILD_COMMENT_MAX_LEN)
		{
			// Àß¸øµÈ ±æÀÌ.. ²÷¾îÁÖÀÚ.
			sys_err("POST_COMMENT: %s comment too long (length: %u)", ch->GetName(), length);
			ch->GetDesc()->SetPhase(PHASE_CLOSE);
			return -1;
		}

		if (uiBytes < 1 + length)
			return -1;

		const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (length && !pGuild->HasGradeAuth(m->grade, GUILD_AUTH_NOTICE) && *(c_pData + 1) == '!')
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °øÁö±ÛÀ» ÀÛ¼ºÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		}
		else
		{
			std::string str(c_pData + 1, length);
			pGuild->AddComment(ch, str);
		}

		return (1 + length);
	}

	case GUILD_SUBHEADER_CG_DELETE_COMMENT:
	{
		const DWORD comment_id = *reinterpret_cast<const DWORD*>(c_pData);

		pGuild->DeleteComment(ch, comment_id);
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_REFRESH_COMMENT:
		pGuild->RefreshComment(ch);
		return SubPacketLen;

	case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:
	{
		const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
		const BYTE grade = *(c_pData + sizeof(DWORD));
		const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (m->grade != GUILD_LEADER_GRADE)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§¸¦ º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		else if (ch->GetPlayerID() == pid)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ Á÷À§´Â º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		else if (grade == 1)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀ¸·Î Á÷À§¸¦ º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		else
			pGuild->ChangeMemberGrade(pid, grade);
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_USE_SKILL:
	{
		const TPacketCGGuildUseSkill* p = reinterpret_cast<const TPacketCGGuildUseSkill*>(c_pData);
		
		const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (m->grade != GUILD_LEADER_GRADE)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LONCA_LIDERI_OLMADAN_SKILL_KULLANAMAZSIN"));
		else
		{
			pGuild->UseSkill(p->dwVnum, ch, p->dwPID);
			if (ch->GetWarMap())
				ch->GetWarMap()->UseSkill(p->dwVnum, ch->GetGuild()->GetID());
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:
	{
		const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
		const BYTE is_general = *(c_pData + sizeof(DWORD));
		const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

		if (nullptr == m)
			return -1;

		if (m->grade != GUILD_LEADER_GRADE)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àå±ºÀ» ÁöÁ¤ÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		}
		else
		{
			if (!pGuild->ChangeMemberGeneral(pid, is_general))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ´õÀÌ»ó Àå¼ö¸¦ ÁöÁ¤ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			}
		}
	}
	return SubPacketLen;

	case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:
	{
		const DWORD guild_id = *reinterpret_cast<const DWORD*>(c_pData);
		const BYTE accept = *(c_pData + sizeof(DWORD));

		CGuild * g = CGuildManager::instance().FindGuild(guild_id);

		if (g)
		{
			if (accept)
				g->InviteAccept(ch);
			else
				g->InviteDeny(ch->GetPlayerID());
		}
	}
	return SubPacketLen;
	}

	return 0;
}

void CInputMain::Fishing(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGFishing* p = (TPacketCGFishing*)c_pData;
	ch->SetRotation(p->dir * 5);
	ch->fishing();
	return;
}

void CInputMain::ItemGive(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGGiveItem* p = (TPacketCGGiveItem*)c_pData;
	LPCHARACTER to_ch = CHARACTER_MANAGER::instance().Find(p->dwTargetVID);

	if (to_ch)
		ch->GiveItem(to_ch, p->ItemPos);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛÀ» °Ç³×ÁÙ ¼ö ¾ø½À´Ï´Ù."));
}

void CInputMain::Hack(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGHack* p = (TPacketCGHack*)c_pData;

	char buf[sizeof(p->szBuf)];
	strlcpy(buf, p->szBuf, sizeof(buf));

	//sys_err("HACK_DETECT: %s %s", ch->GetName(), buf);

	// ÇöÀç Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÀÌ ÆÐÅ¶À» º¸³»´Â °æ¿ì°¡ ¾øÀ¸¹Ç·Î ¹«Á¶°Ç ²÷µµ·Ï ÇÑ´Ù
	ch->GetDesc()->SetPhase(PHASE_CLOSE);
}

long long CInputMain::MyShop(LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	TPacketCGMyShop* p = (TPacketCGMyShop*)c_pData;
	long long iExtraLen = p->bCount * sizeof(TShopItemTable);
#ifdef SHOP_BLOCK_GAME99
	if (g_bChannel >= 99 & ch->GetGMLevel() <= 0) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_OX_ERROR"));
		return (iExtraLen);
	}
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (quest::CQuestManager::instance().GetEventFlag("shop_off") == 1)
		if (ch)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_TEMP_OFF"));
			return (iExtraLen);
		}
#endif
#ifdef SHOP_GM_PRIVILEGES
	if (GM_PLAYER < ch->GetGMLevel() && SHOP_GM_PRIVILEGES >ch->GetGMLevel()) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_PRIV_ERROR"));
		return (iExtraLen);
	}
#endif
	if (uiBytes < sizeof(TPacketCGMyShop) + iExtraLen)
		return -1;

	if (strstr(p->szSign, ("|c")) || strstr(p->szSign, ("|C"))) {
		LogManager::instance().HackLog("COLORFUL_SHOP", ch);
		return (iExtraLen);
	}

	if (ch->GetGold() >= GOLD_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÀ¯ µ·ÀÌ 20¾ï³ÉÀ» ³Ñ¾î °Å·¡¸¦ ÇÛ¼ö°¡ ¾ø½À´Ï´Ù."));
		sys_log(0, "MyShop ==> OverFlow Gold id %u name %s ", ch->GetPlayerID(), ch->GetName());
		return (iExtraLen);
	}

	if (ch->IsStun() || ch->IsDead())
		return (iExtraLen);

	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
		return (iExtraLen);
	}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
		return (iExtraLen);
	}
#endif

	sys_log(0, "MyShop count %d", p->bCount);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	ch->OpenMyShop(p->szSign, (TShopItemTable*)(c_pData + sizeof(TPacketCGMyShop)), p->bCount, p->days, p->model);
#else
	ch->OpenMyShop(p->szSign, (TShopItemTable*)(c_pData + sizeof(TPacketCGMyShop)), p->bCount);
#endif
	return (iExtraLen);
}

void CInputMain::Refine(LPCHARACTER ch, const char* c_pData)
{
	const TPacketCGRefine* p = reinterpret_cast<const TPacketCGRefine*>(c_pData);

	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
		ch->ClearRefineMode();
		return;
	}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
		ch->ClearRefineMode();
		return;
	}
#endif

	if (p->type == 255)
	{
		// DoRefine Cancel
		ch->ClearRefineMode();
		return;
	}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	if (p->pos >= ch->GetExtendInvenMax())
#else
	if (p->pos >= INVENTORY_MAX_NUM)
#endif
	{
		ch->ClearRefineMode();
		return;
	}

	LPITEM item = ch->GetInventoryItem(p->pos);

	if (!item)
	{
		ch->ClearRefineMode();
		return;
	}
	
	// 09/09/2019 Mythsia Deviltower Minor Bug Fix
	const TRefineTable * prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
	if (!prt)
	{
		ch->ClearRefineMode();
		return;
	}

#ifdef ENABLE_BASIC_ITEM_SYSTEM
	if (item->IsBasicItem())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		ch->ClearRefineMode();
		return;
	}
#endif

	ch->SetRefineTime();

	if (p->type == REFINE_TYPE_NORMAL)
	{
		sys_log(0, "refine_type_normal");
		ch->DoRefine(item);
	}
	else if (p->type == REFINE_TYPE_SCROLL || p->type == REFINE_TYPE_HYUNIRON || p->type == REFINE_TYPE_MUSIN || p->type == REFINE_TYPE_BDRAGON || p->type == REFINE_TYPE_RITUALS_SCROLL)
	{
		sys_log(0, "refine_type_scroll, ...");
		ch->DoRefineWithScroll(item);
	}
	else if (p->type == REFINE_TYPE_MONEY_ONLY)
	{
		const LPITEM item = ch->GetInventoryItem(p->pos);

		if (nullptr != item)
		{
			if (500 <= item->GetRefineSet())
			{
				LogManager::instance().HackLog("DEVIL_TOWER_REFINE_HACK", ch);
			}
			else
			{
				if (ch->GetQuestFlag("deviltower_zone.can_refine") > 0) // @fixme158 (allow multiple refine attempts)
				{
					// 09/09/2019 Mythsia Deviltower Minor Bug Fix
					// Add GetGold Control For Refine.
                    if (ch->GetGold() >= prt->cost)
                    {
						if (ch->DoRefine(item, true))
							ch->SetQuestFlag("deviltower_zone.can_refine", ch->GetQuestFlag("deviltower_zone.can_refine") - 1);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "»ç±Í Å¸¿ö ¿Ï·á º¸»óÀº ÇÑ¹ø±îÁö »ç¿ë°¡´ÉÇÕ´Ï´Ù.");
				}
			}
		}
	}

	ch->ClearRefineMode();
}

#ifdef ENABLE_ACCE_SYSTEM
void CInputMain::Sash(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC* pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;
	
	if (!features::IsFeatureEnabled(features::ACCE))
	{
		LogManager::instance().HackLog("ACCEDISABLED", pkChar);
		return;
	}

	TPacketSash* sPacket = (TPacketSash*)c_pData;
	switch (sPacket->subheader)
	{
	case SASH_SUBHEADER_CG_CLOSE:
	{
		pkChar->CloseSash();
	}
	break;
	case SASH_SUBHEADER_CG_ADD:
	{
		pkChar->AddSashMaterial(sPacket->tPos, sPacket->bPos);
	}
	break;
	case SASH_SUBHEADER_CG_REMOVE:
	{
		pkChar->RemoveSashMaterial(sPacket->bPos);
	}
	break;
	case SASH_SUBHEADER_CG_REFINE:
	{
		pkChar->RefineSashMaterials();
	}
	break;
	default:
		break;
	}
}
#endif

#ifdef ENABLE_YOUTUBER_SYSTEM
void CInputMain::YoutubePanel(LPCHARACTER ch, const char* c_pData)
{
	if (quest::CQuestManager::instance().GetEventFlag("disable_youtube_system") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Sistem suan icin GM tarafindan devre disi birakildi!");
		return;
	}

	if (!ch)
		return;

	TPacketYoutuberPanel* p = (TPacketYoutuberPanel*)c_pData;
	ch->YoutubeYonetimPaneli(p->arg1, p->arg2, p->arg3);
}

void CInputMain::YoutuberVideo(LPCHARACTER ch, const char* c_pData)
{
	if (quest::CQuestManager::instance().GetEventFlag("disable_youtube_system") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Sistem suan icin GM tarafindan devre disi birakildi!");
		return;
	}

	if (!ch)
		return;

	TPacketYoutubeVideo* p = (TPacketYoutubeVideo*)c_pData;
	ch->YoutubeVideoListesi(p->arg1, p->arg2);
}
#endif

#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
void CInputMain::ChestDropInfo(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGChestDropInfo* p = (TPacketCGChestDropInfo*)c_pData;

	if (p->wInventoryCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
		return;

	LPITEM pkItem = ch->GetItem(TItemPos(CHEST_ITEMS_INVENTORY, p->wInventoryCell));

	if (!pkItem)
		return;

	std::vector<TChestDropInfoTable> vec_ItemList;
	ITEM_MANAGER::instance().GetChestItemList(pkItem->GetVnum(), vec_ItemList);

	if (vec_ItemList.size() == 0)
		return;

	TPacketGCChestDropInfo packet;
	packet.bHeader = HEADER_GC_CHEST_DROP_INFO;
	packet.wSize = sizeof(packet) + sizeof(TChestDropInfoTable) * vec_ItemList.size();
	packet.dwChestVnum = pkItem->GetVnum();

	ch->GetDesc()->BufferedPacket(&packet, sizeof(packet));
	ch->GetDesc()->Packet(&vec_ItemList[0], sizeof(TChestDropInfoTable) * vec_ItemList.size());
}
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
int CInputMain::FishEvent(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	TPacketCGFishEvent* p = (TPacketCGFishEvent*)data;

	if (uiBytes < sizeof(TPacketCGFishEvent))
		return -1;

	const char* c_pData = data + sizeof(TPacketCGFishEvent);
	uiBytes -= sizeof(TPacketCGFishEvent);

	switch (p->bSubheader)
	{
	case FISH_EVENT_SUBHEADER_BOX_USE:
	{
		if (uiBytes < sizeof(BYTE) + sizeof(WORD))
			return -1;

		BYTE bWindow = *(c_pData++);
		WORD wCell = *(c_pData);

		sys_log(0, "INPUT: %s FISH_EVENT: USE_BOX", ch->GetName());

		ch->FishEventUseBox(TItemPos(bWindow, wCell));
		return (sizeof(BYTE) + sizeof(WORD));
	}

	case FISH_EVENT_SUBHEADER_SHAPE_ADD:
	{
		if (uiBytes < sizeof(BYTE))
			return -1;

		BYTE shapePos = *c_pData;

		sys_log(0, "INPUT: %s FISH_EVENT: ADD_SHAPE", ch->GetName());

		ch->FishEventAddShape(shapePos);
		return sizeof(BYTE);
	}

	default:
		sys_err("CInputMain::FishEvent : Unknown subheader %d : %s", p->bSubheader, ch->GetName());
		break;
	}

	return 0;
}
#endif

#ifdef ENABLE_GEM_SYSTEM
int CInputMain::GemShop(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	TPacketCGGemShop* p = (TPacketCGGemShop*)data;

	if (uiBytes < sizeof(TPacketCGGemShop))
		return -1;

	const char* c_pData = data + sizeof(TPacketCGGemShop);
	uiBytes -= sizeof(TPacketCGGemShop);
	
	if (!features::IsFeatureEnabled(features::GEM))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GEM_SYSTEM_DISABLED"));
		return 0;		
	}

	switch (p->subheader)
	{
	case GEM_SHOP_SUBHEADER_CG_BUY:
	{
		if (uiBytes < sizeof(BYTE))
			return -1;

		BYTE bPos = *c_pData;

		sys_log(1, "INPUT: %s GEM_SHOP: BUY %d", ch->GetName(), bPos);
		ch->GemShopBuy(bPos);
		return sizeof(BYTE);
	}

	case GEM_SHOP_SUBHEADER_CG_ADD:
	{
		if (uiBytes < sizeof(BYTE))
			return -1;

		BYTE bPos = *c_pData;

		sys_log(1, "INPUT: %s GEM_SHOP: ADD %d", ch->GetName(), bPos);
		ch->GemShopAdd(bPos);
		return sizeof(BYTE);
	}

	case GEM_SHOP_SUBHEADER_CG_REFRESH:
		sys_log(1, "INPUT: %s GEM_SHOP: REFRESH", ch->GetName());
		ch->RefreshGemShopWithItem();
		return 0;

	default:
		sys_err("CInputMain::GemShop() : Unknown subheader %d : %s", p->subheader, ch->GetName());
		break;
	}

	return 0;
}
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM
void CInputMain::MailBoxSend(LPCHARACTER ch, const char* data)
{
	if (!features::IsFeatureEnabled(features::AVERAGE_PRICE))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("AVERAGE_PRICE_SYSTEM_DISABLED"));
		return;		
	}

	struct packet_send_mail_box* pinfo = (struct packet_send_mail_box*) data;
	switch (pinfo->subheader)
	{
	case MAILBOX_SUB_HEADER_CLOSE:
	{
		ch->SetOpenMailBox(false);
	}
	break;

	case MAILBOX_SUB_HEADER_CHECK_NAME:
	{
		const char* nombre_pj = pinfo->nombre_pj;
		if (strlen(nombre_pj) == 0) { return; }
		ch->CheckNameMailBox(nombre_pj);
	}
	break;

	case MAILBOX_SUB_HEADER_ACCEPT_MAIL:
	{
		int id_mail = pinfo->id_mail;
		ch->AcceptMail(id_mail);
	}
	break;

	case MAILBOX_SUB_HEADER_DELETE_MAIL:
	{
		int id_mail = pinfo->id_mail;
		ch->DeleteMail(id_mail);
	}
	break;

	case MAILBOX_SUB_HEADER_DELETE_ALL_MAIL: {
		ch->DeleteAllMails();
	}
											 break;

	case MAILBOX_SUB_HEADER_ACCEPT_ALL_MAIL: {
		ch->AcceptAllMails();
	}
											 break;

	case MAILBOX_SUB_HEADER_VIEW_MAIL: {
		int id_mail = pinfo->id_mail;
		ch->CheckViewMail(id_mail);
	}
									   break;

	case MAILBOX_SUB_HEADER_SEND_MAIL:
	{
		if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
			return;

		const char* nombre_pj = pinfo->nombre_pj;
		int slot_item = pinfo->slot_item;
		int yang = pinfo->yang;
#ifdef USE_WON_SYSTEM_MAILBOX
		int won = pinfo->won;
#endif
		char descrip_new[100];
		char asunto_new[25];

		strlcpy(descrip_new, pinfo->descrip, sizeof(descrip_new));
		strlcpy(asunto_new, pinfo->asunto, sizeof(asunto_new));

		if (strlen(nombre_pj) == 0) { ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] No ha colocado ninguno nombre.")); return; }
		if (strlen(asunto_new) == 0) { ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] No ha colocado ninguno asunto.")); return; }
		if (strlen(descrip_new) == 0) { ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] No ha colocado ninguna descripcion.")); return; }

#ifdef USE_WON_SYSTEM_MAILBOX
		if (yang < 0 || won < 0) { ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] Error, Cantidad negativa.")); return; }
#else
		if (yang < 0) { ch->ChatPacket(CHAT_TYPE_INFO, "[MailBox] Error, Cantidad negativa."); return; }
#endif
#ifdef USE_WON_SYSTEM_MAILBOX
		ch->SendMailBox(nombre_pj, asunto_new, descrip_new, slot_item, yang, won);
#else
		ch->SendMailBox(nombre_pj, asunto_new, descrip_new, slot_item, yang);
#endif
	}
	break;
	}
	return;
}

#endif

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
void CInputMain::SafeboxMoney(LPCHARACTER ch, const char * data)
{
	CSafebox* sb = ch->GetSafebox();
	if (sb == nullptr)
		return;

	TPacketCGSafeboxMoney * p = (TPacketCGSafeboxMoney *)data;

	if (p->llMoney < 0)
		return;
	
	if (!features::IsFeatureEnabled(features::SAFEBOX_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_MONEY_SYSTEM_DISABLED"));
		return;		
	}

	switch (p->bState)
	{
		case SAFEBOX_MONEY_STATE_SAVE:
		{
			if (ch->GetGold() < p->llMoney)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_NOT_ENOUGH_GOLD"));
				return;
			}

			if (p->llMoney + sb->GetGold() > GOLD_MAX) // @fixpch
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_MONEY_MUCH"));
				return;
			}

			TSafeboxMoney pack;
			pack.dwAID = ch->GetAID();
			pack.bType = 1;
			pack.llAmount = p->llMoney;
			db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_MONEY, 0, &pack, sizeof(pack));

			sys_log(0, "Safebox save money from: %u amount: %lld", ch->GetAID(), p->llMoney);
			ch->PointChange(POINT_GOLD, -p->llMoney);
		}
		break;

		case SAFEBOX_MONEY_STATE_WITHDRAW:
		{
			if (sb->GetGold() < p->llMoney)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_NOT_ENOUGH_GOLD2"));
				return;
			}
			if (p->llMoney > GOLD_MAX)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_MAX_GOLD"));
				return;
			}
			if (p->llMoney + ch->GetGold() > GOLD_MAX)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_MAX_GOLD2"));
				return;
			}
			
			TSafeboxMoney pack;
			pack.dwAID = ch->GetAID();
			pack.bType = 0;
			pack.llAmount = p->llMoney;
			db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_MONEY, 0, &pack, sizeof(pack));

			sys_log(0, "Safebox withdraw money from: %u amount: %lld", ch->GetAID(), p->llMoney);
			ch->PointChange(POINT_GOLD, p->llMoney);
		}
		break;
	}
}

void CInputMain::SafeboxCheque(LPCHARACTER ch, const char * data)
{
	CSafebox* sb = ch->GetSafebox();
	if (sb == nullptr)
		return;

	TPacketCGSafeboxCheque * p = (TPacketCGSafeboxCheque *)data;

	if (p->wCheque < 0)
		return;
	
	if (!features::IsFeatureEnabled(features::SAFEBOX_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_MONEY_SYSTEM_DISABLED"));
		return;		
	}

	switch (p->bState)
	{
		case SAFEBOX_CHEQUE_STATE_SAVE:
		{
			if (ch->GetCheque() < p->wCheque)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_NOT_ENOUGH_CHEQUE"));
				return;
			}

			if (p->wCheque + sb->GetCheque() > CHEQUE_MAX) // @fixpch
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_CHEQUE_MUCH"));
				return;
			}
			
			TSafeboxCheque pack;
			pack.dwAID = ch->GetAID();
			pack.bType = 1;
			pack.wAmount = p->wCheque;
			db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHEQUE, 0, &pack, sizeof(pack));

			sys_log(0, "Safebox save cheque from: %u amount: %d", ch->GetAID(), p->wCheque);
			ch->PointChange(POINT_CHEQUE, -p->wCheque);
		}
		break;

		case SAFEBOX_CHEQUE_STATE_WITHDRAW:
		{
			if (sb->GetCheque() < p->wCheque)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_NOT_ENOUGH_CHEQUE2"));
				return;
			}
			if (p->wCheque > CHEQUE_MAX)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_MAX_CHEQUE"));
				return;
			}
			if (p->wCheque + ch->GetCheque() > CHEQUE_MAX)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SAFEBOX_SYSTEM_MAX_CHEQUE2"));
				return;
			}
			TSafeboxCheque pack;
			pack.dwAID = ch->GetAID();
			pack.bType = 0;
			pack.wAmount = p->wCheque;
			db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHEQUE, 0, &pack, sizeof(pack));

			sys_log(0, "Safebox withdraw cheque from: %u amount: %d", ch->GetAID(), p->wCheque);
			ch->PointChange(POINT_CHEQUE, p->wCheque);
		}
		break;
	}
}
#endif

#ifdef ENABLE_AURA_SYSTEM
void CInputMain::Aura(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC* pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;
	
	if (!features::IsFeatureEnabled(features::AURA))
	{
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("AURA_SYSTEM_DISABLED"));
		return;		
	}

	TPacketAura* sPacket = (TPacketAura*)c_pData;
	switch (sPacket->subheader)
	{
	case AURA_SUBHEADER_CG_CLOSE:
	{
		pkChar->CloseAura();
	}
	break;
	case AURA_SUBHEADER_CG_ADD:
	{
		pkChar->AddAuraMaterial(sPacket->tPos, sPacket->bPos);
	}
	break;
	case AURA_SUBHEADER_CG_REMOVE:
	{
		pkChar->RemoveAuraMaterial(sPacket->bPos);
	}
	break;
	case AURA_SUBHEADER_CG_REFINE:
	{
		pkChar->RefineAuraMaterials();
	}
	break;
	default:
		break;
	}
}
#endif

#ifdef ENABLE_AVERAGE_ITEM_PRICE
void CInputMain::AverageSearch(LPCHARACTER ch, const char * data)
{
	if (!features::IsFeatureEnabled(features::MAIL_BOX))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("MAIL_BOX_SYSTEM_DISABLED"));
		return;		
	}

	TPacketCGAverageSearch * pinfo = (TPacketCGAverageSearch *)data;

	if (!ch)
		return;
		
	if (!ch->GetDesc())
		return;

	if (!data)
		return;
	
	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CANT_DO_THIS_BECAUSE_OTHER_WINDOW_OPEN"));
		return;
	}

	if (1 == quest::CQuestManager::instance().GetEventFlag("enable_average_system"))
		return;
	
	DWORD dwVnum = pinfo->vnum;
	if (!dwVnum)
		return;
	
	if (!ITEM_MANAGER::instance().GetTable(dwVnum))
		return;
	
	long long totalPrice = 0;
	long long totalCheque = 0;
	long long totalCount = 0;
	
	quest::PC* pPC = quest::CQuestManager::instance().GetPC(ch->GetPlayerID());

	if (!pPC)
		return;
	
	DWORD dwShopSearchSecCycle = 2; // 1 sec
	DWORD dwNowSec = get_global_time();
	DWORD dwLastShopSearchAttrSec = pPC->GetFlag("AverageSearch.LastAverageSearchSecAttr");

	if (dwLastShopSearchAttrSec + dwShopSearchSecCycle > dwNowSec)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_NEED_WAIT_%d_LEFT_(%d)"),
			dwShopSearchSecCycle, dwShopSearchSecCycle - (dwNowSec - dwLastShopSearchAttrSec));
		return;
	}

	pPC->SetFlag("AverageSearch.LastAverageSearchSecAttr", dwNowSec);

	CShopManager::TShopMap viewmap = CShopManager::instance().GetShopMap();
	CShopManager::TShopMap::iterator it = viewmap.begin();
	std::vector<CShop::SHOP_ITEM> sendItems;

	while (it != viewmap.end())
	{
		if (it->second->GetPC() == nullptr) {
			++it;
			continue;
		}
		
		LPCHARACTER tch = static_cast<LPCHARACTER>((it++)->second->GetPC());

		if (!tch)
			continue;
		
		if (!tch->GetMyShop() || tch == ch)
			continue;
		
		std::vector<CShop::SHOP_ITEM> shop_items = tch->GetMyShop()->GetItemVector();
		std::vector<CShop::SHOP_ITEM>::iterator item;
		
		for (item = shop_items.begin(); item != shop_items.end(); ++item)
		{
			if (item->vnum == 0)
				continue;
			
			TItemTable * table = ITEM_MANAGER::instance().GetTable(item->vnum);
			
			if (!table)
				continue;
			
			if (dwVnum != 0)
			{
				if (dwVnum == item->vnum)
				{
					totalCheque += item->price_cheque;
					totalPrice += item->price;
					totalCount += item->count;
				}
			}
		}
	}

	long long cheque_m = totalCheque * 100000000;
	totalPrice += cheque_m;
	
	if (totalPrice <= 0){
		return;
	}
	
	long long avgPrice = totalPrice/totalCount;
	long long avgCheque = 0;
	if (avgPrice >= 100000000){
		avgCheque = (avgPrice / 100000000);
		avgPrice -= avgCheque*100000000;
	}
	
	TPacketGCAverageSearch pack;
	pack.header = HEADER_GC_AVERAGE_ITEM_PRICE;

	pack.vnum = (dwVnum > 0 ? dwVnum : 0) ;
	pack.averagePrice = (avgPrice > 0 ? avgPrice : 0);
	pack.averageCheque = (avgCheque > 0 ? avgCheque : 0);
	ch->GetDesc()->Packet(&pack, sizeof(TPacketGCAverageSearch));
	
	//ch->ChatPacket(CHAT_TYPE_INFO, "Tüm pazarlar aratýldý ve ortalama fiyatlar yüklendi. (Ortalama yang : %lld, Ortalama won : %lld)",avgPrice, avgCheque);
}
#endif

int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
		d->SetPhase(PHASE_CLOSE);
		return (0);
	}

	int iExtraLen = 0;

	if (test_server && bHeader != HEADER_CG_MOVE)
		sys_log(0, "CInputMain::Analyze() ==> Header [%d] ", bHeader);

	switch (bHeader)
	{
	case HEADER_CG_PONG:
		Pong(d);
		break;

	case HEADER_CG_TIME_SYNC:
		Handshake(d, c_pData);
		break;

	case HEADER_CG_CHAT:
		if (test_server)
		{
			char* pBuf = (char*)c_pData;
			sys_log(0, "%s", pBuf + sizeof(TPacketCGChat));
		}

		if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_WHISPER:
		if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_MOVE:
	{
		Move(ch, c_pData);

		if (0 != g_stClientVersion.compare(d->GetClientVersion()))
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("Client version error."));
			d->DelayedDisconnect(0);
			LogManager::instance().HackLog("VERSION_CONFLICT", d->GetAccountTable().login, ch->GetName(), d->GetHostName());
		}
	}
	break;

	case HEADER_CG_CHARACTER_POSITION:
		Position(ch, c_pData);
		break;

	case HEADER_CG_ITEM_USE:
		if (!ch->IsObserverMode())
			ItemUse(ch, c_pData);
		break;

#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
	case HEADER_CG_ITEM_DROP:
		if (!ch->IsObserverMode())
			ItemDrop(ch, c_pData);
		break;

	case HEADER_CG_ITEM_DROP2:
		if (!ch->IsObserverMode())
			ItemDrop2(ch, c_pData);
		break;
#else
	case HEADER_CG_ITEM_DELETE:
		if (!ch->IsObserverMode())
			ItemDelete(ch, c_pData);
		break;

	case HEADER_CG_ITEM_SELL:
		if (!ch->IsObserverMode())
			ItemSell(ch, c_pData);
		break;
#endif

	case HEADER_CG_ITEM_MOVE:
		if (!ch->IsObserverMode())
			ItemMove(ch, c_pData);
		break;

	case HEADER_CG_ITEM_PICKUP:
		if (!ch->IsObserverMode())
			ItemPickup(ch, c_pData);
		break;

	case HEADER_CG_ITEM_USE_TO_ITEM:
		if (!ch->IsObserverMode())
			ItemToItem(ch, c_pData);
		break;

	case HEADER_CG_ITEM_GIVE:
		if (!ch->IsObserverMode())
			ItemGive(ch, c_pData);
		break;

	case HEADER_CG_EXCHANGE:
		if (!ch->IsObserverMode())
			Exchange(ch, c_pData);
		break;

	case HEADER_CG_ATTACK:
	case HEADER_CG_SHOOT:
		if (!ch->IsObserverMode())
		{
			Attack(ch, bHeader, c_pData);
		}
		break;

	case HEADER_CG_USE_SKILL:
		if(!ch || ch == NULL)
			break;
		if(!c_pData || c_pData == NULL)
			break;
		if (!ch->IsObserverMode())
			UseSkill(ch, c_pData);
		break;

	case HEADER_CG_QUICKSLOT_ADD:
		QuickslotAdd(ch, c_pData);
		break;

	case HEADER_CG_QUICKSLOT_DEL:
		QuickslotDelete(ch, c_pData);
		break;

	case HEADER_CG_QUICKSLOT_SWAP:
		QuickslotSwap(ch, c_pData);
		break;

	case HEADER_CG_SHOP:
		if ((iExtraLen = Shop(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_MESSENGER:
		if ((iExtraLen = Messenger(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_ON_CLICK:
		OnClick(ch, c_pData);
		break;

	case HEADER_CG_SYNC_POSITION:
		if ((iExtraLen = SyncPosition(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_ADD_FLY_TARGETING:
	case HEADER_CG_FLY_TARGETING:
		FlyTarget(ch, c_pData, bHeader);
		break;

	case HEADER_CG_SCRIPT_BUTTON:
		ScriptButton(ch, c_pData);
		break;

		// SCRIPT_SELECT_ITEM
	case HEADER_CG_SCRIPT_SELECT_ITEM:
		ScriptSelectItem(ch, c_pData);
		break;
		// END_OF_SCRIPT_SELECT_ITEM

	case HEADER_CG_SCRIPT_ANSWER:
		ScriptAnswer(ch, c_pData);
		break;

	case HEADER_CG_QUEST_INPUT_STRING:
		QuestInputString(ch, c_pData);
		break;

#ifdef ENABLE_OX_RENEWAL
	case HEADER_CG_QUEST_INPUT_LONG_STRING:
		QuestInputLongString(ch, c_pData);
		break;
#endif

	case HEADER_CG_QUEST_CONFIRM:
		QuestConfirm(ch, c_pData);
		break;

	case HEADER_CG_TARGET:
		Target(ch, c_pData);
		break;

	case HEADER_CG_WARP:
		Warp(ch, c_pData);
		break;

	case HEADER_CG_SAFEBOX_CHECKIN:
		SafeboxCheckin(ch, c_pData);
		break;

	case HEADER_CG_SAFEBOX_CHECKOUT:
		SafeboxCheckout(ch, c_pData, false);
		break;

	case HEADER_CG_SAFEBOX_ITEM_MOVE:
		SafeboxItemMove(ch, c_pData);
		break;

	case HEADER_CG_MALL_CHECKOUT:
		SafeboxCheckout(ch, c_pData, true);
		break;

	case HEADER_CG_PARTY_INVITE:
		PartyInvite(ch, c_pData);
		break;

	case HEADER_CG_PARTY_REMOVE:
		PartyRemove(ch, c_pData);
		break;

	case HEADER_CG_PARTY_INVITE_ANSWER:
		PartyInviteAnswer(ch, c_pData);
		break;

	case HEADER_CG_PARTY_SET_STATE:
		PartySetState(ch, c_pData);
		break;

	case HEADER_CG_PARTY_USE_SKILL:
		if(!ch || ch == NULL)
			break;
		if(!c_pData || c_pData == NULL)
			break;
		PartyUseSkill(ch, c_pData);
		break;

	case HEADER_CG_PARTY_PARAMETER:
		PartyParameter(ch, c_pData);
		break;

	case HEADER_CG_ANSWER_MAKE_GUILD:
#ifdef ENABLE_NEWGUILDMAKE
			ch->ChatPacket(CHAT_TYPE_INFO, "<%s> AnswerMakeGuild disabled", __FUNCTION__);
#else
			AnswerMakeGuild(ch, c_pData);
#endif
		break;

	case HEADER_CG_GUILD:
		if ((iExtraLen = Guild(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_FISHING:
		Fishing(ch, c_pData);
		break;

	case HEADER_CG_HACK:
		Hack(ch, c_pData);
		break;

#ifdef ENABLE_GROWTH_PET_SYSTEM
	case HEADER_CG_PetSetName:
		BraveRequestPetName(ch, c_pData);
		break;
#endif

	case HEADER_CG_MYSHOP:
		if ((iExtraLen = MyShop(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_REFINE:
		Refine(ch, c_pData);
		break;

	case HEADER_CG_CLIENT_VERSION:
		Version(ch, c_pData);
		break;

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	case HEADER_CG_SHOP_SEARCH:
		ShopSearch(ch, c_pData, false);
		break;

	case HEADER_CG_SHOP_SEARCH_SUB:
		ShopSearch(ch, c_pData, true);
		break;

	case HEADER_CG_SHOP_SEARCH_BUY:
		ShopSearchBuy(ch, c_pData);
		break;
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
	case HEADER_CG_CL:
		ChangeLook(ch, c_pData);
		break;
#endif

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	case HEADER_CG_TARGET_INFO_LOAD:
		TargetInfoLoad(ch, c_pData);
		break;
#endif

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	case HEADER_CG_COMBAT_ZONE_REQUEST_ACTION:
		CCombatZoneManager::instance().RequestAction(ch, c_pData);
		break;
#endif

#ifdef ENABLE_ACCE_SYSTEM
	case HEADER_CG_SASH:
		Sash(ch, c_pData);
		break;
#endif

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	case HEADER_CG_EXTEND_INVENTORY:
		if (!ch->IsObserverMode())
			ExInventory(ch, c_pData);
		break;
#endif

#ifdef ENABLE_YOUTUBER_SYSTEM
	case HEADER_CG_YOUTUBE_PANEL:
		YoutubePanel(ch, c_pData);
		break;

	case HEADER_CG_YOUTUBE_VIDEO:
		YoutuberVideo(ch, c_pData);
		break;
#endif

#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
	case HEADER_CG_CHEST_DROP_INFO:
		ChestDropInfo(ch, c_pData);
		break;
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
	case HEADER_CG_FISH_EVENT_SEND:
		if ((iExtraLen = FishEvent(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;
#endif

#ifdef ENABLE_GEM_SYSTEM
	case HEADER_CG_GEM_SHOP:
		if ((iExtraLen = GemShop(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;
#endif

	case HEADER_CG_DRAGON_SOUL_REFINE:
	{
		TPacketCGDragonSoulRefine* p = reinterpret_cast <TPacketCGDragonSoulRefine*>((void*)c_pData);
		switch (p->bSubType)
		{
		case DS_SUB_HEADER_CLOSE:
			ch->DragonSoul_RefineWindow_Close();
			break;
		case DS_SUB_HEADER_DO_REFINE_GRADE:
		{
			DSManager::instance().DoRefineGrade(ch, p->ItemGrid);
		}
		break;
		case DS_SUB_HEADER_DO_REFINE_STEP:
		{
			DSManager::instance().DoRefineStep(ch, p->ItemGrid);
		}
		break;
		case DS_SUB_HEADER_DO_REFINE_STRENGTH:
		{
			DSManager::instance().DoRefineStrength(ch, p->ItemGrid);
		}
		break;
		}
	}

	break;
#ifdef ENABLE_MINI_GAME_CATCH_KING
	case HEADER_CG_MINI_GAME_CATCH_KING:
		if ((iExtraLen = CMiniGame::instance().MiniGameCatchKing(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
	case HEADER_CG_ATTENDANCE_REWARD:
		CMiniGame::instance().AttendanceEventRequestReward(ch);
		break;
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
	case HEADER_CG_MAILBOX_SEND:
		MailBoxSend(ch, c_pData);
		break;
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	case HEADER_CG_SAFEBOX_MONEY:
		if (!ch->IsObserverMode())
			SafeboxMoney(ch, c_pData);
		break;
		
	case HEADER_CG_SAFEBOX_CHEQUE:
		if (!ch->IsObserverMode())
			SafeboxCheque(ch, c_pData);
		break;
#endif
#ifdef ENABLE_AURA_SYSTEM
	case HEADER_CG_AURA:
		Aura(ch, c_pData);
		break;
#endif

#ifdef ENABLE_AVERAGE_ITEM_PRICE
	case HEADER_CG_AVERAGE_ITEM_PRICE:
		AverageSearch(ch, c_pData);
		break;
#endif
	}
	return (iExtraLen);
}

int CInputDead::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
		return 0;
	}

	int iExtraLen = 0;

	switch (bHeader)
	{
	case HEADER_CG_PONG:
		Pong(d);
		break;

	case HEADER_CG_TIME_SYNC:
		Handshake(d, c_pData);
		break;

	case HEADER_CG_CHAT:
		if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;

		break;

	case HEADER_CG_WHISPER:
		if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
			return -1;

		break;

	case HEADER_CG_HACK:
		Hack(ch, c_pData);
		break;

	default:
		return (0);
	}

	return (iExtraLen);
}