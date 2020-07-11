#include "stdafx.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "constants.h"
#include "daily_boss.h"
#include "auto_event_manager.h"
#include "questmanager.h"
#include "game_events.h"
#include "cmd.h"

static LPEVENT running_event = nullptr;

EVENTINFO(EventsManagerInfoData)
{
	CEventsManager* pEvents;

	EventsManagerInfoData()
		: pEvents(0)
	{
	}
};

EVENTFUNC(automatic_event_timer)
{
	if (event == nullptr)
		return 0;

	if (event->info == nullptr)
		return 0;

	EventsManagerInfoData * info = dynamic_cast<EventsManagerInfoData*>(event->info);

	if (info == nullptr)
		return 0;

	CEventsManager * pInstance = info->pEvents;

	if (pInstance == nullptr)
		return 0;

	CEventsManager::instance().PrepareChecker();
	return PASSES_PER_SEC(1);
}

void CEventsManager::PrepareChecker()
{
#ifdef ENABLE_AUTO_EVENT_SYSTEM
	time_t cur_Time = time(nullptr);
	struct tm vKey = *localtime(&cur_Time);

	int day = vKey.tm_wday;
	int hour = vKey.tm_hour;
	int minute = vKey.tm_min;
	int second = vKey.tm_sec;
#endif
	CDailyBoss::instance().Check(day, hour, minute, second);
	CEventsManager::instance().Check(day, hour, minute, second);
}

