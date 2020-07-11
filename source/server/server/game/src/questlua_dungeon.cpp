#include "stdafx.h"
#include "constants.h"
#include "questmanager.h"
#include "questlua.h"
#include "dungeon.h"
#include "char.h"
#include "party.h"
#include "buffer_manager.h"
#include "char_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "p2p.h"
#include "map_location.h"

#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

template <class Func> Func CDungeon::ForEachMember(Func f)
{
	for (auto it = m_set_pkCharacter.begin(); it != m_set_pkCharacter.end(); ++it)
	{
		sys_log(0, "Dungeon ForEachMember %s", (*it)->GetName());
		f(*it);
	}
	return f;
}

namespace quest
{
	//
	// "dungeon" lua functions
	//
	ALUA(dungeon_notice)
	{
		if (!lua_isstring(L, 1))
			return 0;

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->Notice(lua_tostring(L, 1));
		return 0;
	}

	ALUA(dungeon_cmdchat)
	{
		if (!lua_isstring(L, 1))
			return 0;

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->CmdChat(lua_tostring(L, 1));
		return 0;
	}

	ALUA(dungeon_set_quest_flag)
	{
		CQuestManager& q = CQuestManager::instance();

		FSetQuestFlag f;

		f.flagname = q.GetCurrentPC()->GetCurrentQuestName() + "." + lua_tostring(L, 1);
		f.value = (int)rint(lua_tonumber(L, 2));

		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ForEachMember(f);

		return 0;
	}

