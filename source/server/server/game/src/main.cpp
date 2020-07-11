#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "event.h"
#include "packet.h"
#include "desc_manager.h"
#include "item_manager.h"
#include "char.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "motion.h"
#include "sectree_manager.h"
#include "shop_manager.h"
#include "regen.h"
#include "text_file_loader.h"
#include "skill.h"
#include "pvp.h"
#include "party.h"
#include "questmanager.h"
#include "profiler.h"
#include "lzo_manager.h"
#include "messenger_manager.h"
#include "db.h"
#include "log.h"
#include "p2p.h"
#include "guild_manager.h"
#include "dungeon.h"
#include "cmd.h"
#include "refine.h"
#include "banword.h"
#include "priv_manager.h"
#include "war_map.h"
#include "building.h"
#include "login_sim.h"
#include "target.h"
#include "marriage.h"
#include "wedding.h"
#include "fishing.h"
#include "item_addon.h"
#include "locale_service.h"
#include "arena.h"
#include "OXEvent.h"
#include "polymorph.h"
#include "blend_item.h"
#include "ani.h"
#include "horsename_manager.h"
#include "MarkManager.h"
#include "DragonLair.h"
#include "skill_power.h"
#include "DragonSoul.h"
#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "fstream"
#endif
#ifdef ENABLE_VERSION_162_UPDATE
#include "TempleOchao.h"
#endif
#ifdef ENABLE_LOYEIN_SVSIDE_HACKSHIELD
#include <dlfcn.h>
#endif

#ifdef USE_STACKTRACE
#include <execinfo.h>
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "MeleyLair.h"
#endif

#ifdef ENABLE_EVENT_SYSTEM
#include "game_events.h"
#endif

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif

#include "auto_event_manager.h"

#include "nearby_scanner.h"

#ifdef ENABLE_ATTENDANCE_EVENT
#include "minigame.h"
#endif

#ifdef ENABLE_BOSS_TRACKING_SYSTEM
#include "boss_tracking.h"
#endif

#include "daily_boss.h"

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

#ifdef ENABLE_AUTO_QUERY
#include "auto_query.h"
#endif

#ifdef ENABLE_AUTO_NOTICE
#include "AutoNotice.h"
#endif

// 게임과 연결되는 소켓
volatile int	num_events_called = 0;
int             max_bytes_written = 0;
int             current_bytes_written = 0;
int             total_bytes_written = 0;
BYTE		g_bLogLevel = 0;

socket_t	tcp_socket = 0;
socket_t	udp_socket = 0;
socket_t	p2p_socket = 0;

LPFDWATCH	main_fdw = nullptr;

int		io_loop(LPFDWATCH fdw);

int		start(int argc, char** argv);
int		idle();
void	destroy();

enum EProfile
{
	PROF_EVENT,
	PROF_CHR_UPDATE,
	PROF_IO,
	PROF_HEARTBEAT,
	PROF_MAX_NUM
};

static DWORD s_dwProfiler[PROF_MAX_NUM];

int g_shutdown_disconnect_pulse;
int g_shutdown_disconnect_force_pulse;
int g_shutdown_core_pulse;
bool g_bShutdown = false;

void ContinueOnFatalError()
{
	sys_err("FatalError");
}

void ShutdownOnFatalError()
{
	if (!g_bShutdown)
	{
		sys_err("ShutdownOnFatalError!!!!!!!!!!");
		{
			char buf[256];

			strlcpy(buf, LC_TEXT("서버에 치명적인 오류가 발생하여 자동으로 재부팅됩니다."), sizeof(buf));
			SendNotice(buf);
			strlcpy(buf, LC_TEXT("10초후 자동으로 접속이 종료되며,"), sizeof(buf));
			SendNotice(buf);
			strlcpy(buf, LC_TEXT("5분 후에 정상적으로 접속하실수 있습니다."), sizeof(buf));
			SendNotice(buf);
		}

		g_bShutdown = true;
		g_bNoMoreClient = true;

		g_shutdown_disconnect_pulse = thecore_pulse() + PASSES_PER_SEC(10);
		g_shutdown_disconnect_force_pulse = thecore_pulse() + PASSES_PER_SEC(20);
		g_shutdown_core_pulse = thecore_pulse() + PASSES_PER_SEC(30);
	}
}

