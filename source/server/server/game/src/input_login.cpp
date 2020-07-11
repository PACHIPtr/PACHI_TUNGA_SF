#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "cmd.h"
#include "buffer_manager.h"
#include "protocol.h"
#include "pvp.h"
#include "start_position.h"
#include "messenger_manager.h"
#include "guild_manager.h"
#include "party.h"
#include "dungeon.h"
#include "war_map.h"
#include "questmanager.h"
#include "building.h"
#include "wedding.h"
#include "affect.h"
#include "arena.h"
#include "OXEvent.h"
#include "priv_manager.h"
#include "log.h"
#include "horsename_manager.h"
#include "MarkManager.h"
#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "MeleyLair.h"
#endif
#ifdef ENABLE_EVENT_SYSTEM
#include "game_events.h"
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
#include "minigame.h"
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif
#include "features.h"

static void _send_bonus_info(LPCHARACTER ch)
{
	int	item_drop_bonus = 0;
	int gold_drop_bonus = 0;
	int gold10_drop_bonus = 0;
	int exp_bonus = 0;

	item_drop_bonus = CPrivManager::instance().GetPriv(ch, PRIV_ITEM_DROP);
	gold_drop_bonus = CPrivManager::instance().GetPriv(ch, PRIV_GOLD_DROP);
	gold10_drop_bonus = CPrivManager::instance().GetPriv(ch, PRIV_GOLD10_DROP);
	exp_bonus = CPrivManager::instance().GetPriv(ch, PRIV_EXP_PCT);

	if (item_drop_bonus)
	{
		ch->ChatPacket(CHAT_TYPE_NOTICE,
			LC_TEXT("아이템 드롭률  %d%% 추가 이벤트 중입니다."), item_drop_bonus);
	}
	if (gold_drop_bonus)
	{
		ch->ChatPacket(CHAT_TYPE_NOTICE,
			LC_TEXT("골드 드롭률 %d%% 추가 이벤트 중입니다."), gold_drop_bonus);
	}
	if (gold10_drop_bonus)
	{
		ch->ChatPacket(CHAT_TYPE_NOTICE,
			LC_TEXT("대박골드 드롭률 %d%% 추가 이벤트 중입니다."), gold10_drop_bonus);
	}
	if (exp_bonus)
	{
		ch->ChatPacket(CHAT_TYPE_NOTICE,
			LC_TEXT("경험치 %d%% 추가 획득 이벤트 중입니다."), exp_bonus);
	}
}

static void _send_game_config(LPCHARACTER ch)
{
	char buf[512];
	snprintf(buf, sizeof(buf), "RecvGameConfig %d %d %d %d %d %d %d %d",
		(int)ch->GetQuestFlag("game_option.hide_health_board"),
		(int)ch->GetQuestFlag("game_option.hide_sash"),
		(int)ch->GetQuestFlag("game_option.hide_costume"),
		(int)ch->GetQuestFlag("game_option.hide_costume_w"),
		(int)ch->GetQuestFlag("game_option.hide_costume_h"),
		(int)ch->GetQuestFlag("game_option.disable_gunsam"),
		(int)ch->GetGMLevel(),
		(int)ch->GetQuestFlag("game_option.level_view")
	);
	ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
}

static bool FN_is_battle_zone(LPCHARACTER ch)
{
	switch (ch->GetMapIndex())
	{
	case 1:         // 신수 1차 마을
	case 2:         // 신수 2차 마을
	case 21:        // 천조 1차 마을
	case 23:        // 천조 2차 마을
	case 41:        // 진노 1차 마을
	case 43:        // 진노 2차 마을
	case 113:       // OX 맵
		return false;
	}

	return true;
}

void CInputLogin::Login(LPDESC d, const char* data)
{
	TPacketCGLogin* pinfo = (TPacketCGLogin*)data;

	char login[LOGIN_MAX_LEN + 1];
	trim_and_lower(pinfo->login, login, sizeof(login));

	sys_log(0, "InputLogin::Login : %s", login);

	TPacketGCLoginFailure failurePacket;

	if (!test_server)
	{
		failurePacket.header = HEADER_GC_LOGIN_FAILURE;
		strlcpy(failurePacket.szStatus, "VERSION", sizeof(failurePacket.szStatus));
		d->Packet(&failurePacket, sizeof(TPacketGCLoginFailure));
		return;
	}

	if (g_bNoMoreClient)
	{
		failurePacket.header = HEADER_GC_LOGIN_FAILURE;
		strlcpy(failurePacket.szStatus, "SHUTDOWN", sizeof(failurePacket.szStatus));
		d->Packet(&failurePacket, sizeof(TPacketGCLoginFailure));
		return;
	}

	if (g_iUserLimit > 0)
	{
		int iTotal;
		int* paiEmpireUserCount;
		int iLocal;

		DESC_MANAGER::instance().GetUserCount(iTotal, &paiEmpireUserCount, iLocal);

		if (g_iUserLimit <= iTotal)
		{
			failurePacket.header = HEADER_GC_LOGIN_FAILURE;
			strlcpy(failurePacket.szStatus, "FULL", sizeof(failurePacket.szStatus));
			d->Packet(&failurePacket, sizeof(TPacketGCLoginFailure));
			return;
		}
	}

	TLoginPacket login_packet;

	strlcpy(login_packet.login, login, sizeof(login_packet.login));
	strlcpy(login_packet.passwd, pinfo->passwd, sizeof(login_packet.passwd));

	db_clientdesc->DBPacket(HEADER_GD_LOGIN, d->GetHandle(), &login_packet, sizeof(TLoginPacket));
}

