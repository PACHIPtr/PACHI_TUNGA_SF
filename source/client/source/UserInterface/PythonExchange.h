#pragma once

#include "Packet.h"

/*
 *	교환 창 관련
 */
class CPythonExchange : public CSingleton<CPythonExchange>
{
	public:
		enum
		{
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
			EXCHANGE_ITEM_MAX_NUM = 20,
#else
			EXCHANGE_ITEM_MAX_NUM = 12,
#endif
		};
		
		typedef struct trade
		{
			char					name[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
			DWORD					vid;
#endif
			DWORD					level;

			DWORD					item_vnum[EXCHANGE_ITEM_MAX_NUM];
			BYTE					item_count[EXCHANGE_ITEM_MAX_NUM];
			DWORD					item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
			TPlayerItemAttribute	item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];

			BYTE					accept;
			DWORD					elk;
#ifdef ENABLE_CHEQUE_SYSTEM
			DWORD					cheque;
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
			DWORD	transmutation[EXCHANGE_ITEM_MAX_NUM];
#endif
		} TExchangeData;

	public:
		CPythonExchange();
		virtual ~CPythonExchange();

		void			Clear();

		void			Start();
		void			End();
		bool			isTrading();

		// Interface
		
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
		void			SetTargetVID(DWORD vid) { m_victim.vid = vid; }
		DWORD			GetTargetVID() const { return m_victim.vid; }
#endif

		void			SetSelfName(const char *name);
		void			SetTargetName(const char *name);

		char			*GetNameFromSelf();
		char			*GetNameFromTarget();
		

		void			SetSelfLevel(DWORD level);
		void			SetTargetLevel(DWORD level);

		DWORD			GetLevelFromSelf();
		DWORD			GetLevelFromTarget();

		void			SetElkToTarget(DWORD elk);
		void			SetElkToSelf(DWORD elk);

		DWORD			GetElkFromTarget();
		DWORD			GetElkFromSelf();
		
#ifdef ENABLE_CHEQUE_SYSTEM
		void			SetChequeToTarget(DWORD cheque);
		void			SetChequeToSelf(DWORD cheque);

		DWORD			GetChequeFromTarget();
		DWORD			GetChequeFromSelf();
#endif

#ifdef ENABLE_NEW_EXCHANGE_WINDOW
		void			SetSelfRace(DWORD race);
		void			SetTargetRace(DWORD race);
		DWORD			GetRaceFromSelf();
		DWORD			GetRaceFromTarget();
		void			SetSelfLevel(DWORD level);
		void			SetTargetLevel(DWORD level);
		DWORD			GetLevelFromSelf();
		DWORD			GetLevelFromTarget();
#endif

		void			SetItemToTarget(DWORD pos, DWORD vnum, BYTE count);
		void			SetItemToSelf(DWORD pos, DWORD vnum, BYTE count);

		void			SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
		void			SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

		void			SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
		void			SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);
	
		void			DelItemOfTarget(BYTE pos);
		void			DelItemOfSelf(BYTE pos);

		DWORD			GetItemVnumFromTarget(BYTE pos);
		DWORD			GetItemVnumFromSelf(BYTE pos);

		BYTE			GetItemCountFromTarget(BYTE pos);
		BYTE			GetItemCountFromSelf(BYTE pos);

		DWORD			GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
		DWORD			GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

		void			GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);
		void			GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);
		
#ifdef ENABLE_CHANGELOOK_SYSTEM
		void			SetItemTransmutation(int iPos, DWORD dwTransmutation, bool bSelf);
		DWORD			GetItemTransmutation(int iPos, bool bSelf);
#endif
		
		void			SetAcceptToTarget(BYTE Accept);
		void			SetAcceptToSelf(BYTE Accept);

		bool			GetAcceptFromTarget();
		bool			GetAcceptFromSelf();

		bool			GetElkMode();
		void			SetElkMode(bool value);

	protected:
		bool				m_isTrading;

		bool				m_elk_mode;   // 엘크를 클릭해서 교환했을때를 위한 변종임.
		TExchangeData		m_self;
		TExchangeData		m_victim;
};
