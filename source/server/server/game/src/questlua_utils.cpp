#include "stdafx.h"
#include "questmanager.h"
#include "config.h"
#include "features.h"
#include "../../common/length.h"
#include "../../common/item_length.h"

namespace quest
{
	ALUA(get_channel_id)
	{
		lua_pushnumber(L, g_bChannel);
		return 1;
	}
	
	ALUA(get_full_user_count)
	{
		lua_pushnumber(L, g_iFullUserCount);
		return 1;
	}
	
	ALUA(get_busy_user_count)
	{
		lua_pushnumber(L, g_iBusyUserCount);
		return 1;		
	}
	
	ALUA(get_client_version)
	{
		lua_pushstring(L, g_stClientVersion.c_str());
		return 1;		
	}
	
	ALUA(get_player_max_level)
	{
		lua_pushnumber(L, gPlayerMaxLevel);
		return 1;		
	}

	ALUA(is_feature_test_enabled)
	{
		lua_pushboolean(L, gbTest_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_wolfman_test_enabled)
	{
		lua_pushboolean(L, gbWolfman_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_weapon_rarity_enabled)
	{
		lua_pushboolean(L, gbWeaponRarity_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_privateshop_search_enabled)
	{
		lua_pushboolean(L, gbPrivateShopSearch_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_skill_select_ex_enabled)
	{
		lua_pushboolean(L, gbSkillSelectEx_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_hair_select_ex_enabled)
	{
		lua_pushboolean(L, gbHairSelectEx_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_point_boxes_enabled)
	{
		lua_pushboolean(L, gbPointBoxes_Enabled);
		return 1;		
	}
	
	ALUA(get_max_inventory_count)
	{
		lua_pushnumber(L, 180);
		return 1;			
	}
	
	ALUA(get_gold_max)
	{
		lua_pushnumber(L, GOLD_MAX);
		return 1;		
	}
	
	ALUA(get_player_max_status)
	{
		lua_pushnumber(L, 90);
		return 1;			
	}
	
	ALUA(get_item_rarity_max)
	{
		lua_pushnumber(L, ITEM_RARITY_MAX);
		return 1;			
	}
	
	ALUA(get_cheque_max)
	{
		lua_pushnumber(L, CHEQUE_MAX);
		return 1;		
	}
	
	ALUA(is_feature_cheque_enabled)
	{
		lua_pushboolean(L, gbCheque_Enabled);
		return 1;		
	}
	
	ALUA(is_feature_acce_enabled)
	{
		lua_pushboolean(L, gbAcce_Enabled);
		return 1;
	}
	
	ALUA(is_feature_changelook_enabled)
	{
		lua_pushboolean(L, gbChangeLook_Enabled);
		return 1;
	}
	
	ALUA(is_feature_combination_enabled)
	{
		lua_pushboolean(L, gbCombination_Enabled);
		return 1;
	}
	
	ALUA(is_feature_special_inventory_enabled)
	{
		lua_pushboolean(L, gbSpecialInventory_Enabled);
		return 1;
	}
	
	ALUA(get_zodiac_point_max)
	{
		lua_pushnumber(L, ZODIAC_POINT_MAX);
		return 1;		
	}
	
	ALUA(is_feature_gem_enabled)
	{
		lua_pushboolean(L, gbGem_Enabled);
		return 1;
	}
	
	ALUA(is_feature_channel_change_enabled)
	{
		lua_pushboolean(L, gbChannelChange_Enabled);
		return 1;
	}
	
	ALUA(is_feature_aura_enabled)
	{
		lua_pushboolean(L, gbAura_Enabled);
		return 1;
	}
	
	ALUA(is_feature_pet_enabled)
	{
		lua_pushboolean(L, gbPet_Enabled);
		return 1;
	}
	
	ALUA(is_feature_costume_set_enabled)
	{
		lua_pushboolean(L, gbCostumeSet_Enabled);
		return 1;
	}
	
	ALUA(is_feature_safebox_money_enabled)
	{
		lua_pushboolean(L, gbSafeboxMoney_Enabled);
		return 1;
	}
	
	ALUA(is_feature_value_pack_enabled)
	{
		lua_pushboolean(L, gbValuePack_Enabled);
		return 1;
	}
	
	ALUA(is_feature_boss_tracking_enabled)
	{
		lua_pushboolean(L, gbBossTracking_Enabled);
		return 1;
	}
	
	ALUA(is_feature_dc_coupon_enabled)
	{
		lua_pushboolean(L, gbDcCoupon_Enabled);
		return 1;
	}
	
	ALUA(is_feature_mail_box_enabled)
	{
		lua_pushboolean(L, gbMailBox_Enabled);
		return 1;
	}
	
	ALUA(is_feature_remove_skill_affect_enabled)
	{
		lua_pushboolean(L, gbRemoveSkillAffect_Enabled);
		return 1;
	}
	
	ALUA(is_feature_open_gift_box_enabled)
	{
		lua_pushboolean(L, gbOpenGiftBox_Enabled);
		return 1;
	}
	
	ALUA(is_feature_transfer_money_enabled)
	{
		lua_pushboolean(L, gbTransferMoney_Enabled);
		return 1;
	}
	
	ALUA(is_feature_guild_rank_enabled)
	{
		lua_pushboolean(L, gbGuildRank_Enabled);
		return 1;
	}
	
	ALUA(is_feature_average_price_enabled)
	{
		lua_pushboolean(L, gbAveragePrice_Enabled);
		return 1;
	}
	
	ALUA(is_feature_shop_money_enabled)
	{
		lua_pushboolean(L, gbShopMoney_Enabled);
		return 1;
	}

	void RegisterUtilsFunctionTable()
	{
		luaL_reg utils_functions[] =
		{
			{"get_channel_id",get_channel_id},
			{"get_full_user_count",get_full_user_count},
			{"get_busy_user_count",get_busy_user_count},
			{"get_client_version",get_client_version},
			{"get_player_max_level",get_player_max_level},
			{"is_feature_test_enabled",is_feature_test_enabled},
			{"is_feature_wolfman_test_enabled",is_feature_wolfman_test_enabled},
			{"is_feature_weapon_rarity_enabled",is_feature_weapon_rarity_enabled},
			{"is_feature_privateshop_search_enabled",is_feature_privateshop_search_enabled},
			{"is_feature_skill_select_ex_enabled",is_feature_skill_select_ex_enabled},
			{"is_feature_hair_select_ex_enabled",is_feature_hair_select_ex_enabled},
			{"is_feature_point_boxes_enabled",is_feature_point_boxes_enabled},
			{"get_max_inventory_count",get_max_inventory_count},
			{"get_gold_max",get_gold_max},
			{"get_player_max_status",get_player_max_status},
			{"get_item_rarity_max",get_item_rarity_max},
			{"get_cheque_max",get_cheque_max},
			{"get_zodiac_point_max",get_zodiac_point_max},
			{"is_feature_cheque_enabled",is_feature_cheque_enabled},
			{"is_feature_acce_enabled", is_feature_acce_enabled},
			{"is_feature_changelook_enabled", is_feature_changelook_enabled},
			{"is_feature_combination_enabled", is_feature_combination_enabled},
			{"is_feature_special_inventory_enabled", is_feature_special_inventory_enabled},
			{"is_feature_gem_enabled", is_feature_gem_enabled},
			{"is_feature_channel_change_enabled", is_feature_channel_change_enabled},
			{"is_feature_aura_enabled", is_feature_aura_enabled},
			{"is_feature_pet_enabled", is_feature_pet_enabled},
			{"is_feature_costume_set_enabled", is_feature_costume_set_enabled},
			{"is_feature_safebox_money_enabled", is_feature_safebox_money_enabled},
			{"is_feature_value_pack_enabled", is_feature_value_pack_enabled},
			{"is_feature_boss_tracking_enabled", is_feature_boss_tracking_enabled},
			{"is_feature_dc_coupon_enabled", is_feature_dc_coupon_enabled},
			{"is_feature_mail_box_enabled", is_feature_mail_box_enabled},
			{"is_feature_remove_skill_affect_enabled", is_feature_remove_skill_affect_enabled},
			{"is_feature_open_gift_box_enabled", is_feature_open_gift_box_enabled},
			{"is_feature_transfer_money_enabled", is_feature_transfer_money_enabled},
			{"is_feature_guild_rank_enabled", is_feature_guild_rank_enabled},
			{"is_feature_average_price_enabled", is_feature_average_price_enabled},
			{"is_feature_shop_money_enabled", is_feature_shop_money_enabled},
			{NULL, NULL}
		};
		
		CQuestManager::instance(). AddLuaFunctionTable("utils", utils_functions);
	}
}