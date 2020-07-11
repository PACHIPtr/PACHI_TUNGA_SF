#include "stdafx.h"
#include <sstream>
#ifndef __WIN32__
#include <ifaddrs.h>
#endif

#include "constants.h"
#include "utils.h"
#include "log.h"
#include "desc.h"
#include "desc_manager.h"
#include "item_manager.h"
#include "p2p.h"
#include "char.h"
#include "war_map.h"
#include "locale_service.h"
#include "config.h"
#include "db.h"
#include "skill_power.h"

using std::string;

BYTE	g_bChannel = 0;
WORD	mother_port = 50080;
int		passes_per_sec = 25;
WORD	db_port = 0;
WORD	p2p_port = 50900;
char	db_addr[ADDRESS_MAX_LEN + 1];
int		save_event_second_cycle = passes_per_sec * 120;	// 3분
int		ping_event_second_cycle = passes_per_sec * 60;
bool	g_bNoMoreClient = false;
bool	g_bNoRegen = false;

int		g_iStatusPointGetLevelLimit = 90;
int		g_iStatusPointSetMaxValue = 90;
int minAddonFKS = -30;
int maxAddonFKS = 30;
int minAddonDSS = 32767;
int maxAddonDSS = 32767;
bool attr_always_add = true;
bool attr_always_5_add = true;
bool stone_always_add = false;
bool emotion_without_mask = true;
bool emotion_same_gender = false;
int taxes = 3;
int pc_max_movement_speed = 200;
int pc_max_attack_speed = 170;
int mob_max_movement_speed = 250;
int mob_max_attack_speed = 250;
bool trade_effect = true;
int trade_effect_exchange_threshold = 500000;
int trade_effect_shop_threshold = 100000000;
bool glass_enable = true;
bool glass_needed = false;
bool syserr_enable = true;
bool syslog_enable = true;
int sb_delay = 36000;
int sb_need_exp = 20000;
bool belt_allow_all_items = false;
int g_iShoutLimitLevel = 15;
int g_iShoutLimitValue = 15;
BYTE g_bItemCountLimit = 200;
bool gbTest_Enabled = true;
bool gbWolfman_Enabled = true;
bool gbWeaponRarity_Enabled = true;
bool gbPrivateShopSearch_Enabled = true;
bool gbSkillSelectEx_Enabled = true;
bool gbHairSelectEx_Enabled = true;
bool gbPointBoxes_Enabled = true;
bool gbCheque_Enabled = true;
bool gbAcce_Enabled = true;
bool gbChangeLook_Enabled = true;
bool gbCombination_Enabled = true;
bool gbSpecialInventory_Enabled = true;
bool gbGem_Enabled = true;
bool gbChannelChange_Enabled = true;
bool gbAura_Enabled = true;
bool gbPet_Enabled = true;
bool gbCostumeSet_Enabled = true;
bool gbSafeboxMoney_Enabled = true;
bool gbValuePack_Enabled = true;
bool gbBossTracking_Enabled = true;
bool gbDcCoupon_Enabled = true;
bool gbMailBox_Enabled = true;
bool gbRemoveSkillAffect_Enabled = true;
bool gbOpenGiftBox_Enabled = true;
bool gbTransferMoney_Enabled = true;
bool gbGuildRank_Enabled = true;
bool gbAveragePrice_Enabled = true;
bool gbShopMoney_Enabled = true;

int			test_server = 0;
bool		guild_mark_server = true;
BYTE		guild_mark_min_level = 3;
bool		no_wander = false;
int		g_iUserLimit = 32768;

char		g_szPublicIP[16] = "0";
char		g_szInternalIP[16] = "0";
bool		g_bSkillDisable = false;
int			g_iFullUserCount = 250;
int			g_iBusyUserCount = 1;
bool		g_bEmpireWhisper = true;
BYTE		g_bAuthServer = false;

string	g_stClientVersion = "1215955205";

string	g_stAuthMasterIP;
WORD		g_wAuthMasterPort = 0;

string g_stHostname = "";
string g_table_postfix = "";

string g_stQuestDir = "./quest";
//string g_stQuestObjectDir = "./quest/object";
string g_stDefaultQuestObjectDir = "./quest/object";
std::set<string> g_setQuestObjectDir;

std::vector<std::string>	g_stAdminPageIP;
std::string	g_stAdminPagePassword = "SHOWMETHEMONEY";

string g_stBlockDate = "30000705";

extern string g_stLocale;

//시야 = VIEW_RANGE + VIEW_BONUS_RANGE
//VIEW_BONUSE_RANGE : 클라이언트와 시야 처리에서너무 딱 떨어질경우 문제가 발생할수있어 500CM의 여분을 항상준다.
int VIEW_RANGE = 5000;
int VIEW_BONUS_RANGE = 500;

int g_server_id = 0;
string g_strWebMallURL = "myte2.com/ishop";

void		LoadStateUserCount();
bool		LoadClientVersion();
bool            g_protectNormalPlayer = false;        // 범법자가 "평화모드" 인 일반유저를 공격하지 못함
bool            g_noticeBattleZone = false;        // 중립지대에 입장하면 안내메세지를 알려줌

int gPlayerMaxLevel = 120;

bool g_BlockCharCreation = false;

bool is_string_true(const char* string)
{
	bool	result = 0;
	if (isnhdigit(*string))
	{
		str_to_number(result, string);
		return result > 0 ? true : false;
	}
	else if (LOWER(*string) == 't')
		return true;
	else
		return false;
}

static std::set<int> s_set_map_allows;

bool map_allow_find(int index)
{
	if (g_bAuthServer)
		return false;

	if (s_set_map_allows.find(index) == s_set_map_allows.end())
		return false;

	return true;
}

