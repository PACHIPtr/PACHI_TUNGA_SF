#include "stdafx.h"
#include <sstream>
#include "../../common/length.h"

#include "db.h"

#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "p2p.h"
#include "log.h"
#include "login_data.h"
#include "locale_service.h"

extern std::string g_stBlockDate;

DBManager::DBManager() : m_bIsConnect(false)
{
}

DBManager::~DBManager()
{
}

bool DBManager::Connect(const char* host, const int port, const char* user, const char* pwd, const char* db)
{
	if (m_sql.Setup(host, user, pwd, db, g_stLocale.c_str(), false, port))
		m_bIsConnect = true;

	if (!m_sql_direct.Setup(host, user, pwd, db, g_stLocale.c_str(), true, port))
	{
		sys_err("cannot open direct sql connection to host %s", host);
	}

	return m_bIsConnect;
}

void DBManager::Query(const char* c_pszFormat, ...)
{
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	std::string sQuery(szQuery);
	m_sql.AsyncQuery(sQuery.substr(0, sQuery.find_first_of(";") == -1 ? sQuery.length() : sQuery.find_first_of(";")).c_str());
}

SQLMsg* DBManager::DirectQuery(const char* c_pszFormat, ...)
{
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	std::string sQuery(szQuery);
	return m_sql_direct.DirectQuery(sQuery.substr(0, sQuery.find_first_of(";") == -1 ? sQuery.length() : sQuery.find_first_of(";")).c_str());
}

bool DBManager::IsConnected()
{
	return m_bIsConnect;
}

void DBManager::ReturnQuery(int iType, DWORD dwIdent, void* pvData, const char* c_pszFormat, ...)
{
	//sys_log(0, "ReturnQuery %s", c_pszQuery);
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	CReturnQueryInfo* p = M2_NEW CReturnQueryInfo;

	p->iQueryType = QUERY_TYPE_RETURN;
	p->iType = iType;
	p->dwIdent = dwIdent;
	p->pvData = pvData;

	m_sql.ReturnQuery(szQuery, p);
}

SQLMsg* DBManager::PopResult()
{
	SQLMsg* p;

	if (m_sql.PopResult(&p))
		return p;

	return nullptr;
}

void DBManager::Process()
{
	SQLMsg* pMsg = nullptr;

	while ((pMsg = PopResult()))
	{
		if (nullptr != pMsg->pvUserData)
		{
			switch (reinterpret_cast<CQueryInfo*>(pMsg->pvUserData)->iQueryType)
			{
			case QUERY_TYPE_RETURN:
				AnalyzeReturnQuery(pMsg);
				break;

			case QUERY_TYPE_FUNCTION:
			{
				CFuncQueryInfo* qi = reinterpret_cast<CFuncQueryInfo*>(pMsg->pvUserData);
				qi->f(pMsg);
				M2_DELETE(qi);
			}
			break;

			case QUERY_TYPE_AFTER_FUNCTION:
			{
				CFuncAfterQueryInfo* qi = reinterpret_cast<CFuncAfterQueryInfo*>(pMsg->pvUserData);
				qi->f();
				M2_DELETE(qi);
			}
			break;
			}
		}

		delete pMsg;
	}
}

CLoginData* DBManager::GetLoginData(DWORD dwKey)
{
	std::map<DWORD, CLoginData*>::iterator it = m_map_pkLoginData.find(dwKey);

	if (it == m_map_pkLoginData.end())
		return nullptr;

	return it->second;
}

void DBManager::InsertLoginData(CLoginData * pkLD)
{
	m_map_pkLoginData.insert(std::make_pair(pkLD->GetKey(), pkLD));
}

void DBManager::DeleteLoginData(CLoginData * pkLD)
{
	std::map<DWORD, CLoginData*>::iterator it = m_map_pkLoginData.find(pkLD->GetKey());

	if (it == m_map_pkLoginData.end())
		return;

	sys_log(0, "DeleteLoginData %s %p", pkLD->GetLogin(), pkLD);

	M2_DELETE(it->second);
	m_map_pkLoginData.erase(it);
}

void DBManager::SendLoginPing(const char* c_pszLogin)
{
	TPacketGGLoginPing ptog;

	ptog.bHeader = HEADER_GG_LOGIN_PING;
	strlcpy(ptog.szLogin, c_pszLogin, sizeof(ptog.szLogin));

	if (!g_pkAuthMasterDesc)  // If I am master, broadcast to others
	{
		P2P_MANAGER::instance().Send(&ptog, sizeof(TPacketGGLoginPing));
	}
	else // If I am slave send login ping to master
	{
		g_pkAuthMasterDesc->Packet(&ptog, sizeof(TPacketGGLoginPing));
	}
}