void CInputLogin::LoginByKey(LPDESC d, const char* data)
{
	TPacketCGLognByKey* pinfo = (TPacketCGLognByKey*)data;

	char login[LOGIN_MAX_LEN + 1];
	trim_and_lower(pinfo->login, login, sizeof(login));

	if (g_bNoMoreClient)
	{
		TPacketGCLoginFailure failurePacket;

		failurePacket.header = HEADER_GC_LOGIN_FAILURE;
		strlcpy(failurePacket.szStatus, "SHUTDOWN", sizeof(failurePacket.szStatus));
		d->Packet(&failurePacket, sizeof(TPacketGCLoginFailure));
		return;
	}

	if (g_iUserLimit > 0)
	{
		int iTotal;
		int* paiEmpireUserCount;
		int iLocal;

		DESC_MANAGER::instance().GetUserCount(iTotal, &paiEmpireUserCount, iLocal);

		if (g_iUserLimit <= iTotal)
		{
			TPacketGCLoginFailure failurePacket;

			failurePacket.header = HEADER_GC_LOGIN_FAILURE;
			strlcpy(failurePacket.szStatus, "FULL", sizeof(failurePacket.szStatus));

			d->Packet(&failurePacket, sizeof(TPacketGCLoginFailure));
			return;
		}
	}

	sys_log(0, "LOGIN_BY_KEY: %s key %u", login, pinfo->dwLoginKey);

	d->SetLoginKey(pinfo->dwLoginKey);
#ifndef _IMPROVED_PACKET_ENCRYPTION_
	d->SetSecurityKey(pinfo->adwClientKey);
#endif

	TPacketGDLoginByKey ptod;

	strlcpy(ptod.szLogin, login, sizeof(ptod.szLogin));
	ptod.dwLoginKey = pinfo->dwLoginKey;
	thecore_memcpy(ptod.adwClientKey, pinfo->adwClientKey, sizeof(DWORD) * 4);
	strlcpy(ptod.szIP, d->GetHostName(), sizeof(ptod.szIP));

	db_clientdesc->DBPacket(HEADER_GD_LOGIN_BY_KEY, d->GetHandle(), &ptod, sizeof(TPacketGDLoginByKey));
}

void CInputLogin::ChangeName(LPDESC d, const char* data)
{
	TPacketCGChangeName* p = (TPacketCGChangeName*)data;
	const TAccountTable& c_r = d->GetAccountTable();

	if (!c_r.id)
	{
		sys_err("no account table");
		return;
	}

	if (!c_r.players[p->index].bChangeName)
	{
		return;
	}

	if (!check_name(p->name))
	{
		TPacketGCCreateFailure pack;
		pack.header = HEADER_GC_CHARACTER_CREATE_FAILURE;
		pack.bType = 0;
		d->Packet(&pack, sizeof(pack));
		return;
	}

	TPacketGDChangeName pdb;

	pdb.pid = c_r.players[p->index].dwID;
	strlcpy(pdb.name, p->name, sizeof(pdb.name));
	db_clientdesc->DBPacket(HEADER_GD_CHANGE_NAME, d->GetHandle(), &pdb, sizeof(TPacketGDChangeName));
}

void CInputLogin::CharacterSelect(LPDESC d, const char* data)
{
	struct command_player_select* pinfo = (struct command_player_select*) data;
	const TAccountTable& c_r = d->GetAccountTable();

	sys_log(0, "player_select: login: %s index: %d", c_r.login, pinfo->index);

	if (!c_r.id)
	{
		sys_err("no account table");
		return;
	}

	if (pinfo->index >= PLAYER_PER_ACCOUNT)
	{
		sys_err("index overflow %d, login: %s", pinfo->index, c_r.login);
		return;
	}

	if (c_r.players[pinfo->index].bChangeName)
	{
		sys_err("name must be changed idx %d, login %s, name %s",
			pinfo->index, c_r.login, c_r.players[pinfo->index].szName);
		return;
	}

	TPlayerLoadPacket player_load_packet;

	player_load_packet.account_id = c_r.id;
	player_load_packet.player_id = c_r.players[pinfo->index].dwID;
	player_load_packet.account_index = pinfo->index;

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_LOAD, d->GetHandle(), &player_load_packet, sizeof(TPlayerLoadPacket));
}

bool RaceToJob(unsigned race, unsigned* ret_job)
{
	*ret_job = 0;

	if (race >= MAIN_RACE_MAX_NUM)
		return false;

	switch (race)
	{
	case MAIN_RACE_WARRIOR_M:
		*ret_job = JOB_WARRIOR;
		break;

	case MAIN_RACE_WARRIOR_W:
		*ret_job = JOB_WARRIOR;
		break;

	case MAIN_RACE_ASSASSIN_M:
		*ret_job = JOB_ASSASSIN;
		break;

	case MAIN_RACE_ASSASSIN_W:
		*ret_job = JOB_ASSASSIN;
		break;

	case MAIN_RACE_SURA_M:
		*ret_job = JOB_SURA;
		break;

	case MAIN_RACE_SURA_W:
		*ret_job = JOB_SURA;
		break;

	case MAIN_RACE_SHAMAN_M:
		*ret_job = JOB_SHAMAN;
		break;

	case MAIN_RACE_SHAMAN_W:
		*ret_job = JOB_SHAMAN;
		break;

	case MAIN_RACE_WOLFMAN_M:
		*ret_job = JOB_WOLFMAN;
		break;

	default:
		return false;
		break;
	}
	return true;
}

