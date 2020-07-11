#include "stdafx.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "constants.h"
#include "questmanager.h"
#include "db.h"
#include "game_events.h"
#include "auto_query.h"

bool CAutoQueryManager::Initialize()
{
	return true;
}

void CAutoQueryManager::Destroy()
{
}

void CAutoQueryManager::UpdateQuestFlags()
{
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='ARMOR' and szState='CHECKER';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='MOUNT' and szState='CHECKER';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='MOUNTITEM' and szState='CHECKER';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='HORSE' and szState='CHECKER';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='ShopSearch' and szState='LastShopSearchSecAttr';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='gift' and szState='time';");
	DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='miningevent' and szState='mining_event_time';");
}

void CAutoQueryManager::UpdateItems()
{
	DBManager::instance().DirectQuery("DELETE FROM player.item WHERE owner_id=0;");
}

void CAutoQueryManager::OldProcess()
{
	if (CGameEventsManager::instance().IsActivateEvent(EVENT_BOSS_HUNTERS) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='attendance_event' and szState='red_dragon_hit_points';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='attendance_event' and szState='jotun_thrym_hit_points';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='attendance_event' and szState='razador_hit_points';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='attendance_event' and szState='nemere_hit_points';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='attendance_event' and szState='beran_hit_points';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_RAMADAN) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='event_ramadan' and szState='first_notice';");
		if (quest::CQuestManager::instance().GetEventFlag("ramadan_teslim_zamani") == 0)
			DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='event_ramadan' and szState='fig_exchange';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='event_ramadan' and szState='duration';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_FOOTBALL_DROP) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='gold_football' and szState='first_seen';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_HARVEST_FESTIVAL) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='harvest_festival_count';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='harvest_festival_mob_complete';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='cake_count';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='rice_count';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='mob_kill1_count';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='mob_kill2_count';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='mob2_info';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='harvest_festival' and szState='mob1_info';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_MINING) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='miningevent' and szState='currentMiningPoint';");
		DBManager::instance().DirectQuery("TRUNCATE log.log_mining_event");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_XMAS) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='new_christmas_2012_nog' and szState='last_nog_give_time';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='new_christmas_2012_sock' and szState='last_reindeer_give_time';");
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='new_christmas_2012_sock' and szState='last_sock_cooldown_time';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_OLYMPIC_BADGE) == false)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.quest WHERE szName='olympic_badge' and szState='medal_done';");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_CATCH_KING) == false)
	{
		DBManager::instance().DirectQuery("TRUNCATE log.log_catck_king_event");
	}
	else if (CGameEventsManager::instance().IsActivateEvent(EVENT_OKEY_CARD) == false)
	{
		DBManager::instance().DirectQuery("TRUNCATE log.log_okay_event");
	}
}