void DBManager::SendAuthLogin(LPDESC d)
{
	const TAccountTable& r = d->GetAccountTable();

	CLoginData* pkLD = GetLoginData(d->GetLoginKey());

	if (!pkLD)
		return;

	TPacketGDAuthLogin ptod;
	ptod.dwID = r.id;

	trim_and_lower(r.login, ptod.szLogin, sizeof(ptod.szLogin));
	strlcpy(ptod.szSocialID, r.social_id, sizeof(ptod.szSocialID));
	ptod.dwLoginKey = d->GetLoginKey();

	thecore_memcpy(ptod.iPremiumTimes, pkLD->GetPremiumPtr(), sizeof(ptod.iPremiumTimes));
	thecore_memcpy(&ptod.adwClientKey, pkLD->GetClientKey(), sizeof(DWORD) * 4);

	db_clientdesc->DBPacket(HEADER_GD_AUTH_LOGIN, d->GetHandle(), &ptod, sizeof(TPacketGDAuthLogin));
	sys_log(0, "SendAuthLogin %s key %u", ptod.szLogin, ptod.dwID);

	SendLoginPing(r.login);
}

void DBManager::LoginPrepare(long lRemainSecs, LPDESC d, DWORD * pdwClientKey, int* paiPremiumTimes)
{
	const TAccountTable& r = d->GetAccountTable();

	CLoginData* pkLD = M2_NEW CLoginData;

	pkLD->SetKey(d->GetLoginKey());
	pkLD->SetLogin(r.login);
	pkLD->SetRemainSecs(lRemainSecs);
	pkLD->SetIP(d->GetHostName());
	pkLD->SetClientKey(pdwClientKey);

	if (paiPremiumTimes)
		pkLD->SetPremium(paiPremiumTimes);

	InsertLoginData(pkLD);
	SendAuthLogin(d);
}

void DBManager::AnalyzeReturnQuery(SQLMsg * pMsg)
{
	CReturnQueryInfo* qi = (CReturnQueryInfo*)pMsg->pvUserData;

	switch (qi->iType)
	{
	case QID_AUTH_LOGIN:
	{
		TPacketCGLogin3* pinfo = (TPacketCGLogin3*)qi->pvData;
		LPDESC d = DESC_MANAGER::instance().FindByLoginKey(qi->dwIdent);

		if (!d)
		{
			M2_DELETE(pinfo);
			break;
		}
		//위치 변경 - By SeMinZ
		d->SetLogin(pinfo->login);

		sys_log(0, "QID_AUTH_LOGIN: START %u %p", qi->dwIdent, get_pointer(d));

		if (pMsg->Get()->uiNumRows == 0)
		{
			sys_log(0, "   NOID");
			LoginFailure(d, "NOID");
			M2_DELETE(pinfo);
		}
		else
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int col = 0;

			// PASSWORD('%s'), password, securitycode, social_id, id, status
			char szEncrytPassword[45 + 1];
			char szPassword[45 + 1];
			char szSocialID[SOCIAL_ID_MAX_LEN + 1];
			char szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
			DWORD dwID = 0;

			if (!row[col])
			{
				sys_err("error column %d", col);
				M2_DELETE(pinfo);
				break;
			}

			strlcpy(szEncrytPassword, row[col++], sizeof(szEncrytPassword));

			if (!row[col])
			{
				sys_err("error column %d", col);
				M2_DELETE(pinfo);
				break;
			}

			strlcpy(szPassword, row[col++], sizeof(szPassword));

			if (!row[col])
			{
				sys_err("error column %d", col);
				M2_DELETE(pinfo);
				break;
			}

			strlcpy(szSocialID, row[col++], sizeof(szSocialID));

			if (!row[col])
			{
				sys_err("error column %d", col);
				M2_DELETE(pinfo);
				break;
			}
			
			str_to_number(dwID, row[col++]);

			if (!row[col])
			{
				sys_err("error column %d", col);
				M2_DELETE(pinfo);
				break;
			}

			strlcpy(szStatus, row[col++], sizeof(szStatus));

			BYTE bNotAvail = 0;
			str_to_number(bNotAvail, row[col++]);

			int aiPremiumTimes[PREMIUM_MAX_NUM];
			memset(&aiPremiumTimes, 0, sizeof(aiPremiumTimes));

			char szCreateDate[256] = "00000000";

			str_to_number(aiPremiumTimes[PREMIUM_EXP], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_ITEM], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_SAFEBOX], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_AUTOLOOT], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_FISH_MIND], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_MARRIAGE_FAST], row[col++]);
			str_to_number(aiPremiumTimes[PREMIUM_GOLD], row[col++]);

			long retValue = 0;
			str_to_number(retValue, row[col]);

			time_t create_time = retValue;
			struct tm* tm1;
			tm1 = localtime(&create_time);
			strftime(szCreateDate, 255, "%Y%m%d", tm1);

			sys_log(0, "Create_Time %d %s", retValue, szCreateDate);
			sys_log(0, "Block Time %d ", strncmp(szCreateDate, g_stBlockDate.c_str(), 8));

			int nPasswordDiff = strcmp(szEncrytPassword, szPassword);

			if (nPasswordDiff)
			{
				LoginFailure(d, "WRONGPWD");
				sys_log(0, "   WRONGPWD");
				M2_DELETE(pinfo);
			}
			else if (bNotAvail)
			{
				LoginFailure(d, "NOTAVAIL");
				sys_log(0, "   NOTAVAIL");
				M2_DELETE(pinfo);
			}
			else if (DESC_MANAGER::instance().FindByLoginName(pinfo->login))
			{
				LoginFailure(d, "ALREADY");
				sys_log(0, "   ALREADY");
				M2_DELETE(pinfo);
			}
			else if (strcmp(szStatus, "OK"))
			{
				LoginFailure(d, szStatus);
				sys_log(0, "   STATUS: %s", szStatus);
				M2_DELETE(pinfo);
			}
