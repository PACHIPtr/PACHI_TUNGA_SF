#include "stdafx.h"
#ifdef __FreeBSD__
#include <md5.h>
#else
#include "../../libthecore/include/xmd5.h"
#endif
#include "utils.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "char.h"
#include "item.h"
#include "arena.h"
#include "buffer_manager.h"
#include "unique_item.h"
#include "log.h"
#include "../../common/VnumHelper.h"
#include "exchange.h"
#include "db.h"
#include "shop.h"
#include "shop_manager.h"
#include <string>
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_EVENT_SYSTEM
#include "event_manager.h"
#endif
#include "regen.h"
#include "shopEx.h"

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
#include "MountSystem.h"
#endif

#ifdef ENABLE_BOSS_MANAGER_SYSTEM
#include "boss_manager.h"
#endif

#include "features.h"

extern int g_server_id;

extern int g_nPortalLimitTime;

extern bool CAN_OPEN_SAFEBOX(int map_index);

ACMD(do_user_horse_ride)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHorseRiding() == false)
	{
		// ¸»ÀÌ ¾Æ´Ñ ´Ù¸¥Å»°ÍÀ» Å¸°íÀÖ´Ù.
		if (ch->GetMountVnum())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Å»°ÍÀ» ÀÌ¿ëÁßÀÔ´Ï´Ù."));
			return;
		}

		if (ch->GetQuestFlag("HORSE.CHECKER") && get_global_time() < ch->GetQuestFlag("HORSE.CHECKER"))
		{
			return;
		}//hizli at in bin yapma engel
		if (ch->GetHorse() == nullptr)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
			return;
		}

		ch->SetQuestFlag("HORSE.CHECKER", get_global_time() + 3);
		ch->StartRiding();
	}
	else
	{
		ch->StopRiding();
	}
}

ACMD(do_user_horse_back)
{
	if (ch->GetHorse() != nullptr)
	{
		ch->HorseSummon(false);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» µ¹·Áº¸³Â½À´Ï´Ù."));
	}
	else if (ch->IsHorseRiding() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡¼­ ¸ÕÀú ³»·Á¾ß ÇÕ´Ï´Ù."));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
	}
}

ACMD(do_user_horse_feed)
{
	// °³ÀÎ»óÁ¡À» ¿¬ »óÅÂ¿¡¼­´Â ¸» ¸ÔÀÌ¸¦ ÁÙ ¼ö ¾ø´Ù.
	if (ch->GetMyShop())
		return;

	if (ch->GetHorse() == nullptr)
	{
		if (ch->IsHorseRiding() == false)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» Åº »óÅÂ¿¡¼­´Â ¸ÔÀÌ¸¦ ÁÙ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	DWORD dwFood = ch->GetHorseGrade() + 50054 - 1;

	if (ch->CountSpecifyItem(dwFood) > 0)
	{
		ch->RemoveSpecifyItem(dwFood, 1);
		ch->FeedHorse();
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡°Ô %s%s ÁÖ¾ú½À´Ï´Ù."), ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName, "");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¾ÆÀÌÅÛÀÌ ÇÊ¿äÇÕ´Ï´Ù"), ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName);
	}
}

#define MAX_REASON_LEN		128

EVENTINFO(TimedEventInfo)
{
	DynamicCharacterPtr ch;
	int		subcmd;
	int         	left_second;
	char		szReason[MAX_REASON_LEN];

	TimedEventInfo()
		: ch()
		, subcmd(0)
		, left_second(0)
	{
		::memset(szReason, 0, MAX_REASON_LEN);
	}
};

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

EVENTINFO(shutdown_event_data)
{
	int seconds;

	shutdown_event_data()
		: seconds(0)
	{
	}
};

EVENTFUNC(shutdown_event)
{
	shutdown_event_data* info = dynamic_cast<shutdown_event_data*>(event->info);

	if (info == nullptr)
	{
		sys_err("shutdown_event> <Factor> nullptr pointer");
		return 0;
	}

	int* pSec = &(info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "shutdown_event sec %d", *pSec);

		if (-- * pSec == -10)
		{
			const DESC_MANAGER::DESC_SET& c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET& c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--* pSec;
		return passes_per_sec;
	}
	else
	{
		char buf[64];
		snprintf(buf, sizeof(buf), LC_TEXT("¼Ë´Ù¿îÀÌ %dÃÊ ³²¾Ò½À´Ï´Ù."), *pSec);
		SendNotice(buf);

		--* pSec;
		return passes_per_sec;
	}
}

void Shutdown(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

	char buf[64];
	snprintf(buf, sizeof(buf), LC_TEXT("%dÃÊ ÈÄ °ÔÀÓÀÌ ¼Ë´Ù¿î µË´Ï´Ù."), iSec);

	SendNotice(buf);

	shutdown_event_data* info = AllocEventInfo<shutdown_event_data>();
	info->seconds = iSec;

	event_create(shutdown_event, info, 1);
}

ACMD(do_shutdown)
{
	if (nullptr == ch)
	{
		sys_err("Accept shutdown command from %s.", ch->GetName());
	}
	TPacketGGShutdown p;
	p.bHeader = HEADER_GG_SHUTDOWN;
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

	Shutdown(10);
}

EVENTFUNC(timed_event)
{
	TimedEventInfo* info = dynamic_cast<TimedEventInfo*>(event->info);

	if (info == nullptr)
	{
		sys_err("timed_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == nullptr) { // <Factor>
		return 0;
	}
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEvent = nullptr;

		switch (info->subcmd)
		{
		case SCMD_LOGOUT:
		case SCMD_QUIT:
		case SCMD_PHASE_SELECT:
		{
			TPacketNeedLoginLogInfo acc_info;
			acc_info.dwPlayerID = ch->GetDesc()->GetAccountTable().id;

			db_clientdesc->DBPacket(HEADER_GD_VALID_LOGOUT, 0, &acc_info, sizeof(acc_info));

			LogManager::instance().DetailLoginLog(false, ch);
		}
		break;
		}

		switch (info->subcmd)
		{
		case SCMD_LOGOUT:
			if (d)
				d->SetPhase(PHASE_CLOSE);
			break;

		case SCMD_QUIT:
			ch->ChatPacket(CHAT_TYPE_COMMAND, "quit");
			break;

		case SCMD_PHASE_SELECT:
		{
			ch->Disconnect("timed_event - SCMD_PHASE_SELECT");

			if (d)
			{
				d->SetPhase(PHASE_SELECT);
			}
		}
		break;
		}

		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÃÊ ³²¾Ò½À´Ï´Ù."), info->left_second);
		--info->left_second;
	}

	return PASSES_PER_SEC(1);
}

ACMD(do_cmd)
{
	/* RECALL_DELAY
	   if (ch->m_pkRecallEvent != nullptr)
	   {
	   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
	   event_cancel(&ch->m_pkRecallEvent);
	   return;
	   }
	// END_OF_RECALL_DELAY */

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}

	switch (subcmd)
	{
	case SCMD_LOGOUT:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·Î±×ÀÎ È­¸éÀ¸·Î µ¹¾Æ °©´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
		break;

	case SCMD_QUIT:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°ÔÀÓÀ» Á¾·á ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
		break;

	case SCMD_PHASE_SELECT:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ä³¸¯ÅÍ¸¦ ÀüÈ¯ ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
		break;
	}

	int nExitLimitTime = 10;

	if (ch->IsHack(false, true, nExitLimitTime) && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		return;
	}

	switch (subcmd)
	{
	case SCMD_LOGOUT:
	case SCMD_QUIT:
	case SCMD_PHASE_SELECT:
	{
		TimedEventInfo* info = AllocEventInfo<TimedEventInfo>();

		{
			if (ch->IsPosition(POS_FIGHTING))
				info->left_second = 10;
			else
				info->left_second = 3;
		}

		info->ch = ch;
		info->subcmd = subcmd;
		strlcpy(info->szReason, argument, sizeof(info->szReason));

		ch->m_pkTimedEvent = event_create(timed_event, info, 1);
	}
	break;
	}
}

ACMD(do_mount)
{
	/*
	   char			arg1[256];
	   struct action_mount_param	param;

	// ÀÌ¹Ì Å¸°í ÀÖÀ¸¸é
	if (ch->GetMountingChr())
	{
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
	return;

	param.x		= atoi(arg1);
	param.y		= atoi(arg2);
	param.vid	= ch->GetMountingChr()->GetVID();
	param.is_unmount = true;

	float distance = DISTANCE_SQRT(param.x - (DWORD) ch->GetX(), param.y - (DWORD) ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á» ´õ °¡±îÀÌ °¡¼­ ³»¸®¼¼¿ä."));
	return;
	}

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	return;
	}

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	return;

	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(atoi(arg1));

	if (!tch->IsNPC() || !tch->IsMountable())
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å±â¿¡´Â Å» ¼ö ¾ø¾î¿ä."));
	return;
	}

	float distance = DISTANCE_SQRT(tch->GetX() - ch->GetX(), tch->GetY() - ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á» ´õ °¡±îÀÌ °¡¼­ Å¸¼¼¿ä."));
	return;
	}

	param.vid		= tch->GetVID();
	param.is_unmount	= false;

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	 */
}

ACMD(do_fishing)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	ch->SetRotation(atof(arg1));
	ch->fishing();
}

ACMD(do_console)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");
}

ACMD(do_restart)
{
	if (false == ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
		ch->StartRecoveryEvent();
		return;
	}

	if (nullptr == ch->m_pkDeadEvent)
		return;

	int iTimeToDead = (event_time(ch->m_pkDeadEvent) / passes_per_sec);

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
	{
		CCombatZoneManager::Instance().OnRestart(ch, subcmd);
		return;
	}
#endif

	if (subcmd != SCMD_RESTART_TOWN && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		if (!test_server)
		{
			if (iTimeToDead > 170)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - 170);
				return;
			}
		}
	}

	//PREVENT_HACK
	//DESC : Ã¢°í, ±³È¯ Ã¢ ÈÄ Æ÷Å»À» »ç¿ëÇÏ´Â ¹ö±×¿¡ ÀÌ¿ëµÉ¼ö ÀÖ¾î¼­
	//		ÄðÅ¸ÀÓÀ» Ãß°¡
	if (subcmd == SCMD_RESTART_TOWN)
	{
		if (ch->IsHack())
		{
			//±æµå¸Ê, ¼ºÁö¸Ê¿¡¼­´Â Ã¼Å© ÇÏÁö ¾Ê´Â´Ù.
			if ((!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - (180 - g_nPortalLimitTime));
				return;
			}
		}

		if (iTimeToDead > 173)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ ¸¶À»¿¡¼­ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%d ÃÊ ³²À½)"), iTimeToDead - 173);
			return;
		}
	}
	//END_PREVENT_HACK
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (subcmd == SCMD_RESTART_HERE)
	{
		if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
		{
			sys_log(0, "do_restart: restart here zodiac");
			BYTE Adet = ch->GetDeadCount() >= 1 ? ch->GetDeadCount()*2 : 1;
			ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenReviveDialog %u %u", (DWORD)ch->GetVID(), Adet);
			return;
		}
	}
#endif

	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");

	ch->GetDesc()->SetPhase(PHASE_GAME);
	ch->SetPosition(POS_STANDING);
	ch->StartRecoveryEvent();

	if (ch->GetDungeon())
		ch->GetDungeon()->UseRevive(ch);

	if (ch->GetWarMap() && !ch->IsObserverMode())
	{
		CWarMap* pMap = ch->GetWarMap();
		DWORD dwGuildOpponent = pMap ? pMap->GetGuildOpponent(ch) : 0;

		if (dwGuildOpponent)
		{
			switch (subcmd)
			{
			case SCMD_RESTART_TOWN:
				sys_log(0, "do_restart: restart town");
				PIXEL_POSITION pos;

				if (CWarMapManager::instance().GetStartPosition(ch->GetMapIndex(), ch->GetGuild()->GetID() < dwGuildOpponent ? 0 : 1, pos))
					ch->Show(ch->GetMapIndex(), pos.x, pos.y);
				else
					ch->ExitToSavedLocation();

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(5);
				ch->CheckPotions();
				ch->CheckTeleportItems();
				ch->CheckSoul();
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
				break;

			case SCMD_RESTART_HERE:
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
				if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
				{
					sys_log(0, "do_restart: restart here zodiac");
					BYTE Adet = ch->GetDeadCount() >= 1 ? ch->GetDeadCount()*2 : 1;
					ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenReviveDialog %u %u", (DWORD)ch->GetVID(), Adet);
					return;
				}
#endif
				sys_log(0, "do_restart: restart here");
				ch->RestartAtSamePos();
				//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(5);
				ch->CheckPotions();
				ch->CheckTeleportItems();
				ch->CheckSoul();
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
				break;
			}

			return;
		}
	}

	switch (subcmd)
	{
	case SCMD_RESTART_TOWN:
		sys_log(0, "do_restart: restart town");
		PIXEL_POSITION pos;

		if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
			ch->WarpSet(pos.x, pos.y);
		else
			ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

		ch->PointChange(POINT_HP, 50 - ch->GetHP());
		ch->DeathPenalty(1);
		break;

	case SCMD_RESTART_HERE:
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
		{
			sys_log(0, "do_restart: restart here zodiac");
			ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacRestart");
			return;
		}
#endif
		sys_log(0, "do_restart: restart here");
		ch->RestartAtSamePos();
		//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
		ch->PointChange(POINT_HP, 50 - ch->GetHP());
		ch->DeathPenalty(0);
		ch->ReviveInvisible(5);
		ch->CheckPotions();
		ch->CheckTeleportItems();
		ch->CheckSoul();
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		ch->CheckMount();
#endif
		break;
	}
}

#define MAX_STAT g_iStatusPointSetMaxValue

ACMD(do_stat_reset)
{
	ch->PointChange(POINT_STAT_RESET_COUNT, 12 - ch->GetPoint(POINT_STAT_RESET_COUNT));
}