	ALUA(dungeon_set_flag)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
		{
			sys_err("wrong set flag");
		}
		else
		{
			CQuestManager& q = CQuestManager::instance();
			LPDUNGEON pDungeon = q.GetCurrentDungeon();

			if (pDungeon)
			{
				const char* sz = lua_tostring(L, 1);
				int value = int(lua_tonumber(L, 2));
				pDungeon->SetFlag(sz, value);
			}
			else
			{
				sys_err("no dungeon !!!");
			}
		}
		return 0;
	}

	ALUA(dungeon_get_flag)
	{
		if (!lua_isstring(L, 1))
		{
			sys_err("wrong get flag");
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			const char* sz = lua_tostring(L, 1);
			lua_pushnumber(L, pDungeon->GetFlag(sz));
		}
		else
		{
			sys_err("no dungeon !!!");
			lua_pushnumber(L, 0);
		}

		return 1;
	}

	ALUA(dungeon_get_flag_from_map_index)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
		{
			sys_err("wrong get flag");
		}

		DWORD dwMapIndex = (DWORD)lua_tonumber(L, 2);
		if (dwMapIndex)
		{
			LPDUNGEON pDungeon = CDungeonManager::instance().FindByMapIndex(dwMapIndex);
			if (pDungeon)
			{
				const char* sz = lua_tostring(L, 1);
				lua_pushnumber(L, pDungeon->GetFlag(sz));
			}
			else
			{
				sys_err("no dungeon !!!");
				lua_pushnumber(L, 0);
			}
		}
		else
		{
			lua_pushboolean(L, 0);
		}
		return 1;
	}

	ALUA(dungeon_get_map_index)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			sys_log(0, "Dungeon GetMapIndex %d", pDungeon->GetMapIndex());
			lua_pushnumber(L, pDungeon->GetMapIndex());
		}
		else
		{
			sys_err("no dungeon !!!");
			lua_pushnumber(L, 0);
		}

		return 1;
	}

	ALUA(dungeon_regen_file)
	{
		if (!lua_isstring(L, 1))
		{
			sys_err("wrong filename");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnRegen(lua_tostring(L, 1));

		return 0;
	}

	ALUA(dungeon_set_regen_file)
	{
		if (!lua_isstring(L, 1))
		{
			sys_err("wrong filename");
			return 0;
		}
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnRegen(lua_tostring(L, 1), false);
		return 0;
	}

	ALUA(dungeon_clear_regen)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();
		if (pDungeon)
			pDungeon->ClearRegen();
		return 0;
	}

	ALUA(dungeon_check_eliminated)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();
		if (pDungeon)
			pDungeon->CheckEliminated();
		return 0;
	}

	ALUA(dungeon_set_exit_all_at_eliminate)
	{
		if (!lua_isnumber(L, 1))
		{
			sys_err("wrong time");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SetExitAllAtEliminate((long)lua_tonumber(L, 1));

		return 0;
	}

	ALUA(dungeon_set_warp_at_eliminate)
	{
		if (!lua_isnumber(L, 1))
		{
			sys_err("wrong time");
			return 0;
		}

		if (!lua_isnumber(L, 2))
		{
			sys_err("wrong map index");
			return 0;
		}

		if (!lua_isnumber(L, 3))
		{
			sys_err("wrong X");
			return 0;
		}

		if (!lua_isnumber(L, 4))
		{
			sys_err("wrong Y");
			return 0;
		}

		const char* c_pszRegenFile = nullptr;

		if (lua_gettop(L) >= 5)
			c_pszRegenFile = lua_tostring(L, 5);

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			pDungeon->SetWarpAtEliminate((long)lua_tonumber(L, 1),
				(long)lua_tonumber(L, 2),
				(long)lua_tonumber(L, 3),
				(long)lua_tonumber(L, 4),
				c_pszRegenFile);
		}
		else
			sys_err("cannot find dungeon");

		return 0;
	}

	ALUA(dungeon_new_jump)
	{
		if (lua_gettop(L) < 3)
		{
			sys_err("not enough argument");
			return 0;
		}

		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("wrong argument");
			return 0;
		}

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		long lMapIndex = (long)lua_tonumber(L, 1);

		LPDUNGEON pDungeon = CDungeonManager::instance().Create(lMapIndex);

		if (!pDungeon)
		{
			sys_err("cannot create dungeon %d", lMapIndex);
			return 0;
		}

		//ch->WarpSet(pDungeon->GetMapIndex(), (int) lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
		ch->WarpSet((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3), pDungeon->GetMapIndex());
		return 0;
	}

	ALUA(dungeon_new_jump_all)
	{
		if (lua_gettop(L) < 3 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("not enough argument");
			return 0;
		}

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		long lMapIndex = (long)lua_tonumber(L, 1);
		//long lX = (long)lua_tonumber(L, 2);
		//long lY = (long)lua_tonumber(L, 3);

		LPDUNGEON pDungeon = CDungeonManager::instance().Create(lMapIndex);

		if (!pDungeon)
		{
			sys_err("cannot create dungeon %d", lMapIndex);
			return 0;
		}

		pDungeon->JumpAll(ch->GetMapIndex(), (int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
		return 0;
	}

	ALUA(dungeon_new_jump_party)
	{
		if (lua_gettop(L) < 3 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("not enough argument");
			return 0;
		}

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		if (ch->GetParty() == nullptr)
		{
			sys_err("cannot go to dungeon alone.");
			return 0;
		}

		long lMapIndex = (long)lua_tonumber(L, 1);

		LPDUNGEON pDungeon = CDungeonManager::instance().Create(lMapIndex);

		if (!pDungeon)
		{
			sys_err("cannot create dungeon %d", lMapIndex);
			return 0;
		}

		if (ch->GetParty() == nullptr)
		{
			sys_err("cannot go to dungeon alone.");
			return 0;
		}
		pDungeon->JumpParty(ch->GetParty(), ch->GetMapIndex(), (int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));

		return 0;
	}

	ALUA(dungeon_jump_all)
	{
		if (lua_gettop(L) < 2 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2))
			return 0;

		LPDUNGEON pDungeon = CQuestManager::instance().GetCurrentDungeon();

		if (!pDungeon)
			return 0;

		pDungeon->JumpAll(pDungeon->GetMapIndex(), (int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
		return 0;
	}

	ALUA(dungeon_warp_all)
	{
		if (lua_gettop(L) < 2 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2))
			return 0;

		LPDUNGEON pDungeon = CQuestManager::instance().GetCurrentDungeon();

		if (!pDungeon)
			return 0;

		pDungeon->WarpAll(pDungeon->GetMapIndex(), (int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
		return 0;
	}

	ALUA(dungeon_get_kill_stone_count)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushnumber(L, pDungeon->GetKillStoneCount());
			return 1;
		}

		lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(dungeon_get_kill_mob_count)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushnumber(L, pDungeon->GetKillMobCount());
			return 1;
		}

		lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(dungeon_is_use_potion)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushboolean(L, 1);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushboolean(L, pDungeon->IsUsePotion());
			return 1;
		}

		lua_pushboolean(L, 1);
		return 1;
	}

	ALUA(dungeon_revived)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushboolean(L, 1);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushboolean(L, pDungeon->IsUseRevive());
			return 1;
		}

		lua_pushboolean(L, 1);
		return 1;
	}

	ALUA(dungeon_set_dest)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
			return 0;

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPPARTY pParty = ch->GetParty();
		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon && pParty)
			pDungeon->SendDestPositionToParty(pParty, (int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	ALUA(dungeon_unique_set_maxhp)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->UniqueSetMaxHP(lua_tostring(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	ALUA(dungeon_unique_set_hp)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->UniqueSetHP(lua_tostring(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	ALUA(dungeon_unique_set_def_grade)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->UniqueSetDefGrade(lua_tostring(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	ALUA(dungeon_unique_get_hp_perc)
	{
		if (!lua_isstring(L, 1))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushnumber(L, pDungeon->GetUniqueHpPerc(lua_tostring(L, 1)));
			return 1;
		}

		lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(dungeon_is_unique_dead)
	{
		if (!lua_isstring(L, 1))
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushboolean(L, pDungeon->IsUniqueDead(lua_tostring(L, 1)) ? 1 : 0);
			return 1;
		}

		lua_pushboolean(L, 0);
		return 1;
	}

	ALUA(dungeon_purge_unique)
	{
		if (!lua_isstring(L, 1))
			return 0;
		sys_log(0, "QUEST_DUNGEON_PURGE_UNIQUE %s", lua_tostring(L, 1));

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->PurgeUnique(lua_tostring(L, 1));

		return 0;
	}

	struct FPurgeArea
	{
		int x1, y1, x2, y2;
		LPCHARACTER ExceptChar;

		FPurgeArea(int a, int b, int c, int d, LPCHARACTER p)
			: x1(a), y1(b), x2(c), y2(d),
			ExceptChar(p)
		{}

		void operator () (LPENTITY ent)
		{
			if (true == ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pChar = static_cast<LPCHARACTER>(ent);

				if (pChar == ExceptChar)
					return;

				if (!pChar->IsPet() && (true == pChar->IsMonster() || true == pChar->IsStone()))
				{
					if (x1 <= pChar->GetX() && pChar->GetX() <= x2 && y1 <= pChar->GetY() && pChar->GetY() <= y2)
					{
						pChar->Dead();
					}
				}
			}
		}
	};

	ALUA(dungeon_purge_area)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
			return 0;
		sys_log(0, "QUEST_DUNGEON_PURGE_AREA");

		int x1 = lua_tonumber(L, 1);
		int y1 = lua_tonumber(L, 2);
		int x2 = lua_tonumber(L, 3);
		int y2 = lua_tonumber(L, 4);

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		const int mapIndex = pDungeon->GetMapIndex();

		if (0 == mapIndex)
		{
			sys_err("_purge_area: cannot get a map index with (%u, %u)", x1, y1);
			return 0;
		}

		LPSECTREE_MAP pSectree = SECTREE_MANAGER::instance().GetMap(mapIndex);

		if (nullptr != pSectree)
		{
			FPurgeArea func(x1, y1, x2, y2, CQuestManager::instance().GetCurrentNPCCharacterPtr());

			pSectree->for_each(func);
		}

		return 0;
	}

	ALUA(dungeon_kill_unique)
	{
		if (!lua_isstring(L, 1))
			return 0;
		sys_log(0, "QUEST_DUNGEON_KILL_UNIQUE %s", lua_tostring(L, 1));

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->KillUnique(lua_tostring(L, 1));

		return 0;
	}

	ALUA(dungeon_spawn_stone_door)
	{
		if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN_STONE_DOOR %s %s", lua_tostring(L, 1), lua_tostring(L, 2));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnStoneDoor(lua_tostring(L, 1), lua_tostring(L, 2));

		return 0;
	}

	ALUA(dungeon_spawn_wooden_door)
	{
		if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN_WOODEN_DOOR %s %s", lua_tostring(L, 1), lua_tostring(L, 2));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnWoodenDoor(lua_tostring(L, 1), lua_tostring(L, 2));

		return 0;
	}

	ALUA(dungeon_spawn_move_group)
	{
		if (!lua_isnumber(L, 1) || !lua_isstring(L, 2) || !lua_isstring(L, 3))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN_MOVE_GROUP %d %s %s", (int)lua_tonumber(L, 1), lua_tostring(L, 2), lua_tostring(L, 3));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnMoveGroup((DWORD)lua_tonumber(L, 1), lua_tostring(L, 2), lua_tostring(L, 3));

		return 0;
	}

	ALUA(dungeon_spawn_move_unique)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2) || !lua_isstring(L, 3) || !lua_isstring(L, 4))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN_MOVE_UNIQUE %s %d %s %s", lua_tostring(L, 1), (int)lua_tonumber(L, 2), lua_tostring(L, 3), lua_tostring(L, 4));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnMoveUnique(lua_tostring(L, 1), (DWORD)lua_tonumber(L, 2), lua_tostring(L, 3), lua_tostring(L, 4));

		return 0;
	}

	ALUA(dungeon_spawn_unique)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2) || !lua_isstring(L, 3))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN_UNIQUE %s %d %s", lua_tostring(L, 1), (int)lua_tonumber(L, 2), lua_tostring(L, 3));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnUnique(lua_tostring(L, 1), (DWORD)lua_tonumber(L, 2), lua_tostring(L, 3));

		return 0;
	}

	ALUA(dungeon_spawn)
	{
		if (!lua_isnumber(L, 1) || !lua_isstring(L, 2))
			return 0;
		sys_log(0, "QUEST_DUNGEON_SPAWN %d %s", (int)lua_tonumber(L, 1), lua_tostring(L, 2));

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->Spawn((DWORD)lua_tonumber(L, 1), lua_tostring(L, 2));

		return 0;
	}

	ALUA(dungeon_set_unique)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		DWORD vid = (DWORD)lua_tonumber(L, 2);

		if (pDungeon)
			pDungeon->SetUnique(lua_tostring(L, 1), vid);
		return 0;
	}

	ALUA(dungeon_get_unique_vid)
	{
		if (!lua_isstring(L, 1))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			lua_pushnumber(L, pDungeon->GetUniqueVid(lua_tostring(L, 1)));
			return 1;
		}

		lua_pushnumber(L, 0);
		return 1;
	}

	ALUA(dungeon_spawn_mob)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("invalid argument");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		DWORD vid = 0;

		if (pDungeon)
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			long x = (long)lua_tonumber(L, 2);
			long y = (long)lua_tonumber(L, 3);
			float radius = lua_isnumber(L, 4) ? (float)lua_tonumber(L, 4) : 0;
			DWORD count = (lua_isnumber(L, 5)) ? (DWORD)lua_tonumber(L, 5) : 1;

			sys_log(0, "dungeon_spawn_mob %u %d %d", dwVnum, x, y);

			if (count == 0)
				count = 1;

			while (count--)
			{
				if (radius < 1)
				{
					LPCHARACTER ch = pDungeon->SpawnMob(dwVnum, x, y);
					if (ch && !vid)
						vid = ch->GetVID();
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
					if (dwVnum == 20434)
						pDungeon->SetMast(ch);
#endif
				}
				else
				{
					float angle = number(0, 999) * M_PI * 2 / 1000;
					float r = number(0, 999) * radius / 1000;

					long nx = x + (long)(r * cos(angle));
					long ny = y + (long)(r * sin(angle));

					LPCHARACTER ch = pDungeon->SpawnMob(dwVnum, nx, ny);
					if (ch && !vid)
						vid = ch->GetVID();

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
					if (dwVnum == 20434)
						pDungeon->SetMast(ch);
#endif
				}
			}
		}

		lua_pushnumber(L, vid);
		return 1;
	}

	ALUA(dungeon_spawn_mob_dir)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			sys_err("invalid argument");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		DWORD vid = 0;

		if (pDungeon)
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			long x = (long)lua_tonumber(L, 2);
			long y = (long)lua_tonumber(L, 3);
			BYTE dir = (int)lua_tonumber(L, 4);

			LPCHARACTER ch = pDungeon->SpawnMob(dwVnum, x, y, dir);
			if (ch && !vid)
				vid = ch->GetVID();

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			if (dwVnum == 20434)
				pDungeon->SetMast(ch);
