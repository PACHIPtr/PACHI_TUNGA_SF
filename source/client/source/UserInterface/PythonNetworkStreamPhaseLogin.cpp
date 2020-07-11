#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"
#include "AccountConnector.h"

// Login ---------------------------------------------------------------------------
void CPythonNetworkStream::LoginPhase()
{
	TPacketHeader header;
	if (!CheckPacket(&header))
		return;

	switch (header)
	{
		case HEADER_GC_PHASE:
			if (RecvPhasePacket())
				return;
			break;

		case HEADER_GC_LOGIN_SUCCESS:
			if (__RecvLoginSuccessPacket())
				return;
			break;


		case HEADER_GC_LOGIN_FAILURE:
			if (__RecvLoginFailurePacket())
				return;		
			break;

		case HEADER_GC_EMPIRE:
			if (__RecvEmpirePacket())
				return;
			break;

		case HEADER_GC_LOGIN_KEY:
			if (__RecvLoginKeyPacket())
				return;
			break;

		case HEADER_GC_PING:
			if (RecvPingPacket())
				return;
			break;


		default:
			if (RecvDefaultPacket(header))
				return;
			break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetLoginPhase()
{
	const char* key = LocaleService_GetSecurityKey();
#ifndef _IMPROVED_PACKET_ENCRYPTION_
	SetSecurityMode(true, key);
#endif

	if ("Login" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Login Phase ##");
	Tracen("");

	m_strPhase = "Login";	

	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::LoginPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveLoginPhase);

	m_dwChangingPhaseTime = ELTimer_GetMSec();

	if (__DirectEnterMode_IsSet())
	{
		if (0 != m_dwLoginKey)
			SendLoginPacketNew(m_stID.c_str(), m_stPassword.c_str());
		else
			SendLoginPacket(m_stID.c_str(), m_stPassword.c_str());

		// 비밀번호를 메모리에 계속 갖고 있는 문제가 있어서, 사용 즉시 날리는 것으로 변경
		ClearLoginInfo();
		CAccountConnector & rkAccountConnector = CAccountConnector::Instance();
		rkAccountConnector.ClearLoginInfo();
	}
	else
	{
		if (0 != m_dwLoginKey)
			SendLoginPacketNew(m_stID.c_str(), m_stPassword.c_str());
		else
			SendLoginPacket(m_stID.c_str(), m_stPassword.c_str());

		// 비밀번호를 메모리에 계속 갖고 있는 문제가 있어서, 사용 즉시 날리는 것으로 변경
		ClearLoginInfo();
		CAccountConnector & rkAccountConnector = CAccountConnector::Instance();
		rkAccountConnector.ClearLoginInfo();

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnLoginStart", Py_BuildValue("()"));

		__ClearSelectCharacterData();
	}
}

bool CPythonNetworkStream::__RecvEmpirePacket()
{
	TPacketGCEmpire kPacketEmpire;
	if (!Recv(sizeof(kPacketEmpire), &kPacketEmpire))
		return false;

	m_dwEmpireID=kPacketEmpire.bEmpire;
	return true;
}

bool CPythonNetworkStream::__RecvLoginSuccessPacket()
{
	TPacketGCLoginSuccess kPacketLoginSuccess;

	if (!Recv(sizeof(kPacketLoginSuccess), &kPacketLoginSuccess))
		return false;	
	
	for (int i = 0; i<PLAYER_PER_ACCOUNT; ++i)
	{
		m_akSimplePlayerInfo[i]=kPacketLoginSuccess.akSimplePlayerInformation[i];
		m_adwGuildID[i]=kPacketLoginSuccess.guild_id[i];
		m_astrGuildName[i]=kPacketLoginSuccess.guild_name[i];
	}

	m_kMarkAuth.m_dwHandle=kPacketLoginSuccess.handle;
	m_kMarkAuth.m_dwRandomKey=kPacketLoginSuccess.random_key;	

	if (__DirectEnterMode_IsSet())
	{
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "Refresh", Py_BuildValue("()"));		
	}

	return true;
}


void CPythonNetworkStream::OnConnectFailure()
{
	if (__DirectEnterMode_IsSet())
	{
		ClosePhase();
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnConnectFailure", Py_BuildValue("()"));	
	}
}


bool CPythonNetworkStream::__RecvLoginFailurePacket()
{
	TPacketGCLoginFailure packet_failure;
	if (!Recv(sizeof(TPacketGCLoginFailure), &packet_failure))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnLoginFailure", Py_BuildValue("(s)", packet_failure.szStatus));
#ifdef _DEBUG
	Tracef(" RecvLoginFailurePacket : [%s]\n", packet_failure.szStatus);
#endif
	return true;
}

bool CPythonNetworkStream::SendDirectEnterPacket(const char* c_szID, const char* c_szPassword, UINT uChrSlot)
{
	TPacketCGDirectEnter kPacketDirectEnter;
	kPacketDirectEnter.bHeader=HEADER_CG_DIRECT_ENTER;
	kPacketDirectEnter.index=uChrSlot;
	strncpy(kPacketDirectEnter.login, c_szID, ID_MAX_NUM);
	strncpy(kPacketDirectEnter.passwd, c_szPassword, PASS_MAX_NUM);

	if (!Send(sizeof(kPacketDirectEnter), &kPacketDirectEnter))
	{
		Tracen("SendDirectEnter");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendLoginPacket(const char* c_szName, const char* c_szPassword)
{
	TPacketCGLogin LoginPacket;
	LoginPacket.header = HEADER_CG_LOGIN;

	strncpy(LoginPacket.name, c_szName, sizeof(LoginPacket.name)-1);
	strncpy(LoginPacket.pwd, c_szPassword, sizeof(LoginPacket.pwd)-1);

	LoginPacket.name[ID_MAX_NUM]='\0';
	LoginPacket.pwd[PASS_MAX_NUM]='\0';

	if (!Send(sizeof(LoginPacket), &LoginPacket))
	{
		Tracen("SendLogin Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendLoginPacketNew(const char * c_szName, const char * c_szPassword)
{
	TPacketCGLogin2 LoginPacket;
	LoginPacket.header = HEADER_CG_LOGIN2;
	LoginPacket.login_key = m_dwLoginKey;

	strncpy(LoginPacket.name, c_szName, sizeof(LoginPacket.name)-1);
	LoginPacket.name[ID_MAX_NUM]='\0';

	extern DWORD g_adwEncryptKey[4];
	extern DWORD g_adwDecryptKey[4];
	for (DWORD i = 0; i < 4; ++i)
		LoginPacket.adwClientKey[i] = g_adwEncryptKey[i];
	
	if (!Send(sizeof(LoginPacket), &LoginPacket))
	{
		Tracen("SendLogin Error");
		return false;
	}


	__SendInternalBuffer();

#ifndef _IMPROVED_PACKET_ENCRYPTION_
	SetSecurityMode(true, (const char *) g_adwEncryptKey, (const char *) g_adwDecryptKey);
#endif
	return true;
}

bool CPythonNetworkStream::__RecvLoginKeyPacket()
{
	TPacketGCLoginKey kLoginKeyPacket;
	if (!Recv(sizeof(TPacketGCLoginKey), &kLoginKeyPacket))
		return false;

	m_dwLoginKey = kLoginKeyPacket.dwLoginKey;
	m_isWaitLoginKey = FALSE;

	return true;
}
