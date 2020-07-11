#include "StdAfx.h"
#include "NetStream.h"
//#include "eterCrypt.h"
#include <stdio.h>

#ifndef _IMPROVED_PACKET_ENCRYPTION_
#include "../eterBase/tea.h"
#endif

//#define _PACKETDUMP

#ifndef _IMPROVED_PACKET_ENCRYPTION_
void CNetworkStream::SetSecurityMode(bool isSecurityMode, const char* c_szTeaKey)
{
	m_isSecurityMode = isSecurityMode;
	memcpy(m_szEncryptKey, c_szTeaKey, TEA_KEY_LENGTH);
	memcpy(m_szDecryptKey, c_szTeaKey, TEA_KEY_LENGTH);
}

void CNetworkStream::SetSecurityMode(bool isSecurityMode, const char* c_szTeaEncryptKey, const char* c_szTeaDecryptKey)
{
	m_isSecurityMode = isSecurityMode;
	memcpy(m_szEncryptKey, c_szTeaEncryptKey, TEA_KEY_LENGTH);
	memcpy(m_szDecryptKey, c_szTeaDecryptKey, TEA_KEY_LENGTH);
}
#endif // _IMPROVED_PACKET_ENCRYPTION_

bool CNetworkStream::IsSecurityMode()
{
#ifdef _IMPROVED_PACKET_ENCRYPTION_
	return m_cipher.activated();
#else
	return m_isSecurityMode;
#endif
}

void CNetworkStream::SetRecvBufferSize(int recvBufSize)
{
	if (m_recvBuf)
	{
		if (m_recvBufSize>recvBufSize)
			return;

		delete [] m_recvBuf;

		if (m_recvTEABuf)
			delete [] m_recvTEABuf;
	}
	m_recvBufSize = recvBufSize;
	m_recvBuf = new char[m_recvBufSize];	
	m_recvTEABufSize = ((m_recvBufSize>>3)+1)<<3;
	m_recvTEABuf = new char[m_recvTEABufSize];
}

void CNetworkStream::SetSendBufferSize(int sendBufSize)
{
	if (m_sendBuf)
	{
		if (m_sendBufSize > sendBufSize)
			return;

		delete [] m_sendBuf;

		if (m_sendTEABuf)
			delete [] m_sendTEABuf;
	}

	m_sendBufSize = sendBufSize;
	m_sendBuf = new char[m_sendBufSize];
	m_sendTEABufSize = ((m_sendBufSize>>3)+1)<<3;
	m_sendTEABuf = new char[m_sendTEABufSize];
}