#ifdef ENABLE_BASIC_ITEM_SYSTEM
static DWORD GET_BASIC_ARMOR(BYTE race)
{
	if (race == 0 || race == 4)
		return 11209;
	else if (race == 1 || race == 5)
		return 11409;
	else if (race == 2 || race == 6)
		return 11609;
	else if (race == 3 || race == 7)
		return 11809;
	else if (race == 8)
		return 21009;

	return 0;
}
#endif

// 신규 캐릭터 지원
bool NewPlayerTable(TPlayerTable * table, const char* name, BYTE race, BYTE shape, BYTE bEmpire)
{
	if (race >= MAIN_RACE_MAX_NUM)
	{
		sys_err("NewPlayerTable.OUT_OF_RACE_RANGE(%d >= max(%d))\n", race, MAIN_RACE_MAX_NUM);
		return false;
	}

	unsigned job;

	if (!RaceToJob(race, &job))
	{
		sys_err("NewPlayerTable.RACE_TO_JOB_ERROR(%d)\n", race);
		return false;
	}

	sys_log(0, "NewPlayerTable(name=%s, race=%d, job=%d)", name, race, job);

	memset(table, 0, sizeof(TPlayerTable));

	strlcpy(table->name, name, sizeof(table->name));

	table->level = 1;
	table->job = race;	// 직업대신 종족을 넣는다
	table->voice = 0;
	table->part_base = shape;

	table->st = JobInitialPoints[job].st;
	table->dx = JobInitialPoints[job].dx;
	table->ht = JobInitialPoints[job].ht;
	table->iq = JobInitialPoints[job].iq;

	table->hp = JobInitialPoints[job].max_hp + table->ht * JobInitialPoints[job].hp_per_ht;
	table->sp = JobInitialPoints[job].max_sp + table->iq * JobInitialPoints[job].sp_per_iq;
	table->stamina = JobInitialPoints[job].max_stamina;

	table->x = CREATE_START_X(bEmpire) + number(-300, 300);
	table->y = CREATE_START_Y(bEmpire) + number(-300, 300);
	table->z = 0;
	table->dir = 0;
	table->playtime = 0;
	table->gold = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
	table->cheque = 0;
#endif
#ifdef ENABLE_GEM_SYSTEM
	table->gem = 0;
#endif

	table->skill_group = 0;

#ifdef ENABLE_BASIC_ITEM_SYSTEM
	table->parts[PART_MAIN] = GET_BASIC_ARMOR(race);
	table->horse.bLevel = 21;
	table->horse.bRiding = 0;
	table->horse.sStamina = 30;
	table->horse.sHealth = 18;
	table->skills[121].bMasterType = 1;
	table->skills[121].bLevel = 20;
	table->skills[123].bMasterType = 1;
	table->skills[123].bLevel = 20;
	table->skills[124].bMasterType = 1;
	table->skills[124].bLevel = 20;
	table->skills[129].bMasterType = 1;
	table->skills[129].bLevel = 20;
	table->skills[131].bMasterType = 1;
	table->skills[131].bLevel = 20;
	table->horse.dwHorseHealthDropTime = get_global_time();
	table->stat_point = 0;
#endif

	return true;
}

void CInputLogin::CharacterCreate(LPDESC d, const char* data)
{
	struct command_player_create* pinfo = (struct command_player_create*) data;
	TPlayerCreatePacket player_create_packet;

	sys_log(0, "PlayerCreate: name %s pos %d job %d shape %d",
		pinfo->name,
		pinfo->index,
		pinfo->job,
		pinfo->shape);

	TPacketGCLoginFailure packFailure;
	memset(&packFailure, 0, sizeof(packFailure));
	packFailure.header = HEADER_GC_CHARACTER_CREATE_FAILURE;

	if (true == g_BlockCharCreation)
	{
		d->Packet(&packFailure, sizeof(packFailure));
		return;
	}

	if (strlen(pinfo->name) > 12)
	{
		d->Packet(&packFailure, sizeof(packFailure));
		return;
	}

	if (!check_name(pinfo->name) || pinfo->shape > 1)
	{
		d->Packet(&packFailure, sizeof(packFailure));
		return;
	}
	
	const TAccountTable& c_rAccountTable = d->GetAccountTable();
	
	if (pinfo->job == JOB_WOLFMAN && !features::IsFeatureEnabled(features::WOLFMAN))
	{
		d->Packet(&packFailure, sizeof(packFailure));
		LogManager::instance().HackLog("CREATEWOLFMANCHARACTER_DISABLED", c_rAccountTable.login, pinfo->name, d->GetHostName());
		return;
	}

	if (0 == strcmp(c_rAccountTable.login, pinfo->name))
	{
		TPacketGCCreateFailure pack;
		pack.header = HEADER_GC_CHARACTER_CREATE_FAILURE;
		pack.bType = 1;

		d->Packet(&pack, sizeof(pack));
		return;
	}

	memset(&player_create_packet, 0, sizeof(TPlayerCreatePacket));

	if (!NewPlayerTable(&player_create_packet.player_table, pinfo->name, pinfo->job, pinfo->shape, d->GetEmpire()))
	{
		sys_err("player_prototype error: job %d face %d ", pinfo->job);
		d->Packet(&packFailure, sizeof(packFailure));
		return;
	}

	trim_and_lower(c_rAccountTable.login, player_create_packet.login, sizeof(player_create_packet.login));
	strlcpy(player_create_packet.passwd, c_rAccountTable.passwd, sizeof(player_create_packet.passwd));

	player_create_packet.account_id = c_rAccountTable.id;
	player_create_packet.account_index = pinfo->index;

	sys_log(0, "PlayerCreate: name %s account_id %d, TPlayerCreatePacketSize(%d), Packet->Gold %d",
		pinfo->name,
		pinfo->index,
		sizeof(TPlayerCreatePacket),
		player_create_packet.player_table.gold);

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_CREATE, d->GetHandle(), &player_create_packet, sizeof(TPlayerCreatePacket));
}