ACMD(do_stat_minus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT_RESET_COUNT) <= 0)
		return;

	if (!strcmp(arg1, "st"))
	{
		if (ch->GetRealPoint(POINT_ST) <= JobInitialPoints[ch->GetJob()].st)
			return;

		ch->SetRealPoint(POINT_ST, ch->GetRealPoint(POINT_ST) - 1);
		ch->SetPoint(POINT_ST, ch->GetPoint(POINT_ST) - 1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
		ch->ComputePointsWithoutUpdatePacket();
#else
		ch->ComputePoints();
#endif
		ch->PointChange(POINT_ST, 0);
	}
	else if (!strcmp(arg1, "dx"))
	{
		if (ch->GetRealPoint(POINT_DX) <= JobInitialPoints[ch->GetJob()].dx)
			return;

		ch->SetRealPoint(POINT_DX, ch->GetRealPoint(POINT_DX) - 1);
		ch->SetPoint(POINT_DX, ch->GetPoint(POINT_DX) - 1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
		ch->ComputePointsWithoutUpdatePacket();
#else
		ch->ComputePoints();
#endif
		ch->PointChange(POINT_DX, 0);
	}
	else if (!strcmp(arg1, "ht"))
	{
		if (ch->GetRealPoint(POINT_HT) <= JobInitialPoints[ch->GetJob()].ht)
			return;

		ch->SetRealPoint(POINT_HT, ch->GetRealPoint(POINT_HT) - 1);
		ch->SetPoint(POINT_HT, ch->GetPoint(POINT_HT) - 1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
		ch->ComputePointsWithoutUpdatePacket();
#else
		ch->ComputePoints();
#endif
		ch->PointChange(POINT_HT, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (!strcmp(arg1, "iq"))
	{
		if (ch->GetRealPoint(POINT_IQ) <= JobInitialPoints[ch->GetJob()].iq)
			return;

		ch->SetRealPoint(POINT_IQ, ch->GetRealPoint(POINT_IQ) - 1);
		ch->SetPoint(POINT_IQ, ch->GetPoint(POINT_IQ) - 1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
		ch->ComputePointsWithoutUpdatePacket();
#else
		ch->ComputePoints();
#endif
		ch->PointChange(POINT_IQ, 0);
		ch->PointChange(POINT_MAX_SP, 0);
	}
	else
		return;

	ch->PointChange(POINT_STAT, +1);
	ch->PointChange(POINT_STAT_RESET_COUNT, -1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
	ch->ComputePointsWithoutUpdatePacket();
#else
	ch->ComputePoints();
#endif
}

ACMD(do_stat)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + 1);
	ch->SetPoint(idx, ch->GetPoint(idx) + 1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
	ch->ComputePointsWithoutUpdatePacket();
#else
	ch->ComputePoints();
#endif
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
	{
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_HT)
	{
		ch->PointChange(POINT_MAX_SP, 0);
	}

	ch->PointChange(POINT_STAT, -1);
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
	ch->ComputePointsWithoutUpdatePacket();
#else
	ch->ComputePoints();
#endif
}

ACMD(do_pvp)
{
	if (ch->GetArena() != nullptr || CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
		return;
#endif

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(vid);

	if (!pkVictim)
		return;

	if (pkVictim->IsNPC())
		return;

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	if (MessengerManager::instance().IsBlocked_Target(ch->GetName(), pkVictim->GetName()))
	{
		//ben bloklad? hac?		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s blokkk"), pkVictim->GetName());
		return;
	}
	if (MessengerManager::instance().IsBlocked_Me(ch->GetName(), pkVictim->GetName()))
	{
		//o bloklad?hac?		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s blokkk_me"), pkVictim->GetName());
		return;
	}
#endif

	if (pkVictim->GetArena() != nullptr)
	{
		pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ´ë·ÃÁßÀÔ´Ï´Ù."));
		return;
	}

	if (quest::CQuestManager::instance().GetEventFlag("ox_duello_engel") == 1)
	{
		if (pkVictim->GetMapIndex() == 113 || ch->GetMapIndex() == 113)
		{
			pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("oxduelloengelxd"));
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("oxduelloengelxd"));
			return;
		}
	}

	CPVPManager::instance().Insert(ch, pkVictim);
}

ACMD(do_guildskillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetGuild())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿¡ ¼ÓÇØÀÖÁö ¾Ê½À´Ï´Ù."));
		return;
	}

	CGuild* g = ch->GetGuild();
	TGuildMember* gm = g->GetMember(ch->GetPlayerID());
	if (gm->grade == GUILD_LEADER_GRADE)
	{
		DWORD vnum = 0;
		str_to_number(vnum, arg1);
		g->SkillLevelUp(vnum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå ½ºÅ³ ·¹º§À» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
	}
}

ACMD(do_skillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vnum = 0;
	str_to_number(vnum, arg1);

	if (true == ch->CanUseSkill(vnum))
	{
		ch->SkillLevelUp(vnum);
	}
	else
	{
		switch (vnum)
		{
		case SKILL_HORSE_WILDATTACK:
		case SKILL_HORSE_CHARGE:
		case SKILL_HORSE_ESCAPE:
		case SKILL_HORSE_WILDATTACK_RANGE:
#ifdef ENABLE_7AND8TH_SKILLS_SYSTEM
		case SKILL_ANTI_PALBANG:
		case SKILL_ANTI_AMSEOP:
		case SKILL_ANTI_SWAERYUNG:
		case SKILL_ANTI_YONGBI:
		case SKILL_ANTI_GIGONGCHAM:
		case SKILL_ANTI_HWAJO:
		case SKILL_ANTI_MARYUNG:
		case SKILL_ANTI_BYEURAK:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case SKILL_ANTI_SALPOONG:
#endif
		case SKILL_HELP_PALBANG:
		case SKILL_HELP_AMSEOP:
		case SKILL_HELP_SWAERYUNG:
		case SKILL_HELP_YONGBI:
		case SKILL_HELP_GIGONGCHAM:
		case SKILL_HELP_HWAJO:
		case SKILL_HELP_MARYUNG:
		case SKILL_HELP_BYEURAK:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case SKILL_HELP_SALPOONG:
#endif
#endif
			ch->SkillLevelUp(vnum);
			break;
		}
	}
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_close)
{
	ch->CloseSafebox();
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	ch->ReqSafeboxLoad(arg1);
}

ACMD(do_safebox_change_password)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	if (!*arg2 || strlen(arg2) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	if (LC_IsBrazil() == true)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (arg2[i] == '\0')
				break;

			if (isalpha(arg2[i]) == false)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ºñ¹Ð¹øÈ£´Â ¿µ¹®ÀÚ¸¸ °¡´ÉÇÕ´Ï´Ù."));
				return;
			}
		}
	}

	TSafeboxChangePasswordPacket p;

	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szOldPassword, arg1, sizeof(p.szOldPassword));
	strlcpy(p.szNewPassword, arg2, sizeof(p.szNewPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_PASSWORD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	int iPulse = thecore_pulse();

	if (ch->GetMall())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í°¡ ÀÌ¹Ì ¿­·ÁÀÖ½À´Ï´Ù."));
		return;
	}

	if (iPulse - ch->GetMallLoadTime() < passes_per_sec * 10) // 10ÃÊ¿¡ ÇÑ¹ø¸¸ ¿äÃ» °¡´É
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í¸¦ ´ÝÀºÁö 10ÃÊ ¾È¿¡´Â ¿­ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	ch->SetMallLoadTime(iPulse);

	TSafeboxLoadPacket p;
	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, ch->GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, arg1, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_MALL_LOAD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_close)
{
	if (ch->GetMall())
	{
		ch->SetMallLoadTime(thecore_pulse());
		ch->CloseMall();
		ch->Save();
	}
}

ACMD(do_ungroup)
{
	if (!ch->GetParty())
		return;

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÀü ¾È¿¡¼­´Â ÆÄÆ¼¿¡¼­ ³ª°¥ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (ch->GetZodiac())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zodyak tapinaginda grubu bozamazsin."));
		return;
	}

	if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zodyak tapinaginda grubu bozamazsin."));
		return;
	}
#endif

	LPPARTY pParty = ch->GetParty();

	if (pParty->GetMemberCount() == 2)
	{
		// party disband
		CPartyManager::instance().DeleteParty(pParty);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
		//pParty->SendPartyRemoveOneToAll(ch);
		pParty->Quit(ch->GetPlayerID());
		//pParty->SendPartyRemoveAllToOne(ch);
	}
}

#ifdef ENABLE_GIFTBOX_SYSTEM
#include "db.h"
#include <string>
#include <boost/algorithm/string.hpp>

ACMD(do_gift_show)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_show");
}
bool GetGift(LPCHARACTER ch, DWORD id, bool all = false)
{
	char szSockets[1024] = { '\0' };
	char* tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i < ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char* tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i < ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}
	char query[8192];
	if (!all)
		snprintf(query, sizeof(query), "SELECT id,vnum,count,transmutation,%s,%s from player_gift where id='%d' and owner_id=%d and status='WAIT'", szSockets, szAttrs, id, ch->GetPlayerID());
	else
		snprintf(query, sizeof(query), "SELECT id,vnum,count,transmutation,%s,%s from player_gift where owner_id=%d and status='WAIT'", szSockets, szAttrs, ch->GetPlayerID());
	SQLMsg * pkMsg(DBManager::instance().DirectQuery(query));
	SQLResult * pRes = pkMsg->Get();
	if (pRes->uiNumRows > 0)
	{
		ch->SetQuestFlag("gift.time", get_global_time() + (1 * pRes->uiNumRows) + 2);
		MYSQL_ROW row;
		bool force = false;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
		{
			DWORD vnum, socket[ITEM_SOCKET_MAX_NUM], attr[ITEM_ATTRIBUTE_MAX_NUM][2];
			int col = 0;
			long long count = 0;
			DWORD transmutation = 0;
			str_to_number(id, row[col++]);
			str_to_number(vnum, row[col++]);
			str_to_number(count, row[col++]);
			str_to_number(transmutation, row[col++]);
			if (vnum == 1)
			{
				if ((ch->GetGold() + count) <= GOLD_MAX - 1)
				{
					ch->PointChange(POINT_GOLD, count, false);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yang'in full seviyede!"));
					continue;
				}
			}
			else if (vnum == 50027)
			{
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
					str_to_number(socket[s], row[col++]);

				if ((ch->GetCheque() + socket[0]) <= CHEQUE_MAX - 1)
				{
					ch->PointChange(POINT_CHEQUE, socket[0], false);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Won'un full seviyede!"));
					continue;
				}
			}
			else {
				if (force)
					continue;
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
					str_to_number(socket[s], row[col++]);

				for (int a = 0; a < ITEM_ATTRIBUTE_MAX_NUM; a++)
				{
					str_to_number(attr[a][0], row[col++]);
					str_to_number(attr[a][1], row[col++]);
				}
				LPITEM item = ITEM_MANAGER::instance().CreateItem(vnum, count, 0, true);
				if (item)
				{
					item->SetTransmutation(transmutation);
					for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
						item->SetSocket(s, socket[s], false);
					item->ClearAttribute();
					for (int a = 0; a < ITEM_ATTRIBUTE_MAX_NUM; a++)
						item->SetForceAttribute(a, attr[a][0], attr[a][1]);
					if (item->GetVnum() == 55002)
						std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%d WHERE id = %d", item->GetID(), id));

					if (item->IsDragonSoul())
					{
						int iEmptyPos = ch->GetEmptyDragonSoulInventory(item);

						if (iEmptyPos != -1)
						{
							item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
						}
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
					else if (item->IsSkillBook())
					{
						int iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(SKILL_BOOK_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
					else if (item->IsUpgradeItem())
					{
						int iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
					else if (item->IsStone())
					{
						int iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(STONE_ITEMS_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
					else if (item->IsChest())
					{
						int iEmptyPos = ch->GetEmptyChestInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
					else if (item->IsAttr())
					{
						int iEmptyPos = ch->GetEmptyAttrInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
					else if (item->IsFlower())
					{
						int iEmptyPos = ch->GetEmptyFlowerInventory(item->GetSize());
						if (iEmptyPos != -1)
							item->AddToCharacter(ch, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyPos));
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
#endif
					else
					{
						int iEmptyPos = ch->GetEmptyInventory(item->GetSize());

						if (iEmptyPos != -1)
						{
							item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
						}
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "<Gift> %s #4", LC_TEXT("UNKNOW_ERROR"));
					force = true;
					continue;
				}
			}

			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM player_gift WHERE id = %d", id));
		}
		if (force)
			return true;
		if (all)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GIFT_ADD_ALL_SUCCESS"));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GIFT_ADD_SUCCESS"));
		ch->SetQuestFlag("gift.time", get_global_time() + 2);
		return true;
	}
	return false;
}
ACMD(do_gift_get)
{
	if (ch->GetQuestFlag("gift.time") > get_global_time())
		return;
	if (ch->IsCubeOpen() == true)
	{
		Cube_close(ch);
	}
	if (ch->GetExchange())
		ch->GetExchange()->Cancel();
	ch->CloseSafebox();
	CShopManager::instance().StopShopping(ch);
	ch->CloseMyShop();
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	bool full = !isdigit(*arg1);
	DWORD id;
	str_to_number(id, arg1);
	if (GetGift(ch, id, full))
	{
		ch->RefreshGift();
		ch->LoadGiftPage(ch->GetLastGiftPage());
	}
}
ACMD(do_gift_refresh)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
	{
		if (ch->GetGiftPages() > 0)
			ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", ch->GetGiftPages());
	}
	else {
		int page;
		str_to_number(page, arg1);
		ch->LoadGiftPage(page);
	}
}
#endif

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void DeleteShop(DWORD id)
{
	CharacterVectorInteractor i;
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30000 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30001, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30001 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30002, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30002 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30003, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30003 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30004, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30004 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30005, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30005 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30006, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30006 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30007, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30007 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30008, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30008 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}
		}
	}
	TPacketShopClose packet;
	packet.shop_id = id;
	packet.pid = 0;
	db_clientdesc->DBPacket(HEADER_GD_SHOP_CLOSE, 0, &packet, sizeof(packet));
}

ACMD(do_close_shop)
{
#ifdef ACCOUNT_SHIELD
	if (ch->IsBlockAccount())
		return;
#endif
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
	{
		ch->CloseMyShop();
	}
	else {
		str_to_number(id, arg1);
		char pid[4096];
		sprintf(pid, "and player_id=%d", ch->GetPlayerID());
		if (id > 0)
		{
			std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT player_id,channel from player_shop WHERE id = %d %s", id, (ch->GetGMLevel() >= SHOP_GM_PRIVILEGES ? "" : pid)));
			SQLResult * pRes = pkMsg->Get();
			if (pRes->uiNumRows > 0)
				DeleteShop(id);
		}
	}
}

ACMD(do_shop_refresh)
{
	ch->SendShops();
}

ACMD(do_shop_yang)
{
	if (!features::IsFeatureEnabled(features::SHOP_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MONEY_SYSTEM_DISABLED"));
		return;		
	}
	
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	if (*arg1)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT gold from player_shop WHERE id = %d and player_id=%d", id,ch->GetPlayerID()));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				long long gold;
				str_to_number(gold, row[0]);
				if (gold >0)
				{
					long long nTotalMoney = ch->GetGold() + gold;

					if (GOLD_MAX <= nTotalMoney)
					{
						sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20ld l R?AE°ú?z?»ó?R?z­Lö°^ lr???"));
						return;
					}

					ch->PointChange(POINT_GOLD,gold, false);
	
					TPrivShop s = ch->GetPrivShopTable(id);
					s.gold = s.gold - gold;
					s.rest_count = s.item_count - 1;
					ch->UpdatePrivShopTable(s.shop_id, s);
					ch->SendShops();
					DBManager::instance().DirectQuery("UPDATE player_shop SET gold=gold - %lld WHERE id = %d", gold, id);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_YANG_ADD"));
				}
			}
		}
	}
}

ACMD(do_shop_won)
{
	if (!features::IsFeatureEnabled(features::SHOP_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MONEY_SYSTEM_DISABLED"));
		return;		
	}
	
	if (ch->IsObserverMode() || ch->GetExchange())
		return;

	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	if (*arg1)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT won from player_shop WHERE id = %d and player_id=%d", id,ch->GetPlayerID()));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				long long won;
				str_to_number(won, row[0]);
				if (won >0)
				{
					long long nTotalMoney = ch->GetCheque() + won;

					if (CHEQUE_MAX <= nTotalMoney)
					{
						sys_err("[OVERFLOW_CHEUE] Overflow (CHEQUE_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20ld l R?AE°ú?z?»ó?R?z­Lö°^ lr???"));
						return;
					}

					ch->PointChange(POINT_CHEQUE,won, false);
	
					TPrivShop s = ch->GetPrivShopTable(id);
					s.won = s.won - won;
					s.rest_count = s.item_count - 1;
					ch->UpdatePrivShopTable(s.shop_id, s);
					ch->SendShops();
					DBManager::instance().DirectQuery("UPDATE player_shop SET won=won - %lld WHERE id = %d", won, id);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_YANG_ADD"));
				}
			}
		}
	}
}

ACMD(do_shop_delete)
{
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage:");
		ch->ChatPacket(CHAT_TYPE_INFO, "/delete_shop <type> <arg> ");
		ch->ChatPacket(CHAT_TYPE_INFO, "Types:");
		ch->ChatPacket(CHAT_TYPE_INFO, "		shopid - Delete shop using ID");
		ch->ChatPacket(CHAT_TYPE_INFO, "		player - Delete all player shops by player name");
		ch->ChatPacket(CHAT_TYPE_INFO, "Example:");
		ch->ChatPacket(CHAT_TYPE_INFO, "		/delete_shop player Best4ever");
		ch->ChatPacket(CHAT_TYPE_INFO, "		/delete_shop shopid 1");
		return;
	}
	if (!strcmp(arg1, "player"))
	{
		std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop WHERE player_id=(select id from player where name='%s')", arg2));
		SQLResult* pRes = pkMsg->Get();
		if (pRes->uiNumRows > 0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
			{
				DWORD id;
				str_to_number(id, row[0]);
				DeleteShop(id);
			}
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Player %s does have any shop"), arg2);
	}
	if (!strcmp(arg1, "shopid"))
	{
		std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop WHERE id='%s'", arg2));
		SQLResult* pRes = pkMsg->Get();
		if (pRes->uiNumRows > 0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
			{
				DWORD id;
				str_to_number(id, arg2);
				DeleteShop(id);
			}
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Shop %s does exists"), arg2);
	}
}

ACMD(do_shop_open)
{
	ch->UseSilkBotary();
}
#else
ACMD(do_close_shop)
{
	if (ch->IsObserverMode())
		return;
	if (ch->GetMyShop())
	{
		ch->CloseMyShop();
		return;
	}
}
#endif

ACMD(do_set_walk_mode)
{
	ch->SetNowWalking(true);
	ch->SetWalking(true);
}

ACMD(do_set_run_mode)
{
	ch->SetNowWalking(false);
	ch->SetWalking(false);
}

ACMD(do_war)
{
	//³» ±æµå Á¤º¸¸¦ ¾ò¾î¿À°í
	CGuild* g = ch->GetGuild();

	if (!g)
		return;

	//ÀüÀïÁßÀÎÁö Ã¼Å©ÇÑ¹ø!
	if (g->UnderAnyWar())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ´Ù¸¥ ÀüÀï¿¡ ÂüÀü Áß ÀÔ´Ï´Ù."));
		return;
	}

	//ÆÄ¶ó¸ÞÅÍ¸¦ µÎ¹è·Î ³ª´©°í
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
	char arg1[256], arg2[256], arg3[256], arg4[256];
	one_argument(three_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3)), arg4, sizeof(arg4));