namespace
{
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

			d->SetPhase(PHASE_CLOSE);
		}
	};
}

extern std::map<DWORD, CLoginSim*> g_sim; // first: AID
extern std::map<DWORD, CLoginSim*> g_simByPID;
extern std::vector<TPlayerTable> g_vec_save;
unsigned int save_idx = 0;

void heartbeat(LPHEART ht, int pulse)
{
	DWORD t;

	t = get_dword_time();
	num_events_called += event_process(pulse);
	s_dwProfiler[PROF_EVENT] += (get_dword_time() - t);

	t = get_dword_time();

	// 1초마다
	if (!(pulse % ht->passes_per_sec))
	{
		if (!g_bAuthServer)
		{
			TPlayerCountPacket pack;
			pack.dwCount = DESC_MANAGER::instance().GetLocalUserCount();
			db_clientdesc->DBPacket(HEADER_GD_PLAYER_COUNT, 0, &pack, sizeof(TPlayerCountPacket));
		}
		else
		{
			DESC_MANAGER::instance().ProcessExpiredLoginKey();
		}

		{
			int count = 0;
			auto it = g_sim.begin();

			while (it != g_sim.end())
			{
				if (!it->second->IsCheck())
				{
					it->second->SendLogin();

					if (++count > 50)
					{
						sys_log(0, "FLUSH_SENT");
						break;
					}
				}

				it++;
			}

			if (save_idx < g_vec_save.size())
			{
				count = MIN(100, g_vec_save.size() - save_idx);

				for (int i = 0; i < count; ++i, ++save_idx)
					db_clientdesc->DBPacket(HEADER_GD_PLAYER_SAVE, 0, &g_vec_save[save_idx], sizeof(TPlayerTable));

				sys_log(0, "SAVE_FLUSH %d", count);
			}
		}
	}

	//
	// 25 PPS(Pulse per second) 라고 가정할 때
	//

	// 약 1.16초마다
	if (!(pulse % (passes_per_sec + 4)))
		CHARACTER_MANAGER::instance().ProcessDelayedSave();

	//4초 마다

	// 약 5.08초마다
	if (!(pulse % (passes_per_sec * 5 + 2)))
	{
		ITEM_MANAGER::instance().Update();
		DESC_MANAGER::instance().UpdateLocalUserCount();
	}
#ifdef ENABLE_MINI_GAME_CATCH_KING
	if (!(pulse % (passes_per_sec)))
	{
		CMiniGame::instance().MiniGameCatchKingCheckEnd();
	}
#endif

#ifdef ENABLE_AUTO_QUERY
	// if (!(pulse % (passes_per_sec * 60 * 5)))
	// {
		// CAutoQueryManager::instance().UpdateQuestFlags();
	// }
	// if (!(pulse % (passes_per_sec * 60 * 30)))
	// {
		// CAutoQueryManager::instance().UpdateItems();
	// }
	// if (!(pulse % (passes_per_sec * 60 * 120)))
	// {
		// CAutoQueryManager::instance().OldProcess();
	// }
#endif

	s_dwProfiler[PROF_HEARTBEAT] += (get_dword_time() - t);

	DBManager::instance().Process();
	AccountDB::instance().Process();
	CPVPManager::instance().Process();

	if (g_bShutdown)
	{
		if (thecore_pulse() > g_shutdown_disconnect_pulse)
		{
			const DESC_MANAGER::DESC_SET& c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), ::SendDisconnectFunc());
			g_shutdown_disconnect_pulse = INT_MAX;
		}
		else if (thecore_pulse() > g_shutdown_disconnect_force_pulse)
		{
			const DESC_MANAGER::DESC_SET& c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), ::DisconnectFunc());
		}
		else if (thecore_pulse() > g_shutdown_disconnect_force_pulse + PASSES_PER_SEC(5))
		{
			thecore_shutdown();
		}
	}
}

