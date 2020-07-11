#include "stdafx.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "constants.h"
#include "char.h"
#include "char_manager.h"
#include "sectree_manager.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "db.h"
#include "questmanager.h"

#include "daily_boss.h"

CDailyBoss::CDailyBoss()
{
}

CDailyBoss::~CDailyBoss()
{
}

static const char* LC_TRANSLATE(const char* key)
{
	typedef std::map<const char*, const char*> TMapTranslate;
	TMapTranslate LC_TRANSLATE;

	LC_TRANSLATE["BOSS_SPAWNED_LINE_1"] = LC_TEXT("Enter to the Boss Map! Grinch was spawned!");
	LC_TRANSLATE["BOSS_SPAWNED_LINE_2"] = LC_TEXT("Kill him and get nice rewards!");
	LC_TRANSLATE["SPAWN_ERROR"] = LC_TEXT("Hmm, where is Grinch? ADMIN find him! something goes wrong with him...");
	LC_TRANSLATE["KILLED"] = LC_TEXT("Yeah, Grinch was killed! Everyone did great job, but last hit was %s.");

	return LC_TRANSLATE[key];
}

void CDailyBoss::SendNoticeLine(const char* format, ...)
{
	if (!format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	SendNotice(chatbuf);
}

void CDailyBoss::Check(int day, int hour, int minute, int second)
{
	if (ENABLE_DAILY == false && CHOOSEN_DAY != day)
		return;

	if (hour == START_HOUR && minute == START_MINUTE && second == 00)
	{
		if (CDailyBoss::Spawn() == true)
		{
			SendNotice(LC_TRANSLATE("BOSS_SPAWNED_LINE_1"));
			SendNotice(LC_TRANSLATE("BOSS_SPAWNED_LINE_2"));
		}
		else
		{
			SendNotice(LC_TRANSLATE("SPAWN_ERROR"));
		}
	}
}

bool CDailyBoss::Spawn()
{
	CharacterVectorInteractor i;

	//delete if exist
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(MOB_VNUM, i))
	{
		auto it = i.begin();

		while (it != i.end())
			M2_DESTROY_CHARACTER(*it++);
	}

	//resp

	if (map_allow_find(MAP_INDEX))
	{
		PIXEL_POSITION posBase;
		if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(MAP_INDEX, posBase))
		{
			sys_err("cannot get map base position %d", MAP_INDEX);
			return false;
		}

		CHARACTER_MANAGER::instance().SpawnMob(MOB_VNUM, MAP_INDEX, posBase.x + SPAWN_X * 100, posBase.y + SPAWN_Y * 100, 0, false, -1);

		return true;
	}

	return false;
}

bool CDailyBoss::IsBossMap(int mapIndex)
{
	if (mapIndex == MAP_INDEX)
		return true;

	return false;
}

void CDailyBoss::OnKill(LPCHARACTER pkKiller)
{
	SendNotice(LC_TRANSLATE("KILLED"), pkKiller->GetName());
}

bool CDailyBoss::IsDay()
{
	time_t curTime = time(nullptr);
	struct tm vKey = *localtime(&curTime);

	if (DailyBossSettings::ENABLE_DAILY == false && vKey.tm_wday != DailyBossSettings::CHOOSEN_DAY)
		return false;

	return true;
}

namespace quest
{
	ALUA(dailyboss_start_time)
	{
		int timeIndex = (int)lua_tonumber(L, 1);

		int m_counter[2] =
		{
			CDailyBoss::instance().GetStartHour(), CDailyBoss::instance().GetStartMinute()
		};

		lua_pushnumber(L, m_counter[timeIndex - 1]);
		return 1;
	}

	ALUA(dailyboss_is_map)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();

		if (pkChar && CDailyBoss::instance().IsBossMap(pkChar->GetMapIndex()) == true)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

	ALUA(dailyboss_is_day)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();

		if (pkChar && CDailyBoss::instance().IsDay() == true)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

	void RegisterDailBossFunctionTable()
	{
		luaL_reg dailyboss_functions[] =
		{
			{"start_time", dailyboss_start_time},
			{"is_dailyboss_map", dailyboss_is_map},
			{"is_day", dailyboss_is_day},
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("dailyboss", dailyboss_functions);
	}
}