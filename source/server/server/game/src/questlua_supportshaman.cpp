#include "stdafx.h"

#include "questlua.h"
#include "questmanager.h"
#include "horsename_manager.h"
#include "char.h"
#include "affect.h"
#include "config.h"
#include "utils.h"
#include "../../common/service.h"
#include "support_shaman.h"
#include "war_map.h"
#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

extern int (*check_name) (const char* str);

namespace quest
{
#ifdef ENABLE_BUFFI_SYSTEM
	ALUA(SupportShaman_summon)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportShaman* SupportShaman = ch->GetSupportShaman();
		LPITEM pItem = CQuestManager::instance().GetCurrentItem();
		if (!ch || !SupportShaman || !pItem)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (0 == SupportShaman)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (CWarMapManager::instance().IsWarMap(ch->GetMapIndex()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LONCA_PET_BINEK_SAMAN"));
			return 1;
		}

		// 소환수의 vnum
		DWORD mobVnum = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		// 소환수의 이름
		const char* SupportShamanName = lua_isstring(L, 2) ? lua_tostring(L, 2) : 0;

		// 소환하면 멀리서부터 달려오는지 여부
		bool bFromFar = lua_isboolean(L, 3) ? lua_toboolean(L, 3) : false;

		CSupportShamanActor* SupportShamanActor = SupportShaman->Summon(mobVnum, pItem, SupportShamanName, bFromFar);

		if (SupportShamanActor != nullptr)
			lua_pushnumber(L, SupportShamanActor->GetVID());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	// syntax: SupportShaman.unsummon(mob_vnum)
	ALUA(SupportShaman_unsummon)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportShaman* SupportShaman = ch->GetSupportShaman();

		if (0 == SupportShaman)
			return 0;

		// 소환수의 vnum
		DWORD mobVnum = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		SupportShaman->Unsummon(mobVnum);
		return 1;
	}

	// syntax: SupportShaman.unsummon(mob_vnum)
	ALUA(SupportShaman_count_summoned)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportShaman* SupportShaman = ch->GetSupportShaman();

		lua_Number count = 0;

		if (0 != SupportShaman)
			count = (lua_Number)SupportShaman->CountSummoned();

		lua_pushnumber(L, count);

		return 1;
	}

	// syntax: SupportShaman.is_summon(mob_vnum)
	ALUA(SupportShaman_is_summon)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportShaman* SupportShaman = ch->GetSupportShaman();

		if (0 == SupportShaman)
			return 0;

		// 소환수의 vnum
		DWORD mobVnum = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		CSupportShamanActor * SupportShamanActor = SupportShaman->GetByVnum(mobVnum);

		if (nullptr == SupportShamanActor)
			lua_pushboolean(L, false);
		else
			lua_pushboolean(L, SupportShamanActor->IsSummoned());

		return 1;
	}

	ALUA(SupportShaman_spawn_effect)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportShaman* SupportShaman = ch->GetSupportShaman();

		if (0 == SupportShaman)
			return 0;

		DWORD mobVnum = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		CSupportShamanActor * SupportShamanActor = SupportShaman->GetByVnum(mobVnum);
		if (nullptr == SupportShamanActor)
			return 0;
		LPCHARACTER SupportShaman_ch = SupportShamanActor->GetCharacter();
		if (nullptr == SupportShaman_ch)
			return 0;

		if (lua_isstring(L, 2))
		{
			SupportShaman_ch->SpecificEffectPacket(lua_tostring(L, 2));
		}
		return 0;
	}

	void RegisterSupportShamanFunctionTable()
	{
		luaL_reg SupportShaman_functions[] =
		{
			{"summon", SupportShaman_summon},
			{"unsummon", SupportShaman_unsummon},
			{"is_summon", SupportShaman_is_summon},
			{"count_summoned", SupportShaman_count_summoned},
			{"spawn_effect", SupportShaman_spawn_effect},
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("supports", SupportShaman_functions);
	}
#endif
}