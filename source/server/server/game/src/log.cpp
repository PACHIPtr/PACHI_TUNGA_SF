#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "log.h"

#include "char.h"
#include "desc.h"
#include "item.h"
#include "locale_service.h"
#include "db.h"
#include "item.h"
#include "guild.h"

static char	__escape_hint[1024];

LogManager::LogManager() : m_bIsConnect(false)
{
}

LogManager::~LogManager()
{
}

bool LogManager::Connect(const char* host, const int port, const char* user, const char* pwd, const char* db)
{
	if (m_sql.Setup(host, user, pwd, db, g_stLocale.c_str(), false, port))
		m_bIsConnect = true;

	return m_bIsConnect;
}

void LogManager::Query(const char* c_pszFormat, ...)
{
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	if (test_server)
		sys_log(0, "LOG: %s", szQuery);

	std::string sQuery(szQuery);
	m_sql.AsyncQuery(sQuery.substr(0, sQuery.find_first_of(";") == -1 ? sQuery.length() : sQuery.find_first_of(";")).c_str());
}

SQLMsg* LogManager::DirectQuery(const char* c_pszFormat, ...)
{
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	if (test_server)
		sys_log(1, "LOG: %s", szQuery);

	std::string sQuery(szQuery);
	return m_sql.DirectQuery(sQuery.substr(0, sQuery.find_first_of(";") == -1 ? sQuery.length() : sQuery.find_first_of(";")).c_str());
}

bool LogManager::IsConnected()
{
	return m_bIsConnect;
}

size_t LogManager::EscapeString(char* dst, size_t dstSize, const char* src, size_t srcSize)
{
	return m_sql.EscapeString(dst, dstSize, src, srcSize);
}

void LogManager::ItemLog(DWORD dwPID, DWORD x, DWORD y, DWORD dwItemID, const char* c_pszText, const char* c_pszHint, const char* c_pszIP, DWORD dwVnum)
{
#ifdef ENABLE_MYSQL_LOGS
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), c_pszHint, strlen(c_pszHint));

	// Query("INSERT DELAYED INTO log_main%s (type, time, who, x, y, what, how, hint, ip, vnum) VALUES('ITEM', NOW(), %u, %u, %u, %u, '%s', '%s', '%s', %u)",
		// get_table_postfix(), dwPID, x, y, dwItemID, c_pszText, __escape_hint, c_pszIP, dwVnum);
#endif
}

void LogManager::ItemDestroyLog(LPCHARACTER ch, LPITEM item)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_destroy" \
		// "(owner,old_item_id, vnum, count, socket0, socket1, socket2, socket3, socket4, socket5," \
		// " attrtype0, attrtype1, attrtype2, attrtype3, attrtype4, attrtype5, attrtype6," \
		// " attrvalue0, attrvalue1, attrvalue2, attrvalue3, attrvalue4, attrvalue5, attrvalue6,"
		// " date)" \
		// "VALUES ("\
		// "%lu, %lu,%lu,%lu,"\
		// "%ld,%ld,%ld,%ld,%ld,%ld,"\
		// "%d,%d,%d,%d,"\
		// "%d,%d,%d,%d,%d,"\
		// "%d,%d,%d,%d,%d,"\
		// "NOW())"
		// , item->GetLastOwnerPID(), item->GetID(), item->GetVnum(), item->GetCount()
		// , item->GetSocket(0), item->GetSocket(1), item->GetSocket(2), item->GetSocket(3), item->GetSocket(4), item->GetSocket(5)
		// , item->GetAttributeType(0), item->GetAttributeType(1), item->GetAttributeType(2), item->GetAttributeType(3)
		// , item->GetAttributeType(4), item->GetAttributeType(5), item->GetAttributeType(6), item->GetAttributeValue(0), item->GetAttributeValue(1)
		// , item->GetAttributeValue(2), item->GetAttributeValue(3), item->GetAttributeValue(4), item->GetAttributeValue(5), item->GetAttributeValue(6)
	// );
#endif
}