void CInputLogin::CharacterDelete(LPDESC d, const char* data)
{
	struct command_player_delete* pinfo = (struct command_player_delete*) data;
	const TAccountTable& c_rAccountTable = d->GetAccountTable();

	if (!c_rAccountTable.id)
	{
		sys_err("PlayerDelete: no login data");
		return;
	}

	sys_log(0, "PlayerDelete: login: %s index: %d, social_id %s", c_rAccountTable.login, pinfo->index, pinfo->private_code);

	if (pinfo->index >= PLAYER_PER_ACCOUNT)
	{
		sys_err("PlayerDelete: index overflow %d, login: %s", pinfo->index, c_rAccountTable.login);
		return;
	}

	if (!c_rAccountTable.players[pinfo->index].dwID)
	{
		sys_err("PlayerDelete: Wrong Social ID index %d, login: %s", pinfo->index, c_rAccountTable.login);
		d->Packet(encode_byte(HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID), 1);
		return;
	}

	TPlayerDeletePacket	player_delete_packet;

	trim_and_lower(c_rAccountTable.login, player_delete_packet.login, sizeof(player_delete_packet.login));
	player_delete_packet.player_id = c_rAccountTable.players[pinfo->index].dwID;
	player_delete_packet.account_index = pinfo->index;
	strlcpy(player_delete_packet.private_code, pinfo->private_code, sizeof(player_delete_packet.private_code));

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_DELETE, d->GetHandle(), &player_delete_packet, sizeof(TPlayerDeletePacket));
}

void CInputLogin::Entergame(LPDESC d, const char* data)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		d->SetPhase(PHASE_CLOSE);
		return;
	}

	PIXEL_POSITION pos = ch->GetXYZ();

	if (!SECTREE_MANAGER::instance().GetMovablePosition(ch->GetMapIndex(), pos.x, pos.y, pos))
	{
		PIXEL_POSITION pos2;
		SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos2);

		sys_err("!GetMovablePosition (name %s %dx%d map %d changed to %dx%d)",
			ch->GetName(),
			pos.x, pos.y,
			ch->GetMapIndex(),
			pos2.x, pos2.y);
		pos = pos2;
	}

	CGuildManager::instance().LoginMember(ch);

#ifdef ENABLE_EVENT_SYSTEM
	CGameEventsManager::instance().SendEventCharacter(ch);
#endif

	// 캐릭터를 맵에 추가
	ch->Show(ch->GetMapIndex(), pos.x, pos.y, pos.z);

	SECTREE_MANAGER::instance().SendNPCPosition(ch);
	ch->ReviveInvisible(5);

	d->SetPhase(PHASE_GAME);

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	ch->LoadPrivShops();
	ch->SendShopCost();
	ch->StartRefreshShopEvent();
#endif

#ifdef ENABLE_GIFTBOX_SYSTEM
	ch->StartRefreshGift();
#endif

	if (ch->GetItemAward_cmd())																		//게임페이즈 들어가면
		quest::CQuestManager::instance().ItemInformer(ch->GetPlayerID(), ch->GetItemAward_vnum());	//questmanager 호출

	sys_log(0, "ENTERGAME: %s %dx%dx%d %s map_index %d",
		ch->GetName(), ch->GetX(), ch->GetY(), ch->GetZ(), d->GetHostName(), ch->GetMapIndex());

	if (ch->GetHorseLevel() > 0)
	{
		ch->EnterHorse();
	}
	
#ifdef ENABLE_CAPTCHA_SYSTEM
	int durum = ch->GetQuestFlag("captcha.durum");
	if (durum == 1)
	{
		int sayi1 = number(1,9);
		int sayi2 = number(1,9);
		int sayi3 = number(1,9);
		int sayi4 = number(1,9);
		int sayi5 = number(1,9);
		long yenicaptcha = (sayi1*10000)+(sayi2*1000)+(sayi3*100)+(sayi4*10)+sayi5;
		ch->ChatPacket(CHAT_TYPE_COMMAND, "captcha %d %d %d %d %d", sayi1, sayi2, sayi3, sayi4, sayi5);
		ch->SetCaptcha(yenicaptcha);
	}
#endif

	// 플레이시간 레코딩 시작
	ch->ResetPlayTime();

	// 자동 저장 이벤트 추가
	ch->StartSaveEvent();
	ch->StartRecoveryEvent();
#ifdef ENABLE_AFK_MODE_SYSTEM
	ch->StartUpdateCharacterEvent();