void CEventsManager::Check(int day, int hour, int minute, int second)
{
	for (int i = 0; i < EVENT_MAX_NUM; ++i)
	{
		if (CGameEventsManager::instance().IsActivateEvent(i) == true)
		{
			if (CGameEventsManager::instance().GetEventTime(i) <= get_global_time())
			{
				CGameEventsManager::instance().SetActivateEvent(i, false);
				if (i == EVENT_MOON_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_moon_drop", 0);
				else if (i == EVENT_HEXAGON_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_hexagon_drop", 0);
				else if (i == EVENT_CARVING_STONE_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_carving_stone_drop", 0);
				else if (i == EVENT_RAMADAN)
					quest::CQuestManager::instance().SetEventFlag("ex_ramadan_drop", 0);
				else if (i == EVENT_XMAS)
					quest::CQuestManager::instance().SetEventFlag("ex_xmas_drop", 0);
				else if (i == EVENT_FOOTBALL_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_football_drop", 0);
				else if (i == EVENT_VALENTINE_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_valentine_drop", 0);
				else if (i == EVENT_PET_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_pet_drop", 0);
				else if (i == EVENT_KIDS_DAY_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_kids_drop", 0);
				else if (i == EVENT_HALLOWEEN)
					quest::CQuestManager::instance().SetEventFlag("ex_halloween_drop", 0);
				else if (i == EVENT_OLYMPIC_BADGE)
					quest::CQuestManager::instance().SetEventFlag("ex_olympic_drop", 0);
				else if (i == EVENT_OKEY_CARD)
					quest::CQuestManager::instance().SetEventFlag("cards_event", 0);
				else if (i == EVENT_MOUNT_DROP)
					quest::CQuestManager::instance().SetEventFlag("ex_mount_drop", 0);
				else if (i == EVENT_ALIGNMENT)
					quest::CQuestManager::instance().SetEventFlag("alignment_event", 0);
				else if (i == EVENT_YEAR_ROUND)
					quest::CQuestManager::instance().SetEventFlag("2017_year_round_event", 0);
				else if (i == EVENT_NEW_FISH)
					quest::CQuestManager::instance().SetEventFlag("2018_fish_event", 0);
				else if (i == EVENT_MINING)
					quest::CQuestManager::instance().SetEventFlag("mining_event", 0);
				else if (i == EVENT_HARVEST_FESTIVAL)
					quest::CQuestManager::instance().SetEventFlag("harvest_festival", 0);
				else if (i == EVENT_MAKE_WONSO)
					quest::CQuestManager::instance().SetEventFlag("newyear_wonso", 0);
				else if (i == EVENT_BOSS_HUNTERS)
					quest::CQuestManager::instance().SetEventFlag("enable_attendance_event", 0);
				else if (i == EVENT_EASTER)
					quest::CQuestManager::instance().SetEventFlag("enable_easter_event", 0);
				else if (i == EVENT_CATCH_KING)
					quest::CQuestManager::instance().SetEventFlag("enable_catch_king_event", 0);
				else if (i == EVENT_GOLDEN_RAIN)
					quest::CQuestManager::instance().SetEventFlag("golden_rain_event", 0);
				else if (i == EVENT_SUPER_METIN)
					quest::CQuestManager::instance().SetEventFlag("super_metin_event", 0);
			}
		}
	}
	if (hour == 19 && quest::CQuestManager::instance().GetEventFlag("2017_mount_drop") == 0)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_mount_drop", 1);
		CGameEventsManager::instance().SetEventTime(EVENT_MOUNT_DROP, 60*60);
		SendNotice(LC_TEXT("2017_MOUNT_DROP_AUTO_EVENT_ON"), true);
	}

	if (hour == 20 && quest::CQuestManager::instance().GetEventFlag("2017_mount_drop") == 1)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_mount_drop", 0);
		SendNotice(LC_TEXT("2017_MOUNT_DROP_AUTO_EVENT_OFF"), true);
	}
	
	if (day == FRIDAY && hour == 18 && quest::CQuestManager::instance().GetEventFlag("2017_football_drop") == 0)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_football_drop", 1);
		CGameEventsManager::instance().SetEventTime(EVENT_FOOTBALL_DROP, 2*60*60);
		SendNotice(LC_TEXT("2017_FOOTBALL_DROP_AUTO_EVET_ON"), true);
	}
	else if (day == FRIDAY && hour == 20 && quest::CQuestManager::instance().GetEventFlag("2017_football_drop") == 1)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_football_drop", 0);
		SendNotice(LC_TEXT("2017_FOOTBALL_DROP_AUTO_EVET_OFF"), true);
	}
	
	if (day == SATURDAY && hour == 17 && quest::CQuestManager::instance().GetEventFlag("2017_moon_drop") == 0)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_moon_drop", 1);
		CGameEventsManager::instance().SetEventTime(EVENT_MOON_DROP, 3*60*60);
		SendNotice(LC_TEXT("2017_MOON_DROP_AUTO_EVET_ON"), true);
	}
	else if (day == SATURDAY && hour == 20 && quest::CQuestManager::instance().GetEventFlag("2017_moon_drop") == 1)
	{
		quest::CQuestManager::instance().SetEventFlag("2017_moon_drop", 0);
		SendNotice(LC_TEXT("2017_MOON_DROP_AUTO_EVET_OFF"), true);
	}
	
	if (day == SUNDAY && hour == 20 && quest::CQuestManager::instance().GetEventFlag("alignment_event") == 0)
	{
		quest::CQuestManager::instance().SetEventFlag("alignment_event", 1);
		CGameEventsManager::instance().SetEventTime(EVENT_ALIGNMENT, 2*60*60);
		SendNotice(LC_TEXT("2017_ALIGNMENT_EVENT_AUTO_EVET_ON"), true);
	}
	else if (day == SUNDAY && hour == 22 && quest::CQuestManager::instance().GetEventFlag("alignment_event") == 1)
	{
		quest::CQuestManager::instance().SetEventFlag("alignment_event", 1);
		SendNotice(LC_TEXT("2017_ALIGNMENT_EVENT_AUTO_EVET_OFF"), true);
	}
}

bool CEventsManager::Initialize()
{
	if (running_event != nullptr)
	{
		event_cancel(&running_event);
		running_event = nullptr;
	}

	EventsManagerInfoData* info = AllocEventInfo<EventsManagerInfoData>();
	info->pEvents = this;

	running_event = event_create(automatic_event_timer, info, PASSES_PER_SEC(30));
	return true;
}

void CEventsManager::Destroy()
{
	if (running_event != nullptr)
	{
		event_cancel(&running_event);
		running_event = nullptr;
	}
}