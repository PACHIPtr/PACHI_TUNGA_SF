#include "stdafx.h"
#include "questmanager.h"
#include "char.h"
#include "char_manager.h"
#include "arena.h"

namespace quest
{
	ALUA(arena_start_duel)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		LPCHARACTER ch2 = CHARACTER_MANAGER::instance().FindPC(lua_tostring(L, 1));
		int nSetPoint = (int)lua_tonumber(L, 2);

		if (ch == nullptr || ch2 == nullptr)
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		if (ch->IsHorseRiding() == true)
		{
			ch->StopRiding();
			ch->HorseSummon(false);
		}

		if (ch2->IsHorseRiding() == true)
		{
			ch2->StopRiding();
			ch2->HorseSummon(false);
		}

		if (CArenaManager::instance().IsMember(ch->GetMapIndex(), ch->GetPlayerID()) != MEMBER_NO ||
			CArenaManager::instance().IsMember(ch2->GetMapIndex(), ch2->GetPlayerID()) != MEMBER_NO)
		{
			lua_pushnumber(L, 2);
			return 1;
		}

		if (CArenaManager::instance().StartDuel(ch, ch2, nSetPoint) == false)
		{
			lua_pushnumber(L, 3);
			return 1;
		}

		lua_pushnumber(L, 1);

		return 1;
	}

	ALUA(arena_add_map)
	{
		int mapIdx = (int)lua_tonumber(L, 1);
		int startposAX = (int)lua_tonumber(L, 2);
		int startposAY = (int)lua_tonumber(L, 3);
		int startposBX = (int)lua_tonumber(L, 4);
		int startposBY = (int)lua_tonumber(L, 5);

		if (CArenaManager::instance().AddArena(mapIdx, startposAX, startposAY, startposBX, startposBY) == false)
		{
			sys_log(0, "Failed to load arena map info(map:%d AX:%d AY:%d BX:%d BY:%d", mapIdx, startposAX, startposAY, startposBX, startposBY);
		}
		else
		{
			sys_log(0, "Add Arena Map:%d startA(%d,%d) startB(%d,%d)", mapIdx, startposAX, startposAY, startposBX, startposBY);
		}

		return 1;
	}

	ALUA(arena_get_duel_list)
	{
		CArenaManager::instance().GetDuelList(L);

		return 1;
	}

	ALUA(arena_add_observer)
	{
		int mapIdx = (int)lua_tonumber(L, 1);
		int ObPointX = (int)lua_tonumber(L, 2);
		int ObPointY = (int)lua_tonumber(L, 3);
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		CArenaManager::instance().AddObserver(ch, mapIdx, ObPointX, ObPointY);

		return 1;
	}

	ALUA(arena_is_in_arena)
	{
		DWORD pid = (DWORD)lua_tonumber(L, 1);

		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(pid);

		if (ch == nullptr)
		{
			lua_pushnumber(L, 1);
		}
		else
		{
			if (ch->GetArena() == nullptr || ch->GetArenaObserverMode() == true)
			{
				if (CArenaManager::instance().IsMember(ch->GetMapIndex(), ch->GetPlayerID()) == MEMBER_DUELIST)
					lua_pushnumber(L, 1);
				else
					lua_pushnumber(L, 0);
			}
			else
			{
				lua_pushnumber(L, 0);
			}
		}
		return 1;
	}

	void RegisterArenaFunctionTable()
	{
		luaL_reg arena_functions[] =
		{
			{"start_duel", arena_start_duel},
			{"add_map", arena_add_map},
			{"get_duel_list", arena_get_duel_list},
			{"add_observer", arena_add_observer},
			{"is_in_arena", arena_is_in_arena},
			{nullptr,	nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("arena", arena_functions);
	}
}