#ifdef ENABLE_MINIGAME_RUMI_EVENT
void LogManager::OkayEventLog(int dwPID, const char* c_pszText, int points)
{
	// Query("INSERT INTO log_okay_event%s (pid, name, points) VALUES(%d, '%s', %d)", get_table_postfix(), dwPID, c_pszText, points);
}
#endif

void LogManager::ItemLog(LPCHARACTER ch, LPITEM item, const char* c_pszText, const char* c_pszHint)
{
#ifdef ENABLE_MYSQL_LOGS
	// if (nullptr == ch || nullptr == item)
	// {
		// sys_err("character or item nil (ch %p item %p text %s)", get_pointer(ch), get_pointer(item), c_pszText);
		// return;
	// }

	// ItemLog(ch->GetPlayerID(), ch->GetX(), ch->GetY(), item->GetID(),
		// nullptr == c_pszText ? "" : c_pszText,
		// c_pszHint, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "",
		// item->GetOriginalVnum());
#endif
}

void LogManager::ItemLog(LPCHARACTER ch, int itemID, int itemVnum, const char* c_pszText, const char* c_pszHint)
{
#ifdef ENABLE_MYSQL_LOGS
	// ItemLog(ch->GetPlayerID(), ch->GetX(), ch->GetY(), itemID, c_pszText, c_pszHint, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "", itemVnum);
#endif
}

void LogManager::CharLog(DWORD dwPID, DWORD x, DWORD y, DWORD dwValue, const char* c_pszText, const char* c_pszHint, const char* c_pszIP)
{
#ifdef ENABLE_MYSQL_LOGS
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), c_pszHint, strlen(c_pszHint));

	// Query("INSERT DELAYED INTO log_main%s (type, time, who, x, y, what, how, hint, ip) VALUES('CHARACTER', NOW(), %u, %u, %u, %u, '%s', '%s', '%s')",
		// get_table_postfix(), dwPID, x, y, dwValue, c_pszText, __escape_hint, c_pszIP);
#endif
}

void LogManager::CharLog(LPCHARACTER ch, DWORD dw, const char* c_pszText, const char* c_pszHint)
{
#ifdef ENABLE_MYSQL_LOGS
	// if (ch)
		// CharLog(ch->GetPlayerID(), ch->GetX(), ch->GetY(), dw, c_pszText, c_pszHint, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
	// else
		// CharLog(0, 0, 0, dw, c_pszText, c_pszHint, "");
#endif
}

void LogManager::LoginLog(bool isLogin, DWORD dwAccountID, DWORD dwPID, BYTE bLevel, BYTE bJob, DWORD dwPlayTime)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_login%s (type, time, channel, account_id, pid, level, job, playtime) VALUES (%s, NOW(), %d, %u, %u, %d, %d, %u)",
		// get_table_postfix(), isLogin ? "'LOGIN'" : "'LOGOUT'", g_bChannel, dwAccountID, dwPID, bLevel, bJob, dwPlayTime);
#endif
}

void LogManager::MoneyLog(BYTE type, DWORD vnum, int gold)
{
#ifdef ENABLE_MYSQL_LOGS
	// if (type == MONEY_LOG_RESERVED || type >= MONEY_LOG_TYPE_MAX_NUM)
	// {
		// sys_err("TYPE ERROR: type %d vnum %u gold %d", type, vnum, gold);
		// return;
	// }

	// Query("INSERT DELAYED INTO log_money%s VALUES (NOW(), %d, %d, %d)", get_table_postfix(), type, vnum, gold);
#endif
}

void LogManager::HackLog(const char* c_pszHackName, const char* c_pszLogin, const char* c_pszName, const char* c_pszIP)
{
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), c_pszHackName, strlen(c_pszHackName));

	// Query("INSERT INTO log_hack (time, login, name, ip, server, why) VALUES(NOW(), '%s', '%s', '%s', '%s', '%s')", c_pszLogin, c_pszName, c_pszIP, g_stHostname.c_str(), __escape_hint);
}

