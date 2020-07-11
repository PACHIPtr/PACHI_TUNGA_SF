#include "stdafx.h"
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
#include "config.h"
#include "constants.h"
#include "item_combination.h"
#include "utils.h"
#include "log.h"
#include "char.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"
#include "unique_item.h"
#include <stdlib.h>
#include <sstream>

void Comb_open_costume(LPCHARACTER ch)
{
	if (nullptr == ch)
		return;

	LPCHARACTER	npc = ch->GetQuestNPC();
	if (nullptr == npc)
	{
		return;
	}

	if (ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_ALREADY_OPENED_WINDOW"));
		return;
	}

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 사용할 수 없습니다."));
		return;
	}

	long distance = DISTANCE_APPROX(ch->GetX() - npc->GetX(), ch->GetY() - npc->GetY());
	if (distance >= COMB_MAX_DISTANCE)
	{
		sys_log(1, "Combination: TOO_FAR: %s distance %d", ch->GetName(), distance);
		return;
	}

	Comb_clean_item(ch);
	ch->SetCombNpc(npc);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "combination open_costume %d", npc->GetRaceNum());
}

void Comb_open_skillbook(LPCHARACTER ch)
{
	if (nullptr == ch)
		return;

	LPCHARACTER	npc = ch->GetQuestNPC();
	if (nullptr == npc)
	{
		return;
	}

	if (ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_ALREADY_OPENED_WINDOW"));
		return;
	}

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 사용할 수 없습니다."));
		return;
	}

	long distance = DISTANCE_APPROX(ch->GetX() - npc->GetX(), ch->GetY() - npc->GetY());
	if (distance >= COMB_MAX_DISTANCE)
	{
		sys_log(1, "Combination: TOO_FAR: %s distance %d", ch->GetName(), distance);
		return;
	}

	Comb_clean_item(ch);
	ch->SetCombNpc(npc);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "combination open_skillbook %d", npc->GetRaceNum());
}

void Comb_add_item(LPCHARACTER ch, int combination_index, int inven_index)
{
	LPITEM	item;
	LPITEM* combination_item;

	if (inven_index < 0 || INVENTORY_AND_EQUIP_SLOT_MAX <= inven_index)
		return;

	if (combination_index < 0 || COMB_MAX_NUM <= combination_index)
		return;

	item = ch->GetInventoryItem(inven_index);
	if (item == nullptr)
		return;

	combination_item = ch->GetCombItem();
	for (int i = 0; i < COMB_MAX_NUM; ++i)
	{
		if (item == combination_item[i])
		{
			combination_item[i] = nullptr;
			break;
		}
	}

	combination_item[combination_index] = item;

	if (test_server)
		ch->ChatPacket(CHAT_TYPE_INFO, "combination[%d]: inventory[%d]: %s added", combination_index, inven_index, item->GetName());

	return;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
void Comb_add_item2(LPCHARACTER ch, int combination_index, int inven_index, int inven_type)
#else
void Comb_add_item2(LPCHARACTER ch, int combination_index, int inven_index)
#endif
{
	LPITEM	item;
	LPITEM* combination_item;

	if (combination_index < 0 || COMB_MAX_NUM <= combination_index)
		return;

	if (225 <= inven_index)
		return;

	item = ch->GetSkillBookInventoryItem(inven_index);

	if (item == nullptr)
		return;

	combination_item = ch->GetCombItem();
	for (int i = 0; i < COMB_MAX_NUM; ++i)
	{
		if (item == combination_item[i])
		{
			combination_item[i] = nullptr;
			break;
		}
	}

	combination_item[combination_index] = item;

	if (test_server)
		ch->ChatPacket(CHAT_TYPE_INFO, "combination[%d]: inventory[%d]: %s added", combination_index, inven_index, item->GetName());

	return;
}

void Comb_del_item(LPCHARACTER ch, int combination_index)
{
	LPITEM	item;
	LPITEM* combination_item;

	if (combination_index < 0 || COMB_MAX_NUM <= combination_index)
		return;

	combination_item = ch->GetCombItem();
	if (combination_item[combination_index] == nullptr)
		return;

	item = combination_item[combination_index];

	combination_item[combination_index] = nullptr;
	if (test_server)
		ch->ChatPacket(CHAT_TYPE_INFO, "combination[%d]: combination[%d]: %s deleted", combination_index, item->GetCell(), item->GetName());

	return;
}

bool Comb_make_cotume(LPCHARACTER ch)
{
	LPCHARACTER	npc;
	LPITEM* items;

	if (!ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_WINDOW_NOT_OPENED"));
		return false;
	}

	npc = ch->GetQuestNPC();
	if (nullptr == npc)
		return false;

	items = ch->GetCombItem();
	if (items[0] == nullptr || items[1] == nullptr || items[2] == nullptr)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_DRAG_ITEMS_WINDOW"));
		return false;
	}

	if (items[0]->GetVnum() != COSTUME_BONUS_TRANSFER_ITEM_VNUM)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_NEED_TRANSFER_SCROLL"));
		return false;
	}

	if (items[2] == nullptr)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_NEED_COSTUME_2"));
		return false;
	}

	if ((items[1]->GetType() != ITEM_COSTUME && items[1]->GetSubType() != COSTUME_BODY) ||
		(items[1]->GetType() != ITEM_COSTUME && items[1]->GetSubType() != COSTUME_HAIR)
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		|| (items[1]->GetType() != ITEM_COSTUME && items[1]->GetSubType() != COSTUME_WEAPON) ||
#endif
		(items[2]->GetType() != ITEM_COSTUME && items[2]->GetSubType() != COSTUME_BODY) ||
		(items[2]->GetType() != ITEM_COSTUME && items[2]->GetSubType() != COSTUME_HAIR)
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		|| (items[2]->GetType() != ITEM_COSTUME && items[2]->GetSubType() != COSTUME_WEAPON)
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_ONLY_COSTUME"));
		return false;
	}

	if (items[2]->GetAttributeCount() < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_NOT_HAVE_BONUS_COSTUME_2"));
		return false;
	}

	if (items[1]->IsEquipped() || items[2]->IsEquipped())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_IS_EQUIPPED"));
		return false;
	}

	if (items[1]->isLocked() || items[2]->isLocked())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_IS_LOCKED"));
		return false;
	}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (items[0]->IsSealed() || items[1]->IsSealed() || items[2]->IsSealed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_IS_BINDED"));
		return false;
	}