#else
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
#endif
	int type = GUILD_WAR_TYPE_FIELD;

	if (!*arg1)
		return;

	if (*arg2)
	{
		str_to_number(type, arg2);

		if ((type >= GUILD_WAR_TYPE_MAX_NUM) || (type < 0))
			type = GUILD_WAR_TYPE_FIELD;

	}

	//±æµåÀÇ ¸¶½ºÅÍ ¾ÆÀÌµð¸¦ ¾ò¾î¿ÂµÚ
	DWORD gm_pid = g->GetMasterPID();

	//¸¶½ºÅÍÀÎÁö Ã¼Å©(±æÀüÀº ±æµåÀå¸¸ÀÌ °¡´É)
	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}

	//»ó´ë ±æµå¸¦ ¾ò¾î¿À°í
	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}

	//»ó´ë±æµå¿ÍÀÇ »óÅÂ Ã¼Å©
	switch (g->GetGuildWarState(opp_g->GetID()))
	{
	case GUILD_WAR_NONE:
	{
		if (opp_g->UnderAnyWar())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));
			return;
		}

		int iWarPrice = KOR_aGuildWarInfo[type].iWarPrice;

		if (g->GetGuildMoney() < iWarPrice)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return;
		}

		if (opp_g->GetGuildMoney() < iWarPrice)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return;
		}
	}
	break;

	case GUILD_WAR_SEND_DECLARE:
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ¼±ÀüÆ÷°í ÁßÀÎ ±æµåÀÔ´Ï´Ù."));
		return;
	}
	break;

	case GUILD_WAR_RECV_DECLARE:
	{
		if (opp_g->UnderAnyWar())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));

			g->RequestRefuseWar(opp_g->GetID()
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
				, 0, 0
#endif
			);
			return;
		}
	}
	break;

	case GUILD_WAR_RESERVE:
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀïÀÌ ¿¹¾àµÈ ±æµå ÀÔ´Ï´Ù."));
		return;
	}
	break;

	case GUILD_WAR_END:
		return;

	default:
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀï ÁßÀÎ ±æµåÀÔ´Ï´Ù."));

		g->RequestRefuseWar(opp_g->GetID()
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
			, 0, 0
#endif
		);
		return;
	}

	if (!g->CanStartWar(type))
	{
		// ±æµåÀüÀ» ÇÒ ¼ö ÀÖ´Â Á¶°ÇÀ» ¸¸Á·ÇÏÁö¾Ê´Â´Ù.
		if (g->GetLadderPoint() == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			sys_log(0, "GuildWar.StartError.NEED_LADDER_POINT");
		}
		else if (g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀüÀ» ÇÏ±â À§ÇØ¼± ÃÖ¼ÒÇÑ %d¸íÀÌ ÀÖ¾î¾ß ÇÕ´Ï´Ù."), GUILD_WAR_MIN_MEMBER_COUNT);
			sys_log(0, "GuildWar.StartError.NEED_MINIMUM_MEMBER[%d]", GUILD_WAR_MIN_MEMBER_COUNT);
		}
		else
		{
			sys_log(0, "GuildWar.StartError.UNKNOWN_ERROR");
		}
		return;
	}

	// ÇÊµåÀü Ã¼Å©¸¸ ÇÏ°í ¼¼¼¼ÇÑ Ã¼Å©´Â »ó´ë¹æÀÌ ½Â³«ÇÒ¶§ ÇÑ´Ù.
	if (!opp_g->CanStartWar(GUILD_WAR_TYPE_FIELD))
	{
		if (opp_g->GetLadderPoint() == 0)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		else if (opp_g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµå¿ø ¼ö°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	do
	{
		if (g->GetMasterCharacter() != nullptr)
			break;

		CCI * pCCI = P2P_MANAGER::instance().FindByPID(g->GetMasterPID());

		if (pCCI != nullptr)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID()
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
			, 0, 0
#endif
		);
		return;
	} while (false);

	do
	{
		if (opp_g->GetMasterCharacter() != nullptr)
			break;

		CCI * pCCI = P2P_MANAGER::instance().FindByPID(opp_g->GetMasterPID());

		if (pCCI != nullptr)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID()
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
			, 0, 0
#endif
		);
		return;
	} while (false);

#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
	int iMaxPlayer = 0;
	if (!*arg3) iMaxPlayer = 1000;
	else str_to_number(iMaxPlayer, arg3);
	if (iMaxPlayer < 3) iMaxPlayer = 1000;

	int iMaxScore = 0;
	if (!*arg4) iMaxScore = KOR_aGuildWarInfo[type].iEndScore;
	else str_to_number(iMaxScore, arg4);
	if (iMaxScore < 5) iMaxScore = KOR_aGuildWarInfo[type].iEndScore;

	g->RequestDeclareWar(opp_g->GetID(), type, iMaxPlayer, iMaxScore);
#else
	g->RequestDeclareWar(opp_g->GetID(), type);
#endif
}

ACMD(do_nowar)
{
	CGuild* g = ch->GetGuild();
	if (!g)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}

	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}

	g->RequestRefuseWar(opp_g->GetID()
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
		, 0, 0
#endif
	);
}

ACMD(do_detaillog)
{
	ch->DetailLog();
}

ACMD(do_monsterlog)
{
	ch->ToggleMonsterLog();
}

ACMD(do_pkmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE mode = 0;
	str_to_number(mode, arg1);

	if (mode == PK_MODE_PROTECT)
		return;

	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()))
		return;
#endif

	ch->SetPKMode(mode);
}

ACMD(do_messenger_auth)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	char answer = LOWER(*arg1);
#ifdef ENABLE_MESSENGER_AUTH_EXPLOIT_FIX
	bool bIsDenied = answer != 'y';
	bool bIsAdded = MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, bIsDenied); // DENY
	if (bIsAdded && bIsDenied)
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ë‹˜ìœ¼ë¡œ ë¶€í„° ì¹œêµ¬ ë“±ë¡ì„ ê±°ë¶€ ë‹¹í–ˆìŠµë‹ˆë‹¤."), ch->GetName());
	}
#else
	if (answer != 'y')
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀ¸·Î ºÎÅÍ Ä£±¸ µî·ÏÀ» °ÅºÎ ´çÇß½À´Ï´Ù."), ch->GetName());
	}

	MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, answer == 'y' ? false : true); // DENY
#endif
}

ACMD(do_setblockmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		int flag = 0;
		str_to_number(flag, arg1);
		ch->SetBlockMode(flag);
	}
}

ACMD(do_unmount)
{
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;

		if (!mountSystem && !mount)
			return;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(mount->GetTransmutation());
		if (pkItemTransmutation)
		{
			for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
			{
				BYTE bType = pkItemTransmutation->aApplies[i].bType;
				long value = pkItemTransmutation->aApplies[i].lValue;
				if (bType == APPLY_MOUNT)
					mobVnum = value;
			}
		}
		else
		{
			if (mount->FindApplyValue(APPLY_MOUNT) != 0)
				mobVnum = mount->FindApplyValue(APPLY_MOUNT);
		}
#else
		if (mount->FindApplyValue(APPLY_MOUNT) != 0)
			mobVnum = mount->FindApplyValue(APPLY_MOUNT);
#endif
		if (ch->GetMountVnum())
		{
			if (mountSystem->CountSummoned() == 0)
			{
				mountSystem->Unmount(mobVnum);
			}
		}
		return;
	}
#endif
	if (ch->UnEquipSpecialRideUniqueItem())
	{
		ch->RemoveAffect(AFFECT_MOUNT);
		ch->RemoveAffect(AFFECT_MOUNT_BONUS);
		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÎº¥Åä¸®°¡ ²Ë Â÷¼­ ³»¸± ¼ö ¾ø½À´Ï´Ù."));
	}
}