void LogManager::HackLog(const std::string & description, LPCHARACTER ch)
{
	// if (ch->GetDesc())
	// {
		// HackLog(description.c_str(),
			// ch->GetDesc()->GetAccountTable().login,
			// ch->GetName(),
			// ch->GetDesc()->GetHostName());
	// }
}

void LogManager::HackCRCLog(const char* c_pszHackName, const char* c_pszLogin, const char* c_pszName, const char* c_pszIP, DWORD dwCRC)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT INTO log_hack_crc (time, login, name, ip, server, why, crc) VALUES(NOW(), '%s', '%s', '%s', '%s', '%s', %u)", c_pszLogin, c_pszName, c_pszIP, g_stHostname.c_str(), c_pszHackName, dwCRC);
#endif
}
void LogManager::GoldBarLog(DWORD dwPID, DWORD dwItemID, GOLDBAR_HOW eHow, const char* c_pszHint)
{
#ifdef ENABLE_MYSQL_LOGS
	// char szHow[32 + 1];

	// switch (eHow)
	// {
	// case PERSONAL_SHOP_BUY:
		// snprintf(szHow, sizeof(szHow), "'BUY'");
		// break;

	// case PERSONAL_SHOP_SELL:
		// snprintf(szHow, sizeof(szHow), "'SELL'");
		// break;

	// case SHOP_BUY:
		// snprintf(szHow, sizeof(szHow), "'SHOP_BUY'");
		// break;

	// case SHOP_SELL:
		// snprintf(szHow, sizeof(szHow), "'SHOP_SELL'");
		// break;

	// case EXCHANGE_TAKE:
		// snprintf(szHow, sizeof(szHow), "'EXCHANGE_TAKE'");
		// break;

	// case EXCHANGE_GIVE:
		// snprintf(szHow, sizeof(szHow), "'EXCHANGE_GIVE'");
		// break;

	// case QUEST:
		// snprintf(szHow, sizeof(szHow), "'QUEST'");
		// break;

	// default:
		// snprintf(szHow, sizeof(szHow), "''");
		// break;
	// }

	// Query("INSERT DELAYED INTO log_gold%s (date, time, pid, what, how, hint) VALUES(CURDATE(), CURTIME(), %u, %u, %s, '%s')",
		// get_table_postfix(), dwPID, dwItemID, szHow, c_pszHint);
#endif
}

void LogManager::CubeLog(DWORD dwPID, DWORD x, DWORD y, DWORD item_vnum, DWORD item_uid, int item_count, bool success)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_cube%s (pid, time, x, y, item_vnum, item_uid, item_count, success) "
		// "VALUES(%u, NOW(), %u, %u, %u, %u, %d, %d)",
		// get_table_postfix(), dwPID, x, y, item_vnum, item_uid, item_count, success ? 1 : 0);
#endif
}

void LogManager::ChangeNameLog(DWORD pid, const char* old_name, const char* new_name, const char* ip)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_change_name%s (pid, old_name, new_name, time, ip) "
		// "VALUES(%u, '%s', '%s', NOW(), '%s') ",
		// get_table_postfix(), pid, old_name, new_name, ip);
#endif
}

void LogManager::GMCommandLog(DWORD dwPID, const char* szName, const char* szIP, BYTE byChannel, const char* szCommand)
{
	m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), szCommand, strlen(szCommand));

	Query("INSERT DELAYED INTO log_command%s (userid, server, ip, port, username, command, date ) "
		"VALUES(%u, 999, '%s', %u, '%s', '%s', NOW()) ",
		get_table_postfix(), dwPID, szIP, byChannel, szName, __escape_hint);
}