bool CNetworkStream::__RecvInternalBuffer()
{
	if (m_recvBufOutputPos>0)
	{
		int recvBufDataSize = m_recvBufInputPos - m_recvBufOutputPos;
		if (recvBufDataSize>0)
		{
			memmove(m_recvBuf, m_recvBuf + m_recvBufOutputPos, recvBufDataSize);
		}
		
		m_recvBufInputPos -= m_recvBufOutputPos;
		m_recvBufOutputPos = 0;
	}
#ifdef _IMPROVED_PACKET_ENCRYPTION_
	int restSize = m_recvBufSize - m_recvBufInputPos;
	if (restSize>0)
	{
		int recvSize = recv(m_sock, m_recvBuf + m_recvBufInputPos, m_recvBufSize - m_recvBufInputPos, 0);
		//Tracenf("RECV %d %d(%d, %d)", recvSize, restSize, m_recvTEABufSize - m_recvTEABufInputPos, m_recvBufSize - m_recvBufInputPos);

		if (recvSize < 0)
		{
			int error = WSAGetLastError();

			if (error != WSAEWOULDBLOCK)
			{
				return false;
			}
		}
		else if (recvSize == 0)
		{
			return false;
		}

		if (IsSecurityMode()) {
			m_cipher.Decrypt(m_recvBuf + m_recvBufInputPos, recvSize);
		}

		m_recvBufInputPos += recvSize;
	}
#else
	if (IsSecurityMode())
	{		
		int restSize = min(m_recvTEABufSize - m_recvTEABufInputPos, m_recvBufSize - m_recvBufInputPos);
 
		if (restSize > 0)
		{
			int recvSize = recv(m_sock, m_recvTEABuf + m_recvTEABufInputPos, restSize, 0);	
			//Tracenf("RECV %d %d(%d, %d)", recvSize, restSize, m_recvTEABufSize - m_recvTEABufInputPos, m_recvBufSize - m_recvBufInputPos);

			if (recvSize < 0)
			{
				int error = WSAGetLastError();

				if (error != WSAEWOULDBLOCK)
				{
					return false;
				}
			}
			else if (recvSize == 0)
			{
				return false;
			}

			m_recvTEABufInputPos += recvSize;

			int decodeSize = m_recvTEABufInputPos;

			if (decodeSize >= 8)
			{
				decodeSize >>= 3;
				decodeSize <<= 3;

				/*int decodeDstSize = tea_decrypt((DWORD *) (m_recvBuf + m_recvBufInputPos),
												 (DWORD *) m_recvTEABuf,
												 (const DWORD *) m_szDecryptKey,
												 decodeSize);
												 */
				int decodeDstSize = tea_decrypt((DWORD *) (m_recvBuf + m_recvBufInputPos),
												(DWORD *) m_recvTEABuf,
												(const DWORD *) m_szDecryptKey,
												decodeSize);

				m_recvBufInputPos += decodeDstSize;

				if (m_recvTEABufInputPos>decodeSize)
					memmove(m_recvTEABuf, m_recvTEABuf+decodeSize, m_recvTEABufInputPos-decodeSize);

				m_recvTEABufInputPos -= decodeSize;
				
				
				//Tracenf("!!!!!! decrypt decodeSrcSize %d -> decodeDstSize %d (recvOutputPos %d, recvInputPos %d, teaInputPos %d)", 
				//		decodeSize, decodeDstSize, m_recvBufOutputPos, m_recvBufInputPos, m_recvTEABufInputPos);
			}
		}
	}
	else
	{
		int restSize = m_recvBufSize - m_recvBufInputPos;
		if (restSize>0)
		{		
			int recvSize = recv(m_sock, m_recvBuf + m_recvBufInputPos, m_recvBufSize - m_recvBufInputPos, 0);	
			//Tracenf("RECV %d %d(%d, %d)", recvSize, restSize, m_recvTEABufSize - m_recvTEABufInputPos, m_recvBufSize - m_recvBufInputPos);

			if (recvSize < 0)
			{
				int error = WSAGetLastError();

				if (error != WSAEWOULDBLOCK)
				{
					return false;
				}
			}
			else if (recvSize == 0)
			{
				return false;
			}

			m_recvBufInputPos += recvSize;
		}
	}
#endif // _IMPROVED_PACKET_ENCRYPTION_

	//Tracef("recvSize: %d input pos %d output pos %d\n", recvSize, m_recvBufInputPos, m_recvBufOutputPos);			

	return true;
}


bool CNetworkStream::__SendInternalBuffer()
{
#ifdef _IMPROVED_PACKET_ENCRYPTION_
	int dataSize=__GetSendBufferSize();
	if (dataSize<=0)
		return true;

	if (IsSecurityMode()) {
		m_cipher.Encrypt(m_sendBuf + m_sendBufOutputPos, dataSize);
	}

	int sendSize = send(m_sock, m_sendBuf+m_sendBufOutputPos, dataSize, 0);
	if (sendSize < 0)
		return false;

	m_sendBufOutputPos+=sendSize;

	__PopSendBuffer();
#else
	if (IsSecurityMode())
	{
		int encodeSize=__GetSendBufferSize();
		if (encodeSize<=0)
			return true;

		m_sendTEABufInputPos += tea_encrypt((DWORD *) (m_sendTEABuf + m_sendTEABufInputPos),
												 (DWORD *) (m_sendBuf + m_sendBufOutputPos),
												 (const DWORD *) m_szEncryptKey,
												 encodeSize);
		m_sendBufOutputPos += encodeSize;

		if (m_sendTEABufInputPos>0)
		{	
			int sendSize = send(m_sock, m_sendTEABuf, m_sendTEABufInputPos, 0);	
			if (sendSize < 0)
				return false;

			if (m_sendTEABufInputPos>sendSize)
				memmove(m_sendTEABuf, m_sendTEABuf+sendSize, m_sendTEABufInputPos-sendSize);

			m_sendTEABufInputPos-=sendSize;			
		}

		__PopSendBuffer();
	}
	else
	{
		int dataSize=__GetSendBufferSize();
		if (dataSize<=0)
			return true;

		int sendSize = send(m_sock, m_sendBuf+m_sendBufOutputPos, dataSize, 0);	
		if (sendSize < 0)
			return false;

		m_sendBufOutputPos+=sendSize;

		__PopSendBuffer();
	}
#endif // _IMPROVED_PACKET_ENCRYPTION_
	return true;
}

