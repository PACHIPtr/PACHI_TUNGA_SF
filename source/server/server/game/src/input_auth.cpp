#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "input.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "protocol.h"
#include "locale_service.h"
#include "db.h"
#include "utils.h"


#ifdef URIEL_AC_VERIFY
int urielac_verify(char*, char*, const unsigned char*, unsigned long, unsigned long, unsigned long, unsigned long);

const unsigned char uriel_secret[32] =
{
	0xB3, 0xEF, 0x65, 0x89, 0x4C, 0x98, 0x9E, 0x5E, 0x0F, 0xBF, 0x68, 0xEB, 0xF6, 0x50, 0xAD, 0xB4,
	0xEA, 0x6C, 0xFB, 0x93, 0x7E, 0x44, 0x5A, 0xF7, 0xD5, 0x84, 0xC7, 0x10, 0x0F, 0x4A, 0x57, 0x5E,
};
#endif

extern time_t get_global_time();

bool FN_IS_VALID_LOGIN_STRING(const char *str)
{
	const char*	tmp;

	if (!str || !*str)
		return false;

	if (strlen(str) < 2)
		return false;

	for (tmp = str; *tmp; ++tmp)
	{
		// ¾EÆAºª°u ¼oAU¸¸ Ca¿e
		if (isdigit(*tmp) || isalpha(*tmp))
			continue;

#ifdef ENABLE_ACCOUNT_W_SPECIALCHARS
	// A³³ª´U´A ¸i¸i Æ?¼o¹®AU Ca¿e
	switch (*tmp)
	{
		case ' ':
		case '_':
		case '-':
		case '.':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '(':
		case ')':
			continue;
	}
#endif
		return false;
	}

	return true;
}

bool Login_IsInChannelService(const char* c_login)
{
	if (c_login[0] == '[')
		return true;
	return false;
}

CInputAuth::CInputAuth()
{
}

