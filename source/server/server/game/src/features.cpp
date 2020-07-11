#include "stdafx.h"
#include "char.h"
#include "config.h"
#include "sectree_manager.h"
#include "start_position.h"
#include "item.h"
#include "p2p.h"
#include "desc.h"
#include "features.h"

namespace features
{
	struct FGetPlayer
	{
	public:
		void operator()(LPENTITY ent)
		{
			vecPlayer.push_back((LPCHARACTER)ent);
		}

		std::vector<LPCHARACTER>	vecPlayer;
	};

	void SendP2PPacket(BYTE bFeatureID, bool bEnabled)
	{
		TPacketGGFeatureEnable packet;
		packet.header = HEADER_GG_FEATURE_ENABLE;
		packet.feature_index = bFeatureID;
		packet.enabled = bEnabled;
		P2P_MANAGER::instance().Send(&packet, sizeof(packet));
	}

	void SendClientPacket(LPDESC pkDesc, BYTE bFeatureID, bool bEnabled)
	{
		TPacketGCFeatureEnable packet;
		packet.header = HEADER_GC_FEATURE_ENABLE;
		packet.feature_index = bFeatureID;
		packet.enabled = bEnabled;
		pkDesc->Packet(&packet, sizeof(packet));
	}

	const std::vector<const char*>& GetFeatureList()
	{
		static std::vector<const char*> vecNameList;
		static bool bInitialized = false;

		if (!bInitialized)
		{
			vecNameList.push_back("TEST");
			vecNameList.push_back("WOLFMAN");
			vecNameList.push_back("WEAPON_RARITY");
			vecNameList.push_back("PRIVATESHOP_SEARCH");
			vecNameList.push_back("SKILL_SELECT_EX");
			vecNameList.push_back("HAIR_SELECT_EX");
			vecNameList.push_back("POINT_BOXES");
			vecNameList.push_back("CHEQUE");
			vecNameList.push_back("ACCE");
			vecNameList.push_back("CHANGELOOK");
			vecNameList.push_back("COMBINATION");
			vecNameList.push_back("GEM");
			vecNameList.push_back("CHANNEL_CHANGE");
			vecNameList.push_back("AURA");
			vecNameList.push_back("PET");
			vecNameList.push_back("COSTUME_SET");
			vecNameList.push_back("SAFEBOX_MONEY");
			vecNameList.push_back("VALUE_PACK");
			vecNameList.push_back("BOSS_TRACKING");
			vecNameList.push_back("DC_COUPON");
			vecNameList.push_back("MAIL_BOX");
			vecNameList.push_back("REMOVE_SKILL_AFFECT");
			vecNameList.push_back("OPEN_GIFTBOX");
			vecNameList.push_back("TRANSFER_MONEY");
			vecNameList.push_back("GUILD_RANK");
			vecNameList.push_back("AVERAGE_PRICE");
			vecNameList.push_back("SHOP_MONEY");
			bInitialized = true;
		}

		return vecNameList;
	}