ACMD(do_observer_exit)
{
	if (ch->IsObserverMode())
	{
		if (ch->GetMapIndex() == 207) {
			ch->LogoutSpecialObserver();
			ch->GoHome();
			return;
		}
		if (ch->GetWarMap())
			ch->SetWarMap(nullptr);

		if (ch->GetArena() != nullptr || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != nullptr)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(nullptr);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else
		{
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

void CHARACTER::LoginSpecialObserver()
{
	bool isObserverMember = GetQuestFlag("warp.is_observer_member") > 0 ? true : false;
	if (isObserverMember) {
		if (IsObserverMode()) {
			sys_err("[%lu]%s already in observermode !", GetAID(), GetName());
			GoHome();
		}

		if (GetParty()) {
			LPPARTY pParty = GetParty();
			if (pParty->GetMemberCount() == 2)
			{
				CPartyManager::instance().DeleteParty(pParty);
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
				pParty->Quit(GetPlayerID());
			}
		}

		if (IsRiding())
			StopRiding();

		sys_log(0, "Observer login event: [%lu]%s", GetAID(), GetName());
		SetObserverMode(true);
		ChatPacket(CHAT_TYPE_INFO, "Izleyici olarak giris yapildi"); // you logon as observer
	}
}

void CHARACTER::LogoutSpecialObserver()
{
	bool isObserverMember = GetQuestFlag("warp.is_observer_member") > 0 ? true : false;
	if (isObserverMember) {
		sys_log(0, "Observer logout event: [%lu]%s", GetAID(), GetName());
		SetObserverMode(false);
		SetQuestFlag("warp.is_observer_member", 0);
	}
}

ACMD(do_party_request)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ÆÄÆ¼¿¡ ¼ÓÇØ ÀÖÀ¸¹Ç·Î °¡ÀÔ½ÅÃ»À» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		if (!ch->RequestToParty(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

ACMD(do_party_request_accept)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToParty(tch);
}

ACMD(do_party_request_deny)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToParty(tch);
}

// LUA_ADD_GOTO_INFO
struct GotoInfo
{
	std::string 	st_name;

	BYTE 	empire;
	int 	mapIndex;
	DWORD 	x, y;

	GotoInfo()
	{
		st_name = "";
		empire = 0;
		mapIndex = 0;

		x = 0;
		y = 0;
	}

	GotoInfo(const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void operator = (const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void __copy__(const GotoInfo& c_src)
	{
		st_name = c_src.st_name;
		empire = c_src.empire;
		mapIndex = c_src.mapIndex;

		x = c_src.x;
		y = c_src.y;
	}
};

extern void BroadcastNotice(const char* c_pszBuf, bool bBigFont);

static const char* FN_point_string(int apply_number)
{
	switch (apply_number)
	{
	case POINT_MAX_HP:
		return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d");

	case POINT_MAX_SP:
		return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d");

	case POINT_HT:
		return LC_TEXT("Ã¼·Â +%d");

	case POINT_IQ:
		return LC_TEXT("Áö´É +%d");

	case POINT_ST:
		return LC_TEXT("±Ù·Â +%d");

	case POINT_DX:
		return LC_TEXT("¹ÎÃ¸ +%d");

	case POINT_ATT_SPEED:
		return LC_TEXT("°ø°Ý¼Óµµ +%d");

	case POINT_MOV_SPEED:
		return LC_TEXT("ÀÌµ¿¼Óµµ %d");

	case POINT_CASTING_SPEED:
		return LC_TEXT("ÄðÅ¸ÀÓ -%d");

	case POINT_HP_REGEN:
		return LC_TEXT("»ý¸í·Â È¸º¹ +%d");

	case POINT_SP_REGEN:
		return LC_TEXT("Á¤½Å·Â È¸º¹ +%d");

	case POINT_POISON_PCT:
		return LC_TEXT("µ¶°ø°Ý %d");
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_BLEEDING_PCT:
		return LC_TEXT("Bleed pct %d%%");
#endif
	case POINT_STUN_PCT:
		return LC_TEXT("½ºÅÏ +%d");

	case POINT_SLOW_PCT:
		return LC_TEXT("½½·Î¿ì +%d");

	case POINT_CRITICAL_PCT:
		return LC_TEXT("%d%% È®·ü·Î Ä¡¸íÅ¸ °ø°Ý");

	case POINT_RESIST_CRITICAL:
		return LC_TEXT("»ó´ëÀÇ Ä¡¸íÅ¸ È®·ü %d%% °¨¼Ò");

	case POINT_PENETRATE_PCT:
		return LC_TEXT("%d%% È®·ü·Î °üÅë °ø°Ý");

	case POINT_RESIST_PENETRATE:
		return LC_TEXT("»ó´ëÀÇ °üÅë °ø°Ý È®·ü %d%% °¨¼Ò");

	case POINT_ATTBONUS_HUMAN:
		return LC_TEXT("ÀÎ°£·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");

	case POINT_ATTBONUS_ANIMAL:
		return LC_TEXT("µ¿¹°·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");

	case POINT_ATTBONUS_ORC:
		return LC_TEXT("¿õ±ÍÁ· Å¸°ÝÄ¡ +%d%%");

	case POINT_ATTBONUS_MILGYO:
		return LC_TEXT("¹Ð±³·ù Å¸°ÝÄ¡ +%d%%");

	case POINT_ATTBONUS_UNDEAD:
		return LC_TEXT("½ÃÃ¼·ù Å¸°ÝÄ¡ +%d%%");

	case POINT_ATTBONUS_DEVIL:
		return LC_TEXT("¾Ç¸¶·ù Å¸°ÝÄ¡ +%d%%");

	case POINT_STEAL_HP:
		return LC_TEXT("Å¸°ÝÄ¡ %d%% ¸¦ »ý¸í·ÂÀ¸·Î Èí¼ö");

	case POINT_STEAL_SP:
		return LC_TEXT("Å¸·ÂÄ¡ %d%% ¸¦ Á¤½Å·ÂÀ¸·Î Èí¼ö");

	case POINT_MANA_BURN_PCT:
		return LC_TEXT("%d%% È®·ü·Î Å¸°Ý½Ã »ó´ë Àü½Å·Â ¼Ò¸ð");

	case POINT_DAMAGE_SP_RECOVER:
		return LC_TEXT("%d%% È®·ü·Î ÇÇÇØ½Ã Á¤½Å·Â È¸º¹");

	case POINT_BLOCK:
		return LC_TEXT("¹°¸®Å¸°Ý½Ã ºí·° È®·ü %d%%");

	case POINT_DODGE:
		return LC_TEXT("È° °ø°Ý È¸ÇÇ È®·ü %d%%");

	case POINT_RESIST_SWORD:
		return LC_TEXT("ÇÑ¼Õ°Ë ¹æ¾î %d%%");

	case POINT_RESIST_TWOHAND:
		return LC_TEXT("¾ç¼Õ°Ë ¹æ¾î %d%%");

	case POINT_RESIST_DAGGER:
		return LC_TEXT("µÎ¼Õ°Ë ¹æ¾î %d%%");

	case POINT_RESIST_BELL:
		return LC_TEXT("¹æ¿ï ¹æ¾î %d%%");

	case POINT_RESIST_FAN:
		return LC_TEXT("ºÎÃ¤ ¹æ¾î %d%%");

	case POINT_RESIST_BOW:
		return LC_TEXT("È°°ø°Ý ÀúÇ× %d%%");

	case POINT_RESIST_CLAW:
		return LC_TEXT("Defense chance against claws +%d%%");

	case POINT_RESIST_FIRE:
		return LC_TEXT("È­¿° ÀúÇ× %d%%");

	case POINT_RESIST_ELEC:
		return LC_TEXT("Àü±â ÀúÇ× %d%%");

	case POINT_RESIST_MAGIC:
		return LC_TEXT("¸¶¹ý ÀúÇ× %d%%");

	case POINT_RESIST_WIND:
		return LC_TEXT("¹Ù¶÷ ÀúÇ× %d%%");

	case POINT_RESIST_ICE:
		return LC_TEXT("³Ã±â ÀúÇ× %d%%");

	case POINT_RESIST_EARTH:
		return LC_TEXT("´ëÁö ÀúÇ× %d%%");

	case POINT_RESIST_DARK:
		return LC_TEXT("¾îµÒ ÀúÇ× %d%%");

	case POINT_REFLECT_MELEE:
		return LC_TEXT("Á÷Á¢ Å¸°ÝÄ¡ ¹Ý»ç È®·ü : %d%%");

	case POINT_REFLECT_CURSE:
		return LC_TEXT("ÀúÁÖ µÇµ¹¸®±â È®·ü %d%%");

	case POINT_POISON_REDUCE:
		return LC_TEXT("µ¶ ÀúÇ× %d%%");

	case POINT_BLEEDING_REDUCE:
		return LC_TEXT("Bleed reduce %d%%");

	case POINT_KILL_SP_RECOVER:
		return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã Á¤½Å·Â È¸º¹");

	case POINT_EXP_DOUBLE_BONUS:
		return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã °æÇèÄ¡ Ãß°¡ »ó½Â");

	case POINT_GOLD_DOUBLE_BONUS:
		return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã µ· 2¹è µå·Ó");

	case POINT_ITEM_DROP_BONUS:
		return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã ¾ÆÀÌÅÛ 2¹è µå·Ó");

	case POINT_POTION_BONUS:
		return LC_TEXT("¹°¾à »ç¿ë½Ã %d%% ¼º´É Áõ°¡");

	case POINT_KILL_HP_RECOVERY:
		return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã »ý¸í·Â È¸º¹");

	case POINT_ATT_GRADE_BONUS:
		return LC_TEXT("°ø°Ý·Â +%d");

	case POINT_DEF_GRADE_BONUS:
		return LC_TEXT("¹æ¾î·Â +%d");

	case POINT_MAGIC_ATT_GRADE:
		return LC_TEXT("¸¶¹ý °ø°Ý·Â +%d");

	case POINT_MAGIC_DEF_GRADE:
		return LC_TEXT("¸¶¹ý ¹æ¾î·Â +%d");

	case POINT_MAX_STAMINA:
		return LC_TEXT("ÃÖ´ë Áö±¸·Â +%d");

	case POINT_ATTBONUS_WARRIOR:
		return LC_TEXT("¹«»ç¿¡°Ô °­ÇÔ +%d%%");

	case POINT_ATTBONUS_ASSASSIN:
		return LC_TEXT("ÀÚ°´¿¡°Ô °­ÇÔ +%d%%");

	case POINT_ATTBONUS_SURA:
		return LC_TEXT("¼ö¶ó¿¡°Ô °­ÇÔ +%d%%");

	case POINT_ATTBONUS_SHAMAN:
		return LC_TEXT("¹«´ç¿¡°Ô °­ÇÔ +%d%%");

	case POINT_ATTBONUS_WOLFMAN:
		return LC_TEXT("Strong against Lycans +%d%%");

	case POINT_ATTBONUS_MONSTER:
		return LC_TEXT("¸ó½ºÅÍ¿¡°Ô °­ÇÔ +%d%%");

	case POINT_MALL_ATTBONUS:
		return LC_TEXT("°ø°Ý·Â +%d%%");

	case POINT_MALL_DEFBONUS:
		return LC_TEXT("¹æ¾î·Â +%d%%");

	case POINT_MALL_EXPBONUS:
		return LC_TEXT("°æÇèÄ¡ %d%%");

	case POINT_MALL_ITEMBONUS:
		return LC_TEXT("¾ÆÀÌÅÛ µå·ÓÀ² %d¹è");

	case POINT_MALL_GOLDBONUS:
		return LC_TEXT("µ· µå·ÓÀ² %d¹è");

	case POINT_MAX_HP_PCT:
		return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d%%");

	case POINT_MAX_SP_PCT:
		return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d%%");

	case POINT_SKILL_DAMAGE_BONUS:
		return LC_TEXT("½ºÅ³ µ¥¹ÌÁö %d%%");

	case POINT_NORMAL_HIT_DAMAGE_BONUS:
		return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö %d%%");

	case POINT_SKILL_DEFEND_BONUS:
		return LC_TEXT("½ºÅ³ µ¥¹ÌÁö ÀúÇ× %d%%");

	case POINT_NORMAL_HIT_DEFEND_BONUS:
		return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö ÀúÇ× %d%%");

	case POINT_RESIST_WARRIOR:
		return LC_TEXT("¹«»ç°ø°Ý¿¡ %d%% ÀúÇ×");

	case POINT_RESIST_ASSASSIN:
		return LC_TEXT("ÀÚ°´°ø°Ý¿¡ %d%% ÀúÇ×");

	case POINT_RESIST_SURA:
		return LC_TEXT("¼ö¶ó°ø°Ý¿¡ %d%% ÀúÇ×");

	case POINT_RESIST_SHAMAN:
		return LC_TEXT("¹«´ç°ø°Ý¿¡ %d%% ÀúÇ×");

	case POINT_RESIST_WOLFMAN:
		return LC_TEXT("Defense chance against Lycan attacks %d%%");
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	case POINT_ANTI_RESIST_MAGIC:
		return LC_TEXT("Anti Magic Resistance: %d%%");
#endif
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	case POINT_ATTBONUS_SWORD:	return LC_TEXT("Sword resistance: %d%%");
	case POINT_ATTBONUS_TWOHANDED:	return LC_TEXT("Two Hand resistance: %d%%");
	case POINT_ATTBONUS_DAGGER:	return LC_TEXT("Dagger resistance: %d%%");
	case POINT_ATTBONUS_BELL:	return LC_TEXT("Bell resistance: %d%%");
	case POINT_ATTBONUS_FAN:	return LC_TEXT("Fan resistance: %d%%");
	case POINT_ATTBONUS_BOW:	return LC_TEXT("Bow resistance: %d%%");
#endif
	default:
		return "UNK_ID %d%%";
	}
}

static bool FN_hair_affect_string(LPCHARACTER ch, char* buf, size_t bufsiz)
{
	if (nullptr == ch || nullptr == buf)
		return false;

	CAffect * aff = nullptr;
	time_t expire = 0;
	struct tm ltm;
	int	year, mon, day;
	int	offset = 0;

	aff = ch->FindAffect(AFFECT_HAIR);

	if (nullptr == aff)
		return false;

	expire = ch->GetQuestFlag("hair.limit_time");

	if (expire < get_global_time())
		return false;

	// set apply string
	offset = snprintf(buf, bufsiz, FN_point_string(aff->bApplyOn), aff->lApplyValue);

	if (offset < 0 || offset >= (int)bufsiz)
		offset = bufsiz - 1;

	localtime_r(&expire, &ltm);

	year = ltm.tm_year + 1900;
	mon = ltm.tm_mon + 1;
	day = ltm.tm_mday;

	snprintf(buf + offset, bufsiz - offset, LC_TEXT(" (¸¸·áÀÏ : %d³â %d¿ù %dÀÏ)"), year, mon, day);

	return true;
}

ACMD(do_hair)
{
	char buf[256];

	if (false == FN_hair_affect_string(ch, buf, sizeof(buf)))
		return;

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

ACMD(do_inventory)
{
	int	index = 0;
	int	count = 1;

	char arg1[256];
	char arg2[256];

	LPITEM	item;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: inventory <start_index> <count>");
		return;
	}

	if (!*arg2)
	{
		index = 0;
		str_to_number(count, arg1);
	}
	else
	{
		str_to_number(index, arg1); index = MIN(index, INVENTORY_MAX_NUM);
		str_to_number(count, arg2); count = MIN(count, INVENTORY_MAX_NUM);
	}

	for (int i = 0; i < count; ++i)
	{
		if (index >= INVENTORY_MAX_NUM)
			break;

		item = ch->GetInventoryItem(index);

		ch->ChatPacket(CHAT_TYPE_INFO, "inventory [%d] = %s",
			index, item ? item->GetName() : "<NONE>");
		++index;
	}
}

//gift notify quest command
ACMD(do_gift)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift");
}

#ifdef ENABLE_GROWTH_PET_SYSTEM
ACMD(do_CubePetAdd) {
	int pos = 0;
	int invpos = 0;

	const char* line;
	char arg1[256], arg2[256], arg3[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
		return;
	//const std::string & strArg1 = std::string(arg1);
	switch (LOWER(arg1[0]))
	{
	case 'a':	// add cue_index inven_index
	{
		if (0 == arg2[0] || !isdigit(*arg2) ||
			0 == arg3[0] || !isdigit(*arg3))
			return;

		str_to_number(pos, arg2);
		str_to_number(invpos, arg3);
	}
	break;

	default:
		return;
	}

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->SetItemCube(pos, invpos);
	else
		return;
}

ACMD(do_PetSkill) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD skillslot = 0;
	str_to_number(skillslot, arg1);
	if (skillslot > 2 || skillslot < 0)
		return;

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->DoPetSkill(skillslot);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Devam etmek icin petini cagir.!"));
}

ACMD(do_FeedCubePet) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD feedtype = 0;
	str_to_number(feedtype, arg1);

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->ItemCubeFeed(feedtype);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once petini cagir!"));
}

ACMD(do_PetEvo) {
	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened() || ch->isAuraOpened(true) || ch->isAuraOpened(false)) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet Gelistirme] Pencere Acik."));
		return;
	}
	if (ch->GetNewPetSystem()->IsActivePet()) {
		int it[3][1] = {
						{ 55003 }, //Here Modify Items to request for 1 evo
						{ 55004 }, //Here Modify Items to request for 2 evo
						{ 55005 }  //Here Modify Items to request for 3 evo
		};
		int ic[3][1] = { { 10 },
						{ 20 },
						{ 30 }
		};
		int tmpevo = ch->GetNewPetSystem()->GetEvolution();

		if ((ch->GetNewPetSystem()->GetLevel() == 40 && tmpevo == 0) ||
			(ch->GetNewPetSystem()->GetLevel() == 60 && tmpevo == 1) ||
			(ch->GetNewPetSystem()->GetLevel() == 80 && tmpevo == 2)) {
			for (int b = 0; b < 1; b++) {
				if (ch->CountSpecifyItem(it[tmpevo][b]) < ic[tmpevo][b]) {
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet Gelistirme] Gereken itemler:"));
					for (int c = 0; c < 1; c++) {
						DWORD vnum = it[tmpevo][c];
						ch->ChatPacket(CHAT_TYPE_INFO, "%s X%d", ITEM_MANAGER::instance().GetTable(vnum)->szLocaleName, ic[tmpevo][c]);
					}
					return;
				}
			}
			for (int c = 0; c < 1; c++) {
				ch->RemoveSpecifyItem(it[tmpevo][c], ic[tmpevo][c]);
			}
			ch->GetNewPetSystem()->IncreasePetEvolution();
		}
		else {
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Suan evcil hayvanin gelistirilemez!"));
			return;
		}
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zaten evcil hayvanin var!"));
}

ACMD(do_PetChangeName)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	int bCell = 0;
	str_to_number(bCell, arg1);
	LPITEM item = ch->GetInventoryItem(bCell);
	if (!item)
		return;
	if (ch->CountSpecifyItem(55008) < 1)
		return;

	if (!strcmp(arg2, "%") ||
		!strcmp(arg2, "/") ||
		!strcmp(arg2, ">") ||
		!strcmp(arg2, "|") ||
		!strcmp(arg2, ";") ||
		!strcmp(arg2, ":") ||
		!strcmp(arg2, "}") ||
		!strcmp(arg2, "{") ||
		!strcmp(arg2, "[") ||
		!strcmp(arg2, "]") ||
		!strcmp(arg2, "%") ||
		!strcmp(arg2, "#") ||
		!strcmp(arg2, "@") ||
		!strcmp(arg2, "^") ||
		!strcmp(arg2, "&") ||
		!strcmp(arg2, "'")
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] Hatali isim girdiniz"));
		return;
	}

	if (ch->GetNewPetSystem()->IsActivePet())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once petini gonder. "));
		return;
	}
	
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	DBManager::instance().EscapeString(szName, sizeof(szName), arg2, strlen(arg2));

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET name = '%s' WHERE id = '%d'", szName, item->GetID()));
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Pet Ismi Basarili Bir Sekilde Degistirildi!"));
	ch->RemoveSpecifyItem(55008, 1);
}

ACMD(do_IncreasePetSkill)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	int itempos = atoi(arg1);
	LPITEM item = ch->GetSkillBookInventoryItem(itempos);
	if (!item)
		return;
	int skill = item->GetValue(0);
	CNewPetSystem* petSystem = ch->GetNewPetSystem();
	if (!petSystem)
		return;
	if (item->GetVnum() < 55009 && item->GetVnum() > 55035)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_UNKNOWN_SKILL"));
		return;
	}
	bool ret = petSystem->IncreasePetSkill(skill);
	if (ret)
		item->SetCount(item->GetCount() - 1);
}
#endif

ACMD(do_cube)
{
	if (!ch->CanDoCube())
		return;

	int cube_index = 0, inven_index = 0, inven_type = 0;
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments(two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));

	if (0 == arg1[0])
	{
		// print usage
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: cube open");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube close");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube add <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube delete <cube_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube list");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube cancel");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube make [all]");
		return;
	}

	const std::string& strArg1 = std::string(arg1);

	// r_info (request information)
	// /cube r_info     ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé ¼ö ÀÖ´Â ·¹½ÃÇÇ ¿äÃ»
	//					    (Server -> Client) /cube r_list npcVNUM resultCOUNT 123,1/125,1/128,1/130,5
	//
	// /cube r_info 3   ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Á¤º¸¸¦ ¿äÃ»
	// /cube r_info 3 5 ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛºÎÅÍ ÀÌÈÄ 5°³ÀÇ ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Àç·á Á¤º¸¸¦ ¿äÃ»
	//					   (Server -> Client) /cube m_info startIndex count 125,1|126,2|127,2|123,5&555,5&555,4/120000@125,1|126,2|127,2|123,5&555,5&555,4/120000
	//
	if (strArg1 == "r_info")
	{
		if (0 == arg2[0])
			Cube_request_result_list(ch);
		else
		{
			if (isdigit(*arg2))
			{
				int listIndex = 0, requestCount = 1;
				str_to_number(listIndex, arg2);

				if (0 != arg3[0] && isdigit(*arg3))
					str_to_number(requestCount, arg3);

				Cube_request_material_info(ch, listIndex, requestCount);
			}
		}

		return;
	}

	switch (LOWER(arg1[0]))
	{
	case 'o':	// open
		Cube_open(ch);
		break;

	case 'c':	// close
		Cube_close(ch);
		break;

	case 'l':	// list
		Cube_show_list(ch);
		break;

	case 'a':	// add cue_index inven_index
	{
		if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3) || 0 == arg4[0] || !isdigit(*arg4))
			return;

		str_to_number(cube_index, arg2);
		str_to_number(inven_index, arg3);
		str_to_number(inven_type, arg4);
		Cube_add_item(ch, cube_index, inven_index, inven_type);
	}
	break;

	case 'd':	// delete
	{
		if (0 == arg2[0] || !isdigit(*arg2))
			return;

		str_to_number(cube_index, arg2);
		Cube_delete_item(ch, cube_index);
	}
	break;

	case 'm':	// make
		Cube_make(ch);
		break;

	default:
		return;
	}
}