#endif

	if (items[0]->IsBasicItem() || items[1]->IsBasicItem() || items[2]->IsBasicItem())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return false;
	}

	for (int i = 0; i < items[1]->GetAttributeCount(); ++i)
	{
		if (items[2]->HasApply(items[1]->GetAttributeType(i)) == true)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_HAS_APPLY_ENOUGH_VAL"));
			return false;
		}
	}

	items[1]->ClearAttribute();
	for (int i = 0; i < items[2]->GetAttributeCount(); i++)
		items[1]->SetForceAttribute(i, items[2]->GetAttributeType(i), items[2]->GetAttributeValue(i));

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_SUCCESS"));
	ch->ChatPacket(CHAT_TYPE_COMMAND, "combination success_costume");

	if (items[0]->GetCount() == 1)
		items[0]->RemoveFromCharacter();
	else
		items[0]->SetCount(items[0]->GetCount() - 1);
	items[2]->RemoveFromCharacter();
	// items[2]->SetCount(0);
	items[2] = nullptr;

	return true;
}

bool Comb_make_skillbook(LPCHARACTER ch)
{
	LPCHARACTER	npc;
	LPITEM* items;
	int	i_number = 0;

	if (!ch->IsCombOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_WINDOW_NOT_OPENED"));
		return false;
	}

	npc = ch->GetQuestNPC();
	if (nullptr == npc)
		return false;

	items = ch->GetCombItem();

	if (ch->GetGold() < 1000000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_NOT_ENOUGH_GOLD"));
		return false;
	}

	if (ch->GetSkillGroup() == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_CH_NOT_HAVE_SKILL_GROUP"));
		return false;
	}

	for (int i = 0; i < COMB_MAX_NUM; ++i)
	{
		if (items[i] == nullptr)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_NEED_10_BOOKS"));
			return false;
		}

		if (items[i]->GetVnum() != 50300)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_ONLY_SKILL_BOOK"));
			return false;
		}

		if (items[i]->isLocked())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_IS_BINDED"));
			return false;
		}

		if (items[i]->GetCount() != 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_COUNT_ERROR"));
			return false;
		}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
		if (items[i]->IsSealed())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_IS_BINDED"));
			return false;
		}
#endif

		if (items[i]->IsBasicItem())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
			return false;
		}
	}

	if (ch->GetJob() == JOB_WARRIOR)
	{
		if (ch->GetSkillGroup() == 1)
			i_number = number(1, 6);
		else if (ch->GetSkillGroup() == 2)
			i_number = number(16, 21);
	}
	else if (ch->GetJob() == JOB_ASSASSIN)
	{
		if (ch->GetSkillGroup() == 1)
			i_number = number(31, 36);
		else if (ch->GetSkillGroup() == 2)
			i_number = number(46, 51);
	}
	else if (ch->GetJob() == JOB_SURA)
	{
		if (ch->GetSkillGroup() == 1)
			i_number = number(61, 66);
		else if (ch->GetSkillGroup() == 2)
			i_number = number(76, 81);
	}
	else if (ch->GetJob() == JOB_SHAMAN)
	{
		if (ch->GetSkillGroup() == 1)
			i_number = number(91, 96);
		else if (ch->GetSkillGroup() == 2)
			i_number = number(106, 111);
	}
	else if (ch->GetJob() == JOB_WOLFMAN)
	{
		if (ch->GetSkillGroup() == 1)
			i_number = number(170, 175);
	}

	for (int i = 0; i < 10; ++i)
		items[i]->RemoveFromCharacter();

	ch->PointChange(POINT_GOLD, -1000000, false);

	LPITEM item = ch->AutoGiveItem(50300);

	if (!item)
		return false;

	item->SetSocket(0, i_number);

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_COMB_SUCCESS"));
	ch->ChatPacket(CHAT_TYPE_COMMAND, "combination success_skillbook");

	return true;
}

void Comb_close(LPCHARACTER ch)
{
	Comb_clean_item(ch);
	ch->SetCombNpc(nullptr);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "combination close");
}

void Comb_clean_item(LPCHARACTER ch)
{
	LPITEM* combination_item;

	combination_item = ch->GetCombItem();
	for (int i = 0; i < COMB_MAX_NUM; ++i)
	{
		if (combination_item[i] == nullptr)
			continue;

		combination_item[i] = nullptr;
	}
}
#endif /*ENABLE_ITEM_COMBINATION_SYSTEM*/