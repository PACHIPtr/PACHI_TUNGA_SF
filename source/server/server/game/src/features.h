#ifndef __FEATURES_H__
#define __FEATURES_H__

namespace features
{
	enum EFeatureList
	{
		TEST				= 0,
		WOLFMAN				= 1,
		WEAPON_RARITY		= 2,
		PRIVATESHOP_SEARCH	= 3,
		SKILL_SELECT_EX		= 4,
		HAIR_SELECT_EX		= 5,
		POINT_BOXES			= 6,
		CHEQUE				= 7,
		ACCE				= 8,
		CHANGELOOK			= 9,
		COMBINATION			= 10,
		SPECIAL_INVENTORY	= 11,
		GEM					= 12,
		CHANNEL_CHANGE		= 13,
		AURA				= 14,
		PET					= 15,
		COSTUME_SET			= 16,
		SAFEBOX_MONEY		= 17,
		VALUE_PACK			= 18,
		BOSS_TRACKING		= 19,
		DC_COUPON			= 20,
		MAIL_BOX			= 21,
		REMOVE_SKILL_AFFECT	= 22,
		OPEN_GIFTBOX		= 23,
		TRANSFER_MONEY		= 24,
		GUILD_RANK			= 25,
		AVERAGE_PRICE		= 26,
		SHOP_MONEY			= 27,
		FEATURES_MAX_NUM,
	};

	extern const std::vector<const char*>& GetFeatureList();
	extern void SetFeatureEnabled(BYTE bFeatureID, bool bEnabled, bool bP2P = false);
	extern bool IsFeatureEnabled(BYTE bFeatureID);
	extern void LoadPlayer(LPCHARACTER pkChr);
	extern void SetTestEnabled(bool bEnabled, bool bP2P = false);
	extern void SetWolfmanEnabled(bool bEnabled, bool bP2P = false);
	extern void SetWeaponRarityEnabled(bool bEnabled, bool bP2P = false);
	extern void SetPrivateShopSearchEnabled(bool bEnabled, bool bP2P = false);
	extern void SetSkillSelectExEnabled(bool bEnabled, bool bP2P = false);
	extern void SetHairSelectExEnabled(bool bEnabled, bool bP2P = false);
	extern void SetPointBoxesEnabled(bool bEnabled, bool bP2P = false);
	extern void SetChequeEnabled(bool bEnabled, bool bP2P = false);
	extern void SetAcceEnabled(bool bEnabled, bool bP2P = false);
	extern void SetChangeLookEnabled(bool bEnabled, bool bP2P = false);
	extern void SetCombinationEnabled(bool bEnabled, bool bP2P = false);
	extern void SetSpecialInventoryEnabled(bool bEnabled, bool bP2P = false);
	extern void SetGemEnabled(bool bEnabled, bool bP2P = false);
	extern void SetChannelChangeEnabled(bool bEnabled, bool bP2P = false);
	extern void SetAuraEnabled(bool bEnabled, bool bP2P = false);
	extern void SetPetEnabled(bool bEnabled, bool bP2P = false);
	extern void SetCostumeSetEnabled(bool bEnabled, bool bP2P = false);
	extern void SetSafeboxMoneyEnabled(bool bEnabled, bool bP2P = false);
	extern void SetValuePackEnabled(bool bEnabled, bool bP2P = false);
	extern void SetBossTrackingEnabled(bool bEnabled, bool bP2P = false);
	extern void SetDcCouponEnabled(bool bEnabled, bool bP2P = false);
	extern void SetMailBoxEnabled(bool bEnabled, bool bP2P = false);
	extern void SetRemoveSkillAffectEnabled(bool bEnabled, bool bP2P = false);
	extern void SetOpenGiftboxEnabled(bool bEnabled, bool bP2P = false);
	extern void SetTransferMoneyEnabled(bool bEnabled, bool bP2P = false);
	extern void SetGuildRankEnabled(bool bEnabled, bool bP2P = false);
	extern void SetAveragePriceEnabled(bool bEnabled, bool bP2P = false);
	extern void SetShopMoneyEnabled(bool bEnabled, bool bP2P = false);
}
#endif /*__FEATURES_H__*/