void map_allow_log()
{
	std::set<int>::iterator i;

	for (i = s_set_map_allows.begin(); i != s_set_map_allows.end(); ++i)
		sys_log(0, "MAP_ALLOW: %d", *i);
}

void map_allow_add(int index)
{
	if (map_allow_find(index) == true)
	{
		fprintf(stdout, "!!! FATAL ERROR !!! multiple MAP_ALLOW setting!!\n");
		exit(1);
	}

	s_set_map_allows.insert(index);
}

void map_allow_copy(long* pl, int size)
{
	int iCount = 0;
	std::set<int>::iterator it = s_set_map_allows.begin();

	while (it != s_set_map_allows.end())
	{
		int i = *(it++);
		*(pl++) = i;

		if (++iCount > size)
			break;
	}
}

static void FN_add_adminpageIP(char* line)
{
	char* last;
	const char* delim = " \t\r\n";
	char* v = strtok_r(line, delim, &last);

	while (v)
	{
		g_stAdminPageIP.push_back(v);
		v = strtok_r(nullptr, delim, &last);
	}
}

static void FN_log_adminpage()
{
	auto iter = g_stAdminPageIP.begin();

	while (iter != g_stAdminPageIP.end())
	{
		++iter;
	}
}

bool GetIPInfo()
{
	struct ifaddrs* ifaddrp = nullptr;

	if (0 != getifaddrs(&ifaddrp))
		return false;

	for (struct ifaddrs* ifap = ifaddrp; nullptr != ifap; ifap = ifap->ifa_next)
	{
		struct sockaddr_in* sai = (struct sockaddr_in*) ifap->ifa_addr;

		if (!ifap->ifa_netmask ||  // ignore if no netmask
			sai->sin_addr.s_addr == 0 || // ignore if address is 0.0.0.0
			sai->sin_addr.s_addr == 16777343) // ignore if address is 127.0.0.1
			continue;

		char* netip = inet_ntoa(sai->sin_addr);

		if (!strncmp(netip, "10.", 3))
		{
			strlcpy(g_szInternalIP, netip, sizeof(g_szInternalIP));
		}
		else if (g_szPublicIP[0] == '0')
		{
			strlcpy(g_szPublicIP, netip, sizeof(g_szPublicIP));
		}
	}
	freeifaddrs(ifaddrp);

	if (g_szPublicIP[0] != '0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
std::map<DWORD, DWORD> g_ShopIndexCount;
std::map<int, TShopCost> g_ShopCosts;
#include <boost/algorithm/string.hpp>
void LoadShopConfig()
{
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT `id`,`time`,`time_type`+0,`cost` from shop_cost"));
	SQLResult* pRes = pkMsg->Get();
	if (pRes->uiNumRows)
	{
		MYSQL_ROW row;
		g_ShopIndexCount.clear();
		while ((row = mysql_fetch_row(pRes->pSQLResult)))
		{
			int id;
			str_to_number(id, row[0]);
			TShopCost c;
			str_to_number(c.days, row[1]);
			BYTE type;
			str_to_number(type, row[2]);
			if (type == 1)
				c.time = 86400;
			else if (type == 2)
				c.time = 3600;
			else
				c.time = 0;
			str_to_number(c.price, row[3]);
			g_ShopCosts[id] = c;
		}
	}
	const DESC_MANAGER::DESC_SET& set = DESC_MANAGER::instance().GetClientSet();
	DESC_MANAGER::DESC_SET::const_iterator it = set.begin();

	while (it != set.end())
	{
		LPDESC d = *(it++);

		if (!d->GetCharacter())
			continue;
		d->GetCharacter()->SendShopCost();
	}
}
void LoadIndexShopLimit()
{
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT map_index,shop_limit from `shop_limit` where channel=%d or channel=0", g_bChannel));
	SQLResult* pRes = pkMsg->Get();
	if (pRes->uiNumRows)
	{
		MYSQL_ROW row;
		g_ShopIndexCount.clear();
		while ((row = mysql_fetch_row(pRes->pSQLResult)))
		{
			int index, limit;
			str_to_number(index, row[0]);
			str_to_number(limit, row[1]);
			if (!map_allow_find(index))
				continue;
			g_ShopIndexCount.insert(std::make_pair(index, limit));
			sys_log(0, "Shop Map %d limit %d\n", index, limit);
		}
	}
	fprintf(stdout, "\n");
}
#endif

void config_init(const string & st_localeServiceName)
{
	FILE* fp;

	char	buf[256];
	char	token_string[256];
	char	value_string[256];

	// LOCALE_SERVICE
	string	st_configFileName;

	st_configFileName.reserve(32);
	st_configFileName = "CONFIG";

	if (!st_localeServiceName.empty())
	{
		st_configFileName += ".";
		st_configFileName += st_localeServiceName;
	}
	// END_OF_LOCALE_SERVICE

	if (!(fp = fopen(st_configFileName.c_str(), "r")))
	{
		fprintf(stderr, "Can not open [%s]\n", st_configFileName.c_str());
		exit(1);
	}

	if (!GetIPInfo())
	{
		fprintf(stderr, "Can not get public ip address\n");
		exit(1);
	}
	
	char db_host[2][64], db_user[2][64], db_pwd[2][64], db_db[2][64];
	int mysql_db_port[2];

	for (int n = 0; n < 2; ++n)
	{
		*db_host[n] = '\0';
		*db_user[n] = '\0';
		*db_pwd[n] = '\0';
		*db_db[n] = '\0';
		mysql_db_port[n] = 0;
	}

	char log_host[64], log_user[64], log_pwd[64], log_db[64];
	int log_port = 0;

	*log_host = '\0';
	*log_user = '\0';
	*log_pwd = '\0';
	*log_db = '\0';

	// DB에서 로케일정보를 세팅하기위해서는 다른 세팅값보다 선행되어서
	// DB정보만 읽어와 로케일 세팅을 한후 다른 세팅을 적용시켜야한다.
	// 이유는 로케일관련된 초기화 루틴이 곳곳에 존재하기 때문.

	bool isCommonSQL = false;
	bool isPlayerSQL = false;

	FILE* fpOnlyForDB;

	if (!(fpOnlyForDB = fopen(st_configFileName.c_str(), "r")))
	{
		fprintf(stderr, "Can not open [%s]\n", st_configFileName.c_str());
		exit(1);
	}

	while (fgets(buf, 256, fpOnlyForDB))
	{
		parse_token(buf, token_string, value_string);

		TOKEN("BLOCK_LOGIN")
		{
			g_stBlockDate = value_string;
		}

		TOKEN("adminpage_ip")
		{
			FN_add_adminpageIP(value_string);
			//g_stAdminPageIP[0] = value_string;
		}

		TOKEN("adminpage_ip1")
		{
			FN_add_adminpageIP(value_string);
			//g_stAdminPageIP[0] = value_string;
		}

		TOKEN("adminpage_ip2")
		{
			FN_add_adminpageIP(value_string);
			//g_stAdminPageIP[1] = value_string;
		}

		TOKEN("adminpage_ip3")
		{
			FN_add_adminpageIP(value_string);
			//g_stAdminPageIP[2] = value_string;
		}

		TOKEN("adminpage_password")
		{
			g_stAdminPagePassword = value_string;
		}

		TOKEN("hostname")
		{
			g_stHostname = value_string;
			fprintf(stdout, "HOSTNAME: %s\n", g_stHostname.c_str());
			continue;
		}

		TOKEN("channel")
		{
			str_to_number(g_bChannel, value_string);
			continue;
		}

		TOKEN("player_sql")
		{
			const char* line = two_arguments(value_string, db_host[0], sizeof(db_host[0]), db_user[0], sizeof(db_user[0]));
			line = two_arguments(line, db_pwd[0], sizeof(db_pwd[0]), db_db[0], sizeof(db_db[0]));

			if ('\0' != line[0])
			{
				char buf[256];
				one_argument(line, buf, sizeof(buf));
				str_to_number(mysql_db_port[0], buf);
			}

			if (!*db_host[0] || !*db_user[0] || !*db_pwd[0] || !*db_db[0])
			{
				fprintf(stderr, "PLAYER_SQL syntax: logsql <host user password db>\n");
				exit(1);
			}

			char buf[1024];
			snprintf(buf, sizeof(buf), "PLAYER_SQL: %s %s %s %s %d", db_host[0], db_user[0], db_pwd[0], db_db[0], mysql_db_port[0]);
			isPlayerSQL = true;
			continue;
		}

		TOKEN("common_sql")
		{
			const char* line = two_arguments(value_string, db_host[1], sizeof(db_host[1]), db_user[1], sizeof(db_user[1]));
			line = two_arguments(line, db_pwd[1], sizeof(db_pwd[1]), db_db[1], sizeof(db_db[1]));

			if ('\0' != line[0])
			{
				char buf[256];
				one_argument(line, buf, sizeof(buf));
				str_to_number(mysql_db_port[1], buf);
			}

			if (!*db_host[1] || !*db_user[1] || !*db_pwd[1] || !*db_db[1])
			{
				fprintf(stderr, "COMMON_SQL syntax: logsql <host user password db>\n");
				exit(1);
			}

			char buf[1024];
			snprintf(buf, sizeof(buf), "COMMON_SQL: %s %s %s %s %d", db_host[1], db_user[1], db_pwd[1], db_db[1], mysql_db_port[1]);
			isCommonSQL = true;
			continue;
		}

		TOKEN("log_sql")
		{
			const char* line = two_arguments(value_string, log_host, sizeof(log_host), log_user, sizeof(log_user));
			line = two_arguments(line, log_pwd, sizeof(log_pwd), log_db, sizeof(log_db));

			if ('\0' != line[0])
			{
				char buf[256];
				one_argument(line, buf, sizeof(buf));
				str_to_number(log_port, buf);
			}

			if (!*log_host || !*log_user || !*log_pwd || !*log_db)
			{
				fprintf(stderr, "LOG_SQL syntax: logsql <host user password db>\n");
				exit(1);
			}

			char buf[1024];
			snprintf(buf, sizeof(buf), "LOG_SQL: %s %s %s %s %d", log_host, log_user, log_pwd, log_db, log_port);
			continue;
		}
	}

	//처리가 끝났으니 파일을 닫자.
	fclose(fpOnlyForDB);

	// CONFIG_SQL_INFO_ERROR
	if (!isCommonSQL)
	{
		puts("LOAD_COMMON_SQL_INFO_FAILURE:");
		puts("");
		puts("CONFIG:");
		puts("------------------------------------------------");
		puts("COMMON_SQL: HOST USER PASSWORD DATABASE");
		puts("");
		exit(1);
	}

	if (!isPlayerSQL)
	{
		puts("LOAD_PLAYER_SQL_INFO_FAILURE:");
		puts("");
		puts("CONFIG:");
		puts("------------------------------------------------");
		puts("PLAYER_SQL: HOST USER PASSWORD DATABASE");
		puts("");
		exit(1);
	}

	// Common DB 가 Locale 정보를 가지고 있기 때문에 가장 먼저 접속해야 한다.
	AccountDB::instance().Connect(db_host[1], mysql_db_port[1], db_user[1], db_pwd[1], db_db[1]);

	if (false == AccountDB::instance().IsConnected())
	{
		fprintf(stderr, "cannot start server while no common sql connected\n");
		exit(1);
	}

	fprintf(stdout, "Common connected\n");

	// 로케일 정보를 가져오자
	// <경고> 쿼리문에 절대 조건문(WHERE) 달지 마세요. (다른 지역에서 문제가 생길수 있습니다)
	{
		char szQuery[512];
		snprintf(szQuery, sizeof(szQuery), "SELECT mKey, mValue FROM locale");

		std::unique_ptr<SQLMsg> pMsg(AccountDB::instance().DirectQuery(szQuery));

		if (pMsg->Get()->uiNumRows == 0)
		{
			fprintf(stderr, "COMMON_SQL: DirectQuery failed : %s\n", szQuery);
			exit(1);
		}

		MYSQL_ROW row;

		while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			// 로케일 세팅
			if (strcasecmp(row[0], "LOCALE") == 0)
			{
				if (LocaleService_Init(row[1]) == false)
				{
					fprintf(stderr, "COMMON_SQL: invalid locale key %s\n", row[1]);
					exit(1);
				}
			}
		}
	}

	AccountDB::instance().SetLocale(g_stLocale);

	AccountDB::instance().ConnectAsync(db_host[1], mysql_db_port[1], db_user[1], db_pwd[1], db_db[1], g_stLocale.c_str());

	// Player DB 접속
	DBManager::instance().Connect(db_host[0], mysql_db_port[0], db_user[0], db_pwd[0], db_db[0]);

	if (!DBManager::instance().IsConnected())
	{
		fprintf(stderr, "PlayerSQL.ConnectError\n");
		exit(1);
	}

	fprintf(stdout, "Player connected\n");

	if (false == g_bAuthServer) // 인증 서버가 아닐 경우
	{
		// Log DB 접속
		LogManager::instance().Connect(log_host, log_port, log_user, log_pwd, log_db);

		if (!LogManager::instance().IsConnected())
		{
			fprintf(stderr, "LogSQL.ConnectError\n");
			exit(1);
		}

		fprintf(stdout, "Log connected\n");

		LogManager::instance().BootLog(g_stHostname.c_str(), g_bChannel);
	}

	// SKILL_POWER_BY_LEVEL
	// 스트링 비교의 문제로 인해서 AccountDB::instance().SetLocale(g_stLocale) 후부터 한다.
	// 물론 국내는 별로 문제가 안된다(해외가 문제)
	{
		char szQuery[256];
		snprintf(szQuery, sizeof(szQuery), "SELECT mValue FROM locale WHERE mKey='SKILL_POWER_BY_LEVEL'");
		std::unique_ptr<SQLMsg> pMsg(AccountDB::instance().DirectQuery(szQuery));

		if (pMsg->Get()->uiNumRows == 0)
		{
			fprintf(stderr, "[SKILL_PERCENT] Query failed: %s", szQuery);
			exit(1);
		}

		MYSQL_ROW row;

		row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		const char* p = row[0];
		int cnt = 0;
		char num[128];
		int aiBaseSkillPowerByLevelTable[SKILL_MAX_LEVEL + 1];

		//fprintf(stdout, "SKILL_POWER_BY_LEVEL %s\n", p);
		while (*p != '\0' && cnt < (SKILL_MAX_LEVEL + 1))
		{
			p = one_argument(p, num, sizeof(num));
			aiBaseSkillPowerByLevelTable[cnt++] = atoi(num);

			//fprintf(stdout, "%d %d\n", cnt - 1, aiBaseSkillPowerByLevelTable[cnt - 1]);
			if (*p == '\0')
			{
				if (cnt != (SKILL_MAX_LEVEL + 1))
				{
					fprintf(stderr, "[SKILL_PERCENT] locale table has not enough skill information! (count: %d query: %s)", cnt, szQuery);
					exit(1);
				}
				break;
			}
		}

		for (int job = 0; job < JOB_MAX_NUM * 2; ++job)
		{
			snprintf(szQuery, sizeof(szQuery), "SELECT mValue from locale where mKey='SKILL_POWER_BY_LEVEL_TYPE%d' ORDER BY CAST(mValue AS unsigned)", job);
			std::unique_ptr<SQLMsg> pMsg(AccountDB::instance().DirectQuery(szQuery));

			// 세팅이 안되어있으면 기본테이블을 사용한다.
			if (pMsg->Get()->uiNumRows == 0)
			{
				CTableBySkill::instance().SetSkillPowerByLevelFromType(job, aiBaseSkillPowerByLevelTable);
				continue;
			}

			row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			cnt = 0;
			p = row[0];
			int aiSkillTable[SKILL_MAX_LEVEL + 1];

			//fprintf(stdout, "SKILL_POWER_BY_JOB %d %s\n", job, p);
			while (*p != '\0' && cnt < (SKILL_MAX_LEVEL + 1))
			{
				p = one_argument(p, num, sizeof(num));
				aiSkillTable[cnt++] = atoi(num);

				//fprintf(stdout, "%d %d\n", cnt - 1, aiBaseSkillPowerByLevelTable[cnt - 1]);
				if (*p == '\0')
				{
					if (cnt != (SKILL_MAX_LEVEL + 1))
					{
						fprintf(stderr, "[SKILL_PERCENT] locale table has not enough skill information! (count: %d query: %s)", cnt, szQuery);
						exit(1);
					}
					break;
				}
			}

			CTableBySkill::instance().SetSkillPowerByLevelFromType(job, aiSkillTable);
		}
	}
	// END_SKILL_POWER_BY_LEVEL

	// LOG_KEEP_DAYS_EXTEND
	log_set_expiration_days(2);
	// END_OF_LOG_KEEP_DAYS_EXTEND

	while (fgets(buf, 256, fp))
	{
		parse_token(buf, token_string, value_string);

		TOKEN("empire_whisper")
		{
			bool b_value = 0;
			str_to_number(b_value, value_string);
			g_bEmpireWhisper = !!b_value;
			continue;
		}

		TOKEN("mark_server")
		{
			guild_mark_server = is_string_true(value_string);
			continue;
		}

		TOKEN("mark_min_level")
		{
			str_to_number(guild_mark_min_level, value_string);
			guild_mark_min_level = MINMAX(0, guild_mark_min_level, GUILD_MAX_LEVEL);
			continue;
		}

		TOKEN("port")
		{
			str_to_number(mother_port, value_string);
			continue;
		}

		TOKEN("log_keep_days")
		{
			int i = 0;
			str_to_number(i, value_string);
			log_set_expiration_days(MINMAX(1, i, 90));
			continue;
		}

		TOKEN("passes_per_sec")
		{
			str_to_number(passes_per_sec, value_string);
			continue;
		}

		TOKEN("p2p_port")
		{
			str_to_number(p2p_port, value_string);
			continue;
		}

		TOKEN("db_port")
		{
			str_to_number(db_port, value_string);
			continue;
		}

		TOKEN("db_addr")
		{
			strlcpy(db_addr, value_string, sizeof(db_addr));

			for (int n = 0; n < ADDRESS_MAX_LEN; ++n)
			{
				if (db_addr[n] == ' ')
					db_addr[n] = '\0';
			}

			continue;
		}

		TOKEN("save_event_second_cycle")
		{
			int	cycle = 0;
			str_to_number(cycle, value_string);
			save_event_second_cycle = cycle * passes_per_sec;
			continue;
		}

		TOKEN("ping_event_second_cycle")
		{
			int	cycle = 0;
			str_to_number(cycle, value_string);
			ping_event_second_cycle = cycle * passes_per_sec;
			continue;
		}

		TOKEN("table_postfix")
		{
			g_table_postfix = value_string;
			continue;
		}

		TOKEN("test_server")
		{
			printf("-----------------------------------------------\n");
			printf("-------------------- Tunga --------------------\n");
			printf("-----------------------------------------------\n");
			str_to_number(test_server, value_string);
			continue;
		}

		TOKEN("shutdowned")
		{
			g_bNoMoreClient = true;
			continue;
		}

		TOKEN("no_regen")
		{
			g_bNoRegen = true;
			continue;
		}

		TOKEN("status_point_get_level_limit")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (flag <= 0) continue;

			g_iStatusPointGetLevelLimit = MINMAX(0, flag, PLAYER_MAX_LEVEL_CONST);
			fprintf(stdout, "STATUS_POINT_GET_LEVEL_LIMIT: %d\n", g_iStatusPointGetLevelLimit);
			continue;
		}

		TOKEN("status_point_set_max_value")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (flag <= 0) continue;

			g_iStatusPointSetMaxValue = flag;
			fprintf(stdout, "STATUS_POINT_SET_MAX_VALUE: %d\n", g_iStatusPointSetMaxValue);
			continue;
		}
		
		TOKEN("min_addon_fks")
		{
			str_to_number(minAddonFKS, value_string);
			minAddonFKS = MINMAX(-32767, minAddonFKS, 32767);
			fprintf(stderr, "MIN_ADDON_FKS: %d\n", minAddonFKS);
		}

		TOKEN("max_addon_fks")
		{
			str_to_number(maxAddonFKS, value_string);
			maxAddonFKS = MINMAX(-32767, maxAddonFKS, 32767);
			fprintf(stderr, "MAX_ADDON_FKS: %d\n", maxAddonFKS);
		}

		TOKEN("min_addon_dss")
		{
			str_to_number(minAddonDSS, value_string);
			minAddonDSS = MINMAX(-32767, minAddonDSS, 32766);
			fprintf(stderr, "MIN_ADDON_DSS: %d\n", maxAddonDSS);
		}

		TOKEN("max_addon_dss")
		{
			str_to_number(maxAddonDSS, value_string);
			maxAddonDSS = MINMAX(-32767, maxAddonDSS, 32766);
			fprintf(stderr, "MAX_ADDON_DSS: %d\n", maxAddonDSS);
		}
		
		TOKEN("attr_always_add")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "ATTR_ALWAYS_ADD: %d\n", flag);
			//if (1 == flag && LC_IsEurope() )
			if (1 == flag)
			{
				attr_always_add = true;
			}
		}

		TOKEN("attr_always_5_add")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "ATTR_ALWAYS_5_ADD: %d\n", flag);
			//if (1 == flag && LC_IsEurope() )
			if (1 == flag)
			{
				attr_always_5_add = true;
			}
		}
		
		TOKEN("stone_always_add")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "STONE_ALWAYS_ADD: %d\n", flag);
			//if (1 == flag && LC_IsEurope() )
			if (1 == flag)
			{
				stone_always_add = true;
			}
		}

		TOKEN("emotion_without_mask")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (1 == flag)
			{
				emotion_without_mask = true;
				fprintf(stderr, "EMOTION_WITHOUT_MASK: %i\n", flag);
			}
		}
		
		TOKEN("emotion_same_gender")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (1 == flag)
			{
				emotion_same_gender = true;
				fprintf(stderr, "EMOTION_SAME_GENDER: %i\n", flag);
			}
		}
		
		TOKEN("taxes")
		{
			str_to_number(taxes, value_string);
			taxes = MINMAX(0, taxes, 100);
			fprintf(stderr, "TAXES: %d\n", taxes);
		}
		
		TOKEN("pc_max_movement_speed")
		{
			str_to_number(pc_max_movement_speed, value_string);
			pc_max_movement_speed = MINMAX(0, pc_max_movement_speed, 2147483646);
			fprintf(stderr, "PC_MAX_MOVEMENT_SPEED: %d\n", pc_max_movement_speed);
		}
		
		TOKEN("pc_max_attack_speed")
		{
			str_to_number(pc_max_attack_speed, value_string);
			pc_max_attack_speed = MINMAX(0, pc_max_attack_speed, 2147483646);
			fprintf(stderr, "PC_MAX_ATTACK_SPEED: %d\n", pc_max_attack_speed);
		}
		
		TOKEN("mob_max_movement_speed")
		{
			str_to_number(mob_max_movement_speed, value_string);
			mob_max_movement_speed = MINMAX(0, mob_max_movement_speed, 2147483646);
			fprintf(stderr, "MOB_MAX_MOVEMENT_SPEED: %d\n", mob_max_movement_speed);
		}
		
		TOKEN("mob_max_attack_speed")
		{
			str_to_number(mob_max_attack_speed, value_string);
			mob_max_attack_speed = MINMAX(0, mob_max_attack_speed, 2147483646);
			fprintf(stderr, "MOB_MAX_ATTACK_SPEED: %d\n", mob_max_attack_speed);
		}
		
		TOKEN("trade_effect")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "TRADE_EFFECT: %d\n", flag);
			if (0 == flag)
			{
				trade_effect = false;
			}
		}

		TOKEN("trade_effect_shop_threshold")
		{
			str_to_number(trade_effect_shop_threshold, value_string);
			trade_effect_shop_threshold = MINMAX(0, trade_effect_shop_threshold, 1000000000);
			fprintf(stderr, "TRADE_EFFECT_SHOP_THRESHOLD: %d\n", trade_effect_shop_threshold);
		}

		TOKEN("trade_effect_exchange_threshold")
		{
			str_to_number(trade_effect_exchange_threshold, value_string);
			trade_effect_exchange_threshold = MINMAX(0, trade_effect_exchange_threshold, 1000000000);
			fprintf(stderr, "TRADE_EFFECT_EXCHANGE_THRESHOLD: %d\n", trade_effect_exchange_threshold);
		}
		
		TOKEN("glass_enable")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "GLASS_ENABLE: %d\n", flag);
			if (1 == flag)
			{
				glass_enable = true;
			}
		}
		
		TOKEN("glass_needed")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "GLASS_NEEDED: %d\n", flag);
			if (0 == flag)
			{
				glass_needed = false;
			}
		}
		
		TOKEN("syserr_enable")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "SYSERR_ENABLE: %d\n", flag);
			if (0 == flag)
			{
				syserr_enable = false;
			}
		}

		TOKEN("syslog_enable")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			fprintf(stderr, "SYSLOG_ENABLE: %d\n", flag);
			if (1 == flag)
			{
				syslog_enable = true;
			}
		}
		
		TOKEN("skillbook_delay")
		{
			str_to_number(sb_delay, value_string);
			sb_delay = MINMAX(0, sb_delay, 2147483646);
			fprintf(stderr, "SKILLBOOK_DELAY: %d\n", sb_delay);
		}

		TOKEN("skillbook_need_exp")
		{
			str_to_number(sb_need_exp, value_string);
			sb_need_exp = MINMAX(0, sb_need_exp, 2147483646);
			fprintf(stderr, "SKILLBOOK_NEED_EXP: %d\n", sb_need_exp);
		}
		
		TOKEN("belt_allow_all_items")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (1 == flag)
			{
				belt_allow_all_items = true;
				fprintf(stderr, "BELT_ALLOW_ALL_ITEMS: %i\n", flag);
			}
		}
		
		TOKEN("shout_limit_level")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (flag <= 0) continue;
			g_iShoutLimitLevel = flag;
			fprintf(stdout, "SHOUT_LIMIT_LEVEL: %d\n", g_iShoutLimitLevel);
			continue;
		}
		
		TOKEN("shout_limit_value")
		{
			int flag = 0;
			str_to_number(flag, value_string);
			if (flag <= 0) continue;
			g_iShoutLimitValue = flag;
			fprintf(stdout, "SHOUT_LIMIT_VALUE: %d\n", g_iShoutLimitValue);
			continue;
		}
		
		TOKEN("item_count_limit")
		{
			str_to_number(g_bItemCountLimit, value_string);
			fprintf(stdout, "ITEM_COUNT_LIMIT: %d\n", g_bItemCountLimit);
			continue;
		}
		
		TOKEN("test_features_enable")
		{
			gbTest_Enabled = is_string_true(value_string);
			fprintf(stdout, "TEST_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("wolfman_features_enable")
		{
			gbWolfman_Enabled = is_string_true(value_string);
			fprintf(stdout, "WOLFMAN_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
	
		TOKEN("weapon_rarity_features_enable")
		{
			gbWeaponRarity_Enabled = is_string_true(value_string);
			fprintf(stdout, "WEAPON_RARITY_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("privateshop_search_features_enable")
		{
			gbPrivateShopSearch_Enabled = is_string_true(value_string);
			fprintf(stdout, "PRIVATESHOP_SEARCH_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("skill_select_ex_features_enable")
		{
			gbSkillSelectEx_Enabled = is_string_true(value_string);
			fprintf(stdout, "SKILL_SELECT_EX_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("hair_select_ex_features_enable")
		{
			gbHairSelectEx_Enabled = is_string_true(value_string);
			fprintf(stdout, "HAIR_SELECT_EX_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("point_boxes_features_enable")
		{
			gbPointBoxes_Enabled = is_string_true(value_string);
			fprintf(stdout, "POINT_BOXES_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("cheque_features_enable")
		{
			gbCheque_Enabled = is_string_true(value_string);
			fprintf(stdout, "CHEQUE_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("acce_features_enable")
		{
			gbAcce_Enabled = is_string_true(value_string);
			fprintf(stdout, "ACCE_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("changelook_features_enable")
		{
			gbChangeLook_Enabled = is_string_true(value_string);
			fprintf(stdout, "CHANGELOOK_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("combination_features_enable")
		{
			gbCombination_Enabled = is_string_true(value_string);
			fprintf(stdout, "COMBINATION_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("special_inventory_features_enable")
		{
			gbSpecialInventory_Enabled = is_string_true(value_string);
			fprintf(stdout, "SPECIAL_INVENTORY_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("gem_features_enable")
		{
			gbGem_Enabled = is_string_true(value_string);
			fprintf(stdout, "GEM_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("channel_change_features_enable")
		{
			gbChannelChange_Enabled = is_string_true(value_string);
			fprintf(stdout, "CHANNEL_CHANGE_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("aura_features_enable")
		{
			gbAura_Enabled = is_string_true(value_string);
			fprintf(stdout, "AURA_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("pet_features_enable")
		{
			gbPet_Enabled = is_string_true(value_string);
			fprintf(stdout, "PET_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("costume_set_features_enable")
		{
			gbCostumeSet_Enabled = is_string_true(value_string);
			fprintf(stdout, "COSTUME_SET_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("safebox_money_features_enable")
		{
			gbSafeboxMoney_Enabled = is_string_true(value_string);
			fprintf(stdout, "SAFEBOX_MONEY_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("value_pack_features_enable")
		{
			gbValuePack_Enabled = is_string_true(value_string);
			fprintf(stdout, "VALUE_PACK_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("boss_tracking_features_enable")
		{
			gbBossTracking_Enabled = is_string_true(value_string);
			fprintf(stdout, "BOSS_TRACKING_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("dc_coupon_features_enable")
		{
			gbDcCoupon_Enabled = is_string_true(value_string);
			fprintf(stdout, "DC_COUPON_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("mail_box_features_enable")
		{
			gbMailBox_Enabled = is_string_true(value_string);
			fprintf(stdout, "MAIL_BOX_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("remove_skill_affect_features_enable")
		{
			gbRemoveSkillAffect_Enabled = is_string_true(value_string);
			fprintf(stdout, "REMOVE_SKILL_AFFECT_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("open_giftbox_features_enable")
		{
			gbOpenGiftBox_Enabled = is_string_true(value_string);
			fprintf(stdout, "OPEN_GIFTBOX_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("transfer_money_features_enable")
		{
			gbTransferMoney_Enabled = is_string_true(value_string);
			fprintf(stdout, "TRANSFER_MONEY_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("guild_rank_features_enable")
		{
			gbGuildRank_Enabled = is_string_true(value_string);
			fprintf(stdout, "GUILD_RANK_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("average_price_features_enable")
		{
			gbAveragePrice_Enabled = is_string_true(value_string);
			fprintf(stdout, "AVERAGE_PRICE_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}
		
		TOKEN("shop_money_features_enable")
		{
			gbShopMoney_Enabled = is_string_true(value_string);
			fprintf(stdout, "SHOP_MONEY_FEATURES_ENABLE: %s\n", value_string);
			continue;
		}

		TOKEN("map_allow")
		{
			char* p = value_string;
			string stNum;

			for (; *p; p++)
			{
				if (isnhspace(*p))
				{
					if (stNum.length())
					{
						int	index = 0;
						str_to_number(index, stNum.c_str());
						map_allow_add(index);
						stNum.clear();
					}
				}
				else
					stNum += *p;
			}

			if (stNum.length())
			{
				int	index = 0;
				str_to_number(index, stNum.c_str());
				map_allow_add(index);
			}

			continue;
		}

		TOKEN("no_wander")
		{
			no_wander = true;
			continue;
		}

		TOKEN("user_limit")
		{
			str_to_number(g_iUserLimit, value_string);
			continue;
		}

		TOKEN("skill_disable")
		{
			str_to_number(g_bSkillDisable, value_string);
			continue;
		}

		TOKEN("auth_server")
		{
			char szIP[32];
			char szPort[32];

			two_arguments(value_string, szIP, sizeof(szIP), szPort, sizeof(szPort));

			if (!*szIP || (!*szPort && strcasecmp(szIP, "master")))
			{
				fprintf(stderr, "AUTH_SERVER: syntax error: <ip|master> <port>\n");
				exit(1);
			}

			g_bAuthServer = true;
			if (!strcasecmp(szIP, "master"))
			{
				fprintf(stdout, "Login server has started.\n");
			}
			else
			{
				g_stAuthMasterIP = szIP;
				str_to_number(g_wAuthMasterPort, szPort);

				fprintf(stdout, "AUTH_SERVER: master %s %u\n", g_stAuthMasterIP.c_str(), g_wAuthMasterPort);
			}
			continue;
		}

		TOKEN("quest_dir")
		{
			sys_log(0, "QUEST_DIR SETTING : %s", value_string);
			g_stQuestDir = value_string;
		}

		TOKEN("quest_object_dir")
		{
			//g_stQuestObjectDir = value_string;
			std::istringstream is(value_string);
			sys_log(0, "QUEST_OBJECT_DIR SETTING : %s", value_string);
			string dir;
			while (!is.eof())
			{
				is >> dir;
				if (is.fail())
					break;
				g_setQuestObjectDir.insert(dir);
				sys_log(0, "QUEST_OBJECT_DIR INSERT : %s", dir.c_str());
			}
		}

		TOKEN("server_id")
		{
			str_to_number(g_server_id, value_string);
		}

		TOKEN("mall_url")
		{
			g_strWebMallURL = value_string;
		}

		TOKEN("PUBLIC_IP")
		{
			strlcpy(g_szPublicIP, value_string, sizeof(g_szPublicIP));
		}

		TOKEN("SHOP_SERVER_ID")
		{
			str_to_number(g_server_id, value_string);
		}

		TOKEN("SHOP_URL")
		{
			g_strWebMallURL = value_string;
		}

		TOKEN("view_range")
		{
			str_to_number(VIEW_RANGE, value_string);
		}

		TOKEN("protect_normal_player")
		{
			str_to_number(g_protectNormalPlayer, value_string);
		}
		TOKEN("notice_battle_zone")
		{
			str_to_number(g_noticeBattleZone, value_string);
		}

		TOKEN("pk_protect_level")
		{
			str_to_number(PK_PROTECT_LEVEL, value_string);
		}

		TOKEN("max_level")
		{
			str_to_number(gPlayerMaxLevel, value_string);

			gPlayerMaxLevel = MINMAX(1, gPlayerMaxLevel, PLAYER_MAX_LEVEL_CONST);
		}

		TOKEN("block_char_creation")
		{
			int tmp = 0;

			str_to_number(tmp, value_string);

			if (0 == tmp)
				g_BlockCharCreation = false;
			else
				g_BlockCharCreation = true;

			continue;
		}
	}

	if (g_setQuestObjectDir.empty())
		g_setQuestObjectDir.insert(g_stDefaultQuestObjectDir);

	if (0 == db_port)
	{
		fprintf(stderr, "DB_PORT not configured\n");
		exit(1);
	}

	if (0 == g_bChannel)
	{
		fprintf(stderr, "CHANNEL not configured\n");
		exit(1);
	}

	if (g_stHostname.empty())
	{
		fprintf(stderr, "HOSTNAME must be configured.\n");
		exit(1);
	}

	// LOCALE_SERVICE
	LocaleService_LoadLocaleStringFile();
	LocaleService_TransferDefaultSetting();
	// END_OF_LOCALE_SERVICE

	fclose(fp);

	if ((fp = fopen("CMD", "r")))
	{
		while (fgets(buf, 256, fp))
		{
			char cmd[32], levelname[32];
			int level;

			two_arguments(buf, cmd, sizeof(cmd), levelname, sizeof(levelname));

			if (!*cmd || !*levelname)
			{
				fprintf(stderr, "CMD syntax error: <cmd> <DISABLE | LOW_WIZARD | WIZARD | HIGH_WIZARD | GOD>\n");
				exit(1);
			}

			if (!strcasecmp(levelname, "LOW_WIZARD"))
				level = GM_LOW_WIZARD;
			else if (!strcasecmp(levelname, "WIZARD"))
				level = GM_WIZARD;
			else if (!strcasecmp(levelname, "HIGH_WIZARD"))
				level = GM_HIGH_WIZARD;
			else if (!strcasecmp(levelname, "GOD"))
				level = GM_GOD;
			else if (!strcasecmp(levelname, "IMPLEMENTOR"))
				level = GM_IMPLEMENTOR;
			else if (!strcasecmp(levelname, "DISABLE"))
				level = GM_IMPLEMENTOR + 1;
			else
			{
				fprintf(stderr, "CMD syntax error: <cmd> <DISABLE | LOW_WIZARD | WIZARD | HIGH_WIZARD | GOD>\n");
				exit(1);
			}

			interpreter_set_privilege(cmd, level);
		}

		fclose(fp);
	}

	LoadStateUserCount();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	LoadShopConfig();
	LoadIndexShopLimit();
#endif

	CWarMapManager::instance().LoadWarMapInfo(nullptr);

	FN_log_adminpage();
	if (g_szPublicIP[0] == '0')
	{
		fprintf(stderr, "Can not get public ip address\n");
		exit(1);
	}
}

const char* get_table_postfix()
{
	return g_table_postfix.c_str();
}

bool LoadClientVersion()
{
	FILE* fp = fopen("VERSION", "r");

	if (!fp)
		return false;

	char buf[256];
	fgets(buf, 256, fp);

	char* p = strchr(buf, '\n');
	if (p)* p = '\0';

	fprintf(stderr, "VERSION: \"%s\"\n", buf);

	g_stClientVersion = buf;
	fclose(fp);
	return true;
}

void CheckClientVersion()
{
	const DESC_MANAGER::DESC_SET& set = DESC_MANAGER::instance().GetClientSet();
	DESC_MANAGER::DESC_SET::const_iterator it = set.begin();

	while (it != set.end())
	{
		LPDESC d = *(it++);

		if (!d->GetCharacter())
			continue;

		if (0 != g_stClientVersion.compare(d->GetClientVersion()))
		{
			d->GetCharacter()->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("클라이언트 버전이 틀려 로그아웃 됩니다. 정상적으로 패치 후 접속하세요."));
			d->DelayedDisconnect(10);
		}
	}
}

void LoadStateUserCount()
{
	FILE* fp = fopen("state_user_count", "r");

	if (!fp)
		return;

	fscanf(fp, " %d %d ", &g_iFullUserCount, &g_iBusyUserCount);

	fclose(fp);
}