#ifdef ENABLE_MINIGAME_RUMI_EVENT
ACMD(do_cards)
{
	const char* line;
	char arg1[256], arg2[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	switch (LOWER(arg1[0]))
	{
	case 'o':    // open
		if (isdigit(*arg2))
		{
			DWORD safemode = 0;
			str_to_number(safemode, arg2);
			ch->Cards_open(safemode);
		}
		break;
	case 'p':    // open
		ch->Cards_pullout();
		break;
	case 'e':    // open
		ch->CardsEnd();
		break;
	case 'd':    // open
		if (isdigit(*arg2))
		{
			DWORD destroy_index = 0;
			str_to_number(destroy_index, arg2);
			ch->CardsDestroy(destroy_index);
		}
		break;
	case 'a':    // open
		if (isdigit(*arg2))
		{
			DWORD accpet_index = 0;
			str_to_number(accpet_index, arg2);
			ch->CardsAccept(accpet_index);
		}
		break;
	case 'r':    // open
		if (isdigit(*arg2))
		{
			DWORD restore_index = 0;
			str_to_number(restore_index, arg2);
			ch->CardsRestore(restore_index);
		}
		break;
	default:
		return;
	}
}
#endif

ACMD(do_in_game_mall)
{
	char country_code[3];

	switch (LC_GetLocalType())
	{
	case LC_GERMANY:	country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0'; break;
	case LC_FRANCE:		country_code[0] = 'f'; country_code[1] = 'r'; country_code[2] = '\0'; break;
	case LC_ITALY:		country_code[0] = 'i'; country_code[1] = 't'; country_code[2] = '\0'; break;
	case LC_SPAIN:		country_code[0] = 'e'; country_code[1] = 's'; country_code[2] = '\0'; break;
	case LC_UK:			country_code[0] = 'e'; country_code[1] = 'n'; country_code[2] = '\0'; break;
	case LC_TURKEY:		country_code[0] = 't'; country_code[1] = 'r'; country_code[2] = '\0'; break;
	case LC_POLAND:		country_code[0] = 'p'; country_code[1] = 'l'; country_code[2] = '\0'; break;
	case LC_PORTUGAL:	country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0'; break;
	case LC_GREEK:		country_code[0] = 'g'; country_code[1] = 'r'; country_code[2] = '\0'; break;
	case LC_RUSSIA:		country_code[0] = 'r'; country_code[1] = 'u'; country_code[2] = '\0'; break;
	case LC_DENMARK:	country_code[0] = 'd'; country_code[1] = 'k'; country_code[2] = '\0'; break;
	case LC_BULGARIA:	country_code[0] = 'b'; country_code[1] = 'g'; country_code[2] = '\0'; break;
	case LC_CROATIA:	country_code[0] = 'h'; country_code[1] = 'r'; country_code[2] = '\0'; break;
	case LC_MEXICO:		country_code[0] = 'm'; country_code[1] = 'x'; country_code[2] = '\0'; break;
	case LC_ARABIA:		country_code[0] = 'a'; country_code[1] = 'e'; country_code[2] = '\0'; break;
	case LC_CZECH:		country_code[0] = 'c'; country_code[1] = 'z'; country_code[2] = '\0'; break;
	case LC_ROMANIA:	country_code[0] = 'r'; country_code[1] = 'o'; country_code[2] = '\0'; break;
	case LC_HUNGARY:	country_code[0] = 'h'; country_code[1] = 'u'; country_code[2] = '\0'; break;
	case LC_NETHERLANDS: country_code[0] = 'n'; country_code[1] = 'l'; country_code[2] = '\0'; break;
	case LC_USA:		country_code[0] = 'u'; country_code[1] = 's'; country_code[2] = '\0'; break;
	case LC_CANADA:	country_code[0] = 'c'; country_code[1] = 'a'; country_code[2] = '\0'; break;
	default:
		if (test_server == true)
		{
			country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0';
		}
		break;
	}

	char buf[512 + 1];
	char sas[33];
	MD5_CTX ctx;
	const char sas_key[] = "GF9001";

	snprintf(buf, sizeof(buf), "%u%u%s", ch->GetPlayerID(), ch->GetAID(), sas_key);

	MD5Init(&ctx);
	MD5Update(&ctx, (const unsigned char*)buf, strlen(buf));

#ifdef __FreeBSD__
	MD5End(&ctx, sas);
#else
	static const char hex[] = "0123456789abcdef";
	unsigned char digest[16];
	MD5Final(digest, &ctx);
	int i;
	for (i = 0; i < 16; ++i) {
		sas[i + i] = hex[digest[i] >> 4];
		sas[i + i + 1] = hex[digest[i] & 0x0f];
	}
	sas[i + i] = '\0';
#endif

	snprintf(buf, sizeof(buf), "mall http://%s/ishop?pid=%u&c=%s&sid=%d&sas=%s",
		g_strWebMallURL.c_str(), ch->GetPlayerID(), country_code, g_server_id, sas);

	ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
}

// ì£¼ì‚¬ìœ„
ACMD(do_dice)
{
	char arg1[256], arg2[256];
	int start = 1, end = 100;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (*arg1 && *arg2)
	{
		start = atoi(arg1);
		end = atoi(arg2);
	}
	else if (*arg1 && !*arg2)
	{
		start = 1;
		end = atoi(arg1);
	}

	end = MAX(start, end);
	start = MIN(start, end);

	int n = number(start, end);

#ifdef ENABLE_DICE_SYSTEM
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_DICE_INFO, LC_TEXT("%s´ÔÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_DICE_INFO, LC_TEXT("´ç½ÅÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), n, start, end);
#else
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_INFO, LC_TEXT("%s´ÔÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ç½ÅÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), n, start, end);
#endif
}

ACMD(do_click_mall)
{
	if (CAN_OPEN_SAFEBOX(ch->GetMapIndex()) == false)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
	}
}

ACMD(do_click_safebox)
{
	if (CAN_OPEN_SAFEBOX(ch->GetMapIndex()) == false)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
	}
}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
ACMD(do_ride)
{
	if (ch->IsDead() || ch->IsStun())
		return;
	if (ch->GetMapIndex() == 113)
		return;
	if (ch->IsPolymorphed()) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Dönüþmüþ haldeyken bineðe binemezsin."));
		return;
	}
	if (ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;
		if (!mountSystem && !mount)
			return;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(mount->GetTransmutation());
		if (pkItemTransmutation)
		{
			for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
			{
				BYTE bType = pkItemTransmutation->aApplies[i].bType;
				long value = pkItemTransmutation->aApplies[i].lValue;
				if (bType == APPLY_MOUNT)
					mobVnum = value;
			}
		}
		else
		{
			if (mount->FindApplyValue(APPLY_MOUNT) != 0)
				mobVnum = mount->FindApplyValue(APPLY_MOUNT);
		}
#else
		if (mount->FindApplyValue(APPLY_MOUNT) != 0)
			mobVnum = mount->FindApplyValue(APPLY_MOUNT);
#endif

		if (ch->GetMountVnum())
		{
			if (mountSystem->CountSummoned() == 0)
				mountSystem->Unmount(mobVnum);
		}
		else
		{
			if (mountSystem->CountSummoned() == 1)
				mountSystem->Mount(mobVnum, mount);
		}

		return;
	}
	if (ch->IsHorseRiding())
	{
		ch->StopRiding();
		return;
	}

	if (ch->GetHorse() != NULL)
	{
		ch->StartRiding();
		return;
	}

	for (BYTE i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (NULL == item)
			continue;

		if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_MOUNT) {
			ch->UseItem(TItemPos(INVENTORY, i));
			return;
		}
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
}
#else
ACMD(do_ride)
{
	if (ch->IsDead() || ch->IsStun())
		return;

	// ³»¸®±â
	{
		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
			return;
		}

		if (ch->GetMountVnum())
		{
			do_unmount(ch, nullptr, 0, 0);
			return;
		}
	}

	if (ch->GetQuestFlag("HORSE.CHECKER") && get_global_time() < ch->GetQuestFlag("HORSE.CHECKER"))
	{
		return;
	}
	if (ch->GetHorse() != nullptr)
	{
		ch->SetQuestFlag("HORSE.CHECKER", get_global_time() + 3);
		ch->StartRiding();
		return;
	}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	for (WORD i = 0; i < ch->GetExtendInvenMax(); ++i)
#else
	for (WORD i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (nullptr == item)
			continue;

		// À¯´ÏÅ© Å»°Í ¾ÆÀÌÅÛ
		if (item->IsRideItem())
		{
			if (nullptr == ch->GetWear(WEAR_UNIQUE1) || nullptr == ch->GetWear(WEAR_UNIQUE2))
			{
				//ch->EquipItem(item);
				ch->UseItem(TItemPos(INVENTORY, i));
				return;
			}
		}

		// ÀÏ¹Ý Å»°Í ¾ÆÀÌÅÛ
		// TODO : Å»°Í¿ë SubType Ãß°¡
		switch (item->GetVnum())
		{
		case 71114:	// Àú½ÅÀÌ¿ë±Ç
		case 71116:	// »ê°ß½ÅÀÌ¿ë±Ç
		case 71118:	// ÅõÁö¹üÀÌ¿ë±Ç kodlar©¥n©¥ ekle biner ctrl gtamam client src ac
		case 71120:	// »çÀÚ¿ÕÀÌ¿ë±Ç
			ch->UseItem(TItemPos(INVENTORY, i));
			return;
		}

		// GF mantis #113524, 52001~52090 ¹ø Å»°Í
		if ((item->GetVnum() > 52000) && (item->GetVnum() < 52091)) {
			ch->UseItem(TItemPos(INVENTORY, i));
			return;
		}
	}

	// Å¸°Å³ª ³»¸± ¼ö ¾øÀ»¶§
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
}
#endif

ACMD(do_ajanisik)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetWarMap())
		return;

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);
	if (!tch)
		return;
	if (!tch->IsPC())
		return;
	if (ch == tch)
		return;
	if (!tch->GetGuild())
		return;
	if (!ch->GetGuild())
		return;
	if (tch->GetGuild()->GetID() != ch->GetGuild()->GetID())
		return;
	CGuild * loncam = CGuildManager::instance().FindGuild(ch->GetGuild()->GetID());
	CGuild * loncan = CGuildManager::instance().FindGuild(tch->GetGuild()->GetID());
	if ((loncam == nullptr) || (loncan == nullptr))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUILD_SPY_SYSTEM_DOESNT_HAVE_GUILD"));
	}
	else
	{
		if (loncan->GetMasterPID() == ch->GetPlayerID())
		{
			if (loncam->RequestRemoveMember(tch->GetPlayerID()))
			{
				ch->ChatPacket(CHAT_TYPE_GUILD, LC_TEXT("%s ajan olarak secilip kovuldu."), tch->GetName());
				tch->GoHome();
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bi hata var ama cozemedim."));
				return;
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("once lonca lideri ol gavat"));
			return;
		}
	}
	return;
}

#ifdef ENABLE_CHANNEL_SWITCH_SYSTEM
ACMD(do_change_channel)
{
	if (!ch)
		return;

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}
	
	if (!features::IsFeatureEnabled(features::CHANNEL_CHANGE))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHANNEL_CHANGE_SYSTEM_DISABLED"));
		return;		
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Usage: channel <new channel>"));
		return;
	}

	short channel;
	str_to_number(channel, arg1);

	if (channel < 0 || channel > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please enter a valid channel."));
		return;
	}

	if (channel == g_bChannel)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are already on channel %d."), g_bChannel);
		return;
	}

	if (g_bChannel == 99)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The map you are at is cross-channel, changing won't have any effect."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change channel while in a dungeon."));
		return;
	}

	TPacketChangeChannel p;
	p.channel = channel;
	p.lMapIndex = ch->GetMapIndex();

	db_clientdesc->DBPacket(HEADER_GD_FIND_CHANNEL, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}
#endif

#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
ACMD(do_view_equip_request)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (!tch || tch == nullptr)
		return;

	if (tch->IsGM())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cant do this because he/she is a gm."), tch->GetName());
		return;
	}

	if (ch->IsGM())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cant do this feature because your gm."));
		return;
	}

	if (tch->IsBlockMode(BLOCK_EQUIPMENT_REQUEST))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is blocked view equipment request."), tch->GetName());
		return;
	}

	if (tch)
	{
		if (!ch->RequestToViewEquip(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "ViewEquipRequestDenied");
	}
}

ACMD(do_view_equip_request_accept)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToViewEquip(tch);
}

ACMD(do_view_equip_request_deny)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToViewEquip(tch);
}
#endif

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
ACMD(do_item_comb)
{
	if (!ch->CanDoComb())
		return;
	
	if (!features::IsFeatureEnabled(features::COMBINATION))
	{
		LogManager::instance().HackLog("COMBINATIONDISABLED", ch);
		return;
	}

	int combination_index = 0, inven_index = 0, inven_type = 0;
	const char* line;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	char arg1[256], arg2[256], arg3[256], arg4[256];
#else
	char arg1[256], arg2[256], arg3[256];
#endif
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	one_argument(line, arg4, sizeof(arg4));
#endif

	if (0 == arg1[0])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb open_costume");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb open_skillbook");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb close");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb add_item <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb del_item <combination_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb make_cotume");
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: item_comb make_skillbook");
		return;
	}

	const std::string& strArg1 = std::string(arg1);
	if (strArg1 == "open_costume")
	{
		Comb_open_costume(ch);
		return;
	}
	else if (strArg1 == "open_skillbook")
	{
		Comb_open_skillbook(ch);
		return;
	}
	else if (strArg1 == "add_item")
	{
		if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3))
			return;
		str_to_number(combination_index, arg2);
		str_to_number(inven_index, arg3);
		Comb_add_item(ch, combination_index, inven_index);
	}
	else if (strArg1 == "add_item2")
	{
		if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3) || 0 == arg4[0] || !isdigit(*arg4))
			return;
		str_to_number(combination_index, arg2);
		str_to_number(inven_index, arg3);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		str_to_number(inven_type, arg4);
		Comb_add_item2(ch, combination_index, inven_index, inven_type);
#else
		Comb_add_item2(ch, combination_index, inven_index);
#endif
	}
	else if (strArg1 == "del_item")
	{
		if (0 == arg2[0] || !isdigit(*arg2))
			return;
		str_to_number(combination_index, arg2);
		Comb_del_item(ch, combination_index);
	}
	else if (strArg1 == "make_cotume")
	{
		Comb_make_cotume(ch);
		return;
	}
	else if (strArg1 == "make_skillbook")
	{
		Comb_make_skillbook(ch);
		return;
	}
	else if (strArg1 == "close")
	{
		Comb_close(ch);
		return;
	}
}
#endif
#ifdef ENABLE_FAST_SKILL_BOOK_SYSTEM
LPEVENT bktimer = nullptr;

EVENTINFO(TMainEventInfo2)
{
	LPCHARACTER	kim;
	long skillindexx;

	TMainEventInfo2()
		: kim(nullptr)
		, skillindexx(0)
	{
	}
};

