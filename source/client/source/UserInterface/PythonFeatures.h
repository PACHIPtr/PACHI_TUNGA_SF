#pragma once

class CPythonFeatures : public CSingleton<CPythonFeatures>
{
public:
	enum EFeatureList
	{
		TEST_FEATURES						= 0,
		WOLFMAN_FEATURES					= 1,
		WEAPON_RARITY_FEATURES 				= 2,
		PRIVATESHOP_SEARCH_FEATURES 		= 3,
		SKILL_SELECT_EX_FEATURES 			= 4,
		HAIR_SELECT_EX_FEATURES 			= 5,
		POINT_BOXES_FEATURES 				= 6,
		CHEQUE_FEATURES 					= 7,
		ACCE_FEATURES						= 8,
		CHANGELOOK_FEATURES					= 9,
		COMBINATION_FEATURES				= 10,
		SPECIAL_INVENTORY_FEATURES			= 11,
		GEM_FEATURES						= 12,
		CHANNEL_CHANGE_FEATURES				= 13,
		AURA_FEATURES						= 14,
		PET_FEATURES						= 15,
		COSTUME_SET_FEATURES				= 16,
		SAFEBOX_MONEY_FEATURES				= 17,
		VALUE_PACK_FEATURES					= 18,
		BOSS_TRACKING_FEATURES				= 19,
		DC_COUPON_FEATURES					= 20,
		MAIL_BOX_FEATURES					= 21,
		REMOVE_SKILL_AFFECT_FEATURES		= 22,
		OPEN_GIFTBOX_FEATURES				= 23,
		TRANSFER_MONEY_FEATURES				= 24,
		GUILD_RANK_FEATURES					= 25,
		AVERAGE_PRICE_FEATURES				= 26,
		SHOP_MONEY_FEATURES					= 27,
		FEATURE_MAX_NUM,
	};

public:
	CPythonFeatures();
	~CPythonFeatures();

	void	Initialize();

	void	SetFeatureEnable(BYTE bFeatureID, bool bEnabled)	{ m_bFeaturesEnabled[bFeatureID] = bEnabled; }
	bool	IsFeatureEnabled(BYTE bFeatureID) const				{ return m_bFeaturesEnabled[bFeatureID]; }

private:
	bool	m_bFeaturesEnabled[FEATURE_MAX_NUM];
};
