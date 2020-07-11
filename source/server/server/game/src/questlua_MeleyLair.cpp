#include "stdafx.h"
#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "questlua.h"
#include "questmanager.h"
#include "MeleyLair.h"
#include "char.h"
#include "guild.h"

namespace quest
{
	ALUA(MeleyLair_GetRequirments)
	{
		lua_pushnumber(L, (int)(MeleyLair::MIN_LVL));
		lua_pushnumber(L, (int)(MeleyLair::LADDER_POINTS_COST));
		return 2;
	}

	ALUA(MeleyLair_GetPartecipantsLimit)
	{
		lua_pushnumber(L, (int)(MeleyLair::PARTECIPANTS_LIMIT));
		return 1;
	}

	ALUA(MeleyLair_GetSubMapIndex)
	{
		lua_pushnumber(L, (int)(MeleyLair::SUBMAP_INDEX));
		return 1;
	}

	ALUA(MeleyLair_Register)
	{
		int iRes1, iRes2;
		MeleyLair::CMgr::instance().Register(CQuestManager::instance().GetCurrentCharacterPtr(), iRes1, iRes2);
		lua_pushnumber(L, iRes1);
		lua_pushnumber(L, iRes2);
		return 2;
	}

	ALUA(MeleyLair_IsRegistered)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
		{
			int iCH = 0;
			bool bTry = pkChar->GetGuild() ? MeleyLair::CMgr::instance().isRegistered(pkChar->GetGuild(), iCH) : false;
			lua_pushboolean(L, bTry);
			lua_pushnumber(L, iCH);
		}
		else
		{
			lua_pushboolean(L, false);
			lua_pushnumber(L, 0);
		}

		return 2;
	}

	ALUA(MeleyLair_Enter)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
		{
			int iLimit = 0;
			bool bTry = pkChar->GetGuild() ? MeleyLair::CMgr::instance().Enter(pkChar->GetGuild(), pkChar, iLimit) : false;
			lua_pushboolean(L, bTry);
			lua_pushnumber(L, iLimit);
		}
		else
		{
			lua_pushboolean(L, false);
			lua_pushnumber(L, 1);
		}

		return 2;
	}

	ALUA(MeleyLair_IsMeleyMap)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		bool bTry = pkChar != nullptr ? MeleyLair::CMgr::instance().IsMeleyMap(pkChar->GetMapIndex()) : false;
		lua_pushboolean(L, bTry);

		return 1;
	}

	ALUA(MeleyLair_Check)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
			MeleyLair::CMgr::instance().Check(pkChar->GetGuild(), pkChar);

		return 0;
	}

	ALUA(MeleyLair_Leave)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
			MeleyLair::CMgr::instance().LeaveRequest(pkChar->GetGuild(), pkChar);

		return 0;
	}

	ALUA(MeleyLair_CanGetReward)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		bool bTry = pkChar != nullptr ? MeleyLair::CMgr::instance().CanGetReward(pkChar, pkChar->GetGuild()) : false;
		lua_pushboolean(L, bTry);

		return 1;
	}

	ALUA(MeleyLair_Reward)
	{
		if (!lua_isnumber(L, 1))
			return 0;

		BYTE bReward = (BYTE)lua_tonumber(L, 1);
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
			MeleyLair::CMgr::instance().Reward(pkChar, pkChar->GetGuild(), bReward);

		return 0;
	}

	ALUA(MeleyLair_OpenRanking)
	{
		MeleyLair::CMgr::instance().OpenRanking(CQuestManager::instance().GetCurrentCharacterPtr());
		return 0;
	}

	void RegisterMeleyLairFunctionTable()
	{
		luaL_reg functions[] =
		{
			{"GetRequirments", MeleyLair_GetRequirments},
			{"GetPartecipantsLimit", MeleyLair_GetPartecipantsLimit},
			{"GetSubMapIndex", MeleyLair_GetSubMapIndex},
			{"Register", MeleyLair_Register},
			{"IsRegistered", MeleyLair_IsRegistered},
			{"Enter", MeleyLair_Enter},
			{"IsMeleyMap", MeleyLair_IsMeleyMap},
			{"Check", MeleyLair_Check},
			{"Leave", MeleyLair_Leave},
			{"CanGetReward", MeleyLair_CanGetReward},
			{"Reward", MeleyLair_Reward},
			{"OpenRanking", MeleyLair_OpenRanking},
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("MeleyLair", functions);
	}
}
#endif