#endif
#ifdef ENABLE_WALLHACK_FIX
	ch->SetLastMoveAblePosition(ch->GetXYZ());
	ch->SetLastMoveableMapIndex();

	ch->StartCheckWallhack();
#endif

	CPVPManager::instance().Connect(ch);
	CPVPManager::instance().SendList(d);

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	CCombatZoneManager::instance().OnLogin(ch);

	if (!CCombatZoneManager::instance().IsCombatZoneMap(ch->GetMapIndex()))
		CCombatZoneManager::instance().RemoveAffect(ch);
#endif

	MessengerManager::instance().Login(ch->GetName());

	CPartyManager::instance().SetParty(ch);
	CGuildManager::instance().SendGuildWar(ch);

	building::CManager::instance().SendLandList(d, ch->GetMapIndex());

	marriage::CManager::instance().Login(ch);

	TPacketGCTime p;
	p.bHeader = HEADER_GC_TIME;
	p.time = get_global_time();
	d->Packet(&p, sizeof(p));

	TPacketGCChannel p2;
	p2.header = HEADER_GC_CHANNEL;
	p2.channel = g_bChannel;
	d->Packet(&p2, sizeof(p2));

	_send_bonus_info(ch);

	for (int i = 0; i <= PREMIUM_MAX_NUM; ++i)
	{
		int remain = ch->GetPremiumRemainSeconds(i);

		if (remain <= 0)
			continue;

		ch->AddAffect(AFFECT_PREMIUM_START + i, POINT_NONE, 0, 0, remain, 0, true);
		sys_log(0, "PREMIUM: %s type %d %dmin", ch->GetName(), i, remain);
	}

	if (!d->GetClientVersion())
	{
		d->DelayedDisconnect(10);
	}
	else
	{
		if (0 != g_stClientVersion.compare(d->GetClientVersion()))
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("클라이언트 버전이 틀려 로그아웃 됩니다. 정상적으로 패치 후 접속하세요."));
			d->DelayedDisconnect(10);
			LogManager::instance().HackLog("VERSION_CONFLICT", ch);
			sys_log(0, "VERSION : WRONG VERSION USER : account:%s name:%s hostName:%s server_version:%s client_version:%s", d->GetAccountTable().login, ch->GetName(), d->GetHostName(), g_stClientVersion.c_str(), d->GetClientVersion());
		}
	}

	if (ch->IsGM() == true)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");

	if (ch->GetMapIndex() >= 10000)
	{
		if (CWarMapManager::instance().IsWarMap(ch->GetMapIndex()))
		{
			CWarMap* pMap = CWarMapManager::instance().Find(ch->GetMapIndex());
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
			if (pMap)
			{
				BYTE bIdx;
				if (ch->GetGuild() && pMap->GetTeamIndex(ch->GetGuild()->GetID(), bIdx))
				{
					// sys_err("[TEST]Current: %d Max: %d", pMap->GetCurrentPlayer(bIdx), pMap->GetMaxPlayer(bIdx));

					if (pMap->GetCurrentPlayer(bIdx) + 1 > pMap->GetMaxPlayer(bIdx)) {
						ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, LC_TEXT("oyuncu limiti dolu!"));
						ch->GoHome();
						return;
					}
				}
			}
#endif
			ch->SetWarMap(pMap);
		}
		else if (marriage::WeddingManager::instance().IsWeddingMap(ch->GetMapIndex()))
			ch->SetWeddingMap(marriage::WeddingManager::instance().Find(ch->GetMapIndex()));
#ifdef ENABLE_MELEY_LAIR_DUNGEON
		else if (MeleyLair::CMgr::instance().IsMeleyMap(ch->GetMapIndex()))
			MeleyLair::CMgr::instance().Leave(ch->GetGuild(), ch, true);
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		else if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
			ch->SetZodiac(CZodiacManager::instance().FindByMapIndex(ch->GetMapIndex()));
