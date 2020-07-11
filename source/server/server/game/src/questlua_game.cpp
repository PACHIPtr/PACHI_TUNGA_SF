#include "stdafx.h"
#include "questlua.h"
#include "questmanager.h"
#include "desc_client.h"
#include "char.h"
#include "item_manager.h"
#include "item.h"
#include "cmd.h"
#include "packet.h"
#include "db.h"
#ifdef ENABLE_DICE_SYSTEM
#include "party.h"
#endif
#include "utils.h"
#ifdef ENABLE_MINI_GAME_CATCH_KING
#include "minigame.h"
#endif
#ifdef ENABLE_EVENT_SYSTEM
#include "game_events.h"
#endif
#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

extern bool CAN_OPEN_SAFEBOX(int map_index);

extern ACMD(do_in_game_mall);

namespace quest
{
	ALUA(game_set_event_flag)
	{
		CQuestManager& q = CQuestManager::instance();

		if (lua_isstring(L, 1) && lua_isnumber(L, 2))
			q.RequestSetEventFlag(lua_tostring(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	ALUA(game_get_event_flag)
	{
		CQuestManager& q = CQuestManager::instance();

		if (lua_isstring(L, 1))
			lua_pushnumber(L, q.GetEventFlag(lua_tostring(L, 1)));
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	ALUA(game_request_make_guild)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDESC d = q.GetCurrentCharacterPtr()->GetDesc();
		if (d)
		{
			BYTE header = HEADER_GC_REQUEST_MAKE_GUILD;
			d->Packet(&header, 1);
		}
		return 0;
	}

	ALUA(game_get_safebox_level)
	{
		CQuestManager& q = CQuestManager::instance();
		lua_pushnumber(L, q.GetCurrentCharacterPtr()->GetSafeboxSize() / SAFEBOX_PAGE_SIZE);
		return 1;
	}
	ALUA(game_mysql_query)
	{
		//MYSQL_FIELD *field;
		SQLMsg* run = DBManager::instance().DirectQuery(lua_tostring(L, 1));
		MYSQL_RES* res = run->Get()->pSQLResult;
		if (!res) {
			lua_pushnumber(L, 0);
			return 0;
		}
		MYSQL_ROW row;
		lua_newtable(L);
		int rowcount = 1;
		while ((row = mysql_fetch_row(res))) {
			lua_newtable(L);
			lua_pushnumber(L, rowcount);
			lua_pushvalue(L, -2);
			lua_settable(L, -4);
			unsigned int fields = mysql_num_fields(res);
			for (unsigned int i = 0; i < fields; i++) {
				lua_pushnumber(L, i + 1);
				lua_pushstring(L, row[i]);
				lua_settable(L, -3);
			}
			lua_pop(L, 1);
			rowcount++;
		}
		return 1;
	}

	ALUA(game_set_safebox_level)
	{
		CQuestManager& q = CQuestManager::instance();

		//q.GetCurrentCharacterPtr()->ChangeSafeboxSize(3*(int)lua_tonumber(L,-1));
		TSafeboxChangeSizePacket p;
		p.dwID = q.GetCurrentCharacterPtr()->GetDesc()->GetAccountTable().id;
		p.bSize = (int)lua_tonumber(L, -1);
		db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_SIZE, q.GetCurrentCharacterPtr()->GetDesc()->GetHandle(), &p, sizeof(p));

		q.GetCurrentCharacterPtr()->SetSafeboxSize(SAFEBOX_PAGE_SIZE * (int)lua_tonumber(L, -1));
		return 0;
	}

	ALUA(game_open_safebox)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		if (CAN_OPEN_SAFEBOX(ch->GetMapIndex()) == false)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
			return 0;
		}
		ch->SetSafeboxOpenPosition();
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
		return 0;
	}

	ALUA(game_open_mall)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();
		ch->SetSafeboxOpenPosition();
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
		return 0;
	}

	ALUA(game_drop_item)
	{
		//
		// Syntax: game.drop_item(50050, 1)
		//
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		DWORD item_vnum = (DWORD)lua_tonumber(L, 1);
		int count = (int)lua_tonumber(L, 2);
		long x = ch->GetX();
		long y = ch->GetY();

		LPITEM item = ITEM_MANAGER::instance().CreateItem(item_vnum, count);

		if (!item)
		{
			sys_err("cannot create item vnum %d count %d", item_vnum, count);
			return 0;
		}

		PIXEL_POSITION pos;
		pos.x = x + number(-200, 200);
		pos.y = y + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}

	ALUA(game_drop_item_with_ownership)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPITEM item = nullptr;
		switch (lua_gettop(L))
		{
		case 1:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1));
			break;
		case 2:
		case 3:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
			break;
		default:
			return 0;
		}

		if (item == nullptr)
		{
			return 0;
		}

		if (lua_isnumber(L, 3))
		{
			int sec = (int)lua_tonumber(L, 3);
			if (sec <= 0)
			{
				item->SetOwnership(ch);
			}
			else
			{
				item->SetOwnership(ch, sec);
			}
		}
		else
			item->SetOwnership(ch);

		PIXEL_POSITION pos;
		pos.x = ch->GetX() + number(-200, 200);
		pos.y = ch->GetY() + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}