EVENTFUNC(bk_event)
{
	TMainEventInfo2* info = dynamic_cast<TMainEventInfo2*>(event->info);

	if (info == nullptr)
	{
		sys_err("ruh_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->kim;
	long skillindex = info->skillindexx;

	if (nullptr == ch || skillindex == 0)
		return 0;

	if (!ch)
		return 0;

	if (ch)
	{
		if (ch->CountSpecifyItem(50300) < 1)
		{
			return 0;
		}

		int skilllevel = ch->GetSkillLevel(skillindex);
		if (skilllevel >= 30)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkskillson"));
			return 0;
		}

		int dwVnum = ch->BKBul(skillindex);
		if (dwVnum == 999)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkskillyok"));
			return 0;
		}

		LPITEM item = ch->GetSkillBookInventoryItem(dwVnum);

		if (!item)
			return 0;

		if (item->GetVnum() != 50300 || item->GetSocket(0) != skillindex)
			return 0;

		if (ch->CountSpecifyItem(71001) > 0)
		{
			if (!ch->IsAffectFlag(513))
			{
				ch->AddAffect(513, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
				ch->RemoveSpecifyItem(71001, 1);
			}
		}

		if (ch->CountSpecifyItem(71001) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkkoturuhyok"));
			return 0;
		}

		if (ch->CountSpecifyItem(71094) >= 1)
		{
			if (!ch->IsAffectFlag(512))
			{
				ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
				ch->RemoveSpecifyItem(71094, 1);
			}
		}
		if (item->GetVnum() == 50300)
		{
			if (true == ch->LearnSkillByBook(skillindex))
			{
				if (item->GetVnum() == 50300 && item->GetSocket(0) == skillindex)
				{
					item->SetCount(item->GetCount() - 1);
					int iReadDelay;
					iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					ch->SetSkillNextReadTime(skillindex, get_global_time() + sb_delay);
				}
			}
			else
			{
				if (item->GetVnum() == 50300 && item->GetSocket(0) == skillindex)
				{
					item->SetCount(item->GetCount() - 1);
					int iReadDelay;
					iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					ch->SetSkillNextReadTime(skillindex, get_global_time() + sb_delay);
				}
			}
		}
		return 1;
	}
	return 0;
}
ACMD(do_bkoku)
{
	int gelen;
	long skillindex;
	char arg1[256], arg2[256];

	if (ch->GetQuestFlag("pcservis.muhur") - time(0) > 0) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Skills are binded."));
		return;
	}

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;
	str_to_number(gelen, arg1);
	str_to_number(skillindex, arg2);

	if (gelen < 0 || skillindex < 0)
		return;

	if (!ch)
		return;

	if (!ch->IsPC())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHack())
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	int skillgrup = ch->GetSkillGroup();
	if (skillgrup == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkokuyamazsin"));
		return;
	}

	if (ch->CountSpecifyItem(50300) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkyok"));
		return;
	}

	if (ch->GetMapIndex() == 246)
		return;

	if (gelen == 1) ///tek
	{
		int skilllevel = ch->GetSkillLevel(skillindex);

		if (skilllevel >= 30)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhskillson"));
			return;
		}

		if (ch->GetQuestFlag("bk.yenisure") > get_global_time())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bk1sn"));
			return;
		}

		int dwVnum = ch->BKBul(skillindex);
		if (dwVnum == 999)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bkskillyok"));
			return;
		}

		LPITEM item = ch->GetSkillBookInventoryItem(dwVnum);

		if (!item)
			return;

		if (item->GetVnum() != 50300 || item->GetSocket(0) != skillindex)
			return;

		if (ch->CountSpecifyItem(71001) > 0)
		{
			if (!ch->IsAffectFlag(513))
			{
				ch->AddAffect(513, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
				ch->RemoveSpecifyItem(71001, 1);
			}
		}

		if (ch->CountSpecifyItem(71094) >= 1)
		{
			if (!ch->IsAffectFlag(512))
			{
				ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
				ch->RemoveSpecifyItem(71094, 1);
			}
		}

		if (item->GetVnum() == 50300)
		{
			if (ch->FindAffect(AFFECT_SKILL_NO_BOOK_DELAY) == false && ch->CountSpecifyItem(71001) < 1)
			{
				ch->SkillLearnWaitMoreTimeMessage(ch->GetSkillNextReadTime(skillindex) - get_global_time());
				return;
			}

			if (true == ch->LearnSkillByBook(skillindex))
			{
				if (item->GetVnum() == 50300 && item->GetSocket(0) == skillindex)
				{
					item->SetCount(item->GetCount() - 1);
					int iReadDelay;
					iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					ch->SetSkillNextReadTime(skillindex, get_global_time() + sb_delay);
				}
			}
			else
			{
				if (item->GetVnum() == 50300 && item->GetSocket(0) == skillindex)
				{
					item->SetCount(item->GetCount() - 1);
					int iReadDelay;
					iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					ch->SetSkillNextReadTime(skillindex, get_global_time() + sb_delay);
				}
			}
			ch->SetQuestFlag("bk.yenisure", get_global_time() + 1);
		}
	}
	else if (gelen == 0) ///hepsi
	{
		if (bktimer)
			event_cancel(&bktimer);

		TMainEventInfo2* info = AllocEventInfo<TMainEventInfo2>();
		info->kim = ch;
		info->skillindexx = skillindex;
		bktimer = event_create(bk_event, info, PASSES_PER_SEC(1));
	}

	return;
}
#endif

#ifdef ENABLE_FAST_SOUL_STONE_SYSTEM
LPEVENT ruhtimer = nullptr;

EVENTINFO(TMainEventInfo)
{
	LPCHARACTER	kim;
	long skillindexx;

	TMainEventInfo()
		: kim(nullptr)
		, skillindexx(0)
	{
	}
};

EVENTFUNC(ruh_event)
{
	TMainEventInfo* info = dynamic_cast<TMainEventInfo*>(event->info);

	if (info == nullptr)
	{
		sys_err("ruh_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->kim;
	long skillindex = info->skillindexx;

	if (nullptr == ch || skillindex == 0)
		return 0;

	if (ch->CountSpecifyItem(50513) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhtasiyok"));
		return 0;
	}

	int skilllevel = ch->GetSkillLevel(skillindex);

	if (skilllevel >= 40)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhskillson"));
		return 0;
	}
	int gerekenderece = (1000 + 1000 * (skilllevel - 30));
	int derecem = (ch->GetRealAlignment() / 10);
	int sonuc = (-19000 + gerekenderece);

	if (derecem < 0)
	{
		gerekenderece = gerekenderece * 2;
		sonuc = (-19000 - gerekenderece);
	}

	if (derecem > sonuc)
	{
		int gerekliknk = gerekenderece;
		int kactane = gerekliknk / 1000;
		if (kactane < 0)
			kactane = 0 - kactane;

		if (derecem < 0)
		{
			if (ch->CountSpecifyItem(70102) < kactane)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbitti %d"), kactane);
				return 0;
			}

			int delta = MIN(-(ch->GetAlignment()), 1000);
#ifdef ENABLE_ALIGNMENT_SYSTEM
			ch->UpdateAlignment(delta * kactane, true);
#else
			ch->UpdateAlignment(delta * kactane);
#endif
			ch->RemoveSpecifyItem(70102, kactane);
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbastim"));
		}
	}

	if (ch->GetQuestFlag("ruh.sure") > get_global_time())
	{
		if (ch->CountSpecifyItem(71001) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhsuredolmadi"));
			return 0;
		}
		else
			ch->RemoveSpecifyItem(71001, 1);
	}

	if (ch->CountSpecifyItem(71094) >= 1)
	{
		ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
		ch->RemoveSpecifyItem(71094, 1);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhmunzevikullandim"));
	}

	if (gerekenderece < 0)
#ifdef ENABLE_ALIGNMENT_SYSTEM
		ch->UpdateAlignment(gerekenderece * 10, true);
#else
		ch->UpdateAlignment(gerekenderece * 10);
#endif
	else
#ifdef ENABLE_ALIGNMENT_SYSTEM
		ch->UpdateAlignment(-gerekenderece * 10, true);
#else
		ch->UpdateAlignment(-gerekenderece * 10);
#endif

	ch->LearnGrandMasterSkill(skillindex);
	ch->RemoveSpecifyItem(50513, 1);
	ch->SetQuestFlag("ruh.sure", get_global_time() + sb_delay);

	return 1;
}

ACMD(do_ruhoku)
{
	int gelen;
	long skillindex;
	char arg1[256], arg2[256];

	if (ch->GetQuestFlag("pcservis.muhur") - time(0) > 0) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Skills are binded."));
		return;
	}

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;
	str_to_number(gelen, arg1);
	str_to_number(skillindex, arg2);

	if (gelen < 0 || skillindex < 0)
		return;

	if (!ch)
		return;

	if (!ch->IsPC())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHack())
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->CountSpecifyItem(50513) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhtasiyok"));
		return;
	}

	if (ch->GetMapIndex() == 246)
		return;

	LPITEM slot1 = ch->GetWear(WEAR_UNIQUE1);
	LPITEM slot2 = ch->GetWear(WEAR_UNIQUE2);

	if (nullptr != slot1)
	{
		if (slot1->GetVnum() == 70048)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pelerin_cikar"));
			return;
		}
	}

	if (nullptr != slot2)
	{
		if (slot2->GetVnum() == 70048)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pelerin_cikar"));
			return;
		}
	}

	int skillgrup = ch->GetSkillGroup();
	if (skillgrup == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhokuyamazsin"));
		return;
	}

	if (gelen == 1) ///tek
	{
		int skilllevel = ch->GetSkillLevel(skillindex);
		int gerekenderece = (1000 + 1000 * (skilllevel - 30));
		int derecem = (ch->GetRealAlignment() / 10);
		int sonuc = (-19000 + gerekenderece);

		if (ch->GetQuestFlag("ruh.yenisure") > get_global_time())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruh1sn"));
			return;
		}

		if (derecem < 0)
		{
			gerekenderece = gerekenderece * 2;
			sonuc = (-19000 - gerekenderece);
		}

		if (derecem > sonuc)
		{
			int gerekliknk = gerekenderece;
			int kactane = gerekliknk / 1000;
			if (kactane < 0)
				kactane = 0 - kactane;

			if (derecem < 0)
			{
				if (ch->CountSpecifyItem(70102) < kactane)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbitti %d"), kactane);
					return;
				}

				int delta = MIN(-(ch->GetAlignment()), 1000);
#ifdef ENABLE_ALIGNMENT_SYSTEM
				ch->UpdateAlignment(delta * kactane, true);
#else
				ch->UpdateAlignment(delta * kactane);
#endif
				ch->RemoveSpecifyItem(70102, kactane);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbastim"));
			}
		}

		if (ch->GetQuestFlag("ruh.sure") > get_global_time())
		{
			if (ch->CountSpecifyItem(71001) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhsuredolmadi"));
				return;
			}
			else
				ch->RemoveSpecifyItem(71001, 1);
		}

		if (ch->CountSpecifyItem(71094) >= 1)
		{
			ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
			ch->RemoveSpecifyItem(71094, 1);
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhmunzevikullandim"));
		}

		if (gerekenderece < 0)
#ifdef ENABLE_ALIGNMENT_SYSTEM
			ch->UpdateAlignment(gerekenderece * 10, true);
#else
			ch->UpdateAlignment(gerekenderece * 10);
#endif
		else
#ifdef ENABLE_ALIGNMENT_SYSTEM
			ch->UpdateAlignment(-gerekenderece * 10, true);
#else
			ch->UpdateAlignment(-gerekenderece * 10);
#endif

		ch->LearnGrandMasterSkill(skillindex);
		ch->RemoveSpecifyItem(50513, 1);
		ch->SetQuestFlag("ruh.sure", get_global_time() + sb_delay);
		ch->SetQuestFlag("ruh.yenisure", get_global_time() + 1);
	}
	else if (gelen == 0) ///hepsi
	{
		if (ruhtimer)
			event_cancel(&ruhtimer);

		TMainEventInfo* info = AllocEventInfo<TMainEventInfo>();
		info->kim = ch;
		info->skillindexx = skillindex;
		ruhtimer = event_create(ruh_event, info, PASSES_PER_SEC(1));
	}
	return;
}
#endif
ACMD(do_biyolog)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!ch->IsPC())
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isAuraOpened(true) || ch->isAuraOpened(false))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	int biodurum = ch->GetQuestFlag("bio.durum");
	int bioverilen = ch->GetQuestFlag("bio.verilen");
	int biokalan = ch->GetQuestFlag("bio.kalan");
	int ruhmu = ch->GetQuestFlag("bio.ruhtasi");
	int gerekenitem = BiyologSistemi[biodurum][0];
	int ruhtasi = BiyologSistemi[biodurum][4];
	int odul = BiyologSistemi[biodurum][5];
	int sans = BiyologSistemi[biodurum][3];
	int bekleme = (BiyologSistemi[biodurum][2] * 60);
	int affectvnum = BiyologSistemi[biodurum][6];
	int affectvalue = BiyologSistemi[biodurum][7];
	int affectvnum2 = BiyologSistemi[biodurum][8];
	int affectvalue2 = BiyologSistemi[biodurum][9];
	int affectvnum3 = BiyologSistemi[biodurum][10];
	int affectvalue3 = BiyologSistemi[biodurum][11];
	int affectvnum4 = BiyologSistemi[biodurum][12];
	int affectvalue4 = BiyologSistemi[biodurum][13];
	int affectvnum5 = BiyologSistemi[biodurum][14];
	int affectvalue5 = BiyologSistemi[biodurum][15];
	int toplam = BiyologSistemi[biodurum][1];
	int level = ch->GetLevel();

	if (*arg1 && *arg2)
	{
		if (ch->GetQuestFlag("bio.odulvakti") == 0)
			return;
		int secim = atoi(arg2);
		int seviye = atoi(arg1);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
		if (secim == 1)
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
		else if (secim == 2)
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60 * 60 * 24 * 365 * 60, 0, false);
		else if (secim == 3)
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60 * 60 * 24 * 365 * 60, 0, false);
		if (seviye == 92)
		{
			if (level >= 94)
			{
				ch->SetQuestFlag("bio.durum", 10);
				ch->SetQuestFlag("bio.verilen", 0);
				ch->SetQuestFlag("bio.kalan", 0);
				ch->SetQuestFlag("bio.ruhtasi", 0);
				ch->SetQuestFlag("bio.odulvakti", 0);
				biodurum = ch->GetQuestFlag("bio.durum");
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
				return;
			}
			else
			{
				ch->SetQuestFlag("bio.durum", 93);
				ch->SetQuestFlag("bio.verilen", 0);
				ch->SetQuestFlag("bio.kalan", 0);
				ch->SetQuestFlag("bio.ruhtasi", 0);
				ch->SetQuestFlag("bio.odulvakti", 0);
				biodurum = ch->GetQuestFlag("bio.durum");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
				return;
			}
		}
		if (seviye == 94)
		{
			if (level >= 94)
			{
				ch->SetQuestFlag("bio.durum", 11);
				ch->SetQuestFlag("bio.verilen", 0);
				ch->SetQuestFlag("bio.kalan", 0);
				ch->SetQuestFlag("bio.ruhtasi", 0);
				ch->SetQuestFlag("bio.odulvakti", 0);
				biodurum = ch->GetQuestFlag("bio.durum");
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevlerbitti"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
				return;
			}
		}
		return;
	}

	if (level < 30)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biolevelyok"));
		return;
	}

	if (ch->GetQuestFlag("bio.odulvakti") == 1)
	{
		if (biodurum == 9)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 92 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
		}
		else if (biodurum == 10)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 94 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
		}
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
		return;
	}

	if (biokalan > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosuredolmadi"));
		return;
	}

	/////ork diþi
	if (biodurum == 1 and level >= 30)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 40)
				{
					ch->SetQuestFlag("bio.durum", 2);
					ch->SetQuestFlag("bio.30", 1);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 31);
					ch->SetQuestFlag("bio.30", 1);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				// ch->ChatPacket(CHAT_TYPE_INFO, "%d %d",sans,prob);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	//////lanet kitabý
	if (biodurum == 2 and level >= 40)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 50)
				{
					ch->SetQuestFlag("bio.durum", 3);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.40", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 41);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.40", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);

				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	////////þeytan hatýrasý
	if (biodurum == 3 and level >= 50)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 60)
				{
					ch->SetQuestFlag("bio.durum", 4);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.50", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 51);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.50", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	////////buz topu
	if (biodurum == 4 and level >= 60)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 70)
				{
					ch->SetQuestFlag("bio.durum", 5);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.60", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 61);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.60", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	////////zelkova
	if (biodurum == 5 and level >= 70)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 80)
				{
					ch->SetQuestFlag("bio.durum", 6);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.70", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 71);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.70", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);

				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	////////tabela
	if (biodurum == 6 and level >= 80)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 85)
				{
					ch->SetQuestFlag("bio.durum", 7);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.80", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 81);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.80", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}

	////////kýrýk dal
	if (biodurum == 7 and level >= 85)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum4].bPointType, affectvalue4, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum5].bPointType, affectvalue5, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 90)
				{
					ch->SetQuestFlag("bio.durum", 8);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.85", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 86);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.85", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}

	////////lider not
	if (biodurum == 8 and level >= 90)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->AutoGiveItem(odul, 1);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum4].bPointType, affectvalue4, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum5].bPointType, affectvalue5, 0, 60 * 60 * 24 * 365 * 60, 0, false);
				if (level >= 92)
				{
					ch->SetQuestFlag("bio.durum", 9);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.90", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum", 91);
					ch->SetQuestFlag("bio.verilen", 0);
					ch->SetQuestFlag("bio.kalan", 0);
					ch->SetQuestFlag("bio.ruhtasi", 0);
					ch->SetQuestFlag("bio.90", 1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}

	////////kemgöz
	if (biodurum == 9 and level >= 92)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1, 100);
			if (ch->GetQuestFlag("bio.sans") == 1)
				sans = sans + 100;
			if (ch->GetQuestFlag("bio.sure") == 1)
				ch->SetQuestFlag("bio.sure", 0);
			if (sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen", bioverilen + 1);
				if (ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans", 0);
				}

				bioverilen = ch->GetQuestFlag("bio.verilen");
				if (bioverilen == toplam)
				{
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 92 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
					ch->SetQuestFlag("bio.odulvakti", 1);
					ch->SetQuestFlag("bio.92", 1);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
					return;
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem, 1);
			return;
		}
	}

	////////kemgöz
	if (biodurum == 10 and level >= 94)
	{
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->RemoveSpecifyItem(ruhtasi, 1);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 94 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
				ch->SetQuestFlag("bio.odulvakti", 1);
				ch->SetQuestFlag("bio.94", 1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
				return;
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1, 100);
				if (ch->GetQuestFlag("bio.sans") == 1)
					sans = sans + 100;
				if (ch->GetQuestFlag("bio.sure") == 1)
					ch->SetQuestFlag("bio.sure", 0);
				if (sans >= prob)
				{
					ch->SetQuestFlag("bio.verilen", bioverilen + 1);
					if (ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans", 0);
					}

					bioverilen = ch->GetQuestFlag("bio.verilen");
					if (bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi", 1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam - bioverilen));
						ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan", get_global_time() + bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem, 1);
				return;
			}
		}
	}
	return;
}