#endif
		else {
			ch->SetDungeon(CDungeonManager::instance().FindByMapIndex(ch->GetMapIndex()));
		}
	}
	else if (CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		int memberFlag = CArenaManager::instance().IsMember(ch->GetMapIndex(), ch->GetPlayerID());
		if (memberFlag == MEMBER_OBSERVER)
		{
			ch->SetObserverMode(true);
			ch->SetArenaObserverMode(true);
			if (CArenaManager::instance().RegisterObserverPtr(ch, ch->GetMapIndex(), ch->GetX() / 100, ch->GetY() / 100))
			{
				sys_log(0, "ARENA : Observer add failed");
			}

			if (ch->IsHorseRiding() == true)
			{
				ch->StopRiding();
				ch->HorseSummon(false);
			}
		}
		else if (memberFlag == MEMBER_DUELIST)
		{
			TPacketGCDuelStart duelStart;
			duelStart.header = HEADER_GC_DUEL_START;
			duelStart.wSize = sizeof(TPacketGCDuelStart);

			ch->GetDesc()->Packet(&duelStart, sizeof(TPacketGCDuelStart));

			if (ch->IsHorseRiding() == true)
			{
				ch->StopRiding();
				ch->HorseSummon(false);
			}

			LPPARTY pParty = ch->GetParty();
			if (pParty != nullptr)
			{
				if (pParty->GetMemberCount() == 2)
				{
					CPartyManager::instance().DeleteParty(pParty);
				}
				else
				{
					pParty->Quit(ch->GetPlayerID());
				}
			}
		}
		else if (memberFlag == MEMBER_NO)
		{
			if (ch->GetGMLevel() == GM_PLAYER)
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
		else
		{
			// wtf
		}
	}
	else if (ch->GetMapIndex() == 113)
	{
		// ox 이벤트 맵
		if (COXEventManager::instance().Enter(ch) == false)
		{
			// ox 맵 진입 허가가 나지 않음. 플레이어면 마을로 보내자
			if (ch->GetGMLevel() == GM_PLAYER)
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
	}
	else
	{
		if (CWarMapManager::instance().IsWarMap(ch->GetMapIndex()) ||
			marriage::WeddingManager::instance().IsWeddingMap(ch->GetMapIndex()))
		{
			if (!test_server)
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
	}

	if (ch->GetHorseLevel() > 0)
	{
		DWORD pid = ch->GetPlayerID();

		if (pid != 0 && CHorseNameManager::instance().GetHorseName(pid) == nullptr)
			db_clientdesc->DBPacket(HEADER_GD_REQ_HORSE_NAME, 0, &pid, sizeof(DWORD));

		ch->SetHorseLevel(ch->GetHorseLevel());
		ch->SkillLevelPacket();
	}

#ifdef ENABLE_FISH_JIGSAW_EVENT
	ch->FishEventGeneralInfo();
#endif
	if (g_noticeBattleZone)
	{
		if (FN_is_battle_zone(ch))
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("이 맵에선 강제적인 대전이 있을수 도 있습니다."));
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("이 조항에 동의하지 않을시"));
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("본인의 주성 및 부성으로 돌아가시기 바랍니다."));
		}
	}

#ifdef ENABLE_EXTENDED_BIOLOG_SYSTEM
	int biodurum = ch->GetQuestFlag("bio.durum");
	if (biodurum == 0 && ch->GetLevel() >= 30)
	{
		ch->SetQuestFlag("bio.durum", 1);
		ch->SetQuestFlag("bio.verilen", 0);
		ch->SetQuestFlag("bio.ruhtasi", 0);
		ch->SetQuestFlag("bio.kalan", get_global_time());

		int bioverilen = ch->GetQuestFlag("bio.verilen");
		int biokalan = ch->GetQuestFlag("bio.kalan");
	//	int bioruhtasi = ch->GetQuestFlag("bio.ruhtasi");
		biodurum = ch->GetQuestFlag("bio.durum");
		ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
	}
	else
	{
		int bioverilen = ch->GetQuestFlag("bio.verilen");
		int biokalan = ch->GetQuestFlag("bio.kalan");
		int bioruhtasi = ch->GetQuestFlag("bio.ruhtasi");

		if (biodurum == 31 || biodurum == 41 || biodurum == 51 || biodurum == 61 || biodurum == 71 || biodurum == 81 || biodurum == 91 || biodurum == 93 || biodurum == 11)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0");
		}
		else if (bioverilen >= BiyologSistemi[biodurum][1] && bioruhtasi == 1)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0", BiyologSistemi[biodurum][4]);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
		}
	}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (!CWarMapManager::instance().IsWarMap(ch->GetMapIndex()))
		ch->CheckMount();
#endif

	if (!CWarMapManager::instance().IsWarMap(ch->GetMapIndex()))
		ch->CheckTeleportItems();

	_send_game_config(ch);

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	ch->CzCheckBox();

	if (ch && ch->GetMapIndex() == 358)
	{
		if (DISTANCE_APPROX(ch->GetX() - 333300, ch->GetY() - 1434400) > 7000)
			ch->WarpSet(333200, 1431000, 358);
	}

	if (ch->GetMapIndex() >= 3580000 && ch->GetMapIndex() < 3590000)
	{
		LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(ch->GetMapIndex());
		if (pZodiac)
		{
			BYTE Floor = pZodiac->GetFloor();
			if (Floor != 0)
			{
				DWORD time = 0;

				if (Floor == 7 || Floor == 14 || Floor == 21 || Floor == 28 || Floor == 35 || Floor == 36 || Floor == 37 || Floor == 38 || Floor == 39 || Floor == 40)
					time = 5*60;
				else
					time = 10*60;

				ch->ZodiacFloorMessage(Floor);

				DWORD Time_1 = (get_global_time()+1) - pZodiac->GetTime();

				if (Floor == 40)
					ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacTime %d %d %d %d", Floor, 0, time, Time_1);
				else
					ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacTime %d %d %d %d", Floor, 1, time, Time_1);
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacTime %d %d %d %d", 0, 1, 10, 0);
			}
		}
		else
		{
			ch->WarpSet(333200, 1431000, 358);
		}
	}
	
	if (ch->GetLastZodiacPointReward() + 86400 < get_global_time() && ch->GetLastZodiacPointReward() != 0)
	{
		ch->PointChange(POINT_ZODIAC, 12, true);
		ch->SetLastZodiacPointReward(get_global_time());
	}
	else if (ch->GetLastZodiacPointReward() == 0)
	{
		ch->PointChange(POINT_ZODIAC, 12, true);
		ch->SetLastZodiacPointReward(get_global_time());
	}

	ch->ChatPacket(CHAT_TYPE_COMMAND, "LastZodiacPointRewardTimeUpdate %d", ch->GetLastZodiacPointReward());
#endif

	features::LoadPlayer(ch);

	if (ch->IsGM())
		ch->SetArmada();