#endif
		}
		lua_pushnumber(L, vid);
		return 1;
	}

	ALUA(dungeon_spawn_mob_ac_dir)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			sys_err("invalid argument");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		DWORD vid = 0;

		if (pDungeon)
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			long x = (long)lua_tonumber(L, 2);
			long y = (long)lua_tonumber(L, 3);
			BYTE dir = (int)lua_tonumber(L, 4);

			LPCHARACTER ch = pDungeon->SpawnMob_ac_dir(dwVnum, x, y, dir);
			if (ch && !vid)
				vid = ch->GetVID();

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			if (dwVnum == 20434)
				pDungeon->SetMast(ch);
#endif
		}
		lua_pushnumber(L, vid);
		return 1;
	}

	ALUA(dungeon_spawn_goto_mob)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
			return 0;

		long lFromX = (long)lua_tonumber(L, 1);
		long lFromY = (long)lua_tonumber(L, 2);
		long lToX = (long)lua_tonumber(L, 3);
		long lToY = (long)lua_tonumber(L, 4);

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->SpawnGotoMob(lFromX, lFromY, lToX, lToY);

		return 0;
	}

	ALUA(dungeon_spawn_name_mob)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isstring(L, 4))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			long x = (long)lua_tonumber(L, 2);
			long y = (long)lua_tonumber(L, 3);
			pDungeon->SpawnNameMob(dwVnum, x, y, lua_tostring(L, 4));
		}
		return 0;
	}

	ALUA(dungeon_spawn_group)
	{
		//
		// argument: vnum,x,y,radius,aggressive,count
		//
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 6))
		{
			sys_err("invalid argument");
			return 0;
		}

		DWORD vid = 0;

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
		{
			DWORD group_vnum = (DWORD)lua_tonumber(L, 1);
			long local_x = (long)lua_tonumber(L, 2) * 100;
			long local_y = (long)lua_tonumber(L, 3) * 100;
			float radius = (float)lua_tonumber(L, 4) * 100;
			bool bAggressive = lua_toboolean(L, 5);
			DWORD count = (DWORD)lua_tonumber(L, 6);

			LPCHARACTER chRet = pDungeon->SpawnGroup(group_vnum, local_x, local_y, radius, bAggressive, count);
			if (chRet)
				vid = chRet->GetVID();
		}

		lua_pushnumber(L, vid);
		return 1;
	}

	ALUA(dungeon_join)
	{
		if (lua_gettop(L) < 1 || !lua_isnumber(L, 1))
			return 0;

		long lMapIndex = (long)lua_tonumber(L, 1);
		LPDUNGEON pDungeon = CDungeonManager::instance().Create(lMapIndex);

		if (!pDungeon)
			return 0;

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (ch->GetParty() && ch->GetParty()->GetLeaderPID() == ch->GetPlayerID())
			pDungeon->JoinParty(ch->GetParty());
		else if (!ch->GetParty())
			pDungeon->Join(ch);

		return 0;
	}

	ALUA(dungeon_exit) // 던전에 들어오기 전 위치로 보냄
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		ch->ExitToSavedLocation();
		return 0;
	}

	ALUA(dungeon_exit_all) // 던전에 있는 모든 사람을 던전에 들어오기 전 위치로 보냄
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ExitAll();

		return 0;
	}

	struct FSayDungeonByItemGroup
	{
		const CDungeon::ItemGroup* item_group;
		std::string can_enter_ment;
		std::string cant_enter_ment;
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;

				if (ch->IsPC())
				{
					struct ::packet_script packet_script;
					TEMP_BUFFER buf;

					for (CDungeon::ItemGroup::const_iterator it = item_group->begin(); it != item_group->end(); it++)
					{
						if (ch->CountSpecifyItem(it->first) >= it->second)
						{
							packet_script.header = HEADER_GC_SCRIPT;
							packet_script.skin = quest::CQuestManager::QUEST_SKIN_NORMAL;
							packet_script.src_size = can_enter_ment.size();
							packet_script.size = packet_script.src_size + sizeof(struct packet_script);

							buf.write(&packet_script, sizeof(struct packet_script));
							buf.write(&can_enter_ment[0], can_enter_ment.size());
							ch->GetDesc()->Packet(buf.read_peek(), buf.size());
							return;
						}
					}

					packet_script.header = HEADER_GC_SCRIPT;
					packet_script.skin = quest::CQuestManager::QUEST_SKIN_NORMAL;
					packet_script.src_size = cant_enter_ment.size();
					packet_script.size = packet_script.src_size + sizeof(struct packet_script);

					buf.write(&packet_script, sizeof(struct packet_script));
					buf.write(&cant_enter_ment[0], cant_enter_ment.size());
					ch->GetDesc()->Packet(buf.read_peek(), buf.size());
				}
			}
		}
	};

	ALUA(dungeon_say_diff_by_item_group)
	{
		if (!lua_isstring(L, 1) || !lua_isstring(L, 2) || !lua_isstring(L, 3))
		{
			sys_log(0, "QUEST wrong set flag");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		CDungeon* pDungeon = q.GetCurrentDungeon();

		if (!pDungeon)
		{
			sys_err("QUEST : no dungeon");
			return 0;
		}

		SECTREE_MAP* pMap = SECTREE_MANAGER::instance().GetMap(pDungeon->GetMapIndex());

		if (!pMap)
		{
			sys_err("cannot find map by index %d", pDungeon->GetMapIndex());
			return 0;
		}
		FSayDungeonByItemGroup f;
		sys_log(0, "diff_by_item");

		std::string group_name(lua_tostring(L, 1));
		f.item_group = pDungeon->GetItemGroup(group_name);

		if (f.item_group == nullptr)
		{
			sys_err("invalid item group");
			return 0;
		}

		f.can_enter_ment = lua_tostring(L, 2);
		f.can_enter_ment += "[ENTER][ENTER][ENTER][ENTER][DONE]";
		f.cant_enter_ment = lua_tostring(L, 3);
		f.cant_enter_ment += "[ENTER][ENTER][ENTER][ENTER][DONE]";

		pMap->for_each(f);

		return 0;
	}

	struct FExitDungeonByItemGroup
	{
		const CDungeon::ItemGroup* item_group;

		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;

				if (ch->IsPC())
				{
					for (CDungeon::ItemGroup::const_iterator it = item_group->begin(); it != item_group->end(); it++)
					{
						if (ch->CountSpecifyItem(it->first) >= it->second)
						{
							return;
						}
					}
					ch->ExitToSavedLocation();
				}
			}
		}
	};

	ALUA(dungeon_exit_all_by_item_group) // 특정 아이템 그룹에 속한 아이템이 없는사람은 강퇴
	{
		if (!lua_isstring(L, 1))
		{
			sys_log(0, "QUEST wrong set flag");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		CDungeon* pDungeon = q.GetCurrentDungeon();

		if (!pDungeon)
		{
			sys_err("QUEST : no dungeon");
			return 0;
		}

		SECTREE_MAP* pMap = SECTREE_MANAGER::instance().GetMap(pDungeon->GetMapIndex());

		if (!pMap)
		{
			sys_err("cannot find map by index %d", pDungeon->GetMapIndex());
			return 0;
		}
		FExitDungeonByItemGroup f;

		std::string group_name(lua_tostring(L, 1));
		f.item_group = pDungeon->GetItemGroup(group_name);

		if (f.item_group == nullptr)
		{
			sys_err("invalid item group");
			return 0;
		}

		pMap->for_each(f);

		return 0;
	}

	struct FDeleteItemInItemGroup
	{
		const CDungeon::ItemGroup* item_group;

		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;

				if (ch->IsPC())
				{
					for (CDungeon::ItemGroup::const_iterator it = item_group->begin(); it != item_group->end(); it++)
					{
						if (ch->CountSpecifyItem(it->first) >= it->second)
						{
							ch->RemoveSpecifyItem(it->first, it->second);
							return;
						}
					}
				}
			}
		}
	};

	ALUA(dungeon_delete_item_in_item_group_from_all) // 특정 아이템을 던전 내 pc에게서 삭제.
	{
		if (!lua_isstring(L, 1))
		{
			sys_log(0, "QUEST wrong set flag");
			return 0;
		}

		CQuestManager& q = CQuestManager::instance();
		CDungeon* pDungeon = q.GetCurrentDungeon();

		if (!pDungeon)
		{
			sys_err("QUEST : no dungeon");
			return 0;
		}

		SECTREE_MAP* pMap = SECTREE_MANAGER::instance().GetMap(pDungeon->GetMapIndex());

		if (!pMap)
		{
			sys_err("cannot find map by index %d", pDungeon->GetMapIndex());
			return 0;
		}
		FDeleteItemInItemGroup f;

		std::string group_name(lua_tostring(L, 1));
		f.item_group = pDungeon->GetItemGroup(group_name);

		if (f.item_group == nullptr)
		{
			sys_err("invalid item group");
			return 0;
		}

		pMap->for_each(f);

		return 0;
	}

	ALUA(dungeon_kill_all)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->KillAll();

		return 0;
	}

	ALUA(dungeon_purge)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->Purge();

		return 0;
	}

	ALUA(dungeon_exit_all_to_start_position)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ExitAllToStartPosition();

		return 0;
	}

	ALUA(dungeon_count_monster)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			lua_pushnumber(L, pDungeon->CountMonster());
		else
		{
			sys_err("not in a dungeon");
			lua_pushnumber(L, LONG_MAX);
		}

		return 1;
	}

	ALUA(dungeon_select)
	{
		DWORD dwMapIndex = (DWORD)lua_tonumber(L, 1);
		if (dwMapIndex)
		{
			LPDUNGEON pDungeon = CDungeonManager::instance().FindByMapIndex(dwMapIndex);
			if (pDungeon)
			{
				CQuestManager::instance().SelectDungeon(pDungeon);
				lua_pushboolean(L, 1);
			}
			else
			{
				CQuestManager::instance().SelectDungeon(nullptr);
				lua_pushboolean(L, 0);
			}
		}
		else
		{
			CQuestManager::instance().SelectDungeon(nullptr);
			lua_pushboolean(L, 0);
		}
		return 1;
	}

	ALUA(dungeon_find)
	{
		DWORD dwMapIndex = (DWORD)lua_tonumber(L, 1);
		if (dwMapIndex)
		{
			LPDUNGEON pDungeon = CDungeonManager::instance().FindByMapIndex(dwMapIndex);
			if (pDungeon)
			{
				lua_pushboolean(L, 1);
			}
			else
			{
				lua_pushboolean(L, 0);
			}
		}
		else
		{
			lua_pushboolean(L, 0);
		}
		return 1;
	}

	ALUA(dungeon_all_near_to)
	{
		LPDUNGEON pDungeon = CQuestManager::instance().GetCurrentDungeon();

		if (pDungeon != nullptr)
		{
			lua_pushboolean(L, pDungeon->IsAllPCNearTo((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2), 30));
		}
		else
		{
			lua_pushboolean(L, false);
		}

		return 1;
	}

	ALUA(dungeon_set_warp_location)
	{
		LPDUNGEON pDungeon = CQuestManager::instance().GetCurrentDungeon();

		if (pDungeon == nullptr)
		{
			return 0;
		}

		if (lua_gettop(L) < 3 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			return 0;
		}

		FSetWarpLocation f((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
		pDungeon->ForEachMember(f);

		return 0;
	}

	ALUA(dungeon_set_item_group)
	{
		if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
		{
			return 0;
		}
		std::string group_name(lua_tostring(L, 1));
		int size = lua_tonumber(L, 2);

		CDungeon::ItemGroup item_group;

		for (int i = 0; i < size; i++)
		{
			if (!lua_isnumber(L, i * 2 + 3) || !lua_isnumber(L, i * 2 + 4))
			{
				return 0;
			}
			item_group.push_back(std::pair <DWORD, int>(lua_tonumber(L, i * 2 + 3), lua_tonumber(L, i * 2 + 4)));
		}
		LPDUNGEON pDungeon = CQuestManager::instance().GetCurrentDungeon();

		if (pDungeon == nullptr)
		{
			return 0;
		}

		pDungeon->CreateItemGroup(group_name, item_group);
		return 0;
	}

	ALUA(dungeon_set_quest_flag2)
	{
		CQuestManager& q = CQuestManager::instance();

		FSetQuestFlag f;

		if (!lua_isstring(L, 1) || !lua_isstring(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("Invalid Argument");
		}

		f.flagname = string(lua_tostring(L, 1)) + "." + lua_tostring(L, 2);
		f.value = (int)rint(lua_tonumber(L, 3));

		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ForEachMember(f);

		return 0;
	}

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
	ALUA(dungeon_purge_vid)
	{
		DWORD dwVID = (DWORD)lua_tonumber(L, 1);

		LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(dwVID);

		if (ch)
			M2_DESTROY_CHARACTER(ch);

		return 0;
	}

	ALUA(dungeon_set_victim_mast)
	{
		CQuestManager& q = CQuestManager::instance();

		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		DWORD dwVID = (DWORD)lua_tonumber(L, 1);

		LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(dwVID);

		if (ch)
		{
			if (!pDungeon->GetMast())
				return 0;
			if ((int)lua_tonumber(L, 2) == 1)
				ch->SetVictim(pDungeon->GetMast());
			else
				ch->SetVictim(nullptr);
		}

		return 0;
	}

	ALUA(dungeon_get_mast)
	{
		CQuestManager& q = CQuestManager::instance();

		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon) 
		{
			if (pDungeon->GetMast())
			{
				if (pDungeon->GetMast()->IsStun() || pDungeon->GetMast()->IsDead())
				{
					lua_pushnumber(L, 0);
				}
				else
				{
					lua_pushnumber(L, 1);
				}
			}
			else
			{
				lua_pushnumber(L, 0);
			}
		}

		return 1;
	}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	//-------------------------------------------------------------------------------//
	#ifdef ENABLE_ZODIAC_TEMPLE_CHAT
	ALUA(dungeon_zodiac_clear)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ZodiacMessageClear();

		return 0;
	}

	ALUA(dungeon_zodiac_notice)
	{
		if (!lua_isstring(L, 1))
			return 0;

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ZodiacMessage(lua_tostring(L, 1));

		return 0;
	}
	#endif
	//-------------------------------------------------------------------------------//
	ALUA(dungeon_zodiac_time)
	{
		if (!lua_isnumber(L,1) || !lua_isnumber(L,2) || !lua_isnumber(L,3))
			return 0;

		BYTE current = (BYTE)lua_tonumber(L, 1);
		BYTE next = (BYTE)lua_tonumber(L, 2);
		int time = (int)lua_tonumber(L, 3);

		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ZodiacTime(current, next, time);

		return 0;
	}

	ALUA(dungeon_zodiac_time_clear)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDUNGEON pDungeon = q.GetCurrentDungeon();

		if (pDungeon)
			pDungeon->ZodiacTimeClear();

		return 0;
	}
