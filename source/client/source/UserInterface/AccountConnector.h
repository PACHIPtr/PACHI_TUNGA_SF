#pragma 

#include "../EterLib/NetStream.h"
#include "../EterLib/FuncObject.h"

class CAccountConnector : public CNetworkStream, public CSingleton<CAccountConnector>
{
	public:
		enum EAccountConnectorState
		{
			STATE_OFFLINE,
			STATE_HANDSHAKE,
			STATE_AUTH,
		};

	public:
		CAccountConnector();
		virtual ~CAccountConnector();

		void SetHandler(PyObject* poHandler);
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
		void SetLoginInfo(const char * c_szName, const char * c_szPwd, const char * c_szClientVersion, const char * c_szPin);
#else
		void SetLoginInfo(const char * c_szName, const char * c_szPwd);
#endif
		void ClearLoginInfo( void );

		bool Connect(const char * c_szAddr, int iPort, const char * c_szAccountAddr, int iAccountPort);
		void Disconnect();
		void Process();

	protected:
		void OnConnectFailure();
		void OnConnectSuccess();
		void OnRemoteDisconnect();
		void OnDisconnect();

	protected:
		void __Inialize();
		bool __StateProcess();

		void __OfflineState_Set();
		void __HandshakeState_Set();
		void __AuthState_Set();

		bool __HandshakeState_Process();
		bool __AuthState_Process();

		bool __AuthState_RecvEmpty();
		bool __AuthState_RecvPhase();
		bool __AuthState_RecvHandshake();
		bool __AuthState_RecvPing();
		bool __AuthState_SendPong();
		bool __AuthState_RecvAuthSuccess();
		bool __AuthState_RecvAuthFailure();
#ifdef _IMPROVED_PACKET_ENCRYPTION_
		bool __AuthState_RecvKeyAgreement();
		bool __AuthState_RecvKeyAgreementCompleted();
#endif

		bool __AnalyzePacket(UINT uHeader, UINT uPacketSize, bool (CAccountConnector::*pfnDispatchPacket)());
		// TODO:  지금 현재는 임시다.  header뒤에 size 4byte가 무조건 온다는 가정임.
		// 제대로 하려면  Packet System Refactoring해야 한다. 
		bool __AnalyzeVarSizePacket(UINT uHeader, bool (CAccountConnector::*pfnDispatchPacket)(int));

#ifndef _IMPROVED_PACKET_ENCRYPTION_
		void __BuildClientKey();
#endif


	protected:
		UINT m_eState;
		std::string m_strID;
		std::string m_strPassword;
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
		std::string m_strClientVersion;
		std::string m_strPin;
#endif

		std::string m_strAddr;
		int m_iPort;

		PyObject * m_poHandler;

		// CHINA_CRYPT_KEY
		// END_OF_CHINA_CRYPT_KEY
};