	void SetFeatureEnabled(BYTE bFeatureID, bool bEnabled, bool bP2P)
	{
		switch (bFeatureID)
		{
			case features::TEST:
				SetTestEnabled(bEnabled, bP2P);
				break;

			case features::WOLFMAN:
				SetWolfmanEnabled(bEnabled, bP2P);
				break;
				
			case features::WEAPON_RARITY:
				SetWeaponRarityEnabled(bEnabled, bP2P);
				break;
				
			case features::PRIVATESHOP_SEARCH:
				SetPrivateShopSearchEnabled(bEnabled, bP2P);
				break;
				
			case features::SKILL_SELECT_EX:
				SetSkillSelectExEnabled(bEnabled, bP2P);
				break;
				
			case features::HAIR_SELECT_EX:
				SetHairSelectExEnabled(bEnabled, bP2P);
				break;
				
			case features::POINT_BOXES:
				SetPointBoxesEnabled(bEnabled, bP2P);
				break;
				
			case features::CHEQUE:
				SetChequeEnabled(bEnabled, bP2P);
				break;
				
			case features::ACCE:
				SetAcceEnabled(bEnabled, bP2P);
				break;
				
			case features::CHANGELOOK:
				SetChangeLookEnabled(bEnabled, bP2P);
				break;
				
			case features::COMBINATION:
				SetCombinationEnabled(bEnabled, bP2P);
				break;
				
			case features::SPECIAL_INVENTORY:
				SetSpecialInventoryEnabled(bEnabled, bP2P);
				break;
				
			case features::GEM:
				SetGemEnabled(bEnabled, bP2P);
				break;
				
			case features::CHANNEL_CHANGE:
				SetChannelChangeEnabled(bEnabled, bP2P);
				break;
				
			case features::AURA:
				SetAuraEnabled(bEnabled, bP2P);
				break;
				
			case features::PET:
				SetPetEnabled(bEnabled, bP2P);
				break;
				
			case features::COSTUME_SET:
				SetCostumeSetEnabled(bEnabled, bP2P);
				break;
				
			case features::SAFEBOX_MONEY:
				SetSafeboxMoneyEnabled(bEnabled, bP2P);
				break;
				
			case features::VALUE_PACK:
				SetValuePackEnabled(bEnabled, bP2P);
				break;
			
			case features::BOSS_TRACKING:
				SetBossTrackingEnabled(bEnabled, bP2P);
				break;
				
			case features::DC_COUPON:
				SetDcCouponEnabled(bEnabled, bP2P);
				break;
				
			case features::MAIL_BOX:
				SetMailBoxEnabled(bEnabled, bP2P);
				break;
				
			case features::REMOVE_SKILL_AFFECT:
				SetRemoveSkillAffectEnabled(bEnabled, bP2P);
				break;
				
			case features::OPEN_GIFTBOX:
				SetOpenGiftboxEnabled(bEnabled, bP2P);
				break;
				
			case features::TRANSFER_MONEY:
				SetTransferMoneyEnabled(bEnabled, bP2P);
				break;
				
			case features::GUILD_RANK:
				SetGuildRankEnabled(bEnabled, bP2P);
				break;
				
			case features::AVERAGE_PRICE:
				SetAveragePriceEnabled(bEnabled, bP2P);
				break;
				
			case features::SHOP_MONEY:
				SetShopMoneyEnabled(bEnabled, bP2P);
				break;

			default:
				sys_err("unkown feature index %d", bFeatureID);
				break;
		}
	}

	bool IsFeatureEnabled(BYTE bFeatureID)
	{
		switch (bFeatureID)
		{
			case features::TEST:
				return gbTest_Enabled;
				break;
				
			case features::WOLFMAN:
				return gbWolfman_Enabled;
				break;
			
			case features::WEAPON_RARITY:
				return gbWeaponRarity_Enabled;
				break;
				
			case features::PRIVATESHOP_SEARCH:
				return gbPrivateShopSearch_Enabled;
				break;
				
			case features::SKILL_SELECT_EX:
				return gbSkillSelectEx_Enabled;
				break;
				
			case features::HAIR_SELECT_EX:
				return gbHairSelectEx_Enabled;
				break;
				
			case features::POINT_BOXES:
				return gbPointBoxes_Enabled;
				break;
				
			case features::CHEQUE:
				return gbCheque_Enabled;
				break;
				
			case features::ACCE:
				return gbAcce_Enabled;
				break;
				
			case features::CHANGELOOK:
				return gbChangeLook_Enabled;
				break;
				
			case features::COMBINATION:
				return gbCombination_Enabled;
				break;
				
			case features::SPECIAL_INVENTORY:
				return gbSpecialInventory_Enabled;
				break;
				
			case features::GEM:
				return gbGem_Enabled;
				break;
				
			case features::CHANNEL_CHANGE:
				return gbChannelChange_Enabled;
				break;
				
			case features::AURA:
				return gbAura_Enabled;
				break;
				
			case features::PET:
				return gbPet_Enabled;
				break;
				
			case features::COSTUME_SET:
				return gbCostumeSet_Enabled;
				break;
				
			case features::SAFEBOX_MONEY:
				return gbSafeboxMoney_Enabled;
				break;
				
			case features::VALUE_PACK:
				return gbValuePack_Enabled;
				break;
				
			case features::BOSS_TRACKING:
				return gbBossTracking_Enabled;
				break;
				
			case features::DC_COUPON:
				return gbDcCoupon_Enabled;
				break;
				
			case features::MAIL_BOX:
				return gbMailBox_Enabled;
				break;
				
			case features::REMOVE_SKILL_AFFECT:
				return gbRemoveSkillAffect_Enabled;
				break;
				
			case features::OPEN_GIFTBOX:
				return gbOpenGiftBox_Enabled;
				break;
				
			case features::TRANSFER_MONEY:
				return gbTransferMoney_Enabled;
				break;
				
			case features::GUILD_RANK:
				return gbGuildRank_Enabled;
				break;
				
			case features::AVERAGE_PRICE:
				return gbAveragePrice_Enabled;
				break;
				
			case features::SHOP_MONEY:
				return gbShopMoney_Enabled;
				break;
				
			default:
				sys_err("unkown feature index %d", bFeatureID);
				break;
		}
	}