#ifdef ENABLE_IMPROVE_DRAGON_COIN
	ch->RefreshDragonCoin();
#endif

#ifdef ENABLE_ATTENDANCE_EVENT
	CMiniGame::instance().AttendanceEventInfo(ch);
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
	CMiniGame::instance().MiniGameCatchKingEventInfo(ch);
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
	ch->LoadingMails();
	ch->SendMInfoMap();
#endif
#ifdef ENABLE_SKILL_RENEWAL
	ch->SendSkillBookCount();
#endif
}

void CInputLogin::Empire(LPDESC d, const char* c_pData)
{
	const TPacketCGEmpire* p = reinterpret_cast<const TPacketCGEmpire*>(c_pData);

	if (EMPIRE_MAX_NUM <= p->bEmpire)
	{
		d->SetPhase(PHASE_CLOSE);
		return;
	}

	const TAccountTable& r = d->GetAccountTable();

	if (r.bEmpire != 0)
	{
		for (int i = 0; i < PLAYER_PER_ACCOUNT; ++i)
		{
			if (0 != r.players[i].dwID)
			{
				sys_err("EmpireSelectFailed %d", r.players[i].dwID);
				return;
			}
		}
	}

	TEmpireSelectPacket pd;

	pd.dwAccountID = r.id;
	pd.bEmpire = p->bEmpire;

	db_clientdesc->DBPacket(HEADER_GD_EMPIRE_SELECT, d->GetHandle(), &pd, sizeof(pd));
}

int CInputLogin::GuildSymbolUpload(LPDESC d, const char* c_pData, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGGuildSymbolUpload))
		return -1;

	sys_log(0, "GuildSymbolUpload uiBytes %u", uiBytes);

	TPacketCGGuildSymbolUpload* p = (TPacketCGGuildSymbolUpload*)c_pData;

	if (uiBytes < p->size)
		return -1;

	int iSymbolSize = p->size - sizeof(TPacketCGGuildSymbolUpload);

	if (iSymbolSize <= 0 || iSymbolSize > 64 * 1024)
	{
		// 64k 보다 큰 길드 심볼은 올릴수없다
		// 접속을 끊고 무시
		d->SetPhase(PHASE_CLOSE);
		return 0;
	}

	// 땅을 소유하지 않은 길드인 경우.
	if (!test_server)
		if (!building::CManager::instance().FindLandByGuild(p->guild_id))
		{
			d->SetPhase(PHASE_CLOSE);
			return 0;
		}

	sys_log(0, "GuildSymbolUpload Do Upload %02X%02X%02X%02X %d", c_pData[7], c_pData[8], c_pData[9], c_pData[10], sizeof(*p));

	CGuildMarkManager::instance().UploadSymbol(p->guild_id, iSymbolSize, (const BYTE*)(c_pData + sizeof(*p)));
	CGuildMarkManager::instance().SaveSymbol(GUILD_SYMBOL_FILENAME);
	return iSymbolSize;
}

void CInputLogin::GuildSymbolCRC(LPDESC d, const char* c_pData)
{
	const TPacketCGSymbolCRC& CGPacket = *((TPacketCGSymbolCRC*)c_pData);

	sys_log(0, "GuildSymbolCRC %u %u %u", CGPacket.guild_id, CGPacket.crc, CGPacket.size);

	const CGuildMarkManager::TGuildSymbol* pkGS = CGuildMarkManager::instance().GetGuildSymbol(CGPacket.guild_id);

	if (!pkGS)
		return;

	sys_log(0, "  Server %u %u", pkGS->crc, pkGS->raw.size());

	if (pkGS->raw.size() != CGPacket.size || pkGS->crc != CGPacket.crc)
	{
		TPacketGCGuildSymbolData GCPacket;

		GCPacket.header = HEADER_GC_SYMBOL_DATA;
		GCPacket.size = sizeof(GCPacket) + pkGS->raw.size();
		GCPacket.guild_id = CGPacket.guild_id;

		d->BufferedPacket(&GCPacket, sizeof(GCPacket));
		d->Packet(&pkGS->raw[0], pkGS->raw.size());

		sys_log(0, "SendGuildSymbolHead %02X%02X%02X%02X Size %d",
			pkGS->raw[0], pkGS->raw[1], pkGS->raw[2], pkGS->raw[3], pkGS->raw.size());
	}
}

void CInputLogin::GuildMarkUpload(LPDESC d, const char* c_pData)
{
	TPacketCGMarkUpload* p = (TPacketCGMarkUpload*)c_pData;
	CGuildManager& rkGuildMgr = CGuildManager::instance();
	CGuild* pkGuild;

	if (!(pkGuild = rkGuildMgr.FindGuild(p->gid)))
	{
		sys_err("MARK_SERVER: GuildMarkUpload: no guild. gid %u", p->gid);
		return;
	}

	if (pkGuild->GetLevel() < guild_mark_min_level)
	{
		sys_log(0, "MARK_SERVER: GuildMarkUpload: level < %u (%u)", guild_mark_min_level, pkGuild->GetLevel());
		return;
	}

	CGuildMarkManager& rkMarkMgr = CGuildMarkManager::instance();

	sys_log(0, "MARK_SERVER: GuildMarkUpload: gid %u", p->gid);

	bool isEmpty = true;

	for (DWORD iPixel = 0; iPixel < SGuildMark::SIZE; ++iPixel)
		if (*((DWORD*)p->image + iPixel) != 0x00000000)
			isEmpty = false;

	if (isEmpty)
		rkMarkMgr.DeleteMark(p->gid);
	else
		rkMarkMgr.SaveMark(p->gid, p->image);
}