void CNetworkStream::__PopSendBuffer()
{
	if (m_sendBufOutputPos<=0)
		return;
		
	int sendBufDataSize = m_sendBufInputPos - m_sendBufOutputPos;

	if (sendBufDataSize>0)
	{
		memmove(m_sendBuf, m_sendBuf+m_sendBufOutputPos, sendBufDataSize);
	}

	m_sendBufInputPos = sendBufDataSize;
	m_sendBufOutputPos = 0;	
}

#pragma warning(push)
#pragma warning(disable:4127)
void CNetworkStream::Process()
{
	if (m_sock == INVALID_SOCKET)
		return;

	fd_set fdsRecv;
	fd_set fdsSend;

	FD_ZERO(&fdsRecv);
	FD_ZERO(&fdsSend);

	FD_SET(m_sock, &fdsRecv);
	FD_SET(m_sock, &fdsSend);

	TIMEVAL delay;

	delay.tv_sec = 0;
	delay.tv_usec = 0;
	
	if (select(0, &fdsRecv, &fdsSend, nullptr, &delay) == SOCKET_ERROR)
		return;

	if (!m_isOnline)
	{
		if (FD_ISSET(m_sock, &fdsSend))
		{
			m_isOnline = true;
			OnConnectSuccess();
		}
		else if (time(nullptr) > m_connectLimitTime)
		{
			Clear();
			OnConnectFailure();
		}

		return;
	}

	if (FD_ISSET(m_sock, &fdsSend) && (m_sendBufInputPos > m_sendBufOutputPos))
	{
		if (!__SendInternalBuffer())
		{
			int error = WSAGetLastError();

			if (error != WSAEWOULDBLOCK)
			{
				OnRemoteDisconnect();
				Clear();
				return;
			}
		}
	}

	if (FD_ISSET(m_sock, &fdsRecv))
	{
		if (!__RecvInternalBuffer())
		{
			OnRemoteDisconnect();
			Clear();
			return;
		}
	}

	if (!OnProcess())
	{
		OnRemoteDisconnect();
		Clear();
	}
}
#pragma warning(pop)

void CNetworkStream::Disconnect()
{
	if (m_sock == INVALID_SOCKET)
		return;

	//OnDisconnect();

	Clear();
}

void CNetworkStream::Clear()
{
	if (m_sock == INVALID_SOCKET)
		return;

#ifdef _IMPROVED_PACKET_ENCRYPTION_
	m_cipher.CleanUp();
#endif

	closesocket(m_sock);
	m_sock = INVALID_SOCKET;

#ifndef _IMPROVED_PACKET_ENCRYPTION_
	memset(m_szEncryptKey, 0, sizeof(m_szEncryptKey));
	memset(m_szDecryptKey, 0, sizeof(m_szDecryptKey));

	m_isSecurityMode = false;
#endif

	m_isOnline = false;
	m_connectLimitTime = 0;

	m_recvTEABufInputPos = 0;
	m_sendTEABufInputPos = 0;

	m_recvBufInputPos = 0;	
	m_recvBufOutputPos = 0;
	
	m_sendBufInputPos = 0;	
	m_sendBufOutputPos = 0;

}