#ifdef ENABLE_HWID_BAN_SYSTEM
			else if (!strcmp(szStatus, "HW_BAN") || !strcmp(szStatus, "HDD_BAN"))
			{
				LoginFailure(d, "HWBANNED");
				sys_log(0, "   HWBANNED");
				M2_DELETE(pinfo);
			}
#endif
			else
			{
				//stBlockData >= 0 == 날짜가 BlockDate 보다 미래
				if (strncmp(szCreateDate, g_stBlockDate.c_str(), 8) >= 0)
				{
					LoginFailure(d, "BLKLOGIN");
					sys_log(0, "   BLKLOGIN");
					M2_DELETE(pinfo);
					break;
				}

#ifdef ENABLE_HWID_BAN_SYSTEM

				char szHwid[255 * 2 + 1];
				DBManager::instance().EscapeString(szHwid, sizeof(szHwid), pinfo->mac_addr, strlen(pinfo->mac_addr));

				char szCpuID[255 * 2 + 1];
				DBManager::instance().EscapeString(szCpuID, sizeof(szCpuID), pinfo->cpu_id, strlen(pinfo->cpu_id));

				char szHDDModel[255 * 2 + 1];
				DBManager::instance().EscapeString(szHDDModel, sizeof(szHDDModel), pinfo->hdd_model, strlen(pinfo->hdd_model));

				char szMachineGUID[255 * 2 + 1];
				DBManager::instance().EscapeString(szMachineGUID, sizeof(szMachineGUID), pinfo->machine_guid, strlen(pinfo->machine_guid));

				char szHDDSerial[255 * 2 + 1];
				DBManager::instance().EscapeString(szHDDSerial, sizeof(szHDDSerial), pinfo->hdd_serial, strlen(pinfo->hdd_serial));

				std::unique_ptr<SQLMsg> msg2(DBManager::instance().DirectQuery("UPDATE account SET cpu_id='%s',hdd_model='%s',machine_guid='%s',mac_addr='%s',hdd_serial='%s' WHERE id=%u", szCpuID, szHDDModel, szMachineGUID, szHwid, szHDDSerial, dwID));

				std::unique_ptr<SQLMsg> _hwid_msg(DBManager::instance().DirectQuery("SELECT hwid FROM account.banned_hwid_list"));
				bool bReturn = true;
				if (!_hwid_msg->uiSQLErrno) {
					if (_hwid_msg->Get()->uiNumRows) {
						while (MYSQL_ROW row_hwid = mysql_fetch_row(_hwid_msg->Get()->pSQLResult)) {
							if (!strcmp(row_hwid[0], pinfo->mac_addr)) {
								LoginFailure(d, "HWBANNED");
								sys_log(0, "   HWBANNED");
								M2_DELETE(pinfo);
								bReturn = false;
								break;
							}
						}
					}
				}

				std::unique_ptr<SQLMsg> _hwid_msg2(DBManager::instance().DirectQuery("SELECT hdd_serial FROM account.banned_hdd_list"));
				if (!_hwid_msg2->uiSQLErrno) {
					if (_hwid_msg2->Get()->uiNumRows) {
						while (MYSQL_ROW row_hwid2 = mysql_fetch_row(_hwid_msg2->Get()->pSQLResult)) {
							if (!strcmp(row_hwid2[0], pinfo->hdd_serial)) {
								LoginFailure(d, "HDD_BAN");
								sys_log(0, "   HDD_BAN");
								M2_DELETE(pinfo);
								bReturn = false;
								break;
							}
						}
					}
				}

#endif

#ifdef ENABLE_CLIENT_VERSION_SYSTEM
						bool bClientVersion = false;
						bool bServerStatus = false;

						BYTE status_server = 0;
						std::unique_ptr<SQLMsg> _clientversionmsg(DBManager::instance().DirectQuery("SELECT version,status FROM account.version"));
						if (!_clientversionmsg->uiSQLErrno)
						{
							if (_clientversionmsg->Get()->uiNumRows)
							{
								while (MYSQL_ROW row_clientv = mysql_fetch_row(_clientversionmsg->Get()->pSQLResult)) {
									str_to_number(status_server, row_clientv[1]);
									if (strcmp(row_clientv[0], pinfo->clientversion))
										bClientVersion = true;
									else if (status_server == 0)
										bServerStatus = true;
								}
							}
						}

						if (bClientVersion == true)
						{
							LoginFailure(d, "NOCLIENT");
							M2_DELETE(pinfo);
							bClientVersion = false;
							break;							
						}
						

						if (bServerStatus == true)
						{
							std::unique_ptr<SQLMsg> _gm_login(DBManager::instance().DirectQuery("SELECT mAccount FROM common.gmlist WHERE mAccount='%s'", pinfo->login));
							if (!_gm_login->uiSQLErrno)
							{
								if (_gm_login->Get()->uiNumRows < 1)
								{
									LoginFailure(d, "NOSERVER");
									M2_DELETE(pinfo);
									bServerStatus = false;
									break;
								}
							}
						}

						bool bPin = false;
						std::unique_ptr<SQLMsg> _pinmsg(DBManager::instance().DirectQuery("SELECT pin FROM account.account WHERE login='%s'", pinfo->login));
						if (!_pinmsg->uiSQLErrno)
						{
							if (_pinmsg->Get()->uiNumRows)
							{
								while (MYSQL_ROW row_clientv = mysql_fetch_row(_pinmsg->Get()->pSQLResult)) {
									if (strcmp(row_clientv[0], pinfo->pin))
										bPin = true;
								}
							}
						}

						if (bPin == true)
						{
							LoginFailure(d, "NOPIN");
							M2_DELETE(pinfo);
							bPin = false;
							break;							
						}
						
						

#endif

				if (!bReturn)
					return;

				char szQuery[54 + 1];
				snprintf(szQuery, sizeof(szQuery), "UPDATE account SET last_play=NOW() WHERE id=%u", dwID);
				std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery(szQuery));

				TAccountTable& r = d->GetAccountTable();

				r.id = dwID;
				trim_and_lower(pinfo->login, r.login, sizeof(r.login));
				strlcpy(r.passwd, pinfo->passwd, sizeof(r.passwd));
				strlcpy(r.social_id, szSocialID, sizeof(r.social_id));
				DESC_MANAGER::instance().ConnectAccount(r.login, d);

				LoginPrepare(0, d, pinfo->adwClientKey, aiPremiumTimes);
				M2_DELETE(pinfo);
			}
		}
	}
	break;

	case QID_SAFEBOX_SIZE:
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(qi->dwIdent);

		if (ch)
		{
			if (pMsg->Get()->uiNumRows > 0)
			{
				MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
				int	size = 0;
				str_to_number(size, row[0]);
				ch->SetSafeboxSize(SAFEBOX_PAGE_SIZE * size);
			}
		}
	}
	break;

	case QID_HIGHSCORE_REGISTER:
	{
		THighscoreRegisterQueryInfo* info = (THighscoreRegisterQueryInfo*)qi->pvData;
		bool bQuery = true;

		if (pMsg->Get()->uiNumRows)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

			if (row && row[0])
			{
				int iCur = 0;
				str_to_number(iCur, row[0]);

				if ((info->bOrder && iCur >= info->iValue) ||
					(!info->bOrder && iCur <= info->iValue))
					bQuery = false;
			}
		}

		if (bQuery)
			Query("REPLACE INTO highscore%s VALUES('%s', %u, %d)",
				get_table_postfix(), info->szBoard, info->dwPID, info->iValue);

		M2_DELETE(info);
	}
	break;

	case QID_HIGHSCORE_SHOW:
	{
	}
	break;

	// BLOCK_CHAT
	case QID_BLOCK_CHAT_LIST:
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(qi->dwIdent);

		if (ch == nullptr)
			break;
		if (pMsg->Get()->uiNumRows)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "%s %s sec", row[0], row[1]);
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "No one currently blocked.");
		}
	}
	break;
	// END_OF_BLOCK_CHAT

	default:
		sys_err("FATAL ERROR!!! Unhandled return query id %d", qi->iType);
		break;
	}

	M2_DELETE(qi);
}