int main(int argc, char** argv)
{
	ilInit(); // DevIL Initialize
	SECTREE_MANAGER	sectree_manager;
	CHARACTER_MANAGER	char_manager;
	ITEM_MANAGER	item_manager;
	CShopManager	shop_manager;

	CMobManager		mob_manager;
	CMotionManager	motion_manager;
	CPartyManager	party_manager;
	CSkillManager	skill_manager;
	CPVPManager		pvp_manager;
	LZOManager		lzo_manager;
	DBManager		db_manager;
	AccountDB 		account_db;

	LogManager		log_manager;
	MessengerManager	messenger_manager;
	P2P_MANAGER		p2p_manager;
	CGuildManager	guild_manager;
	CGuildMarkManager mark_manager;
	CDungeonManager	dungeon_manager;
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	CZodiacManager	zodiac_manager;
#endif
	CRefineManager	refine_manager;
	CBanwordManager	banword_manager;
	CPrivManager	priv_manager;
	CWarMapManager	war_map_manager;
	building::CManager	building_manager;
	CTargetManager	target_manager;
	marriage::CManager	marriage_manager;
	marriage::WeddingManager wedding_manager;
	CItemAddonManager	item_addon_manager;
	CArenaManager arena_manager;
	COXEventManager OXEvent_manager;
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	CCombatZoneManager CombatZone_manager;
#endif
	CHorseNameManager horsename_manager;

	DESC_MANAGER	desc_manager;

	CTableBySkill SkillPowerByLevel;
	CPolymorphUtils polymorph_utils;
	CProfiler		profiler;
	CDragonLairManager	dl_manager;

	DSManager dsManager;

#ifdef ENABLE_VERSION_162_UPDATE
	TempleOchao::CMgr	TempleOchao_manager;
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	MeleyLair::CMgr	MeleyLair_manager;
#endif

#ifdef ENABLE_EVENT_SYSTEM
	CGameEventsManager	gameEventsManager;
#endif

	CEventsManager	EventsManager;

	CNearbyScanner nearby_scanner;

#ifdef ENABLE_ATTENDANCE_EVENT
	CMiniGame		mini_game;
#endif

#ifdef ENABLE_BOSS_TRACKING_SYSTEM
	CBossTracking	BossTrackingManager;
#endif

	CDailyBoss DailyBossManager;
	
#ifdef ENABLE_AUTO_QUERY
	CAutoQueryManager	AutoQueryManager;
#endif

#ifdef ENABLE_AUTO_NOTICE
	CAutoNotice	AutoNoticeManager;
#endif

	if (!start(argc, argv))
		return 0;

	quest::CQuestManager quest_manager;

	if (!quest_manager.Initialize())
		return 0;

	MessengerManager::instance().Initialize();
	CGuildManager::instance().Initialize();
	fishing::Initialize();
	OXEvent_manager.Initialize();
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	CombatZone_manager.Initialize();
#endif
	Cube_init();
	Blend_Item_init();
	ani_init();

#ifdef ENABLE_GROWTH_PET_SYSTEM
	std::string temp_exp_line;
	std::ifstream exppet_table_open("/usr/game/share/locale/turkey/exppettable.txt");
	/*if (!exp_table_open.is_open())
	return 0;*/

	int exppet_table_counter = 0;
	while (!exppet_table_open.eof())
	{
		exppet_table_open >> temp_exp_line;
		str_to_number(exppet_table_common[exppet_table_counter], temp_exp_line.c_str());
		if (exppet_table_common[exppet_table_counter] < 2147483647) {
			//sys_log(0, "Loading exppettable.txt: %d !", exppet_table_common[exppet_table_counter]);
			exppet_table_counter++;
		}
		else {
			fprintf(stderr, "[main] Cannot load exppettable.txt\n");
			break;
		}
	}
#endif

	//if game server
	if (!g_bAuthServer)
	{
#ifdef ENABLE_VERSION_162_UPDATE
		TempleOchao_manager.Initialize();
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
		MeleyLair_manager.Initialize();
#endif
#ifdef ENABLE_AUTO_EVENT_SYSTEM
		EventsManager.Initialize();
#endif
#ifdef ENABLE_AUTO_QUERY
		AutoQueryManager.Initialize();
#endif
	}

	while (idle());

	sys_log(0, "<shutdown> Starting...");
	g_bShutdown = true;
	g_bNoMoreClient = true;

	if (g_bAuthServer)
	{
		int iLimit = DBManager::instance().CountQuery() / 50;
		int i = 0;

		do
		{
			DWORD dwCount = DBManager::instance().CountQuery();
			sys_log(0, "Queries %u", dwCount);

			if (dwCount == 0)
				break;

			usleep(500000);

			if (++i >= iLimit)
				if (dwCount == DBManager::instance().CountQuery())
					break;
		} while (1);
	}

	sys_log(0, "<shutdown> Destroying CArenaManager...");
	arena_manager.Destroy();
	sys_log(0, "<shutdown> Destroying COXEventManager...");
	OXEvent_manager.Destroy();

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	CombatZone_manager.Destroy();
#endif

	sys_log(0, "<shutdown> Destroying CEventsManager...");
	EventsManager.Destroy();

#ifdef ENABLE_ATTENDANCE_EVENT
	sys_log(0, "<shutdown> Destroying mini_game...");
	mini_game.Destroy();
#endif

	sys_log(0, "<shutdown> Disabling signal timer...");
	signal_timer_disable();

	sys_log(0, "<shutdown> Shutting down CHARACTER_MANAGER...");
	char_manager.GracefulShutdown();
	sys_log(0, "<shutdown> Shutting down ITEM_MANAGER...");
	item_manager.GracefulShutdown();

	sys_log(0, "<shutdown> Flushing db_clientdesc...");
	db_clientdesc->FlushOutput();
	sys_log(0, "<shutdown> Flushing p2p_manager...");
	p2p_manager.FlushOutput();

	sys_log(0, "<shutdown> Destroying CShopManager...");
	shop_manager.Destroy();
	sys_log(0, "<shutdown> Destroying CHARACTER_MANAGER...");
	char_manager.Destroy();
	sys_log(0, "<shutdown> Destroying ITEM_MANAGER...");
	item_manager.Destroy();
	sys_log(0, "<shutdown> Destroying DESC_MANAGER...");
	desc_manager.Destroy();
	sys_log(0, "<shutdown> Destroying quest::CQuestManager...");
	quest_manager.Destroy();
	sys_log(0, "<shutdown> Destroying building::CManager...");
	building_manager.Destroy();

	if (!g_bAuthServer)
	{
#ifdef ENABLE_VERSION_162_UPDATE
		sys_log(0, "<shutdown> Destroying TempleOchao_manager.");
		TempleOchao_manager.Destroy();
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
		sys_log(0, "<shutdown> Destroying MeleyLair_manager.");
		MeleyLair_manager.Destroy();
#endif
	}

	destroy();
	return 1;
}