ACMD(do_health_board_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_health_board", iValue);
	ch->UpdatePacket();
}

ACMD(do_sash_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_sash", iValue);
	ch->UpdatePacket();
}

ACMD(do_costume_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume", iValue);
	if (iValue == 1)
	{
		LPITEM pkBody = ch->GetWear(WEAR_BODY);
		if (pkBody)
		{
			DWORD dwRes = pkBody->GetTransmutation() != 0 ? pkBody->GetTransmutation() : pkBody->GetVnum();
			ch->SetPart(PART_MAIN, dwRes);
		}
	}
	else
	{
		LPITEM pkBody = ch->GetWear(WEAR_COSTUME_BODY);
		if (pkBody)
		{
			DWORD dwRes = pkBody->GetTransmutation() != 0 ? pkBody->GetTransmutation() : pkBody->GetVnum();
			ch->SetPart(PART_MAIN, dwRes);
		}
	}
	ch->UpdatePacket();
}

ACMD(do_costume_w_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume_w", iValue);
	if (iValue == 1)
	{
		LPITEM pkWeapon = ch->GetWear(WEAR_WEAPON);
		if (pkWeapon)
		{
			DWORD dwRes = pkWeapon->GetTransmutation() != 0 ? pkWeapon->GetTransmutation() : pkWeapon->GetVnum();
			ch->SetPart(PART_WEAPON, dwRes);
		}
	}
	else
	{
		LPITEM pkWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
		if (pkWeapon)
		{
			DWORD dwRes = pkWeapon->GetTransmutation() != 0 ? pkWeapon->GetTransmutation() : pkWeapon->GetVnum();
			ch->SetPart(PART_WEAPON, dwRes);
		}
	}
	ch->UpdatePacket();
}

ACMD(do_costume_h_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume_h", iValue);
	if (iValue == 1)
		ch->SetPart(PART_HAIR, 0);
	else
	{
		LPITEM pkHair = ch->GetWear(WEAR_COSTUME_HAIR);
		if (pkHair)
		{
			TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(pkHair->GetTransmutation());
			ch->SetPart(PART_HAIR, (pItemTable != nullptr) ? pItemTable->alValues[3] : pkHair->GetValue(3));
		}
	}
	ch->UpdatePacket();
}

ACMD(do_level_view_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.level_view", iValue);
	ch->ViewReencode();
}

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
ACMD(do_evo_points)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD dwPoints = atoi(arg1);
	CItem* pWeapon = ch->GetWear(WEAR_WEAPON);

	if (pWeapon && IS_SET(pWeapon->GetFlag(), ITEM_FLAG_RARE_ABILITY))
	{
		pWeapon->UpdateRarePoints(dwPoints);
		ch->ChatPacket(CHAT_TYPE_INFO, "%d evolution points has been successfully added to your weapon", dwPoints);
	}
}

ACMD(do_refine_evo_points)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	three_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3));

	if (!*arg1 || !*arg2)
		return;

	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­¸°»óÅÂ¿¡¼­´Â »óÁ¡°Å·¡¸¦ ÇÒ¼ö °¡ ¾ø½À´Ï´Ù."));
		return;
	}

	DWORD dwPos = 0;
	str_to_number(dwPos, arg1);

	if (dwPos < 0 || dwPos >= INVENTORY_MAX_NUM)
		return;

	LPITEM item = ch->GetInventoryItem(dwPos);

	if (!item || item->IsExchanging() || item->IsEquipped())
		return;
	
	DWORD dwSecondPos = 0;
	str_to_number(dwSecondPos, arg2);
	
	if (dwSecondPos < 0 || dwSecondPos >= INVENTORY_MAX_NUM)
		return;
	
	LPITEM second_item = ch->GetInventoryItem(dwSecondPos);

	if (!second_item || second_item->IsExchanging() || second_item->IsEquipped())
		return;
	
	LPITEM third_item = nullptr;
	
	if (*arg3)
	{
		DWORD dwThirdPos = 0;
		str_to_number(dwThirdPos, arg3);
		if (dwThirdPos < 0 || dwThirdPos >= INVENTORY_MAX_NUM)
			return;
		
		third_item = ch->GetInventoryItem(dwThirdPos);
		
		if (!third_item || third_item->IsExchanging() || third_item->IsEquipped())
			return;
	}
		
	ch->DoRefine_Rarity(item, second_item, third_item);
}
#endif

ACMD(do_spawnmetinacords)
{
	int prob = number(1, 10);
	char szFileName[256 + 1];
	snprintf(szFileName, sizeof(szFileName), "data/mmevent/%d.txt", prob);
	SECTREE_MAP* pkMap = SECTREE_MANAGER::instance().GetMap(ch->GetMapIndex());
	if (!pkMap)
		return;
	regen_load_in_file(szFileName, ch->GetMapIndex(), pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
}

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
ACMD(do_open_cz)
{
	int iPulse = thecore_pulse();

	if (iPulse - ch->GetLastZodiacCzLastTime() < passes_per_sec * 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}

	ch->SetLastZodiacCzLastTime(iPulse);

	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenUI12zi %d %d %d %d %d", ch->GetYellowmark(), ch->GetGreenmark(), ch->GetYellowreward(), ch->GetGreenreward(), ch->GetGoldreward());
}

ACMD(do_cz_reward)
{
	int iPulse = thecore_pulse();

	if (iPulse - ch->GetLastZodiacCzLastTime() < passes_per_sec * 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}

	ch->SetLastZodiacCzLastTime(iPulse);

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Eksik islem uygulandi!");
		return;
	}

	BYTE type = 0;
	str_to_number(type, arg1);

	if (type == 1)
	{
		if (ch->GetYellowmark() == 1073741823)
		{
			ch->AutoGiveItem(33026, 1);
			ch->SetYellowreward(1);
			ch->ClearYellowMark();
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince sarý sanduka puanin yok!");
			return;
		}
	}
	else if (type == 2)
	{
		if (ch->GetGreenmark() == 1073741823)
		{
			ch->AutoGiveItem(33027, 1);
			ch->SetGreenreward(1);
			ch->ClearGreenMark();
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince Yeþil sanduka puanin yok!");
			return;
		}
	}
	else if (type == 3)
	{
		int sari = ch->GetYellowreward();
		int yesil = ch->GetGreenreward();
		int altin = ch->GetGoldreward();

		if ((sari-altin) == 0 || (yesil-altin) == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince Yildiz sanduka puanin yok!");
			return;
		}

		ch->AutoGiveItem(33028, 1);
		ch->SetGoldreward(1);
	}

	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenUI12zi %d %d %d %d %d", ch->GetYellowmark(), ch->GetGreenmark(), ch->GetYellowreward(), ch->GetGreenreward(), ch->GetGoldreward());
}

ACMD(do_cz_check_box)
{
	if (!ch)
		return;

	int iPulse = thecore_pulse();

	if (iPulse - ch->GetLastZodiacCzLastTime() < passes_per_sec / 2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}

	ch->SetLastZodiacCzLastTime(iPulse);

	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Eksik islem uygulandi!");
		return;
	}

	BYTE type = 0, value = 0, zero = 0;
	str_to_number(type, arg1);
	str_to_number(value, arg2);

	DWORD column_item_list_yellow[] = { 33001, 33003, 33005, 33007, 33009, 33011 };
	DWORD column_item_list_green[] = { 33002, 33004, 33006, 33008, 33010, 33012 };

	DWORD row_item_list_yellow[] = { 33013, 33015, 33017, 33019, 33021 };
	DWORD row_item_list_green[] = { 33014, 33016, 33018, 33020, 33022 };

	{
		if (type == 0)
		{
			if (value == 0 || value == 6 || value == 12 || value == 18 || value == 24)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[0]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[0], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 1 || value == 7 || value == 13 || value == 19 || value == 25)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[1]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[1], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 2 || value == 8 || value == 14 || value == 20 || value == 26)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[2]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[2], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 3 || value == 9 || value == 15 || value == 21 || value == 27)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[3]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[3], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 4 || value == 10 || value == 16 || value == 22 || value == 28)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[4]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[4], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 5 || value == 11 || value == 17 || value == 23 || value == 29)
			{
				if (ch->CountSpecifyItem(column_item_list_yellow[5]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_yellow[5], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (value >= zero && value <= 5)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[0]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[0], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 6 && value <= 11)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[1]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[1], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 12 && value <= 17)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[2]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[2], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 18 && value <= 23)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[3]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[3], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 24 && value <= 29)
			{
				if (ch->CountSpecifyItem(row_item_list_yellow[4]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_yellow[4], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
		}
		else///////////////////////////////////////////////////////////////////////////////////
		{
			if (value == 0 || value == 6 || value == 12 || value == 18 || value == 24)
			{
				if (ch->CountSpecifyItem(column_item_list_green[0]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[0], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 1 || value == 7 || value == 13 || value == 19 || value == 25)
			{
				if (ch->CountSpecifyItem(column_item_list_green[1]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[1], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 2 || value == 8 || value == 14 || value == 20 || value == 26)
			{
				if (ch->CountSpecifyItem(column_item_list_green[2]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[2], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 3 || value == 9 || value == 15 || value == 21 || value == 27)
			{
				if (ch->CountSpecifyItem(column_item_list_green[3]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[3], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 4 || value == 10 || value == 16 || value == 22 || value == 28)
			{
				if (ch->CountSpecifyItem(column_item_list_green[4]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[4], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value == 5 || value == 11 || value == 17 || value == 23 || value == 29)
			{
				if (ch->CountSpecifyItem(column_item_list_green[5]) >= 50)
					ch->RemoveSpecifyItem(column_item_list_green[5], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (value >= zero && value <= 5)
			{
				if (ch->CountSpecifyItem(row_item_list_green[0]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[0], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 6 && value <= 11)
			{
				if (ch->CountSpecifyItem(row_item_list_green[1]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[1], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 12 && value <= 17)
			{
				if (ch->CountSpecifyItem(row_item_list_green[2]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[2], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 18 && value <= 23)
			{
				if (ch->CountSpecifyItem(row_item_list_green[3]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[3], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
			else if (value >= 24 && value <= 29)
			{
				if (ch->CountSpecifyItem(row_item_list_green[4]) >= 50)
					ch->RemoveSpecifyItem(row_item_list_green[4], 50);
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Yeterince zodyak tilsimin yok!");
					return;
				}
			}
		}
	}

	int size = 1;
	for (BYTE b = 0; b < value; ++b)
		size *= 2;

	if (type == 0)
		ch->SetYellowmark(size);
	else
		ch->SetGreenmark(size);

	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenUI12zi %d %d %d %d %d", ch->GetYellowmark(), ch->GetGreenmark(), ch->GetYellowreward(), ch->GetGreenreward(), ch->GetGoldreward());
}

ACMD(do_revivedialog)
{
	if (!ch)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Veri eksik!");
		return;
	}

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (!tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltecek kisi yok!");
		return;
	}

	if (!tch->IsPC())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltmeye calistiginiz kisi insan degil!");
		return;
	}

	if (!tch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltmeye calistiginiz kisi olu degil!");
		return;
	}

	BYTE Adet = tch->GetDeadCount() >= 1 ? tch->GetDeadCount()*2 : 1;
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenReviveDialog %u %u", (DWORD)tch->GetVID(), Adet);
}

ACMD(do_revive)
{
	if (!ch)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Veri eksik!");
		return;
	}

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (!tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltecek kisi yok!");
		return;
	}

	if (!tch->IsPC())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltmeye calistiginiz kisi insan degil!");
		return;
	}

	if (!tch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Diriltmeye calistiginiz kisi olu degil!");
		return;
	}

	if (!(ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000) || !(tch->GetMapIndex() >= 3580000 && tch->GetMapIndex() < 3590000))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Zodyak tapinaginda degilsiniz!");
		return;
	}

	BYTE Adet = tch->GetDeadCount() >= 1 ? tch->GetDeadCount()*2 : 1;
	if (ch->CountSpecifyItem(33025) < (int)Adet)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "NotEnoughPrism %u", Adet);
		return;
	}

	ch->RemoveSpecifyItem(33025, Adet);
	tch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
	tch->GetDesc()->SetPhase(PHASE_GAME);
	tch->SetPosition(POS_STANDING);
	tch->StartRecoveryEvent();
	tch->RestartAtSamePos();
	tch->PointChange(POINT_HP, tch->GetMaxHP() - tch->GetHP());
	tch->PointChange(POINT_SP, tch->GetMaxSP() - tch->GetSP());
	tch->ReviveInvisible(5);
	tch->CheckMount();
	sys_log(0, "do_restart: restart here zodiac");
}

ACMD(do_jump_floor)
{
	if (ch)
	{
		if ((ch->GetParty() && ch->GetParty()->GetLeaderPID() == ch->GetPlayerID()) || !ch->GetParty())
		{
			LPZODIAC pkZodiac = CZodiacManager::instance().FindByMapIndex(ch->GetMapIndex());
			if (pkZodiac && pkZodiac->IsNextFloor() == true)
			{
				pkZodiac->NewFloor(pkZodiac->GetNextFloor());
			}
		}
	}
}

ACMD(do_next_floor)
{
	if (ch)
	{
		if ((ch->GetParty() && ch->GetParty()->GetLeaderPID() == ch->GetPlayerID()) || !ch->GetParty())
		{
			LPZODIAC pkZodiac = CZodiacManager::instance().FindByMapIndex(ch->GetMapIndex());
			if (pkZodiac && pkZodiac->IsNextFloor() == true)
			{
				pkZodiac->NewFloor(pkZodiac->GetFloor()+1);
			}
		}
	}
}
#endif

ACMD(do_guild_history_req)
{
	if (!ch || !ch->GetGuild() || ch->GetGuild()->GetMasterPID() != ch->GetPlayerID())
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	char szEscapedName[1024];
	DBManager::instance().EscapeString(szEscapedName, sizeof(szEscapedName), arg1, strlen(arg1));

	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery), "SELECT guild, name FROM player.lonca_gecmis WHERE name = '%s' ORDER BY time DESC", szEscapedName);
	std::unique_ptr<SQLMsg> msg_(DBManager::instance().DirectQuery(szQuery));

	if (msg_->Get()->uiNumRows == 0)
	{
		// ch->ChatPacket(CHAT_TYPE_INFO, "The character has never joined to any guild!");
		return;
	}

	while (MYSQL_ROW row1 = mysql_fetch_row(msg_->Get()->pSQLResult))
		ch->ChatPacket(CHAT_TYPE_COMMAND, "AppendLoncaGecmisi %s %s %s", row1[0], 0, row1[2]);

	ch->ChatPacket(CHAT_TYPE_INFO, "History list completed!");
}

#ifdef ENABLE_FAST_SKILL_SELECT_SYSTEM
ACMD(do_skill_select)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->GetLevel() < 5)
		return;

	if (ch->GetSkillGroup() != 0)
		return;

	int iSkillIndex = 0;
	str_to_number(iSkillIndex, arg1);
	if (iSkillIndex == 1)
		ch->SetSkillGroup(1);
	else
		ch->SetSkillGroup(2);
	ch->ClearSkill();
	ch->SetSkillLevel(121, 20);
	ch->SetSkillLevel(137, 20);
	ch->SetSkillLevel(138, 20);
	ch->SetSkillLevel(139, 20);
	if (ch->GetJob() == JOB_ASSASSIN)
		ch->SetSkillLevel(140, 20);
	ch->SkillLevelPacket();
}
#endif

#ifdef ENABLE_DANCE_EVENT_SYSTEM
ACMD(do_gunsam_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.disable_gunsam", iValue);
	ch->UpdatePacket();
}
#endif

#ifdef ENABLE_CHEQUE_COUPON_SYSTEM
ACMD(do_use_won_ticket)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	
	if (!*arg1 || !*arg2)
		return;
	
	int itemPos = atoi(arg1);
	int inputWon = atoi(arg2);
	
	if (itemPos < 0 || itemPos > ch->GetExtendInvenMax())
		return;
	
	if (inputWon < 0)
		return;
	
	if (inputWon > ch->GetCheque())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_COUPON_ENOUGH_CHEQUE"));
		return;
	}
	
	if (inputWon > 999)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_COUPON_MAX_CHEQUE"));
		return;
	}
	
	LPITEM item = ch->GetInventoryItem(itemPos);
	
	if (!item)
		return;
	
	if (item->GetSocket(0) != 0)
		return;
	
	if (item->GetVnum() != 50027)
		return;
	
	ch->PointChange(POINT_CHEQUE, -inputWon, false);
	item->SetSocket(0, inputWon);
}
#endif

#ifdef ENABLE_PET_ATTR_DETERMINE
ACMD(do_determine_pet)
{
	if (!features::IsFeatureEnabled(features::PET))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;		
	}
	if (ch->CountSpecifyItem(55032) < 1)
		return;
	if (ch->GetNewPetSystem()->IsActivePet())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OnResultPetAttrDetermine %d", ch->GetNewPetSystem()->GetPetType());
	}
	ch->RemoveSpecifyItem(55032, 1); // @fixpch
}

ACMD(do_change_pet)
{
	if (!features::IsFeatureEnabled(features::PET))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;		
	}
	char arg1[256], arg2[256], arg3[256], arg4[256];
	four_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3), arg4, sizeof(arg4));
	
	if (!*arg1 || !*arg2 || !*arg3 || !*arg4)
		return;
	
	int firstInvenType = atoi(arg1);
	int firstInvenPos = atoi(arg2);
	int secondInvenType = atoi(arg3);
	int secondInvenPos = atoi(arg4);
	
	if (firstInvenType < 0)
		return;
	
	if (secondInvenType <0)
		return;
	
	if (firstInvenType == INVENTORY)
	{
		if (firstInvenPos >= ch->GetExtendInvenMax())
			return;
	}
	else
	{
		if (firstInvenPos >= 225)
			return;
	}
	
	if (secondInvenType == INVENTORY)
	{
		if (secondInvenPos >= ch->GetExtendInvenMax())
			return;
	}
	else
	{
		if (secondInvenPos >= 225)
			return;
	}
	
	LPITEM petItem = ch->GetItem(TItemPos(firstInvenType, firstInvenPos));
	LPITEM secondItem = ch->GetItem(TItemPos(secondInvenType, secondInvenPos));
	
	if (!petItem)
		return;
	
	if (!secondItem)
		return;
	
	if (secondItem->GetVnum() != 55033)
		return;
	
	int	m_dwbonuspet[3][2];
	
	for (int x = 0; x < 3; ++x) //Inizializzazione bonus del pet
	{
		int btype[3] = { 1, 54, 53 };
		m_dwbonuspet[x][0] = btype[x];
		m_dwbonuspet[x][1] = 0;
	}
	
	int newpettype = number(0,5);
	int level = petItem->GetSocket(1);
	int age = petItem->GetSocket(3);
	int tmpage = (time(0) + age) / 86400;
	
	if (newpettype == 0)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);;
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1+lvbonus, 5+lvbonus);
				m_dwbonuspet[2][1] += number(1+lvbonus, 5+lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1+lvbonus, 5+lvbonus);
			}
		}
	}
	else if (newpettype == 1) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(2+lvbonus, 3+lvbonus);
				m_dwbonuspet[2][1] += number(2+lvbonus, 3+lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(2+lvbonus, 3+lvbonus);
			}
		}
	}
	else if (newpettype == 2)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1+lvbonus, 7+lvbonus);
				m_dwbonuspet[2][1] += number(1+lvbonus, 7+lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1+lvbonus, 7+lvbonus);
			}
		}
	}
	else if (newpettype == 3) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(3+lvbonus, 4+lvbonus);
				m_dwbonuspet[2][1] += number(3+lvbonus, 4+lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(3+lvbonus, 4+lvbonus);
			}
		}
	}
	else if (newpettype == 4)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1+lvbonus, 9+lvbonus);
				m_dwbonuspet[2][1] += number(1+lvbonus, 9+lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1+lvbonus, 9+lvbonus);
			}
		}
	}
	else if (newpettype == 5) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(5+lvbonus, 6+lvbonus);
				m_dwbonuspet[2][1] += number(5+lvbonus, 6+lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(5+lvbonus, 6+lvbonus);
			}
		}
	}
	
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET bonus0 = '%d', bonus1 = '%d', bonus2 = '%d', pet_type = '%d' WHERE id = '%d'", m_dwbonuspet[0][1], m_dwbonuspet[1][1], m_dwbonuspet[2][1], newpettype, petItem->GetID()));
	for (int b = 0; b < 3; b++) {
		petItem->SetForceAttribute(b, 1, m_dwbonuspet[b][1]);
	}
	
	secondItem->SetCount(secondItem->GetCount()-1);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OnResultPetAttrChange %d", newpettype);
}
#endif