void DBManager::SendMoneyLog(BYTE type, DWORD vnum, int gold)
{
	if (!gold)
		return;
	TPacketMoneyLog p;
	p.type = type;
	p.vnum = vnum;
	p.gold = gold;
	db_clientdesc->DBPacket(HEADER_GD_MONEY_LOG, 0, &p, sizeof(p));
}

size_t DBManager::EscapeString(char* dst, size_t dstSize, const char* src, size_t srcSize)
{
	return m_sql_direct.EscapeString(dst, dstSize, src, srcSize);
}

//
// Common SQL
//
AccountDB::AccountDB() :
	m_IsConnect(false)
{
}

bool AccountDB::IsConnected()
{
	return m_IsConnect;
}

bool AccountDB::Connect(const char* host, const int port, const char* user, const char* pwd, const char* db)
{
	m_IsConnect = m_sql_direct.Setup(host, user, pwd, db, "", true, port);

	if (false == m_IsConnect)
	{
		fprintf(stderr, "cannot open direct sql connection to host: %s user: %s db: %s\n", host, user, db);
		return false;
	}

	return m_IsConnect;
}

bool AccountDB::ConnectAsync(const char* host, const int port, const char* user, const char* pwd, const char* db, const char* locale)
{
	m_sql.Setup(host, user, pwd, db, locale, false, port);
	return true;
}