void usage()
{
	printf("Option list\n"
		"-p <port>    : bind port number (port must be over 1024)\n"
		"-l <level>   : sets log level\n"
		"-v           : log to stdout\n"
		"-r           : do not load regen tables\n"
		"-t           : traffic profile on\n");
}

int start(int argc, char** argv)
{
#ifdef ENABLE_LOYEIN_SVSIDE_HACKSHIELD
	dlopen("/usr/game/share/bin/libsvside.so", RTLD_NOW | RTLD_GLOBAL);
#endif
	std::string st_localeServiceName;

	bool bVerbose = false;
	char ch;

	while ((ch = getopt(argc, argv, "npverltI")) != -1)
	{
		char* ep = nullptr;

		switch (ch)
		{
		case 'I': // IP
			strlcpy(g_szPublicIP, argv[optind], sizeof(g_szPublicIP));

			printf("IP %s\n", g_szPublicIP);

			optind++;
			optreset = 1;
			break;

		case 'p': // port
			mother_port = strtol(argv[optind], &ep, 10);

			if (mother_port <= 1024)
			{
				usage();
				return 0;
			}

			printf("port %d\n", mother_port);

			optind++;
			optreset = 1;
			break;

		case 'l':
		{
			long l = strtol(argv[optind], &ep, 10);

			log_set_level(l);

			optind++;
			optreset = 1;
		}
		break;

		// LOCALE_SERVICE
		case 'n':
		{
			if (optind < argc)
			{
				st_localeServiceName = argv[optind++];
				optreset = 1;
			}
		}
		break;
		// END_OF_LOCALE_SERVICE

		case 'v': // verbose
			bVerbose = true;
			break;

		case 'r':
			g_bNoRegen = true;
			break;
		}
	}

	// LOCALE_SERVICE
	config_init(st_localeServiceName);
	// END_OF_LOCALE_SERVICE

	if (!bVerbose)
		freopen("stdout", "a", stdout);

	bool is_thecore_initialized = thecore_init(25, heartbeat);

	if (!is_thecore_initialized)
	{
		fprintf(stderr, "Could not initialize thecore, check owner of pid, syslog\n");
		exit(0);
	}

	signal_timer_disable();

	main_fdw = fdwatch_new(4096);

	if ((tcp_socket = socket_tcp_bind(g_szPublicIP, mother_port)) == INVALID_SOCKET)
	{
		perror("socket_tcp_bind: tcp_socket");
		return 0;
	}

	// if internal ip exists, p2p socket uses internal ip, if not use public ip
	//if ((p2p_socket = socket_tcp_bind(*g_szInternalIP ? g_szInternalIP : g_szPublicIP, p2p_port)) == INVALID_SOCKET)
	if ((p2p_socket = socket_tcp_bind(g_szPublicIP, p2p_port)) == INVALID_SOCKET)
	{
		perror("socket_tcp_bind: p2p_socket");
		return 0;
	}

	fdwatch_add_fd(main_fdw, tcp_socket, nullptr, FDW_READ, false);
	fdwatch_add_fd(main_fdw, udp_socket, nullptr, FDW_READ, false);
	fdwatch_add_fd(main_fdw, p2p_socket, nullptr, FDW_READ, false);

	db_clientdesc = DESC_MANAGER::instance().CreateConnectionDesc(main_fdw, db_addr, db_port, PHASE_DBCLIENT, true);
	if (!g_bAuthServer) {
		db_clientdesc->UpdateChannelStatus(0, true);
	}

	if (g_bAuthServer)
	{
		if (g_stAuthMasterIP.length() != 0)
		{
			fprintf(stderr, "SlaveAuth");
			g_pkAuthMasterDesc = DESC_MANAGER::instance().CreateConnectionDesc(main_fdw, g_stAuthMasterIP.c_str(), g_wAuthMasterPort, PHASE_P2P, true);
			P2P_MANAGER::instance().RegisterConnector(g_pkAuthMasterDesc);
			g_pkAuthMasterDesc->SetP2P(g_stAuthMasterIP.c_str(), g_wAuthMasterPort, g_bChannel);
		}
	}

	signal_timer_enable(30);
	return 1;
}

