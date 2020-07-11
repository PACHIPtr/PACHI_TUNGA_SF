#ifndef __INC_METIN_II_GAME_SHOP_MANAGER_H__
#define __INC_METIN_II_GAME_SHOP_MANAGER_H__

class CShop;
typedef class CShop* LPSHOP;

class CShopManager : public singleton<CShopManager>
{
public:
	typedef std::map<DWORD, CShop*> TShopMap;

public:
	CShopManager();
	virtual ~CShopManager();

	bool	Initialize(TShopTable* table, int size);
	void	Destroy();

	LPSHOP	Get(DWORD dwVnum);
	LPSHOP	GetByNPCVnum(DWORD dwVnum);

	bool	StartShopping(LPCHARACTER pkChr, LPCHARACTER pkShopKeeper, int iShopVnum = 0);
	void	StopShopping(LPCHARACTER ch);

	void	Buy(LPCHARACTER ch, BYTE pos);
	void	Sell(LPCHARACTER ch, UINT bCell, BYTE bCount = 0, BYTE bType = 0);

	LPSHOP	CreatePCShop(LPCHARACTER ch, TShopItemTable* pTable, BYTE bItemCount);
	LPSHOP	FindPCShop(DWORD dwVID);
	void	DestroyPCShop(LPCHARACTER ch);
	bool	AddGuest(LPCHARACTER ch, DWORD dwVID);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
public:
	bool	StartOfflineShop(DWORD id, bool onboot = false);
	bool	CreateOfflineShop(LPCHARACTER owner, const char* szSign, const std::vector<TShopItemTable*> pTable, DWORD days, DWORD model, DWORD level, DWORD align);
	LPSHOP	CreateNPCShop(LPCHARACTER ch, std::vector<TShopItemTable*> map_shop);
	DWORD	GetShopCount() { return m_map_pkShop.size(); }
#endif
	TShopMap	GetShopMap() { return m_map_pkShopByPC; }
#ifdef ENABLE_GEM_SYSTEM
public:
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	bool	InitializeGemShop(TGemShopTable* table, int size);
	bool	InitializeBossGemShop(TGemShopTable* table, int size);
	DWORD 	GemShopGetRandomId(DWORD dwRow, bool gemShopType);
	DWORD	GemShopGetID(DWORD index, bool gemShopType);
	bool 	GemShopGetInfoById(DWORD itemId, DWORD& dwVnum, BYTE& bCount, DWORD& dwPrice, bool gemShopType);
	DWORD 	GemShopGetVnumById(DWORD id, bool gemShopType);
	BYTE 	GemShopGetCountById(DWORD id, bool gemShopType);
	DWORD 	GemShopGetPriceById(DWORD id, bool gemShopType);
	#else
	bool	InitializeGemShop(TGemShopTable* table, int size);
	DWORD 	GemShopGetRandomId(DWORD dwRow);
	bool 	GemShopGetInfoById(DWORD itemId, DWORD& dwVnum, BYTE& bCount, DWORD& dwPrice);
	#endif
private:
	int					m_iGemShopTableSize;
	TGemShopTable* m_pGemShopTable;
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	int					m_iGemBossShopTableSize;
	TGemShopTable* m_pGemBossShopTable;
	#endif
#endif

private:
	TShopMap	m_map_pkShop;
	TShopMap	m_map_pkShopByNPCVnum;
	TShopMap	m_map_pkShopByPC;

	bool	ReadShopTableEx(const char* stFileName);
};

#endif