#ifdef ENABLE_DICE_SYSTEM
	ALUA(game_drop_item_with_ownership_and_dice)
	{
		LPITEM item = nullptr;
		switch (lua_gettop(L))
		{
		case 1:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1));
			break;
		case 2:
		case 3:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
			break;
		default:
			return 0;
		}

		if (item == nullptr)
		{
			return 0;
		}

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		if (ch->GetParty())
		{
			FPartyDropDiceRoll f(item, ch);
			f.Process(nullptr);
		}

		if (lua_isnumber(L, 3))
		{
			int sec = (int)lua_tonumber(L, 3);
			if (sec <= 0)
			{
				item->SetOwnership(ch);
			}
			else
			{
				item->SetOwnership(ch, sec);
			}
		}
		else
			item->SetOwnership(ch);

		PIXEL_POSITION pos;
		pos.x = ch->GetX() + number(-200, 200);
		pos.y = ch->GetY() + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}
#endif

	ALUA(game_web_mall)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (ch != nullptr)
		{
			do_in_game_mall(ch, const_cast<char*>(""), 0, 0);
		}
		return 0;
	}

	//new quest functions
	ALUA(game_drop_item_and_select)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (!lua_isnumber(L, 1) && !lua_isnumber(L, 2))
		{
			sys_err("QUEST Make item call error : wrong argument");
			return 0;
		}

		DWORD item_vnum = (DWORD)lua_tonumber(L, 1);
		int count = (int)lua_tonumber(L, 2);
		long x = ch->GetX();
		long y = ch->GetY();

		LPITEM item = ITEM_MANAGER::instance().CreateItem(item_vnum, count);

		if (!item)
		{
			sys_err("cannot create item vnum %d count %d", item_vnum, count);
			return 0;
		}
		else
		{
			CQuestManager::Instance().SetCurrentItem(item);
		}

		PIXEL_POSITION pos;
		pos.x = x + number(-200, 200);
		pos.y = y + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}

	ALUA(game_mysql_query2)
	{
		//MYSQL_FIELD *field;
		SQLMsg* run = DBManager::instance().DirectQuery(lua_tostring(L, 1));
		MYSQL_RES* res = run->Get()->pSQLResult;

		if (!res)
		{
			lua_pushnumber(L, 0);
			return 0;
		}

		MYSQL_ROW row;
		lua_newtable(L);
		int rowcount = 1;
		while ((row = mysql_fetch_row(res)))
		{
			lua_newtable(L);
			lua_pushnumber(L, rowcount);
			lua_pushvalue(L, -2);
			lua_settable(L, -4);
			unsigned int fields = mysql_num_fields(res);
			for (unsigned int i = 0; i < fields; i++)
			{
				lua_pushnumber(L, i + 1);
				lua_pushstring(L, row[i]);
				lua_settable(L, -3);
			}
			lua_pop(L, 1);
			rowcount++;
		}
		return 1;
	}
#ifdef ENABLE_MINI_GAME_CATCH_KING
	ALUA(mini_game_catch_king_get_score)
	{
		DWORD dwArg = (DWORD)lua_tonumber(L, 1);
		bool isTotal = dwArg ? true : false;

		CMiniGame::instance().MiniGameCatchKingGetScore(L, isTotal);
		return 1;
	}
#endif

	ALUA(game_open_mailbox)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();

#ifdef ENABLE_MAIL_BOX_SYSTEM

		if (ch->IsOpenMailBox())
		{
			return 0;
		}
		ch->OpenMailBox();

#endif

		return 0;
	}
	
#ifdef ENABLE_EVENT_SYSTEM
	ALUA(game_set_event_time)
	{
		CQuestManager& q = CQuestManager::instance();

		if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
			CGameEventsManager::instance().SetEventTime((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}
#endif

	void RegisterGameFunctionTable()
	{
		luaL_reg game_functions[] =
		{
			{"get_safebox_level", game_get_safebox_level},
			{"request_make_guild", game_request_make_guild},
			{"set_safebox_level", game_set_safebox_level},
			{"open_safebox", game_open_safebox},
			{"open_mall", game_open_mall},
			{"get_event_flag", game_get_event_flag},
			{"set_event_flag", game_set_event_flag},
			{"drop_item", game_drop_item},
			{"drop_item_with_ownership", game_drop_item_with_ownership},
#ifdef ENABLE_DICE_SYSTEM
			{"drop_item_with_ownership_and_dice", game_drop_item_with_ownership_and_dice},
#endif
			{"open_web_mall", game_web_mall},
			{"mysql_query", game_mysql_query},
			{"drop_item_and_select", game_drop_item_and_select},
			{"mysql_query2", game_mysql_query2},
#ifdef ENABLE_MINI_GAME_CATCH_KING
			{"mini_game_catch_king_get_score", mini_game_catch_king_get_score},
#endif
			{"open_mailbox", game_open_mailbox},
#ifdef ENABLE_EVENT_SYSTEM
			{"set_event_time", game_set_event_time},
#endif
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("game", game_functions);
	}
}