void AccountDB::SetLocale(const std::string & stLocale)
{
	m_sql_direct.SetLocale(stLocale);
	m_sql_direct.QueryLocaleSet();
}

SQLMsg* AccountDB::DirectQuery(const char* query)
{
	return m_sql_direct.DirectQuery(query);
}

void AccountDB::AsyncQuery(const char* query)
{
	m_sql.AsyncQuery(query);
}

void AccountDB::ReturnQuery(int iType, DWORD dwIdent, void* pvData, const char* c_pszFormat, ...)
{
	char szQuery[4096];
	va_list args;

	va_start(args, c_pszFormat);
	vsnprintf(szQuery, sizeof(szQuery), c_pszFormat, args);
	va_end(args);

	CReturnQueryInfo* p = M2_NEW CReturnQueryInfo;

	p->iQueryType = QUERY_TYPE_RETURN;
	p->iType = iType;
	p->dwIdent = dwIdent;
	p->pvData = pvData;

	m_sql.ReturnQuery(szQuery, p);
}

SQLMsg* AccountDB::PopResult()
{
	SQLMsg* p;

	if (m_sql.PopResult(&p))
		return p;

	return nullptr;
}

void AccountDB::Process()
{
	SQLMsg* pMsg = nullptr;

	while ((pMsg = PopResult()))
	{
		CQueryInfo* qi = (CQueryInfo*)pMsg->pvUserData;

		switch (qi->iQueryType)
		{
		case QUERY_TYPE_RETURN:
			AnalyzeReturnQuery(pMsg);
			break;
		}
	}

	delete pMsg;
}

void AccountDB::AnalyzeReturnQuery(SQLMsg * pMsg)
{
	CReturnQueryInfo* qi = (CReturnQueryInfo*)pMsg->pvUserData;

	M2_DELETE(qi);
}