void LogManager::RefineLog(DWORD pid, const char* item_name, DWORD item_id, int item_refine_level, int is_success, const char* how)
{
#ifdef ENABLE_MYSQL_LOGS
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), item_name, strlen(item_name));

	// Query("INSERT INTO log_refine%s (pid, item_name, item_id, step, time, is_success, setType) VALUES(%u, '%s', %u, %d, NOW(), %d, '%s')",
		// get_table_postfix(), pid, __escape_hint, item_id, item_refine_level, is_success, how);
#endif
}

void LogManager::ShoutLog(BYTE bChannel, BYTE bEmpire, const char* pszText)
{
#ifdef ENABLE_MYSQL_LOGS
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), pszText, strlen(pszText));

	// Query("INSERT INTO log_shout%s VALUES(NOW(), %d, %d,'%s')", get_table_postfix(), bChannel, bEmpire, __escape_hint);
#endif
}

void LogManager::LevelLog(LPCHARACTER pChar, unsigned int level, unsigned int playhour)
{
#ifdef ENABLE_MYSQL_LOGS
	// DWORD aid = 0;

	// if (nullptr != pChar->GetDesc())
	// {
		// aid = pChar->GetDesc()->GetAccountTable().id;
	// }

	// Query("REPLACE INTO log_level%s (name, level, time, account_id, pid, playtime) VALUES('%s', %u, NOW(), %u, %u, %d)",
		// get_table_postfix(), pChar->GetName(), level, aid, pChar->GetPlayerID(), playhour);
#endif
}

void LogManager::BootLog(const char* c_pszHostName, BYTE bChannel)
{
	Query("INSERT INTO log_boot (time, hostname, channel) VALUES(NOW(), '%s', %d)",
		c_pszHostName, bChannel);
}

void LogManager::FishLog(DWORD dwPID, int prob_idx, int fish_id, int fish_level, DWORD dwMiliseconds, DWORD dwVnum, DWORD dwValue)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT INTO log_fish%s VALUES(NOW(), %u, %d, %u, %d, %u, %u, %u)",
		// get_table_postfix(),
		// dwPID,
		// prob_idx,
		// fish_id,
		// fish_level,
		// dwMiliseconds,
		// dwVnum,
		// dwValue);
#endif
}

void LogManager::QuestRewardLog(const char* c_pszQuestName, DWORD dwPID, DWORD dwLevel, int iValue1, int iValue2)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT INTO log_quest_reward%s VALUES('%s',%u,%u,2,%u,%u,NOW())",
		// get_table_postfix(),
		// c_pszQuestName,
		// dwPID,
		// dwLevel,
		// iValue1,
		// iValue2);
#endif
}

void LogManager::DetailLoginLog(bool isLogin, LPCHARACTER ch)
{
#ifdef ENABLE_MYSQL_LOGS
	// if (nullptr == ch->GetDesc())
		// return;

	// if (true == isLogin)
	// {
		// Query("INSERT INTO log_login2(type, is_gm, login_time, channel, account_id, pid, ip, client_version) "
			// "VALUES('INVALID', %s, NOW(), %d, %u, %u, inet_aton('%s'), '%s')",
			// ch->IsGM() == true ? "'Y'" : "'N'",
			// g_bChannel,
			// ch->GetDesc()->GetAccountTable().id,
			// ch->GetPlayerID(),
			// ch->GetDesc()->GetHostName(),
			// ch->GetDesc()->GetClientVersion());
	// }
	// else
	// {
		// Query("SET @i = (SELECT MAX(id) FROM log_login2 WHERE account_id=%u AND pid=%u)",
			// ch->GetDesc()->GetAccountTable().id,
			// ch->GetPlayerID());

		// Query("UPDATE log_login2 SET type='VALID', logout_time=NOW(), playtime=TIMEDIFF(logout_time,login_time) WHERE id=@i");
	// }
#endif
}

