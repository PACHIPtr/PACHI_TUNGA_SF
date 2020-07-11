#pragma once

#include "Packet.h"

typedef enum
{
	SHOP_COIN_TYPE_GOLD				= 0,
	SHOP_COIN_TYPE_SECONDARY_COIN	= 1,
#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
	SHOP_COIN_TYPE_YEAR_ROUND_COIN	= 2,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	SHOP_COIN_TYPE_COMBAT_ZONE		= 3,
#endif
	SHOP_COIN_TYPE_CHEQUE			= 4,
	SHOP_COIN_TYPE_GAYA				= 5,
	SHOP_COIN_TYPE_ZODIAC			= 6,
	SHOP_COIN_TYPE_WORSHIP			= 7,
	SHOP_COIN_TYPE_EXP				= 8,
	SHOP_COIN_TYPE_DRAGON_COIN		= 9,
	SHOP_COIN_TYPE_DRAGON_MARK		= 10,
} EShopCoinType;

class CPythonShop : public CSingleton<CPythonShop>
{
	public:
		CPythonShop(void);
		virtual ~CPythonShop(void);

		void Clear();
		
		void SetItemData(DWORD dwIndex, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData);

		void SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);
		
		void SetTabCount(BYTE bTabCount) { m_bTabCount = bTabCount; }
		BYTE GetTabCount() { return m_bTabCount; }

		void SetTabCoinType(BYTE tabIdx, BYTE coinType);
		BYTE GetTabCoinType(BYTE tabIdx);

		void SetTabName(BYTE tabIdx, const char* name);
		const char* GetTabName(BYTE tabIdx);

		//BOOL GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID);
		
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		void SetLimitedItemShop(bool isLimitedItemShop);
		BOOL IsLimitedItemShop();
#endif

		void Open(BOOL isPrivateShop, BOOL isMainPrivateShop);
		void Close();
		BOOL IsOpen();
		BOOL IsPrivateShop();
		BOOL IsMainPlayerPrivateShop();
		void ClearPrivateShopStock();
		void AddPrivateShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice, DWORD dwPriceCheque);
		void DelPrivateShopItemStock(TItemPos ItemPos);
		int GetPrivateShopItemPrice(TItemPos ItemPos);
		int GetPrivateShopItemPriceCheque(TItemPos ItemPos);
#ifndef ENABLE_OFFLINE_SHOP_SYSTEM
		void BuildPrivateShop(const char * c_szName);
#else
		void BuildPrivateShop(const char * c_szName, DWORD days, DWORD model);
#endif

	protected:
		BOOL				m_isShoping;
		BOOL				m_isPrivateShop;
		BOOL				m_isMainPlayerPrivateShop;
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		BOOL				m_isLimitedItemShop;
#endif
		struct ShopTab
		{
			ShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopItemData		items[SHOP_HOST_ITEM_MAX_NUM];
		};
		
		BYTE m_bTabCount;
		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];
		typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
		TPrivateShopItemStock	m_PrivateShopItemStock;
};