bool CNetworkStream::Connect(const CNetworkAddress& c_rkNetAddr, int limitSec)
{
	Clear();

	m_addr = c_rkNetAddr;
	
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET) 
	{
		Clear();
		OnConnectFailure();
		return false;
	}

	DWORD arg = 1;
	ioctlsocket(m_sock, FIONBIO, &arg);	// Non-blocking mode

	if (connect(m_sock, (PSOCKADDR)&m_addr, m_addr.GetSize()) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error != WSAEWOULDBLOCK)
		{
			Tracen("error != WSAEWOULDBLOCK");
			Clear();
			OnConnectFailure();
			return false;
		}
	}
	
	m_connectLimitTime = time(nullptr) + limitSec;
	return true;	
}

bool CNetworkStream::Connect(DWORD dwAddr, int port, int limitSec)
{
	char szAddr[256];
	{
		BYTE ip[4];
		ip[0]=dwAddr&0xff;dwAddr>>=8;
		ip[1]=dwAddr&0xff;dwAddr>>=8;
		ip[2]=dwAddr&0xff;dwAddr>>=8;
		ip[3]=dwAddr&0xff;dwAddr>>=8;

		sprintf(szAddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	}

	return Connect(szAddr, port, limitSec);
}

bool CNetworkStream::Connect(const char* c_szAddr, int port, int /*limitSec*/)
{
	CNetworkAddress kNetAddr;
	kNetAddr.Set(c_szAddr, port);

	return Connect(kNetAddr);
}

void CNetworkStream::ClearRecvBuffer()
{
	m_recvBufOutputPos = m_recvBufInputPos = 0;	 
}

int CNetworkStream::GetRecvBufferSize()
{
	return m_recvBufInputPos - m_recvBufOutputPos;
}

bool CNetworkStream::Peek(int size)
{
	if (GetRecvBufferSize() < size)
		return false;

	return true;
}

bool CNetworkStream::Peek(int size, char * pDestBuf)
{
	if (GetRecvBufferSize() < size)
		return false;

	memcpy(pDestBuf, m_recvBuf + m_recvBufOutputPos, size);
	return true;
}


#ifdef _PACKETDUMP
const char * GetSendHeaderName(BYTE header)
{
	static bool b = false;
	static string stringList[UCHAR_MAX+1];
	if (b==false)
	{
		for (DWORD i = 0; i < UCHAR_MAX+1; i++)
		{
			char buf[10];
			sprintf(buf,"%d",i);
			stringList[i] = buf;
		}
		stringList[1] = "HEADER_CG_LOGIN";
		stringList[2] = "HEADER_CG_ATTACK";
		stringList[3] = "HEADER_CG_CHAT";
		stringList[4] = "HEADER_CG_PLAYER_CREATE";
		stringList[5] = "HEADER_CG_PLAYER_DESTROY";
		stringList[6] = "HEADER_CG_PLAYER_SELECT";
		stringList[7] = "HEADER_CG_CHARACTER_MOVE";
		stringList[8] = "HEADER_CG_SYNC_POSITION";
		stringList[9] = "HEADER_CG_DIRECT_ENTER";
		stringList[10] = "HEADER_CG_ENTERGAME";
		stringList[11] = "HEADER_CG_ITEM_USE";
		stringList[12] = "HEADER_CG_ITEM_DROP";
		stringList[13] = "HEADER_CG_ITEM_MOVE";
		stringList[15] = "HEADER_CG_ITEM_PICKUP";
		stringList[16] = "HEADER_CG_QUICKSLOT_ADD";
		stringList[17] = "HEADER_CG_QUICKSLOT_DEL";
		stringList[18] = "HEADER_CG_QUICKSLOT_SWAP";
		stringList[19] = "HEADER_CG_WHISPER";
		stringList[20] = "HEADER_CG_ITEM_DROP2";
		stringList[26] = "HEADER_CG_ON_CLICK";
		stringList[27] = "HEADER_CG_EXCHANGE";
		stringList[28] = "HEADER_CG_CHARACTER_POSITION";
		stringList[29] = "HEADER_CG_SCRIPT_ANSWER";
		stringList[30] = "HEADER_CG_QUEST_INPUT_STRING";
		stringList[31] = "HEADER_CG_QUEST_CONFIRM";
		stringList[41] = "HEADER_CG_PVP";
		stringList[50] = "HEADER_CG_SHOP";
		stringList[51] = "HEADER_CG_FLY_TARGETING";
		stringList[52] = "HEADER_CG_USE_SKILL";
		stringList[53] = "HEADER_CG_ADD_FLY_TARGETING";
		stringList[54] = "HEADER_CG_SHOOT";
		stringList[55] = "HEADER_CG_MYSHOP";
		stringList[60] = "HEADER_CG_ITEM_USE_TO_ITEM";
		stringList[61] = "HEADER_CG_TARGET";
		stringList[65] = "HEADER_CG_WARP";
		stringList[66] = "HEADER_CG_SCRIPT_BUTTON";
		stringList[67] = "HEADER_CG_MESSENGER";
		stringList[69] = "HEADER_CG_MALL_CHECKOUT";
		stringList[70] = "HEADER_CG_SAFEBOX_CHECKIN";
		stringList[71] = "HEADER_CG_SAFEBOX_CHECKOUT";
		stringList[72] = "HEADER_CG_PARTY_INVITE";
		stringList[73] = "HEADER_CG_PARTY_INVITE_ANSWER";
		stringList[74] = "HEADER_CG_PARTY_REMOVE";
		stringList[75] = "HEADER_CG_PARTY_SET_STATE";
		stringList[76] = "HEADER_CG_PARTY_USE_SKILL";
		stringList[77] = "HEADER_CG_SAFEBOX_ITEM_MOVE";
		stringList[78] = "HEADER_CG_PARTY_PARAMETER";
		stringList[80] = "HEADER_CG_GUILD";
		stringList[81] = "HEADER_CG_ANSWER_MAKE_GUILD";
		stringList[82] = "HEADER_CG_FISHING";
		stringList[83] = "HEADER_CG_GIVE_ITEM";
		stringList[90] = "HEADER_CG_EMPIRE";
		stringList[96] = "HEADER_CG_REFINE";
		stringList[100] = "HEADER_CG_MARK_LOGIN";
		stringList[101] = "HEADER_CG_MARK_CRCLIST";
		stringList[102] = "HEADER_CG_MARK_UPLOAD";
		stringList[103] = "HEADER_CG_CRC_REPORT";
		stringList[105] = "HEADER_CG_HACK";
		stringList[106] = "HEADER_CG_CHANGE_NAME";
		stringList[107] = "HEADER_CG_SMS";
		stringList[108] = "HEADER_CG_MATRIX_CARD";
		stringList[109] = "HEADER_CG_LOGIN2";
		stringList[110] = "HEADER_CG_DUNGEON";
		stringList[111] = "HEADER_CG_LOGIN3";
		stringList[112] = "HEADER_CG_GUILD_SYMBOL_UPLOAD";
		stringList[113] = "HEADER_CG_GUILD_SYMBOL_CRC";
		stringList[114] = "HEADER_CG_SCRIPT_SELECT_ITEM";
		stringList[0xfc] = "HEADER_CG_TIME_SYNC";
		stringList[0xfd] = "HEADER_CG_CLIENT_VERSION";
		stringList[0xfe] = "HEADER_CG_PONG";
		stringList[0xff] = "HEADER_CG_HANDSHAKE";
	}
	return stringList[header].c_str();
}

const char * GetRecvHeaderName(BYTE header)
{
	static bool b = false;
	static string stringList[UCHAR_MAX+1];
	if (b==false)
	{
		for (DWORD i = 0; i < UCHAR_MAX+1; i++)
		{
			char buf[10];
			sprintf(buf,"%d",i);
			stringList[i] = buf;
		}
		stringList[1] = "HEADER_GC_CHARACTER_ADD";
		stringList[2] = "HEADER_GC_CHARACTER_DEL";
		stringList[3] = "HEADER_GC_CHARACTER_MOVE";
		stringList[4] = "HEADER_GC_CHAT";
		stringList[5] = "HEADER_GC_SYNC_POSITION";
		stringList[6] = "HEADER_GC_LOGIN_SUCCESS";
		stringList[7] = "HEADER_GC_LOGIN_FAILURE";
		stringList[8] = "HEADER_GC_PLAYER_CREATE_SUCCESS";
		stringList[9] = "HEADER_GC_PLAYER_CREATE_FAILURE";
		stringList[10] = "HEADER_GC_PLAYER_DELETE_SUCCESS";
		stringList[11] = "HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID";

		stringList[13] = "HEADER_GC_STUN";
		stringList[14] = "HEADER_GC_DEAD";
		stringList[15] = "HEADER_GC_MAIN_CHARACTER";
		stringList[16] = "HEADER_GC_PLAYER_POINTS";
		stringList[17] = "HEADER_GC_PLAYER_POINT_CHANGE";
		stringList[18] = "HEADER_GC_CHANGE_SPEED";
		stringList[19] = "HEADER_GC_CHARACTER_UPDATE";
		stringList[20] = "HEADER_GC_ITEM_SET";
		stringList[22] = "HEADER_GC_ITEM_USE";
		stringList[23] = "HEADER_GC_ITEM_DROP";
		stringList[25] = "HEADER_GC_ITEM_UPDATE";
		stringList[26] = "HEADER_GC_ITEM_GROUND_ADD";
		stringList[27] = "HEADER_GC_ITEM_GROUND_DEL";
		stringList[28] = "HEADER_GC_QUICKSLOT_ADD";
		stringList[29] = "HEADER_GC_QUICKSLOT_DEL";
		stringList[30] = "HEADER_GC_QUICKSLOT_SWAP";
		stringList[31] = "HEADER_GC_ITEM_OWNERSHIP";
		stringList[33] = "HEADER_GC_ITEM_UNBIND_TIME";
		stringList[34] = "HEADER_GC_WHISPER	";
		stringList[35] = "HEADER_GC_ALERT";
		stringList[36] = "HEADER_GC_MOTION";
		stringList[38] = "HEADER_GC_SHOP";
		stringList[39] = "HEADER_GC_SHOP_SIGN";
		stringList[41] = "HEADER_GC_PVP";
		stringList[42] = "HEADER_GC_EXCHANGE";
		stringList[43] = "HEADER_GC_CHARACTER_POSITION";
		stringList[44] = "HEADER_GC_PING";
		stringList[45] = "HEADER_GC_SCRIPT";
		stringList[46] = "HEADER_GC_QUEST_CONFIRM";

		stringList[61] = "HEADER_GC_MOUNT";
		stringList[62] = "HEADER_GC_OWNERSHIP"; 
		stringList[63] = "HEADER_GC_TARGET";
		stringList[65] = "HEADER_GC_WARP"; 
		stringList[69] = "HEADER_GC_ADD_FLY_TARGETING";

		stringList[70] = "HEADER_GC_CREATE_FLY";
		stringList[71] = "HEADER_GC_FLY_TARGETING";
		stringList[72] = "HEADER_GC_SKILL_LEVEL";
		stringList[73] = "HEADER_GC_SKILL_COOLTIME_END";
		stringList[74] = "HEADER_GC_MESSENGER";
		stringList[75] = "HEADER_GC_GUILD";
		stringList[76] = "HEADER_GC_SKILL_LEVEL_NEW";
		stringList[77] = "HEADER_GC_PARTY_INVITE";
		stringList[78] = "HEADER_GC_PARTY_ADD";
		stringList[79] = "HEADER_GC_PARTY_UPDATE";
		stringList[80] = "HEADER_GC_PARTY_REMOVE";
		stringList[81] = "HEADER_GC_QUEST_INFO";
		stringList[82] = "HEADER_GC_REQUEST_MAKE_GUILD";
		stringList[83] = "HEADER_GC_PARTY_PARAMETER";
		stringList[84] = "HEADER_GC_SAFEBOX_MONEY_CHANGE";
		stringList[85] = "HEADER_GC_SAFEBOX_SET";
		stringList[86] = "HEADER_GC_SAFEBOX_DEL";
		stringList[87] = "HEADER_GC_SAFEBOX_WRONG_PASSWORD";
		stringList[88] = "HEADER_GC_SAFEBOX_SIZE";
		stringList[89] = "HEADER_GC_FISHING";
		stringList[90] = "HEADER_GC_EMPIRE";
		stringList[91] = "HEADER_GC_PARTY_LINK";
		stringList[92] = "HEADER_GC_PARTY_UNLINK";

		stringList[95] = "HEADER_GC_REFINE_INFORMATION";
		stringList[96] = "HEADER_GC_OBSERVER_ADD";
		stringList[97] = "HEADER_GC_OBSERVER_REMOVE";
		stringList[98] = "HEADER_GC_OBSERVER_MOVE";
		stringList[99] = "HEADER_GC_VIEW_EQUIP";
		stringList[100] = "HEADER_GC_MARK_BLOCK";
		stringList[101] = "HEADER_GC_MARK_DIFF_DATA";

		stringList[106] = "HEADER_GC_TIME";
		stringList[107] = "HEADER_GC_CHANGE_NAME";
		stringList[110] = "HEADER_GC_DUNGEON";
		stringList[111] = "HEADER_GC_WALK_MODE"; 
		stringList[112] = "HEADER_GC_CHANGE_SKILL_GROUP";
		stringList[113] = "HEADER_GC_MAIN_CHARACTER_NEW";
		stringList[114] = "HEADER_GC_USE_POTION";
		stringList[115] = "HEADER_GC_NPC_POSITION";
		stringList[116] = "HEADER_GC_MATRIX_CARD";
		stringList[117] = "HEADER_GC_CHARACTER_UPDATE2";
		stringList[118] = "HEADER_GC_LOGIN_KEY";
		stringList[119] = "HEADER_GC_REFINE_INFORMATION_NEW";
		stringList[120] = "HEADER_GC_CHARACTER_ADD2";
		stringList[121] = "HEADER_GC_CHANNEL";
		stringList[122] = "HEADER_GC_MALL_OPEN";
		stringList[123] = "HEADER_GC_TARGET_UPDATE";
		stringList[124] = "HEADER_GC_TARGET_DELETE";
		stringList[125] = "HEADER_GC_TARGET_CREATE_NEW";
		stringList[126] = "HEADER_GC_AFFECT_ADD";
		stringList[127] = "HEADER_GC_AFFECT_REMOVE";
		stringList[128] = "HEADER_GC_MALL_SET";
		stringList[129] = "HEADER_GC_MALL_DEL";
		stringList[130] = "HEADER_GC_LAND_LIST";
		stringList[131] = "HEADER_GC_LOVER_INFO";
		stringList[132] = "HEADER_GC_LOVE_POINT_UPDATE";
		stringList[133] = "HEADER_GC_GUILD_SYMBOL_DATA";
		stringList[134] = "HEADER_GC_DIG_MOTION";
		stringList[135] = "HEADER_GC_DAMAGE_INFO";
		stringList[136] = "HEADER_GC_CHAR_ADDITIONAL_INFO";
		stringList[150] = "HEADER_GC_AUTH_SUCCESS"
		stringList[154] = "HEADER_GC_AUTH_SUCCESS_OPENID";

		stringList[0xfc] = "HEADER_GC_HANDSHAKE_OK";
		stringList[0xfd] = "HEADER_GC_PHASE";
		stringList[0xfe] = "HEADER_GC_BINDUDP";
		stringList[0xff] = "HEADER_GC_HANDSHAKE";
	}
	return stringList[header].c_str();
}

#endif

bool CNetworkStream::Recv(int size)
{
	if (!Peek(size))
		return false;

	m_recvBufOutputPos += size;
	
	return true;
}

bool CNetworkStream::Recv(int size, char * pDestBuf)
{
	if (!Peek(size, pDestBuf)) 
		return false;
	
#ifdef _PACKETDUMP
	if (*pDestBuf != 0 )
	{
		TraceError("RECV< %s (%d)", GetRecvHeaderName(*pDestBuf), size);
		string contents;
		char buf[10];
		for(int i = 1; i < size; i++)
		{
			sprintf(buf," %02x", (unsigned char)(pDestBuf[i]));
			contents.append(buf);
		}
		TraceError(contents.c_str());
	}
		
#endif
	m_recvBufOutputPos += size;
	return true;
}

int CNetworkStream::__GetSendBufferSize()
{
	return m_sendBufInputPos-m_sendBufOutputPos;
}


bool CNetworkStream::Send(int size, const char * pSrcBuf)
{
	int sendBufRestSize = m_sendBufSize - m_sendBufInputPos;
	if ((size + 1) > sendBufRestSize)
		return false;

	memcpy(m_sendBuf + m_sendBufInputPos, pSrcBuf, size);
	m_sendBufInputPos += size;

#ifdef _PACKETDUMP
	if (*pSrcBuf != 0 )
	{
		TraceError("SEND> %s (%d)", GetSendHeaderName(*pSrcBuf), size);
		string contents;
		char buf[10];
		for(int i = 1; i < size; i++)
		{
			sprintf(buf," %02x", (unsigned char)(pSrcBuf[i]));
			contents.append(buf);
		}
		TraceError(contents.c_str());

	}
#endif

	return true;
	/*
	if (size > 0)
	{
		if (IsSecurityMode())
		{
			m_sendBufInputPos += TEA_Encrypt((DWORD *) (m_sendBuf + m_sendBufInputPos),
											 (DWORD *) (m_sendBuf + m_sendBufInputPos),
											 (const DWORD *) gs_szTeaKey,
											 size);
		}
		else
		{
		}
	}

	return __SendInternalBuffer();
	*/
}

bool CNetworkStream::Peek(int len, void* pDestBuf)
{
	return Peek(len, (char*)pDestBuf);
}

bool CNetworkStream::Recv(int len, void* pDestBuf)
{
	return Recv(len, (char*)pDestBuf);
}

bool CNetworkStream::SendFlush(int len, const void* pSrcBuf)
{
	if (!Send(len, pSrcBuf))
		return false;

	return __SendInternalBuffer();
}

bool CNetworkStream::Send(int len, const void* pSrcBuf)
{
	return Send(len, (const char*)pSrcBuf);
}

bool CNetworkStream::IsOnline()
{
	return m_isOnline;
}

bool CNetworkStream::OnProcess()
{
	return true;
}

void CNetworkStream::OnRemoteDisconnect()
{
}

void CNetworkStream::OnDisconnect()
{
}

void CNetworkStream::OnConnectSuccess()
{
	Tracen("Succeed connecting.");
}

void CNetworkStream::OnConnectFailure()
{
	Tracen("Failed to connect.");
}

//void CNetworkStream::OnCheckinSuccess()
//{
//}

//void CNetworkStream::OnCheckinFailure()
//{
//}


CNetworkStream::CNetworkStream()
{
	m_sock = INVALID_SOCKET;

#ifndef _IMPROVED_PACKET_ENCRYPTION_
	m_isSecurityMode = false;
#endif
	m_isOnline = false;
	m_connectLimitTime = 0;

	m_recvTEABuf = nullptr;
	m_recvTEABufSize = 0;
	m_recvTEABufInputPos = 0;
	
	m_recvBuf = nullptr;	
	m_recvBufSize = 0;	
	m_recvBufOutputPos = 0;
	m_recvBufInputPos = 0;	

	m_sendTEABuf = nullptr;
	m_sendTEABuf = 0;
	m_sendTEABufInputPos = 0;

	m_sendBuf = nullptr;	
	m_sendBufSize = 0;	
	m_sendBufOutputPos = 0;
	m_sendBufInputPos = 0;
}

CNetworkStream::~CNetworkStream()
{
	Clear();

	if (m_sendTEABuf)
	{
		delete [] m_sendTEABuf;
		m_sendTEABuf=nullptr;
	}

	if (m_recvTEABuf)
	{
		delete [] m_recvTEABuf;
		m_recvTEABuf=nullptr;
	}

	if (m_recvBuf)
	{
		delete [] m_recvBuf;
		m_recvBuf=nullptr;
	}

	if (m_sendBuf)
	{
		delete [] m_sendBuf;
		m_sendBuf=nullptr;
	}
}

#ifdef _IMPROVED_PACKET_ENCRYPTION_
size_t CNetworkStream::Prepare(void* buffer, size_t* length)
{
	return m_cipher.Prepare(buffer, length);
}

bool CNetworkStream::Activate(size_t agreed_length, const void* buffer, size_t length)
{
	return m_cipher.Activate(true, agreed_length, buffer, length);
}

void CNetworkStream::ActivateCipher()
{
	return m_cipher.set_activated(true);
}
#endif // _IMPROVED_PACKET_ENCRYPTION_