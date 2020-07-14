#include "stdafx.h"
#include <sstream>
#include "constants.h"
#include "char.h"
#include "char_manager.h"
#include "log.h"
#include "questmanager.h"
#include "questlua.h"
#include "questevent.h"
#include "config.h"
#include "mining.h"
#include "fishing.h"
#include "priv_manager.h"
#include "utils.h"
#include "p2p.h"
#include "item_manager.h"
#include "db.h"
#include "mob_manager.h"
#include "start_position.h"
#include "OXEvent.h"
#include "regen.h"
#include "cmd.h"
#include "guild.h"
#include "guild_manager.h"
#include "sectree_manager.h"
#ifdef ENABLE_BUFFI_SYSTEM
#include "support_shaman.h"
#endif
#include "features.h"

#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)

#endif

extern ACMD(do_block_chat);

namespace quest
{
	ALUA(_get_locale)
	{
		lua_pushstring(L, g_stLocale.c_str());
		return 1;
	}

	ALUA(_number)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
			lua_pushnumber(L, 0);
		else
			lua_pushnumber(L, number((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2)));
		return 1;
	}

	ALUA(_time_to_str)
	{
		time_t curTime = (time_t)lua_tonumber(L, -1);
		lua_pushstring(L, asctime(gmtime(&curTime)));
		return 1;
	}

	ALUA(_say)
	{
		ostringstream s;
		combine_lua_string(L, s);
		CQuestManager::Instance().AddScript("[TEXT_HORIZONTAL_ALIGN_CENTER]");
		CQuestManager::Instance().AddScript(s.str() + "[ENTER]");
		return 0;
	}

	ALUA(_chat)
	{
		ostringstream s;
		combine_lua_string(L, s);

		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_TALKING, "%s", s.str().c_str());
		return 0;
	}

	ALUA(_cmdchat)
	{
		ostringstream s;
		combine_lua_string(L, s);
		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_COMMAND, "%s", s.str().c_str());
		return 0;
	}

	ALUA(_syschat)
	{
		ostringstream s;
		combine_lua_string(L, s);
		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_INFO, "%s", s.str().c_str());
		return 0;
	}

	ALUA(_notice)
	{
		ostringstream s;
		combine_lua_string(L, s);
		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_NOTICE, "%s", s.str().c_str());
		return 0;
	}

	ALUA(_left_image)
	{
		if (lua_isstring(L, -1))
		{
			string s = lua_tostring(L, -1);
			CQuestManager::Instance().AddScript("[LEFTIMAGE src;" + s + "]");
		}
		return 0;
	}

	ALUA(_top_image)
	{
		if (lua_isstring(L, -1))
		{
			string s = lua_tostring(L, -1);
			CQuestManager::Instance().AddScript("[TOPIMAGE src;" + s + "]");
		}
		return 0;
	}

	ALUA(_set_skin) // Quest UI style
	{
		if (lua_isnumber(L, -1))
		{
			CQuestManager::Instance().SetSkinStyle((int)rint(lua_tonumber(L, -1)));
		}
		else
		{
			sys_err("QUEST wrong skin index");
		}

		return 0;
	}

	ALUA(_set_server_timer)
	{
		int n = lua_gettop(L);
		if ((n != 2 || !lua_isnumber(L, 2) || !lua_isstring(L, 1)) &&
			(n != 3 || !lua_isstring(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3)))
		{
			sys_err("QUEST set_server_timer argument count wrong.");
			return 0;
		}

		const char* name = lua_tostring(L, 1);
		double t = lua_tonumber(L, 2);
		DWORD arg = 0;

		CQuestManager& q = CQuestManager::instance();

		if (lua_isnumber(L, 3))
			arg = (DWORD)lua_tonumber(L, 3);

		int timernpc = q.LoadTimerScript(name);

		LPEVENT event = quest_create_server_timer_event(name, t, timernpc, false, arg);
		q.AddServerTimer(name, arg, event);
		return 0;
	}

	ALUA(_set_server_loop_timer)
	{
		int n = lua_gettop(L);
		if ((n != 2 || !lua_isnumber(L, 2) || !lua_isstring(L, 1)) &&
			(n != 3 || !lua_isstring(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3)))
		{
			sys_err("QUEST set_server_timer argument count wrong.");
			return 0;
		}
		const char* name = lua_tostring(L, 1);
		double t = lua_tonumber(L, 2);
		DWORD arg = 0;
		CQuestManager& q = CQuestManager::instance();

		if (lua_isnumber(L, 3))
			arg = (DWORD)lua_tonumber(L, 3);

		int timernpc = q.LoadTimerScript(name);

		LPEVENT event = quest_create_server_timer_event(name, t, timernpc, true, arg);
		q.AddServerTimer(name, arg, event);
		return 0;
	}

	ALUA(_clear_server_timer)
	{
		CQuestManager& q = CQuestManager::instance();
		const char* name = lua_tostring(L, 1);
		DWORD arg = (DWORD)lua_tonumber(L, 2);
		if (name && arg)
			q.ClearServerTimer(name, arg);
		else
			sys_err("LUA PREVENT: Wrong argument on ClearServerTimer!");
		return 0;
	}

	ALUA(_set_named_loop_timer)
	{
		int n = lua_gettop(L);

		if (n != 2 || !lua_isnumber(L, -1) || !lua_isstring(L, -2))
			sys_err("QUEST set_timer argument count wrong.");
		else
		{
			const char* name = lua_tostring(L, -2);
			double t = lua_tonumber(L, -1);

			CQuestManager & q = CQuestManager::instance();
			int timernpc = q.LoadTimerScript(name);
			q.GetCurrentPC()->AddTimer(name, quest_create_timer_event(name, q.GetCurrentCharacterPtr()->GetPlayerID(), t, timernpc, true));
		}

		return 0;
	}

	ALUA(_get_server_timer_arg)
	{
		lua_pushnumber(L, CQuestManager::instance().GetServerTimerArg());
		return 1;
	}

	ALUA(_set_timer)
	{
		if (lua_gettop(L) != 1 || !lua_isnumber(L, -1))
			sys_err("QUEST invalid argument.");
		else
		{
			double t = lua_tonumber(L, -1);

			CQuestManager & q = CQuestManager::instance();
			quest_create_timer_event("", q.GetCurrentCharacterPtr()->GetPlayerID(), t);
		}

		return 0;
	}

	ALUA(_set_named_timer)
	{
		int n = lua_gettop(L);

		if (n != 2 || !lua_isnumber(L, -1) || !lua_isstring(L, -2))
		{
			sys_err("QUEST set_timer argument count wrong.");
		}
		else
		{
			const char* name = lua_tostring(L, -2);
			double t = lua_tonumber(L, -1);

			CQuestManager & q = CQuestManager::instance();
			int timernpc = q.LoadTimerScript(name);
			q.GetCurrentPC()->AddTimer(name, quest_create_timer_event(name, q.GetCurrentCharacterPtr()->GetPlayerID(), t, timernpc));
		}

		return 0;
	}

	ALUA(_timer)
	{
		if (lua_gettop(L) == 1)
			return _set_timer(L);
		else
			return _set_named_timer(L);
	}

	ALUA(_clear_named_timer)
	{
		int n = lua_gettop(L);

		if (n != 1 || !lua_isstring(L, -1))
			sys_err("QUEST set_timer argument count wrong.");
		else
		{
			CQuestManager& q = CQuestManager::instance();
			q.GetCurrentPC()->RemoveTimer(lua_tostring(L, -1));
		}

		return 0;
	}

	ALUA(_getnpcid)
	{
		const char* name = lua_tostring(L, -1);
		CQuestManager & q = CQuestManager::instance();
		lua_pushnumber(L, q.FindNPCIDByName(name));
		return 1;
	}

	ALUA(_is_test_server)
	{
		lua_pushboolean(L, test_server);
		return 1;
	}

	ALUA(_raw_script)
	{
		if (test_server)
			sys_log(0, "_raw_script : %s ", lua_tostring(L, -1));
		if (lua_isstring(L, -1))
			CQuestManager::Instance().AddScript(lua_tostring(L, -1));
		else
			sys_err("QUEST wrong argument: questname: %s", CQuestManager::instance().GetCurrentQuestName().c_str());

		return 0;
	}

	ALUA(_char_log)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		DWORD what = 0;
		const char* how = "";
		const char* hint = "";

		if (lua_isnumber(L, 1)) what = (DWORD)lua_tonumber(L, 1);
		if (lua_isstring(L, 2)) how = lua_tostring(L, 2);
		if (lua_tostring(L, 3)) hint = lua_tostring(L, 3);

		if (ch)
			LogManager::instance().CharLog(ch, what, how, hint);
		else
			sys_err("LUA PREVENT: !ch on _char_log!");
		return 0;
	}

	ALUA(_item_log)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		DWORD dwItemID = 0;
		const char* how = "";
		const char* hint = "";

		if (lua_isnumber(L, 1)) dwItemID = (DWORD)lua_tonumber(L, 1);
		if (lua_isstring(L, 2)) how = lua_tostring(L, 2);
		if (lua_tostring(L, 3)) hint = lua_tostring(L, 3);

		LPITEM item = ITEM_MANAGER::instance().Find(dwItemID);

		if (item)
			LogManager::instance().ItemLog(ch, item, how, hint);

		return 0;
	}

	ALUA(_syslog)
	{
		if (!lua_isnumber(L, 1) || !lua_isstring(L, 2))
			return 0;

		if (lua_tonumber(L, 1) >= 1)
		{
			if (!test_server)
				return 0;
		}

		PC* pc = CQuestManager::instance().GetCurrentPC();

		if (!pc)
			return 0;

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (!ch)
			return 0;

		sys_log(0, "QUEST: quest: %s player: %s : %s", pc->GetCurrentQuestName().c_str(), ch->GetName(), lua_tostring(L, 2));

		if (true == test_server)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "QUEST_SYSLOG %s", lua_tostring(L, 2));
		}

		return 0;
	}

	ALUA(_syserr)
	{
		if (!lua_isstring(L, 1))
			return 0;
		sys_err("From LUA: %s", lua_tostring(L, 1));
		/*
				PC* pc = CQuestManager::instance().GetCurrentPC();

				if (!pc)
					return 0;

				LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

				if (!ch)
					return 0;

				sys_err("QUEST: quest: %s player: %s : %s", pc->GetCurrentQuestName().c_str(), ch->GetName(), lua_tostring(L, 1));
				ch->ChatPacket(CHAT_TYPE_INFO, "QUEST_SYSERR %s", lua_tostring(L, 1));
			*/
		return 0;
	}

	// LUA_ADD_BGM_INFO
	ALUA(_set_bgm_volume_enable)
	{
		CHARACTER_SetBGMVolumeEnable();

		return 0;
	}

	ALUA(_add_bgm_info)
	{
		if (!lua_isnumber(L, 1) || !lua_isstring(L, 2))
			return 0;

		int mapIndex = (int)lua_tonumber(L, 1);

		const char* bgmName = lua_tostring(L, 2);
		if (!bgmName)
			return 0;

		float bgmVol = lua_isnumber(L, 3) ? lua_tonumber(L, 3) : (1.0f / 5.0f) * 0.1f;

		CHARACTER_AddBGMInfo(mapIndex, bgmName, bgmVol);

		return 0;
	}
	// END_OF_LUA_ADD_BGM_INFO

	// LUA_ADD_GOTO_INFO
	ALUA(_add_goto_info)
	{
		const char* name = lua_tostring(L, 1);

		int empire = (int)lua_tonumber(L, 2);
		int mapIndex = (int)lua_tonumber(L, 3);
		int x = (int)lua_tonumber(L, 4);
		int y = (int)lua_tonumber(L, 5);

		if (!name)
			return 0;

		CHARACTER_AddGotoInfo(name, empire, mapIndex, x, y);
		return 0;
	}
	// END_OF_LUA_ADD_GOTO_INFO

	// REFINE_PICK
	ALUA(_refine_pick)
	{
		UINT bCell = (BYTE)lua_tonumber(L, -1);

		CQuestManager & q = CQuestManager::instance();

		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		LPITEM item = ch->GetInventoryItem(bCell);

		int ret = mining::RealRefinePick(ch, item);
		lua_pushnumber(L, ret);
		return 1;
	}
	// END_OF_REFINE_PICK

	ALUA(_fish_real_refine_rod)
	{
		UINT bCell = (BYTE)lua_tonumber(L, -1);

		CQuestManager & q = CQuestManager::instance();

		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		LPITEM item = ch->GetInventoryItem(bCell);

		int ret = fishing::RealRefineRod(ch, item);
		lua_pushnumber(L, ret);
		return 1;
	}

	ALUA(_give_char_privilege)
	{
		int pid = CQuestManager::instance().GetCurrentCharacterPtr()->GetPlayerID();
		int type = (int)lua_tonumber(L, 1);
		int value = (int)lua_tonumber(L, 2);

		if (MAX_PRIV_NUM <= type)
		{
			sys_err("PRIV_MANAGER: _give_char_privilege: wrong empire priv type(%u)", type);
			return 0;
		}

		CPrivManager::instance().RequestGiveCharacterPriv(pid, type, value);

		return 0;
	}

	ALUA(_give_empire_privilege)
	{
		int empire = (int)lua_tonumber(L, 1);
		int type = (int)lua_tonumber(L, 2);
		int value = (int)lua_tonumber(L, 3);
		int time = (int)lua_tonumber(L, 4);
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (MAX_PRIV_NUM <= type)
		{
			sys_err("PRIV_MANAGER: _give_empire_privilege: wrong empire priv type(%u)", type);
			return 0;
		}

		if (ch)
			sys_log(0, "_give_empire_privileage(empire=%d, type=%d, value=%d, time=%d), by quest, %s",
				empire, type, value, time, ch->GetName());
		else
			sys_log(0, "_give_empire_privileage(empire=%d, type=%d, value=%d, time=%d), by quest, NULL",
				empire, type, value, time);

		CPrivManager::instance().RequestGiveEmpirePriv(empire, type, value, time);
		return 0;
	}

	ALUA(_give_guild_privilege)
	{
		int guild_id = (int)lua_tonumber(L, 1);
		int type = (int)lua_tonumber(L, 2);
		int value = (int)lua_tonumber(L, 3);
		int time = (int)lua_tonumber(L, 4);

		if (MAX_PRIV_NUM <= type)
		{
			sys_err("PRIV_MANAGER: _give_guild_privilege: wrong empire priv type(%u)", type);
			return 0;
		}

		sys_log(0, "_give_guild_privileage(empire=%d, type=%d, value=%d, time=%d)",
			guild_id, type, value, time);

		CPrivManager::instance().RequestGiveGuildPriv(guild_id, type, value, time);

		return 0;
	}

	ALUA(_get_empire_privilege_string)
	{
		int empire = (int)lua_tonumber(L, 1);
		ostringstream os;
		bool found = false;

		for (int type = PRIV_NONE + 1; type < MAX_PRIV_NUM; ++type)
		{
			CPrivManager::SPrivEmpireData* pkPrivEmpireData = CPrivManager::instance().GetPrivByEmpireEx(empire, type);

			if (pkPrivEmpireData && pkPrivEmpireData->m_value)
			{
				if (found)
					os << ", ";

				os << LC_TEXT(c_apszPrivNames[type]) << " : " <<
					pkPrivEmpireData->m_value << "%" << " (" <<
					((pkPrivEmpireData->m_end_time_sec - get_global_time()) / 3600.0f) << " hours)" << endl;
				found = true;
			}
		}

		if (!found)
			os << "None!" << endl;

		lua_pushstring(L, os.str().c_str());
		return 1;
	}

	ALUA(_get_empire_privilege)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushnumber(L, 0);
			return 1;
		}
		int empire = (int)lua_tonumber(L, 1);
		int type = (int)lua_tonumber(L, 2);
		int value = CPrivManager::instance().GetPrivByEmpire(empire, type);
		lua_pushnumber(L, value);
		return 1;
	}

	ALUA(_get_guild_privilege_string)
	{
		int guild = (int)lua_tonumber(L, 1);
		ostringstream os;
		bool found = false;

		for (int type = PRIV_NONE + 1; type < MAX_PRIV_NUM; ++type)
		{
			const CPrivManager::SPrivGuildData* pPrivGuildData = CPrivManager::instance().GetPrivByGuildEx(guild, type);

			if (pPrivGuildData && pPrivGuildData->value)
			{
				if (found)
					os << ", ";

				os << LC_TEXT(c_apszPrivNames[type]) << " : " << pPrivGuildData->value << "%"
					<< " (" << ((pPrivGuildData->end_time_sec - get_global_time()) / 3600.0f) << " hours)" << endl;
				found = true;
			}
		}

		if (!found)
			os << "None!" << endl;

		lua_pushstring(L, os.str().c_str());
		return 1;
	}

	ALUA(_get_guildid_byname)
	{
		if (!lua_isstring(L, 1)) {
			sys_err("_get_guildid_byname() - invalud argument");
			lua_pushnumber(L, 0);
			return 1;
		}

		const char* pszGuildName = lua_tostring(L, 1);
		CGuild* pFindGuild = CGuildManager::instance().FindGuildByName(pszGuildName);
		if (pFindGuild)
			lua_pushnumber(L, pFindGuild->GetID());
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(_get_guild_privilege)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushnumber(L, 0);
			return 1;
		}
		int guild = (int)lua_tonumber(L, 1);
		int type = (int)lua_tonumber(L, 2);
		int value = CPrivManager::instance().GetPrivByGuild(guild, type);
		lua_pushnumber(L, value);
		return 1;
	}

	ALUA(_item_name)
	{
		if (lua_isnumber(L, 1))
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			TItemTable* pTable = ITEM_MANAGER::instance().GetTable(dwVnum);
			if (pTable)
				lua_pushstring(L, pTable->szLocaleName);
			else
				lua_pushstring(L, "");
		}
		else
			lua_pushstring(L, "");
		return 1;
	}

	ALUA(_mob_name)
	{
		if (lua_isnumber(L, 1))
		{
			DWORD dwVnum = (DWORD)lua_tonumber(L, 1);
			const CMob* pkMob = CMobManager::instance().Get(dwVnum);

			if (pkMob)
				lua_pushstring(L, pkMob->m_table.szLocaleName);
			else
				lua_pushstring(L, "");
		}
		else
			lua_pushstring(L, "");

		return 1;
	}

	ALUA(_mob_vnum)
	{
		if (lua_isstring(L, 1))
		{
			const char* str = lua_tostring(L, 1);
			const CMob* pkMob = CMobManager::instance().Get(str, false);
			if (pkMob)
				lua_pushnumber(L, pkMob->m_table.dwVnum);
			else
				lua_pushnumber(L, 0);
		}
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(_get_global_time)
	{
		lua_pushnumber(L, get_global_time());
		return 1;
	}

	ALUA(_get_channel_id)
	{
		lua_pushnumber(L, g_bChannel);

		return 1;
	}

	ALUA(_do_command)
	{
		if (!lua_isstring(L, 1))
			return 0;

		const char* str = lua_tostring(L, 1);
		size_t len = strlen(str);

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		::interpret_command(ch, str, len);
		return 0;
	}

	ALUA(_find_pc)
	{
		if (!lua_isstring(L, 1))
		{
			sys_err("invalid argument");
			lua_pushnumber(L, 0);
			return 1;
		}

		const char* name = lua_tostring(L, 1);
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);
		lua_pushnumber(L, tch ? tch->GetVID() : 0);
		return 1;
	}

	ALUA(_find_pc_cond)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			sys_err("invalid argument");
			lua_pushnumber(L, 0);
			return 1;
		}

		int iMinLev = (int)lua_tonumber(L, 1);
		int iMaxLev = (int)lua_tonumber(L, 2);
		unsigned int uiJobFlag = (unsigned int)lua_tonumber(L, 3);

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		LPCHARACTER tch;

		if (test_server)
		{
			sys_log(0, "find_pc_cond map=%d, job=%d, level=%d~%d",
				ch->GetMapIndex(),
				uiJobFlag,
				iMinLev, iMaxLev);
		}

		tch = CHARACTER_MANAGER::instance().FindSpecifyPC(uiJobFlag,
			ch->GetMapIndex(),
			ch,
			iMinLev,
			iMaxLev);

		lua_pushnumber(L, tch ? tch->GetVID() : 0);
		return 1;
	}

	ALUA(_find_npc_by_vnum)
	{
		if (!lua_isnumber(L, 1))
		{
			sys_err("invalid argument");
			lua_pushnumber(L, 0);
			return 1;
		}

		DWORD race = (DWORD)lua_tonumber(L, 1);

		CharacterVectorInteractor i;

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(race, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();

			while (it != i.end())
			{
				LPCHARACTER tch = *(it++);

				if (tch->GetMapIndex() == CQuestManager::instance().GetCurrentCharacterPtr()->GetMapIndex())
				{
					lua_pushnumber(L, tch->GetVID());
					return 1;
				}
			}
		}

		//sys_err("not find(race=%d)", race);

		lua_pushnumber(L, 0);
		return 1;
	}

	// 새로운 state를 만든다.
	ALUA(_set_quest_state)
	{
		if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
			return 0;

		CQuestManager & q = CQuestManager::instance();
		QuestState * pqs = q.GetCurrentState();
		PC * pPC = q.GetCurrentPC();
		//assert(L == pqs->co);
		if (L != pqs->co)
		{
			luaL_error(L, "running thread != current thread???");
			sys_log(0, "running thread != current thread???");
			return -1;
		}
		if (pPC)
		{
			//const char* szQuestName = lua_tostring(L, 1);
			//const char* szStateName = lua_tostring(L, 2);
			const string stQuestName(lua_tostring(L, 1));
			const string stStateName(lua_tostring(L, 2));
			if (test_server)
				sys_log(0, "set_state %s %s ", stQuestName.c_str(), stStateName.c_str());
			if (pPC->GetCurrentQuestName() == stQuestName)
			{
				pqs->st = q.GetQuestStateIndex(pPC->GetCurrentQuestName(), lua_tostring(L, -1));
				pPC->SetCurrentQuestStateName(lua_tostring(L, -1));
			}
			else
			{
				pPC->SetQuestState(stQuestName, stStateName);
			}
		}
		return 0;
	}

	ALUA(_get_quest_state)
	{
		if (!lua_isstring(L, 1))
			return 0;

		CQuestManager& q = CQuestManager::instance();
		PC* pPC = q.GetCurrentPC();

		if (pPC)
		{
			std::string stQuestName = lua_tostring(L, 1);
			stQuestName += ".__status";

			int nRet = pPC->GetFlag(stQuestName.c_str());

			lua_pushnumber(L, nRet);

			if (test_server)
				sys_log(0, "Get_quest_state name %s value %d", stQuestName.c_str(), nRet);
		}
		else
		{
			if (test_server)
				sys_log(0, "PC == 0 ");

			lua_pushnumber(L, 0);
		}
		return 1;
	}

	ALUA(_under_han)
	{
		if (!lua_isstring(L, 1))
			lua_pushboolean(L, 0);
		else
			lua_pushboolean(L, under_han(lua_tostring(L, 1)));
		return 1;
	}

	ALUA(_notice_all)
	{
		ostringstream s;
		combine_lua_string(L, s);

		TPacketGGNotice p;
		p.bHeader = HEADER_GG_NOTICE;
		p.lSize = strlen(s.str().c_str()) + 1;

		TEMP_BUFFER buf;
		buf.write(&p, sizeof(p));
		buf.write(s.str().c_str(), p.lSize);

		P2P_MANAGER::instance().Send(buf.read_peek(), buf.size()); // HEADER_GG_NOTICE

		SendNotice(s.str().c_str());
		return 1;
	}

	EVENTINFO(warp_all_to_village_event_info)
	{
		DWORD dwWarpMapIndex;

		warp_all_to_village_event_info()
			: dwWarpMapIndex(0)
		{
		}
	};

	struct FWarpAllToVillage
	{
		FWarpAllToVillage() {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;
				if (ch->IsPC() && !ch->IsGM())
				{
					BYTE bEmpire = ch->GetEmpire();
					if (bEmpire == 0)
					{
						sys_err("Unkonwn Empire %s %d ", ch->GetName(), ch->GetPlayerID());
						return;
					}
					ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
				}
			}
		}
	};

	EVENTFUNC(warp_all_to_village_event)
	{
		warp_all_to_village_event_info* info = dynamic_cast<warp_all_to_village_event_info*>(event->info);

		if (info == nullptr)
		{
			sys_err("warp_all_to_village_event> <Factor> nullptr pointer");
			return 0;
		}

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

		if (nullptr != pSecMap)
		{
			FWarpAllToVillage f;
			pSecMap->for_each(f);
		}

		return 0;
	}

	ALUA(_warp_all_to_village)
	{
		int iMapIndex = static_cast<int>(lua_tonumber(L, 1));
		int iSec = static_cast<int>(lua_tonumber(L, 2));

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = iMapIndex;

		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(iSec));

		SendNoticeMap(LC_TEXT("잠시후 모두 마을로 이동됩니다."), iMapIndex, false);

		return 0;
	}

	ALUA(_get_table_postfix2)
	{
		lua_pushstring(L, get_table_postfix());
		return 1;
	}

	ALUA(_mysql_direct_query2)
	{
		// char szQuery[1024];

		if (!lua_isstring(L, 1))
			return 0;
		// strncpy(szQuery, lua_tostring(L, 1), sizeof(szQuery));

		int i = 0, m = 1;
		MYSQL_ROW row;
		MYSQL_FIELD* field;
		MYSQL_RES* result;
		// SQLMsg * pMsg = DBManager::instance().DirectQuery(szQuery);
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(lua_tostring(L, 1)));
		if (pMsg.get())
		{
			// ret1 (number of affected rows)
			lua_pushnumber(L, pMsg->Get()->uiAffectedRows); //-1 if error such as duplicate occurs (-2147483648)
			// ret2 (table of affected rows)
			lua_newtable(L);
			if ((result = pMsg->Get()->pSQLResult) &&
				!(pMsg->Get()->uiAffectedRows == 0 || pMsg->Get()->uiAffectedRows == (uint32_t)-1))
			{
				// LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %u fields\n", __FUNCTION__, mysql_num_fields(result));
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %u rows\n", __FUNCTION__, mysql_num_rows(result));
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Affected %u rows\n", __FUNCTION__, pMsg->Get()->uiAffectedRows);
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Num %u rows\n", __FUNCTION__, pMsg->Get()->uiNumRows);

				while ((row = mysql_fetch_row(result)))
				{
					lua_pushnumber(L, m);
					lua_newtable(L);
					while ((field = mysql_fetch_field(result)))
					{
						lua_pushstring(L, field->name);
						if (!(field->flags & NOT_NULL_FLAG) && (row[i] == nullptr))
						{
							// lua_pushstring(L, "nullptr");
							lua_pushnil(L);
						}
						else if (IS_NUM(field->type))
						{
							double val = NAN;
							lua_pushnumber(L, (sscanf(row[i], "%lf", &val) == 1) ? val : NAN);
						}
						else if (field->type == MYSQL_TYPE_BLOB)
						{
							lua_newtable(L);
							for (DWORD iBlob = 0; iBlob < field->max_length; iBlob++)
							{
								lua_pushnumber(L, row[i][iBlob]);
								lua_rawseti(L, -2, iBlob + 1);
							}
						}
						else
							lua_pushstring(L, row[i]);
						// LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
						// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %d flag %s for %s\n", __FUNCTION__, field->type, field->name, row[i]?row[i]:"nullptr");
						lua_rawset(L, -3);
						i++;
					}
					mysql_field_seek(result, 0);
					i = 0;

					lua_rawset(L, -3);
					m++;
				}
			}
		}
		else { lua_pushnumber(L, 0); lua_newtable(L); }

		// delete pMsg;
		return 2;
	}

	ALUA(_mysql_escape_string2)
	{
		char szQuery[1024] = { 0 };

		if (!lua_isstring(L, 1))
			return 0;
		// mysql_escape_string(szQuery, lua_tostring(L, 1), sizeof(szQuery));
		DBManager::instance().EscapeString(szQuery, sizeof(szQuery), lua_tostring(L, 1), strlen(lua_tostring(L, 1)));
		lua_pushstring(L, szQuery);
		return 1;
	}

	ALUA(_warp_to_village)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (nullptr != ch)
		{
			BYTE bEmpire = ch->GetEmpire();
			ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
		}

		return 0;
	}

	ALUA(_say_in_map)
	{
		int iMapIndex = static_cast<int>(lua_tonumber(L, 1));
		std::string Script(lua_tostring(L, 2));

		Script += "[ENTER]";
		Script += "[DONE]";

		struct ::packet_script packet_script;

		packet_script.header = HEADER_GC_SCRIPT;
		packet_script.skin = CQuestManager::QUEST_SKIN_NORMAL;
		packet_script.src_size = Script.size();
		packet_script.size = packet_script.src_size + sizeof(struct packet_script);

		FSendPacket f;
		f.buf.write(&packet_script, sizeof(struct packet_script));
		f.buf.write(&Script[0], Script.size());

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(iMapIndex);

		if (pSecMap)
		{
			pSecMap->for_each(f);
		}

		return 0;
	}

	struct FKillSectree2
	{
		void operator () (LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;
#ifdef ENABLE_GROWTH_PET_SYSTEM
				if (!ch->IsPC() && !ch->IsPet() && !ch->IsNewPet() && !ch->IsMount() && !ch->IsSupportShaman())
#else
				if (!ch->IsPC() && !ch->IsPet() && !ch->IsSupportShaman())
#endif
					ch->Dead();
			}
		}
	};

	ALUA(_kill_all_in_map)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(lua_tonumber(L, 1));

		if (nullptr != pSecMap)
		{
			FKillSectree2 f;
			pSecMap->for_each(f);
		}

		return 0;
	}

	//주의: 몹 리젠이 안되는 맵에서만 사용
	ALUA(_regen_in_map)
	{
		int iMapIndex = static_cast<int>(lua_tonumber(L, 1));
		std::string szFilename(lua_tostring(L, 2));

		LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(iMapIndex);

		if (pkMap != nullptr)
		{
			regen_load_in_file(szFilename.c_str(), iMapIndex, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
		}

		return 0;
	}

	ALUA(_add_ox_quiz)
	{
		int level = (int)lua_tonumber(L, 1);
		const char* quiz = lua_tostring(L, 2);
		bool answer = lua_toboolean(L, 3);

		if (COXEventManager::instance().AddQuiz(level, quiz, answer) == false)
		{
			sys_log(0, "OXEVENT : Cannot add quiz. %d %s %d", level, quiz, answer);
		}

		return 1;
	}

	EVENTFUNC(warp_all_to_map_my_empire_event)
	{
		warp_all_to_map_my_empire_event_info* info = dynamic_cast<warp_all_to_map_my_empire_event_info*>(event->info);

		if (info == nullptr)
		{
			sys_err("warp_all_to_map_my_empire_event> <Factor> nullptr pointer");
			return 0;
		}

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->m_lMapIndexFrom);

		if (pSecMap)
		{
			FWarpEmpire f;

			f.m_lMapIndexTo = info->m_lMapIndexTo;
			f.m_x = info->m_x;
			f.m_y = info->m_y;
			f.m_bEmpire = info->m_bEmpire;

			pSecMap->for_each(f);
		}

		return 0;
	}

	ALUA(_block_chat)
	{
		LPCHARACTER pChar = CQuestManager::instance().GetCurrentCharacterPtr();

		if (pChar != nullptr)
		{
			if (lua_isstring(L, 1) != true && lua_isstring(L, 2) != true)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			std::string strName(lua_tostring(L, 1));
			std::string strTime(lua_tostring(L, 2));

			std::string strArg = strName + " " + strTime;

			do_block_chat(pChar, const_cast<char*>(strArg.c_str()), 0, 0);

			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushboolean(L, false);
		return 1;
	}

	ALUA(_spawn_mob)
	{
		if (false == lua_isnumber(L, 1) || false == lua_isnumber(L, 2) || false == lua_isboolean(L, 3))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		const DWORD dwVnum = static_cast<DWORD>(lua_tonumber(L, 1));
		const size_t count = MINMAX(1, static_cast<size_t>(lua_tonumber(L, 2)), 10);
		const bool isAggresive = static_cast<bool>(lua_toboolean(L, 3));
		size_t SpawnCount = 0;

		const CMob* pMonster = CMobManager::instance().Get(dwVnum);

		if (nullptr != pMonster)
		{
			const LPCHARACTER pChar = CQuestManager::instance().GetCurrentCharacterPtr();

			for (size_t i = 0; i < count; ++i)
			{
				const LPCHARACTER pSpawnMonster = CHARACTER_MANAGER::instance().SpawnMobRange(dwVnum,
					pChar->GetMapIndex(),
					pChar->GetX() - number(200, 750),
					pChar->GetY() - number(200, 750),
					pChar->GetX() + number(200, 750),
					pChar->GetY() + number(200, 750),
					true,
					pMonster->m_table.bType == CHAR_TYPE_STONE,
					isAggresive);

				if (nullptr != pSpawnMonster)
				{
					++SpawnCount;
				}
			}

			sys_log(0, "QUEST Spawn Monstster: VNUM(%u) COUNT(%u) isAggresive(%b)", dwVnum, SpawnCount, isAggresive);
		}

		lua_pushnumber(L, SpawnCount);

		return 1;
	}

	ALUA(_spawn_mob0)
	{
		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			lua_pushnumber(L, -1);
			return 1;
		}
		const DWORD dwVnum = static_cast<DWORD>(lua_tonumber(L, 1));
		const long lMapIndex = static_cast<long>(lua_tonumber(L, 2));
		const DWORD dwX = static_cast<DWORD>(lua_tonumber(L, 3));
		const DWORD dwY = static_cast<DWORD>(lua_tonumber(L, 4));

		const CMob* pMonster = CMobManager::instance().Get(dwVnum);
		if (!pMonster)
		{
			lua_pushnumber(L, -2);
			return 1;
		}
		LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
		if (!pkSectreeMap)
		{
			lua_pushnumber(L, -3);
			return 1;
		}
		const LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwVnum, lMapIndex, pkSectreeMap->m_setting.iBaseX + dwX * 100, pkSectreeMap->m_setting.iBaseY + dwY * 100, 0, false, -1);
		lua_pushnumber(L, (ch) ? ch->GetVID() : 0);
		return 1;
	}

	ALUA(_notice_in_map)
	{
		const LPCHARACTER pChar = CQuestManager::instance().GetCurrentCharacterPtr();

		if (nullptr != pChar)
		{
			SendNoticeMap(lua_tostring(L, 1), pChar->GetMapIndex(), lua_toboolean(L, 2));
		}

		return 0;
	}

	ALUA(_get_locale_base_path)
	{
		lua_pushstring(L, LocaleService_GetBasePath().c_str());

		return 1;
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
#ifdef ENABLE_GROWTH_PET_SYSTEM
				if (!pChar->IsPet() && !pChar->IsNewPet() && !pChar->IsMount() && !pChar->IsSupportShaman() && (true == pChar->IsMonster() || true == pChar->IsStone()))
#else
				if (!pChar->IsPet() && !pChar->IsMount() && !pChar->IsSupportShaman() && (true == pChar->IsMonster() || true == pChar->IsStone()))
#endif
				{
					if (x1 <= pChar->GetX() && pChar->GetX() <= x2 && y1 <= pChar->GetY() && pChar->GetY() <= y2)
					{
						M2_DESTROY_CHARACTER(pChar);
					}
				}
			}
		}
	};

	ALUA(_purge_area)
	{
		int x1 = lua_tonumber(L, 1);
		int y1 = lua_tonumber(L, 2);
		int x2 = lua_tonumber(L, 3);
		int y2 = lua_tonumber(L, 4);

		const int mapIndex = SECTREE_MANAGER::instance().GetMapIndex(x1, y1);

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

	struct FWarpAllInAreaToArea
	{
		int from_x1, from_y1, from_x2, from_y2;
		int to_x1, to_y1, to_x2, to_y2;
		size_t warpCount;

		FWarpAllInAreaToArea(int a, int b, int c, int d, int e, int f, int g, int h)
			: from_x1(a), from_y1(b), from_x2(c), from_y2(d),
			to_x1(e), to_y1(f), to_x2(g), to_y2(h),
			warpCount(0)
		{}

		void operator () (LPENTITY ent)
		{
			if (true == ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pChar = static_cast<LPCHARACTER>(ent);

				if (true == pChar->IsPC())
				{
					if (from_x1 <= pChar->GetX() && pChar->GetX() <= from_x2 && from_y1 <= pChar->GetY() && pChar->GetY() <= from_y2)
					{
						++warpCount;

						pChar->WarpSet(number(to_x1, to_x2), number(to_y1, to_y2));
					}
				}
			}
		}
	};

	ALUA(_warp_all_in_area_to_area)
	{
		int from_x1 = lua_tonumber(L, 1);
		int from_y1 = lua_tonumber(L, 2);
		int from_x2 = lua_tonumber(L, 3);
		int from_y2 = lua_tonumber(L, 4);

		int to_x1 = lua_tonumber(L, 5);
		int to_y1 = lua_tonumber(L, 6);
		int to_x2 = lua_tonumber(L, 7);
		int to_y2 = lua_tonumber(L, 8);

		const int mapIndex = SECTREE_MANAGER::instance().GetMapIndex(from_x1, from_y1);

		if (0 == mapIndex)
		{
			sys_err("_warp_all_in_area_to_area: cannot get a map index with (%u, %u)", from_x1, from_y1);
			lua_pushnumber(L, 0);
			return 1;
		}

		LPSECTREE_MAP pSectree = SECTREE_MANAGER::instance().GetMap(mapIndex);

		if (nullptr != pSectree)
		{
			FWarpAllInAreaToArea func(from_x1, from_y1, from_x2, from_y2, to_x1, to_y1, to_x2, to_y2);

			pSectree->for_each(func);

			lua_pushnumber(L, func.warpCount);
			sys_log(0, "_warp_all_in_area_to_area: %u character warp", func.warpCount);
			return 1;
		}
		else
		{
			lua_pushnumber(L, 0);
			sys_err("_warp_all_in_area_to_area: no sectree");
			return 1;
		}
	}

	ALUA(_get_special_item_group)
	{
		if (!lua_isnumber(L, 1))
		{
			sys_err("invalid argument");
			return 0;
		}

		const CSpecialItemGroup* pItemGroup = ITEM_MANAGER::instance().GetSpecialItemGroup((DWORD)lua_tonumber(L, 1));

		if (!pItemGroup)
		{
			sys_err("cannot find special item group %d", (DWORD)lua_tonumber(L, 1));
			return 0;
		}

		int count = pItemGroup->GetGroupSize();

		for (int i = 0; i < count; i++)
		{
			lua_pushnumber(L, (int)pItemGroup->GetVnum(i));
			lua_pushnumber(L, (int)pItemGroup->GetCount(i));
		}

		return count * 2;
	}

#ifdef ENABLE_VERSION_162_UPDATE
	ALUA(_add_restart_city_pos)
	{
		int iMapIndex = (int)lua_tonumber(L, 1);
		int iEmpire = (int)lua_tonumber(L, 2);
		int iX = (int)lua_tonumber(L, 3);
		int iY = (int)lua_tonumber(L, 4);
		int iZ = (int)lua_tonumber(L, 5);
		SECTREE_MANAGER::instance().AddRestartCityPos(iMapIndex, iEmpire, iX, iY, iZ);
		return 0;
	}
#endif

#ifdef ENABLE_FULL_NOTICE
	ALUA(_big_notice)
	{
		ostringstream s;
		combine_lua_string(L, s);
		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_BIG_NOTICE, "%s", s.str().c_str());
		return 0;
	}

	ALUA(_big_notice_in_map)
	{
		const LPCHARACTER pChar = CQuestManager::instance().GetCurrentCharacterPtr();

		if (nullptr != pChar)
		{
			SendNoticeMap(lua_tostring(L, 1), pChar->GetMapIndex(), true);
		}

		return 0;
	}

	ALUA(_big_notice_all)
	{
		ostringstream s;
		combine_lua_string(L, s);

		TPacketGGNotice p;
		p.bHeader = HEADER_GG_BIG_NOTICE;
		p.lSize = strlen(s.str().c_str()) + 1;

		TEMP_BUFFER buf;
		buf.write(&p, sizeof(p));
		buf.write(s.str().c_str(), p.lSize);

		P2P_MANAGER::instance().Send(buf.read_peek(), buf.size()); // HEADER_GG_NOTICE

		SendNotice(s.str().c_str(), true);
		return 1;
	}