void destroy()
{
	sys_log(0, "<shutdown> regen_free()...");
	regen_free();

	sys_log(0, "<shutdown> Closing sockets...");
	socket_close(tcp_socket);
	socket_close(udp_socket);
	socket_close(p2p_socket);

	sys_log(0, "<shutdown> fdwatch_delete()...");
	fdwatch_delete(main_fdw);

	sys_log(0, "<shutdown> event_destroy()...");
	event_destroy();

	sys_log(0, "<shutdown> CTextFileLoader::DestroySystem()...");
	CTextFileLoader::DestroySystem();

	sys_log(0, "<shutdown> thecore_destroy()...");
	thecore_destroy();
}

int idle()
{
	static struct timeval	pta = { 0, 0 };
	static int			process_time_count = 0;
	struct timeval		now;

	if (pta.tv_sec == 0)
		gettimeofday(&pta, (struct timezone*) 0);

	int passed_pulses;

	if (!(passed_pulses = thecore_idle()))
		return 0;

	assert(passed_pulses > 0);

	DWORD t;

	while (passed_pulses--) {
		heartbeat(thecore_heart, ++thecore_heart->pulse);

		// To reduce the possibility of abort() in checkpointing
		thecore_tick();
	}

	t = get_dword_time();
	CHARACTER_MANAGER::instance().Update(thecore_heart->pulse);
	db_clientdesc->Update(t);
	s_dwProfiler[PROF_CHR_UPDATE] += (get_dword_time() - t);

	t = get_dword_time();
	if (!io_loop(main_fdw)) return 0;
	s_dwProfiler[PROF_IO] += (get_dword_time() - t);

	log_rotate();

	gettimeofday(&now, (struct timezone*) 0);
	++process_time_count;

	if (now.tv_sec - pta.tv_sec > 0)
	{
#ifdef ENABLE_PTSYS_LOG
		pt_log("[%3d] event %5d/%-5d idle %-4ld event %-4ld heartbeat %-4ld I/O %-4ld chrUpate %-4ld | WRITE: %-7d | PULSE: %d",
			process_time_count,
			num_events_called,
			event_count(),
			thecore_profiler[PF_IDLE],
			s_dwProfiler[PROF_EVENT],
			s_dwProfiler[PROF_HEARTBEAT],
			s_dwProfiler[PROF_IO],
			s_dwProfiler[PROF_CHR_UPDATE],
			current_bytes_written,
			thecore_pulse());
#endif

		num_events_called = 0;
		current_bytes_written = 0;

		process_time_count = 0;
		gettimeofday(&pta, (struct timezone*) 0);

		memset(&thecore_profiler[0], 0, sizeof(thecore_profiler));
		memset(&s_dwProfiler[0], 0, sizeof(s_dwProfiler));
	}

	return 1;
}