void CInputAuth::Login(LPDESC d, const char* c_pData)
{
	TPacketCGLogin3* pinfo = (TPacketCGLogin3*)c_pData;

	if (!g_bAuthServer)
	{
		sys_err("CInputAuth class is not for game server. IP %s might be a hacker.",
			inet_ntoa(d->GetAddr().sin_addr));
		d->DelayedDisconnect(5);
		return;
	}
	
#ifdef URIEL_AC_VERIFY
	
	int urielac_hash = urielac_verify(pinfo->login, pinfo->passwd, uriel_secret, pinfo->uriel1, pinfo->uriel2, pinfo->uriel3, pinfo->uriel4);
	
	if(!urielac_hash)
	{
		sys_log(0, "InputAuth::Login : IS_NOT_VALID_URIELAC_HASH");
		LoginFailure(d, "SHUTDOWN");
		return;
	}
	
	char uriel_query[1024];
	snprintf(uriel_query, sizeof(uriel_query), "SELECT * FROM account.urielac_tokens WHERE token = %d", urielac_hash);
	std::unique_ptr<SQLMsg> exec_uriel_query(DBManager::instance().DirectQuery(uriel_query));

	if (exec_uriel_query->Get()->uiNumRows > 0)
	{
		sys_log(0, "InputAuth::Login : IS_NOT_UNIQUE_URIELAC_HASH");
		LoginFailure(d, "SHUTDOWN");
		return;
	}
	
	char uriel_query2[1024];
	snprintf(uriel_query2, sizeof(uriel_query2), "INSERT INTO account.urielac_tokens SET token = %d", urielac_hash);
	DBManager::instance().DirectQuery(uriel_query2);

#endif

	// string 무결성을 위해 복사
	char login[LOGIN_MAX_LEN + 1];
	trim_and_lower(pinfo->login, login, sizeof(login));

	char passwd[PASSWD_MAX_LEN + 1];
	strlcpy(passwd, pinfo->passwd, sizeof(passwd));
	
	sys_log(0, "InputAuth::Login : %s(%d) desc %p",
		login, strlen(login), get_pointer(d));

	// check login string
	if (false == FN_IS_VALID_LOGIN_STRING(login))
	{
		sys_log(0, "InputAuth::Login : IS_NOT_VALID_LOGIN_STRING(%s) desc %p",
			login, get_pointer(d));
		LoginFailure(d, "NOID");
		return;
	}
	if (g_bNoMoreClient)
	{
		TPacketGCLoginFailure failurePacket;

		failurePacket.header = HEADER_GC_LOGIN_FAILURE;
		strlcpy(failurePacket.szStatus, "SHUTDOWN", sizeof(failurePacket.szStatus));

		d->Packet(&failurePacket, sizeof(failurePacket));
		return;
	}

	if (DESC_MANAGER::instance().FindByLoginName(login))
	{
		LoginFailure(d, "ALREADY");
		return;
	}

	DWORD dwKey = DESC_MANAGER::instance().CreateLoginKey(d);

	sys_log(0, "InputAuth::Login : key %u: login %s", dwKey, login);
	TPacketCGLogin3* p = M2_NEW TPacketCGLogin3;
	thecore_memcpy(p, pinfo, sizeof(TPacketCGLogin3));

	char szPasswd[PASSWD_MAX_LEN * 2 + 1];
	DBManager::instance().EscapeString(szPasswd, sizeof(szPasswd), passwd, strlen(passwd));

	char szLogin[LOGIN_MAX_LEN * 2 + 1];
	DBManager::instance().EscapeString(szLogin, sizeof(szLogin), login, strlen(login));
	
	// CHANNEL_SERVICE_LOGIN
	if (Login_IsInChannelService(szLogin))
	{
		sys_log(0, "ChannelServiceLogin [%s]", szLogin);

		DBManager::instance().ReturnQuery(QID_AUTH_LOGIN, dwKey, p,
			"SELECT '%s',password,social_id,id,status,availDt - NOW() > 0,"
			"UNIX_TIMESTAMP(silver_expire),"
			"UNIX_TIMESTAMP(gold_expire),"
			"UNIX_TIMESTAMP(safebox_expire),"
			"UNIX_TIMESTAMP(autoloot_expire),"
			"UNIX_TIMESTAMP(fish_mind_expire),"
			"UNIX_TIMESTAMP(marriage_fast_expire),"
			"UNIX_TIMESTAMP(money_drop_rate_expire),"
			"UNIX_TIMESTAMP(create_time)"
			" FROM account WHERE login='%s'",

			szPasswd, szLogin);
	}
	// END_OF_CHANNEL_SERVICE_LOGIN
	else
	{
		DBManager::instance().ReturnQuery(QID_AUTH_LOGIN, dwKey, p,
			"SELECT PASSWORD('%s'),password,social_id,id,status,availDt - NOW() > 0,"
			"UNIX_TIMESTAMP(silver_expire),"
			"UNIX_TIMESTAMP(gold_expire),"
			"UNIX_TIMESTAMP(safebox_expire),"
			"UNIX_TIMESTAMP(autoloot_expire),"
			"UNIX_TIMESTAMP(fish_mind_expire),"
			"UNIX_TIMESTAMP(marriage_fast_expire),"
			"UNIX_TIMESTAMP(money_drop_rate_expire),"
			"UNIX_TIMESTAMP(create_time)"
			" FROM account WHERE login='%s'",
			szPasswd, szLogin);
	}
}

extern void socket_timeout(socket_t s, long sec, long usec);
int CInputAuth::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)
{
	if (!g_bAuthServer)
	{
		sys_err("CInputAuth class is not for game server. IP %s might be a hacker.",
			inet_ntoa(d->GetAddr().sin_addr));
		d->DelayedDisconnect(5);
		return 0;
	}

	int iExtraLen = 0;

	if (test_server)
		sys_log(0, " InputAuth Analyze Header[%d] ", bHeader);

	switch (bHeader)
	{
	case HEADER_CG_PONG:
		Pong(d);
		break;

	case HEADER_CG_LOGIN3:
		Login(d, c_pData);
		break;

	case HEADER_CG_HANDSHAKE:
		break;

	default:
		sys_err("This phase does not handle this header %d (0x%x)(phase: AUTH)", bHeader, bHeader);
		break;
	}

	return iExtraLen;
}