#endif

#ifdef ENABLE_LUA_FUNCTION_TIME
	#include <time.h>       /* time_t, struct tm, time, localtime */

	ALUA(_print_datetime)
	{
		time_t currentTime;
		struct tm *localTime;
		
		time(&currentTime);
		localTime = localtime(&currentTime);
		CQuestManager::Instance().GetCurrentCharacterPtr()->ChatPacket(CHAT_TYPE_NOTICE, "%s", asctime(localTime));
		return 0;
	}
#endif
	
	ALUA(_is_player_online)
	{
		if (!lua_isstring(L, 1))
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(lua_tostring(L, 1));
		if (!ch)
		{
			CCI* pkCCI = P2P_MANAGER::instance().Find(lua_tostring(L, 1));
			if (!pkCCI)
			{
				lua_pushboolean(L, 0);
				return 1;
			}
		}

		lua_pushboolean(L, 1);
		return 1;
	}
	
	ALUA(_is_feature_enabled)
	{
		if (!lua_isnumber(L, 1))
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		lua_pushboolean(L, features::IsFeatureEnabled(lua_tonumber(L, 1)));
		return 1;
	}
	
	ALUA(_set_feature_disable)
	{
		if (!lua_isnumber(L, 1))
		{
			lua_pushboolean(L, 0);
			return 1;
		}
		
		features::SetFeatureEnabled(lua_tonumber(L, 1), false);
		return 0;
	}
	
	ALUA(_set_feature_enable)
	{
		if (!lua_isnumber(L, 1))
		{
			lua_pushboolean(L, 0);
			return 1;
		}
		
		features::SetFeatureEnabled(lua_tonumber(L, 1), true);
		return 0;
	}
	
	void RegisterGlobalFunctionTable(lua_State * L)
	{
		extern ALUA(quest_setstate);

		luaL_reg global_functions[] =
		{
			{"sys_err", _syserr},
			{"sys_log", _syslog},
			{"char_log", _char_log},
			{"item_log", _item_log},
			{"set_state", quest_setstate},
			{"set_skin", _set_skin},
			{"setskin", _set_skin},
			{"time_to_str", _time_to_str},
			{"say", _say},
			{"chat", _chat},
			{"cmdchat", _cmdchat},
			{"syschat", _syschat},
			{"get_locale", _get_locale},
			{"setleftimage", _left_image},
			{"settopimage", _top_image},
			{"server_timer", _set_server_timer},
			{"clear_server_timer", _clear_server_timer},
			{"server_loop_timer", _set_server_loop_timer},
			{"get_server_timer_arg", _get_server_timer_arg},
			{"timer", _timer},
			{"loop_timer", _set_named_loop_timer},
			{"cleartimer", _clear_named_timer},
			{"getnpcid", _getnpcid},
			{"is_test_server", _is_test_server},
			{"raw_script", _raw_script},
			{"number", _number},
			{"set_bgm_volume_enable", _set_bgm_volume_enable},
			{"add_bgm_info", _add_bgm_info},
			{"add_goto_info", _add_goto_info},
			{"__refine_pick", _refine_pick},
			{"add_ox_quiz", _add_ox_quiz},
			{"__fish_real_refine_rod", _fish_real_refine_rod}, // XXX
			{"__give_char_priv", _give_char_privilege},
			{"__give_empire_priv", _give_empire_privilege},
			{"__give_guild_priv", _give_guild_privilege},
			{"__get_empire_priv_string", _get_empire_privilege_string},
			{"__get_empire_priv", _get_empire_privilege},
			{"__get_guild_priv_string", _get_guild_privilege_string},
			{"__get_guildid_byname", _get_guildid_byname},
			{"__get_guild_priv", _get_guild_privilege},
			{"item_name", _item_name},
			{"mob_name", _mob_name},
			{"mob_vnum", _mob_vnum},
			{"get_time", _get_global_time},
			{"get_global_time", _get_global_time},
			{"get_channel_id", _get_channel_id},
			{"command", _do_command},
			{"find_pc_cond", _find_pc_cond},
			{"find_pc_by_name", _find_pc},
			{"find_npc_by_vnum", _find_npc_by_vnum},
			{"set_quest_state", _set_quest_state},
			{"get_quest_state", _get_quest_state},
			{"under_han", _under_han},
			{"notice", _notice},
			{"notice_all", _notice_all},
			{"notice_in_map", _notice_in_map},
			{"warp_all_to_village", _warp_all_to_village},
			{"warp_to_village", _warp_to_village},
			{"say_in_map", _say_in_map},
			{"kill_all_in_map", _kill_all_in_map},
			{"regen_in_map", _regen_in_map},
			{"block_chat", _block_chat},
			{"spawn_mob", _spawn_mob},
			{"get_locale_base_path", _get_locale_base_path},
			{"purge_area", _purge_area},
			{"warp_all_in_area_to_area", _warp_all_in_area_to_area},
			{"get_special_item_group", _get_special_item_group},
			{"get_table_postfix2", _get_table_postfix2},
			{"mysql_direct_query2", _mysql_direct_query2},
			{"mysql_escape_string2", _mysql_escape_string2},
#ifdef ENABLE_VERSION_162_UPDATE
			{"add_restart_city_pos", _add_restart_city_pos},
#endif
			{"spawn_mob0", _spawn_mob0},
#ifdef ENABLE_FULL_NOTICE
			{"big_notice", _big_notice},
			{"big_notice_all", _big_notice_all},
			{"big_notice_in_map", _big_notice_in_map},
#endif
#ifdef ENABLE_LUA_FUNCTION_TIME
			{"print_datetime", _print_datetime},
#endif
			{"is_player_online", _is_player_online},
			{"is_feature_enabled", _is_feature_enabled},
			{"set_feature_disable", _set_feature_disable},
			{"set_feature_enable", _set_feature_enable},
			{nullptr, nullptr	}
		};

		int i = 0;

		while (global_functions[i].name != nullptr)
		{
			lua_register(L, global_functions[i].name, global_functions[i].func);
			++i;
		}
	}
}