	void LoadPlayer(LPCHARACTER pkChr)
	{
		for (int i = 0; i < features::FEATURES_MAX_NUM; ++i)
			SendClientPacket(pkChr->GetDesc(), i, IsFeatureEnabled(i));
	}

	void SetTestEnabled(bool bEnabled, bool bP2P)
	{
		if (gbTest_Enabled == bEnabled)
			return;

		if (!bP2P)
			SendP2PPacket(features::TEST, bEnabled);

		gbTest_Enabled = bEnabled;
	}
	
	void SetWolfmanEnabled(bool bEnabled, bool bP2P)
	{
		if (gbWolfman_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::WOLFMAN, bEnabled);
		
		gbWolfman_Enabled = bEnabled;
	}

	void SetWeaponRarityEnabled(bool bEnabled, bool bP2P)
	{
		if (gbWeaponRarity_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::WEAPON_RARITY, bEnabled);
		
		gbWeaponRarity_Enabled = bEnabled;
	}
	
	void SetPrivateShopSearchEnabled(bool bEnabled, bool bP2P)
	{
		if (gbPrivateShopSearch_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::PRIVATESHOP_SEARCH, bEnabled);
		
		gbPrivateShopSearch_Enabled = bEnabled;
	}
	
	void SetSkillSelectExEnabled(bool bEnabled, bool bP2P)
	{
		if (gbSkillSelectEx_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::SKILL_SELECT_EX, bEnabled);
		
		gbSkillSelectEx_Enabled = bEnabled;
	}
	
	void SetHairSelectExEnabled(bool bEnabled, bool bP2P)
	{
		if (gbHairSelectEx_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::HAIR_SELECT_EX, bEnabled);
		
		gbHairSelectEx_Enabled = bEnabled;
	}
	
	void SetPointBoxesEnabled(bool bEnabled, bool bP2P)
	{
		if (gbPointBoxes_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::POINT_BOXES, bEnabled);
		
		gbPointBoxes_Enabled = bEnabled;
	}
	
	void SetChequeEnabled(bool bEnabled, bool bP2P)
	{
		if (gbCheque_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::CHEQUE, bEnabled);
		
		gbCheque_Enabled = bEnabled;
	}
	
	void SetAcceEnabled(bool bEnabled, bool bP2P)
	{
		if (gbAcce_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::ACCE, bEnabled);
		
		gbAcce_Enabled = bEnabled;
	}
	
	void SetChangeLookEnabled(bool bEnabled, bool bP2P)
	{
		if (gbChangeLook_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::ACCE, bEnabled);
		
		gbChangeLook_Enabled = bEnabled;
	}
	
	void SetCombinationEnabled(bool bEnabled, bool bP2P)
	{
		if (gbCombination_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::COMBINATION, bEnabled);
		
		gbCombination_Enabled = bEnabled;
	}
	