void LogManager::DragonSlayLog(DWORD dwGuildID, DWORD dwDragonVnum, DWORD dwStartTime, DWORD dwEndTime)
{
#ifdef ENABLE_MYSQL_LOGS
	Query("INSERT INTO log_dragon_slay%s VALUES( %d, %d, FROM_UNIXTIME(%d), FROM_UNIXTIME(%d) )",
		get_table_postfix(),
		dwGuildID, dwDragonVnum, dwStartTime, dwEndTime);
#endif
}

#ifdef ENABLE_MELEY_LAIR_DUNGEON
void LogManager::MeleyLog(DWORD dwGuildID, DWORD dwPartecipants, DWORD dwTime)
{
	// std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT partecipants, time FROM log.log_meley_dungeon%s WHERE guild_id=%u;", get_table_postfix(), dwGuildID));
	// if (pMsg->Get()->uiNumRows == 0)
		// Query("INSERT INTO log_meley_dungeon%s (guild_id, partecipants, time, date) VALUES(%u, %u, %u, NOW())", get_table_postfix(), dwGuildID, dwPartecipants, dwTime);
	// else
	// {
		// DWORD dwPartecipantsR = 0, dwTimeR = 0;
		// MYSQL_ROW mRow;
		// while (nullptr != (mRow = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		// {
			// int iCur = 0;
			// str_to_number(dwPartecipantsR, mRow[iCur++]);
			// str_to_number(dwTimeR, mRow[iCur]);
		// }

		// if ((dwTimeR == dwTime) && (dwPartecipantsR < dwPartecipants))
			// Query("UPDATE log_meley_dungeon%s SET partecipants=%u, time=%u, date=NOW() WHERE guild_id=%u;", get_table_postfix(), dwPartecipants, dwTime, dwGuildID);
		// else if (dwTimeR > dwTime)
			// Query("UPDATE log_meley_dungeon%s SET partecipants=%u, time=%u, date=NOW() WHERE guild_id=%u;", get_table_postfix(), dwPartecipants, dwTime, dwGuildID);
	// }
}
#endif

#ifdef ENABLE_ACCE_SYSTEM
void LogManager::SashLog(DWORD dwPID, DWORD x, DWORD y, DWORD item_vnum, DWORD item_uid, int item_count, int abs_chance, bool success)
{
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_sash%s (pid, time, x, y, item_vnum, item_uid, item_count, item_abs_chance, success) VALUES(%u, NOW(), %u, %u, %u, %u, %d, %d, %d)", get_table_postfix(), dwPID, x, y, item_vnum, item_uid, item_count, abs_chance, success ? 1 : 0);
#endif
}
#endif

void LogManager::ChatLog(DWORD where, DWORD who_id, const char* who_name, DWORD whom_id, const char* whom_name, const char* type, const char* msg, const char* ip)
{
	// Query("INSERT DELAYED INTO `log_chat%s` (`where`, `who_id`, `who_name`, `whom_id`, `whom_name`, `type`, `msg`, `when`, `ip`) "
		// "VALUES (%u, %u, '%s', %u, '%s', '%s', '%s', NOW(), '%s');",
		// get_table_postfix(),
		// where, who_id, who_name, whom_id, whom_name, type, msg, ip);
}

void LogManager::MiningEventLog(int dwPID, const char* c_pszText, int points)
{
	// Query("INSERT DELAYED INTO log_mining_event%s (pid, name, total_points) VALUES(%d, '%s', %d)", get_table_postfix(), dwPID, c_pszText, points);
}

void LogManager::WhisperLog(DWORD fromPID, DWORD toPID, const char* message)
{
	// m_sql.EscapeString(__escape_hint, sizeof(__escape_hint), message, strlen(message)); // Message mysql_real_escape_string Output = __escape_hint;
#ifdef ENABLE_MYSQL_LOGS
	// Query("INSERT DELAYED INTO log_whisper%s (`time`, `from`, `to`, `message`) VALUES(NOW(), %u, %u,'%s')", get_table_postfix(), fromPID, toPID, __escape_hint);
#endif
}