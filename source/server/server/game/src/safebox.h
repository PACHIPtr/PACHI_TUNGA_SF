#ifndef __INC_METIN_II_GAME_SAFEBOX_H__
#define __INC_METIN_II_GAME_SAFEBOX_H__

class CHARACTER;
class CItem;
class CGrid;

class CSafebox
{
public:
	CSafebox(LPCHARACTER pkChrOwner, int iSize, long long dwGold, WORD wCheque);
	~CSafebox();

	bool		Add(DWORD dwPos, LPITEM pkItem);
	LPITEM		Get(DWORD dwPos);
	LPITEM		Remove(DWORD dwPos);
	void		ChangeSize(int iSize);

	bool		MoveItem(INT bCell, INT bDestCell, BYTE count);
	LPITEM		GetItem(INT bCell);

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	void SetGold(long long llGold, BYTE type=0);
	long long GetGold() { return  m_lGold; };
	void SetCheque(WORD wCheque, BYTE type=0);
	WORD GetCheque() { return m_wCheque; };
#else
	void		Save();
#endif

	bool		IsEmpty(DWORD dwPos, BYTE bSize);
	bool		IsValidPosition(DWORD dwPos);

	void		SetWindowMode(BYTE bWindowMode);

protected:
	void		__Destroy();

	LPCHARACTER	m_pkChrOwner;
	LPITEM		m_pkItems[SAFEBOX_MAX_NUM];
	CGrid* m_pkGrid;
	int		m_iSize;
	long long		m_lGold;
	WORD			m_wCheque;

	BYTE		m_bWindowMode;
};

#endif