void CInputLogin::GuildMarkIDXList(LPDESC d, const char* c_pData)
{
	CGuildMarkManager& rkMarkMgr = CGuildMarkManager::instance();

	DWORD bufSize = sizeof(WORD) * 2 * rkMarkMgr.GetMarkCount();
	char* buf = nullptr;

	if (bufSize > 0)
	{
		buf = (char*)malloc(bufSize);
		rkMarkMgr.CopyMarkIdx(buf);
	}

	TPacketGCMarkIDXList p;
	p.header = HEADER_GC_MARK_IDXLIST;
	p.bufSize = sizeof(p) + bufSize;
	p.count = rkMarkMgr.GetMarkCount();

	if (buf)
	{
		d->BufferedPacket(&p, sizeof(p));
		d->LargePacket(buf, bufSize);
		free(buf);
	}
	else
		d->Packet(&p, sizeof(p));

	sys_log(0, "MARK_SERVER: GuildMarkIDXList %d bytes sent.", p.bufSize);
}

void CInputLogin::GuildMarkCRCList(LPDESC d, const char* c_pData)
{
	TPacketCGMarkCRCList* pCG = (TPacketCGMarkCRCList*)c_pData;

	std::map<BYTE, const SGuildMarkBlock*> mapDiffBlocks;
	CGuildMarkManager::instance().GetDiffBlocks(pCG->imgIdx, pCG->crclist, mapDiffBlocks);

	DWORD blockCount = 0;
	TEMP_BUFFER buf(1024 * 1024); // 1M 버퍼

	for (auto it = mapDiffBlocks.begin(); it != mapDiffBlocks.end(); ++it)
	{
		BYTE posBlock = it->first;
		const SGuildMarkBlock& rkBlock = *it->second;

		buf.write(&posBlock, sizeof(BYTE));
		buf.write(&rkBlock.m_sizeCompBuf, sizeof(DWORD));
		buf.write(rkBlock.m_abCompBuf, rkBlock.m_sizeCompBuf);

		++blockCount;
	}

	TPacketGCMarkBlock pGC;

	pGC.header = HEADER_GC_MARK_BLOCK;
	pGC.imgIdx = pCG->imgIdx;
	pGC.bufSize = buf.size() + sizeof(TPacketGCMarkBlock);
	pGC.count = blockCount;

	sys_log(0, "MARK_SERVER: Sending blocks. (imgIdx %u diff %u size %u)", pCG->imgIdx, mapDiffBlocks.size(), pGC.bufSize);

	if (buf.size() > 0)
	{
		d->BufferedPacket(&pGC, sizeof(TPacketGCMarkBlock));
		d->LargePacket(buf.read_peek(), buf.size());
	}
	else
		d->Packet(&pGC, sizeof(TPacketGCMarkBlock));
}

int CInputLogin::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)
{
	int iExtraLen = 0;

	switch (bHeader)
	{
	case HEADER_CG_PONG:
		Pong(d);
		break;

	case HEADER_CG_TIME_SYNC:
		Handshake(d, c_pData);
		break;

	case HEADER_CG_LOGIN:
		Login(d, c_pData);
		break;

	case HEADER_CG_LOGIN_KEY:
		LoginByKey(d, c_pData);
		break;

	case HEADER_CG_CHARACTER_SELECT:
		CharacterSelect(d, c_pData);
		break;

	case HEADER_CG_CHARACTER_CREATE:
		CharacterCreate(d, c_pData);
		break;

	case HEADER_CG_CHARACTER_DELETE:
		CharacterDelete(d, c_pData);
		break;

	case HEADER_CG_ENTERGAME:
		Entergame(d, c_pData);
		break;

	case HEADER_CG_EMPIRE:
		Empire(d, c_pData);
		break;

	case HEADER_CG_MOVE:
	case HEADER_CG_TARGET:
	case HEADER_CG_ATTACK:
	case HEADER_CG_SCRIPT_ANSWER:
	case HEADER_CG_ITEM_PICKUP:
		break;
		
	case HEADER_CG_MARK_LOGIN:
		break;

		///////////////////////////////////////
		// Guild Mark
		/////////////////////////////////////
	case HEADER_CG_MARK_CRCLIST:
		GuildMarkCRCList(d, c_pData);
		break;

	case HEADER_CG_MARK_IDXLIST:
		GuildMarkIDXList(d, c_pData);
		break;

	case HEADER_CG_MARK_UPLOAD:
		GuildMarkUpload(d, c_pData);
		break;

		//////////////////////////////////////
		// Guild Symbol
		/////////////////////////////////////
	case HEADER_CG_GUILD_SYMBOL_UPLOAD:
		if ((iExtraLen = GuildSymbolUpload(d, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_CG_SYMBOL_CRC:
		GuildSymbolCRC(d, c_pData);
		break;

	case HEADER_CG_CHANGE_NAME:
		ChangeName(d, c_pData);
		break;

	case HEADER_CG_CLIENT_VERSION:
		Version(d->GetCharacter(), c_pData);
		break;
	default:
		sys_err("login phase does not handle this packet! header %d", bHeader);
		//d->SetPhase(PHASE_CLOSE);
		return (0);
	}

	return (iExtraLen);
}