#ifdef ENABLE_CAPTCHA_SYSTEM
ACMD(do_captcha)
{

	char arg1[256];
	long gelensifre;
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	str_to_number(gelensifre, arg1);

	if( gelensifre == 0)
	{

		ch->GetDesc()->DelayedDisconnect(7);
		ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, LC_TEXT("%s_captcha_hata"), ch->GetName());
		return;
	}

	long captcham = ch->GetCaptcha();

	if (captcham == gelensifre)
	{
		ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, LC_TEXT("%s_captcha_dogru"), ch->GetName());
		int sans;
		if (ch->GetLevel() >= 75 && ch->GetLevel() < 90)
		{
			sans = number(3600, 3600);
		}
		else
		{
			sans = number(3600, 3600);
		}

		ch->SetQuestFlag("captcha.durum", 0);
		ch->SetQuestFlag("captcha.sure", get_global_time() + (sans));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, LC_TEXT("%s_captcha_yanlis"), ch->GetName());
		int sayi1 = number(1,9);
		int sayi2 = number(1,9);
		int sayi3 = number(1,9);
		int sayi4 = number(1,9);
		int sayi5 = number(1,9);
		long yenicaptcha = (sayi1*10000)+(sayi2*1000)+(sayi3*100)+(sayi4*10)+sayi5;
		ch->ChatPacket(CHAT_TYPE_COMMAND, "captcha %d %d %d %d %d", sayi1, sayi2, sayi3, sayi4, sayi5);
		ch->SetCaptcha(yenicaptcha);

	}
	return;
}
#endif

#ifdef ENABLE_RANGE_NPC_SYSTEM
ACMD(do_open_range_npc)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;
	
	DWORD vnum = 0;
	str_to_number(vnum, arg1);
	
	if (ch->IsDead()) 
		return;
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­¸°»óÅÂ¿¡¼­´Â »óÁ¡°Å·¡¸¦ ÇÒ¼ö °¡ ¾ø½À´Ï´Ù."));
		return;
	}

	LPSHOP shop = CShopManager::instance().Get(vnum);
	if (!shop) return;
	
	ch->SetShopOwner(ch);
	shop->AddGuest(ch, 0, false);	
}
#endif

#ifdef ENABLE_AFFECT_POLYMORPH_REMOVE
ACMD(do_remove_polymorph)
{
	if (!ch->IsPolymorphed())
		return;
	
	ch->SetPolymorph(0);
	ch->RemoveAffect(AFFECT_POLYMORPH);
}
#endif

#ifdef ENABLE_SKILL_AFFECT_REMOVE
ACMD(do_remove_skill_affect)
{
	if (!features::IsFeatureEnabled(features::REMOVE_SKILL_AFFECT))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("REMOVE_SKILL_SYSTEM_DISABLED"));
		return;		
	}
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	
	if (!*arg1)
		return;
	
	DWORD affectvnum = 0;
	str_to_number(affectvnum, arg1);
	
	if (!ch->IsAffectFlag(affectvnum))
		return;
	
	if (affectvnum != AFF_HOSIN && affectvnum != AFF_BOHO && affectvnum != AFF_GICHEON && affectvnum != AFF_KWAESOK && affectvnum != AFF_JEUNGRYEOK)
		return;
	
	CAffect* pAffect = ch->FindAffectByFlag(affectvnum);
	if (!pAffect)
		return;
	
	ch->RemoveAffect(pAffect);
}
#endif

#ifdef ENABLE_FAST_CHEQUE_GOLD_TRANSFER
ACMD(do_transfer_gold)
{
	if (!features::IsFeatureEnabled(features::TRANSFER_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("TRANSFER_SYSTEM_DISABLED"));
		return;		
	}
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	
	DWORD dwGold = 0;
	str_to_number(dwGold, arg1);
	
	dwGold *= 100000000;
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­¸°»óÅÂ¿¡¼­´Â »óÁ¡°Å·¡¸¦ ÇÒ¼ö °¡ ¾ø½À´Ï´Ù."));
		return;
	}
	
	if (dwGold > ch->GetGold())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_ENOUGH_GOLD"));
		return;
	}
	
	if (dwGold > 1999999999)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_GOLD_OVERFLOW"));
		return;
	}
	
	WORD wCheque = dwGold/100000000;
	
	if ((ch->GetCheque() + wCheque) > CHEQUE_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_MAX_CHEQUE_INVEN"));
		return;
	}
	
	ch->PointChange(POINT_GOLD, -dwGold);
	ch->PointChange(POINT_CHEQUE, wCheque);
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_SUCCESSFULL_COVERT_TO_CHEQUE"));
}

ACMD(do_transfer_cheque)
{
	if (!features::IsFeatureEnabled(features::TRANSFER_MONEY))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("TRANSFER_SYSTEM_DISABLED"));
		return;		
	}
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	
	DWORD wCheque = 0;
	str_to_number(wCheque, arg1);
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsCombOpen() || ch->isSashOpened(true) || ch->isSashOpened(false) || ch->isChangeLookOpened())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Ã¢ÀÌ ¿­¸°»óÅÂ¿¡¼­´Â »óÁ¡°Å·¡¸¦ ÇÒ¼ö °¡ ¾ø½À´Ï´Ù."));
		return;
	}
	
	if (wCheque > ch->GetCheque())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_ENOUGH_CHEQUE"));
		return;
	}
	
	if (wCheque > 999)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_CHEQUE_OVERFLOW"));
		return;
	}
	
	DWORD dwGold = wCheque*100000000;
	
	if ((ch->GetGold() + dwGold) > GOLD_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_MAX_GOLD_INVEN"));
		return;
	}
	
	ch->PointChange(POINT_CHEQUE, -wCheque);
	ch->PointChange(POINT_GOLD, dwGold);
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CHEQUE_SYSTEM_SUCCESSFULL_COVERT_TO_GOLD"));
}
#endif

#ifdef ENABLE_GUILD_RANKING_SYSTEM
ACMD(do_get_guild_rank_list)
{
	if (!features::IsFeatureEnabled(features::GUILD_RANK))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUILD_RANK_SYSTEM_DISABLED"));
		return;		
	}
	if (!ch || !ch->GetDesc())
		return;
	
	ch->SendGuildRankList();
}
#endif

#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
ACMD(do_change_special_gacha)
{
	return; // daha sonra acilacak.
	if (ch->GetSpecialGachaItemVnum() != 0)
		return;
	char buf[256];
	snprintf(buf, sizeof(buf), "%d_special_gacha", ch->GetSpecialGachaItemVnum());

	if (int(ch->GetQuestFlag(buf)) == 0)
		ch->SetQuestFlag(buf, 1000000);
	else
	{
		if (ch->GetGold() < ch->GetQuestFlag(buf))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LUCKY_BOX_ENOUGH_GOLD"));
			return;
		}
		ch->SetQuestFlag(buf, ch->GetQuestFlag(buf)*2);
		int goldvalue = (int) ch->GetQuestFlag(buf);
		ch->PointChange(POINT_GOLD, -goldvalue);
	}
	std::vector<TChestDropInfoTable> vec_ItemList;
	ITEM_MANAGER::instance().GetChestItemList(ch->GetSpecialGachaItemVnum(), vec_ItemList);
	
	if (vec_ItemList.size() == 0)
		return;
	
	DWORD dwSendItemVnum = 0;
	DWORD dwSendItemCount = 0;
	int col = 0;
	do
	{
		TChestDropInfoTable pkmem = vec_ItemList.at(col);
		if (pkmem.dwItemVnum != ch->GetSpecialGachaItemVnumGive())
		{
			int iProb = number(1,100);
			if (iProb < 35);
			{
				dwSendItemVnum = pkmem.dwItemVnum;
				dwSendItemCount = pkmem.bItemCount;
			}
		}
		col++;
	} while(dwSendItemVnum != 0);
	
	ch->SetSpecialGachaItemVnumGive(dwSendItemVnum);
	ch->SetSpecialGachaItemCountGive(dwSendItemCount);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "Binary_LuckyBoxAppend %d %d %d", dwSendItemVnum, dwSendItemCount, ch->GetQuestFlag(buf));
}

ACMD(do_get_special_gacha_item)
{
	return; // daha sonra acilacak.
	if (ch->GetSpecialGachaItemVnum() != 0)
		return;
	
	if (ch->CountSpecifyItem(ch->GetSpecialGachaItemVnum()) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LUCKY_BOX_ENOUGH_ITEM"));
		return;
	}
	
	LPITEM item = ITEM_MANAGER::instance().CreateItem(ch->GetSpecialGachaItemVnumGive(), ch->GetSpecialGachaItemCountGive(), 0, true);
	
	if (!item)
		return;
	
	ch->RemoveSpecifyItem(ch->GetSpecialGachaItemVnum(), 1);
	
	int iEmptyPos = 0;
	if (item->IsDragonSoul())
		iEmptyPos = ch->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsStone())
		iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsChest())
		iEmptyPos = ch->GetEmptyChestInventory(item->GetSize());
	else if (item->IsAttr())
		iEmptyPos = ch->GetEmptyAttrInventory(item->GetSize());
	else if (item->IsFlower())
		iEmptyPos = ch->GetEmptyFlowerInventory(item->GetSize());
#endif
	else
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());

	if (iEmptyPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("LUCKY_BOX_ENOUGH_SPACE"));
		return;
	}
	
	if (item->IsDragonSoul())
		item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (item->IsSkillBook())
		item->AddToCharacter(ch, TItemPos(SKILL_BOOK_INVENTORY, iEmptyPos));
	else if (item->IsStone())
		item->AddToCharacter(ch, TItemPos(STONE_ITEMS_INVENTORY, iEmptyPos));
	else if (item->IsUpgradeItem())
		item->AddToCharacter(ch, TItemPos(UPGRADE_ITEMS_INVENTORY, iEmptyPos));
	else if (item->IsChest())
		item->AddToCharacter(ch, TItemPos(CHEST_ITEMS_INVENTORY, iEmptyPos));
	else if (item->IsAttr())
		item->AddToCharacter(ch, TItemPos(ATTR_ITEMS_INVENTORY, iEmptyPos));
	else if (item->IsFlower())
		item->AddToCharacter(ch, TItemPos(FLOWER_ITEMS_INVENTORY, iEmptyPos));
#endif
	else
		item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
	
#ifdef ENABLE_AUTO_POTION_RENEWAL
		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(0) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = ch->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionHP())
				{
					int addvalue = item->GetValue(0) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d HP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
		else if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION && item->GetValue(1) > 0)
		{
			for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item2 = ch->GetInventoryItem(i);
				if (!item2)
					continue;

				if (item2->IsAutoPotionSP())
				{
					int addvalue = item->GetValue(1) * item->GetCount();
					item2->SetSocket(2, item2->GetSocket(2) + addvalue);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("your autopotion add %d SP"), addvalue);
					item->RemoveFromCharacter();
				}
			}
		}
#endif

	ITEM_MANAGER::instance().FlushDelayedSave(item);
}
#endif

#ifdef ENABLE_FAST_SOUL_REFINE
ACMD(do_open_soul_window)
{
	if (ch->DragonSoul_IsQualified())
		ch->DragonSoul_RefineWindow_Open(ch);
}
#endif

#ifdef ENABLE_BOSS_MANAGER_SYSTEM
ACMD(do_get_boss_data)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD dwBossVnum = 0;
	str_to_number(dwBossVnum, arg1);

	CBossManager::instance().SendBossInformation(ch, dwBossVnum); // send to client
}
#endif