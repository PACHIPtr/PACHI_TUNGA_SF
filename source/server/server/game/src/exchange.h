#ifndef __INC_METIN_II_GAME_EXCHANGE_H__
#define __INC_METIN_II_GAME_EXCHANGE_H__

class CGrid;

enum EExchangeValues
{
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	EXCHANGE_ITEM_MAX_NUM = 20,
#else
	EXCHANGE_ITEM_MAX_NUM = 12,
#endif
	EXCHANGE_MAX_DISTANCE = 1000
};

class CExchange
{
public:
	CExchange(LPCHARACTER pOwner);
	~CExchange();

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	void		Accept(bool bIsAccept = true);
#else
	bool		Accept(bool bIsAccept = true);
#endif
	void		Cancel();

	bool		AddGold(long lGold);
#ifdef ENABLE_CHEQUE_SYSTEM
	bool		AddCheque(long bCheque);
#endif
	bool		AddItem(TItemPos item_pos, BYTE display_pos);
	bool		RemoveItem(BYTE pos);

	LPCHARACTER	GetOwner() { return m_pOwner; }
	CExchange* GetCompany() { return m_pCompany; }

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	void		SendInfo(bool isError, const char* format, ...);

	int			CountExchangingItems();
	int			GetExchangingGold() const { return m_lGold; }
	int			GetExchangingCheque() const { return m_bCheque; }

	int			GetLastCriticalUpdatePulse() const { return m_lLastCriticalUpdatePulse; };
	LPCHARACTER	GetOwner() const { return m_pOwner; }
	CExchange* GetCompany() const { return m_pCompany; }
	bool		GetAcceptStatus() const { return m_bAccept; }
	CItem* GetItemByPosition(int i) const { return m_apItems[i]; }
	int			GetPositionByItem(LPITEM item) const;
#endif

	bool		GetAcceptStatus() { return m_bAccept; }

	void		SetCompany(CExchange* pExchange) { m_pCompany = pExchange; }

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	bool		Done(bool firstPlayer);
	bool		SanityCheck();
	bool		PerformTrade();
#else
	bool		Done();
	bool		Check(int* piItemCount); ,
#endif
	bool		CheckSpace();
	bool		CheckSpecialStorageSpace(BYTE bStorageType);

private:
	CExchange* m_pCompany;	// 상대방의 CExchange 포인터

	LPCHARACTER	m_pOwner;

	TItemPos	m_aItemPos[EXCHANGE_ITEM_MAX_NUM];
	LPITEM		m_apItems[EXCHANGE_ITEM_MAX_NUM];
	BYTE		m_abItemDisplayPos[EXCHANGE_ITEM_MAX_NUM];

	bool 		m_bAccept;
	long		m_lGold;
#ifdef ENABLE_CHEQUE_SYSTEM
	long		m_bCheque;
#endif
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	int			m_lLastCriticalUpdatePulse;
#endif

	CGrid* m_pGrid;
};

#endif