#endif

#ifdef ENABLE_DUNGEON_BACK_SYSTEM
	ALUA(dungeon_join_coords)
	{
		if (lua_gettop(L)<3 || !lua_isnumber(L,1) || !lua_isnumber(L, 2) || !lua_isnumber(L,3))
		{
			sys_err("not enough argument");
			return 0;
		}

		long lMapIndex = (long)lua_tonumber(L, 1);
		
		LPDUNGEON pDungeon = CDungeonManager::instance().Create(lMapIndex);

		if (!pDungeon)
			return 0;

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		long index2 = (long)ch->GetMapIndex();

		if (ch->GetParty() && ch->GetParty()->GetLeaderPID() == ch->GetPlayerID())
			pDungeon->JoinParty_Coords(ch->GetParty(), (long)lua_tonumber(L, 2), (long)lua_tonumber(L, 3),index2);
		else if (!ch->GetParty())
			pDungeon->Join_Coords(ch, (long)lua_tonumber(L, 2), (long)lua_tonumber(L, 3),index2);

		return 0;
	}
#endif

	void RegisterDungeonFunctionTable()
	{
		luaL_reg dungeon_functions[] =
		{
			{"join", dungeon_join},
			{"exit", dungeon_exit},
			{"exit_all", dungeon_exit_all},
			{"set_item_group", dungeon_set_item_group},
			{"exit_all_by_item_group", dungeon_exit_all_by_item_group},
			{"say_diff_by_item_group", dungeon_say_diff_by_item_group},
			{"delete_item_in_item_group_from_all", dungeon_delete_item_in_item_group_from_all},
			{"purge", dungeon_purge},
			{"kill_all", dungeon_kill_all},
			{"spawn", dungeon_spawn},
			{"spawn_mob", dungeon_spawn_mob},
			{"spawn_mob_dir", dungeon_spawn_mob_dir},
			{"spawn_mob_ac_dir", dungeon_spawn_mob_ac_dir},
			{"spawn_name_mob", dungeon_spawn_name_mob},
			{"spawn_goto_mob", dungeon_spawn_goto_mob},
			{"spawn_group", dungeon_spawn_group},
			{"spawn_unique", dungeon_spawn_unique},
			{"spawn_move_unique", dungeon_spawn_move_unique},
			{"spawn_move_group", dungeon_spawn_move_group},
			{"spawn_stone_door", dungeon_spawn_stone_door},
			{"spawn_wooden_door", dungeon_spawn_wooden_door},
			{"purge_unique", dungeon_purge_unique},
			{"purge_area", dungeon_purge_area},
			{"kill_unique", dungeon_kill_unique},
			{"is_unique_dead", dungeon_is_unique_dead},
			{"unique_get_hp_perc", dungeon_unique_get_hp_perc},
			{"unique_set_def_grade", dungeon_unique_set_def_grade},
			{"unique_set_hp", dungeon_unique_set_hp},
			{"unique_set_maxhp", dungeon_unique_set_maxhp},
			{"get_unique_vid", dungeon_get_unique_vid},
			{"get_kill_stone_count", dungeon_get_kill_stone_count},
			{"get_kill_mob_count", dungeon_get_kill_mob_count},
			{"is_use_potion", dungeon_is_use_potion},
			{"revived", dungeon_revived},
			{"set_dest", dungeon_set_dest},
			{"jump_all", dungeon_jump_all},
			{"warp_all", dungeon_warp_all},
			{"new_jump_all", dungeon_new_jump_all},
			{"new_jump_party", dungeon_new_jump_party},
			{"new_jump", dungeon_new_jump},
			{"regen_file", dungeon_regen_file},
			{"set_regen_file", dungeon_set_regen_file},
			{"clear_regen", dungeon_clear_regen},
			{"set_exit_all_at_eliminate", dungeon_set_exit_all_at_eliminate},
			{"set_warp_at_eliminate", dungeon_set_warp_at_eliminate},
			{"get_map_index", dungeon_get_map_index},
			{"check_eliminated", dungeon_check_eliminated},
			{"exit_all_to_start_position", dungeon_exit_all_to_start_position },
			{"count_monster", dungeon_count_monster},
			{"setf", dungeon_set_flag},
			{"getf", dungeon_get_flag},
			{"getf_from_map_index",	dungeon_get_flag_from_map_index},
			{"set_unique", dungeon_set_unique},
			{"select", dungeon_select},
			{"find", dungeon_find},
			{"notice", dungeon_notice},
			{"setqf", dungeon_set_quest_flag},
			{"all_near_to",	dungeon_all_near_to},
			{"set_warp_location", dungeon_set_warp_location},
			{"setqf2", dungeon_set_quest_flag2},
			{"cmdchat", dungeon_cmdchat},
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			{"purge_vid", dungeon_purge_vid},
			{"set_victim_mast", dungeon_set_victim_mast},
			{"get_mast", dungeon_get_mast},
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	#ifdef ENABLE_ZODIAC_TEMPLE_CHAT
			{"zodiac_notice_clear", dungeon_zodiac_clear},
			{"zodiac_notice", dungeon_zodiac_notice},
	#endif
			{"zodiac_time", dungeon_zodiac_time},
			{"zodiac_time_clear", dungeon_zodiac_time_clear},
#endif
#ifdef ENABLE_DUNGEON_BACK_SYSTEM
			{"join_coords", dungeon_join_coords},
#endif
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("d", dungeon_functions);
	}
}