int io_loop(LPFDWATCH fdw)
{
	LPDESC	d;
	int		num_events, event_idx;

	DESC_MANAGER::instance().DestroyClosed(); // PHASE_CLOSE인 접속들을 끊어준다.
	DESC_MANAGER::instance().TryConnect();

	if ((num_events = fdwatch(fdw, 0)) < 0)
		return 0;

	for (event_idx = 0; event_idx < num_events; ++event_idx)
	{
		d = (LPDESC)fdwatch_get_client_data(fdw, event_idx);

		if (!d)
		{
			if (FDW_READ == fdwatch_check_event(fdw, tcp_socket, event_idx))
			{
				DESC_MANAGER::instance().AcceptDesc(fdw, tcp_socket);
				fdwatch_clear_event(fdw, tcp_socket, event_idx);
			}
			else if (FDW_READ == fdwatch_check_event(fdw, p2p_socket, event_idx))
			{
				DESC_MANAGER::instance().AcceptP2PDesc(fdw, p2p_socket);
				fdwatch_clear_event(fdw, p2p_socket, event_idx);
			}
			/*
			else if (FDW_READ == fdwatch_check_event(fdw, udp_socket, event_idx))
			{
				char			buf[256];
				struct sockaddr_in	cliaddr;
				socklen_t		socklen = sizeof(cliaddr);

				int iBytesRead;

				if ((iBytesRead = socket_udp_read(udp_socket, buf, 256, (struct sockaddr *) &cliaddr, &socklen)) > 0)
				{
					static CInputUDP s_inputUDP;

					s_inputUDP.SetSockAddr(cliaddr);

					int iBytesProceed;
					s_inputUDP.Process(nullptr, buf, iBytesRead, iBytesProceed);
				}

				fdwatch_clear_event(fdw, udp_socket, event_idx);
			}
			*/
			continue;
		}

		int iRet = fdwatch_check_event(fdw, d->GetSocket(), event_idx);

		switch (iRet)
		{
		case FDW_READ:
			if (db_clientdesc == d)
			{
				int size = d->ProcessInput();

				if (size)
					sys_log(1, "DB_BYTES_READ: %d", size);

				if (size < 0)
				{
					d->SetPhase(PHASE_CLOSE);
				}
			}
			else if (d->ProcessInput() < 0)
			{
				d->SetPhase(PHASE_CLOSE);
			}
			break;

		case FDW_WRITE:
			if (db_clientdesc == d)
			{
				int buf_size = buffer_size(d->GetOutputBuffer());
				int sock_buf_size = fdwatch_get_buffer_size(fdw, d->GetSocket());

				int ret = d->ProcessOutput();

				if (ret < 0)
				{
					d->SetPhase(PHASE_CLOSE);
				}

				if (buf_size)
					sys_log(1, "DB_BYTES_WRITE: size %d sock_buf %d ret %d", buf_size, sock_buf_size, ret);
			}
			else if (d->ProcessOutput() < 0)
			{
				d->SetPhase(PHASE_CLOSE);
			}
			break;

		case FDW_EOF:
		{
			d->SetPhase(PHASE_CLOSE);
		}
		break;

		default:
			sys_err("fdwatch_check_event returned unknown %d", iRet);
			d->SetPhase(PHASE_CLOSE);
			break;
		}
	}

	return 1;
}