	void SetSpecialInventoryEnabled(bool bEnabled, bool bP2P)
	{
		if (gbSpecialInventory_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::SPECIAL_INVENTORY, bEnabled);
		
		gbSpecialInventory_Enabled = bEnabled;
	}
	
	void SetGemEnabled(bool bEnabled, bool bP2P)
	{
		if (gbGem_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::GEM, bEnabled);
		
		gbGem_Enabled = bEnabled;
	}
	
	void SetChannelChangeEnabled(bool bEnabled, bool bP2P)
	{
		if (gbChannelChange_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::CHANNEL_CHANGE, bEnabled);
		
		gbChannelChange_Enabled = bEnabled;
	}
	
	void SetAuraEnabled(bool bEnabled, bool bP2P)
	{
		if (gbAura_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::AURA, bEnabled);
		
		gbAura_Enabled = bEnabled;
	}
	
	void SetPetEnabled(bool bEnabled, bool bP2P)
	{
		if (gbPet_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::PET, bEnabled);
		
		gbPet_Enabled = bEnabled;
	}
	
	void SetCostumeSetEnabled(bool bEnabled, bool bP2P)
	{
		if (gbCostumeSet_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::COSTUME_SET, bEnabled);
		
		gbCostumeSet_Enabled = bEnabled;
	}
	
	void SetSafeboxMoneyEnabled(bool bEnabled, bool bP2P)
	{
		if (gbSafeboxMoney_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::SAFEBOX_MONEY, bEnabled);
		
		gbSafeboxMoney_Enabled = bEnabled;
	}
	
	void SetValuePackEnabled(bool bEnabled, bool bP2P)
	{
		if (gbValuePack_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::VALUE_PACK, bEnabled);
		
		gbValuePack_Enabled = bEnabled;
	}
	
	void SetBossTrackingEnabled(bool bEnabled, bool bP2P)
	{
		if (gbBossTracking_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::BOSS_TRACKING, bEnabled);
		
		gbBossTracking_Enabled = bEnabled;
	}
	
	void SetDcCouponEnabled(bool bEnabled, bool bP2P)
	{
		if (gbDcCoupon_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::DC_COUPON, bEnabled);
		
		gbDcCoupon_Enabled = bEnabled;
	}
	
	void SetMailBoxEnabled(bool bEnabled, bool bP2P)
	{
		if (gbMailBox_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::MAIL_BOX, bEnabled);
		
		gbMailBox_Enabled = bEnabled;
	}
	
	void SetRemoveSkillAffectEnabled(bool bEnabled, bool bP2P)
	{
		if (gbRemoveSkillAffect_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::REMOVE_SKILL_AFFECT, bEnabled);
		
		gbRemoveSkillAffect_Enabled = bEnabled;
	}
	
	void SetOpenGiftboxEnabled(bool bEnabled, bool bP2P)
	{
		if (gbOpenGiftBox_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::OPEN_GIFTBOX, bEnabled);
		
		gbOpenGiftBox_Enabled = bEnabled;
	}
	
	void SetTransferMoneyEnabled(bool bEnabled, bool bP2P)
	{
		if (gbTransferMoney_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::TRANSFER_MONEY, bEnabled);
		
		gbTransferMoney_Enabled = bEnabled;
	}
	
	void SetGuildRankEnabled(bool bEnabled, bool bP2P)
	{
		if (gbGuildRank_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::GUILD_RANK, bEnabled);
		
		gbGuildRank_Enabled = bEnabled;
	}
	
	void SetAveragePriceEnabled(bool bEnabled, bool bP2P)
	{
		if (gbAveragePrice_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::AVERAGE_PRICE, bEnabled);
		
		gbAveragePrice_Enabled = bEnabled;
	}
	
	void SetShopMoneyEnabled(bool bEnabled, bool bP2P)
	{
		if (gbShopMoney_Enabled == bEnabled)
			return;
		
		if (!bP2P)
			SendP2PPacket(features::SHOP_MONEY, bEnabled);
		
		gbShopMoney_Enabled = bEnabled;
	}
}
