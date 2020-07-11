#include "stdafx.h"
#include "../../common/VnumHelper.h"
#include "char.h"
#include "config.h"
#include "utils.h"
#include "crc32.h"
#include "char_manager.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "item_manager.h"
#include "motion.h"
#include "vector.h"
#include "packet.h"
#include "cmd.h"
#include "fishing.h"
#include "exchange.h"
#include "battle.h"
#include "affect.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "pvp.h"
#include "party.h"
#include "start_position.h"
#include "questmanager.h"
#include "log.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "unique_item.h"
#include "priv_manager.h"
#include "war_map.h"
#include "xmas_event.h"
#include "banword.h"
#include "target.h"
#include "wedding.h"
#include "mob_manager.h"
#include "mining.h"
#include "arena.h"
#include "horsename_manager.h"
#include "gm.h"
#include "map_location.h"
#include "BlueDragon_Binder.h"
#include "skill_power.h"
#include "buff_on_attributes.h"
#include "OXEvent.h"

#ifdef ENABLE_PET_SYSTEM
#include "PetSystem.h"
#endif
#include "DragonSoul.h"

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
#include <algorithm>
#include <iterator>
using namespace std;
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "New_PetSystem.h"
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "MeleyLair.h"
#endif

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif

#ifdef ENABLE_BUFFI_SYSTEM
#include "support_shaman.h"
#endif

#include "inventory.h"

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

#include <iostream>
#include <memory>

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
#include "MountSystem.h"
#endif

#include "features.h"

extern const BYTE g_aBuffOnAttrPoints;
extern bool RaceToJob(unsigned race, unsigned* ret_job);
extern int test_server;

extern bool IS_SUMMONABLE_ZONE(int map_index); // char_item.cpp
bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index);
bool CAN_OPEN_SAFEBOX(int map_index);

extern int pc_max_movement_speed;
extern int pc_max_attack_speed;
extern int mob_max_movement_speed;
extern int mob_max_attack_speed;

bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index)
{
	switch (map_index)
	{
	case 301:
	case 302:
	case 303:
	case 304:
	case 351:
	case 352:
	case 212:
		if (ch->GetLevel() < 90)
			return false;
	case 353:
	case 354:
	case 355:
		if (ch->GetLevel() < 95)
			return false;
	case 72:
	case 73:
		if (ch->GetLevel() < 75)
			return false;
	case 104:
		if (ch->GetLevel() < 45)
			return false;
	case 71:
		if (ch->GetLevel() < 55)
			return false;
	case 68:
		if (ch->GetLevel() < 65)
			return false;
	case 67:
		if (ch->GetLevel() < 60)
			return false;
	case 220:
		if (ch->GetLevel() < 115)
			return false;
	}
	return true;
}

bool CAN_OPEN_SAFEBOX(int map_index)
{
	switch (map_index)
	{
	case 66:
	case 207:
	case 212:
	case 351:
	case 352:
	case 356:
		return false;
	}
	return true;
}

// <Factor> DynamicCharacterPtr member function definitions

void ELPlainCoord_GetRotatedPixelPosition(int32_t centerX, int32_t centerY, float distance, float rotDegree, int32_t* pdstX, int32_t* pdstY)
{
	float rotRadian = float(3.141592*rotDegree / 180.0f);
	*pdstX = static_cast<int32_t>(centerX + distance*float(sin(static_cast<double>(rotRadian))));
	*pdstY = static_cast<int32_t>(centerY + distance*float(cos(static_cast<double>(rotRadian))));
}

LPCHARACTER DynamicCharacterPtr::Get() const {
	LPCHARACTER p = nullptr;
	if (is_pc) {
		p = CHARACTER_MANAGER::instance().FindByPID(id);
	}
	else {
		p = CHARACTER_MANAGER::instance().Find(id);
	}
	return p;
}

DynamicCharacterPtr& DynamicCharacterPtr::operator=(LPCHARACTER character) {
	if (character == nullptr) {
		Reset();
		return *this;
	}
	if (character->IsPC()) {
		is_pc = true;
		id = character->GetPlayerID();
	}
	else {
		is_pc = false;
		id = character->GetVID();
	}
	return *this;
}

CHARACTER::CHARACTER()
{
	m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateIdle, &CHARACTER::EndStateEmpty);
	m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
	m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateBattle, &CHARACTER::EndStateEmpty);

	Initialize();
}

CHARACTER::~CHARACTER()
{
	Destroy();
}

void CHARACTER::Initialize()
{
	CEntity::Initialize(ENTITY_CHARACTER);

	m_bNoOpenedShop = true;

	m_bOpeningSafebox = false;

	m_fSyncTime = get_float_time() - 3;
	m_dwPlayerID = 0;
	m_dwKillerPID = 0;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_pkRefreshShopEvent = nullptr;
	bprivShop = 0;
	bprivShopOwner = 0;
	bprivShop = 0;
	bprivShopOwner = 0;
	dw_ShopTime = 0;
	m_mapshops.clear();
#endif
#ifdef ENABLE_GIFTBOX_SYSTEM
	m_pkGiftRefresh = nullptr;
	m_dwLastGiftPage = 0;
	m_mapGiftGrid.clear();
#endif

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	dwLastTargetInfoPulse = 0;
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	m_stImmortalSt = 0;

	m_newpetskillcd[0] = 0;
	m_newpetskillcd[1] = 0;
	m_newpetskillcd[2] = 0;
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
	m_bChangeLook = false;
	#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	m_bMountChangeLook = false;
	#endif
#endif
	m_iMoveCount = 0;

	m_pkRegen = nullptr;
	regen_id_ = 0;
	m_posRegen.x = m_posRegen.y = m_posRegen.z = 0;
	m_posStart.x = m_posStart.y = 0;
	m_posDest.x = m_posDest.y = 0;
	m_fRegenAngle = 0.0f;

	m_pkMobData = nullptr;
	m_pkMobInst = nullptr;

	m_pkShop = nullptr;
	m_pkChrShopOwner = nullptr;
	m_pkMyShop = nullptr;
	m_pkExchange = nullptr;
	m_pkParty = nullptr;
	m_pkPartyRequestEvent = nullptr;

	m_pGuild = nullptr;

	m_pkChrTarget = nullptr;

	m_pkMuyeongEvent = nullptr;

	m_pkWarpNPCEvent = nullptr;
	m_pkDeadEvent = nullptr;
	m_pkStunEvent = nullptr;
	m_pkSaveEvent = nullptr;
	m_pkRecoveryEvent = nullptr;
	m_pkTimedEvent = nullptr;
	m_pkFishingEvent = nullptr;
	m_pkWarpEvent = nullptr;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	m_pkCombatZoneLeaveEvent = nullptr;
	m_pkCombatZoneWarpEvent = nullptr;
#endif

	// MINING
	m_pkMiningEvent = nullptr;
	// END_OF_MINING

	m_pkPoisonEvent = nullptr;
#ifdef ENABLE_WOLFMAN_CHARACTER
	m_pkBleedEvent = nullptr;
#endif
	m_pkFireEvent = nullptr;

	m_pkAffectEvent = nullptr;
	m_afAffectFlag = TAffectFlag(0, 0);

	m_pkDestroyWhenIdleEvent = nullptr;

	m_pkChrSyncOwner = nullptr;

	memset(&m_points, 0, sizeof(m_points));
	memset(&m_pointsInstant, 0, sizeof(m_pointsInstant));
	memset(&m_quickslot, 0, sizeof(m_quickslot));
#ifdef ENABLE_MINIGAME_RUMI_EVENT
	memset(&character_cards, 0, sizeof(character_cards));
	memset(&randomized_cards, 0, sizeof(randomized_cards));
#endif
	m_bCharType = CHAR_TYPE_MONSTER;

	SetPosition(POS_STANDING);

	m_dwPlayStartTime = m_dwLastMoveTime = get_dword_time();

	GotoState(m_stateIdle);
	m_dwStateDuration = 1;

	m_dwLastAttackTime = std::min(0UL, static_cast<DWORD>(get_dword_time()) - 120000UL); //2m ago (for some reason?)

	m_bAddChrState = 0;

	m_pkChrStone = nullptr;

	m_pkSafebox = nullptr;
	m_iSafeboxSize = -1;
	m_iSafeboxLoadTime = 0;

	m_pkMall = nullptr;
	m_iMallLoadTime = 0;

	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;
	m_lWarpMapIndex = 0;

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;

	m_pSkillLevels = nullptr;

	m_dwMoveStartTime = 0;
	m_dwMoveDuration = 0;

	m_dwFlyTargetID = 0;

	m_dwNextStatePulse = 0;

	m_dwLastDeadTime = get_dword_time() - 180000;

	m_bSkipSave = false;

	m_bItemLoaded = false;

	m_bHasPoisoned = false;
#ifdef ENABLE_WOLFMAN_CHARACTER
	m_bHasBleeded = false;
#endif
	m_pkDungeon = nullptr;
	m_iEventAttr = 0;

	m_kAttackLog.dwVID = 0;
	m_kAttackLog.dwTime = 0;

	m_bNowWalking = m_bWalking = false;
	ResetChangeAttackPositionTime();

	m_bDetailLog = false;
	m_bMonsterLog = false;

	m_bDisableCooltime = false;

#ifdef ENABLE_GUILD_SPY_SYSTEM
	m_iWarKillCount = 0;
	m_iWarDeadCount = 0;
#endif
	m_iAlignment = 0;
	m_iRealAlignment = 0;

	m_iKillerModePulse = 0;
	m_bPKMode = PK_MODE_PEACE;

	m_dwQuestNPCVID = 0;
	m_dwQuestByVnum = 0;
	m_pQuestItem = nullptr;

	m_dwUnderGuildWarInfoMessageTime = get_dword_time() - 60000;

	m_bUnderRefine = false;

	// REFINE_NPC
	m_dwRefineNPCVID = 0;
	// END_OF_REFINE_NPC

	m_dwPolymorphRace = 0;

	m_bStaminaConsume = false;

	ResetChainLightningIndex();

	m_dwMountVnum = 0;
	m_chHorse = nullptr;
	m_chRider = nullptr;

	m_pWarMap = nullptr;
	m_pWeddingMap = nullptr;
	m_bChatCounter = 0;

	ResetStopTime();

	m_dwLastVictimSetTime = get_dword_time() - 3000;
	m_iMaxAggro = -100;

	m_bSendHorseLevel = 0;
	m_bSendHorseHealthGrade = 0;
	m_bSendHorseStaminaGrade = 0;

	m_dwLoginPlayTime = 0;

	m_pkChrMarried = nullptr;

	m_posSafeboxOpen.x = -1000;
	m_posSafeboxOpen.y = -1000;

	// EQUIP_LAST_SKILL_DELAY
	m_dwLastSkillTime = get_dword_time();
	// END_OF_EQUIP_LAST_SKILL_DELAY

	// MOB_SKILL_COOLTIME
	memset(m_adwMobSkillCooltime, 0, sizeof(m_adwMobSkillCooltime));
	// END_OF_MOB_SKILL_COOLTIME

	// ARENA
	m_pArena = nullptr;
	m_nPotionLimit = quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count");
	// END_ARENA

	//PREVENT_TRADE_WINDOW
	m_isOpenSafebox = 0;
	//END_PREVENT_TRADE_WINDOW

	//PREVENT_REFINE_HACK
	m_iRefineTime = 0;
	//END_PREVENT_REFINE_HACK

	//RESTRICT_USE_SEED_OR_MOONBOTTLE
	m_iSeedTime = 0;
	//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
	//PREVENT_PORTAL_AFTER_EXCHANGE
	m_iExchangeTime = 0;
	//END_PREVENT_PORTAL_AFTER_EXCHANGE
	//
	m_iSafeboxLoadTime = 0;

	m_iMyShopTime = 0;

	m_deposit_pulse = 0;

	m_strNewName.clear();

	m_known_guild.clear();

	m_dwLogOffInterval = 0;
	m_dwLastComboTime = 0;
	m_bComboIndex = 0;
	m_dwSkipComboAttackByTime = 0;

	m_dwMountTime = 0;

	m_dwLastGoldDropTime = 0;

	m_bIsLoadedAffect = false;
	cannot_dead = false;

#ifdef ENABLE_PET_SYSTEM
	m_petSystem = 0;
	m_bIsPet = false;
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	m_newpetSystem = 0;
	m_bIsNewPet = false;
	m_eggvid = 0;
#endif

	m_fAttMul = 1.0f;
	m_fDamMul = 1.0f;

	m_pointsInstant.iDragonSoulActiveDeck = -1;

	memset(&m_tvLastSyncTime, 0, sizeof(m_tvLastSyncTime));
	m_bShopValidPos = true;

	memset(itemAward_cmd, 0, 20);

#ifdef ENABLE_DROP_HACK_FIX
	CountDrops = 0;
	LastDropTime = 0;
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	m_iCombatZonePoints = 0;
	m_iCombatZoneDeaths = 0;
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	m_SupportShaman = 0;
	m_dwSupportShamanWeapon = 0;
	m_dwSupportShamanCostume = 0;
#ifdef ENABLE_ACCE_SYSTEM
	m_dwSupportShamanSash = 0;
#endif
	m_bIsSupportShaman = false;
	m_Owner = nullptr;
#endif

#ifdef ENABLE_ACCE_SYSTEM
	m_bSashCombination = false;
	m_bSashAbsorption = false;
#endif

#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	m_pkViewEquipRequestEvent = nullptr;
#endif

#ifdef ENABLE_CAMP_FIRE_FIX
	LastCampFireUse = 0;
#endif
#ifdef ENABLE_ANTI_CMD_FLOOD
	m_dwCmdAntiFloodCount = 0;
	m_dwCmdAntiFloodPulse = 0;
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	SetExtendedInventory(0);
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
	m_fishSlots = nullptr;
	m_dwFishUseCount = 0;
	m_bFishAttachedShape = 0;
#endif

#ifdef ENABLE_CAPE_EFFECT_FIX
	m_iCapeEffectPulse = 0;
#endif

#ifdef ENABLE_GEM_SYSTEM
	m_gemItems = nullptr;
	m_dwGemNextRefresh = 0;
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	m_gemBossItems = nullptr;
	m_dwGemBossNextRefresh = 0;
	#endif
#endif

#ifdef ENABLE_ATTENDANCE_EVENT
	m_hitCount.clear();
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
	m_vecCatchKingFieldCards.clear();
	bCatchKingHandCard = 0;
	bCatchKingHandCardLeft = 0;
	bCatchKingBetSetNumber = 0;
	dwCatchKingTotalScore = 0;
	dwCatchKingGameStatus = false;
#endif

#ifdef ENABLE_AFK_MODE_SYSTEM
	m_pkUpdateCharacter = nullptr;
	m_isAway = false;
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM
	m_isOpenMailBox = 0;
	LOAD_CONFIG_MAILS();
#endif

#ifdef ENABLE_COSTUME_SET_SYSTEM
	m_bHalloweenSetStatus = 0;
#endif

	m_map_affectStack.clear();
	
	//Inventory
	m_inventory = nullptr;
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	m_pkZodiac = nullptr;
	m_dwZodiacCzLastTime = 0;
	m_dwLastZodiacAttackTime = 0;
	m_dwDeadCount = 0;
#endif

#ifdef ENABLE_ABUSE_SYSTEM
	m_bIsSuspectWaitDmg = false;
	m_abuse = nullptr;
#endif

#ifdef ENABLE_WALLHACK_FIX
	m_pkCheckWallHackEvent = nullptr;
	m_lastMoveAblePos.x = 0;
	m_lastMoveAblePos.y = 0;
	m_lastMoveAblePos.z = 0;
#endif

#ifdef ENABLE_FISHBOT_FIX
	m_iLastFishTime = 0;
	m_iFishHackCount = 0;
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	m_mountSystem = 0;
	m_bIsMount = false;
	m_bMountCounter = 0;
#endif

#ifdef ENABLE_DEBUG_MODE_SYSTEM
	m_bDebug = 0;
#endif

#ifdef ENABLE_AURA_SYSTEM
	m_bAuraRefine = false;
	m_bAuraAbsorption = false;
#endif

#ifdef ENABLE_BOSS_GEM_SYSTEM
	m_bGemShopType = false;
#endif

#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
	m_dwSpecialGachaVnum = 0;
	m_dwSpecialGachaCount = 0;
	m_dwSpecialGachaGiveVnum = 0;
	m_dwSpecialGachaGiveCount = 0;
#endif

#ifdef ENABLE_GYEONGGONG_SKILL
	m_pkGyeongGongEvent = NULL;
#endif
}

void CHARACTER::Create(const char* c_pszName, DWORD vid, bool isPC)
{
	static int s_crc = 172814;

	char crc_string[128 + 1];
	snprintf(crc_string, sizeof(crc_string), "%s%p%d", c_pszName, this, ++s_crc);
	m_vid = VID(vid, GetCRC32(crc_string, strlen(crc_string)));

	if (isPC)
	{
		m_stName = c_pszName;
		m_inventory = std::make_shared<CInventory>(this);
#ifdef ENABLE_ABUSE_SYSTEM
		m_abuse = spAbuseController(new AbuseController(this));
#endif
	}
}

void CHARACTER::Destroy()
{
	CloseMyShop();

#ifdef ENABLE_GIFTBOX_SYSTEM
	event_cancel(&m_pkGiftRefresh);
	m_dwLastGiftPage = 0;
	m_mapGiftGrid.clear();
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	event_cancel(&m_pkRefreshShopEvent);
	bprivShop = 0;
	bprivShopOwner = 0;
	dw_ShopTime = 0;
	m_mapshops.clear();
#endif

	if (m_pkRegen)
	{
		if (m_pkDungeon) {
			// Dungeon regen may not be valid at this point
			if (m_pkDungeon->IsValidRegen(m_pkRegen, regen_id_)) {
				--m_pkRegen->count;
			}
		}
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		else if (m_pkZodiac)
		{
			if (m_pkZodiac->IsValidRegen(m_pkRegen, regen_id_))
			{
				--m_pkRegen->count;
			}
		}
#endif
		else {
			// Is this really safe?
			--m_pkRegen->count;
		}
		m_pkRegen = nullptr;
	}

	if (m_pkDungeon)
	{
		SetDungeon(nullptr);
	}
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (m_pkZodiac)
	{
		SetZodiac(nullptr);
	}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (m_mountSystem)
	{
		m_mountSystem->Destroy();
		delete m_mountSystem;
		m_mountSystem = 0;
	}
	if (GetMountVnum())
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
	}
	HorseSummon(false);
#endif

#ifdef ENABLE_PET_SYSTEM
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;

		m_petSystem = 0;
	}
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	if (m_SupportShaman)
	{
		m_SupportShaman->Destroy();
		delete m_SupportShaman;

		m_SupportShaman = 0;
	}
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;

		m_newpetSystem = 0;
	}
#endif

	HorseSummon(false);

	if (GetRider())
		GetRider()->ClearHorseInfo();

	if (GetDesc())
	{
		GetDesc()->BindCharacter(nullptr);
		//		BindDesc(nullptr);
	}

	if (m_pkExchange)
		m_pkExchange->Cancel();

	SetVictim(nullptr);

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(nullptr);
	}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (IsPC())
	{
		CCombatZoneManager::instance().OnLogout(this);
	}
#endif

	ClearStone();
	ClearSync();
	ClearTarget();

	if (nullptr == m_pkMobData)
	{
		DragonSoul_CleanUp();
		ClearItem();
	}

	// <Factor> m_pkParty becomes nullptr after CParty destructor call!
	LPPARTY party = m_pkParty;
	if (party)
	{
		if (party->GetLeaderPID() == GetVID() && !IsPC())
		{
			M2_DELETE(party);
		}
		else
		{
			party->Unlink(this);

			if (!IsPC())
				party->Quit(GetVID());
		}

		SetParty(nullptr); // 안해도 되지만 안전하게.
	}

	if (m_pkMobInst)
	{
		M2_DELETE(m_pkMobInst);
		m_pkMobInst = nullptr;
	}

	m_pkMobData = nullptr;

	if (m_pkSafebox)
	{
		M2_DELETE(m_pkSafebox);
		m_pkSafebox = nullptr;
	}

	if (m_pkMall)
	{
		M2_DELETE(m_pkMall);
		m_pkMall = nullptr;
	}

	//40k buff fix
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		if (nullptr != it->second)
		{
			M2_DELETE(it->second);
		}
	}
	m_map_buff_on_attrs.clear();
	//40k buff fix
	m_set_pkChrSpawnedBy.clear();

	StopMuyeongEvent();
#ifdef ENABLE_GYEONGGONG_SKILL
	StopGyeongGongEvent();
#endif
	event_cancel(&m_pkWarpNPCEvent);
	event_cancel(&m_pkRecoveryEvent);
	event_cancel(&m_pkDeadEvent);
	event_cancel(&m_pkSaveEvent);
	event_cancel(&m_pkTimedEvent);
	event_cancel(&m_pkStunEvent);
	event_cancel(&m_pkFishingEvent);
	event_cancel(&m_pkPoisonEvent);
#ifdef ENABLE_WOLFMAN_CHARACTER
	event_cancel(&m_pkBleedEvent);
#endif
	event_cancel(&m_pkFireEvent);
	event_cancel(&m_pkPartyRequestEvent);
	//DELAYED_WARP
	event_cancel(&m_pkWarpEvent);
	//END_DELAYED_WARP
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	event_cancel(&m_pkCombatZoneLeaveEvent);
	event_cancel(&m_pkCombatZoneWarpEvent);
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	event_cancel(&m_pkViewEquipRequestEvent);
#endif
#ifdef ENABLE_AFK_MODE_SYSTEM
	event_cancel(&m_pkUpdateCharacter);
#endif
#ifdef ENABLE_WALLHACK_FIX
	event_cancel(&m_pkCheckWallHackEvent);
#endif
	// RECALL_DELAY
	//event_cancel(&m_pkRecallEvent);
	// END_OF_RECALL_DELAY

	// MINING
	event_cancel(&m_pkMiningEvent);
	// END_OF_MINING

	for (auto it = m_mapMobSkillEvent.begin(); it != m_mapMobSkillEvent.end(); ++it)
	{
		LPEVENT pkEvent = it->second;
		event_cancel(&pkEvent);
	}
	m_mapMobSkillEvent.clear();

	//event_cancel(&m_pkAffectEvent);
	ClearAffect();

	event_cancel(&m_pkDestroyWhenIdleEvent);

	if (m_pSkillLevels)
	{
		M2_DELETE_ARRAY(m_pSkillLevels);
		m_pSkillLevels = nullptr;
	}

#ifdef ENABLE_FISH_JIGSAW_EVENT
	if (m_fishSlots)
	{
		M2_DELETE_ARRAY(m_fishSlots);
		m_fishSlots = nullptr;
	}
#endif

#ifdef ENABLE_GEM_SYSTEM
	if (m_gemItems)
	{
		M2_DELETE_ARRAY(m_gemItems);
		m_gemItems = nullptr;
	}
#endif
	
#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (m_gemBossItems)
	{
		M2_DELETE_ARRAY(m_gemBossItems);
		m_gemBossItems = nullptr;
	}
#endif

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);

	if (m_bMonsterLog)
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
}

const char* CHARACTER::GetName() const
{
	return m_stName.empty() ? (m_pkMobData ? m_pkMobData->m_table.szLocaleName : "") : m_stName.c_str();
}

void CHARACTER::CloseMyShop()
{
	if (GetMyShop())
	{
		m_stShopSign.clear();
		CShopManager::instance().DestroyPCShop(this);
		m_pkMyShop = nullptr;

		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_SHOP_SIGN;
		p.dwVID = GetVID();
		p.szSign[0] = '\0';

		PacketAround(&p, sizeof(p));

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if (IsPrivShop())
		{
			M2_DESTROY_CHARACTER(this);
			return;
		}
#endif
		SetPolymorph(GetJob(), true);
	}
}

void EncodeMovePacket(TPacketGCMove & pack, DWORD dwVID, BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, BYTE bRot)
{
	pack.bHeader = HEADER_GC_MOVE;
	pack.bFunc = bFunc;
	pack.bArg = bArg;
	pack.dwVID = dwVID;
	pack.dwTime = dwTime ? dwTime : get_dword_time();
	pack.bRot = bRot;
	pack.lX = x;
	pack.lY = y;
	pack.dwDuration = dwDuration;
}

void CHARACTER::RestartAtSamePos()
{
	if (m_bIsObserver)
		return;

	EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		EncodeRemovePacket(entity);
		if (!m_bIsObserver)
			EncodeInsertPacket(entity);

		if (entity->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER lpChar = (LPCHARACTER)entity;
			if (lpChar->IsPC() || lpChar->IsNPC() || lpChar->IsMonster())
			{
				if (!entity->IsObserverMode())
					entity->EncodeInsertPacket(this);
			}
		}
		else
		{
			if (!entity->IsObserverMode())
			{
				entity->EncodeInsertPacket(this);
			}
		}
	}
}

// Entity에 내가 나타났다고 패킷을 보낸다.
void CHARACTER::EncodeInsertPacket(LPENTITY entity)
{
	LPDESC d;

	if (!(d = entity->GetDesc()))
		return;

	// 길드이름 버그 수정 코드
	LPCHARACTER ch = (LPCHARACTER)entity;
	ch->SendGuildName(GetGuild());
	// 길드이름 버그 수정 코드

	TPacketGCCharacterAdd pack;

	pack.header = HEADER_GC_CHARACTER_ADD;
	pack.dwVID = m_vid;
	pack.bType = GetCharType();
	pack.angle = GetRotation();
	pack.x = GetX();
	pack.y = GetY();
	pack.z = GetZ();
	pack.wRaceNum = GetRaceNum();
#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (IsPet() || IsNewPet() || IsSupportShaman())
#else
	if (IsPet() || IsSupportShaman())
#endif
	{
		pack.bMovingSpeed = 200;
	}
	else
	{
		pack.bMovingSpeed = GetLimitPoint(POINT_MOV_SPEED);
	}
#if defined(WJ_SHOP_MOB_INFO)
	if (IsMonster() || IsStone() /*|| IsGrowthPet()*/)
	{
		pack.dwLevel = GetLevel();
		pack.dwAIFlag = IsMonster() ? GetAIFlag() : 0;
	}
	else
	{
		pack.dwLevel = 0;
		pack.dwAIFlag = 0;
	}
#endif
	pack.bAttackSpeed = GetLimitPoint(POINT_ATT_SPEED);
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];

	pack.bStateFlag = m_bAddChrState;

	int iDur = 0;

	if (m_posDest.x != pack.x || m_posDest.y != pack.y)
	{
		iDur = (m_dwMoveStartTime + m_dwMoveDuration) - get_dword_time();

		if (iDur <= 0)
		{
			pack.x = m_posDest.x;
			pack.y = m_posDest.y;
		}
	}

	d->Packet(&pack, sizeof(pack));

	if (IsPC() == true || m_bCharType == CHAR_TYPE_NPC)
	{
		TPacketGCCharacterAdditionalInfo addPacket;
		memset(&addPacket, 0, sizeof(TPacketGCCharacterAdditionalInfo));

		addPacket.header = HEADER_GC_CHAR_ADDITIONAL_INFO;
		addPacket.dwVID = m_vid;

		addPacket.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
		addPacket.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
		addPacket.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
		addPacket.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
#ifdef ENABLE_ACCE_SYSTEM
		if (IsPC())
			addPacket.awPart[CHR_EQUIPPART_SASH] = GetQuestFlag("game_option.hide_sash") ? 0 : GetPart(PART_SASH);
		else
			addPacket.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
#endif
#ifdef ENABLE_AURA_SYSTEM
		addPacket.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif

		addPacket.bPKMode = m_bPKMode;
		addPacket.dwMountVnum = GetMountVnum();
		addPacket.bEmpire = m_bEmpire;
		
		if (IsPC())
			addPacket.dwLevel = GetQuestFlag("game_option.level_view") ? 0 : GetLevel();
		else
			addPacket.dwLevel = GetLevel();
		strlcpy(addPacket.name, GetName(), sizeof(addPacket.name));

#ifdef ENABLE_QUIVER_SYSTEM
		addPacket.dwArrow = (IsPC()&&GetWear(WEAR_ARROW)!=NULL)?GetWear(WEAR_ARROW)->GetOriginalVnum():0;
#endif

		////////////////////////////////////////
		///////
		addPacket.dwGuildID = 0;
#ifdef ENABLE_GUILD_LEADER_SYSTEM
		addPacket.bMemberType = 0;
#endif

		CGuild* g = GetGuild();

		if (g != nullptr)
		{
			addPacket.dwGuildID = g->GetID();

#ifdef ENABLE_GUILD_LEADER_SYSTEM
			if (IsPC())
			{
				TGuildMember* gm = g->GetMember(GetPlayerID());

				if (gm->grade == GUILD_LEADER_GRADE)
					addPacket.bMemberType = 1;
				else if (gm->is_general)
					addPacket.bMemberType = 2;
			}
#endif
		}
		///////
		////////////////////////////////////////

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
		addPacket.dwWeaponRareLv = 0;
		if (IsPC())
		{
			if ((GetWear(WEAR_WEAPON) && IS_SET(GetWear(WEAR_WEAPON)->GetFlag(), ITEM_FLAG_RARE_ABILITY)) && features::IsFeatureEnabled(features::WEAPON_RARITY))
				addPacket.dwWeaponRareLv = GetWear(WEAR_WEAPON)->GetRarePoints();;
		}
#endif

		if (IsPC())
			addPacket.sAlignment = m_iAlignment / 10;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
		if (IsPC())
			addPacket.combat_zone_rank = GetCombatZoneRank();
#endif

#ifdef ENABLE_BUFFI_SYSTEM
		addPacket.bIsSupportShaman = IsSupportShaman();
#endif

#ifdef ENABLE_EFFECT_STONE_SYSTEM
		if (IsPC())
		{
			if (GetWear(WEAR_BODY))
				addPacket.bStoneEffect = GetWear(WEAR_BODY)->GetAddedEffectStone();
			if (GetWear(WEAR_WEAPON))
				addPacket.bWeaponStoneEffect = GetWear(WEAR_WEAPON)->GetAddedEffectStoneWeapon();
		}
#endif

		d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));
	}

	if (iDur)
	{
		TPacketGCMove pack;
		EncodeMovePacket(pack, GetVID(), FUNC_MOVE, 0, m_posDest.x, m_posDest.y, iDur, 0, (BYTE)(GetRotation() / 5));
		d->Packet(&pack, sizeof(pack));

		TPacketGCWalkMode p;
		p.vid = GetVID();
		p.header = HEADER_GC_WALK_MODE;
		p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

		d->Packet(&p, sizeof(p));
	}

	if (entity->IsType(ENTITY_CHARACTER) && GetDesc())
	{
		LPCHARACTER ch = (LPCHARACTER)entity;
		if (ch->IsWalking())
		{
			TPacketGCWalkMode p;
			p.vid = ch->GetVID();
			p.header = HEADER_GC_WALK_MODE;
			p.mode = ch->m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;
			GetDesc()->Packet(&p, sizeof(p));
		}
	}

	if (GetMyShop())
	{
		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_SHOP_SIGN;
		p.dwVID = GetVID();
		strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));
		d->Packet(&p, sizeof(TPacketGCShopSign));
	}

	if (entity->IsType(ENTITY_CHARACTER))
	{
		sys_log(3, "EntityInsert %s (RaceNum %d) (%d %d) TO %s",
			GetName(), GetRaceNum(), GetX() / SECTREE_SIZE, GetY() / SECTREE_SIZE, ((LPCHARACTER)entity)->GetName());
	}
}

void CHARACTER::EncodeRemovePacket(LPENTITY entity)
{
	if (entity->GetType() != ENTITY_CHARACTER)
		return;

	LPDESC d;

	if (!(d = entity->GetDesc()))
		return;

	TPacketGCCharacterDelete pack;

	pack.header = HEADER_GC_CHARACTER_DEL;
	pack.id = m_vid;

	d->Packet(&pack, sizeof(TPacketGCCharacterDelete));

	if (entity->IsType(ENTITY_CHARACTER))
		sys_log(3, "EntityRemove %s(%d) FROM %s", GetName(), (DWORD)m_vid, ((LPCHARACTER)entity)->GetName());
}

#ifdef ENABLE_AFFECT_PACKET_RENEWAL
void CHARACTER::UpdateAffectFlag()
{
	TPacketAddAffectFlag pack;
	pack.bHeader = HEADER_GC_AFFECT_FLAG_ADD;
	pack.dwVID = m_vid;
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];
	PacketAround(&pack, sizeof(pack));
}
#endif

void CHARACTER::UpdatePacket()
{
	if (IsPC() && (!GetDesc() || !GetDesc()->GetCharacter()))
		return;

	TPacketGCCharacterUpdate pack;

	pack.header = HEADER_GC_CHARACTER_UPDATE;
	pack.dwVID = m_vid;

	pack.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
	pack.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
	pack.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
	pack.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
#ifdef ENABLE_ACCE_SYSTEM
	if (IsPC())
		pack.awPart[CHR_EQUIPPART_SASH] = GetQuestFlag("game_option.hide_sash") ? 0 : GetPart(PART_SASH);
	else
		pack.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
#endif
#ifdef ENABLE_AURA_SYSTEM
	pack.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	if (IsSupportShaman())
	{
		pack.awPart[CHR_EQUIPPART_ARMOR] = GetSupportShamanArmor();
		pack.awPart[CHR_EQUIPPART_WEAPON] = GetSupportShamanWeapon();
#ifdef ENABLE_ACCE_SYSTEM
		pack.awPart[CHR_EQUIPPART_SASH] = GetSupportShamanSash();
#endif
		pack.awPart[CHR_EQUIPPART_HEAD] = GetSupportShamanHair();
	}
#endif

	pack.bMovingSpeed = GetLimitPoint(POINT_MOV_SPEED);
	pack.bAttackSpeed = GetLimitPoint(POINT_ATT_SPEED);
	pack.bStateFlag = m_bAddChrState;
#ifndef ENABLE_AFFECT_PACKET_RENEWAL
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];
#endif
	pack.dwGuildID = 0;
	pack.sAlignment = m_iAlignment / 10;
	if (IsPC())
		pack.dwLevel = GetQuestFlag("game_option.level_view") ? 0 : GetLevel();
	else
		pack.dwLevel = GetLevel();
	pack.bPKMode = m_bPKMode;
	pack.dwMountVnum = GetMountVnum();

#ifdef ENABLE_QUIVER_SYSTEM
	if (IsPC())
		pack.dwArrow = GetWear(WEAR_ARROW) != NULL ? GetWear(WEAR_ARROW)->GetOriginalVnum() : 0;
#endif

	////////////////////////////////////////
	///////
	pack.dwGuildID = 0;
#ifdef ENABLE_GUILD_LEADER_SYSTEM
	pack.bMemberType = 0;
#endif

	CGuild* g = GetGuild();

	if (g != nullptr)
	{
		pack.dwGuildID = g->GetID();

#ifdef ENABLE_GUILD_LEADER_SYSTEM
		if (IsPC())
		{
			TGuildMember* gm = g->GetMember(GetPlayerID());
			if (gm)
			{
				if (gm->grade == GUILD_LEADER_GRADE)
					pack.bMemberType = 1;
				else if (gm->is_general)
					pack.bMemberType = 2;
			}
		}
#endif
	}
	///////
	////////////////////////////////////////

	pack.dwWeaponRareLv = 0;

	if (IsPC())
	{
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
		pack.combat_zone_points = GetCombatZonePoints();
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
		if ((GetWear(WEAR_WEAPON) && IS_SET(GetWear(WEAR_WEAPON)->GetFlag(), ITEM_FLAG_RARE_ABILITY)) && features::IsFeatureEnabled(features::WEAPON_RARITY))
			pack.dwWeaponRareLv = GetWear(WEAR_WEAPON)->GetRarePoints();
#endif
	}

#ifdef ENABLE_BUFFI_SYSTEM
	pack.bIsSupportShaman = IsSupportShaman();
#endif

#ifdef ENABLE_EFFECT_STONE_SYSTEM
	if (IsPC())
	{
		if (GetWear(WEAR_BODY))
			pack.bStoneEffect = GetWear(WEAR_BODY)->GetAddedEffectStone();
		if (GetWear(WEAR_WEAPON))
			pack.bWeaponStoneEffect = GetWear(WEAR_WEAPON)->GetAddedEffectStoneWeapon();
	}
#endif

	PacketAround(&pack, sizeof(pack));
}

LPCHARACTER CHARACTER::FindCharacterInView(const char* c_pszName, bool bFindPCOnly)
{
	ENTITY_MAP::iterator it = m_map_view.begin();

	for (; it != m_map_view.end(); ++it)
	{
		if (!it->first->IsType(ENTITY_CHARACTER))
			continue;

		LPCHARACTER tch = (LPCHARACTER)it->first;

		if (bFindPCOnly && tch->IsNPC())
			continue;

		if (!strcasecmp(tch->GetName(), c_pszName))
			return (tch);
	}

	return nullptr;
}

void CHARACTER::SetPosition(int pos)
{
	if (pos == POS_STANDING)
	{
		REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN);

		event_cancel(&m_pkDeadEvent);
		event_cancel(&m_pkStunEvent);
	}
	else if (pos == POS_DEAD)
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);

	if (!IsStone())
	{
		switch (pos)
		{
		case POS_FIGHTING:
			if (!IsState(m_stateBattle))
				MonsterLog("[BATTLE] Fighting status");

			GotoState(m_stateBattle);
			break;

		default:
			if (!IsState(m_stateIdle))
				MonsterLog("[IDLE] Off state");

			GotoState(m_stateIdle);
			break;
		}
	}

	m_pointsInstant.position = pos;
}

void CHARACTER::Save()
{
	if (!m_bSkipSave)
		CHARACTER_MANAGER::instance().DelayedSave(this);
}

void CHARACTER::CreatePlayerProto(TPlayerTable & tab)
{
	memset(&tab, 0, sizeof(TPlayerTable));

	if (GetNewName().empty())
	{
		strlcpy(tab.name, GetName(), sizeof(tab.name));
	}
	else
	{
		strlcpy(tab.name, GetNewName().c_str(), sizeof(tab.name));
	}

	strlcpy(tab.ip, GetDesc()->GetHostName(), sizeof(tab.ip));

	tab.id = m_dwPlayerID;
	tab.voice = GetPoint(POINT_VOICE);
	tab.level = GetLevel();
	tab.level_step = GetPoint(POINT_LEVEL_STEP);
	tab.exp = GetExp();
	tab.gold = GetGold();
#ifdef ENABLE_CHEQUE_SYSTEM
	tab.cheque = GetCheque();
#endif
#ifdef ENABLE_GEM_SYSTEM
	tab.gem = GetGem();
#endif
	tab.job = m_points.job;
	tab.part_base = m_pointsInstant.bBasePart;
	tab.skill_group = m_points.skill_group;
#ifdef ENABLE_EVENT_SYSTEM
	tab.year_round_point = GetYearRoundPoint();
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	tab.zodiac_point = GetZodiacPoint();
	tab.last_zodiac_point_reward = GetLastZodiacPointReward();
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	tab.lWorshipPoint = GetWorshipPoint();
#endif
#ifdef ENABLE_DUNGEON_BACK_SYSTEM
	tab.dungeonindex = m_points.dungeonindex;
#endif

	DWORD dwPlayedTime = (get_dword_time() - m_dwPlayStartTime);

	if (dwPlayedTime > 60000)
	{
		if (GetSectree() && !GetSectree()->IsAttr(GetX(), GetY(), ATTR_BANPK))
		{
			if (GetRealAlignment() < 0)
			{
				if (IsEquipUniqueItem(UNIQUE_ITEM_FASTER_ALIGNMENT_UP_BY_TIME))
#ifdef ENABLE_ALIGNMENT_SYSTEM
					UpdateAlignment(120 * (dwPlayedTime / 60000), true);
				else
					UpdateAlignment(60 * (dwPlayedTime / 60000), true);
			}
			else
				UpdateAlignment(5 * (dwPlayedTime / 60000), true);
#else
					UpdateAlignment(120 * (dwPlayedTime / 60000));
				else
					UpdateAlignment(60 * (dwPlayedTime / 60000));
			}
			else
				UpdateAlignment(5 * (dwPlayedTime / 60000));
#endif
		}

		SetRealPoint(POINT_PLAYTIME, GetRealPoint(POINT_PLAYTIME) + dwPlayedTime / 60000);
		ResetPlayTime(dwPlayedTime % 60000);
	}

	tab.playtime = GetRealPoint(POINT_PLAYTIME);
	tab.lAlignment = m_iRealAlignment;

	if (m_posWarp.x != 0 || m_posWarp.y != 0)
	{
		tab.x = m_posWarp.x;
		tab.y = m_posWarp.y;
		tab.z = 0;
		tab.lMapIndex = m_lWarpMapIndex;
	}
	else
	{
		tab.x = GetX();
		tab.y = GetY();
		tab.z = GetZ();
		tab.lMapIndex = GetMapIndex();
	}

	if (m_lExitMapIndex == 0)
	{
		tab.lExitMapIndex = tab.lMapIndex;
		tab.lExitX = tab.x;
		tab.lExitY = tab.y;
	}
	else
	{
		tab.lExitMapIndex = m_lExitMapIndex;
		tab.lExitX = m_posExit.x;
		tab.lExitY = m_posExit.y;
	}

	sys_log(0, "SAVE: %s %dx%d", GetName(), tab.x, tab.y);

	tab.st = GetRealPoint(POINT_ST);
	tab.ht = GetRealPoint(POINT_HT);
	tab.dx = GetRealPoint(POINT_DX);
	tab.iq = GetRealPoint(POINT_IQ);

	tab.stat_point = GetPoint(POINT_STAT);
	tab.skill_point = GetPoint(POINT_SKILL);
	tab.sub_skill_point = GetPoint(POINT_SUB_SKILL);
	tab.horse_skill_point = GetPoint(POINT_HORSE_SKILL);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	tab.extend_inventory = m_points.extend_inventory;
#endif

	tab.stat_reset_count = GetPoint(POINT_STAT_RESET_COUNT);

	tab.hp = GetHP();
	tab.sp = GetSP();

	tab.stamina = GetStamina();

	tab.sRandomHP = m_points.iRandomHP;
	tab.sRandomSP = m_points.iRandomSP;
	
	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
		tab.quickslot[i] = m_quickslot[i];
	thecore_memcpy(tab.parts, m_pointsInstant.parts, sizeof(tab.parts));

	// REMOVE_REAL_SKILL_LEVLES
	thecore_memcpy(tab.skills, m_pSkillLevels, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES

	tab.horse = GetHorseData();

#ifdef ENABLE_FISH_JIGSAW_EVENT
	thecore_memcpy(tab.fishSlots, m_fishSlots, sizeof(TPlayerFishEventSlot) * FISH_EVENT_SLOTS_NUM);
	tab.fishEventUseCount = GetFishEventUseCount();
#endif

#ifdef ENABLE_GEM_SYSTEM
	thecore_memcpy(tab.gemItems, m_gemItems, sizeof(TPlayerGemItems) * GEM_SLOTS_MAX_NUM);
	tab.gem_next_refresh = GetGemNextRefresh();
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	thecore_memcpy(tab.gemBossItems, m_gemBossItems, sizeof(TPlayerGemItems) * GEM_SLOTS_MAX_NUM);
	#endif
#endif
}

void CHARACTER::SaveReal()
{
	if (m_bSkipSave)
		return;

	if (!GetDesc())
	{
		sys_err("Character::Save : no descriptor when saving (name: %s)", GetName());
		return;
	}

	TPlayerTable table;
	CreatePlayerProto(table);

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_SAVE, GetDesc()->GetHandle(), &table, sizeof(TPlayerTable));

	quest::PC* pkQuestPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());

	if (!pkQuestPC)
		sys_err("CHARACTER::Save : nullptr quest::PC pointer! (name %s)", GetName());
	else
	{
		pkQuestPC->Save();
	}

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
	if (pMarriage)
		pMarriage->Save();
}

void CHARACTER::FlushDelayedSaveItem()
{
	// 저장 안된 소지품을 전부 저장시킨다.
	LPITEM item;

	for (int i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
		if ((item = GetInventoryItem(i)))
			ITEM_MANAGER::instance().FlushDelayedSave(item);
}

void CHARACTER::Disconnect(const char* c_pszReason)
{
	assert(GetDesc() != nullptr);

	sys_log(0, "DISCONNECT: %s (%s)", GetName(), c_pszReason ? c_pszReason : "unset");

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(nullptr);
	}

	if (GetArena() != nullptr)
	{
		GetArena()->OnDisconnect(GetPlayerID());
	}

	if (GetParty() != nullptr)
	{
		GetParty()->UpdateOfflineState(GetPlayerID());
	}

	marriage::CManager::instance().Logout(this);

	// P2P Logout
	TPacketGGLogout p;
	p.bHeader = HEADER_GG_LOGOUT;
	strlcpy(p.szName, GetName(), sizeof(p.szName));
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogout));
	char buf[51];
	snprintf(buf, sizeof(buf), "%s %d %d %ld %lld",
		inet_ntoa(GetDesc()->GetAddr().sin_addr), GetGold(), g_bChannel, GetMapIndex(), GetAlignment());

	LogManager::instance().CharLog(this, 0, "LOGOUT", buf);

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if (MeleyLair::CMgr::instance().IsMeleyMap(GetMapIndex()))
		MeleyLair::CMgr::instance().Leave(GetGuild(), this, false);
#endif

	if (m_pWarMap)
		SetWarMap(nullptr);

	if (m_pWeddingMap)
		SetWeddingMap(nullptr);

	if (GetGuild())
		GetGuild()->LogoutMember(this);

	quest::CQuestManager::instance().LogoutPC(this);

	if (GetParty())
		GetParty()->Unlink(this);

	// 죽었을 때 접속끊으면 경험치 줄게 하기
	if (IsStun() || IsDead())
	{
		DeathPenalty(0);
		PointChange(POINT_HP, 50 - GetHP());
	}

	if (GetMapIndex() == 208)
		LogoutSpecialObserver();

	if (!CHARACTER_MANAGER::instance().FlushDelayedSave(this))
	{
		SaveReal();
	}

	FlushDelayedSaveItem();

	SaveAffect();
	m_bIsLoadedAffect = false;

	m_bSkipSave = true; // 이 이후에는 더이상 저장하면 안된다.

	quest::CQuestManager::instance().DisconnectPC(this);

	CloseSafebox();

	CloseMall();

	CPVPManager::instance().Disconnect(this);

	CTargetManager::instance().Logout(GetPlayerID());

	MessengerManager::instance().Logout(GetName());

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (GetMountVnum())
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
	}
#endif

	if (GetDesc())
	{
		packet_point_change pack;
		pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
		pack.dwVID = m_vid;
		pack.type = POINT_PLAYTIME;
		pack.value = GetRealPoint(POINT_PLAYTIME) + (get_dword_time() - m_dwPlayStartTime) / 60000;
		pack.amount = 0;
		GetDesc()->Packet(&pack, sizeof(struct packet_point_change));
		GetDesc()->BindCharacter(nullptr);
		//		BindDesc(nullptr);
	}

	M2_DESTROY_CHARACTER(this);
}

bool CHARACTER::Show(long lMapIndex, long x, long y, long z, bool bShowSpawnMotion, bool bAggressive)
{
	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
#ifdef ENABLE_FULL_LOG
		sys_log(0, "cannot find sectree by %dx%d mapindex %d", x, y, lMapIndex);
#endif
		return false;
	}

	SetMapIndex(lMapIndex);

	bool bChangeTree = false;

	if (!GetSectree() || GetSectree() != sectree)
		bChangeTree = true;

	if (bChangeTree)
	{
		if (GetSectree())
			GetSectree()->RemoveEntity(this);

		ViewCleanup();
	}

	if (!IsNPC())
	{
		sys_log(0, "SHOW: %s %dx%dx%d", GetName(), x, y, z);
		if (GetStamina() < GetMaxStamina())
			StartAffectEvent();
	}
	else if (m_pkMobData)
	{
		m_pkMobInst->m_posLastAttacked.x = x;
		m_pkMobInst->m_posLastAttacked.y = y;
		m_pkMobInst->m_posLastAttacked.z = z;
	}

	if (bShowSpawnMotion)
	{
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
		m_afAffectFlag.Set(AFF_SPAWN);
	}

	if (bAggressive)//-__-'
		SetAggressive();

	SetXYZ(x, y, z);

	m_posDest.x = x;
	m_posDest.y = y;
	m_posDest.z = z;

	m_posStart.x = x;
	m_posStart.y = y;
	m_posStart.z = z;

	if (bChangeTree)
	{
		EncodeInsertPacket(this);
		sectree->InsertEntity(this);

		UpdateSectree();
	}
	else
	{
		ViewReencode();
		sys_log(0, "      in same sectree");
	}

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);

	SetValidComboInterval(0);
	
#ifdef ENABLE_ABUSE_SYSTEM
	if (IsPC())
		GetAbuseController()->ResetMovementSpeedhackChecker();
#endif
	return true;
}

// BGM_INFO
struct BGMInfo
{
	std::string	name;
	float		vol;
};

typedef std::map<unsigned, BGMInfo> BGMInfoMap;

static BGMInfoMap 	gs_bgmInfoMap;
static bool		gs_bgmVolEnable = false;

void CHARACTER_SetBGMVolumeEnable()
{
	gs_bgmVolEnable = true;
	sys_log(0, "bgm_info.set_bgm_volume_enable");
}

void CHARACTER_AddBGMInfo(unsigned mapIndex, const char* name, float vol)
{
	BGMInfo newInfo;
	newInfo.name = name;
	newInfo.vol = vol;

	gs_bgmInfoMap[mapIndex] = newInfo;

	//sys_log(0, "bgm_info.add_info(%d, '%s', %f)", mapIndex, name, vol);
}

const BGMInfo& CHARACTER_GetBGMInfo(unsigned mapIndex)
{
	BGMInfoMap::iterator f = gs_bgmInfoMap.find(mapIndex);
	if (gs_bgmInfoMap.end() == f)
	{
		static BGMInfo s_empty = { "", 0.0f };
		return s_empty;
	}
	return f->second;
}

bool CHARACTER_IsBGMVolumeEnable()
{
	return gs_bgmVolEnable;
}
// END_OF_BGM_INFO

void CHARACTER::MainCharacterPacket()
{
	const unsigned mapIndex = GetMapIndex() < 10000 ? GetMapIndex() : GetMapIndex() / 10000;
	const BGMInfo& bgmInfo = CHARACTER_GetBGMInfo(mapIndex);
#ifdef ENABLE_MUSIC_EVENT
	int MuzikDeger = quest::CQuestManager::instance().GetEventFlag("xmas_muzik");
#endif

	// SUPPORT_BGM
	if (!bgmInfo.name.empty())
	{
		if (CHARACTER_IsBGMVolumeEnable())
		{
			sys_log(1, "bgm_info.play_bgm_vol(%d, name='%s', vol=%f)", mapIndex, bgmInfo.name.c_str(), bgmInfo.vol);
			TPacketGCMainCharacter4_BGM_VOL mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER4_BGM_VOL;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));

			mainChrPacket.fBGMVol = bgmInfo.vol;
#ifdef ENABLE_MUSIC_EVENT
			if (MuzikDeger == 0) {
				strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 1) {
				strlcpy(mainChrPacket.szBGMName, "1.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 2) {
				strlcpy(mainChrPacket.szBGMName, "2.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 3) {
				strlcpy(mainChrPacket.szBGMName, "3.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 4) {
				strlcpy(mainChrPacket.szBGMName, "4.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 4) {
				strlcpy(mainChrPacket.szBGMName, "5.mp3", sizeof(mainChrPacket.szBGMName));
			}
#else
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
#endif
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter4_BGM_VOL));
		}
		else
		{
			sys_log(1, "bgm_info.play(%d, '%s')", mapIndex, bgmInfo.name.c_str());
			TPacketGCMainCharacter3_BGM mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER3_BGM;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));
#ifdef ENABLE_MUSIC_EVENT
			if (MuzikDeger == 0) {
				strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 1) {
				strlcpy(mainChrPacket.szBGMName, "1.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 2) {
				strlcpy(mainChrPacket.szBGMName, "2.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 3) {
				strlcpy(mainChrPacket.szBGMName, "3.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 4) {
				strlcpy(mainChrPacket.szBGMName, "4.mp3", sizeof(mainChrPacket.szBGMName));
			}
			else if (MuzikDeger == 4) {
				strlcpy(mainChrPacket.szBGMName, "5.mp3", sizeof(mainChrPacket.szBGMName));
			}
#else
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
#endif
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter3_BGM));
		}
	}
	// END_OF_SUPPORT_BGM
	else
	{
		sys_log(0, "bgm_info.play(%d, DEFAULT_BGM_NAME)", mapIndex);

		TPacketGCMainCharacter pack;
		pack.header = HEADER_GC_MAIN_CHARACTER;
		pack.dwVID = m_vid;
		pack.wRaceNum = GetRaceNum();
		pack.lx = GetX();
		pack.ly = GetY();
		pack.lz = GetZ();
		pack.empire = GetDesc()->GetEmpire();
		pack.skill_group = GetSkillGroup();
		strlcpy(pack.szName, GetName(), sizeof(pack.szName));
		GetDesc()->Packet(&pack, sizeof(TPacketGCMainCharacter));
	}
}

void CHARACTER::PointsPacket()
{
	if (!GetDesc())
		return;

	TPacketGCPoints pack;

	pack.header = HEADER_GC_CHARACTER_POINTS;
	pack.points[POINT_LEVEL] = GetLevel();
	pack.points[POINT_EXP] = GetExp();
	pack.points[POINT_NEXT_EXP] = GetNextExp();
	pack.points[POINT_HP] = GetHP();
	pack.points[POINT_MAX_HP] = GetMaxHP();
	pack.points[POINT_SP] = GetSP();
	pack.points[POINT_MAX_SP] = GetMaxSP();
	pack.points[POINT_GOLD] = GetGold();
	pack.points[POINT_STAMINA] = GetStamina();
	pack.points[POINT_MAX_STAMINA] = GetMaxStamina();

	for (int i = POINT_ST; i < POINT_MAX_NUM; ++i)
		pack.points[i] = GetPoint(i);
#ifdef ENABLE_CHEQUE_SYSTEM
	pack.points[POINT_CHEQUE] = GetCheque();
#endif
#ifdef ENABLE_GEM_SYSTEM
	pack.points[POINT_GEM] = GetGem();
#endif
#ifdef ENABLE_EVENT_SYSTEM
	pack.points[POINT_YEAR_ROUND] = GetYearRoundPoint();
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	pack.points[POINT_ZODIAC] = GetZodiacPoint();
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	pack.points[POINT_WORSHIP] = GetWorshipPoint();
#endif

	GetDesc()->Packet(&pack, sizeof(TPacketGCPoints));
}

bool CHARACTER::ChangeSex()
{
	int src_race = GetRaceNum();

	switch (src_race)
	{
	case MAIN_RACE_WARRIOR_M:
		m_points.job = MAIN_RACE_WARRIOR_W;
		break;

	case MAIN_RACE_WARRIOR_W:
		m_points.job = MAIN_RACE_WARRIOR_M;
		break;

	case MAIN_RACE_ASSASSIN_M:
		m_points.job = MAIN_RACE_ASSASSIN_W;
		break;

	case MAIN_RACE_ASSASSIN_W:
		m_points.job = MAIN_RACE_ASSASSIN_M;
		break;

	case MAIN_RACE_SURA_M:
		m_points.job = MAIN_RACE_SURA_W;
		break;

	case MAIN_RACE_SURA_W:
		m_points.job = MAIN_RACE_SURA_M;
		break;

	case MAIN_RACE_SHAMAN_M:
		m_points.job = MAIN_RACE_SHAMAN_W;
		break;

	case MAIN_RACE_SHAMAN_W:
		m_points.job = MAIN_RACE_SHAMAN_M;
		break;
#ifdef ENABLE_WOLFMAN_CHARACTER
	case MAIN_RACE_WOLFMAN_M:
		m_points.job = MAIN_RACE_WOLFMAN_M;
		break;
#endif
	default:
		sys_err("CHANGE_SEX: %s unknown race %d", GetName(), src_race);
		return false;
	}

	sys_log(0, "CHANGE_SEX: %s (%d -> %d)", GetName(), src_race, m_points.job);
	return true;
}

WORD CHARACTER::GetRaceNum() const
{
	if (m_dwPolymorphRace)
		return m_dwPolymorphRace;

	if (m_pkMobData)
		return m_pkMobData->m_table.dwVnum;

	return m_points.job;
}

void CHARACTER::SetRace(BYTE race)
{
	if (race >= MAIN_RACE_MAX_NUM)
	{
		sys_err("CHARACTER::SetRace(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
		return;
	}

	m_points.job = race;
}

BYTE CHARACTER::GetJob() const
{
	unsigned race = m_points.job;
	unsigned job;

	if (RaceToJob(race, &job))
		return job;

	sys_err("CHARACTER::GetJob(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
	return JOB_WARRIOR;
}

void CHARACTER::SetLevel(BYTE level)
{
	m_points.level = level;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (IsPC() && CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif

	if (IsPC())
	{
		if (level < PK_PROTECT_LEVEL)
			SetPKMode(PK_MODE_PROTECT);
		else if (GetGMLevel() != GM_PLAYER)
			SetPKMode(PK_MODE_PROTECT);
		else if (m_bPKMode == PK_MODE_PROTECT)
			SetPKMode(PK_MODE_PEACE);
	}
}

void CHARACTER::SetEmpire(BYTE bEmpire)
{
	m_bEmpire = bEmpire;
}

void CHARACTER::SetPlayerProto(const TPlayerTable * t)
{
	if (!GetDesc() || !*GetDesc()->GetHostName())
		sys_err("cannot get desc or hostname");
	else
		SetGMLevel();

	m_bCharType = CHAR_TYPE_PC;

	m_dwPlayerID = t->id;

	m_iAlignment = t->lAlignment;
	m_iRealAlignment = t->lAlignment;

	m_points.voice = t->voice;

	m_points.skill_group = t->skill_group;

	m_pointsInstant.bBasePart = t->part_base;
	SetPart(PART_HAIR, t->parts[PART_HAIR]);
#ifdef ENABLE_ACCE_SYSTEM
	SetPart(PART_SASH, t->parts[PART_SASH]);
#endif
#ifdef ENABLE_AURA_SYSTEM
	SetPart(PART_AURA, t->parts[PART_AURA]);
#endif

	m_points.iRandomHP = t->sRandomHP;
	m_points.iRandomSP = t->sRandomSP;
	
#ifdef ENABLE_DUNGEON_BACK_SYSTEM
	m_points.dungeonindex = t->dungeonindex;
#endif


	// REMOVE_REAL_SKILL_LEVLES
	if (m_pSkillLevels)
		M2_DELETE_ARRAY(m_pSkillLevels);

	m_pSkillLevels = M2_NEW TPlayerSkill[SKILL_MAX_NUM];
	thecore_memcpy(m_pSkillLevels, t->skills, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES

#ifdef ENABLE_FISH_JIGSAW_EVENT
	if (m_fishSlots)
		M2_DELETE_ARRAY(m_fishSlots);

	m_fishSlots = M2_NEW TPlayerFishEventSlot[FISH_EVENT_SLOTS_NUM];
	thecore_memcpy(m_fishSlots, t->fishSlots, sizeof(TPlayerFishEventSlot) * FISH_EVENT_SLOTS_NUM);

	m_dwFishUseCount = t->fishEventUseCount;
#endif

#ifdef ENABLE_GEM_SYSTEM
	if (m_gemItems)
		M2_DELETE_ARRAY(m_gemItems);

	m_gemItems = M2_NEW TPlayerGemItems[GEM_SLOTS_MAX_NUM];
	thecore_memcpy(m_gemItems, t->gemItems, sizeof(TPlayerGemItems) * GEM_SLOTS_MAX_NUM);
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (m_gemBossItems)
		M2_DELETE_ARRAY(m_gemBossItems);
	
	m_gemBossItems = M2_NEW TPlayerGemItems[GEM_SLOTS_MAX_NUM];
	thecore_memcpy(m_gemBossItems, t->gemBossItems, sizeof(TPlayerGemItems) * GEM_SLOTS_MAX_NUM);
	#endif
#endif

	if (t->lMapIndex >= 10000)
	{
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		bool zodiac = (t->lMapIndex >= 3580000 && t->lMapIndex < 3590000) ? false : true;
		if (zodiac)
		{
			m_posWarp.x = t->lExitX;
			m_posWarp.y = t->lExitY;
			m_lWarpMapIndex = t->lExitMapIndex;
		}
#else
		m_posWarp.x = t->lExitX;
		m_posWarp.y = t->lExitY;
		m_lWarpMapIndex = t->lExitMapIndex;
#endif
	}

	SetRealPoint(POINT_PLAYTIME, t->playtime);
	m_dwLoginPlayTime = t->playtime;
	SetRealPoint(POINT_ST, t->st);
	SetRealPoint(POINT_HT, t->ht);
	SetRealPoint(POINT_DX, t->dx);
	SetRealPoint(POINT_IQ, t->iq);

	SetPoint(POINT_ST, t->st);
	SetPoint(POINT_HT, t->ht);
	SetPoint(POINT_DX, t->dx);
	SetPoint(POINT_IQ, t->iq);

	SetPoint(POINT_STAT, t->stat_point);
	SetPoint(POINT_SKILL, t->skill_point);
	SetPoint(POINT_SUB_SKILL, t->sub_skill_point);
	SetPoint(POINT_HORSE_SKILL, t->horse_skill_point);

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	SetExtendedInventory(t->extend_inventory);
#endif

	SetPoint(POINT_STAT_RESET_COUNT, t->stat_reset_count);

	SetPoint(POINT_LEVEL_STEP, t->level_step);
	SetRealPoint(POINT_LEVEL_STEP, t->level_step);

	SetRace(t->job);

	SetLevel(t->level);
	SetExp(t->exp);
	SetGold(t->gold);
#ifdef ENABLE_CHEQUE_SYSTEM
	SetCheque(t->cheque);
#endif
#ifdef ENABLE_GEM_SYSTEM
	SetGem(t->gem);
#endif
#ifdef ENABLE_EVENT_SYSTEM
	SetYearRoundPoint(t->year_round_point);
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	SetZodiacPoint(t->zodiac_point);
	SetLastZodiacPointReward(t->last_zodiac_point_reward);
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	SetWorshipPoint(t->lWorshipPoint);
#endif

	SetMapIndex(t->lMapIndex);
	SetXYZ(t->x, t->y, t->z);

	ComputePoints();

	SetHP(GetMaxHP()); // -32k hp bugu fix
	SetSP(GetMaxSP()); // -32k sp bugu fix
	SetStamina(t->stamina);
#ifdef ENABLE_YOUTUBER_SYSTEM
	Yayinci_Liste();
#endif

	//GM일때 보호모드
	if (!test_server)
	{
		if (GetGMLevel() > GM_LOW_WIZARD)
		{
			m_afAffectFlag.Set(AFF_YMIR);
			m_bPKMode = PK_MODE_PROTECT;
		}
	}

	if (GetLevel() < PK_PROTECT_LEVEL)
		m_bPKMode = PK_MODE_PROTECT;

	SetHorseData(t->horse);

	if (GetHorseLevel() > 0)
		UpdateHorseDataByLogoff(t->logoff_interval);

	thecore_memcpy(m_aiPremiumTimes, t->aiPremiumTimes, sizeof(t->aiPremiumTimes));

	m_dwLogOffInterval = t->logoff_interval;
#ifdef ENABLE_GEM_SYSTEM
	m_dwGemNextRefresh = t->gem_next_refresh;
#endif

	sys_log(0, "PLAYER_LOAD: %s PREMIUM %d %d, LOGGOFF_INTERVAL %u PTR: %p", t->name, m_aiPremiumTimes[0], m_aiPremiumTimes[1], t->logoff_interval, this);

	if (GetGMLevel() != GM_PLAYER)
	{
		LogManager::instance().CharLog(this, GetGMLevel(), "GM_LOGIN", "");
		sys_log(0, "GM_LOGIN(gmlevel=%d, name=%s(%d), pos=(%d, %d)", GetGMLevel(), GetName(), GetPlayerID(), GetX(), GetY());
	}

#ifdef ENABLE_PET_SYSTEM
	// NOTE: 일단 캐릭터가 PC인 경우에만 PetSystem을 갖도록 함. 유럽 머신당 메모리 사용률때문에 NPC까지 하긴 좀..
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;
	}

	m_petSystem = M2_NEW CPetSystem(this);
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	if (m_SupportShaman)
	{
		m_SupportShaman->Destroy();
		delete m_SupportShaman;
	}

	m_SupportShaman = M2_NEW CSupportShaman(this);
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;
	}

	m_newpetSystem = M2_NEW CNewPetSystem(this);
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (m_mountSystem)
	{
		m_mountSystem->Destroy();
		delete m_mountSystem;
	}
	m_mountSystem = M2_NEW CMountSystem(this);
#endif
}

EVENTFUNC(kill_ore_load_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("kill_ore_load_even> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == nullptr) { // <Factor>
		return 0;
	}

	ch->m_pkMiningEvent = nullptr;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::SetProto(const CMob * pkMob)
{
	if (m_pkMobInst)
		M2_DELETE(m_pkMobInst);

	m_pkMobData = pkMob;
	m_pkMobInst = M2_NEW CMobInstance;

	m_bPKMode = PK_MODE_FREE;

	const TMobTable* t = &m_pkMobData->m_table;

	m_bCharType = t->bType;

	SetLevel(t->bLevel);
	SetEmpire(t->bEmpire);

	SetExp(t->dwExp);
	SetRealPoint(POINT_ST, t->bStr);
	SetRealPoint(POINT_DX, t->bDex);
	SetRealPoint(POINT_HT, t->bCon);
	SetRealPoint(POINT_IQ, t->bInt);

	ComputePoints();

	SetHP(GetMaxHP());
	SetSP(GetMaxSP());

	////////////////////
	m_pointsInstant.dwAIFlag = t->dwAIFlag;
	SetImmuneFlag(t->dwImmuneFlag);

	AssignTriggers(t);

	ApplyMobAttribute(t);

	if (IsStone())
	{
		DetermineDropMetinStone();
	}

	if (IsWarp() || IsGoto()
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		|| GetRaceNum() == 3949
#endif
		)
	{
		StartWarpNPCEvent();
	}

	try {
		CHARACTER_MANAGER::instance().RegisterRaceNumMap(this);
	}
	catch (...) {
		sys_err("Catched exception on RegisterRaceNumMap %s %d", this->GetName(), this->GetRaceNum());
	}

	// XXX X-mas santa hardcoding
	if (GetRaceNum() == xmas::MOB_SANTA_VNUM)
	{
		SetPoint(POINT_ATT_GRADE_BONUS, 10);
		SetPoint(POINT_DEF_GRADE_BONUS, 6);

		//산타용
		//m_dwPlayStartTime = get_dword_time() + 10 * 60 * 1000;
		//신선자 노해
		m_dwPlayStartTime = get_dword_time() + 30 * 1000;
		if (test_server)
			m_dwPlayStartTime = get_dword_time() + 30 * 1000;
	}

	// XXX CTF GuildWar hardcoding
	if (warmap::IsWarFlag(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
	}

	if (warmap::IsWarFlagBase(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
	}

	if (m_bCharType == CHAR_TYPE_HORSE ||
		GetRaceNum() == 20101 ||
		GetRaceNum() == 20102 ||
		GetRaceNum() == 20103 ||
		GetRaceNum() == 20104 ||
		GetRaceNum() == 20105 ||
		GetRaceNum() == 20106 ||
		GetRaceNum() == 20107 ||
		GetRaceNum() == 20108 ||
		GetRaceNum() == 20109
		)
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
	}

	// MINING
	if (mining::IsVeinOfOre(GetRaceNum()))
	{
		char_event_info* info = AllocEventInfo<char_event_info>();

		info->ch = this;

		m_pkMiningEvent = event_create(kill_ore_load_event, info, PASSES_PER_SEC(number(7 * 60, 15 * 60)));
	}
}

const TMobTable& CHARACTER::GetMobTable() const
{
	return m_pkMobData->m_table;
}

bool CHARACTER::IsRaceFlag(DWORD dwBit) const
{
	return m_pkMobData ? IS_SET(m_pkMobData->m_table.dwRaceFlag, dwBit) : 0;
}

DWORD CHARACTER::GetMobDamageMin() const
{
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
	if (IsDefanceWaweMastAttackMob(GetRaceNum()))
		return m_pkMobData->m_table.dwDamageRange[0] + 500;
	else
		return m_pkMobData->m_table.dwDamageRange[0];
#else
	return m_pkMobData->m_table.dwDamageRange[0];
#endif
}

DWORD CHARACTER::GetMobDamageMax() const
{
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
	if (IsDefanceWaweMastAttackMob(GetRaceNum()))
		return m_pkMobData->m_table.dwDamageRange[1] + 500;
	else
		return m_pkMobData->m_table.dwDamageRange[1];
#else
	return m_pkMobData->m_table.dwDamageRange[1];
#endif
}

float CHARACTER::GetMobDamageMultiply() const
{
	float fDamMultiply = GetMobTable().fDamMultiply;

	if (IsBerserk())
		fDamMultiply = fDamMultiply * 2.0f; // BALANCE: 광폭화 시 두배

	return fDamMultiply;
}

DWORD CHARACTER::GetMobDropItemVnum() const
{
	return m_pkMobData->m_table.dwDropItemVnum;
}

bool CHARACTER::IsSummonMonster() const
{
	return GetSummonVnum() != 0;
}

DWORD CHARACTER::GetSummonVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwSummonVnum : 0;
}

DWORD CHARACTER::GetPolymorphItemVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwPolymorphItemVnum : 0;
}

DWORD CHARACTER::GetMonsterDrainSPPoint() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwDrainSP : 0;
}

BYTE CHARACTER::GetMobRank() const
{
	if (!m_pkMobData)
		return MOB_RANK_KNIGHT;	// PC일 경우 KNIGHT급

	return m_pkMobData->m_table.bRank;
}

BYTE CHARACTER::GetMobSize() const
{
	if (!m_pkMobData)
		return MOBSIZE_MEDIUM;

	return m_pkMobData->m_table.bSize;
}

WORD CHARACTER::GetMobAttackRange() const
{
	switch (GetMobBattleType())
	{
	case BATTLE_TYPE_RANGE:
	case BATTLE_TYPE_MAGIC:
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		if (GetRaceNum() == 3960 || GetRaceNum() == 3961 || GetRaceNum() == 3962)
			return m_pkMobData->m_table.wAttackRange + GetPoint(POINT_BOW_DISTANCE) + 300;
		else
			return m_pkMobData->m_table.wAttackRange + GetPoint(POINT_BOW_DISTANCE);
#else
		return m_pkMobData->m_table.wAttackRange + GetPoint(POINT_BOW_DISTANCE);
#endif
	default:
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		if ((GetRaceNum() <= 3955 && GetRaceNum() >= 3950 && GetRaceNum() != 3953))
			return m_pkMobData->m_table.wAttackRange + 300;
		else
			return m_pkMobData->m_table.wAttackRange;
#else
		return m_pkMobData->m_table.wAttackRange;
#endif
	}
}

BYTE CHARACTER::GetMobBattleType() const
{
	if (!m_pkMobData)
		return BATTLE_TYPE_MELEE;

	return (m_pkMobData->m_table.bBattleType);
}

void CHARACTER::ComputeBattlePoints()
{
	if (IsPolymorphed())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob* pMob = CMobManager::instance().Get(dwMobVnum);
		int iAtt = 0;
		int iDef = 0;

		if (pMob)
		{
			iAtt = GetLevel() * 2 + GetPolymorphPoint(POINT_ST) * 2;
			iDef = GetLevel() + GetPolymorphPoint(POINT_HT) + pMob->m_table.wDef;
		}

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
	else if (IsPC())
	{
		SetPoint(POINT_ATT_GRADE, 0);
		SetPoint(POINT_DEF_GRADE, 0);
		SetPoint(POINT_CLIENT_DEF_GRADE, 0);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));

		int iAtk = GetLevel() * 2;
		int iStatAtk = 0;
		switch (GetJob())
		{
		case JOB_WARRIOR:
		case JOB_SURA:
			iStatAtk = (2 * GetPoint(POINT_ST));
			break;

		case JOB_ASSASSIN:
			iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_DX)) / 3;
			break;

		case JOB_SHAMAN:
			iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_IQ)) / 3;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case JOB_WOLFMAN:
			// TODO: 수인족 공격력 공식 기획자에게 요청
			iStatAtk = (2 * GetPoint(POINT_ST));
			break;
#endif
		default:
			sys_err("invalid job %d", GetJob());
			iStatAtk = (2 * GetPoint(POINT_ST));
			break;
		}

		if (GetMountVnum() && iStatAtk < 2 * GetPoint(POINT_ST))
			iStatAtk = (2 * GetPoint(POINT_ST));

		iAtk += iStatAtk;
		if (GetMountVnum())
		{
			if (GetJob() == JOB_SURA && GetSkillGroup() == 1)
			{
				iAtk += (iAtk * GetHorseLevel()) / 60;
			}
			else
			{
				iAtk += (iAtk * GetHorseLevel()) / 30;
			}
		}

		iAtk += GetPoint(POINT_ATT_GRADE_BONUS);
		PointChange(POINT_ATT_GRADE, iAtk);

		// DEF = LEV + CON + ARMOR
		int iShowDef = GetLevel() + GetPoint(POINT_HT); // For Ymir(천마)
		int iDef = GetLevel() + (int)(GetPoint(POINT_HT) / 1.25); // For Other
		int iArmor = 0;

		LPITEM pkItem;
		for (int i = 0; i < WEAR_MAX_NUM; ++i)
			if ((pkItem = GetWear(i)) && pkItem->GetType() == ITEM_ARMOR)
			{
				if (pkItem->GetSubType() == ARMOR_BODY || pkItem->GetSubType() == ARMOR_HEAD || pkItem->GetSubType() == ARMOR_FOOTS || pkItem->GetSubType() == ARMOR_SHIELD)
				{
					iArmor += pkItem->GetValue(1);
					iArmor += (2 * pkItem->GetValue(5));
				}
			}

		if (true == IsHorseRiding())
		{
			if (iArmor < GetHorseArmor())
				iArmor = GetHorseArmor();

			const char* pHorseName = CHorseNameManager::instance().GetHorseName(GetPlayerID());
			if (pHorseName != nullptr && strlen(pHorseName))
			{
				iArmor += 20;
			}
		}

		iArmor += GetPoint(POINT_DEF_GRADE_BONUS);
		iArmor += GetPoint(POINT_PARTY_DEFENDER_BONUS);
		// INTERNATIONAL_VERSION
		PointChange(POINT_DEF_GRADE, iDef + iArmor);
		PointChange(POINT_CLIENT_DEF_GRADE, (iShowDef + iArmor) - GetPoint(POINT_DEF_GRADE));
		// END_OF_INTERNATIONAL_VERSION

		PointChange(POINT_MAGIC_ATT_GRADE, GetLevel() * 2 + GetPoint(POINT_IQ) * 2 + GetPoint(POINT_MAGIC_ATT_GRADE_BONUS));
		PointChange(POINT_MAGIC_DEF_GRADE, GetLevel() + (GetPoint(POINT_IQ) * 3 + GetPoint(POINT_HT)) / 3 + iArmor / 2 + GetPoint(POINT_MAGIC_DEF_GRADE_BONUS));
	}
	else
	{
		int iAtt = GetLevel() * 2 + GetPoint(POINT_ST) * 2;
		int iDef = GetLevel() + GetPoint(POINT_HT) + GetMobTable().wDef;

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
}

void CHARACTER::ComputePoints()
{
	long lStat = GetPoint(POINT_STAT);
	long lStatResetCount = GetPoint(POINT_STAT_RESET_COUNT);
	long lSkillActive = GetPoint(POINT_SKILL);
	long lSkillSub = GetPoint(POINT_SUB_SKILL);
	long lSkillHorse = GetPoint(POINT_HORSE_SKILL);
	long lLevelStep = GetPoint(POINT_LEVEL_STEP);

	long lAttackerBonus = GetPoint(POINT_PARTY_ATTACKER_BONUS);
	long lTankerBonus = GetPoint(POINT_PARTY_TANKER_BONUS);
	long lBufferBonus = GetPoint(POINT_PARTY_BUFFER_BONUS);
	long lSkillMasterBonus = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
	long lHasteBonus = GetPoint(POINT_PARTY_HASTE_BONUS);
	long lDefenderBonus = GetPoint(POINT_PARTY_DEFENDER_BONUS);

	long lHPRecovery = GetPoint(POINT_HP_RECOVERY);
	long lSPRecovery = GetPoint(POINT_SP_RECOVERY);

	memset(m_pointsInstant.points, 0, sizeof(m_pointsInstant.points));
	BuffOnAttr_ClearAll();
	m_SkillDamageBonus.clear();

	SetPoint(POINT_STAT, lStat);
	SetPoint(POINT_SKILL, lSkillActive);
	SetPoint(POINT_SUB_SKILL, lSkillSub);
	SetPoint(POINT_HORSE_SKILL, lSkillHorse);
	SetPoint(POINT_LEVEL_STEP, lLevelStep);
	SetPoint(POINT_STAT_RESET_COUNT, lStatResetCount);

	SetPoint(POINT_ST, GetRealPoint(POINT_ST));
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetPart(PART_MAIN, GetOriginalPart(PART_MAIN));
	SetPart(PART_WEAPON, GetOriginalPart(PART_WEAPON));
	SetPart(PART_HEAD, GetOriginalPart(PART_HEAD));
	SetPart(PART_HAIR, GetOriginalPart(PART_HAIR));
#ifdef ENABLE_ACCE_SYSTEM
	SetPart(PART_SASH, GetOriginalPart(PART_SASH));
#endif
#ifdef ENABLE_AURA_SYSTEM
	SetPart(PART_AURA, GetOriginalPart(PART_AURA));
#endif

	SetPoint(POINT_PARTY_ATTACKER_BONUS, lAttackerBonus);
	SetPoint(POINT_PARTY_TANKER_BONUS, lTankerBonus);
	SetPoint(POINT_PARTY_BUFFER_BONUS, lBufferBonus);
	SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, lSkillMasterBonus);
	SetPoint(POINT_PARTY_HASTE_BONUS, lHasteBonus);
	SetPoint(POINT_PARTY_DEFENDER_BONUS, lDefenderBonus);

	SetPoint(POINT_HP_RECOVERY, lHPRecovery);
	SetPoint(POINT_SP_RECOVERY, lSPRecovery);

	int iMaxHP, iMaxSP;
	int iMaxStamina;

	if (IsPC())
	{
		// 최대 생명력/정신력
		iMaxHP = JobInitialPoints[GetJob()].max_hp + m_points.iRandomHP + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].hp_per_ht;
		iMaxSP = JobInitialPoints[GetJob()].max_sp + m_points.iRandomSP + GetPoint(POINT_IQ) * JobInitialPoints[GetJob()].sp_per_iq;
		iMaxStamina = JobInitialPoints[GetJob()].max_stamina + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].stamina_per_con;

		{
			CSkillProto* pkSk = CSkillManager::instance().Get(SKILL_ADD_HP);

			if (nullptr != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_ADD_HP) / 100.0f);

				iMaxHP += static_cast<int>(pkSk->kPointPoly.Eval());
			}
		}

		SetPoint(POINT_MOV_SPEED, 111);
		SetPoint(POINT_ATT_SPEED, 112);
		PointChange(POINT_ATT_SPEED, GetPoint(POINT_PARTY_HASTE_BONUS));
		SetPoint(POINT_CASTING_SPEED, 100);
	}
	else
	{
		iMaxHP = m_pkMobData->m_table.dwMaxHP;
		iMaxSP = 0;
		iMaxStamina = 0;

		SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		SetPoint(POINT_MOV_SPEED, m_pkMobData->m_table.sMovingSpeed);
		SetPoint(POINT_CASTING_SPEED, m_pkMobData->m_table.sAttackSpeed);
	}

	if (IsPC())
	{
		// 말 타고 있을 때는 기본 스탯이 말의 기준 스탯보다 낮으면 높게 만든다.
		// 따라서 말의 기준 스탯이 무사 기준이므로, 수라/무당은 전체 스탯 합이
		// 대채적으로 더 올라가게 될 것이다.
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		if (GetMountVnum() && !GetWear(WEAR_COSTUME_MOUNT))
#else
		if (GetMountVnum())
#endif
		{
			if (GetHorseST() > GetPoint(POINT_ST))
				PointChange(POINT_ST, GetHorseST() - GetPoint(POINT_ST));

			if (GetHorseDX() > GetPoint(POINT_DX))
				PointChange(POINT_DX, GetHorseDX() - GetPoint(POINT_DX));

			if (GetHorseHT() > GetPoint(POINT_HT))
				PointChange(POINT_HT, GetHorseHT() - GetPoint(POINT_HT));

			if (GetHorseIQ() > GetPoint(POINT_IQ))
				PointChange(POINT_IQ, GetHorseIQ() - GetPoint(POINT_IQ));
		}
	}

	ComputeBattlePoints();

	if (iMaxHP != GetMaxHP())
	{
		SetRealPoint(POINT_MAX_HP, iMaxHP); // 기본HP를 RealPoint에 저장해 놓는다.
	}

	PointChange(POINT_MAX_HP, 0);

	if (iMaxSP != GetMaxSP())
	{
		SetRealPoint(POINT_MAX_SP, iMaxSP); // 기본SP를 RealPoint에 저장해 놓는다.
	}

	PointChange(POINT_MAX_SP, 0);

	SetMaxStamina(iMaxStamina);
	// @fixme118 part1
	int iCurHP = this->GetHP();
	int iCurSP = this->GetSP();

	m_pointsInstant.dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; i++)
	{
		LPITEM pItem = GetWear(i);
		if (pItem)
		{
			pItem->ModifyPoints(true);
			SET_BIT(m_pointsInstant.dwImmuneFlag, GetWear(i)->GetImmuneFlag());
		}
	}

	// 용혼석 시스템
	// ComputePoints에서는 케릭터의 모든 속성값을 초기화하고,
	// 아이템, 버프 등에 관련된 모든 속성값을 재계산하기 때문에,
	// 용혼석 시스템도 ActiveDeck에 있는 모든 용혼석의 속성값을 다시 적용시켜야 한다.
	if (DragonSoul_IsDeckActivated())
	{
		for (int i = WEAR_MAX_NUM + DS_SLOT_MAX * DragonSoul_GetActiveDeck();
			i < WEAR_MAX_NUM + DS_SLOT_MAX * (DragonSoul_GetActiveDeck() + 1); i++)
		{
			LPITEM pItem = GetWear(i);
			if (pItem)
			{
				if (DSManager::instance().IsTimeLeftDragonSoul(pItem))
					pItem->ModifyPoints(true);
			}
		}
	}

	if (GetHP() > GetMaxHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetSP() > GetMaxSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());

	ComputeSkillPoints();

	RefreshAffect();
	// @fixme118 part2 (before petsystem stuff)
	if (IsPC())
	{
		if (this->GetHP() != iCurHP)
			this->PointChange(POINT_HP, iCurHP - this->GetHP());
		if (this->GetSP() != iCurSP)
			this->PointChange(POINT_SP, iCurSP - this->GetSP());
	}

	CPetSystem * pPetSystem = GetPetSystem();
	if (nullptr != pPetSystem)
	{
		pPetSystem->RefreshBuff();
	}
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		it->second->GiveAllAttributes();
	}
	UpdatePacket();
}

#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
void CHARACTER::ComputePointsWithoutUpdatePacket()
{
	long lStat = GetPoint(POINT_STAT);
	long lStatResetCount = GetPoint(POINT_STAT_RESET_COUNT);
	long lSkillActive = GetPoint(POINT_SKILL);
	long lSkillSub = GetPoint(POINT_SUB_SKILL);
	long lSkillHorse = GetPoint(POINT_HORSE_SKILL);
	long lLevelStep = GetPoint(POINT_LEVEL_STEP);

	long lAttackerBonus = GetPoint(POINT_PARTY_ATTACKER_BONUS);
	long lTankerBonus = GetPoint(POINT_PARTY_TANKER_BONUS);
	long lBufferBonus = GetPoint(POINT_PARTY_BUFFER_BONUS);
	long lSkillMasterBonus = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
	long lHasteBonus = GetPoint(POINT_PARTY_HASTE_BONUS);
	long lDefenderBonus = GetPoint(POINT_PARTY_DEFENDER_BONUS);

	long lHPRecovery = GetPoint(POINT_HP_RECOVERY);
	long lSPRecovery = GetPoint(POINT_SP_RECOVERY);

	memset(m_pointsInstant.points, 0, sizeof(m_pointsInstant.points));
	BuffOnAttr_ClearAll();
	m_SkillDamageBonus.clear();

	SetPoint(POINT_STAT, lStat);
	SetPoint(POINT_SKILL, lSkillActive);
	SetPoint(POINT_SUB_SKILL, lSkillSub);
	SetPoint(POINT_HORSE_SKILL, lSkillHorse);
	SetPoint(POINT_LEVEL_STEP, lLevelStep);
	SetPoint(POINT_STAT_RESET_COUNT, lStatResetCount);

	SetPoint(POINT_ST, GetRealPoint(POINT_ST));
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetPart(PART_MAIN, GetOriginalPart(PART_MAIN));
	SetPart(PART_WEAPON, GetOriginalPart(PART_WEAPON));
	SetPart(PART_HEAD, GetOriginalPart(PART_HEAD));
	SetPart(PART_HAIR, GetOriginalPart(PART_HAIR));
#ifdef ENABLE_ACCE_SYSTEM
	SetPart(PART_SASH, GetOriginalPart(PART_SASH));
#endif
#ifdef ENABLE_AURA_SYSTEM
	SetPart(PART_AURA, GetOriginalPart(PART_AURA));
#endif

	SetPoint(POINT_PARTY_ATTACKER_BONUS, lAttackerBonus);
	SetPoint(POINT_PARTY_TANKER_BONUS, lTankerBonus);
	SetPoint(POINT_PARTY_BUFFER_BONUS, lBufferBonus);
	SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, lSkillMasterBonus);
	SetPoint(POINT_PARTY_HASTE_BONUS, lHasteBonus);
	SetPoint(POINT_PARTY_DEFENDER_BONUS, lDefenderBonus);

	SetPoint(POINT_HP_RECOVERY, lHPRecovery);
	SetPoint(POINT_SP_RECOVERY, lSPRecovery);

	int iMaxHP, iMaxSP;
	int iMaxStamina;

	if (IsPC())
	{
		// 최대 생명력/정신력
		iMaxHP = JobInitialPoints[GetJob()].max_hp + m_points.iRandomHP + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].hp_per_ht;
		iMaxSP = JobInitialPoints[GetJob()].max_sp + m_points.iRandomSP + GetPoint(POINT_IQ) * JobInitialPoints[GetJob()].sp_per_iq;
		iMaxStamina = JobInitialPoints[GetJob()].max_stamina + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].stamina_per_con;

		{
			CSkillProto* pkSk = CSkillManager::instance().Get(SKILL_ADD_HP);

			if (nullptr != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_ADD_HP) / 100.0f);

				iMaxHP += static_cast<int>(pkSk->kPointPoly.Eval());
			}
		}

		SetPoint(POINT_MOV_SPEED, 111);
		SetPoint(POINT_ATT_SPEED, 112);
		PointChange(POINT_ATT_SPEED, GetPoint(POINT_PARTY_HASTE_BONUS));
		SetPoint(POINT_CASTING_SPEED, 100);
	}
	else
	{
		iMaxHP = m_pkMobData->m_table.dwMaxHP;
		iMaxSP = 0;
		iMaxStamina = 0;

		SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		SetPoint(POINT_MOV_SPEED, m_pkMobData->m_table.sMovingSpeed);
		SetPoint(POINT_CASTING_SPEED, m_pkMobData->m_table.sAttackSpeed);
	}

	if (IsPC())
	{
		// 말 타고 있을 때는 기본 스탯이 말의 기준 스탯보다 낮으면 높게 만든다.
		// 따라서 말의 기준 스탯이 무사 기준이므로, 수라/무당은 전체 스탯 합이
		// 대채적으로 더 올라가게 될 것이다.
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		if (GetMountVnum() && !GetWear(WEAR_COSTUME_MOUNT))
#else
		if (GetMountVnum())
#endif
		{
			if (GetHorseST() > GetPoint(POINT_ST))
				PointChange(POINT_ST, GetHorseST() - GetPoint(POINT_ST));

			if (GetHorseDX() > GetPoint(POINT_DX))
				PointChange(POINT_DX, GetHorseDX() - GetPoint(POINT_DX));

			if (GetHorseHT() > GetPoint(POINT_HT))
				PointChange(POINT_HT, GetHorseHT() - GetPoint(POINT_HT));

			if (GetHorseIQ() > GetPoint(POINT_IQ))
				PointChange(POINT_IQ, GetHorseIQ() - GetPoint(POINT_IQ));
		}
	}

	ComputeBattlePoints();

	if (iMaxHP != GetMaxHP())
	{
		SetRealPoint(POINT_MAX_HP, iMaxHP); // 기본HP를 RealPoint에 저장해 놓는다.
	}

	PointChange(POINT_MAX_HP, 0);

	if (iMaxSP != GetMaxSP())
	{
		SetRealPoint(POINT_MAX_SP, iMaxSP); // 기본SP를 RealPoint에 저장해 놓는다.
	}

	PointChange(POINT_MAX_SP, 0);

	SetMaxStamina(iMaxStamina);
	// @fixme118 part1
	int iCurHP = this->GetHP();
	int iCurSP = this->GetSP();

	m_pointsInstant.dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; i++)
	{
		LPITEM pItem = GetWear(i);
		if (pItem)
		{
			pItem->ModifyPoints(true);
			SET_BIT(m_pointsInstant.dwImmuneFlag, GetWear(i)->GetImmuneFlag());
		}
	}

	// 용혼석 시스템
	// ComputePoints에서는 케릭터의 모든 속성값을 초기화하고,
	// 아이템, 버프 등에 관련된 모든 속성값을 재계산하기 때문에,
	// 용혼석 시스템도 ActiveDeck에 있는 모든 용혼석의 속성값을 다시 적용시켜야 한다.
	if (DragonSoul_IsDeckActivated())
	{
		for (int i = WEAR_MAX_NUM + DS_SLOT_MAX * DragonSoul_GetActiveDeck();
			i < WEAR_MAX_NUM + DS_SLOT_MAX * (DragonSoul_GetActiveDeck() + 1); i++)
		{
			LPITEM pItem = GetWear(i);
			if (pItem)
			{
				if (DSManager::instance().IsTimeLeftDragonSoul(pItem))
					pItem->ModifyPoints(true);
			}
		}
	}

	if (GetHP() > GetMaxHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetSP() > GetMaxSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());

	ComputeSkillPoints();

	RefreshAffect();
	// @fixme118 part2 (before petsystem stuff)
	if (IsPC())
	{
		if (this->GetHP() != iCurHP)
			this->PointChange(POINT_HP, iCurHP - this->GetHP());
		if (this->GetSP() != iCurSP)
			this->PointChange(POINT_SP, iCurSP - this->GetSP());
	}

	CPetSystem * pPetSystem = GetPetSystem();
	if (nullptr != pPetSystem)
	{
		pPetSystem->RefreshBuff();
	}
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		it->second->GiveAllAttributes();
	}
}
#endif

// m_dwPlayStartTime의 단위는 milisecond다. 데이터베이스에는 분단위로 기록하기
// 때문에 플레이시간을 계산할 때 / 60000 으로 나눠서 하는데, 그 나머지 값이 남았
// 을 때 여기에 dwTimeRemain으로 넣어서 제대로 계산되도록 해주어야 한다.
void CHARACTER::ResetPlayTime(DWORD dwTimeRemain)
{
	m_dwPlayStartTime = get_dword_time() - dwTimeRemain;
}

const int aiRecoveryPercents[10] =
{
	1, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

EVENTFUNC(recovery_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("recovery_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}

	if (!ch->IsPC())
	{
		if (CMobVnumHelper::IsShadowWarriorMob(ch->GetRaceNum()) == true)
			return 0;

		if (CMobVnumHelper::IsBaashidoWarriorMob(ch->GetRaceNum()) == true)
			return 0;

		if (CMobVnumHelper::IsEventBeranSetaou(ch->GetRaceNum()) == true)
			return 0;

		if (CMobVnumHelper::IsEventNemere(ch->GetRaceNum()) == true)
			return 0;

		if (CMobVnumHelper::IsEventRazador(ch->GetRaceNum()) == true)
			return 0;

		if (CMobVnumHelper::IsEventRedDragon(ch->GetRaceNum()) == true)
			return 0;

		if (ch->IsAffectFlag(AFF_POISON))
			return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
#ifdef ENABLE_WOLFMAN_CHARACTER
		if (ch->IsAffectFlag(AFF_BLEEDING))
			return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
#endif
		if (2493 == ch->GetMobTable().dwVnum)
		{
			int regenPct = BlueDragon_GetRangeFactor("hp_regen", ch->GetHPPct());
			regenPct += ch->GetMobTable().bRegenPercent;

			for (int i = 1; i <= 4; ++i)
			{
				if (REGEN_PECT_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap(ch->GetMapIndex(), dwDragonStoneID);

					regenPct += (val * cnt);

					break;
				}
			}

			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() * regenPct) / 100));
		}
		else if (!ch->IsDoor())
		{
			ch->MonsterLog("HP_REGEN +%d", MAX(1, (ch->GetMaxHP() * ch->GetMobTable().bRegenPercent) / 100));
			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() * ch->GetMobTable().bRegenPercent) / 100));
		}

		if (ch->GetHP() >= ch->GetMaxHP())
		{
			ch->m_pkRecoveryEvent = nullptr;
			return 0;
		}

		if (2493 == ch->GetMobTable().dwVnum)
		{
			for (int i = 1; i <= 4; ++i)
			{
				if (REGEN_TIME_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap(ch->GetMapIndex(), dwDragonStoneID);

					return PASSES_PER_SEC(MAX(1, (ch->GetMobTable().bRegenCycle - (val * cnt))));
				}
			}
		}

		return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
	}
	else
	{
		//
		// PC 회복
		//
		ch->CheckTarget();
		//ch->UpdateSectree(); // 여기서 이걸 왜하지?
		ch->UpdateKillerMode();

		if (ch->IsAffectFlag(AFF_POISON) == true)
		{
			// 중독인 경우 자동회복 금지
			// 파법술인 경우 자동회복 금지
			return 3;
		}
#ifdef ENABLE_WOLFMAN_CHARACTER
		if (ch->IsAffectFlag(AFF_BLEEDING))
			return 3;
#endif
		int iSec = (get_dword_time() - ch->GetLastMoveTime()) / 3000;

		// SP 회복 루틴.
		// 왜 이걸로 해서 함수로 빼놨는가 ?!
		ch->DistributeSP(ch);

		if (ch->GetMaxHP() <= ch->GetHP())
			return PASSES_PER_SEC(3);

		int iPercent = 0;
		int iAmount = 0;

		{
			iPercent = aiRecoveryPercents[MIN(9, iSec)];
			iAmount = 15 + (ch->GetMaxHP() * iPercent) / 100;
		}

		iAmount += (iAmount * ch->GetPoint(POINT_HP_REGEN)) / 100;

		sys_log(1, "RECOVERY_EVENT: %s %d HP_REGEN %d HP +%d", ch->GetName(), iPercent, ch->GetPoint(POINT_HP_REGEN), iAmount);

		ch->PointChange(POINT_HP, iAmount, false);
		return PASSES_PER_SEC(3);
	}
}

void CHARACTER::StartRecoveryEvent()
{
	if (m_pkRecoveryEvent)
		return;

	if (IsDead() || IsStun())
		return;

	if (IsNPC() && GetHP() >= GetMaxHP()) // 몬스터는 체력이 다 차있으면 시작 안한다.
		return;

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if ((MeleyLair::CMgr::instance().IsMeleyMap(GetMapIndex())) && ((GetRaceNum() == (WORD)(MeleyLair::STATUE_VNUM)) || ((GetRaceNum() == (WORD)(MeleyLair::BOSS_VNUM)))))
		return;
#endif

	char_event_info * info = AllocEventInfo<char_event_info>();

	info->ch = this;

	int iSec = IsPC() ? 3 : (MAX(1, GetMobTable().bRegenCycle));
	m_pkRecoveryEvent = event_create(recovery_event, info, PASSES_PER_SEC(iSec));
}

void CHARACTER::Standup()
{
	struct packet_position pack_position;

	if (!IsPosition(POS_SITTING))
		return;

	SetPosition(POS_STANDING);

	sys_log(1, "STANDUP: %s", GetName());

	pack_position.header = HEADER_GC_CHARACTER_POSITION;
	pack_position.vid = GetVID();
	pack_position.position = POSITION_GENERAL;

	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::Sitdown(int is_ground)
{
	struct packet_position pack_position;

	if (IsPosition(POS_SITTING))
		return;

	SetPosition(POS_SITTING);
	sys_log(1, "SITDOWN: %s", GetName());

	pack_position.header = HEADER_GC_CHARACTER_POSITION;
	pack_position.vid = GetVID();
	pack_position.position = POSITION_SITTING_GROUND;
	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::SetRotation(float fRot)
{
	m_pointsInstant.fRot = fRot;
}

// x, y 방향으로 보고 선다.
void CHARACTER::SetRotationToXY(long x, long y)
{
	SetRotation(GetDegreeFromPositionXY(GetX(), GetY(), x, y));
}

bool CHARACTER::CannotMoveByAffect() const
{
	return (IsAffectFlag(AFF_STUN));
}

bool CHARACTER::CanMove() const
{
	if (CannotMoveByAffect())
		return false;

	if (GetMyShop())	// 상점 연 상태에서는 움직일 수 없음
		return false;

	// 0.2초 전이라면 움직일 수 없다.
	/*
	   if (get_float_time() - m_fSyncTime < 0.2f)
	   return false;
	 */
	return true;
}

// 무조건 x, y 위치로 이동 시킨다.
bool CHARACTER::Sync(long x, long y)
{
	if (!GetSectree())
		return false;

#ifdef ENABLE_ABUSE_SYSTEM
	if (IsPC() && IsDead()) 
	{
		GetAbuseController()->DeadWalk();
		return false;
	}
#endif

	LPSECTREE new_tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), x, y);

	if (!new_tree)
	{
		if (GetDesc())
		{
			sys_err("cannot find tree at %d %d (name: %s)", x, y, GetName());
			//GetDesc()->SetPhase(PHASE_CLOSE);
		}
		else
		{
			sys_err("no tree: %s %d %d %d", GetName(), x, y, GetMapIndex());
			Dead();
		}

		return false;
	}
	
#ifdef ENABLE_ABUSE_SYSTEM
	if (IsPC()) 
	{
		spAbuseController controller = GetAbuseController();
		controller->CheckSpeedhack(x, y);
	}
#endif

	SetRotationToXY(x, y);
	SetXYZ(x, y, 0);

	if (GetDungeon())
	{
		// 던젼용 이벤트 속성 변화
		int iLastEventAttr = m_iEventAttr;
		m_iEventAttr = new_tree->GetEventAttribute(x, y);

		if (m_iEventAttr != iLastEventAttr)
		{
			if (GetParty())
			{
				quest::CQuestManager::instance().AttrOut(GetParty()->GetLeaderPID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetParty()->GetLeaderPID(), this, m_iEventAttr);
			}
			else
			{
				quest::CQuestManager::instance().AttrOut(GetPlayerID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetPlayerID(), this, m_iEventAttr);
			}
		}
	}

	if (GetSectree() != new_tree)
	{
		if (!IsNPC())
		{
			SECTREEID id = new_tree->GetID();
			SECTREEID old_id = GetSectree()->GetID();

			const float fDist = DISTANCE_SQRT(id.coord.x - old_id.coord.x, id.coord.y - old_id.coord.y);
			sys_log(0, "SECTREE DIFFER: %s %dx%d was %dx%d dist %.1fm",
					GetName(),
					id.coord.x,
					id.coord.y,
					old_id.coord.x,
					old_id.coord.y,
					fDist);
		}

		new_tree->InsertEntity(this);
	}

	return true;
}

void CHARACTER::Stop()
{
	if (!IsState(m_stateIdle))
	{
		MonsterLog("[IDLE] Starting");
	}

	GotoState(m_stateIdle);

	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
}

bool CHARACTER::Goto(long x, long y)
{
	// TODO 거리체크 필요
	// 같은 위치면 이동할 필요 없음 (자동 성공)
	if (GetX() == x && GetY() == y)
		return false;

	if (m_posDest.x == x && m_posDest.y == y)
	{
		if (!IsState(m_stateMove))
		{
			m_dwStateDuration = 4;
			GotoState(m_stateMove);
		}
		return false;
	}

	m_posDest.x = x;
	m_posDest.y = y;

	CalculateMoveDuration();

	m_dwStateDuration = 4;

	if (!IsState(m_stateMove))
	{
		MonsterLog("[MOVE] %s", GetVictim() ? "To victim" : "Free");

		if (GetVictim())
		{
			//MonsterChat(MONSTER_CHAT_CHASE);
			MonsterChat(MONSTER_CHAT_ATTACK);
		}
	}

	GotoState(m_stateMove);

	return true;
}

DWORD CHARACTER::GetMotionMode() const
{
	DWORD dwMode = MOTION_MODE_GENERAL;

	if (IsPolymorphed())
		return dwMode;

	LPITEM pkItem;
	if ((pkItem = GetWear(WEAR_WEAPON)))
	{
		switch (pkItem->GetProto()->bSubType)
		{
		case WEAPON_SWORD:
			dwMode = MOTION_MODE_ONEHAND_SWORD;
			break;

		case WEAPON_TWO_HANDED:
			dwMode = MOTION_MODE_TWOHAND_SWORD;
			break;

		case WEAPON_DAGGER:
			dwMode = MOTION_MODE_DUALHAND_SWORD;
			break;

		case WEAPON_BOW:
			dwMode = MOTION_MODE_BOW;
			break;

		case WEAPON_BELL:
			dwMode = MOTION_MODE_BELL;
			break;

		case WEAPON_FAN:
			dwMode = MOTION_MODE_FAN;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case WEAPON_CLAW:
			dwMode = MOTION_MODE_CLAW;
			break;
#endif
		}
	}

	return dwMode;
}

float CHARACTER::GetMoveMotionSpeed() const
{
	DWORD dwMode = GetMotionMode();

	const CMotion* pkMotion = nullptr;

	if (!GetMountVnum())
		pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(dwMode, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	else
	{
		pkMotion = CMotionManager::instance().GetMotion(GetMountVnum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));

		if (!pkMotion)
			pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_HORSE, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	}

	if (pkMotion)
		return -pkMotion->GetAccumVector().y / pkMotion->GetDuration();
	else
		return 449.9f;
}

float CHARACTER::GetMoveSpeed() const
{
	return GetMoveMotionSpeed() * 10000 / CalculateDuration(GetLimitPoint(POINT_MOV_SPEED), 10000);
}

void CHARACTER::CalculateMoveDuration()
{
	m_posStart.x = GetX();
	m_posStart.y = GetY();

	float fDist = DISTANCE_SQRT(m_posStart.x - m_posDest.x, m_posStart.y - m_posDest.y);

	float motionSpeed = GetMoveMotionSpeed();

	m_dwMoveDuration = CalculateDuration(GetLimitPoint(POINT_MOV_SPEED),
		(int)((fDist / motionSpeed) * 1000.0f));

	if (IsNPC())
		sys_log(1, "%s: GOTO: distance %f, spd %u, duration %u, motion speed %f pos %d %d -> %d %d",
			GetName(), fDist, GetLimitPoint(POINT_MOV_SPEED), m_dwMoveDuration, motionSpeed,
			m_posStart.x, m_posStart.y, m_posDest.x, m_posDest.y);

	m_dwMoveStartTime = get_dword_time();
}

// x y 위치로 이동 한다. (이동할 수 있는 가 없는 가를 확인 하고 Sync 메소드로 실제 이동 한다)
// 서버는 char의 x, y 값을 바로 바꾸지만,
// 클라에서는 이전 위치에서 바꾼 x, y까지 interpolation한다.
// 걷거나 뛰는 것은 char의 m_bNowWalking에 달려있다.
// Warp를 의도한 것이라면 Show를 사용할 것.
bool CHARACTER::Move(long x, long y)
{
	// 같은 위치면 이동할 필요 없음 (자동 성공)
	if (GetX() == x && GetY() == y)
		return true;

	if (test_server)
		if (m_bDetailLog)
			sys_log(0, "%s position %u %u", GetName(), x, y);

	OnMove();
	return Sync(x, y);
}

#ifdef ENABLE_BUFFI_SYSTEM
void CHARACTER::SendSupportShamanSkillPacket(DWORD skill_vnum)
{
	TPacketGCSupportShamanSkill pack;
	pack.bHeader = HEADER_GC_SUPPORT_SHAMAN_SKILL;
	pack.dwVnum = skill_vnum;
	pack.dwVid = GetVID();
	pack.dwLevel = 41;

	PacketView(&pack, sizeof(TPacketGCSupportShamanSkill), this);
}
#endif

void CHARACTER::SendMovePacket(BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, int iRot)
{
	TPacketGCMove pack;

	if (bFunc == FUNC_WAIT)
	{
		x = m_posDest.x;
		y = m_posDest.y;
		dwDuration = m_dwMoveDuration;
	}

	EncodeMovePacket(pack, GetVID(), bFunc, bArg, x, y, dwDuration, dwTime, iRot == -1 ? (int)GetRotation() / 5 : iRot);
	PacketView(&pack, sizeof(TPacketGCMove), this);
}

int CHARACTER::GetRealPoint(BYTE type) const
{
	return m_points.points[type];
}

void CHARACTER::SetRealPoint(BYTE type, int val)
{
	m_points.points[type] = val;
}

int CHARACTER::GetPolymorphPoint(BYTE type) const
{
	if (IsPolymorphed() && !IsPolyMaintainStat())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob* pMob = CMobManager::instance().Get(dwMobVnum);
		int iPower = GetPolymorphPower();

		if (pMob)
		{
			switch (type)
			{
			case POINT_ST:
				if ((GetJob() == JOB_SHAMAN) || ((GetJob() == JOB_SURA) && (GetSkillGroup() == 2)))
					return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_IQ);
				return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_ST);

			case POINT_HT:
				return pMob->m_table.bCon * iPower / 100 + GetPoint(POINT_HT);

			case POINT_IQ:
				return pMob->m_table.bInt * iPower / 100 + GetPoint(POINT_IQ);

			case POINT_DX:
				return pMob->m_table.bDex * iPower / 100 + GetPoint(POINT_DX);
			}
		}
	}

	return GetPoint(type);
}

int CHARACTER::GetPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	int val = m_pointsInstant.points[type];
	int max_val = INT_MAX;

	switch (type)
	{
	case POINT_STEAL_HP:
	case POINT_STEAL_SP:
		max_val = 50;
		break;
	}

	if (val > max_val) 
	{
		sys_err("POINT_ERROR: %s type %d val %d (max: %d)", GetName(), val, max_val);
		val = max_val;
	}

	return (val);
}

int CHARACTER::GetLimitPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	int val = m_pointsInstant.points[type];
	int max_val = INT_MAX;
	int limit = INT_MAX;
	int min_limit = -INT_MAX;

	switch (type)
	{
	case POINT_ATT_SPEED:
		min_limit = 0;
		if (IsPC())
			limit = pc_max_attack_speed;
		else
			limit = mob_max_attack_speed;
		break;

	case POINT_MOV_SPEED:
		min_limit = 0;

		if (IsPC())
			limit = pc_max_movement_speed;
		else
			limit = mob_max_movement_speed;
		break;

	case POINT_STEAL_HP:
	case POINT_STEAL_SP:
		limit = 50;
		max_val = 50;
		break;

	case POINT_MALL_ATTBONUS:
	case POINT_MALL_DEFBONUS:
		limit = 20;
		max_val = 50;
		break;
	}

	if (val > max_val)
	{
		sys_err("POINT_ERROR: %s type %d val %d (max: %d)", GetName(), type, val, max_val);
		val = max_val;
	}
	
	if (val > limit)
		val = limit;

	if (val < min_limit)
		val = min_limit;

	return (val);
}

void CHARACTER::SetPoint(BYTE type, int val)
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return;
	}

	m_pointsInstant.points[type] = val;

	// 아직 이동이 다 안끝났다면 이동 시간 계산을 다시 해야 한다.
	if (type == POINT_MOV_SPEED && get_dword_time() < m_dwMoveStartTime + m_dwMoveDuration)
	{
		CalculateMoveDuration();
	}
}

INT CHARACTER::GetAllowedGold() const
{
	if (GetLevel() <= 10)
		return 100000;
	else if (GetLevel() <= 20)
		return 500000;
	else
		return 50000000;
}

void CHARACTER::CheckMaximumPoints()
{
	if (GetMaxHP() < GetHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetMaxSP() < GetSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());
}

void CHARACTER::PointChange(BYTE type, int amount, bool bAmount, bool bBroadcast)
{
	int val = 0;

	//sys_log(0, "PointChange %d %d | %d -> %d cHP %d mHP %d", type, amount, GetPoint(type), GetPoint(type)+amount, GetHP(), GetMaxHP());

	switch (type)
	{
	case POINT_NONE:
		return;

	case POINT_LEVEL:
		if ((GetLevel() + amount) > gPlayerMaxLevel)
			return;

		SetLevel(GetLevel() + amount);
		val = GetLevel();

		sys_log(0, "LEVELUP: %s %d NEXT EXP %d", GetName(), GetLevel(), GetNextExp());
		
		if (features::IsFeatureEnabled(features::SKILL_SELECT_EX))
		{
			if (val >= 5 && (GetSkillGroup() == 0))
			{
				char buf[64];
				snprintf(buf, sizeof(buf), "OpenSkillsGui %d", GetJob());
				ChatPacket(CHAT_TYPE_COMMAND, buf);
			}
		}

		PointChange(POINT_NEXT_EXP, GetNextExp(), false);

#ifdef ENABLE_ANNOUNCEMENT_LEVELUP
		switch (val)
		{
		case 99:
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
			char szNoticeLevelUp[QUERY_MAX_LEN];
			snprintf(szNoticeLevelUp, sizeof(szNoticeLevelUp), "<Bilgi> [CH%d]: [%s] isimli oyuncumuz [%u] seviye olmustur tebrikler !", g_bChannel, GetName(), GetLevel());
			BroadcastNotice(szNoticeLevelUp);
			break;

		default:
			break;
		}
#endif
		if (amount)
		{
			int biodurum = GetQuestFlag("bio.durum");
			if (biodurum == 0 && GetLevel() >= 30)
			{
				SetQuestFlag("bio.durum", 1);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 31 && GetLevel() >= 40)
			{
				SetQuestFlag("bio.durum", 2);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 41 && GetLevel() >= 50)
			{
				SetQuestFlag("bio.durum", 3);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 51 && GetLevel() >= 60)
			{
				SetQuestFlag("bio.durum", 4);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 61 && GetLevel() >= 70)
			{
				SetQuestFlag("bio.durum", 5);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 71 && GetLevel() >= 80)
			{
				SetQuestFlag("bio.durum", 6);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 81 && GetLevel() >= 85)
			{
				SetQuestFlag("bio.durum", 7);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 86 && GetLevel() >= 90)
			{
				SetQuestFlag("bio.durum", 8);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 91 && GetLevel() >= 92)
			{
				SetQuestFlag("bio.durum", 9);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			else if (biodurum == 93 && GetLevel() >= 94)
			{
				SetQuestFlag("bio.durum", 10);
				SetQuestFlag("bio.verilen", 0);
				SetQuestFlag("bio.ruhtasi", 0);
				SetQuestFlag("bio.kalan", get_global_time());
				int bioverilen = GetQuestFlag("bio.verilen");
				int biokalan = GetQuestFlag("bio.kalan");
				biodurum = GetQuestFlag("bio.durum");
				ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d ", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan);
			}
			quest::CQuestManager::instance().LevelUp(GetPlayerID());

			// UpdateP2P
			TPacketGGLogin p;
			p.bHeader = HEADER_GG_LOGIN;
			strlcpy(p.szName, GetName(), sizeof(p.szName));
			p.dwPID = GetPlayerID();
			p.bEmpire = GetEmpire();
			p.lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(GetX(), GetY());
			p.bChannel = g_bChannel;
			p.iLevel = GetLevel();
			P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogin));

			LogManager::instance().LevelLog(this, val, GetRealPoint(POINT_PLAYTIME) + (get_dword_time() - m_dwPlayStartTime) / 60000);

			if (GetGuild())
			{
				GetGuild()->LevelChange(GetPlayerID(), GetLevel());
			}

			if (GetParty())
			{
				GetParty()->RequestSetMemberLevel(GetPlayerID(), GetLevel());
			}
		}
		break;

	case POINT_NEXT_EXP:
		val = GetNextExp();
		bAmount = false;
		break;

	case POINT_EXP:
	{
		DWORD exp = GetExp();
		DWORD next_exp = GetNextExp();
		if (amount < 0 && (int)exp < -amount)
		{
			sys_log(1, "%s AMOUNT < 0 %d, CUR EXP: %d", GetName(), -amount, exp);
			amount = -static_cast<int>(exp);

			SetExp(exp + amount);
			val = GetExp();
		}
		else
		{
			if (gPlayerMaxLevel <= GetLevel())
				return;

			if (test_server)
				ChatPacket(CHAT_TYPE_INFO, "You have gained %d exp.", amount);

			DWORD iExpBalance = 0;
			if (exp + amount >= next_exp)
			{
				iExpBalance = (exp + amount) - next_exp;
				amount = next_exp - exp;

				SetExp(0);
				exp = next_exp;
			}
			else
			{
				SetExp(exp + amount);
				exp = GetExp();
			}

			DWORD q = DWORD(next_exp / 4.0f);
			int iLevStep = GetRealPoint(POINT_LEVEL_STEP);
			if (iLevStep >= 4)
			{
				sys_err("%s LEVEL_STEP bigger than 4! (%d)", GetName(), iLevStep);
				iLevStep = 4;
			}

			if (exp >= next_exp && iLevStep < 4)
			{
				for (int i = 0; i < 4 - iLevStep; ++i)
					PointChange(POINT_LEVEL_STEP, 1, false, true);
			}
			else if (exp >= q * 3 && iLevStep < 3)
			{
				for (int i = 0; i < 3 - iLevStep; ++i)
					PointChange(POINT_LEVEL_STEP, 1, false, true);
			}
			else if (exp >= q * 2 && iLevStep < 2)
			{
				for (int i = 0; i < 2 - iLevStep; ++i)
					PointChange(POINT_LEVEL_STEP, 1, false, true);
			}
			else if (exp >= q && iLevStep < 1)
				PointChange(POINT_LEVEL_STEP, 1);

			if (iExpBalance)
			{
				PointChange(POINT_EXP, iExpBalance);
			}

			val = GetExp();
		}
	}
	break;

	case POINT_LEVEL_STEP:
		if (amount > 0)
		{
			val = GetPoint(POINT_LEVEL_STEP) + amount;

			switch (val)
			{
			case 1:
			case 2:
			case 3:
				if ((GetLevel() <= g_iStatusPointGetLevelLimit) &&
					(GetLevel() <= gPlayerMaxLevel)) // @fixme104
					PointChange(POINT_STAT, 1);
				break;

			case 4:
			{
				int iHP = number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end);
				int iSP = number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end);

				m_points.iRandomHP += iHP;
				m_points.iRandomSP += iSP;
				if (GetSkillGroup())
				{
					if (GetLevel() >= 5)
						PointChange(POINT_SKILL, 1);

					if (GetLevel() >= 9)
						PointChange(POINT_SUB_SKILL, 1);
				}

				PointChange(POINT_MAX_HP, iHP);
				PointChange(POINT_MAX_SP, iSP);
				PointChange(POINT_LEVEL, 1, false, true);

				val = 0;
			}
			break;
			}
			PointChange(POINT_HP, GetMaxHP() - GetHP());
			PointChange(POINT_SP, GetMaxSP() - GetSP());
			PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());

			SetPoint(POINT_LEVEL_STEP, val);
			SetRealPoint(POINT_LEVEL_STEP, val);

			Save();
		}
		else
			val = GetPoint(POINT_LEVEL_STEP);

		break;

	case POINT_HP:
	{
		if (IsDead() || IsStun())
			return;

		int prev_hp = GetHP();

		amount = MIN(GetMaxHP() - GetHP(), amount);
		SetHP(GetHP() + amount);
		val = GetHP();

		BroadcastTargetPacket();
#ifdef ENABLE_HEALTH_BOARD_SYSTEM
		HealthBoardPacket();
#endif

		if (GetParty() && IsPC() && val != prev_hp)
			GetParty()->SendPartyInfoOneToAll(this);
	}
	break;

	case POINT_SP:
	{
		if (IsDead() || IsStun())
			return;

		amount = MIN(GetMaxSP() - GetSP(), amount);
		SetSP(GetSP() + amount);
		val = GetSP();
	}
	break;

	case POINT_STAMINA:
	{
		if (IsDead() || IsStun())
			return;

		int prev_val = GetStamina();
		amount = MIN(GetMaxStamina() - GetStamina(), amount);
		SetStamina(GetStamina() + amount);
		val = GetStamina();

		if (val == 0)
		{
			SetNowWalking(true);
		}
		else if (prev_val == 0)
		{
			ResetWalking();
		}

		if (amount < 0 && val != 0)
			return;
	}
	break;

	case POINT_MAX_HP:
	{
		SetPoint(type, GetPoint(type) + amount);
		//SetMaxHP(GetMaxHP() + amount);
		// 최대 생명력 = (기본 최대 생명력 + 추가) * 최대생명력%
		int hp = GetRealPoint(POINT_MAX_HP);
		int add_hp = MIN(3500, hp * GetPoint(POINT_MAX_HP_PCT) / 100);
		add_hp += GetPoint(POINT_MAX_HP);
		add_hp += GetPoint(POINT_PARTY_TANKER_BONUS);

		SetMaxHP(hp + add_hp);
		val = GetMaxHP();
	}
	break;

	case POINT_MAX_SP:
	{
		SetPoint(type, GetPoint(type) + amount);
		//SetMaxSP(GetMaxSP() + amount);
		// 최대 정신력 = (기본 최대 정신력 + 추가) * 최대정신력%
		int sp = GetRealPoint(POINT_MAX_SP);
		int add_sp = MIN(800, sp * GetPoint(POINT_MAX_SP_PCT) / 100);
		add_sp += GetPoint(POINT_MAX_SP);
		add_sp += GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);

		SetMaxSP(sp + add_sp);
		val = GetMaxSP();
	}
	break;

	case POINT_MAX_HP_PCT:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);

		PointChange(POINT_MAX_HP, 0);
		break;

	case POINT_MAX_SP_PCT:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);

		PointChange(POINT_MAX_SP, 0);
		break;

	case POINT_MAX_STAMINA:
		SetMaxStamina(GetMaxStamina() + amount);
		val = GetMaxStamina();
		break;

	case POINT_GOLD:
	{
		const int64_t nTotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(amount);

		if (GOLD_MAX <= nTotalMoney)
		{
			sys_err("[OVERFLOW_GOLD] OriGold %d AddedGold %d id %u Name %s ", GetGold(), amount, GetPlayerID(), GetName());
			LogManager::instance().CharLog(this, GetGold() + amount, "OVERFLOW_GOLD", "");
			return;
		}

		SetGold(GetGold() + amount);
		val = GetGold();
	}
	break;

#ifdef ENABLE_GEM_SYSTEM
	case POINT_GEM:
	{
		const int64_t nTotalGem = static_cast<int64_t>(GetGem()) + static_cast<int64_t>(amount);

		if (GEM_MAX <= nTotalGem)
		{
			LogManager::instance().CharLog(this, GetGem() + amount, "OVERFLOW_GEM", "");
			return;
		}

		SetGem(GetGem() + amount);
		val = GetGem();
	}
	break;
#endif

#ifdef ENABLE_CHEQUE_SYSTEM
	case POINT_CHEQUE:
	{
		const int16_t nTotalCheque = static_cast<int16_t>(GetCheque()) + static_cast<int16_t>(amount);

		if (CHEQUE_MAX <= nTotalCheque)
		{
			sys_err("[OVERFLOW_CHEQUE] OriCheque %d AddedCheque %d id %u Name %s ", GetCheque(), amount, GetPlayerID(), GetName());
			LogManager::instance().CharLog(this, GetCheque() + amount, "OVERFLOW_CHEQUE", "");
			return;
		}

		SetCheque(GetCheque() + amount);
		val = GetCheque();
	}
	break;
#endif

#ifdef ENABLE_EVENT_SYSTEM
	case POINT_YEAR_ROUND:
		SetYearRoundPoint(GetYearRoundPoint() + amount);
		val = GetYearRoundPoint();
		break;
#endif

	case POINT_SKILL:
	case POINT_STAT:
	case POINT_SUB_SKILL:
	case POINT_STAT_RESET_COUNT:
	case POINT_HORSE_SKILL:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		SetRealPoint(type, val);
		break;
	case POINT_DEF_GRADE:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		PointChange(POINT_CLIENT_DEF_GRADE, amount);
		break;
	case POINT_CLIENT_DEF_GRADE:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;
	case POINT_MOV_SPEED:
	{
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
		if (FindAffect(AFFECT_COMBAT_ZONE_MOVEMENT))
		{
			SetPoint(type, COMBAT_ZONE_REDUCTION_MAX_MOVEMENT_SPEED);
			val = GetPoint(type);
			break;
		}
#endif

		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
	}
	break;
	case POINT_ST:
	case POINT_HT:
	case POINT_DX:
	case POINT_IQ:
	case POINT_HP_REGEN:
	case POINT_SP_REGEN:
	case POINT_ATT_SPEED:
	case POINT_ATT_GRADE:
	case POINT_CASTING_SPEED:
	case POINT_MAGIC_ATT_GRADE:
	case POINT_MAGIC_DEF_GRADE:
	case POINT_BOW_DISTANCE:
	case POINT_HP_RECOVERY:
	case POINT_SP_RECOVERY:
	case POINT_ATTBONUS_HUMAN:
	case POINT_ATTBONUS_ANIMAL:
	case POINT_ATTBONUS_ORC:
	case POINT_ATTBONUS_MILGYO:
	case POINT_ATTBONUS_UNDEAD:
	case POINT_ATTBONUS_DEVIL:
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	case POINT_ATTBONUS_SWORD:
	case POINT_ATTBONUS_TWOHANDED:
	case POINT_ATTBONUS_DAGGER:
	case POINT_ATTBONUS_BELL:
	case POINT_ATTBONUS_FAN:
	case POINT_ATTBONUS_BOW:
#endif
	case POINT_ATTBONUS_MONSTER:
	case POINT_ATTBONUS_SURA:
	case POINT_ATTBONUS_ASSASSIN:
	case POINT_ATTBONUS_WARRIOR:
	case POINT_ATTBONUS_SHAMAN:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_ATTBONUS_WOLFMAN:
#endif
	case POINT_POISON_PCT:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_BLEEDING_PCT:
#endif
	case POINT_STUN_PCT:
	case POINT_SLOW_PCT:
	case POINT_BLOCK:
	case POINT_DODGE:
	case POINT_CRITICAL_PCT:
	case POINT_RESIST_CRITICAL:
	case POINT_PENETRATE_PCT:
	case POINT_RESIST_PENETRATE:
	case POINT_CURSE_PCT:
	case POINT_STEAL_HP:
	case POINT_STEAL_SP:
	case POINT_MANA_BURN_PCT:
	case POINT_DAMAGE_SP_RECOVER:
	case POINT_RESIST_NORMAL_DAMAGE:
	case POINT_RESIST_SWORD:
	case POINT_RESIST_TWOHAND:
	case POINT_RESIST_DAGGER:
	case POINT_RESIST_BELL:
	case POINT_RESIST_FAN:
	case POINT_RESIST_BOW:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_RESIST_CLAW:
#endif
	case POINT_RESIST_FIRE:
	case POINT_RESIST_ELEC:
	case POINT_RESIST_MAGIC:
	case POINT_RESIST_WIND:
	case POINT_RESIST_ICE:
	case POINT_RESIST_EARTH:
	case POINT_RESIST_DARK:
	case POINT_REFLECT_MELEE:
	case POINT_REFLECT_CURSE:
	case POINT_POISON_REDUCE:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_BLEEDING_REDUCE:
#endif
	case POINT_KILL_SP_RECOVER:
	case POINT_KILL_HP_RECOVERY:
	case POINT_HIT_HP_RECOVERY:
	case POINT_HIT_SP_RECOVERY:
	case POINT_MANASHIELD:
	case POINT_ATT_BONUS:
	case POINT_DEF_BONUS:
	case POINT_SKILL_DAMAGE_BONUS:
	case POINT_NORMAL_HIT_DAMAGE_BONUS:
	case POINT_SKILL_DEFEND_BONUS:
	case POINT_NORMAL_HIT_DEFEND_BONUS:
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	case POINT_ATTBONUS_ELEC:
	case POINT_ATTBONUS_FIRE:
	case POINT_ATTBONUS_ICE:
	case POINT_ATTBONUS_WIND:
	case POINT_ATTBONUS_EARTH:
	case POINT_ATTBONUS_DARK:
#endif
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	case POINT_ANTI_RESIST_MAGIC:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;
#endif
	case POINT_PARTY_ATTACKER_BONUS:
	case POINT_PARTY_TANKER_BONUS:
	case POINT_PARTY_BUFFER_BONUS:
	case POINT_PARTY_SKILL_MASTER_BONUS:
	case POINT_PARTY_HASTE_BONUS:
	case POINT_PARTY_DEFENDER_BONUS:
	case POINT_RESIST_WARRIOR:
	case POINT_RESIST_ASSASSIN:
	case POINT_RESIST_SURA:
	case POINT_RESIST_SHAMAN:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case POINT_RESIST_WOLFMAN:
#endif
	case POINT_RESIST_HUMAN:
	case POINT_ATTBONUS_STONE:
	case POINT_ATTBONUS_DESERT:
	case POINT_ATTBONUS_CZ:
#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
	case POINT_ATTBONUS_RAZADOR:
	case POINT_ATTBONUS_NEMERE:
	case POINT_ATTBONUS_HYDRA:
	case POINT_ATTBONUS_JOTUN_THRYM:
	case POINT_ATTBONUS_LUSIFER:
	case POINT_ATTBONUS_BERAN_SETAOU:
	case POINT_ATTBONUS_AZRAEL:
	case POINT_ATTBONUS_MELEY:
#endif
#ifdef ENABLE_MUSHROOM_DUNGEON
	case POINT_ATTBONUS_MUSHROOM:
	case POINT_RESIST_MUSHROOM:
#endif
#ifdef ENABLE_WATER_DUNGEON
	case POINT_ATTBONUS_WATER:
	case POINT_RESIST_WATER:
#endif
#ifdef ENABLE_BONUS_TO_BOSS
	case POINT_ATTBONUS_BOSS:
#endif
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;

	case POINT_MALL_ATTBONUS:
	case POINT_MALL_DEFBONUS:
	case POINT_MALL_EXPBONUS:
	case POINT_MALL_ITEMBONUS:
	case POINT_MALL_GOLDBONUS:
	case POINT_MELEE_MAGIC_ATT_BONUS_PER:
	case POINT_MAGIC_ATT_BONUS_PER:
		if (GetPoint(type) + amount > 500)
		{
			sys_err("MALL_BONUS exceeded over 500!! point type: %d name: %s amount %d", type, GetName(), amount);
			amount = 500 - GetPoint(type);
		} else {
			amount = (GetPoint(type) + amount);
		}

		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;

	case POINT_RAMADAN_CANDY_BONUS_EXP:
		SetPoint(type, amount);
		val = GetPoint(type);
		break;

	case POINT_EXP_DOUBLE_BONUS:
	case POINT_GOLD_DOUBLE_BONUS:
	case POINT_ITEM_DROP_BONUS:
	case POINT_POTION_BONUS:
		if (GetPoint(type) + amount > 500)
		{
			//sys_err("BONUS exceeded over 500!! point type: %d name: %s amount %d", type, GetName(), amount);
			amount = 500 - GetPoint(type);
		} else {
			amount = (GetPoint(type) + amount);
		}

		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		break;

	case POINT_IMMUNE_STUN:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		if (val)
		{
			SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
		}
		else
		{
			REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
		}
		break;

	case POINT_IMMUNE_SLOW:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		if (val)
		{
			SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
		}
		else
		{
			REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
		}
		break;

	case POINT_IMMUNE_FALL:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		if (val)
		{
			SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
		}
		else
		{
			REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
		}
		break;

	case POINT_ATT_GRADE_BONUS:
		SetPoint(type, GetPoint(type) + amount);
		PointChange(POINT_ATT_GRADE, amount);
		val = GetPoint(type);
		break;

	case POINT_DEF_GRADE_BONUS:
		SetPoint(type, GetPoint(type) + amount);
		PointChange(POINT_DEF_GRADE, amount);
		val = GetPoint(type);
		break;

	case POINT_MAGIC_ATT_GRADE_BONUS:
		SetPoint(type, GetPoint(type) + amount);
		PointChange(POINT_MAGIC_ATT_GRADE, amount);
		val = GetPoint(type);
		break;

	case POINT_MAGIC_DEF_GRADE_BONUS:
		SetPoint(type, GetPoint(type) + amount);
		PointChange(POINT_MAGIC_DEF_GRADE, amount);
		val = GetPoint(type);
		break;

	case POINT_VOICE:
	case POINT_EMPIRE_POINT:
		val = GetRealPoint(type);
		break;

	case POINT_POLYMORPH:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		SetPolymorph(val);
		break;

	case POINT_MOUNT:
		SetPoint(type, GetPoint(type) + amount);
		val = GetPoint(type);
		MountVnum(val);
		break;

	case POINT_ENERGY:
	case POINT_COSTUME_ATTR_BONUS:
	{
		int old_val = GetPoint(type);
		SetPoint(type, old_val + amount);
		val = GetPoint(type);
		BuffOnAttr_ValueChange(type, old_val, val);
	}
	break;

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	case POINT_ZODIAC:
	{
		const WORD nTotalZodiacPoint = static_cast<WORD>(GetZodiacPoint()) + static_cast<WORD>(amount);
		if (ZODIAC_POINT_MAX <= nTotalZodiacPoint)
		{
			sys_err("[OVERFLOW_ZODIAC_POINT] OriZodiacPoint %d AddedZodiacPoint %d id %u Name %s", GetZodiacPoint(), amount, GetPlayerID(), GetName());
			LogManager::instance().CharLog(this, GetZodiacPoint() + amount, "OVERFLOW_ZODIAC_POINT", "");
			return;
		}

		SetZodiacPoint(GetZodiacPoint() + amount);
		val = GetZodiacPoint();
	}
	break;
#endif

#ifdef ENABLE_WORSHIP_SYSTEM
	case POINT_WORSHIP:
	{
		const WORD nTotalWorshipPoint = static_cast<WORD>(GetWorshipPoint()) + static_cast<WORD>(amount);
		if (WORSHIP_POINT_MAX <= nTotalWorshipPoint)
		{
			sys_err("[OVERFLOW_WORSHIP_POINT] OriWorShipPoint %d AddedWorShipPoint %d id %u Name %s", GetWorshipPoint(), amount, GetPlayerID(), GetName());
			LogManager::instance().CharLog(this, GetWorshipPoint() + amount, "OVERFLOW_WORSHIP_POINT", "");
			return;
		}

		SetWorshipPoint(GetWorshipPoint() + amount);
		val = GetWorshipPoint();
	}
	break;
#endif

	default:
		sys_err("CHARACTER::PointChange: %s: unknown point change type %d", GetName(), type);
		return;
	}

	switch (type)
	{
	case POINT_LEVEL:
	case POINT_ST:
	case POINT_DX:
	case POINT_IQ:
	case POINT_HT:
		ComputeBattlePoints();
		break;

	case POINT_MAX_HP:
	case POINT_MAX_SP:
	case POINT_MAX_STAMINA:
		break;
	}

	if (type == POINT_HP && amount == 0)
		return;

	if (GetDesc())
	{
		struct packet_point_change pack;
		pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
		pack.dwVID = m_vid;
		pack.type = type;
		pack.value = val;

		if (bAmount)
			pack.amount = amount;
		else
			pack.amount = 0;

		if (!bBroadcast)
			GetDesc()->Packet(&pack, sizeof(struct packet_point_change));
		else
			PacketAround(&pack, sizeof(pack));
	}
}

#ifdef ENABLE_GROWTH_PET_SYSTEM
void CHARACTER::SendPetLevelUpEffect(int vid, int type, int value, int amount) {
	struct packet_point_change pack;

	pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
	pack.dwVID = vid;
	pack.type = type;
	pack.value = value;
	pack.amount = amount;
	PacketAround(&pack, sizeof(pack));
}
#endif

void CHARACTER::ApplyPoint(BYTE bApplyType, int iVal)
{
	switch (bApplyType)
	{
	case APPLY_NONE:
		break;

	case APPLY_CON:
		PointChange(POINT_HT, iVal);
		PointChange(POINT_MAX_HP, (iVal * JobInitialPoints[GetJob()].hp_per_ht));
		PointChange(POINT_MAX_STAMINA, (iVal * JobInitialPoints[GetJob()].stamina_per_con));
		break;

	case APPLY_INT:
		PointChange(POINT_IQ, iVal);
		PointChange(POINT_MAX_SP, (iVal * JobInitialPoints[GetJob()].sp_per_iq));
		break;

	case APPLY_SKILL:
	{
		BYTE bSkillVnum = (BYTE)(((DWORD)iVal) >> 24);
		int iAdd = iVal & 0x00800000;
		int iChange = iVal & 0x007fffff;

		sys_log(1, "APPLY_SKILL skill %d add? %d change %d", bSkillVnum, iAdd ? 1 : 0, iChange);
		if (0 == iAdd)
			iChange = -iChange;

		std::unordered_map<BYTE, int>::iterator iter = m_SkillDamageBonus.find(bSkillVnum);
		if (iter == m_SkillDamageBonus.end())
			m_SkillDamageBonus.insert(std::make_pair(bSkillVnum, iChange));
		else
			iter->second += iChange;
	}
	// END_OF_SKILL_DAMAGE_BONUS
	break;

	// NOTE: 아이템에 의한 최대HP 보너스나 퀘스트 보상 보너스가 똑같은 방식을 사용하므로
	// 그냥 MAX_HP만 계산하면 퀘스트 보상의 경우 문제가 생김. 사실 원래 이쪽이 합리적이기도 하고..
	// 바꾼 공식은 현재 최대 hp와 보유 hp의 비율을 구한 뒤 바뀔 최대 hp를 기준으로 hp를 보정한다.
	// 원래 PointChange에서 하는게 좋을것 같은데 설계 문제로 어려워서 skip..
	// SP도 똑같이 계산한다.
	// Mantis : 101460			~ ity ~
	case APPLY_MAX_HP:
	case APPLY_MAX_HP_PCT:
	{
		int i = GetMaxHP(); if (i == 0) break;
		PointChange(aApplyInfo[bApplyType].bPointType, iVal);
		float fRatio = (float)GetMaxHP() / (float)i;
		PointChange(POINT_HP, GetHP() * fRatio - GetHP());
	}
	break;

	case APPLY_MAX_SP:
	case APPLY_MAX_SP_PCT:
	{
		int i = GetMaxSP(); if (i == 0) break;
		PointChange(aApplyInfo[bApplyType].bPointType, iVal);
		float fRatio = (float)GetMaxSP() / (float)i;
		PointChange(POINT_SP, GetSP() * fRatio - GetSP());
	}
	break;

	case APPLY_STR:
	case APPLY_DEX:
	case APPLY_ATT_SPEED:
	case APPLY_MOV_SPEED:
	case APPLY_CAST_SPEED:
	case APPLY_HP_REGEN:
	case APPLY_SP_REGEN:
	case APPLY_POISON_PCT:
	case APPLY_BLEEDING_PCT:
	case APPLY_STUN_PCT:
	case APPLY_SLOW_PCT:
	case APPLY_CRITICAL_PCT:
	case APPLY_PENETRATE_PCT:
	case APPLY_ATTBONUS_HUMAN:
	case APPLY_ATTBONUS_ANIMAL:
	case APPLY_ATTBONUS_ORC:
	case APPLY_ATTBONUS_MILGYO:
	case APPLY_ATTBONUS_UNDEAD:
	case APPLY_ATTBONUS_DEVIL:
	case APPLY_ATTBONUS_WARRIOR:
	case APPLY_ATTBONUS_ASSASSIN:
	case APPLY_ATTBONUS_SURA:
	case APPLY_ATTBONUS_SHAMAN:
	case APPLY_ATTBONUS_MONSTER:
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	case APPLY_ATTBONUS_SWORD:
	case APPLY_ATTBONUS_TWOHANDED:
	case APPLY_ATTBONUS_DAGGER:
	case APPLY_ATTBONUS_BELL:
	case APPLY_ATTBONUS_FAN:
	case APPLY_ATTBONUS_BOW:
#endif
	case APPLY_STEAL_HP:
	case APPLY_STEAL_SP:
	case APPLY_MANA_BURN_PCT:
	case APPLY_DAMAGE_SP_RECOVER:
	case APPLY_BLOCK:
	case APPLY_DODGE:
	case APPLY_RESIST_SWORD:
	case APPLY_RESIST_TWOHAND:
	case APPLY_RESIST_DAGGER:
	case APPLY_RESIST_BELL:
	case APPLY_RESIST_FAN:
	case APPLY_RESIST_BOW:
	case APPLY_RESIST_FIRE:
	case APPLY_RESIST_ELEC:
	case APPLY_RESIST_MAGIC:
	case APPLY_RESIST_WIND:
	case APPLY_RESIST_ICE:
	case APPLY_RESIST_EARTH:
	case APPLY_RESIST_DARK:
	case APPLY_REFLECT_MELEE:
	case APPLY_REFLECT_CURSE:
	case APPLY_ANTI_CRITICAL_PCT:
	case APPLY_ANTI_PENETRATE_PCT:
	case APPLY_POISON_REDUCE:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case APPLY_BLEEDING_REDUCE:
#endif
	case APPLY_KILL_SP_RECOVER:
	case APPLY_EXP_DOUBLE_BONUS:
	case APPLY_GOLD_DOUBLE_BONUS:
	case APPLY_ITEM_DROP_BONUS:
	case APPLY_POTION_BONUS:
	case APPLY_KILL_HP_RECOVER:
	case APPLY_IMMUNE_STUN:
	case APPLY_IMMUNE_SLOW:
	case APPLY_IMMUNE_FALL:
	case APPLY_BOW_DISTANCE:
	case APPLY_ATT_GRADE_BONUS:
	case APPLY_DEF_GRADE_BONUS:
	case APPLY_MAGIC_ATT_GRADE:
	case APPLY_MAGIC_DEF_GRADE:
	case APPLY_CURSE_PCT:
	case APPLY_MAX_STAMINA:
	case APPLY_MALL_ATTBONUS:
	case APPLY_MALL_DEFBONUS:
	case APPLY_MALL_EXPBONUS:
	case APPLY_MALL_ITEMBONUS:
	case APPLY_MALL_GOLDBONUS:
	case APPLY_SKILL_DAMAGE_BONUS:
	case APPLY_NORMAL_HIT_DAMAGE_BONUS:
	case APPLY_SKILL_DEFEND_BONUS:
	case APPLY_NORMAL_HIT_DEFEND_BONUS:
	case APPLY_RESIST_WARRIOR:
	case APPLY_RESIST_ASSASSIN:
	case APPLY_RESIST_SURA:
	case APPLY_RESIST_SHAMAN:
	case APPLY_ENERGY:
	case APPLY_DEF_GRADE:
	case APPLY_COSTUME_ATTR_BONUS:
	case APPLY_MAGIC_ATTBONUS_PER:
	case APPLY_MELEE_MAGIC_ATTBONUS_PER:
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	case APPLY_ATTBONUS_ELEC:
	case APPLY_ATTBONUS_FIRE:
	case APPLY_ATTBONUS_ICE:
	case APPLY_ATTBONUS_WIND:
	case APPLY_ATTBONUS_EARTH:
	case APPLY_ATTBONUS_DARK:
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	case APPLY_ANTI_RESIST_MAGIC:
		PointChange(aApplyInfo[bApplyType].bPointType, iVal);
		break;
#endif
#ifdef ENABLE_WOLFMAN_CHARACTER
	case APPLY_ATTBONUS_WOLFMAN:
	case APPLY_RESIST_WOLFMAN:
	case APPLY_RESIST_CLAW:
#endif
	case APPLY_RESIST_HUMAN:
	case APPLY_ATTBONUS_STONE:
	case APPLY_ATTBONUS_DESERT:
	case APPLY_ATTBONUS_CZ:
#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
	case APPLY_ATTBONUS_RAZADOR:
	case APPLY_ATTBONUS_NEMERE:
	case APPLY_ATTBONUS_HYDRA:
	case APPLY_ATTBONUS_JOTUN_THRYM:
	case APPLY_ATTBONUS_LUSIFER:
	case APPLY_ATTBONUS_BERAN_SETAOU:
	case APPLY_ATTBONUS_AZRAEL:
	case APPLY_ATTBONUS_MELEY:
#endif
#ifdef ENABLE_MUSHROOM_DUNGEON
	case APPLY_ATTBONUS_MUSHROOM:
	case APPLY_RESIST_MUSHROOM:
#endif
#ifdef ENABLE_WATER_DUNGEON
	case APPLY_ATTBONUS_WATER:
	case APPLY_RESIST_WATER:
#endif
#ifdef ENABLE_BONUS_TO_BOSS
	case APPLY_ATTBONUS_BOSS:
#endif
		PointChange(aApplyInfo[bApplyType].bPointType, iVal);
		break;

	default:
		sys_err("Unknown apply type %d name %s", bApplyType, GetName());
		break;
	}
}

void CHARACTER::MotionPacketEncode(BYTE motion, LPCHARACTER victim, struct packet_motion* packet)
{
	packet->header = HEADER_GC_MOTION;
	packet->vid = m_vid;
	packet->motion = motion;

	if (victim)
		packet->victim_vid = victim->GetVID();
	else
		packet->victim_vid = 0;
}

void CHARACTER::Motion(BYTE motion, LPCHARACTER victim)
{
	struct packet_motion pack_motion;
	MotionPacketEncode(motion, victim, &pack_motion);
	PacketAround(&pack_motion, sizeof(struct packet_motion));
}

EVENTFUNC(save_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("save_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}
	sys_log(1, "SAVE_EVENT: %s", ch->GetName());
	ch->Save();
	ch->FlushDelayedSaveItem();
	return (save_event_second_cycle);
}

void CHARACTER::StartSaveEvent()
{
	if (m_pkSaveEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;
	m_pkSaveEvent = event_create(save_event, info, save_event_second_cycle);
}

void CHARACTER::MonsterLog(const char* format, ...)
{
	if (!test_server)
		return;

	if (IsPC())
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	int len = snprintf(chatbuf, sizeof(chatbuf), "(%u)", (DWORD)GetVID());

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	va_list args;

	va_start(args, format);

	int len2 = vsnprintf(chatbuf + len, sizeof(chatbuf) - len, format, args);

	if (len2 < 0 || len2 >= (int) sizeof(chatbuf) - len)
		len += (sizeof(chatbuf) - len) - 1;
	else
		len += len2;

	// \0 문자 포함
	++len;

	va_end(args);

	TPacketGCChat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(TPacketGCChat) + len;
	pack_chat.type = CHAT_TYPE_TALKING;
	pack_chat.id = (DWORD)GetVID();
	pack_chat.bEmpire = 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(TPacketGCChat));
	buf.write(chatbuf, len);

	CHARACTER_MANAGER::instance().PacketMonsterLog(this, buf.read_peek(), buf.size());
}

void CHARACTER::ChatPacket(BYTE type, const char* format, ...)
{
	LPDESC d = GetDesc();

	if (!d || !format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	int len = vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	struct packet_chat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(struct packet_chat) + len;
	pack_chat.type = type;
	pack_chat.id = 0;
	pack_chat.bEmpire = d->GetEmpire();

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(chatbuf, len);

	d->Packet(buf.read_peek(), buf.size());

	if (type == CHAT_TYPE_COMMAND && test_server)
		sys_log(0, "SEND_COMMAND %s %s", GetName(), chatbuf);
}

// MINING
void CHARACTER::mining_take()
{
	m_pkMiningEvent = nullptr;
}

void CHARACTER::mining_cancel()
{
	if (m_pkMiningEvent)
	{
		sys_log(0, "XXX MINING CANCEL");
		event_cancel(&m_pkMiningEvent);
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("채광을 중단하였습니다."));
	}
}

void CHARACTER::mining(LPCHARACTER chLoad)
{
	if (m_pkMiningEvent)
	{
		mining_cancel();
		return;
	}

	if (!chLoad)
		return;

	if (GetMapIndex() != chLoad->GetMapIndex() || DISTANCE_APPROX(GetX() - chLoad->GetX(), GetY() - chLoad->GetY()) > 1000)  //Madenci bot fixed.
		return;

	if (mining::GetRawOreFromLoad(chLoad->GetRaceNum()) == 0)
		return;

	LPITEM pick = GetWear(WEAR_WEAPON);

	if (!pick || pick->GetType() != ITEM_PICK)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("곡괭이를 장착하세요."));
		return;
	}
	
#ifdef ENABLE_CAPTCHA_SYSTEM
	#ifdef ENABLE_AUTO_HUNTING_SYSTEM
	if (nullptr == FindAffect(AFFECT_AUTO_HUNT, APPLY_NONE))
	#endif
	{
		if(get_global_time() > int(GetQuestFlag("captcha.sure")))
		{       
			int sans;
			if (GetLevel() >= 75 && GetLevel() < 90)
			{
				sans = number(3600, 3600);
			}
			else
			{
				sans = number(3600, 3600);
			}
			if(int(GetQuestFlag("captcha.durum")) == 0)
			{
				SetQuestFlag("captcha.durum", 1);
				SetQuestFlag("captcha.sure", get_global_time() + (60*sans));
				int sayi1 = number(1,9);
				int sayi2 = number(1,9);
				int sayi3 = number(1,9);
				int sayi4 = number(1,9);
				int sayi5 = number(1,9);
				long yenicaptcha = (sayi1*10000)+(sayi2*1000)+(sayi3*100)+(sayi4*10)+sayi5;
				ChatPacket(CHAT_TYPE_COMMAND, "captcha %d %d %d %d %d", sayi1, sayi2, sayi3, sayi4, sayi5);
				SetCaptcha(yenicaptcha);
				return;
			}
		}
		if(int(GetQuestFlag("captcha.durum")) == 1)
		{
			return;
		}
	}
#endif

	int count = number(5, 15); // 동작 횟수, 한 동작당 2초

	// 채광 동작을 보여줌
	TPacketGCDigMotion p;
	p.header = HEADER_GC_DIG_MOTION;
	p.vid = GetVID();
	p.target_vid = chLoad->GetVID();
	p.count = count;

	PacketAround(&p, sizeof(p));

	m_pkMiningEvent = mining::CreateMiningEvent(this, chLoad, count);
}

#ifdef ENABLE_ANTINEARWATER_FIX
bool CHARACTER::IS_VALID_FISHING_POSITION(int32_t *returnPosx, int32_t *returnPosy) const
{
	int32_t charX = GetX();
	int32_t charY = GetY();
	

	LPSECTREE curWaterPostitionTree;

	int32_t fX, fY;
	for (float fRot = 0.0f; fRot <= 180.0f; fRot += 10.0f) //mimics behaviour of client.
	{
		ELPlainCoord_GetRotatedPixelPosition(charX, charY, 600.0f, GetRotation() + fRot, &fX, &fY);
		curWaterPostitionTree = SECTREE_MANAGER::instance().Get(GetMapIndex(), fX, fY);
		if (curWaterPostitionTree && curWaterPostitionTree->IsAttr(fX, fY, ATTR_WATER))	{
			*returnPosx = fX;
			*returnPosy = fY;
			return true;
		}
		//No idea if thats needed client uses it.
		ELPlainCoord_GetRotatedPixelPosition(charX, charY, 600.0f, GetRotation() - fRot, &fX, &fY);
		curWaterPostitionTree = SECTREE_MANAGER::instance().Get(GetMapIndex(), fX, fY);
		if (curWaterPostitionTree && curWaterPostitionTree->IsAttr(fX, fY, ATTR_WATER))	{
			*returnPosx = fX;
			*returnPosy = fY;
			return true;
		}
	}

	return false;
}
#endif

void CHARACTER::fishing()
{
	if (m_pkFishingEvent)
	{
		fishing_take();
		return;
	}
	
#ifdef ENABLE_CAPTCHA_SYSTEM
	if(get_global_time() > int(GetQuestFlag("captcha.sure")))
	{       
		int sans;
		if (GetLevel() >= 75 && GetLevel() < 90)
		{
			sans = number(3600, 3600);
		}
		else
		{
			sans = number(3600, 3600);
		}
		if(int(GetQuestFlag("captcha.durum")) == 0)
		{
			SetQuestFlag("captcha.durum", 1);
			SetQuestFlag("captcha.sure", get_global_time() + (60*sans));
			int sayi1 = number(1,9);
			int sayi2 = number(1,9);
			int sayi3 = number(1,9);
			int sayi4 = number(1,9);
			int sayi5 = number(1,9);
			long yenicaptcha = (sayi1*10000)+(sayi2*1000)+(sayi3*100)+(sayi4*10)+sayi5;
			ChatPacket(CHAT_TYPE_COMMAND, "captcha %d %d %d %d %d", sayi1, sayi2, sayi3, sayi4, sayi5);
			SetCaptcha(yenicaptcha);
			return;
		}
	}
	if(int(GetQuestFlag("captcha.durum")) == 1)
	{
		return;
	}
#endif

	LPITEM rod = GetWear(WEAR_WEAPON);

	// 낚시대 장착
	if (!rod || rod->GetType() != ITEM_ROD)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시대를 장착 하세요."));
		return;
	}

	if (0 == rod->GetSocket(2))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("미끼를 끼고 던져 주세요."));
		return;
	}

	{
		LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

		int32_t	x = GetX();
		int32_t y = GetY();

		LPSECTREE playerTree = pkSectreeMap->Find(x, y);
		if (IS_SET(playerTree->GetAttribute(x, y), ATTR_BLOCK))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_CANNOT_DO_FISHING_HERE"));
			return;
		}

#ifdef ENABLE_ANTINEARWATER_FIX
		int32_t newPosx, newPosy;
		if (!IS_VALID_FISHING_POSITION(&newPosx, &newPosy))	{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("I cannot go fishing here."));
			LogManager::instance().HackLog("FISH_BOT_LOCATION", this);
			return;
		}

		SetRotationToXY(newPosx, newPosy);
	}
#endif

	m_pkFishingEvent = fishing::CreateFishingEvent(this);
}

void CHARACTER::fishing_take()
{
	LPITEM rod = GetWear(WEAR_WEAPON);
	if (rod && rod->GetType() == ITEM_ROD)
	{
		using fishing::fishing_event_info;
		if (m_pkFishingEvent)
		{
			struct fishing_event_info* info = dynamic_cast<struct fishing_event_info*>(m_pkFishingEvent->info);

			if (info)
				fishing::Take(info, this);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시대가 아닌 물건으로 낚시를 할 수 없습니다!"));
	}

	event_cancel(&m_pkFishingEvent);
}

bool CHARACTER::StartStateMachine(int iNextPulse)
{
	if (CHARACTER_MANAGER::instance().AddToStateList(this))
	{
		m_dwNextStatePulse = thecore_heart->pulse + iNextPulse;
		return true;
	}

	return false;
}

void CHARACTER::StopStateMachine()
{
	CHARACTER_MANAGER::instance().RemoveFromStateList(this);
}

void CHARACTER::UpdateStateMachine(DWORD dwPulse)
{
	if (dwPulse < m_dwNextStatePulse)
		return;

	if (IsDead())
		return;

	Update();
	m_dwNextStatePulse = dwPulse + m_dwStateDuration;
}

void CHARACTER::SetNextStatePulse(int iNextPulse)
{
	CHARACTER_MANAGER::instance().AddToStateList(this);
	m_dwNextStatePulse = iNextPulse;

	if (iNextPulse < 10)
		MonsterLog("Let's go to the next state");
}

// 캐릭터 인스턴스 업데이트 함수.
void CHARACTER::UpdateCharacter(DWORD dwPulse)
{
	CFSM::Update();
}

void CHARACTER::SetShop(LPSHOP pkShop)
{
	if ((m_pkShop = pkShop))
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
	else
	{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
		SetShopOwner(nullptr);
	}
}

void CHARACTER::SetExchange(CExchange * pkExchange)
{
	m_pkExchange = pkExchange;
}

void CHARACTER::SetPart(BYTE bPartPos, WORD wVal)
{
	assert(bPartPos < PART_MAX_NUM);
	m_pointsInstant.parts[bPartPos] = wVal;
}

WORD CHARACTER::GetPart(BYTE bPartPos) const
{
	assert(bPartPos < PART_MAX_NUM);
	return m_pointsInstant.parts[bPartPos];
}

WORD CHARACTER::GetOriginalPart(BYTE bPartPos) const
{
	switch (bPartPos)
	{
	case PART_MAIN:
		if (!IsPC()) // PC가 아닌 경우 현재 파트를 그대로 리턴
			return GetPart(PART_MAIN);
		else
			return m_pointsInstant.bBasePart;

	case PART_HAIR:
		return GetPart(PART_HAIR);
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	case PART_WEAPON:
		return GetPart(PART_WEAPON);
#endif
#ifdef ENABLE_ACCE_SYSTEM
	case PART_SASH:
		return GetPart(PART_SASH);
#endif
#ifdef ENABLE_AURA_SYSTEM
	case PART_AURA:
		return GetPart(PART_AURA);
#endif
	default:
		return 0;
	}
}

BYTE CHARACTER::GetCharType() const
{
	return m_bCharType;
}

bool CHARACTER::SetSyncOwner(LPCHARACTER ch, bool bRemoveFromList)
{
	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
		return false;
	// END_OF_TRENT_MONSTER

	if (ch) // @fixme131
	{
		if (!battle_is_attackable(ch, this))
		{
			SendDamagePacket(ch, 0, DAMAGE_BLOCK);
			return false;
		}
	}

	if (ch == this)
	{
		sys_err("SetSyncOwner owner == this (%p)", this);
		return false;
	}

	if (!ch)
	{
		if (bRemoveFromList && m_pkChrSyncOwner)
		{
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
		}

		if (m_pkChrSyncOwner)
			sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());

		// 리스트에서 제거하지 않더라도 포인터는 nullptr로 셋팅되어야 한다.
		m_pkChrSyncOwner = nullptr;
	}
	else
	{
		if (!IsSyncOwner(ch))
			return false;

		// 거리가 200 이상이면 SyncOwner가 될 수 없다.
		if (DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY()) > 250)
		{
			sys_log(1, "SetSyncOwner distance over than 250 %s %s", GetName(), ch->GetName());

			// SyncOwner일 경우 Owner로 표시한다.
			if (m_pkChrSyncOwner == ch)
				return true;

			return false;
		}

		if (m_pkChrSyncOwner != ch)
		{
			if (m_pkChrSyncOwner)
			{
				sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());
				m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
			}

			m_pkChrSyncOwner = ch;
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.push_back(this);

			// SyncOwner가 바뀌면 LastSyncTime을 초기화한다.
			static const timeval zero_tv = { 0, 0 };
			SetLastSyncTime(zero_tv);

			sys_log(1, "SetSyncOwner set %s %p to %s", GetName(), this, ch->GetName());
		}

		m_fSyncTime = get_float_time();
	}

	// TODO: Sync Owner가 같더라도 계속 패킷을 보내고 있으므로,
	//       동기화 된 시간이 3초 이상 지났을 때 풀어주는 패킷을
	//       보내는 방식으로 하면 패킷을 줄일 수 있다.
	TPacketGCOwnership pack;
	if (!ch) return false;

	pack.bHeader = HEADER_GC_OWNERSHIP;
	pack.dwOwnerVID = ch ? ch->GetVID() : 0;
	pack.dwVictimVID = GetVID();

	PacketAround(&pack, sizeof(TPacketGCOwnership));
	return true;
}

struct FuncClearSync
{
	void operator () (LPCHARACTER ch)
	{
		assert(ch != nullptr);
		ch->SetSyncOwner(nullptr, false);	// false 플래그로 해야 for_each 가 제대로 돈다.
	}
};

void CHARACTER::ClearSync()
{
	SetSyncOwner(nullptr);

	// 아래 for_each에서 나를 m_pkChrSyncOwner로 가진 자들의 포인터를 nullptr로 한다.
	std::for_each(m_kLst_pkChrSyncOwned.begin(), m_kLst_pkChrSyncOwned.end(), FuncClearSync());
	m_kLst_pkChrSyncOwned.clear();
}

bool CHARACTER::IsSyncOwner(LPCHARACTER ch) const
{
	if (m_pkChrSyncOwner == ch)
		return true;

	// 마지막으로 동기화 된 시간이 3초 이상 지났다면 소유권이 아무에게도
	// 없다. 따라서 아무나 SyncOwner이므로 true 리턴
	if (get_float_time() - m_fSyncTime >= 3.0f)
		return true;

	return false;
}

void CHARACTER::SetParty(LPPARTY pkParty)
{
	if (pkParty == m_pkParty)
		return;

	if (pkParty && m_pkParty)
		sys_err("%s is trying to reassigning party (current %p, new party %p)", GetName(), get_pointer(m_pkParty), get_pointer(pkParty));

	sys_log(1, "PARTY set to %p", get_pointer(pkParty));

	if (m_pkDungeon && IsPC() && !pkParty)
		SetDungeon(nullptr);
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (m_pkZodiac && IsPC() && !pkParty)
		SetZodiac(nullptr);
#endif

	m_pkParty = pkParty;

	if (IsPC())
	{
		if (m_pkParty)
			SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);
		else
			REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);

		UpdatePacket();
	}
}

// PARTY_JOIN_BUG_FIX
/// 파티 가입 event 정보
EVENTINFO(TPartyJoinEventInfo)
{
	DWORD	dwGuestPID;		///< 파티에 참여할 캐릭터의 PID
	DWORD	dwLeaderPID;		///< 파티 리더의 PID

	TPartyJoinEventInfo() : dwGuestPID(0), dwLeaderPID(0)
	{
	}
};

EVENTFUNC(party_request_event)
{
	TPartyJoinEventInfo* info = dynamic_cast<TPartyJoinEventInfo*>(event->info);

	if (info == nullptr)
	{
		sys_err("party_request_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->dwGuestPID);

	if (ch)
	{
		sys_log(0, "PartyRequestEvent %s", ch->GetName());
		ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
		ch->SetPartyRequestEvent(nullptr);
	}

	return 0;
}

bool CHARACTER::RequestToParty(LPCHARACTER leader)
{
	if (leader->GetParty())
		leader = leader->GetParty()->GetLeaderCharacter();

	if (!leader)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("파티장이 접속 상태가 아니라서 요청을 할 수 없습니다."));
		return false;
	}

	if (m_pkPartyRequestEvent)
		return false;

	if (!IsPC() || !leader->IsPC())
		return false;

	if (leader->IsBlockMode(BLOCK_PARTY_REQUEST))
		return false;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return false;
#endif

	PartyJoinErrCode errcode = IsPartyJoinableCondition(leader, this);

	switch (errcode)
	{
	case PERR_NONE:
		break;

	case PERR_SERVER:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 서버 문제로 파티 관련 처리를 할 수 없습니다."));
		return false;

	case PERR_DIFFEMPIRE:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 다른 제국과 파티를 이룰 수 없습니다."));
		return false;

	case PERR_DUNGEON:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 던전 안에서는 파티 초대를 할 수 없습니다."));
		return false;

	case PERR_OBSERVER:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 관전 모드에선 파티 초대를 할 수 없습니다."));
		return false;

	case PERR_LVBOUNDARY:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> -30 ~ +30 레벨 이내의 상대방만 초대할 수 있습니다."));
		return false;

	case PERR_LOWLEVEL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최고 레벨 보다 30레벨이 낮아 초대할 수 없습니다."));
		return false;

	case PERR_HILEVEL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최저 레벨 보다 30레벨이 높아 초대할 수 없습니다."));
		return false;

	case PERR_ALREADYJOIN:
		return false;

	case PERR_PARTYISFULL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 더 이상 파티원을 초대할 수 없습니다."));
		return false;

	default:
		sys_err("Do not process party join error(%d)", errcode);
		return false;
	}

	TPartyJoinEventInfo* info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = GetPlayerID();
	info->dwLeaderPID = leader->GetPlayerID();

	SetPartyRequestEvent(event_create(party_request_event, info, PASSES_PER_SEC(10)));

	leader->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequest %u", (DWORD)GetVID());
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 님에게 파티가입 신청을 했습니다."), leader->GetName());
	return true;
}

void CHARACTER::DenyToParty(LPCHARACTER member)
{
	sys_log(1, "DenyToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo* info = dynamic_cast<TPartyJoinEventInfo*>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err("CHARACTER::DenyToParty> <Factor> nullptr pointer");
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

void CHARACTER::AcceptToParty(LPCHARACTER member)
{
	sys_log(1, "AcceptToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo* info = dynamic_cast<TPartyJoinEventInfo*>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err("CHARACTER::AcceptToParty> <Factor> nullptr pointer");
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	if (!GetParty())
		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 파티에 속해있지 않습니다."));
	else
	{
		if (GetPlayerID() != GetParty()->GetLeaderPID())
			return;

		PartyJoinErrCode errcode = IsPartyJoinableCondition(this, member);
		switch (errcode)
		{
		case PERR_NONE: 		member->PartyJoin(this); return;
		case PERR_SERVER:		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 서버 문제로 파티 관련 처리를 할 수 없습니다.")); break;
		case PERR_DUNGEON:		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 던전 안에서는 파티 초대를 할 수 없습니다.")); break;
		case PERR_OBSERVER: 	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 관전 모드에선 파티 초대를 할 수 없습니다.")); break;
		case PERR_LVBOUNDARY:	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> -30 ~ +30 레벨 이내의 상대방만 초대할 수 있습니다.")); break;
		case PERR_LOWLEVEL: 	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최고 레벨 보다 30레벨이 낮아 초대할 수 없습니다.")); break;
		case PERR_HILEVEL: 		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최저 레벨 보다 30레벨이 높아 초대할 수 없습니다.")); break;
		case PERR_ALREADYJOIN: 	break;
		case PERR_PARTYISFULL: {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 더 이상 파티원을 초대할 수 없습니다."));
			member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티의 인원제한이 초과하여 파티에 참가할 수 없습니다."));
			break;
		}
		default: sys_err("Do not process party join error(%d)", errcode);
		}
	}

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

/**
 * 파티 초대 event callback 함수.
 * event 가 발동하면 초대 거절로 처리한다.
 */
EVENTFUNC(party_invite_event)
{
	TPartyJoinEventInfo* pInfo = dynamic_cast<TPartyJoinEventInfo*>(event->info);

	if (pInfo == nullptr)
	{
		sys_err("party_invite_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER pchInviter = CHARACTER_MANAGER::instance().FindByPID(pInfo->dwLeaderPID);

	if (pchInviter)
	{
		sys_log(1, "PartyInviteEvent %s", pchInviter->GetName());
		pchInviter->PartyInviteDeny(pInfo->dwGuestPID);
	}

	return 0;
}

void CHARACTER::PartyInvite(LPCHARACTER pchInvitee)
{
	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티원을 초대할 수 있는 권한이 없습니다."));
		return;
	}
	else if (pchInvitee->IsBlockMode(BLOCK_PARTY_INVITE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> %s 님이 파티 거부 상태입니다."), pchInvitee->GetName());
		return;
	}
	PartyJoinErrCode errcode = IsPartyJoinableCondition(this, pchInvitee);

	switch (errcode)
	{
	case PERR_NONE:
		break;

	case PERR_SERVER:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 서버 문제로 파티 관련 처리를 할 수 없습니다."));
		return;

	case PERR_DIFFEMPIRE:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 다른 제국과 파티를 이룰 수 없습니다."));
		return;

	case PERR_DUNGEON:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 던전 안에서는 파티 초대를 할 수 없습니다."));
		return;

	case PERR_OBSERVER:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 관전 모드에선 파티 초대를 할 수 없습니다."));
		return;

	case PERR_LVBOUNDARY:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> -30 ~ +30 레벨 이내의 상대방만 초대할 수 있습니다."));
		return;

	case PERR_LOWLEVEL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최고 레벨 보다 30레벨이 낮아 초대할 수 없습니다."));
		return;

	case PERR_HILEVEL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최저 레벨 보다 30레벨이 높아 초대할 수 없습니다."));
		return;

	case PERR_ALREADYJOIN:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 이미 %s님은 파티에 속해 있습니다."), pchInvitee->GetName());
		return;

	case PERR_PARTYISFULL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 더 이상 파티원을 초대할 수 없습니다."));
		return;

	default:
		sys_err("Do not process party join error(%d)", errcode);
		return;
	}

	if (m_PartyInviteEventMap.end() != m_PartyInviteEventMap.find(pchInvitee->GetPlayerID()))
		return;

	//
	// EventMap 에 이벤트 추가
	//
	TPartyJoinEventInfo * info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = pchInvitee->GetPlayerID();
	info->dwLeaderPID = GetPlayerID();

	m_PartyInviteEventMap.insert(EventMap::value_type(pchInvitee->GetPlayerID(), event_create(party_invite_event, info, PASSES_PER_SEC(10))));

	//
	// 초대 받는 character 에게 초대 패킷 전송
	//

	TPacketGCPartyInvite p;
	p.header = HEADER_GC_PARTY_INVITE;
	p.leader_vid = GetVID();
	pchInvitee->GetDesc()->Packet(&p, sizeof(p));
}

void CHARACTER::PartyInviteAccept(LPCHARACTER pchInvitee)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(pchInvitee->GetPlayerID());

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteAccept from not invited character(%s)", pchInvitee->GetName());
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티원을 초대할 수 있는 권한이 없습니다."));
		return;
	}

	PartyJoinErrCode errcode = IsPartyJoinableMutableCondition(this, pchInvitee);

	switch (errcode)
	{
	case PERR_NONE:
		break;

	case PERR_SERVER:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 서버 문제로 파티 관련 처리를 할 수 없습니다."));
		return;

	case PERR_DUNGEON:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 던전 안에서는 파티 초대에 응할 수 없습니다."));
		return;

	case PERR_OBSERVER:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 관전 모드에선 파티 초대를 할 수 없습니다."));
		return;

	case PERR_LVBOUNDARY:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> -30 ~ +30 레벨 이내의 상대방만 초대할 수 있습니다."));
		return;

	case PERR_LOWLEVEL:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최고 레벨 보다 30레벨이 낮아 초대할 수 없습니다."));
		return;

	case PERR_HILEVEL:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티내 최저 레벨 보다 30레벨이 높아 초대할 수 없습니다."));
		return;

	case PERR_ALREADYJOIN:
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티 초대에 응할 수 없습니다."));
		return;

	case PERR_PARTYISFULL:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 더 이상 파티원을 초대할 수 없습니다."));
		pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티의 인원제한이 초과하여 파티에 참가할 수 없습니다."));
		return;

	default:
		sys_err("ignore party join error(%d)", errcode);
		return;
	}

	//
	// 파티 가입 처리
	//

	if (GetParty())
		pchInvitee->PartyJoin(this);
	else
	{
		LPPARTY pParty = CPartyManager::instance().CreateParty(this);

		pParty->Join(pchInvitee->GetPlayerID());
		pParty->Link(pchInvitee);
		pParty->SendPartyInfoAllToOne(this);
	}
}

void CHARACTER::PartyInviteDeny(DWORD dwPID)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(dwPID);

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteDeny to not exist event(inviter PID: %d, invitee PID: %d)", GetPlayerID(), dwPID);
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	LPCHARACTER pchInvitee = CHARACTER_MANAGER::instance().FindByPID(dwPID);
	if (pchInvitee)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> %s님이 파티 초대를 거절하셨습니다."), pchInvitee->GetName());
}

void CHARACTER::PartyJoin(LPCHARACTER pLeader)
{
	pLeader->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> %s님이 파티에 참가하셨습니다."), GetName());
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> %s님의 파티에 참가하셨습니다."), pLeader->GetName());

	pLeader->GetParty()->Join(GetPlayerID());
	pLeader->GetParty()->Link(this);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
	if (pchLeader->GetEmpire() != pchGuest->GetEmpire())
		return PERR_DIFFEMPIRE;

	return IsPartyJoinableMutableCondition(pchLeader, pchGuest);
}

static bool __party_can_join_by_level(LPCHARACTER leader, LPCHARACTER quest)
{
	int level_limit = 30;
	return (abs(leader->GetLevel() - quest->GetLevel()) <= level_limit);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableMutableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
	if (!CPartyManager::instance().IsEnablePCParty())
		return PERR_SERVER;
	else if (pchLeader->GetDungeon())
		return PERR_DUNGEON;
	else if (pchGuest->IsObserverMode())
		return PERR_OBSERVER;
	else if (false == __party_can_join_by_level(pchLeader, pchGuest))
		return PERR_LVBOUNDARY;
	else if (pchGuest->GetParty())
		return PERR_ALREADYJOIN;
	else if (pchLeader->GetParty())
	{
		if (pchLeader->GetParty()->GetMemberCount() == PARTY_MAX_MEMBER)
			return PERR_PARTYISFULL;
	}

	return PERR_NONE;
}
// END_OF_PARTY_JOIN_BUG_FIX

void CHARACTER::SetDungeon(LPDUNGEON pkDungeon)
{
	if (pkDungeon && m_pkDungeon)
		sys_err("%s is trying to reassigning dungeon (current %p, new party %p)", GetName(), get_pointer(m_pkDungeon), get_pointer(pkDungeon));

	if (m_pkDungeon == pkDungeon) {
		return;
	}

	if (m_pkDungeon)
	{
		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->DecPartyMember(GetParty(), this);
			else
				m_pkDungeon->DecMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->DecMonster();
		}
	}

	m_pkDungeon = pkDungeon;

	if (pkDungeon)
	{
		sys_log(0, "%s DUNGEON set to %p, PARTY is %p", GetName(), get_pointer(pkDungeon), get_pointer(m_pkParty));

		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->IncPartyMember(GetParty(), this);
			else
				m_pkDungeon->IncMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->IncMonster();
		}
	}
}

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
void CHARACTER::SetZodiac(LPZODIAC pkZodiac)
{
	if (pkZodiac && m_pkZodiac)
		sys_err("%s is trying to reassigning zodiac (current %p, new party %p)", GetName(), get_pointer(m_pkZodiac), get_pointer(pkZodiac));

	if (m_pkZodiac == pkZodiac)
	{
		return;
	}

	if (m_pkZodiac)
	{
		if (IsPC())
		{
			if (GetParty())
				m_pkZodiac->DecPartyMember(GetParty(), this);
			else
				m_pkZodiac->DecMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkZodiac->DecMonster();
		}
	}

	m_pkZodiac = pkZodiac;

	if (pkZodiac)
	{
		sys_log(0, "%s ZODIAC set to %p, PARTY is %p", GetName(), get_pointer(pkZodiac), get_pointer(m_pkParty));

		if (IsPC())
		{
			if (GetParty())
				m_pkZodiac->IncPartyMember(GetParty(), this);
			else
				m_pkZodiac->IncMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkZodiac->IncMonster();
		}
	}
}
#endif

void CHARACTER::SetWarMap(CWarMap * pWarMap)
{
	if (m_pWarMap)
		m_pWarMap->DecMember(this);

	m_pWarMap = pWarMap;

	if (m_pWarMap)
		m_pWarMap->IncMember(this);
}

void CHARACTER::SetWeddingMap(marriage::WeddingMap * pMap)
{
	if (m_pWeddingMap)
		m_pWeddingMap->DecMember(this);

	m_pWeddingMap = pMap;

	if (m_pWeddingMap)
		m_pWeddingMap->IncMember(this);
}

void CHARACTER::SetRegen(LPREGEN pkRegen)
{
	m_pkRegen = pkRegen;
	if (pkRegen != nullptr) {
		regen_id_ = pkRegen->id;
	}
	m_fRegenAngle = GetRotation();
	m_posRegen = GetXYZ();
}

bool CHARACTER::OnIdle()
{
	return false;
}

void CHARACTER::OnMove(bool bIsAttack)
{
	m_dwLastMoveTime = get_dword_time();

	if (bIsAttack)
	{
		m_dwLastAttackTime = m_dwLastMoveTime;

		if (IsAffectFlag(AFF_REVIVE_INVISIBLE))
			RemoveAffect(AFFECT_REVIVE_INVISIBLE);
		if (IsAffectFlag(AFF_EUNHYUNG))
		{
			RemoveAffect(SKILL_EUNHYUNG);
			SetAffectedEunhyung();
		}
		else
		{
			ClearAffectedEunhyung();
		}

		SECTREE* sectree = GetSectree();

		if (sectree && sectree->IsAttr(GetX(), GetY(), ATTR_BANPK) && IsMonster())
			Return();

		/*if (IsAffectFlag(AFF_JEONSIN))
		  RemoveAffect(SKILL_JEONSINBANGEO);*/
	}

	/*if (IsAffectFlag(AFF_GUNGON))
	  RemoveAffect(SKILL_GUNGON);*/

	  // MINING
	mining_cancel();
	// END_OF_MINING
}

void CHARACTER::OnClick(LPCHARACTER pkChrCauser)
{
	if (!pkChrCauser)
	{
		sys_err("OnClick %s by nullptr", GetName());
		return;
	}

	DWORD vid = GetVID();
	sys_log(0, "OnClick %s[vnum %d ServerUniqueID %d, pid %d] by %s", GetName(), GetRaceNum(), vid, GetPlayerID(), pkChrCauser->GetName());

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if ((IsNPC()) && (GetRaceNum() == (WORD)(MeleyLair::GATE_VNUM)) && (MeleyLair::CMgr::instance().IsMeleyMap(pkChrCauser->GetMapIndex())))
	{
		MeleyLair::CMgr::instance().Start(pkChrCauser, pkChrCauser->GetGuild());
		return;
	}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (pkChrCauser != this && IsPC() && pkChrCauser->IsPC() && IsPolymorphed() && (GetMapIndex() >= 3580000 && GetMapIndex() < 3590000) && (pkChrCauser->GetMapIndex() >= 3580000 && pkChrCauser->GetMapIndex() < 3590000))
	{
		if (GetPolymorphVnum() >= 20452 && GetPolymorphVnum() <= 20463)
		{
			SetPolymorph(0);
			return;
		}
	}
#endif

	// 상점을 연상태로 퀘스트를 진행할 수 ?愎?
	{
		// 단, 자신은 자신의 상점을 클릭할 수 있다.
		if (pkChrCauser->GetMyShop() && pkChrCauser != this)
		{
			sys_err("OnClick Fail (%s->%s) - pc has shop", pkChrCauser->GetName(), GetName());
			return;
		}
	}

	// 교환중일때 퀘스트를 진행할 수 없다.
	{
		if (pkChrCauser->GetExchange())
		{
			sys_err("OnClick Fail (%s->%s) - pc is exchanging", pkChrCauser->GetName(), GetName());
			return;
		}
	}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsPC() || (IsNPC() && (GetRaceNum() >= 30000 && GetRaceNum() <= 30014)))
	{
		if (!CTargetManager::instance().GetTargetInfo(pkChrCauser->GetPlayerID(), TARGET_TYPE_VID, GetVID()) || (GetRaceNum() >= 30000 && GetRaceNum() <= 30014))
#else

	if (IsPC())
	{
		if (!CTargetManager::instance().GetTargetInfo(pkChrCauser->GetPlayerID(), TARGET_TYPE_VID, GetVID()))
#endif
		{
			// 2005.03.17.myevan.타겟이 아닌 경우는 개인 상점 처리 기능을 작동시킨다.
			if (GetMyShop())
			{
				if (pkChrCauser->IsDead() == true) return;
				//PREVENT_TRADE_WINDOW
				if (pkChrCauser == this) // 자기는 가능
				{
					if ((GetExchange() || IsOpenSafebox() || GetShopOwner()) || IsCubeOpen())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
						return;
					}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
					if (IsCombOpen())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
						return;
					}
#endif
				}
				else // 다른 사람이 클릭했을때
				{
					// 클릭한 사람이 교환/창고/개인상점/상점이용중이라면 불가
					if ((pkChrCauser->GetExchange() || pkChrCauser->IsOpenSafebox() || pkChrCauser->GetMyShop() || pkChrCauser->GetShopOwner()) || pkChrCauser->IsCubeOpen())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
						return;
					}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
					if (pkChrCauser->IsCombOpen())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
						return;
					}
#endif

					// 클릭한 대상이 교환/창고/상점이용중이라면 불가
					//if ((GetExchange() || IsOpenSafebox() || GetShopOwner()))
					if ((GetExchange() || IsOpenSafebox() || IsCubeOpen()))
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 다른 거래를 하고 있는 중입니다."));
						return;
					}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
					if (IsCombOpen())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
						return;
					}
#endif
				}
				//END_PREVENT_TRADE_WINDOW

				if (pkChrCauser->GetShop())
				{
					pkChrCauser->GetShop()->RemoveGuest(pkChrCauser);
					pkChrCauser->SetShop(nullptr);
				}

				CTargetManager::Instance().DeleteTarget(pkChrCauser->GetPlayerID(), IsPrivShop() ? GetPrivShop() : GetPlayerID(), "SHOP_SEARCH_TARGET");

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
				if (GetMyShop()->IsLocked())
					return;
#endif
				GetMyShop()->AddGuest(pkChrCauser, GetVID(), false);
				pkChrCauser->SetShopOwner(this);
				return;
			}

			if (test_server)
				sys_err("%s.OnClickFailure(%s) - target is PC", pkChrCauser->GetName(), GetName());

			return;
		}
	}

	pkChrCauser->SetQuestNPCID(GetVID());

	if (quest::CQuestManager::instance().Click(pkChrCauser->GetPlayerID(), this))
	{
		return;
	}

	// NPC 전용 기능 수행 : 상점 열기 등
	if (!IsPC())
	{
		if (!m_triggerOnClick.pFunc)
		{
			// NPC 트리거 시스템 로그 보기
			//sys_err("%s.OnClickFailure(%s) : triggerOnClick.pFunc is EMPTY(pid=%d)",
			//			pkChrCauser->GetName(),
			//			GetName(),
			//			pkChrCauser->GetPlayerID());
			return;
		}

		m_triggerOnClick.pFunc(this, pkChrCauser);
	}
	}

BYTE CHARACTER::GetGMLevel() const
{
	if (test_server)
		return GM_IMPLEMENTOR;
	return m_pointsInstant.gm_level;
}

int CHARACTER::GetChannel() const
{
	return g_bChannel;
}

void CHARACTER::SetGMLevel()
{
	if (GetDesc())
	{
		m_pointsInstant.gm_level = gm_get_level(GetName(), GetDesc()->GetHostName(), GetDesc()->GetAccountTable().login);
	}
	else
	{
		m_pointsInstant.gm_level = GM_PLAYER;
	}
}

BOOL CHARACTER::IsGM() const
{
	if (m_pointsInstant.gm_level != GM_PLAYER)
		return true;
	if (test_server)
		return true;
	return false;
}

void CHARACTER::SetStone(LPCHARACTER pkChrStone)
{
	m_pkChrStone = pkChrStone;

	if (m_pkChrStone)
	{
		if (pkChrStone->m_set_pkChrSpawnedBy.find(this) == pkChrStone->m_set_pkChrSpawnedBy.end())
			pkChrStone->m_set_pkChrSpawnedBy.insert(this);
	}
}

struct FuncDeadSpawnedByStone
{
	void operator () (LPCHARACTER ch)
	{
		ch->Dead(nullptr);
		ch->SetStone(nullptr);
	}
};

void CHARACTER::ClearStone()
{
	if (!m_set_pkChrSpawnedBy.empty())
	{
		// 내가 스폰시킨 몬스터들을 모두 죽인다.
		FuncDeadSpawnedByStone f;
		std::for_each(m_set_pkChrSpawnedBy.begin(), m_set_pkChrSpawnedBy.end(), f);
		m_set_pkChrSpawnedBy.clear();
	}

	if (!m_pkChrStone)
		return;

	m_pkChrStone->m_set_pkChrSpawnedBy.erase(this);
	m_pkChrStone = nullptr;
}

void CHARACTER::ClearTarget()
{
	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);
		m_pkChrTarget = nullptr;
	}

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	p.dwVID = 0;
	p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
	p.iMinHP = 0;
	p.iMaxHP = 0;
#endif
	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();
	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *(it++);
		pkChr->m_pkChrTarget = nullptr;

		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}

	m_set_pkChrTargetedBy.clear();
}

void CHARACTER::SetTarget(LPCHARACTER pkChrTarget)
{
	if (m_pkChrTarget == pkChrTarget)
		return;

	if (m_pkChrTarget)
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);

	m_pkChrTarget = pkChrTarget;

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.insert(this);
		p.dwVID = m_pkChrTarget->GetVID();
#ifdef ENABLE_VIEW_TARGET_MONSTER_HP
		if ((m_pkChrTarget->GetMaxHP() <= 0))
		{
			p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else if (m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed())
		{
			p.bHPPercent = MINMAX(0, (m_pkChrTarget->GetHP() * 100) / m_pkChrTarget->GetMaxHP(), 100);
#ifdef USE_TARGET_DECIMAL_HP
			p.iMinHP = m_pkChrTarget->GetHP();
			p.iMaxHP = m_pkChrTarget->GetMaxHP();
#endif
#else
		if ((m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed()) || (m_pkChrTarget->GetMaxHP() <= 0))
		{
			p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
#endif
		}
		else
		{
			if (m_pkChrTarget->GetRaceNum() == 20101 ||
				m_pkChrTarget->GetRaceNum() == 20102 ||
				m_pkChrTarget->GetRaceNum() == 20103 ||
				m_pkChrTarget->GetRaceNum() == 20104 ||
				m_pkChrTarget->GetRaceNum() == 20105 ||
				m_pkChrTarget->GetRaceNum() == 20106 ||
				m_pkChrTarget->GetRaceNum() == 20107 ||
				m_pkChrTarget->GetRaceNum() == 20108 ||
				m_pkChrTarget->GetRaceNum() == 20109)
			{
				LPCHARACTER owner = m_pkChrTarget->GetVictim();
				if (owner)
				{
					int iHorseHealth = owner->GetHorseHealth();
					int iHorseMaxHealth = owner->GetHorseMaxHealth();
					if (iHorseMaxHealth)
					{
						p.bHPPercent = MINMAX(0, iHorseHealth * 100 / iHorseMaxHealth, 100);
#ifdef USE_TARGET_DECIMAL_HP
						p.iMinHP = 100;
						p.iMaxHP = 100;
#endif
					}
					else
					{
						p.bHPPercent = 100;
#ifdef USE_TARGET_DECIMAL_HP
						p.iMinHP = 100;
						p.iMaxHP = 100;
#endif
					}
				}
				else
				{
					p.bHPPercent = 100;
#ifdef USE_TARGET_DECIMAL_HP
					p.iMinHP = 100;
					p.iMaxHP = 100;
#endif
				}
			}
			else
			{
				if (m_pkChrTarget->GetMaxHP() <= 0)
				{
					p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
					p.iMinHP = 0;
					p.iMaxHP = 0;
#endif
				}
				else
				{
					p.bHPPercent = MINMAX(0, (m_pkChrTarget->GetHP() * 100) / m_pkChrTarget->GetMaxHP(), 100);
#ifdef USE_TARGET_DECIMAL_HP
					p.iMinHP = m_pkChrTarget->GetHP();
					p.iMaxHP = m_pkChrTarget->GetMaxHP();
#endif
				}
			}
		}
		}
	else
	{
		p.dwVID = 0;
		p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
		p.iMinHP = 0;
		p.iMaxHP = 0;
#endif
	}

#ifdef ENABLE_TARGET_ELEMENT_SYSTEM
	const int ELEMENT_BASE = 11;
	DWORD curElementBase = ELEMENT_BASE;
	DWORD raceFlag;
	if (m_pkChrTarget && m_pkChrTarget->IsMonster() && (raceFlag = m_pkChrTarget->GetMobTable().dwRaceFlag) >= RACE_FLAG_ATT_ELEC)
	{
		for (int i = RACE_FLAG_ATT_ELEC; i <= RACE_FLAG_ATT_DARK; i *= 2)
		{
			curElementBase++;
			int diff = raceFlag - i;
			if (abs(diff) <= 1024)
				break;
		}
		p.bElement = curElementBase - ELEMENT_BASE;
	}
	else
	{
		p.bElement = 0;
	}

	if (m_pkChrTarget && m_pkChrTarget->IsPC())
	{
		LPITEM targetCharm = m_pkChrTarget->GetWear(WEAR_CHARM);
		if (targetCharm)
		{
			if (targetCharm->GetVnum() >= 9600 && targetCharm->GetVnum() <= 9800)
				p.bElement = 2;
			else if (targetCharm->GetVnum() >= 9830 && targetCharm->GetVnum() <= 10030)
				p.bElement = 3;
			else if (targetCharm->GetVnum() >= 10060 && targetCharm->GetVnum() <= 10260)
				p.bElement = 5;
			else if (targetCharm->GetVnum() >= 10290 && targetCharm->GetVnum() <= 10490)
				p.bElement = 6;
			else if (targetCharm->GetVnum() >= 10520 && targetCharm->GetVnum() <= 10720)
				p.bElement = 4;
			else if (targetCharm->GetVnum() >= 10750 && targetCharm->GetVnum() <= 10950)
				p.bElement = 1;
		}
	}
#endif

	GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
}

#ifdef ENABLE_HEALTH_BOARD_SYSTEM
void CHARACTER::HealthBoardPacket()
{
	if (!IsPC())
		return;

	if (GetQuestFlag("game_option.hide_health_board"))
	{
		TPacketGCHealthBoard p2;
		p2.bHeader = HEADER_GC_HEALTH_BOARD;
		p2.dwVID = GetVID();
		p2.bHPPercent = 0;
		PacketAround(&p2, sizeof(TPacketGCHealthBoard));
		return;
	}

	TPacketGCHealthBoard p2;
	p2.bHeader = HEADER_GC_HEALTH_BOARD;
	p2.dwVID = GetVID();
	p2.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);
	PacketAround(&p2, sizeof(TPacketGCHealthBoard));
}
#endif

void CHARACTER::BroadcastTargetPacket()
{
	if (m_set_pkChrTargetedBy.empty())
		return;

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	p.dwVID = GetVID();
	if (GetMaxHP() <= 0)
	{
		p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
		p.iMinHP = 0;
		p.iMaxHP = 0;
#endif
	}
	else
	{
#ifdef ENABLE_VIEW_TARGET_MONSTER_HP
		p.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);
#ifdef USE_TARGET_DECIMAL_HP
		p.iMinHP = GetHP();
		p.iMaxHP = GetMaxHP();
#endif
#else
		if (IsPC() || IsPet() || IsNewPet() || IsMount())
		{
			p.bHPPercent = 0;
#ifdef USE_TARGET_DECIMAL_HP
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else
		{
			p.bHPPercent = MINMAX(0, (GetHP() * 100) / GetMaxHP(), 100);
#ifdef USE_TARGET_DECIMAL_HP
			p.iMinHP = GetHP();
			p.iMaxHP = GetMaxHP();
#endif
		}
#endif
	}

	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();
	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *it++;
		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}
}

void CHARACTER::CheckTarget()
{
	if (!m_pkChrTarget)
		return;

	if (DISTANCE_APPROX(GetX() - m_pkChrTarget->GetX(), GetY() - m_pkChrTarget->GetY()) >= 4800)
		SetTarget(nullptr);
}

void CHARACTER::SetWarpLocation(long lMapIndex, long x, long y)
{
	m_posWarp.x = x * 100;
	m_posWarp.y = y * 100;
	m_lWarpMapIndex = lMapIndex;
}

void CHARACTER::SaveExitLocation()
{
	m_posExit = GetXYZ();
	m_lExitMapIndex = GetMapIndex();
}

void CHARACTER::ExitToSavedLocation()
{
	sys_log(0, "ExitToSavedLocation");
	WarpSet(m_posWarp.x, m_posWarp.y, m_lWarpMapIndex);

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;
}

// fixme
// 지금까진 privateMapIndex 가 현재 맵 인덱스와 같은지 체크 하는 것을 외부에서 하고,
// 다르면 warpset을 불렀는데
// 이를 warpset 안으로 넣자.
bool CHARACTER::WarpSet(long x, long y, long lPrivateMapIndex)
{
	if (!IsPC())
		return false;

	long lAddr;
	long lMapIndex;
	WORD wPort;

	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
	{
		SetWarpLocation(EMPIRE_START_MAP(GetEmpire()),
			EMPIRE_START_X(GetEmpire()) / 100,
			EMPIRE_START_Y(GetEmpire()) / 100);
		return false;
	}

	if (lPrivateMapIndex >= 10000)
	{
		if (lPrivateMapIndex / 10000 != lMapIndex)
		{
			sys_err("Invalid map index %d, must be child of %d", lPrivateMapIndex, lMapIndex);
			return false;
		}

		lMapIndex = lPrivateMapIndex;
	}

	Stop();
	Save();

	if (GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();

		EncodeRemovePacket(this);
	}

	m_lWarpMapIndex = lMapIndex;
	m_posWarp.x = x;
	m_posWarp.y = y;

	sys_log(0, "WarpSet %s %d %d current map %d target map %d", GetName(), x, y, GetMapIndex(), lMapIndex);

	TPacketGCWarp p;

	p.bHeader = HEADER_GC_WARP;
	p.lX = x;
	p.lY = y;
	p.lAddr = lAddr;
	p.wPort = wPort;

	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));

	char buf[256];
	snprintf(buf, sizeof(buf), "%s MapIdx %ld DestMapIdx%ld DestX%ld DestY%ld Empire%d", GetName(), GetMapIndex(), lPrivateMapIndex, x, y, GetEmpire());
#ifdef ENABLE_USELESS_LOGS
	LogManager::instance().CharLog(this, 0, "WARP", buf);
#endif

	return true;
}

void CHARACTER::WarpEnd()
{
	if (test_server)
		sys_log(0, "WarpEnd %s", GetName());

	if (m_posWarp.x == 0 && m_posWarp.y == 0)
		return;

	int index = m_lWarpMapIndex;

	if (index > 10000)
		index /= 10000;

	if (!map_allow_find(index))
	{
		GoHome();
		return;
	}

	sys_log(0, "WarpEnd %s %d %u %u", GetName(), m_lWarpMapIndex, m_posWarp.x, m_posWarp.y);

	Show(m_lWarpMapIndex, m_posWarp.x, m_posWarp.y, 0);
	Stop();

	m_lWarpMapIndex = 0;
	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;

	{
		// P2P Login
		TPacketGGLogin p;

		p.bHeader = HEADER_GG_LOGIN;
		strlcpy(p.szName, GetName(), sizeof(p.szName));
		p.dwPID = GetPlayerID();
		p.bEmpire = GetEmpire();
		p.lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(GetX(), GetY());
		p.bChannel = g_bChannel;
		p.iLevel = GetLevel();

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogin));
	}
}

bool CHARACTER::Return()
{
	if (!IsNPC())
		return false;

	int x, y;
	/*
	   float fDist = DISTANCE_SQRT(m_pkMobData->m_posLastAttacked.x - GetX(), m_pkMobData->m_posLastAttacked.y - GetY());
	   float fx, fy;
	   GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);
	   x = GetX() + (int) fx;
	   y = GetY() + (int) fy;
	 */
	SetVictim(nullptr);

	x = m_pkMobInst->m_posLastAttacked.x;
	y = m_pkMobInst->m_posLastAttacked.y;

	SetRotationToXY(x, y);

	if (!Goto(x, y))
		return false;

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	if (test_server)
		sys_log(0, "%s %p 포기하고 돌아가자! %d %d", GetName(), this, x, y);

	if (GetParty())
		GetParty()->SendMessage(this, PM_RETURN, x, y);

	return true;
}

bool CHARACTER::Follow(LPCHARACTER pkChr, float fMinDistance)
{
	if (IsPC())
	{
		sys_err("CHARACTER::Follow : PC cannot use this method", GetName());
		return false;
	}

	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
	{
		if (pkChr->IsPC()) // 쫓아가는 상대가 PC일 때
		{
			// If i'm in a party. I must obey party leader's AI.
			if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
			{
				if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // 마지막으로 공격받은지 15초가 지났고
				{
					// 마지막 맞은 곳으로 부터 50미터 이상 차이나면 포기하고 돌아간다.
					if (m_pkMobData->m_table.wAttackRange < DISTANCE_APPROX(pkChr->GetX() - GetX(), pkChr->GetY() - GetY()))
						if (Return())
							return true;
				}
			}
		}
		return false;
	}
	// END_OF_TRENT_MONSTER

	long x = pkChr->GetX();
	long y = pkChr->GetY();

	if (pkChr->IsPC()) // 쫓아가는 상대가 PC일 때
	{
		// If i'm in a party. I must obey party leader's AI.
		if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
		{
			if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // 마지막으로 공격받은지 15초가 지났고
			{
				// 마지막 맞은 곳으로 부터 50미터 이상 차이나면 포기하고 돌아간다.
				if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
					if (Return())
						return true;
			}
		}
	}

	if (IsGuardNPC())
	{
		if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
			if (Return())
				return true;
	}

	if (pkChr->IsState(pkChr->m_stateMove) &&
		GetMobBattleType() != BATTLE_TYPE_RANGE &&
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet() && false == IsNewPet() && false == IsMount())
	{
		// 대상이 이동중이면 예측 이동을 한다
		// 나와 상대방의 속도차와 거리로부터 만날 시간을 예상한 후
		// 상대방이 그 시간까지 직선으로 이동한다고 가정하여 거기로 이동한다.
		float rot = pkChr->GetRotation();
		float rot_delta = GetDegreeDelta(rot, GetDegreeFromPositionXY(GetX(), GetY(), pkChr->GetX(), pkChr->GetY()));

		float yourSpeed = pkChr->GetMoveSpeed();
		float mySpeed = GetMoveSpeed();

		float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());
		float fFollowSpeed = mySpeed - yourSpeed * cos(rot_delta * M_PI / 180);

		if (fFollowSpeed >= 0.1f)
		{
			float fMeetTime = fDist / fFollowSpeed;
			float fYourMoveEstimateX, fYourMoveEstimateY;

			if (fMeetTime * yourSpeed <= 100000.0f)
			{
				GetDeltaByDegree(pkChr->GetRotation(), fMeetTime * yourSpeed, &fYourMoveEstimateX, &fYourMoveEstimateY);

				x += (long)fYourMoveEstimateX;
				y += (long)fYourMoveEstimateY;

				float fDistNew = sqrt(((double)x - GetX()) * (x - GetX()) + ((double)y - GetY()) * (y - GetY()));
				if (fDist < fDistNew)
				{
					x = (long)(GetX() + (x - GetX()) * fDist / fDistNew);
					y = (long)(GetY() + (y - GetY()) * fDist / fDistNew);
				}
			}
		}
	}

	// 가려는 위치를 바라봐야 한다.
	SetRotationToXY(x, y);

	float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());

	if (fDist <= fMinDistance)
		return false;

	float fx, fy;

	if (IsChangeAttackPosition(pkChr) && GetMobRank() < MOB_RANK_BOSS)
	{
		// 상대방 주변 랜덤한 곳으로 이동
		SetChangeAttackPositionTime();

		int retry = 16;
		int dx, dy;
		int rot = (int)GetDegreeFromPositionXY(x, y, GetX(), GetY());

		while (--retry)
		{
			if (fDist < 500.0f)
				GetDeltaByDegree((rot + number(-90, 90) + number(-90, 90)) % 360, fMinDistance, &fx, &fy);
			else
				GetDeltaByDegree(number(0, 359), fMinDistance, &fx, &fy);

			dx = x + (int)fx;
			dy = y + (int)fy;

			LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), dx, dy);

			if (nullptr == tree)
				break;

			if (0 == (tree->GetAttribute(dx, dy) & (ATTR_BLOCK | ATTR_OBJECT)))
				break;
		}
		//sys_log(0, "근처 ??딘가?이동 %s retry %d", GetName(), retry);
		if (!Goto(dx, dy))
			return false;
	}
	else
	{
		// 직선 따라가기
		float fDistToGo = fDist - fMinDistance;
		GetDeltaByDegree(GetRotation(), fDistToGo, &fx, &fy);

		//sys_log(0, "직선?막?이동 %s", GetName());
		if (!Goto(GetX() + (int)fx, GetY() + (int)fy))
			return false;
	}

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	//MonsterLog("쫓아가기; %s", pkChr->GetName());
	return true;
}

float CHARACTER::GetDistanceFromSafeboxOpen() const
{
	return DISTANCE_APPROX(GetX() - m_posSafeboxOpen.x, GetY() - m_posSafeboxOpen.y);
}

void CHARACTER::SetSafeboxOpenPosition()
{
	m_posSafeboxOpen = GetXYZ();
}

CSafebox* CHARACTER::GetSafebox() const
{
	return m_pkSafebox;
}

void CHARACTER::ReqSafeboxLoad(const char* pszPassword)
{
	if (!*pszPassword || strlen(pszPassword) > SAFEBOX_PASSWORD_MAX_LEN)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 잘못된 암호를 입력하셨습니다."));
		return;
	}
	else if (m_pkSafebox)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 창고가 이미 열려있습니다."));
		return;
	}

	int iPulse = thecore_pulse();

	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(10))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 창고를 닫은지 10초 안에는 열 수 없습니다."));
		return;
	}
	else if (m_bOpeningSafebox)
	{
		sys_log(0, "Overlapped safebox load request from %s", GetName());
		return;
	}

	SetSafeboxLoadTime();
	m_bOpeningSafebox = true;

	TSafeboxLoadPacket p;
	p.dwID = GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, pszPassword, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_LOAD, GetDesc()->GetHandle(), &p, sizeof(p));
}

void CHARACTER::LoadSafebox(int iSize, long long dwGold, WORD wCheque, int iItemCount, TPlayerItem * pItems)
{
	if (CAN_OPEN_SAFEBOX(GetMapIndex()) == false)
		return;

	bool bLoaded = false;

	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(true);
	//END_PREVENT_TRADE_WINDOW

	if (m_pkSafebox)
		bLoaded = true;

	if (!m_pkSafebox)
		m_pkSafebox = M2_NEW CSafebox(this, iSize, dwGold, wCheque);
	else
		m_pkSafebox->ChangeSize(iSize);

	m_iSafeboxSize = iSize;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = iSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));
	m_pkSafebox->SetGold(0);
	m_pkSafebox->SetCheque(0);

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkSafebox->IsValidPosition(pItems->pos))
				continue;

			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
#ifdef ENABLE_CHANGELOOK_SYSTEM
			item->SetTransmutation(pItems->transmutation);
#endif
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			item->SetSealBind(pItems->sealbind);
#endif

			if (!m_pkSafebox->Add(pItems->pos, item))
			{
				M2_DESTROY_ITEM(item);
			}
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::ChangeSafeboxSize(BYTE bSize)
{
	//if (!m_pkSafebox)
	//return;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = bSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (m_pkSafebox)
		m_pkSafebox->ChangeSize(bSize);

	m_iSafeboxSize = bSize;
}

void CHARACTER::CloseSafebox()
{
	if (!m_pkSafebox)
		return;
	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(false);
	//END_PREVENT_TRADE_WINDOW

#ifndef ENABLE_SAFEBOX_MONEY_SYSTEM
	m_pkSafebox->Save();
#endif

	M2_DELETE(m_pkSafebox);
	m_pkSafebox = nullptr;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseSafebox");

	SetSafeboxLoadTime();
	m_bOpeningSafebox = false;

	Save();
}

CSafebox* CHARACTER::GetMall() const
{
	return m_pkMall;
}

void CHARACTER::LoadMall(int iItemCount, TPlayerItem * pItems)
{
	bool bLoaded = false;

	if (m_pkMall)
		bLoaded = true;

	if (!m_pkMall)
#ifdef ENABLE_SAFEBOX_UPDATE
		m_pkMall = M2_NEW CSafebox(this, 5 * SAFEBOX_PAGE_SIZE, 0, 0);
#else
		m_pkMall = M2_NEW CSafebox(this, 3 * SAFEBOX_PAGE_SIZE, 0, 0);
#endif
	else
#ifdef ENABLE_SAFEBOX_UPDATE
		m_pkMall->ChangeSize(5 * SAFEBOX_PAGE_SIZE);
#else
		m_pkMall->ChangeSize(3 * SAFEBOX_PAGE_SIZE);
#endif

	m_pkMall->SetWindowMode(MALL);

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_MALL_OPEN;
#ifdef ENABLE_SAFEBOX_UPDATE
	p.bSize = 5 * SAFEBOX_PAGE_SIZE;
#else
	p.bSize = 3 * SAFEBOX_PAGE_SIZE;
#endif

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	SetSafeboxLoadTime();

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkMall->IsValidPosition(pItems->pos))
				continue;

			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
#ifdef ENABLE_CHANGELOOK_SYSTEM
			item->SetTransmutation(pItems->transmutation);
#endif
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			item->SetSealBind(pItems->sealbind);
#endif

			if (!m_pkMall->Add(pItems->pos, item))
				M2_DESTROY_ITEM(item);
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::CloseMall()
{
	if (!m_pkMall)
		return;

#ifndef ENABLE_SAFEBOX_MONEY_SYSTEM
	m_pkMall->Save();
#endif

	M2_DELETE(m_pkMall);
	m_pkMall = nullptr;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseMall");
}

bool CHARACTER::BuildUpdatePartyPacket(TPacketGCPartyUpdate & out)
{
	if (!GetParty())
		return false;

	memset(&out, 0, sizeof(out));

	out.header = HEADER_GC_PARTY_UPDATE;
	out.pid = GetPlayerID();
	if (GetMaxHP() <= 0)
		out.percent_hp = 0;
	else
		out.percent_hp = MINMAX(0, GetHP() * 100 / GetMaxHP(), 100);
	out.role = GetParty()->GetRole(GetPlayerID());

	sys_log(1, "PARTY %s role is %d", GetName(), out.role);

	LPCHARACTER l = GetParty()->GetLeaderCharacter();

	if (l && DISTANCE_APPROX(GetX() - l->GetX(), GetY() - l->GetY()) < PARTY_DEFAULT_RANGE)
	{
		out.affects[0] = GetParty()->GetPartyBonusExpPercent();
		out.affects[1] = GetPoint(POINT_PARTY_ATTACKER_BONUS);
		out.affects[2] = GetPoint(POINT_PARTY_TANKER_BONUS);
		out.affects[3] = GetPoint(POINT_PARTY_BUFFER_BONUS);
		out.affects[4] = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
		out.affects[5] = GetPoint(POINT_PARTY_HASTE_BONUS);
		out.affects[6] = GetPoint(POINT_PARTY_DEFENDER_BONUS);
	}

	return true;
}

int CHARACTER::GetLeadershipSkillLevel() const
{
	return GetSkillLevel(SKILL_LEADERSHIP);
}

void CHARACTER::QuerySafeboxSize()
{
	if (m_iSafeboxSize == -1)
	{
		DBManager::instance().ReturnQuery(QID_SAFEBOX_SIZE,
			GetPlayerID(),
			nullptr,
			"SELECT size FROM safebox%s WHERE account_id = %u",
			get_table_postfix(),
			GetDesc()->GetAccountTable().id);
	}
}

void CHARACTER::SetSafeboxSize(int iSize)
{
	sys_log(1, "SetSafeboxSize: %s %d", GetName(), iSize);
	m_iSafeboxSize = iSize;
	DBManager::instance().Query("UPDATE safebox%s SET size = %d WHERE account_id = %u", get_table_postfix(), iSize / SAFEBOX_PAGE_SIZE, GetDesc()->GetAccountTable().id);
}

int CHARACTER::GetSafeboxSize() const
{
	return m_iSafeboxSize;
}

void CHARACTER::SetNowWalking(bool bWalkFlag)
{
	//if (m_bNowWalking != bWalkFlag || IsNPC())
	if (m_bNowWalking != bWalkFlag)
	{
		if (bWalkFlag)
		{
			m_bNowWalking = true;
			m_dwWalkStartTime = get_dword_time();
		}
		else
		{
			m_bNowWalking = false;
		}

		TPacketGCWalkMode p;
		p.vid = GetVID();
		p.header = HEADER_GC_WALK_MODE;
		p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

		PacketView(&p, sizeof(p));

		if (IsNPC())
		{
			if (m_bNowWalking)
				MonsterLog("Walking");
			else
				MonsterLog("Jumps");
		}

		//sys_log(0, "%s is now %s", GetName(), m_bNowWalking?"walking.":"running.");
	}
}

void CHARACTER::StartStaminaConsume()
{
	if (m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = true;
	//ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
	if (IsStaminaHalfConsume())
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec / 2, GetStamina());
	else
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
}

void CHARACTER::StopStaminaConsume()
{
	if (!m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = false;
	ChatPacket(CHAT_TYPE_COMMAND, "StopStaminaConsume %d", GetStamina());
}

bool CHARACTER::IsStaminaConsume() const
{
	return m_bStaminaConsume;
}

bool CHARACTER::IsStaminaHalfConsume() const
{
	return IsEquipUniqueItem(UNIQUE_ITEM_HALF_STAMINA);
}

void CHARACTER::ResetStopTime()
{
	m_dwStopTime = get_dword_time();
}

DWORD CHARACTER::GetStopTime() const
{
	return m_dwStopTime;
}

void CHARACTER::ResetPoint(int iLv)
{
	BYTE bJob = GetJob();

	PointChange(POINT_LEVEL, iLv - GetLevel());

	SetRealPoint(POINT_ST, JobInitialPoints[bJob].st);
	SetPoint(POINT_ST, GetRealPoint(POINT_ST));

	SetRealPoint(POINT_HT, JobInitialPoints[bJob].ht);
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));

	SetRealPoint(POINT_DX, JobInitialPoints[bJob].dx);
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));

	SetRealPoint(POINT_IQ, JobInitialPoints[bJob].iq);
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetRandomHP((iLv - 1) * number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end));
	SetRandomSP((iLv - 1) * number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end));

	// @fixme104
	PointChange(POINT_STAT, (MINMAX(1, iLv, g_iStatusPointGetLevelLimit) * 3) + GetPoint(POINT_LEVEL_STEP) - GetPoint(POINT_STAT));

	ComputePoints();

	// 회복
	PointChange(POINT_HP, GetMaxHP() - GetHP());
	PointChange(POINT_SP, GetMaxSP() - GetSP());

	PointsPacket();

#ifdef ENABLE_USELESS_LOGS
	LogManager::instance().CharLog(this, 0, "RESET_POINT", "");
#endif
}

bool CHARACTER::IsChangeAttackPosition(LPCHARACTER target) const
{
	if (!IsNPC())
		return true;

	DWORD dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_NEAR;

	if (DISTANCE_APPROX(GetX() - target->GetX(), GetY() - target->GetY()) >
		AI_CHANGE_ATTACK_POISITION_DISTANCE + GetMobAttackRange())
		dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_FAR;

	return get_dword_time() - m_dwLastChangeAttackPositionTime > dwChangeTime;
}

void CHARACTER::GiveRandomSkillBook()
{
	LPITEM item = AutoGiveItem(50300);
	if (nullptr != item)
	{
		BYTE bJob = 0;
		if (!number(0, 1))
			bJob = GetJob() + 1;

		DWORD dwSkillVnum = 0;
		do
		{
			if (number(1, 20) == 2)
			{
				dwSkillVnum = number(170, 175);
			}
			else
			{
				dwSkillVnum = number(1, 111);
			}
			const CSkillProto* pkSk = CSkillManager::instance().Get(dwSkillVnum);

			if (nullptr == pkSk)
				continue;

			if (bJob && bJob != pkSk->dwType)
				continue;

			break;
		} while (true);

		item->SetSocket(0, dwSkillVnum);
	}
}

void CHARACTER::ReviveInvisible(int iDur)
{
	AddAffect(AFFECT_REVIVE_INVISIBLE, POINT_NONE, 0, AFF_REVIVE_INVISIBLE, iDur, 0, true);
}

void CHARACTER::ToggleMonsterLog()
{
	m_bMonsterLog = !m_bMonsterLog;

	if (m_bMonsterLog)
	{
		CHARACTER_MANAGER::instance().RegisterForMonsterLog(this);
	}
	else
	{
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
	}
}

void CHARACTER::SetGuild(CGuild * pGuild)
{
	if (m_pGuild != pGuild)
	{
		m_pGuild = pGuild;
		UpdatePacket();
	}
}

void CHARACTER::BeginStateEmpty()
{
	MonsterLog("!");
}

void CHARACTER::EffectPacket(int enumEffectType)
{
	TPacketGCSpecialEffect p;

	p.header = HEADER_GC_SEPCIAL_EFFECT;
	p.type = enumEffectType;
	p.vid = GetVID();

	PacketAround(&p, sizeof(TPacketGCSpecialEffect));
}

void CHARACTER::SpecificEffectPacket(const char filename[128])
{
	TPacketGCSpecificEffect p;

	p.header = HEADER_GC_SPECIFIC_EFFECT;
	p.vid = GetVID();
	memcpy(p.effect_file, filename, 128);

	PacketAround(&p, sizeof(TPacketGCSpecificEffect));
}

void CHARACTER::MonsterChat(BYTE bMonsterChatType)
{
	if (IsPC())
		return;

	char sbuf[CHAT_MAX_LEN + 1];

	if (IsMonster())
	{
		if (number(0, 60))
			return;

		snprintf(sbuf, sizeof(sbuf),
			"(locale.monster_chat[%i] and locale.monster_chat[%i][%d] or '')",
			GetRaceNum(), GetRaceNum(), bMonsterChatType * 3 + number(1, 3));
	}
	else
	{
		if (bMonsterChatType != MONSTER_CHAT_WAIT)
			return;

		if (IsGuardNPC())
		{
			if (number(0, 6))
				return;
		}
		else
		{
			if (number(0, 30))
				return;
		}

		snprintf(sbuf, sizeof(sbuf), "(locale.monster_chat[%i] and locale.monster_chat[%i][number(1, table.getn(locale.monster_chat[%i]))] or '')", GetRaceNum(), GetRaceNum(), GetRaceNum());
	}

	std::string text = quest::ScriptToString(sbuf);

	if (text.empty())
		return;

	struct packet_chat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(struct packet_chat) + text.size() + 1;
	pack_chat.type = CHAT_TYPE_TALKING;
	pack_chat.id = GetVID();
	pack_chat.bEmpire = 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(text.c_str(), text.size() + 1);

	PacketAround(buf.read_peek(), buf.size());
}

void CHARACTER::SetQuestNPCID(DWORD vid)
{
	m_dwQuestNPCVID = vid;
}

LPCHARACTER CHARACTER::GetQuestNPC() const
{
	return CHARACTER_MANAGER::instance().Find(m_dwQuestNPCVID);
}

void CHARACTER::SetQuestItemPtr(LPITEM item)
{
	m_pQuestItem = item;
}

void CHARACTER::ClearQuestItemPtr()
{
	m_pQuestItem = nullptr;
}

LPITEM CHARACTER::GetQuestItemPtr() const
{
	return m_pQuestItem;
}

LPDUNGEON CHARACTER::GetDungeonForce() const
{
	if (m_lWarpMapIndex > 10000)
		return CDungeonManager::instance().FindByMapIndex(m_lWarpMapIndex);

	return m_pkDungeon;
}

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
LPZODIAC CHARACTER::GetZodiacForce() const
{ 
	if (m_lWarpMapIndex > 10000)
		return CZodiacManager::instance().FindByMapIndex(m_lWarpMapIndex);

	return m_pkZodiac;
}
#endif

void CHARACTER::SetBlockMode(int bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;

	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);

	SetQuestFlag("game_option.block_exchange", bFlag & BLOCK_EXCHANGE ? 1 : 0);
	SetQuestFlag("game_option.block_party_invite", bFlag & BLOCK_PARTY_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_guild_invite", bFlag & BLOCK_GUILD_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_whisper", bFlag & BLOCK_WHISPER ? 1 : 0);
	SetQuestFlag("game_option.block_messenger_invite", bFlag & BLOCK_MESSENGER_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_party_request", bFlag & BLOCK_PARTY_REQUEST ? 1 : 0);
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	SetQuestFlag("game_option.block_equipment_request", bFlag & BLOCK_EQUIPMENT_REQUEST ? 1 : 0);
#endif
}

void CHARACTER::SetBlockModeForce(int bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;
	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);
}

bool CHARACTER::IsGuardNPC() const
{
	return IsNPC() && (GetRaceNum() == 11000 || GetRaceNum() == 11002 || GetRaceNum() == 11004);
}

int CHARACTER::GetPolymorphPower() const
{
	if (test_server)
	{
		int value = quest::CQuestManager::instance().GetEventFlag("poly");
		if (value)
			return value;
	}
	return aiPolymorphPowerByLevel[MINMAX(0, GetSkillLevel(SKILL_POLYMORPH), 40)];
}

void CHARACTER::SetPolymorph(DWORD dwRaceNum, bool bMaintainStat)
{
#ifdef ENABLE_WOLFMAN_CHARACTER
	if (dwRaceNum < MAIN_RACE_MAX_NUM)
#else
	if (dwRaceNum < JOB_MAX_NUM)
#endif
	{
		dwRaceNum = 0;
		bMaintainStat = false;
	}

	if (m_dwPolymorphRace == dwRaceNum)
		return;

	m_bPolyMaintainStat = bMaintainStat;
	m_dwPolymorphRace = dwRaceNum;

	sys_log(0, "POLYMORPH: %s race %u ", GetName(), dwRaceNum);

	if (dwRaceNum != 0)
		StopRiding();

	SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
	m_afAffectFlag.Set(AFF_SPAWN);

	ViewReencode();

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);

	if (!bMaintainStat)
	{
		PointChange(POINT_ST, 0);
		PointChange(POINT_DX, 0);
		PointChange(POINT_IQ, 0);
		PointChange(POINT_HT, 0);
	}

	// 폴리모프 상태에서 죽는 경우, 폴리모프가 풀리게 되는데
	// 폴리 모프 전후로 valid combo interval이 다르기 때문에
	// Combo 핵 또는 Hacker로 인식하는 경우가 있다.
	// 따라서 폴리모프를 풀거나 폴리모프 하게 되면,
	// valid combo interval을 reset한다.
	SetValidComboInterval(0);
	ComputeBattlePoints();
}

int CHARACTER::GetQuestFlag(const std::string & flag) const
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());
	if (!pPC) {
		return 0;
	}
	return pPC->GetFlag(flag);
}

void CHARACTER::SetQuestFlag(const std::string & flag, int value)
{
	quest::CQuestManager& q = quest::CQuestManager::instance();
	quest::PC* pPC = q.GetPC(GetPlayerID());
	if (!pPC) {
		return;
	}
	pPC->SetFlag(flag, value);
}

void CHARACTER::DetermineDropMetinStone()
{
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	const int METIN_STONE_NUM = 17;
#else
	const int METIN_STONE_NUM = 15;
#endif
	static DWORD c_adwMetin[METIN_STONE_NUM] =
	{
		28012,
		28030,
		28031,
		28032,
		28033,
		28034,
		28035,
		28036,
		28037,
		28038,
		28039,
		28040,
		28041,
		28042,
		28043,
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		28044,
		28045,
#endif
	};
	DWORD stone_num = GetRaceNum();
	int idx = std::lower_bound(aStoneDrop, aStoneDrop + STONE_INFO_MAX_NUM, stone_num) - aStoneDrop;
	if (idx >= STONE_INFO_MAX_NUM || aStoneDrop[idx].dwMobVnum != stone_num)
	{
		m_dwDropMetinStone = 0;
	}
	else
	{
		const SStoneDropInfo& info = aStoneDrop[idx];
		m_bDropMetinStonePct = info.iDropPct;
		{
			m_dwDropMetinStone = c_adwMetin[number(0, METIN_STONE_NUM - 1)];
			int iGradePct = number(1, 100);
			for (int iStoneLevel = 0; iStoneLevel < STONE_LEVEL_MAX_NUM; iStoneLevel++)
			{
				int iLevelGradePortion = info.iLevelPct[iStoneLevel];
				if (iGradePct <= iLevelGradePortion)
				{
					break;
				}
				else
				{
					iGradePct -= iLevelGradePortion;
					m_dwDropMetinStone += 100; // 돌 +a -> +(a+1)이 될때마다 100씩 증가
				}
			}
		}
	}
}

bool CHARACTER::CanSummon(int iLeaderShip)
{
	return ((iLeaderShip >= 20) || ((iLeaderShip >= 12) && ((m_dwLastDeadTime + 180) > get_dword_time())));
}

void CHARACTER::MountVnum(DWORD vnum)
{
	if (m_dwMountVnum == vnum)
		return;
	if ((m_dwMountVnum != 0) && (vnum != 0))
		MountVnum(0);

	m_dwMountVnum = vnum;
	m_dwMountTime = get_dword_time();

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif

	if (m_bIsObserver)
		return;

	//NOTE : Mount한다고 해서 Client Side의 객체를 삭제하진 않는다.
	//그리고 서버Side에서 탔을때 위치 이동은 하지 않는다. 왜냐하면 Client Side에서 Coliision Adjust를 할수 있는데
	//객체를 소멸시켰다가 서버위치로 이동시키면 이때 collision check를 하지는 않으므로 배경에 끼거나 뚫고 나가는 문제가 존재한다.
	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
	//EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		//Mount한다고 해서 Client Side의 객체를 삭제하진 않는다.
		//EncodeRemovePacket(entity);
		//if (!m_bIsObserver)
		EncodeInsertPacket(entity);

		//if (!entity->IsObserverMode())
		//	entity->EncodeInsertPacket(this);
	}

	SetValidComboInterval(0);

	ComputePoints();
}

namespace {
	class FuncCheckWarp
	{
	public:
		FuncCheckWarp(LPCHARACTER pkWarp)
		{
			m_lTargetY = 0;
			m_lTargetX = 0;

			m_lX = pkWarp->GetX();
			m_lY = pkWarp->GetY();

			m_bInvalid = false;
			m_bEmpire = pkWarp->GetEmpire();

			char szTmp[64];

			if (3 != sscanf(pkWarp->GetName(), " %s %ld %ld ", szTmp, &m_lTargetX, &m_lTargetY)
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
				&& pkWarp->GetRaceNum() != 3949
#endif
				)
			{
				if (number(1, 100) < 5)
					sys_err("Warp NPC name wrong : vnum(%d) name(%s)", pkWarp->GetRaceNum(), pkWarp->GetName());

				m_bInvalid = true;

				return;
			}

			m_lTargetX *= 100;
			m_lTargetY *= 100;

			m_bUseWarp = true;

			if (pkWarp->IsGoto())
			{
				LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(pkWarp->GetMapIndex());
				m_lTargetX += pkSectreeMap->m_setting.iBaseX;
				m_lTargetY += pkSectreeMap->m_setting.iBaseY;
				m_bUseWarp = false;
			}

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			pkWarps = pkWarp;
#endif
		}

		bool Valid()
		{
			return !m_bInvalid;
		}

		void operator () (LPENTITY ent)
		{
			if (!Valid())
				return;

			if (!ent->IsType(ENTITY_CHARACTER))
				return;

			LPCHARACTER pkChr = (LPCHARACTER)ent;

			if (!pkChr->IsPC())
				return;

			int iDist = DISTANCE_APPROX(pkChr->GetX() - m_lX, pkChr->GetY() - m_lY);

			if (iDist > 300)
				return;

			if (m_bEmpire && pkChr->GetEmpire() && m_bEmpire != pkChr->GetEmpire())
				return;

			if (pkChr->IsHack())
				return;

			if (!pkChr->CanHandleItem(false, true))
				return;

			if (m_bUseWarp)
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			{
				if (pkWarps->GetRaceNum() == 3949)
					pkChr->WarpSet(168600, 611200);
				else
					pkChr->WarpSet(m_lTargetX, m_lTargetY);
			}
#else
				pkChr->WarpSet(m_lTargetX, m_lTargetY);
#endif
			else
			{
				pkChr->Show(pkChr->GetMapIndex(), m_lTargetX, m_lTargetY);
				pkChr->Stop();
			}
		}

		bool m_bInvalid;
		bool m_bUseWarp;

		long m_lX;
		long m_lY;
		long m_lTargetX;
		long m_lTargetY;

		BYTE m_bEmpire;
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		LPCHARACTER pkWarps;
#endif
	};
}

EVENTFUNC(warp_npc_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("warp_npc_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}

	if (!ch->GetSectree())
	{
		ch->m_pkWarpNPCEvent = nullptr;
		return 0;
	}

	FuncCheckWarp f(ch);
	if (f.Valid())
		ch->GetSectree()->ForEachAround(f);

	return passes_per_sec / 2;
}

void CHARACTER::StartWarpNPCEvent()
{
	if (m_pkWarpNPCEvent)
		return;

	if (!IsWarp() && !IsGoto()
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		&& GetRaceNum() != 3949
#endif
		)
		return;

	char_event_info * info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkWarpNPCEvent = event_create(warp_npc_event, info, passes_per_sec / 2);
}

void CHARACTER::SyncPacket()
{
	TEMP_BUFFER buf;

	TPacketCGSyncPositionElement elem;

	elem.dwVID = GetVID();
	elem.lX = GetX();
	elem.lY = GetY();

	TPacketGCSyncPosition pack;

	pack.bHeader = HEADER_GC_SYNC_POSITION;
	pack.wSize = sizeof(TPacketGCSyncPosition) + sizeof(elem);

	buf.write(&pack, sizeof(pack));
	buf.write(&elem, sizeof(elem));

	PacketAround(buf.read_peek(), buf.size());
}

LPCHARACTER CHARACTER::GetMarryPartner() const
{
	return m_pkChrMarried;
}

void CHARACTER::SetMarryPartner(LPCHARACTER ch)
{
	m_pkChrMarried = ch;
}

int CHARACTER::GetMarriageBonus(DWORD dwItemVnum, bool bSum)
{
	if (IsNPC())
		return 0;

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());

	if (!pMarriage)
		return 0;

	return pMarriage->GetBonus(dwItemVnum, bSum, this);
}

void CHARACTER::ConfirmWithMsg(const char* szMsg, int iTimeout, DWORD dwRequestPID)
{
	if (!IsPC())
		return;

	TPacketGCQuestConfirm p;

	p.header = HEADER_GC_QUEST_CONFIRM;
	p.requestPID = dwRequestPID;
	p.timeout = iTimeout;
	strlcpy(p.msg, szMsg, sizeof(p.msg));

	GetDesc()->Packet(&p, sizeof(p));
}

int CHARACTER::GetPremiumRemainSeconds(BYTE bType) const
{
	if (bType >= PREMIUM_MAX_NUM)
		return 0;

	return m_aiPremiumTimes[bType] - get_global_time();
}

bool CHARACTER::WarpToPID(DWORD dwPID)
{
	LPCHARACTER victim;
	if ((victim = (CHARACTER_MANAGER::instance().FindByPID(dwPID))))
	{
		int mapIdx = victim->GetMapIndex();
		if (IS_SUMMONABLE_ZONE(mapIdx))
		{
			if (CAN_ENTER_ZONE(this, mapIdx))
			{
				WarpSet(victim->GetX(), victim->GetY());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 있는 곳으로 워프할 수 없습니다."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 있는 곳으로 워프할 수 없습니다."));
			return false;
		}
	}
	else
	{
		// 다른 서버에 로그인된 사람이 있음 -> 메시지 보내 좌표를 받아오자
		// 1. A.pid, B.pid 를 뿌림
		// 2. B.pid를 가진 서버가 뿌린서버에게 A.pid, 좌표 를 보냄
		// 3. 워프
		CCI* pcci = P2P_MANAGER::instance().FindByPID(dwPID);

		if (!pcci)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 온라인 상태가 아닙니다."));
			return false;
		}

		if (pcci->bChannel != g_bChannel)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 %d 채널에 있습니다. (현재 채널 %d)"), pcci->bChannel, g_bChannel);
			return false;
		}
		else if (false == IS_SUMMONABLE_ZONE(pcci->lMapIndex))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 있는 곳으로 워프할 수 없습니다."));
			return false;
		}
		else
		{
			if (!CAN_ENTER_ZONE(this, pcci->lMapIndex))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 있는 곳으로 워프할 수 없습니다."));
				return false;
			}

			TPacketGGFindPosition p;
			p.header = HEADER_GG_FIND_POSITION;
			p.dwFromPID = GetPlayerID();
			p.dwTargetPID = dwPID;
			pcci->pkDesc->Packet(&p, sizeof(TPacketGGFindPosition));

			if (test_server)
				ChatPacket(CHAT_TYPE_PARTY, "sent find position packet for teleport");
		}
	}
	return true;
}

// ADD_REFINE_BUILDING
CGuild* CHARACTER::GetRefineGuild() const
{
	LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);

	return (chRefineNPC ? chRefineNPC->GetGuild() : nullptr);
}

bool CHARACTER::IsRefineThroughGuild() const
{
	return GetRefineGuild() != nullptr;
}

int CHARACTER::ComputeRefineFee(int iCost, int iMultiply) const
{
	CGuild* pGuild = GetRefineGuild();
	if (pGuild)
	{
		if (pGuild == GetGuild())
			return iCost * iMultiply * 9 / 10;

		// 다른 제국 사람이 시도하는 경우 추가로 3배 더
		LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);
		if (chRefineNPC && chRefineNPC->GetEmpire() != GetEmpire())
			return iCost * iMultiply * 3;

		return iCost * iMultiply;
	}
	else
		return iCost;
}

void CHARACTER::PayRefineFee(int iTotalMoney)
{
	int iFee = iTotalMoney / 10;
	CGuild* pGuild = GetRefineGuild();

	int iRemain = iTotalMoney;

	if (pGuild)
	{
		// 자기 길드이면 iTotalMoney에 이미 10%가 제외되어있다
		if (pGuild != GetGuild())
		{
			pGuild->RequestDepositMoney(this, iFee);
			iRemain -= iFee;
		}
	}

	PointChange(POINT_GOLD, -iRemain);
}
// END_OF_ADD_REFINE_BUILDING

//Hack 방지를 위한 체크.
bool CHARACTER::IsHack(bool bSendMsg, bool bCheckShopOwner, int limittime)
{
	const int iPulse = thecore_pulse();

	if (test_server)
		bSendMsg = true;

	//창고 연후 체크
	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("창고를 연후 %d초 이내에는 다른곳으로 이동할수 없습니다."), limittime);

		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(limittime));
		return true;
	}

	//거래관련 창 체크
	if (bCheckShopOwner)
	{
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || (m_bSashCombination) || (m_bSashAbsorption) || (m_bChangeLook) || (m_bAuraRefine) || (m_bAuraAbsorption))
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 다른곳으로 이동,종료 할수 없습니다"));

			return true;
		}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
		if (IsCombOpen())
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 다른곳으로 이동,종료 할수 없습니다"));

			return true;
		}
#endif
	}
	else
	{
		if (GetExchange() || GetMyShop() || IsOpenSafebox() || IsCubeOpen() || (m_bSashCombination) || (m_bSashAbsorption) || (m_bChangeLook) || (m_bAuraRefine) || (m_bAuraAbsorption))
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 다른곳으로 이동,종료 할수 없습니다"));

			return true;
		}

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
		if (IsCombOpen())
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 다른곳으로 이동,종료 할수 없습니다"));

			return true;
		}
#endif
	}

	//PREVENT_PORTAL_AFTER_EXCHANGE
	//교환 후 시간체크
	if (iPulse - GetExchangeTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래 후 %d초 이내에는 다른지역으로 이동 할 수 없습니다."), limittime);
		return true;
	}
	//END_PREVENT_PORTAL_AFTER_EXCHANGE

	//PREVENT_ITEM_COPY
	if (iPulse - GetMyShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래 후 %d초 이내에는 다른지역으로 이동 할 수 없습니다."), limittime);
		return true;
	}

	if (iPulse - GetRefineTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 개량후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), limittime);
		return true;
	}
	//END_PREVENT_ITEM_COPY

	return false;
}

void CHARACTER::Say(const std::string & s)
{
	struct ::packet_script packet_script;

	packet_script.header = HEADER_GC_SCRIPT;
	packet_script.skin = 1;
	packet_script.src_size = s.size();
	packet_script.size = packet_script.src_size + sizeof(struct packet_script);

	TEMP_BUFFER buf;

	buf.write(&packet_script, sizeof(struct packet_script));
	buf.write(&s[0], s.size());

	if (IsPC())
	{
		GetDesc()->Packet(buf.read_peek(), buf.size());
	}
}
//------------------------------------------------
void CHARACTER::UpdateDepositPulse()
{
	m_deposit_pulse = thecore_pulse() + PASSES_PER_SEC(60 * 5);	// 5분
}

bool CHARACTER::CanDeposit() const
{
	return (m_deposit_pulse == 0 || (m_deposit_pulse < thecore_pulse()));
}
//------------------------------------------------

ESex GET_SEX(LPCHARACTER ch)
{
	switch (ch->GetRaceNum())
	{
	case MAIN_RACE_WARRIOR_M:
	case MAIN_RACE_SURA_M:
	case MAIN_RACE_ASSASSIN_M:
	case MAIN_RACE_SHAMAN_M:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case MAIN_RACE_WOLFMAN_M:
#endif
		return SEX_MALE;

	case MAIN_RACE_ASSASSIN_W:
	case MAIN_RACE_SHAMAN_W:
	case MAIN_RACE_WARRIOR_W:
	case MAIN_RACE_SURA_W:
		return SEX_FEMALE;
	}

	/* default sex = male */
	return SEX_MALE;
}

int CHARACTER::GetHPPct() const
{
	if (GetMaxHP() <= 0)
		return 0;
	return (GetHP() * 100) / GetMaxHP();
}

bool CHARACTER::IsBerserk() const
{
	if (m_pkMobInst != nullptr)
		return m_pkMobInst->m_IsBerserk;
	else
		return false;
}

void CHARACTER::SetBerserk(bool mode)
{
	if (m_pkMobInst != nullptr)
		m_pkMobInst->m_IsBerserk = mode;
}

bool CHARACTER::IsGodSpeed() const
{
	if (m_pkMobInst != nullptr)
	{
		return m_pkMobInst->m_IsGodSpeed;
	}
	else
	{
		return false;
	}
}

void CHARACTER::SetGodSpeed(bool mode)
{
	if (m_pkMobInst != nullptr)
	{
		m_pkMobInst->m_IsGodSpeed = mode;

		if (mode == true)
		{
			SetPoint(POINT_ATT_SPEED, 250);
		}
		else
		{
			SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		}
	}
}

bool CHARACTER::IsDeathBlow() const
{
	if (number(1, 100) <= m_pkMobData->m_table.bDeathBlowPoint)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct FFindReviver
{
	FFindReviver()
	{
		pChar = nullptr;
		HasReviver = false;
	}

	void operator() (LPCHARACTER ch)
	{
		if (ch->IsMonster() != true)
		{
			return;
		}

		if (ch->IsReviver() == true && pChar != ch && ch->IsDead() != true)
		{
			if (number(1, 100) <= ch->GetMobTable().bRevivePoint)
			{
				HasReviver = true;
				pChar = ch;
			}
		}
	}

	LPCHARACTER pChar;
	bool HasReviver;
};

bool CHARACTER::HasReviverInParty() const
{
	LPPARTY party = GetParty();

	if (party != nullptr)
	{
		if (party->GetMemberCount() == 1) return false;

		FFindReviver f;
		party->ForEachMemberPtr(f);
		return f.HasReviver;
	}

	return false;
}

bool CHARACTER::IsRevive() const
{
	if (m_pkMobInst != nullptr)
	{
		return m_pkMobInst->m_IsRevive;
	}

	return false;
}

void CHARACTER::SetRevive(bool mode)
{
	if (m_pkMobInst != nullptr)
	{
		m_pkMobInst->m_IsRevive = mode;
	}
}

void CHARACTER::GoHome()
{
	WarpSet(EMPIRE_START_X(GetEmpire()), EMPIRE_START_Y(GetEmpire()));
}

void CHARACTER::GotoWarpNPC()
{
	if (GetMapIndex() == 1)
	{
		DWORD x = 465158;
		DWORD y = 958539;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 3)
	{
		DWORD x = 357265;
		DWORD y = 876680;
		int WarpNpcMapIndex = GetMapIndex();

		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 21)
	{
		DWORD x = 51979;
		DWORD y = 153693;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 23)
	{
		DWORD x = 136957;
		DWORD y = 240260;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 41)
	{
		DWORD x = 964891;
		DWORD y = 273431;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 43)
	{
		DWORD x = 867323;
		DWORD y = 240349;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 67)
	{
		DWORD x = 303452;
		DWORD y = 27003;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 68)
	{
		DWORD x = 303452;
		DWORD y = 27003;
		int WarpNpcMapIndex = 67;
		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 64)
	{
		DWORD x = 335869;
		DWORD y = 755614;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 104)
	{
		DWORD x = 296743;
		DWORD y = 547455;
		int WarpNpcMapIndex = 63;
		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 72)
	{
		DWORD x = 335869;
		DWORD y = 755614;
		int WarpNpcMapIndex = 64;
		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 73)
	{
		DWORD x = 335869;
		DWORD y = 755614;
		int WarpNpcMapIndex = 64;
		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 61)
	{
		DWORD x = 436452;
		DWORD y = 215524;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 63)
	{
		DWORD x = 296743;
		DWORD y = 547455;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 62)
	{
		DWORD x = 600801;
		DWORD y = 687531;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 70)
	{
		DWORD x = 296743;
		DWORD y = 547455;
		int WarpNpcMapIndex = 63;
		WarpSet(x, y, WarpNpcMapIndex);
	}

	else if (GetMapIndex() == 65)
	{
		DWORD x = 594632;
		DWORD y = 133317;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}
	else if (GetMapIndex() == 304)
	{
		DWORD x = 1137616;
		DWORD y = 1523729;
		int WarpNpcMapIndex = GetMapIndex();
		Show(WarpNpcMapIndex, x, y, 0);
		Stop();

		//		WarpSet(x, y, WarpNpcMapIndex);
	}
	else
		return;
}

void CHARACTER::SendGuildName(CGuild * pGuild)
{
	if (nullptr == pGuild) return;

	DESC * desc = GetDesc();

	if (nullptr == desc) return;
	if (m_known_guild.find(pGuild->GetID()) != m_known_guild.end()) return;

	m_known_guild.insert(pGuild->GetID());

	TPacketGCGuildName	pack;
	memset(&pack, 0x00, sizeof(pack));

	pack.header = HEADER_GC_GUILD;
	pack.subheader = GUILD_SUBHEADER_GC_GUILD_NAME;
	pack.size = sizeof(TPacketGCGuildName);
	pack.guildID = pGuild->GetID();
	memcpy(pack.guildName, pGuild->GetName(), GUILD_NAME_MAX_LEN);

	desc->Packet(&pack, sizeof(pack));
}

void CHARACTER::SendGuildName(DWORD dwGuildID)
{
	SendGuildName(CGuildManager::instance().FindGuild(dwGuildID));
}

EVENTFUNC(destroy_when_idle_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("destroy_when_idle_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;
	if (ch == nullptr) { // <Factor>
		return 0;
	}

	if (ch->GetVictim())
	{
		return PASSES_PER_SEC(300);
	}

	sys_log(1, "DESTROY_WHEN_IDLE: %s", ch->GetName());

	ch->m_pkDestroyWhenIdleEvent = nullptr;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::StartDestroyWhenIdleEvent()
{
	if (m_pkDestroyWhenIdleEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkDestroyWhenIdleEvent = event_create(destroy_when_idle_event, info, PASSES_PER_SEC(300));
}

void CHARACTER::SetLastComboTime(DWORD time)
{
	m_dwLastComboTime = time;
}

DWORD CHARACTER::GetLastComboTime() const
{
	return m_dwLastComboTime;
}

void CHARACTER::SetValidComboInterval(int interval)
{
	m_iValidComboInterval = interval;
}

int CHARACTER::GetValidComboInterval() const
{
	return m_iValidComboInterval;
}

BYTE CHARACTER::GetComboIndex() const
{
	return m_bComboIndex;
}

void CHARACTER::SkipComboAttackByTime(int interval)
{
	m_dwSkipComboAttackByTime = get_dword_time() + interval;
}

DWORD CHARACTER::GetSkipComboAttackByTime() const
{
	return m_dwSkipComboAttackByTime;
}

void CHARACTER::ResetChatCounter()
{
	m_bChatCounter = 0;
}

BYTE CHARACTER::IncreaseChatCounter()
{
	return ++m_bChatCounter;
}

BYTE CHARACTER::GetChatCounter() const
{
	return m_bChatCounter;
}

// 말이나 다른것을 타고 있나?
bool CHARACTER::IsRiding() const
{
	return IsHorseRiding() || GetMountVnum();
}

bool CHARACTER::CanWarp()
{
	const int iPulse = thecore_pulse();
	const int limit_time = PASSES_PER_SEC(g_nPortalLimitTime);

	if ((iPulse - GetSafeboxLoadTime()) < limit_time)
		return false;

	if ((iPulse - GetExchangeTime()) < limit_time)
		return false;

	if ((iPulse - GetMyShopTime()) < limit_time)
		return false;

	if ((iPulse - GetRefineTime()) < limit_time)
		return false;

	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || (m_bSashCombination) || (m_bSashAbsorption) || (m_bChangeLook) || (m_bAuraRefine) || (m_bAuraAbsorption))
		return false;

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (IsCombOpen())
		return false;
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM
	if (IsOpenMailBox())
		return false;
#endif

	return true;
}

DWORD CHARACTER::GetNextExp() const
{
	if (PLAYER_EXP_TABLE_MAX < GetLevel())
		return 2500000000;
	else
		return exp_table[GetLevel()];
}

#ifdef ENABLE_BUFFI_SYSTEM
DWORD CHARACTER::SupportShamanGetNextExp() const
{
	if (PLAYER_EXP_TABLE_MAX < GetLevel())
		return 2500000000;
	else
		return exp_supportshaman_table[GetLevel()];
}
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
DWORD CHARACTER::PetGetNextExp() const
{
	if (IsNewPet()) {
		if (PLAYER_EXP_TABLE_MAX < GetLevel())
			return 2500000000;
		else
			return exppet_table[GetLevel()];
	}

	return 0;
}
#endif

int	CHARACTER::GetSkillPowerByLevel(int level, bool bMob) const
{
	return CTableBySkill::instance().GetSkillPowerByLevelFromType(GetJob(), GetSkillGroup(), MINMAX(0, level, SKILL_MAX_LEVEL), bMob);
}

#ifdef ENABLE_GIFTBOX_SYSTEM
#include <boost/algorithm/string.hpp>
EVENTFUNC(gift_refresh_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("gift_refresh_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (nullptr == ch || ch->IsNPC())
		return 0;
	ch->RefreshGift();
	return PASSES_PER_SEC(5 * 60);
}
void CHARACTER::StartRefreshGift()
{
	if (m_pkGiftRefresh)
		return;
	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;
	m_pkGiftRefresh = event_create(gift_refresh_event, info, PASSES_PER_SEC(1));
}
void SendGift(LPCHARACTER ch, TGiftItem item)
{
	char szSockets[1024] = { '\0' };
	char* tempSockets = szSockets;
	for (int soc = 0; soc < ITEM_SOCKET_MAX_NUM; soc++)
		tempSockets += sprintf(tempSockets, "%ld%s", item.alSockets[soc], (soc < ITEM_SOCKET_MAX_NUM - 1 ? "|" : ""));
	char szAttrs[1024] = { '\0' };
	char* tempAttrs = szAttrs;
	for (int attr = 0; attr < ITEM_ATTRIBUTE_MAX_NUM; attr++)
		tempAttrs += sprintf(tempAttrs, "%u,%d%s", item.aAttr[attr].bType, item.aAttr[attr].sValue, (attr < ITEM_ATTRIBUTE_MAX_NUM - 1 ? "|" : ""));
	if (item.vnum == 55002)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_item %d %d %d %d %d %s %s %d %s %s", item.id, item.vnum, item.count, item.pos, item.dwDateAdd, item.szFrom, item.szReason, item.transmutation, szSockets, szAttrs);
	else
		ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_item %d %d %d %d %d %s %s %d %s %s", item.id, item.vnum, item.count, item.pos, item.dwDateAdd, item.szFrom, item.szReason, item.transmutation, szSockets, szAttrs);
}
void CHARACTER::LoadGiftPage(int page)
{
	page = MINMAX(1, page, m_mapGiftGrid.size());
	ChatPacket(CHAT_TYPE_COMMAND, "gift_clear");
	GIFT_MAP::iterator it = m_mapGiftGrid.find(page);
	if (it == m_mapGiftGrid.end())
		return;
	m_dwLastGiftPage = page;
	for (int i = 0; i < it->second.size(); i++)
		SendGift(this, it->second[i]);
	ChatPacket(CHAT_TYPE_COMMAND, "gift_load");
}
static CGrid * grid;
void CHARACTER::AddGiftGrid(int page)
{
	if (m_mapGiftGrid.find(page) == m_mapGiftGrid.end())
	{
		std::vector<TGiftItem> vec;
		m_mapGiftGrid.insert(std::make_pair(page, vec));
	}
}
int CHARACTER::AddGiftGridItem(int page, int size)
{
	AddGiftGrid(page);
	int iPos = grid->FindBlank(1, size);

	if (iPos < 0)
		return -1;

	if (!grid->IsEmpty(iPos, 1, size))
		return -1;

	grid->Put(iPos, 1, size);
	return iPos;
}
void CHARACTER::RefreshGift()
{
	char szSockets[1024] = { '\0' };
	char* tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i < ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char* tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i < ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT id,vnum,count,unix_timestamp(date_add),replace(reason,' ','_'),replace(`from`,' ','_'),transmutation,%s,%s from player_gift where  owner_id = %d and status = 'WAIT' ORDER BY date_add ", szSockets, szAttrs, GetPlayerID()));
	SQLResult * pRes = pkMsg->Get();
	m_mapGiftGrid.clear();
	grid = M2_NEW CGrid(15, 8);
	grid->Clear();
	int page = 1;
	if (pRes->uiNumRows > 0)
	{
		AddGiftGrid(page);
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
		{
			int col = 0;
			TGiftItem item;
			memset(&item, 0, sizeof(item));
			str_to_number(item.id, row[col++]);
			str_to_number(item.vnum, row[col++]);
			str_to_number(item.count, row[col++]);
			str_to_number(item.dwDateAdd, row[col++]);
			strlcpy(item.szReason, row[col++], sizeof(item.szReason) - 1);
			strlcpy(item.szFrom, row[col++], sizeof(item.szFrom) - 1);
			str_to_number(item.transmutation, row[col++]);
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
				str_to_number(item.alSockets[i], row[col++]);
			for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
			{
				str_to_number(item.aAttr[i].bType, row[col++]);
				str_to_number(item.aAttr[i].sValue, row[col++]);
			}

			const TItemTable* item_table = ITEM_MANAGER::instance().GetTable(item.vnum);
			if (!item_table)
				continue;
			int iPos = AddGiftGridItem(page, item_table->bSize);
			if (iPos < 0)
			{
				page++;
				grid->Clear();
				iPos = AddGiftGridItem(page, item_table->bSize);
				if (iPos < 0)
				{
					sys_err("iPos <0");
					break;
				}
			}
			item.pos = iPos;
			m_mapGiftGrid[page].push_back(item);
		}
		M2_DELETE(grid);
	}
	if (GetGiftPages() > 0)
		ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", GetGiftPages());
}
#endif

struct CheckShopPos
{
	LPCHARACTER m_ch;
	CheckShopPos(LPCHARACTER ch)
	{
		m_ch = ch;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->GetRaceNum() != 30000 && ch->GetRaceNum() != 30001 && ch->GetRaceNum() != 30002 && ch->GetRaceNum() != 30003 && ch->GetRaceNum() != 30004 && ch->GetRaceNum() != 30005 && ch->GetRaceNum() != 30006 && ch->GetRaceNum() != 30007 && ch->GetRaceNum() != 30008 && ch->GetRaceNum() != 30009 && ch->GetRaceNum() != 30010 && ch->GetRaceNum() != 30011 && ch->GetRaceNum() != 30012 && ch->GetRaceNum() != 30013 && ch->GetRaceNum() != 30014) //shop mob vnum
				return;

			if (DISTANCE_APPROX(ch->GetX() - m_ch->GetX(), ch->GetY() - m_ch->GetY()) < 400) //distance between shops
			{
				m_ch->SetShopValidPos(false);
			}
		}
	}
};

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
extern std::map<DWORD, DWORD> g_ShopIndexCount;
extern std::map<int, TShopCost> g_ShopCosts;
void CHARACTER::OpenMyShop(const char* c_pszSign, TShopItemTable * pTable, BYTE bItemCount, DWORD id, DWORD model)
{
#ifdef ENABLE_SHOP_DECORATION_SYSTEM
	if (model != 0 && !FindAffect(AFFECT_SHOP_DECO))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("KASMIR_PAKET_YOK"));
		return;
	}
#endif
	if (g_ShopCosts.find(id) == g_ShopCosts.end())
	{
		sys_log(0, "Shop days error %s %d", c_pszSign, id);
		return;
	}
	if (!CanHandleItem())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
		return;
	}
	LPSECTREE sectree = nullptr;
	sectree = GetSectree();
	if (sectree)
	{
		SetShopValidPos(true);

		CheckShopPos f(this);
		sectree->ForEachAround(f);

		if (!GetShopValidPos())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open a shop here (too close to other shop)."));
			return;
		}
	}
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cz_cannot_open_shop"));
		return;
	}
#endif
	//shops pos check
	if (g_bChannel != 1)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PLEASE_CHANGE_CHANNEL_ON_OFF_SHOP"));
		return;
	}
	if (GetMapIndex() != 1 && GetMapIndex() != 21 && GetMapIndex() != 41)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BU_HARITADA_PAZAR_KURAMAZSIN"));
		return;
	}
	if (GetGiftPages() > 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hediye kutusundan nesneleri geri almalisin."));
		return;
	}
	int days = g_ShopCosts[id].days;
	long long price = g_ShopCosts[id].price;
	if (price > 0 && GetGold() < price)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_NEED_MONEY"), price);
		return;
	}
	if (IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_HAVE_MOUNT"));
		return;
	}
	if (GetMyShop())
	{
		CloseMyShop();
		return;
	}
	int gShopMax = quest::CQuestManager::Instance().GetEventFlag("shop_max");
	if (m_mapshops.size() >= (gShopMax ? gShopMax : 2))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MAX_ERR"));
		return;
	}

	if (g_ShopIndexCount.count(GetMapIndex()) > 0)
	{
		int shop_max = g_ShopIndexCount[GetMapIndex()];
		bool block = false;

#ifdef SHOP_ONLY_ALLOWED_INDEX
		if (shop_max > 0)
		{
#else
		if (shop_max == 0)
			block = true;
		else {
#endif
			std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT map_index from player_shop WHERE channel=%d and status='OK' and map_index=%d", g_bChannel, GetMapIndex()));
			SQLResult* pRes = pkMsg->Get();
			if (pRes->uiNumRows >= shop_max)
				block = true;
		}
		if (block)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_MAP_MAX"));
			return;
		}
		}
#ifdef SHOP_ONLY_ALLOWED_INDEX
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_CANNOT_OPEN_HERE"));
		return;
	}
#endif

	if (GetMyShop())
	{
		CloseMyShop();
		return;
	}

	int64_t nTotalMoney = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
	int64_t nTotalCheque = 0;
#endif

	for (int n = 0; n < bItemCount; ++n)
	{
		nTotalMoney += static_cast<int64_t> ((pTable + n)->price);
#ifdef ENABLE_CHEQUE_SYSTEM
		nTotalCheque += static_cast<int64_t> ((pTable + n)->price_cheque);
#endif
	}

	nTotalMoney += static_cast<int64_t> (GetGold());
#ifdef ENABLE_CHEQUE_SYSTEM
	nTotalCheque += static_cast<int64_t> (GetCheque());
#endif

	if (GOLD_MAX <= nTotalMoney)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2 Milyardan fazla satisa yang koyamazsin."));
		return;
	}

#ifdef ENABLE_CHEQUE_SYSTEM
	if (CHEQUE_MAX <= nTotalCheque)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("999 Wondan yuksek won koyamazsin."));
		return;
	}
#endif

	if (IsCubeOpen() == true)
		Cube_close(this);

	if ((m_bSashCombination) || (m_bSashAbsorption))
		CloseSash();

	if (m_bChangeLook)
		ChangeLookWindow(false, true);

	if (IsCombOpen() == true)
		Comb_close(this);
	
#ifdef ENABLE_AURA_SYSTEM
	if ((m_bAuraRefine) || (m_bAuraAbsorption))
		CloseAura();
#endif

	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());
	if (pPC->IsRunning())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hareket halindeyken pazar kuramazsin!"));
		return;
	}
	std::vector<TShopItemTable*> map_shop;
	// MYSHOP_PRICE_LIST
#ifdef ENABLE_CHEQUE_SYSTEM
	std::map<DWORD, TItemPriceType> itemkind;
#else
	std::map<DWORD, DWORD> itemkind;  // 아이템 종류별 가격, first: vnum, second: 단일 수량 가격
#endif
	// END_OF_MYSHOP_PRICE_LIST
	for (BYTE i = 0; i < bItemCount; ++i)
	{
		// ANTI_GIVE, ANTI_MYSHOP check
		LPITEM pkItem = GetItem((pTable + i)->pos);

		if (pkItem)
		{
			const TItemTable* item_table = pkItem->GetProto();

			if (item_table && (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MYSHOP)))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PLEASE_CHECK_PUTTING_ITEMS"));
				return;
			}

			if (pkItem->IsEquipped())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YOU_CANNOT_EQUIPPED_ITEMS_PUT_SHOP"));
				return;
			}

			if (pkItem->isLocked())
				return;

			if (pkItem->GetOwner() != this)
				return;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			if (pkItem->IsSealed())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ruha baglanmis esyayi pazara koyamazsin."));
				return;
			}
#endif

			if (pkItem->IsBasicItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
				return;
			}

#ifdef ENABLE_CHEQUE_SYSTEM
			itemkind[pkItem->GetVnum()] = TItemPriceType((pTable + i)->price / pkItem->GetCount(), (pTable + i)->price_cheque);
#else
			itemkind[pkItem->GetVnum()] = (pTable + i)->price / pkItem->GetCount();
#endif
		}
		if (days != 0)
			map_shop.push_back(pTable++);
	}
	if (days != 0 && map_shop.size() == 0)
		return;
	char szName[256];
	DBManager::instance().EscapeString(szName, 256, c_pszSign, strlen(c_pszSign));
	m_stShopSign = szName;
	boost::replace_all(m_stShopSign, "%", "%%");

	if (CountSpecifyItem(71049) > 0)
	{
		TItemPriceListTable header;
		memset(&header, 0, sizeof(TItemPriceListTable));

		header.dwOwnerID = GetPlayerID();
		header.byCount = itemkind.size();

		size_t idx = 0;
		for (auto it = itemkind.begin(); it != itemkind.end(); ++it)
		{
#ifdef ENABLE_CHEQUE_SYSTEM
			header.aPriceInfo[idx].dwVnum = it->first;
			header.aPriceInfo[idx].price.dwPrice = it->second.dwPrice;
			header.aPriceInfo[idx].price.byChequePrice = it->second.byChequePrice;
#else
			header.aPriceInfo[idx].dwVnum = it->first;
			header.aPriceInfo[idx].dwPrice = it->second;
#endif
			idx++;
		}

		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_UPDATE, GetDesc()->GetHandle(), &header, sizeof(TItemPriceListTable));
	}

	if (m_stShopSign.length() > 30)
		m_stShopSign.resize(30);

	if (m_stShopSign.length() == 0)
		return;

	int npc = 30000;
	switch (model)
	{
		case 0:
			npc = 30000;
			break;
		case 1:
			npc = 30002;
			break;
		case 2:
			npc = 30005;
			break;
		case 3:
			npc = 30006;
			break;
		case 4:
			npc = 30007;
			break;
		case 5:
			npc = 30008;
			break;
		case 6:
			npc = 30009;
			break;
		case 7:
			npc = 30010;
			break;
		case 8:
			npc = 30011;
			break;
		case 9:
			npc = 30012;
			break;
		case 10:
			npc = 30013;
			break;
		case 11:
			npc = 30014;
			break;
	}

#ifdef STRING_PROTECTION
	if (CBanwordManager::instance().CheckString(m_stShopSign.c_str(), m_stShopSign.length()) != "")
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_SECURITY_STRING"));
		return;
	}
#else
	if (CBanwordManager::instance().CheckString(m_stShopSign.c_str(), m_stShopSign.length()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_BANWORD_STRING"));
		return;
	}
#endif

	if (m_pkExchange)
		m_pkExchange->Cancel();

	CloseSafebox();
	CShopManager::instance().StopShopping(this);
	CloseMyShop();

	if (price > 0)
		PointChange(POINT_GOLD, -price);

	if (days != 0)
	{
		CShopManager::instance().CreateOfflineShop(this, m_stShopSign.c_str(), map_shop, id, npc, GetLevel(), GetAlignment());
		ChatPacket(CHAT_TYPE_COMMAND, "ClosePrivateShopDialog");
		m_stShopSign.clear();
		return;
	}

	TPacketGCShopSign p;
	p.bHeader = HEADER_GC_SHOP_SIGN;
	p.dwVID = GetVID();
	strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));

	PacketAround(&p, sizeof(TPacketGCShopSign));

	m_pkMyShop = CShopManager::instance().CreatePCShop(this, pTable, bItemCount);

	if (!m_pkMyShop)
	{
		sys_err("Cannot open normal shop %s %s", GetName(), m_stShopSign.c_str());
		return;
	}

	if (IsPolymorphed() == true)
		RemoveAffect(AFFECT_POLYMORPH);

	if (GetHorse())
		HorseSummon(false, true);
	else if (GetMountVnum())
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
	}

	SetPolymorph(30000, true);
	ChatPacket(CHAT_TYPE_COMMAND, "ClosePrivateShopDialog");
}
void CHARACTER::SetShopSign(const char* name)
{
	m_stShopSign = name;
	TPacketGCShopSign p;
	p.bHeader = HEADER_GC_SHOP_SIGN;
	p.dwVID = GetVID();
	strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));
	PacketAround(&p, sizeof(TPacketGCShopSign));
}

TPrivShop CHARACTER::GetPrivShopTable(DWORD id)
{
	TPrivShop shop;
	memset(&shop, 0, sizeof(TPrivShop));
	PSHOP_MAP::iterator it = m_mapshops.find(id);
	if (it != m_mapshops.end())
		return it->second;
	return shop;
}
void CHARACTER::UpdatePrivShopTable(DWORD id, TPrivShop shop)
{
	if (m_mapshops.find(id) != m_mapshops.end())
		m_mapshops[id] = shop;
	SendShops();
}
void CHARACTER::RemovePrivShopTable(DWORD id)
{
	if (m_mapshops.find(id) != m_mapshops.end())
		m_mapshops.erase(m_mapshops.find(id));
	SendShops();
}
void CHARACTER::UpdateShopItems()
{
	LPSHOP npcshop = GetMyShop();
	LPCHARACTER owner = CHARACTER_MANAGER::instance().FindByPID(GetPrivShopOwner());
	if (!npcshop || !IsPrivShop())
		return;
	npcshop->RemoveGuests(owner);
	npcshop->SetLocked(true);
	npcshop->ClearItems();
	std::vector<TShopItemTable*> map_shop;
	char szSockets[1024] = { '\0' };
	char* tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i < ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char* tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i < ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}
	std::unique_ptr<SQLMsg> Msg(DBManager::instance().DirectQuery("SELECT id,vnum,count,display_pos,price,price_cheque,transmutation,%s,%s from player_shop_items where shop_id='%d'", szSockets, szAttrs, GetPrivShop()));
	SQLResult * Res = Msg->Get();
	if (Res->uiNumRows > 0)
	{
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(Res->pSQLResult)) != nullptr)
		{
			int col = 0;

			TShopItemTable* shop = new TShopItemTable;
			memset(shop, 0, sizeof(TShopItemTable));
			DWORD id = 0;
			str_to_number(id, row[col++]);
			str_to_number(shop->vnum, row[col++]);
			str_to_number(shop->count, row[col++]);
			str_to_number(shop->display_pos, row[col++]);
			//col++;
			str_to_number(shop->price, row[col++]);
			str_to_number(shop->price_cheque, row[col++]);

			const TItemTable* item_table = ITEM_MANAGER::instance().GetTable(shop->vnum);

			if (!item_table)
			{
				sys_err("Shop: no item table by item vnum #%d", shop->vnum);
				continue;
			}

			//LPITEM item = ITEM_MANAGER::instance().CreateItem(shop->vnum, shop->count, id, false, -1, true);
			LPITEM item = ITEM_MANAGER::instance().CreateItem(shop->vnum, shop->count, id, false, -1, true);

			if (item)
			{
				item->ClearAttribute();
				item->SetSkipSave(true); // sexsex
				item->SetRealID(id);
				DWORD trans;
				str_to_number(trans, row[col++]);
				item->SetTransmutation(trans);
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
				{
					DWORD soc;
					str_to_number(soc, row[col++]);
					item->SetSocket(s, soc, false);
				}
				for (int at = 0; at < ITEM_ATTRIBUTE_MAX_NUM; at++)
				{
					DWORD attr;
					long val;
					str_to_number(attr, row[col++]);
					str_to_number(val, row[col++]);
					item->SetForceAttribute(at, attr, val);
				}

				////////////////////////////////////////////////////////
				if (item->IsDragonSoul())
				{
					for (int i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
					{
						if (nullptr != GetDragonSoulInventoryItem(i) && GetDragonSoulInventoryItem(i)->GetRealID() == id)
							GetDragonSoulInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if(item->IsUpgradeItem())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetUpgradeItemsInventoryItem(i) && GetUpgradeItemsInventoryItem(i)->GetRealID() == id)
							GetUpgradeItemsInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if(item->IsSkillBook())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetSkillBookInventoryItem(i) && GetSkillBookInventoryItem(i)->GetRealID() == id)
							GetSkillBookInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if(item->IsStone())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetStoneItemsInventoryItem(i) && GetStoneItemsInventoryItem(i)->GetRealID() == id)
							GetStoneItemsInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if(item->IsChest())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetChestItemsInventoryItem(i) && GetChestItemsInventoryItem(i)->GetRealID() == id)
							GetChestItemsInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if (item->IsAttr())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetAttrItemsInventoryItem(i) && GetAttrItemsInventoryItem(i)->GetRealID() == id)
							GetAttrItemsInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else if (item->IsFlower())
				{
					for (int i = 0; i < 225; ++i)
					{
						if (nullptr != GetFlowerItemsInventoryItem(i) && GetFlowerItemsInventoryItem(i)->GetRealID() == id)
							GetFlowerItemsInventoryItem(i)->RemoveFromCharacter();
					}
				}
				else
				{
					for (int i = 0; i < GetExtendInvenMax(); ++i)
					{
						if (nullptr != GetInventoryItem(i) && GetInventoryItem(i)->GetRealID() == id)
							GetInventoryItem(i)->RemoveFromCharacter();
					}
				}
				////////////////////////////////////////////////////////
				//----------------------------------------------------//
				////////////////////////////////////////////////////////
				int empty_pos;
				if (item->IsDragonSoul())
				{
					empty_pos = GetEmptyDragonSoulInventory(item);
					shop->pos.window_type = DRAGON_SOUL_INVENTORY;
				}
				else if(item->IsUpgradeItem())
				{
					empty_pos = GetEmptyUpgradeItemsInventory(item->GetSize());
					shop->pos.window_type = UPGRADE_ITEMS_INVENTORY;
				}
				else if(item->IsSkillBook())
				{
					empty_pos = GetEmptySkillBookInventory(item->GetSize());
					shop->pos.window_type = SKILL_BOOK_INVENTORY;
				}
				else if(item->IsStone())
				{
					empty_pos = GetEmptyStoneInventory(item->GetSize());
					shop->pos.window_type = STONE_ITEMS_INVENTORY;
				}
				else if(item->IsChest())
				{
					empty_pos = GetEmptyChestInventory(item->GetSize());
					shop->pos.window_type = CHEST_ITEMS_INVENTORY;
				}
				else if(item->IsAttr())
				{
					empty_pos = GetEmptyAttrInventory(item->GetSize());
					shop->pos.window_type = ATTR_ITEMS_INVENTORY;
				}
				else if(item->IsAttr())
				{
					empty_pos = GetEmptyFlowerInventory(item->GetSize());
					shop->pos.window_type = FLOWER_ITEMS_INVENTORY;
				}
				else
				{
					empty_pos = GetEmptyInventory(item->GetSize());
					shop->pos.window_type = INVENTORY;
				}

				shop->pos.cell = empty_pos;

				if (empty_pos < 0)
				{
					sys_err("no empty position in npc inventory");
					continue;
				}
				////////////////////////////////////////////////////////

				if (item->IsDragonSoul())
					item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
				else if (item->IsUpgradeItem())
					item->AddToCharacter(this, TItemPos(UPGRADE_ITEMS_INVENTORY, empty_pos));
				else if (item->IsSkillBook())
					item->AddToCharacter(this, TItemPos(SKILL_BOOK_INVENTORY, empty_pos));
				else if (item->IsStone())
					item->AddToCharacter(this, TItemPos(STONE_ITEMS_INVENTORY, empty_pos));
				else if (item->IsChest())
					item->AddToCharacter(this, TItemPos(CHEST_ITEMS_INVENTORY, empty_pos));
				else if (item->IsAttr())
					item->AddToCharacter(this, TItemPos(ATTR_ITEMS_INVENTORY, empty_pos));
				else if (item->IsFlower())
					item->AddToCharacter(this, TItemPos(FLOWER_ITEMS_INVENTORY, empty_pos));
				else
					item->AddToCharacter(this, TItemPos(INVENTORY, empty_pos));
			}
			else
			{
				sys_err("%d is not item", shop->vnum);
				continue;
			}
			map_shop.push_back(shop);
		}
	}

	if (map_shop.size() == 0 || map_shop.size() > SHOP_HOST_ITEM_MAX_NUM)
	{
		DeleteMyShop();
		return;
	}
	npcshop->SetPrivShopItems(map_shop);
	npcshop->SetLocked(false);
	for (int i = 0; i < SHOP_HOST_ITEM_MAX_NUM; i++)
		npcshop->BroadcastUpdateItem(i);

	if (owner)
	{
		owner->LoadPrivShops();
		owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_EDIT_SUCCESS"));
	}
}
void CHARACTER::LoadPrivShops()
{
	m_mapshops.clear();
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT id,shop_vid,replace(replace(name,' ','\\\\'),'#','_'),gold,won,item_count,(select count(id) from player_shop_items where shop_id=player_shop.id),DATEDIFF(date_close,now()) AS days,UNIX_TIMESTAMP(date_close) from player_shop WHERE player_id=%d", GetPlayerID()));
	SQLResult* pRes = pkMsg->Get();
	if (pRes->uiNumRows > 0)
	{
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
		{
			int col = 0;
			TPrivShop shop;
			memset(&shop, 0, sizeof(TPrivShop));
			str_to_number(shop.shop_id, row[col++]);
			str_to_number(shop.shop_vid, row[col++]);
			strlcpy(shop.szSign, row[col++], sizeof(shop.szSign));
			str_to_number(shop.gold, row[col++]);
			str_to_number(shop.won, row[col++]);
			str_to_number(shop.item_count, row[col++]);
			str_to_number(shop.rest_count, row[col++]);
			str_to_number(shop.days, row[col++]);
			str_to_number(shop.date_close, row[col++]);
			m_mapshops.insert(std::make_pair(shop.shop_id, shop));
		}
	}
	SendShops();
}
void CHARACTER::SendShops(bool isGm)
{
	ChatPacket(CHAT_TYPE_COMMAND, "shop_clear");
	PSHOP_MAP::iterator it = m_mapshops.begin();
	while (it != m_mapshops.end())
	{
		ChatPacket(CHAT_TYPE_COMMAND, "shop_add %d %d %s %lld %d %d %d %d %d", it->second.shop_id, it->second.shop_vid, it->second.szSign, it->second.gold, it->second.won, it->second.item_count, it->second.rest_count, it->second.days, it->second.date_close);
		it++;
	}
}
void CHARACTER::SendShopCost()
{
	extern std::map<int, TShopCost> g_ShopCosts;
	ChatPacket(CHAT_TYPE_COMMAND, "shop_cost_clear");
	std::map<int, TShopCost>::iterator it = g_ShopCosts.begin();
	while (it != g_ShopCosts.end())
	{
		ChatPacket(CHAT_TYPE_COMMAND, "shop_cost %d %d %d %lld", it->first, it->second.days, it->second.time, it->second.price);
		it++;
	}
}
void CHARACTER::OpenShop(DWORD id, const char* name, DWORD npc, DWORD level, DWORD align, bool onboot)
{
	if (GetMyShop())
	{
		CloseMyShop();
		return;
	}
	if (IsPC())
		return;
	char szSockets[1024] = { '\0' };
	char* tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i < ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char* tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i < ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT id,vnum,count,display_pos,price,price_cheque,transmutation,%s,%s from player_shop_items where shop_id='%d'", szSockets, szAttrs, id));

	SQLResult * pRes = pkMsg->Get();
	BYTE bItemCount = pRes->uiNumRows;

	TShopItemTable * m_pShopTable = new TShopItemTable[bItemCount];
	memset(&m_pShopTable[0], 0, sizeof(TShopItemTable) * bItemCount);

	std::vector<TShopItemTable*> map_shop;

	if (bItemCount > 0)
	{
		bItemCount = 0;
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
		{
			int col = 0;

			TShopItemTable* shop = new TShopItemTable;
			memset(shop, 0, sizeof(TShopItemTable));
			DWORD id;
			shop->pos.window_type = INVENTORY;
			str_to_number(id, row[col++]);
			str_to_number(shop->vnum, row[col++]);
			str_to_number(shop->count, row[col++]);
			str_to_number(shop->display_pos, row[col++]);
			//col++;
			str_to_number(shop->price, row[col++]);
			str_to_number(shop->price_cheque, row[col++]);

			const TItemTable* item_table = ITEM_MANAGER::instance().GetTable(shop->vnum);

			if (!item_table)
			{
				sys_err("Shop: no item table by item vnum #%d", shop->vnum);
				continue;
			}

			int emptynpcpos = shop->pos.cell = GetEmptyInventory(item_table->bSize);

			if (-1 == emptynpcpos)
			{
				sys_err("no empty position in npc inventory");
				return;
			}

			LPITEM item = ITEM_MANAGER::instance().CreateItem(shop->vnum, shop->count, 0, false, -1, true);

			if (item)
			{
				item->ClearAttribute();
				item->SetSkipSave(true);
				item->SetRealID(id);
				DWORD trans;
				str_to_number(trans, row[col++]);
				item->SetTransmutation(trans);
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
				{
					DWORD soc;
					str_to_number(soc, row[col++]);
					item->SetSocket(s, soc, false);
				}
				for (int at = 0; at < ITEM_ATTRIBUTE_MAX_NUM; at++)
				{
					DWORD attr;
					long val;
					str_to_number(attr, row[col++]);
					str_to_number(val, row[col++]);
					item->SetForceAttribute(at, attr, val);
				}

				item->AddToCharacter(this, shop->pos);
			}
			else
			{
				sys_err("%d is not item", shop->vnum);
				continue;
			}

			map_shop.push_back(shop);
			++bItemCount;
		}
	}

	if ((bItemCount == 0 && !onboot) || bItemCount > SHOP_HOST_ITEM_MAX_NUM)
		return;

	m_stShopSign = name;

	if (m_stShopSign.length() > 30)
		m_stShopSign.resize(30);
	if (m_stShopSign.length() == 0)
		return;

	TPacketGCShopSign p;

	p.bHeader = HEADER_GC_SHOP_SIGN;
	p.dwVID = GetVID();
	strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));
	PacketAround(&p, sizeof(TPacketGCShopSign));

	m_pkMyShop = CShopManager::instance().CreateNPCShop(this, map_shop);
}
void CHARACTER::DeleteMyShop()
{
	if (GetMyShop() && GetPrivShop())
	{
		std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("SELECT gold,won FROM player_shop where id='%d'", GetPrivShop()));
		SQLResult* pRes = pkMsg->Get();
		if (pRes->uiNumRows > 0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != nullptr)
			{
				long long gold;
				str_to_number(gold, row[0]);
				long long won;
				str_to_number(won, row[1]);
				if (gold > 0)
				{
					char query[1024];
					sprintf(query, "INSERT INTO player_gift SET \
								   					owner_id=%d,vnum=1,count='%s',reason='%s',`from`='%s',status='WAIT',date_add=NOW()",
						GetPrivShopOwner(), row[0], LC_TEXT("SHOP_REASON"), GetName());
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(query));
				}
				if (won > 0)
				{
					char query[1024];
					sprintf(query, "INSERT INTO player_gift SET \
								   				owner_id=%d,vnum=50027,count='1',socket0='%lld',reason='%s',`from`='%s',status='WAIT',date_add=NOW()",
						GetPrivShopOwner(), won, LC_TEXT("SHOP_REASON"), GetName());
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(query));
				}
			}
			GetMyShop()->GetItems();
			LPCHARACTER owner = CHARACTER_MANAGER::instance().FindByPID(GetPrivShopOwner());
			if (owner)
			{
#ifdef ENABLE_GIFTBOX_SYSTEM
				owner->RefreshGift();
#endif
				owner->RemovePrivShopTable(GetPrivShop());
				owner->SendShops();
				owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_SUCCESS_CLOSE"));
			}
			else {
				TPacketShopClose packet;
				packet.shop_id = GetPrivShop();
				packet.pid = GetPrivShopOwner();
				db_clientdesc->DBPacket(HEADER_GD_SHOP_CLOSE, 0, &packet, sizeof(packet));
			}

			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("delete from player_shop where id='%d'", GetPrivShop()));
			CloseMyShop();
		}
		return;
	}
	M2_DESTROY_CHARACTER(this);
}

EVENTFUNC(RefreshShopEvent)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("ishop_refresh_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (nullptr == ch)
		return 0;

	ch->SendShops();
	ch->SendShopCost();
	if (ch->GetGiftPages() > 0)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", ch->GetGiftPages());
	return PASSES_PER_SEC(10);
}
void CHARACTER::StartRefreshShopEvent()
{
	if (m_pkRefreshShopEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkRefreshShopEvent = event_create(RefreshShopEvent, info, PASSES_PER_SEC(1));
}
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
void CHARACTER::ChangeLookWindow(bool bOpen, bool bRequest, bool bMount)
#else
void CHARACTER::ChangeLookWindow(bool bOpen, bool bRequest)
#endif
{
	if ((bOpen) && (isChangeLookOpened()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] The window is already opened."));
		return;
	}

	if ((!bOpen) && (!isChangeLookOpened()))
	{
		if (!bRequest)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] The window is not opened."));

		return;
	}
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_GC_CL;
	sPacket.subheader = CL_SUBHEADER_OPEN;
	sPacket.subheader = bOpen == true ? CL_SUBHEADER_OPEN : CL_SUBHEADER_CLOSE;
	sPacket.dwCost = bOpen == true ? CL_TRANSMUTATION_PRICE : 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	sPacket.bMount = bMount;
#endif
	GetDesc()->Packet(&sPacket, sizeof(TPacketChangeLook));

	m_bChangeLook = bOpen;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	m_bMountChangeLook = bMount;
#endif
	ClearClWindowMaterials();
}

void CHARACTER::ClearClWindowMaterials()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetClWindowMaterials();
	for (int i = 0; i < CL_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			continue;

		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = nullptr;
	}
}

BYTE CHARACTER::CheckClEmptyMaterialSlot()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetClWindowMaterials();
	for (int i = 0; i < CL_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			return i;
	}

	return 255;
}

void CHARACTER::AddClMaterial(TItemPos tPos, BYTE bPos)
{
	if (!isChangeLookOpened())
		return;
	else if (bPos >= CL_WINDOW_MAX_MATERIALS)
	{
		if (bPos != 255)
			return;

		LPITEM pkScroll = GetItem(tPos);
		if (!pkScroll)
			return;
		//@fixme
		if (pkScroll->GetVnum() == CL_SCROLL_VNUM)
			bPos = 2;
		else
			bPos = CheckClEmptyMaterialSlot();
		//@fixme
		if (bPos >= CL_WINDOW_MAX_MATERIALS)
			return;
	}

	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (tPos.IsBeltInventoryPosition()))
		return;
	if (pkItem->IsEquipped())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot transmute an item while it is equipped."));
		return;
	}
	if ((bPos == 2 && pkItem->GetVnum() != CL_SCROLL_VNUM) || (bPos != 2 && pkItem->GetVnum() == CL_SCROLL_VNUM))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
		return;
	}
	if (pkItem->GetVnum() != CL_SCROLL_VNUM)
	{
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		if (m_bMountChangeLook == true)
		{
			if (!pkItem->IsMountItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
				return;
			}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			else if ((pkItem->IsSealed()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You can't add binded items."));
				return;
			}
#endif
#ifdef ENABLE_BASIC_ITEM_SYSTEM
			else if (pkItem->IsBasicItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
				return;
			}
#endif
			else if (pkItem->GetTransmutation() != 0)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You can't add items which are transmuted yet."));
				return;
			}
		}
		else
#endif
		{
			if ((pkItem->GetType() != ITEM_WEAPON) && (pkItem->GetType() != ITEM_ARMOR) && (pkItem->GetType() != ITEM_COSTUME) && (pkItem->GetType() != ITEM_UNIQUE) && (!pkItem->IsPetItem()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
				return;
			}
#ifdef ENABLE_QUIVER_SYSTEM
			else if ((pkItem->GetType() == ITEM_WEAPON) && ((pkItem->GetSubType() == WEAPON_ARROW) || (pkItem->GetSubType() == WEAPON_MOUNT_SPEAR) || (pkItem->GetSubType() == WEAPON_QUIVER)))
#else
			else if ((pkItem->GetType() == ITEM_WEAPON) && ((pkItem->GetSubType() == WEAPON_ARROW) || (pkItem->GetSubType() == WEAPON_MOUNT_SPEAR)))
#endif
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
				return;
			}
			else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_BODY))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
				return;
			}
			else if ((pkItem->GetType() == ITEM_COSTUME) && (pkItem->GetSubType() != COSTUME_BODY) && (pkItem->GetSubType() != COSTUME_HAIR) && (pkItem->GetSubType() != COSTUME_WEAPON))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] This item cannot be transmuted."));
				return;
			}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
			else if ((pkItem->IsSealed()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You can't add binded items."));
				return;
			}
#endif
#ifdef ENABLE_BASIC_ITEM_SYSTEM
			else if (pkItem->IsBasicItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
				return;
			}
#endif
			else if (pkItem->GetTransmutation() != 0)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You can't add items which are transmuted yet."));
				return;
			}
		}
	}

	LPITEM* pkItemMaterial;
	pkItemMaterial = GetClWindowMaterials();
	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;

	if (pkItemMaterial[bPos])
		return;

#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	if (m_bMountChangeLook)
	{
		if (bPos == 1)
		{
			bool bStop = true;
			if (pkItem->IsMountItem() && pkItemMaterial[0]->IsMountItem())
				bStop = false;

			if (pkItemMaterial[0]->GetOriginalVnum() == pkItem->GetOriginalVnum())
				bStop = true;
			
			if (bStop)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot submit this item."));
				return;
			}
		}
	}
	else
#endif
	{
		if (bPos == 1)
		{
			bool bStop = false;
			if ((pkItem->GetType() != pkItemMaterial[0]->GetType()))
			{
				if ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_BODY) && (pkItemMaterial[0]->GetType() == ITEM_ARMOR && pkItemMaterial[0]->GetSubType() == ARMOR_BODY))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_WEAPON) && (pkItemMaterial[0]->GetType() == ITEM_WEAPON))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_BODY) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_BODY))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_HAIR) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_HAIR))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_WEAPON) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_WEAPON))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_ARMOR && pkItem->GetSubType() == ARMOR_BODY) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_BODY))
					bStop = false;
				else if ((pkItem->GetType() == ITEM_WEAPON) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_WEAPON))
					bStop = false;
				else if ((pkItem->IsPetItem()) && pkItemMaterial[0]->IsPetItem())
					bStop = false;
				else
					bStop = true;
			}
			else
			{
				if (pkItemMaterial[0]->GetType() != pkItem->GetType())
					bStop = true;
				else if (pkItemMaterial[0]->GetSubType() != pkItem->GetSubType())
					bStop = true;
			}

			if (pkItemMaterial[0]->GetOriginalVnum() == pkItem->GetOriginalVnum())
				bStop = true;
			else if (((IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE)) && (!IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE))) || ((IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_MALE)) && (!IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_MALE))))
				bStop = true;
			else if ((pkItem->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR) && (!IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_WARRIOR)))
				bStop = true;
			else if ((pkItem->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN) && (!IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_ASSASSIN)))
				bStop = true;
			else if ((pkItem->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN) && (!IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_SHAMAN)))
				bStop = true;
			else if ((pkItem->GetAntiFlag() & ITEM_ANTIFLAG_SURA) && (!IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_SURA)))
				bStop = true;
			else if ((pkItem->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN) && (!IS_SET(pkItemMaterial[0]->GetAntiFlag(), ITEM_ANTIFLAG_WOLFMAN)))
				bStop = true;
			else if ((pkItem->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetType() == ITEM_COSTUME) && ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_WEAPON) && (pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_WEAPON)))
			{
				if (pkItem->GetValue(3) != pkItemMaterial[0]->GetValue(3))
					bStop = true;
			}
			// Yansitma Fix Suky
			else if ((pkItem->GetType() == ITEM_WEAPON && pkItemMaterial[0]->GetType() == ITEM_COSTUME) && ((pkItem->GetType() == ITEM_WEAPON && ((pkItemMaterial[0]->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetSubType() == COSTUME_WEAPON)))))
			{
				if (pkItem->GetType() != pkItemMaterial[0]->GetValue(3))
					bStop = true;			
			}
			else if ((pkItem->GetType() == ITEM_COSTUME && pkItemMaterial[0]->GetType() == ITEM_WEAPON) && ((pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_WEAPON) && ((pkItemMaterial[0]->GetType() == ITEM_WEAPON))))
			{
				if (pkItem->GetValue(3) != pkItemMaterial[0]->GetType())
					bStop = true;			
			}
			
			if (bStop)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot submit this item."));
				return;
			}
		}
	}

	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_GC_CL;
	sPacket.subheader = CL_SUBHEADER_ADD;
	sPacket.dwCost = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	GetDesc()->Packet(&sPacket, sizeof(TPacketChangeLook));
}

void CHARACTER::RemoveClMaterial(BYTE bPos)
{
	if (bPos >= CL_WINDOW_MAX_MATERIALS)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetClWindowMaterials();

	if (!pkItemMaterial[bPos])
		return;

	if (bPos == 1 || bPos == 2)
	{
		pkItemMaterial[bPos]->Lock(false);
		pkItemMaterial[bPos] = nullptr;
	}
	else
		ClearClWindowMaterials();

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_GC_CL;
	sPacket.subheader = CL_SUBHEADER_REMOVE;
	sPacket.dwCost = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	GetDesc()->Packet(&sPacket, sizeof(TPacketChangeLook));
}

void CHARACTER::RefineClMaterials()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetClWindowMaterials();
	if (!pkItemMaterial[0])
		return;
	else if (!pkItemMaterial[1])
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] Please submit the item you want to transmute."));
		return;
	}

	DWORD dwPrice = CL_TRANSMUTATION_PRICE;
	bool isNeedGold = true;
	if (pkItemMaterial[2] && pkItemMaterial[2]->GetVnum() == CL_SCROLL_VNUM)
		isNeedGold = false;
	if (isNeedGold == true)
	{
#ifdef ENABLE_CHEQUE_SYSTEM
		if (GetCheque() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You don't have enough Cheque."));
			return;
		}
#else
		if (GetGold() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You don't have enough Yang."));
			return;
		}
#endif
	}

	DWORD dwVnum = pkItemMaterial[1]->GetVnum();
	if (isNeedGold == false)
		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[2], "SPECIFY ITEM TRANSMUTED");
	else
#ifdef ENABLE_CHEQUE_SYSTEM
		PointChange(POINT_CHEQUE, -dwPrice);
#else
		PointChange(POINT_GOLD, -dwPrice);
#endif
	DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);
	ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "TRANSMUTED (SUCCESSFULLY)");

	pkItemMaterial[0]->SetTransmutation(dwVnum, true);
	ClearClWindowMaterials();

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketChangeLook sPacket;
	sPacket.header = HEADER_GC_CL;
	sPacket.subheader = CL_SUBHEADER_REFINE;
	sPacket.dwCost = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	GetDesc()->Packet(&sPacket, sizeof(TPacketChangeLook));
}

bool CHARACTER::CleanTransmutation(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_WEAPON) && (pkTarget->GetType() != ITEM_ARMOR) && (pkTarget->GetType() != ITEM_COSTUME))
		return false;
	else if ((pkTarget->IsSealed()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't remove the transmute because item is binded."));
		return false;
	}
	else if (pkTarget->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't remove the transmute because item is locked."));
		return false;
	}

	if (pkTarget->GetTransmutation() == 0)
		return false;

	pkTarget->SetTransmutation(0);
	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN TRANSMUTED)", pkTarget->GetName());
	return true;
}
#endif

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
void CHARACTER::UpdateCombatZoneRankings(const char* memberName, DWORD memberEmpire, DWORD memberPoints)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("INSERT INTO player.combat_zone_ranking_weekly (memberName, memberEmpire, memberPoints) VALUES('%s', '%d', '%d') ON DUPLICATE KEY UPDATE memberPoints = memberPoints + '%d';", memberName, memberEmpire, memberPoints, memberPoints));
	std::unique_ptr<SQLMsg> pkMsg(DBManager::instance().DirectQuery("INSERT INTO player.combat_zone_ranking_general (memberName, memberEmpire, memberPoints) VALUES('%s', '%d', '%d') ON DUPLICATE KEY UPDATE memberPoints = memberPoints + '%d';", memberName, memberEmpire, memberPoints, memberPoints));
}

BYTE CHARACTER::GetCombatZoneRank()
{
	if (GetDesc() != nullptr)
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT combat_zone_rank FROM player.player WHERE id = %u", GetPlayerID()));
		if (pMsg->Get()->uiNumRows == 0)
			return 0;
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		return atoi(row[0]);
	}

	return 0;
}

DWORD CHARACTER::GetRealCombatZonePoints()
{
	return CCombatZoneManager::Instance().GetValueByType(this, COMBAT_ZONE_GET_POINTS);
}

void CHARACTER::SetRealCombatZonePoints(DWORD iValue)
{
	DBManager::instance().Query("UPDATE player.player SET combat_zone_points = %d WHERE id = %u", iValue, GetPlayerID());
}
#endif

#ifdef ENABLE_ACCE_SYSTEM
void CHARACTER::OpenSash(bool bCombination)
{
	if (isSashOpened(bCombination))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The sash window it's already opened."));
		return;
	}

	if (bCombination)
	{
		if (m_bSashAbsorption)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the sash absorption window."));
			return;
		}

		m_bSashCombination = true;
	}
	else
	{
		if (m_bSashCombination)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the sash combine window."));
			return;
		}

		m_bSashAbsorption = true;
	}

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_OPEN;
	sPacket.bWindow = bCombination;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));

	ClearSashMaterials();
}

void CHARACTER::CloseSash()
{
	if ((!m_bSashCombination) && (!m_bSashAbsorption))
		return;

	bool bWindow = (m_bSashCombination == true ? true : false);

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_CLOSE;
	sPacket.bWindow = bWindow;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));

	if (bWindow)
		m_bSashCombination = false;
	else
		m_bSashAbsorption = false;

	ClearSashMaterials();
}

void CHARACTER::ClearSashMaterials()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			continue;

		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = nullptr;
	}
}

bool CHARACTER::SashIsSameGrade(long lGrade)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial[0])
		return false;

	bool bReturn = (pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD) == lGrade ? true : false);
	return bReturn;
}

DWORD CHARACTER::GetSashCombinePrice(long lGrade)
{
	DWORD dwPrice = 0;
	switch (lGrade)
	{
	case 2:
	{
		dwPrice = SASH_GRADE_2_PRICE;
	}
	break;
	case 3:
	{
		dwPrice = SASH_GRADE_3_PRICE;
	}
	break;
	case 4:
	{
		dwPrice = SASH_GRADE_4_PRICE;
	}
	break;
	default:
	{
		dwPrice = SASH_GRADE_1_PRICE;
	}
	break;
	}

	return dwPrice;
}

BYTE CHARACTER::CheckEmptyMaterialSlot()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			return i;
	}

	return 255;
}

bool Sash30Abs(DWORD vnum)
{
	switch(vnum)
	{
		case 85044:
		case 85054:
		case 85064:
		case 85074:
		case 86104:
		case 86108:
		case 86114:
		case 86118:
		case 86130:
		case 86126:
		case 86122:
			return true;
	}
	return false;
	
}

void CHARACTER::GetSashCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetSashMaterials();

	if (m_bSashCombination)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
			if (lVal == 4)
			{
				dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
				dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
				DWORD dwMaxAbsCalc;
				if (Sash30Abs(dwItemVnum) == true)
					dwMaxAbsCalc = (dwMinAbs + SASH_GRADE_4_ABS_RANGE > SASH_GRADE_NEW_ABS_30 ? SASH_GRADE_NEW_ABS_30 : (dwMinAbs + SASH_GRADE_4_ABS_RANGE));
				else
					dwMaxAbsCalc = (dwMinAbs + SASH_GRADE_4_ABS_RANGE > SASH_GRADE_4_ABS_MAX ? SASH_GRADE_4_ABS_MAX : (dwMinAbs + SASH_GRADE_4_ABS_RANGE));
				
				dwMaxAbs = dwMaxAbsCalc;
			}
			else
			{
				DWORD dwMaskVnum = pkItemMaterial[0]->GetOriginalVnum();
				TItemTable* pTable = ITEM_MANAGER::instance().GetTable(dwMaskVnum + 1);
				if (pTable)
					dwMaskVnum += 1;

				dwItemVnum = dwMaskVnum;
				switch (lVal)
				{
				case 2:
				{
					dwMinAbs = SASH_GRADE_3_ABS;
					dwMaxAbs = SASH_GRADE_3_ABS;
				}
				break;
				case 3:
				{
					dwMinAbs = SASH_GRADE_4_ABS_MIN;
					dwMaxAbs = SASH_GRADE_4_ABS_MAX_COMB;
				}
				break;
				default:
				{
					dwMinAbs = SASH_GRADE_2_ABS;
					dwMaxAbs = SASH_GRADE_2_ABS;
				}
				break;
				}
			}
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
	else
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
			dwMaxAbs = dwMinAbs;
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
}

void CHARACTER::AddSashMaterial(TItemPos tPos, BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
	{
		if (bPos == 255)
		{
			bPos = CheckEmptyMaterialSlot();
			if (bPos >= SASH_WINDOW_MAX_MATERIALS)
				return;
		}
		else
			return;
	}

	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	else if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (pkItem->IsEquipped()) || (tPos.IsBeltInventoryPosition()) || (pkItem->IsDragonSoul()))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_SASH) && (m_bSashCombination))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_SASH) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if (pkItem->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't add locked items."));
		return;
	}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	else if ((pkItem->IsSealed()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You can't add binded items."));
		return;
	}
#endif
	else if (pkItem->IsBasicItem())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return;
	}
	else if ((m_bSashCombination) && (bPos == 1) && (!SashIsSameGrade(pkItem->GetValue(SASH_GRADE_VALUE_FIELD))))
	{
		if (Sash30Abs(pkItem->GetVnum()) == true)
		{
			if ((m_bSashCombination) && (pkItem->GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_GRADE_NEW_ABS_30))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This sash got already maximum absorption chance."));
				return;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This sash got already maximum absorption chance."));
			return;
		}
	}
	else if ((m_bSashCombination) && (pkItem->GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_GRADE_4_ABS_MAX))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This sash got already maximum absorption chance."));
		return;
	}
	else if ((bPos == 1) && (m_bSashAbsorption))
	{
		if ((pkItem->GetType() != ITEM_WEAPON) && (pkItem->GetType() != ITEM_ARMOR))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can absorb just the bonuses from armors and weapons."));
			return;
		}
		else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_BODY))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can absorb just the bonuses from armors and weapons."));
			return;
		}
		else if (pkItem->IsRealTimeItem() == true && pkItem->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can absorb only non-timed weapon."));
			return;
		}
	}
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (m_bSashCombination))
		return;
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if ((pkItem->GetSocket(SASH_ABSORBED_SOCKET) > 0) && (bPos == 0) && (m_bSashAbsorption))
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;

	if (pkItemMaterial[bPos])
		return;

	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);

	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_ADDED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = GetSashCombinePrice(pkItem->GetValue(SASH_GRADE_VALUE_FIELD));
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = dwItemVnum;
	sPacket.dwMinAbs = dwMinAbs;
	sPacket.dwMaxAbs = dwMaxAbs;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

void CHARACTER::RemoveSashMaterial(BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();

	DWORD dwPrice = 0;

	if (bPos == 1)
	{
		if (pkItemMaterial[bPos])
		{
			pkItemMaterial[bPos]->Lock(false);
			pkItemMaterial[bPos] = nullptr;
		}

		if (pkItemMaterial[0])
			dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
	}
	else
		ClearSashMaterials();

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_REMOVED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = dwPrice;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

BYTE CHARACTER::CanRefineSashMaterials()
{
	BYTE bReturn = 0;
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if (m_bSashCombination)
	{
		for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
		{
			if (pkItemMaterial[i])
			{
				if ((pkItemMaterial[i]->GetType() == ITEM_COSTUME) && (pkItemMaterial[i]->GetSubType() == COSTUME_SASH))
					bReturn = 1;
				else
				{
					bReturn = 0;
					break;
				}
			}
			else
			{
				bReturn = 0;
				break;
			}
		}
	}
	else if (m_bSashAbsorption)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			if ((pkItemMaterial[0]->GetType() == ITEM_COSTUME) && (pkItemMaterial[0]->GetSubType() == COSTUME_SASH))
				bReturn = 2;
			else
				bReturn = 0;

			if ((pkItemMaterial[1]->GetType() == ITEM_WEAPON) || ((pkItemMaterial[1]->GetType() == ITEM_ARMOR) && (pkItemMaterial[1]->GetSubType() == ARMOR_BODY)))
				bReturn = 2;
			else
				bReturn = 0;

			if (pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET) > 0)
				bReturn = 0;
		}
		else
			bReturn = 0;
	}

	return bReturn;
}

void CHARACTER::RefineSashMaterials()
{
	BYTE bCan = CanRefineSashMaterials();
	if (bCan == 0)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();

	if (!pkItemMaterial[0] || !pkItemMaterial[1])
		return;

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
	DWORD dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));

	if (bCan == 1)
	{
		int iSuccessChance = 0;
		long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
		case 2:
		{
			iSuccessChance = SASH_COMBINE_GRADE_2;
		}
		break;
		case 3:
		{
			iSuccessChance = SASH_COMBINE_GRADE_3;
		}
		break;
		case 4:
		{
			iSuccessChance = SASH_COMBINE_GRADE_4;
		}
		break;
		default:
		{
			iSuccessChance = SASH_COMBINE_GRADE_1;
		}
		break;
		}

		if (GetGold() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough Yang."));
			return;
		}

		int iChance = number(1, 100);
		bool bSucces = (iChance <= iSuccessChance ? true : false);
		if (bSucces)
		{
			LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwItemVnum, 1, 0, false);
			if (!pkItem)
			{
				sys_err("%d can't be created.", dwItemVnum);
				return;
			}

			ITEM_MANAGER::CopyAllAttrTo(pkItemMaterial[0], pkItem);
			LogManager::instance().ItemLog(this, pkItem, "COMBINE SUCCESS", pkItem->GetName());
			DWORD dwAbs = (dwMinAbs == dwMaxAbs ? dwMinAbs : number(dwMinAbs + 1, dwMaxAbs));
			pkItem->SetSocket(SASH_ABSORPTION_SOCKET, dwAbs);
			pkItem->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET));

			PointChange(POINT_GOLD, -dwPrice);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

			WORD wCell = pkItemMaterial[0]->GetCell();
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[0], "COMBINE (REFINE SUCCESS)");
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE SUCCESS)");

			pkItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
			pkItem->AttrLog();

			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("New absorption rate: %d%"), dwAbs);
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Success."));

			EffectPacket(SE_EFFECT_SASH_SUCCEDED);
			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, pkItem->GetID(), 1, dwAbs, 1);

			ClearSashMaterials();
		}
		else
		{
			PointChange(POINT_GOLD, -dwPrice);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE FAIL)");

			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("New absorption rate: %d%"), pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET));
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Failed."));

			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, 0, 0, 0, 0);

			pkItemMaterial[1] = nullptr;
		}

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 0;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		if (bSucces)
			sPacket.dwMaxAbs = 100;
		else
			sPacket.dwMaxAbs = 0;

		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
	else
	{
		pkItemMaterial[1]->CopyAttributeTo(pkItemMaterial[0]);
		LogManager::instance().ItemLog(this, pkItemMaterial[0], "ABSORB (REFINE SUCCESS)", pkItemMaterial[0]->GetName());
		pkItemMaterial[0]->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[1]->GetOriginalVnum());
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (pkItemMaterial[0]->GetAttributeValue(i) < 0)
				pkItemMaterial[0]->SetForceAttribute(i, pkItemMaterial[0]->GetAttributeType(i), 0);
		}

		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "ABSORBED (REFINE SUCCESS)");

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemMaterial[0]);
		pkItemMaterial[0]->AttrLog();

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Success."));

		ClearSashMaterials();

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 255;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		sPacket.dwMaxAbs = 1;
		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
}

bool CHARACTER::CleanSashAttr(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_COSTUME) && (pkTarget->GetSubType() != COSTUME_SASH))
		return false;

	if (pkTarget->GetSocket(SASH_ABSORBED_SOCKET) <= 0)
		return false;

	pkTarget->SetSocket(SASH_ABSORBED_SOCKET, 0);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		pkTarget->SetForceAttribute(i, 0, 0);

	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN ATTR)", pkTarget->GetName());
	return true;
}
#endif

#ifdef ENABLE_CHANNEL_SWITCH_SYSTEM
bool CHARACTER::SwitchChannel(long newAddr, WORD newPort)
{
	if (!IsPC() || !GetDesc() || !CanWarp())
		return false;

	long x = GetX();
	long y = GetY();

	long lAddr = newAddr;
	long lMapIndex = GetMapIndex();
	WORD wPort = newPort;

	// If we currently are in a dungeon.
	if (lMapIndex >= 10000)
	{
		sys_err("Invalid change channel request from dungeon %d!", lMapIndex);
		return false;
	}

	// If we are on CH99.
	if (g_bChannel == 99)
	{
		sys_err("%s attempted to change channel from CH99, ignoring req.", GetName());
		return false;
	}

	Stop();
	Save();

	if (GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();
		EncodeRemovePacket(this);
	}

	m_lWarpMapIndex = lMapIndex;
	m_posWarp.x = x;
	m_posWarp.y = y;

	// TODO: This log message should mention channel we are changing to instead of port.
	sys_log(0, "ChangeChannel %s, %ld %ld map %ld to port %d", GetName(), x, y, GetMapIndex(), wPort);

	TPacketGCWarp p;

	p.bHeader = HEADER_GC_WARP;
	p.lX = x;
	p.lY = y;
	p.lAddr = lAddr;
	p.wPort = wPort;

	GetDesc()->Packet(&p, sizeof(p));

	char buf[256];
	// TODO: This log message should mention channel we are changing to instead of port
	snprintf(buf, sizeof(buf), "%s Port%d Map%ld x%ld y%ld", GetName(), wPort, GetMapIndex(), x, y);
	LogManager::instance().CharLog(this, 0, "CHANGE_CH", buf);

	return true;
}

EVENTINFO(switch_channel_info)
{
	DynamicCharacterPtr ch;
	int secs;
	long newAddr;
	WORD newPort;
	switch_channel_info()
		: ch(),
		secs(0),
		newAddr(0),
		newPort(0)
	{
	}
};

EVENTFUNC(switch_channel)
{
	switch_channel_info* info = dynamic_cast<switch_channel_info*>(event->info);
	if (!info)
	{
		sys_err("No switch channel event info!");
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (!ch)
	{
		sys_err("No char to work on for the switch.");
		return 0;
	}

	if (!ch->GetDesc())
		return 0;

	if (info->secs > 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Channel switch in %d seconds."), info->secs);
		--info->secs;
		return PASSES_PER_SEC(1);
	}

	ch->SwitchChannel(info->newAddr, info->newPort);
	ch->m_pkTimedEvent = nullptr;
	return 0;
}

bool CHARACTER::StartChannelSwitch(long newAddr, WORD newPort)
{
	if (IsHack(false, true, 10))
		return false;

	switch_channel_info* info = AllocEventInfo<switch_channel_info>();
	info->ch = this;
	info->secs = CanWarp() && !IsPosition(POS_FIGHTING) ? 3 : 10;
	info->newAddr = newAddr;
	info->newPort = newPort;

	m_pkTimedEvent = event_create(switch_channel, info, 1);
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Channel switch starting."));
	return true;
}
#endif

#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
EVENTINFO(TViewEquipEventInfo)
{
	DWORD	dwGuestPID;
	DWORD	dwTargetPID;

	TViewEquipEventInfo() : dwGuestPID(0), dwTargetPID(0) { }
};

EVENTFUNC(view_equip_request_event)
{
	TViewEquipEventInfo* info = dynamic_cast<TViewEquipEventInfo*>(event->info);

	if (info == nullptr)
	{
		sys_err("view_equip_request_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->dwGuestPID);

	if (ch)
	{
		sys_log(0, "ViewEquipRequestEvent %s", ch->GetName());
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ViewEquipRequestDenied");
		ch->SetViewEquipRequestEvent(nullptr);
	}

	return 0;
}

bool CHARACTER::RequestToViewEquip(LPCHARACTER pTarget)
{
	if (!pTarget)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't make a request because target player is not online."));
		return false;
	}

	if (GetGMLevel() > GM_PLAYER)
	{
		pTarget->SendEquipment(this);
		return true;
	}

	if (m_pkViewEquipRequestEvent)
		return false;

	if (!IsPC() || !pTarget->IsPC())
		return false;

	TViewEquipEventInfo * info = AllocEventInfo<TViewEquipEventInfo>();

	info->dwGuestPID = GetPlayerID();
	info->dwTargetPID = pTarget->GetPlayerID();

	SetViewEquipRequestEvent(event_create(view_equip_request_event, info, PASSES_PER_SEC(10)));

	pTarget->ChatPacket(CHAT_TYPE_COMMAND, "ViewEquipRequest %u", (DWORD)GetVID());
	return true;
}

void CHARACTER::DenyToViewEquip(LPCHARACTER guest)
{
	sys_log(1, "DenyToViewEquip %s guest %s %p", GetName(), guest->GetName(), get_pointer(guest->m_pkViewEquipRequestEvent));

	if (!guest->m_pkViewEquipRequestEvent)
		return;

	TViewEquipEventInfo* info = dynamic_cast<TViewEquipEventInfo*>(guest->m_pkViewEquipRequestEvent->info);

	if (!info)
	{
		sys_err("CHARACTER::DenyToViewEquip> <Factor> nullptr pointer");
		return;
	}

	if (info->dwGuestPID != guest->GetPlayerID())
		return;

	if (info->dwTargetPID != GetPlayerID())
		return;

	event_cancel(&guest->m_pkViewEquipRequestEvent);
	guest->ChatPacket(CHAT_TYPE_COMMAND, "ViewEquipRequestDenied");
}

void CHARACTER::AcceptToViewEquip(LPCHARACTER guest)
{
	sys_log(1, "AcceptToViewEquip %s guest %s %p", GetName(), guest->GetName(), get_pointer(guest->m_pkViewEquipRequestEvent));

	if (!guest->m_pkViewEquipRequestEvent)
		return;

	TViewEquipEventInfo* info = dynamic_cast<TViewEquipEventInfo*>(guest->m_pkViewEquipRequestEvent->info);

	if (!info)
	{
		sys_err("CHARACTER::AcceptToViewEquip> <Factor> nullptr pointer");
		return;
	}

	if (info->dwGuestPID != guest->GetPlayerID())
		return;

	if (info->dwTargetPID != GetPlayerID())
		return;

	event_cancel(&guest->m_pkViewEquipRequestEvent);

	SendEquipment(guest);

	return;
}

void CHARACTER::SendEquipment(LPCHARACTER ch)
{
	TPacketViewEquip p;
	p.header = HEADER_GC_VIEW_EQUIP;
	p.vid = GetVID();

	int pos[20] = {
					WEAR_BODY,			// 0
					WEAR_HEAD,			// 1
					WEAR_FOOTS,			// 2
					WEAR_WRIST,			// 3
					WEAR_WEAPON,		// 4
					WEAR_NECK,			// 5
					WEAR_EAR,			// 6
					WEAR_UNIQUE1,		// 7
					WEAR_UNIQUE2,		// 8
					WEAR_ARROW,			// 9
					WEAR_SHIELD,		// 10
					WEAR_COSTUME_BODY,	// 11
					WEAR_COSTUME_HAIR,	// 12
					WEAR_COSTUME_WEAPON,// 13
					WEAR_COSTUME_MOUNT,	// 14
					WEAR_COSTUME_SASH,	// 15
					WEAR_RING1,			// 16
					WEAR_RING2,			// 17
					WEAR_BELT,			// 18
					WEAR_CHARM,			// 19
	};

	for (int i = 0; i < 20; i++)
	{
		LPITEM item = GetWear(pos[i]);

		if (item)
		{
			p.equips[i].vnum = item->GetVnum();
			p.equips[i].count = item->GetCount();

			thecore_memcpy(p.equips[i].alSockets, item->GetSockets(), sizeof(p.equips[i].alSockets));
			thecore_memcpy(p.equips[i].aAttr, item->GetAttributes(), sizeof(p.equips[i].aAttr));
			p.equips[i].sealbind = item->GetSealBindTime();
			p.equips[i].transmutation = item->GetTransmutation();
		}
		else
			p.equips[i].vnum = 0;
	}

	DESC* desc = ch->GetDesc();
	if (nullptr == desc)
		return;

	desc->Packet(&p, sizeof(p));
}
#endif

int CHARACTER::BKBul(long skillindex) const
{
	int	cell = 999;
	LPITEM item;

	for (int i = 0; i < SKILL_BOOK_INVENTORY_MAX_NUM; ++i)
	{
		item = GetSkillBookInventoryItem(i);
		if (nullptr != item && item->GetSocket(0) == skillindex && item->GetVnum() == 50300)
		{
			// 개인 상점에 등록된 물건이면 넘어간다.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				cell = item->GetCell();
		}
	}

	return cell;
}

bool CHARACTER::IsInSafezone() const
{
	LPSECTREE sectree = GetSectree();
	return (sectree && sectree->IsAttr(GetX(), GetY(), ATTR_BANPK));
}

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
void CHARACTER::SendExtendInventoryPacket() const
{
	if (!GetDesc())
		return;

	TPacketGCExtendInventory ExtendInvSetStage;
	ExtendInvSetStage.header = HEADER_GC_EXTEND_INVENTORY;
	ExtendInvSetStage.subheader = SET_STAGE_EXTEND_INVEN;
	ExtendInvSetStage.dwExtend_inventory_stage = GetExtendInvenStage();
	GetDesc()->Packet(&ExtendInvSetStage, sizeof(ExtendInvSetStage));

	if (test_server)
		sys_log(0, "SendExtendInventoryPacket: Character %s Stage: %u", GetName(), ExtendInvSetStage.dwExtend_inventory_stage);

	if (ExtendInvSetStage.dwExtend_inventory_stage > 0)
	{
		TPacketGCExtendInventory p1;
		p1.header = HEADER_GC_EXTEND_INVENTORY;
		p1.subheader = SET_MAX_EXTEND_INVENTORY;
		const short current_slots = m_points.extend_inventory;
		p1.dwExtend_inventory_max = current_slots;

		GetDesc()->Packet(&p1, sizeof(p1));

		if (test_server)
			sys_log(0, "SendExtendInventoryPacket: Character %s Max Slots: %u", GetName(), current_slots);
	}
}

void CHARACTER::UpgradeExInventory(CHARACTER * ch)
{
	int NeedCount = ch->GetExtendItemNeedCount();
	int ItemCountOne = ch->CountSpecifyItem(72319);
	int ItemCountTwo = ch->CountSpecifyItem(72320);

	if ((ItemCountOne + ItemCountTwo) < NeedCount)
		return;

	const short current_slots = m_points.extend_inventory;

	if (current_slots >= EXTEND_INVENTORY_SLOT_MAX)
	{
		if (test_server)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("IS_FULL_MAX_EXTEND_INVENTORY_SLOTS"));
	}

	if (ItemCountOne < NeedCount)
	{
		NeedCount -= ItemCountOne;
		if (ItemCountTwo >= NeedCount)
		{
			ch->RemoveSpecifyItem(72319, ItemCountOne);
			ch->RemoveSpecifyItem(72320, NeedCount);
		}
		else
			return;
	}
	else
		ch->RemoveSpecifyItem(72319, NeedCount);

	ch->SetExtendedInventory(current_slots + 5);
	ch->SendExtendInventoryPacket();

	return;
}
int CHARACTER::CheckExtendAvailableSlots3()
{
	if (this->GetExtendInvenStage() == 1)
		return 1;
	else if (this->GetExtendInvenStage() == 2)
		return 2;
	else if (this->GetExtendInvenStage() == 3)
		return 3;
	else if (this->GetExtendInvenStage() == 4)
		return 4;
	else if (this->GetExtendInvenStage() == 5)
		return 5;
	else if (this->GetExtendInvenStage() == 6)
		return 6;
	else if (this->GetExtendInvenStage() == 7)
		return 7;
	else if (this->GetExtendInvenStage() == 8)
		return 8;
	else if (this->GetExtendInvenStage() >= 9)
		return 9;

	return 0;
}

int CHARACTER::CheckExtendAvailableSlots4()
{
	if (this->GetExtendInvenStage() == 10)
		return 1;
	else if (this->GetExtendInvenStage() == 11)
		return 2;
	else if (this->GetExtendInvenStage() == 12)
		return 3;
	else if (this->GetExtendInvenStage() == 13)
		return 4;
	else if (this->GetExtendInvenStage() == 14)
		return 5;
	else if (this->GetExtendInvenStage() == 15)
		return 6;
	else if (this->GetExtendInvenStage() == 16)
		return 7;
	else if (this->GetExtendInvenStage() == 17)
		return 8;
	else if (this->GetExtendInvenStage() >= 18)
		return 9;

	return 0;
}

bool CHARACTER::IsAvailableCell(int cell, short ExtendSlots)
{
	return cell <= ExtendSlots;
}

// Used for packet
int CHARACTER::GetExtendItemNeedCount()
{
	if (this->GetExtendInvenStage() < 3)
		return 2;
	else if (this->GetExtendInvenStage() < 6)
		return 3;
	else if (this->GetExtendInvenStage() < 9)
		return 4;
	else if (this->GetExtendInvenStage() < 12)
		return 5;
	else if (this->GetExtendInvenStage() < 15)
		return 6;
	else if (this->GetExtendInvenStage() < 18)
		return 7;
	else if (this->GetExtendInvenStage() < 21)
		return 8;
	else if (this->GetExtendInvenStage() < 24)
		return 9;
	else if (this->GetExtendInvenStage() < 27)
		return 10;

	return 0;
}
#endif

#ifdef ENABLE_YOUTUBER_SYSTEM
void CHARACTER::YoutubeYonetimPaneli(const char* arg1, const char* arg2, const char* arg3)
{
	if (quest::CQuestManager::instance().GetEventFlag("disable_youtube_system") == 1)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Sistem suan icin GM tarafindan devre disi birakildi!"));
		return;
	}

	if (!*arg1 || 0 == arg1[0])
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen islem secin!"));
		return;
	}

	const std::string& IslemSec = std::string(arg1);
	if (IslemSec == "olustur")
	{
		if (!*arg2 || 0 == arg2[0])
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen bir baslik giriniz!"));
			return;
		}

		if (!*arg3 || 0 == arg3[0])
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen video link uzantisini giriniz!"));
			return;
		}

		char Baslik[32];
		char Link[32];
		strlcpy(Baslik, arg2, sizeof(Baslik));
		strlcpy(Link, arg3, sizeof(Link));

		if (strlen(Baslik) == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen bir baslik giriniz!"));
			return;
		}

		if (strlen(Link) == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen video linkini giriniz!"));
			return;
		}

		if (strstr(Baslik, "%") || strstr(Baslik, "'") || strstr(Baslik, " "))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Baslikta gecersiz karakterler kullanamazsiniz!"));
			return;
		}

		if (strstr(Link, "%") || strstr(Link, "'") || strstr(Link, " "))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Linkte gecersiz karakterler kullanamazsiniz!"));
			return;
		}

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT baslik, link FROM player.youtuber WHERE owner_id = %u", this->GetPlayerID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row;
			while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				char Baslikk[32];
				char Linkk[32];
				strlcpy(Baslikk, row[0], sizeof(Baslikk));
				strlcpy(Linkk, row[1], sizeof(Linkk));

				if (std::string(Baslik) == std::string(Baslikk))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Daha once bu baslikla video olusturulmus! Lutfen baska baslik secin."));
					return;
				}

				if (std::string(Link) == std::string(Linkk))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Daha once bu linkten zaten video olusturulmus!"));
					return;
				}
			}
		}

		char szQuery[512];
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO player.youtuber(owner_id, youtuber, baslik, link) VALUES(%u, '%s', '%s', '%s')", this->GetPlayerID(), this->GetName(), Baslik, Link);
		std::unique_ptr<SQLMsg> pkMsg(DBManager::Instance().DirectQuery(szQuery));
	}
	else if (IslemSec == "sil")
	{
		if (!*arg2 || 0 == arg2[0])
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen silinecek videoyu seciniz!"));
			return;
		}

		char Baslik[32];
		strlcpy(Baslik, arg2, sizeof(Baslik));

		if (strlen(Baslik) == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen bir baslik giriniz!"));
			return;
		}

		if (strstr(Baslik, "%") || strstr(Baslik, "'") || strstr(Baslik, " "))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Baslikta gecersiz karakterler kullanamazsiniz!"));
			return;
		}

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM player.youtuber WHERE owner_id = %u and baslik = '%s';", this->GetPlayerID(), Baslik));
	}
	else if (IslemSec == "listem")
	{
		ChatPacket(CHAT_TYPE_COMMAND, "video_listem_yenile");
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT baslik, link FROM player.youtuber WHERE owner_id = %u", this->GetPlayerID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row;
			while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				char	szBaslik[32];
				char	szLink[32];
				strlcpy(szBaslik, row[0], sizeof(szBaslik));
				strlcpy(szLink, row[1], sizeof(szLink));

				ChatPacket(CHAT_TYPE_COMMAND, "video_listem %s %s", szBaslik, szLink);
			}
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Gecersiz Secim yaptiniz!"));
		return;
	}
}

void CHARACTER::YoutubeVideoListesi(const char* arg1, const char* arg2)
{
	if (quest::CQuestManager::instance().GetEventFlag("disable_youtube_system") == 1)
	{
		ChatPacket(CHAT_TYPE_INFO, "Sistem suan icin GM tarafindan devre disi birakildi!");
		return;
	}

	if (!*arg1 || 0 == arg1[0])
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen bir islem seciniz!"));
		return;
	}

	const std::string& IslemSec = std::string(arg1);
	if (IslemSec == "youtuber")
	{
		ChatPacket(CHAT_TYPE_COMMAND, "youtuber_listesi_yenile");
		Youtuberr.clear();
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT youtuber FROM player.youtuber"));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row;
			while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				char Youtuberler[32];
				strlcpy(Youtuberler, row[0], sizeof(Youtuberler));

				if (find(Youtuberr.begin(), Youtuberr.end(), std::string(Youtuberler)) == Youtuberr.end())
				{
					Youtuberr.push_back(Youtuberler);
					ChatPacket(CHAT_TYPE_COMMAND, "youtuber_listesi %s", Youtuberler);
				}
			}
		}
	}
	else if (IslemSec == "video")
	{
		if (!*arg2 || 0 == arg2[0])
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Lutfen bir isim seciniz!"));
			return;
		}

		ChatPacket(CHAT_TYPE_COMMAND, "youtuber_video_listesi_yenile");
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT baslik, link FROM player.youtuber WHERE youtuber = '%s'", arg2));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row;
			while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				char szBaslik[32];
				char szLink[32];
				strlcpy(szBaslik, row[0], sizeof(szBaslik));
				strlcpy(szLink, row[1], sizeof(szLink));

				ChatPacket(CHAT_TYPE_COMMAND, "youtuber_video_listesi %s %s", szBaslik, szLink);
			}
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bu kisinin hic videosu yok!"));
		return;
	}
}

void CHARACTER::Yayinci_Liste()
{
	std::unique_ptr<SQLMsg> _yayinci_sorgu(DBManager::instance().DirectQuery("SELECT tanim FROM player.yayinci_liste WHERE karakter_id = %u", GetPlayerID()));
	if (!_yayinci_sorgu->uiSQLErrno) {
		if (_yayinci_sorgu->Get()->uiNumRows) {
			while (MYSQL_ROW row_yayinci = mysql_fetch_row(_yayinci_sorgu->Get()->pSQLResult)) {
				if (!strcmp(row_yayinci[0], "YOUTUBER"))
					m_afAffectFlag.Set(AFF_YOUTUBER);
				else if (!strcmp(row_yayinci[1], "TWITCH"))
					m_afAffectFlag.Set(AFF_TWITCH);
			}
		}
	}
}
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
void CHARACTER::FishEventGeneralInfo()
{
	if (!GetDesc())
		return;

	TPacketGCFishEventInfo packFishEvent;
	packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
	packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_GC_ENABLE;
	packFishEvent.dwFirstArg = quest::CQuestManager::instance().GetEventFlag("enable_fish_event");
	packFishEvent.dwSecondArg = GetFishEventUseCount();
	GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));

	if (GetFishEventUseCount() == 0)
	{
		for (int i = 0; i < FISH_EVENT_SLOTS_NUM; i++)
		{
			m_fishSlots[i].bIsMain = false;
			m_fishSlots[i].bShape = 0;
		}
	}

	for (int i = 0; i < FISH_EVENT_SLOTS_NUM; i++)
	{
		if (m_fishSlots[i].bIsMain)
		{
			TPacketGCFishEventInfo packFishEvent;
			packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
			packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_SHAPE_ADD;
			packFishEvent.dwFirstArg = i;
			packFishEvent.dwSecondArg = m_fishSlots[i].bShape;
			GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));
		}
	}
}

void CHARACTER::FishEventUseBox(TItemPos itemPos)
{
	if (itemPos.window_type != INVENTORY)
		return;

	if (!GetDesc())
		return;

	LPITEM item;

	if (!CanHandleItem())
		return;

	if (!IsValidItemPosition(itemPos) || !(item = GetItem(itemPos)))
		return;

	if (item->IsExchanging())
		return;

	if (item->GetVnum() == ITEM_FISH_EVENT_BOX)
	{
		BYTE randomShape = number(FISH_EVENT_SHAPE_1, FISH_EVENT_SHAPE_6);
		SetFishAttachedShape(randomShape);
		FishEventIncreaseUseCount();
		item->SetCount(item->GetCount() - 1);
	}
	else if (item->GetVnum() == ITEM_FISH_EVENT_BOX_SPECIAL)
	{
		SetFishAttachedShape(FISH_EVENT_SHAPE_7);
		FishEventIncreaseUseCount();
		item->SetCount(item->GetCount() - 1);
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can not use this item here."));
		return;
	}

	TPacketGCFishEventInfo packFishEvent;
	packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
	packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_BOX_USE;
	packFishEvent.dwFirstArg = GetFishAttachedShape();
	packFishEvent.dwSecondArg = GetFishEventUseCount();
	GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));
}

bool CHARACTER::FishEventIsValidPosition(BYTE shapePos, BYTE shapeType)
{
	bool finalReturn = false;

	BYTE positionList[7][8] = {
		{ FISH_EVENT_SHAPE_1, 12, 0, 6, 12, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_2, 24, 0, 0, 0, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_3, 17, 0, 6, 7, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_4, 17, 0, 1, 7, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_5, 17, 0, 1, 6, 7, 0, 0 },
		{ FISH_EVENT_SHAPE_6, 16, 0, 1, 7, 8, 0, 0 },
		{ FISH_EVENT_SHAPE_7, 16, 0, 1, 2, 6, 7, 8 },
	};

	for (int i = 0; i < sizeof(positionList) / sizeof(positionList[0]); i++)
	{
		if (positionList[i][0] == shapeType)
		{
			for (int j = 0; j < positionList[i][1]; j++)
			{
				if (shapePos == j
					&& m_fishSlots[j + positionList[i][2]].bShape == 0
					&& m_fishSlots[j + positionList[i][3]].bShape == 0
					&& m_fishSlots[j + positionList[i][4]].bShape == 0
					&& m_fishSlots[j + positionList[i][5]].bShape == 0
					&& m_fishSlots[j + positionList[i][6]].bShape == 0
					&& m_fishSlots[j + positionList[i][7]].bShape == 0)
				{
					finalReturn = true;
					break;
				}
			}

			break;
		}
	}

	return finalReturn;
}

void CHARACTER::FishEventPlaceShape(BYTE shapePos, BYTE shapeType)
{
	BYTE positionList[7][7] = {
		{ FISH_EVENT_SHAPE_1, 0, 6, 12, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_2, 0, 0, 0, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_3, 0, 6, 7, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_4, 0, 1, 7, 0, 0, 0 },
		{ FISH_EVENT_SHAPE_5, 0, 1, 6, 7, 0, 0 },
		{ FISH_EVENT_SHAPE_6, 0, 1, 7, 8, 0, 0 },
		{ FISH_EVENT_SHAPE_7, 0, 1, 2, 6, 7, 8 },
	};

	for (int i = 0; i < sizeof(positionList) / sizeof(positionList[0]); i++)
	{
		if (positionList[i][0] == shapeType)
		{
			for (int j = 1; j < 7; j++)
			{
				if (j > 1 && positionList[i][j] == 0)
					continue;

				if (positionList[i][j] == 0)
				{
					m_fishSlots[shapePos].bIsMain = true;
					m_fishSlots[shapePos].bShape = shapeType;
				}
				else
				{
					m_fishSlots[shapePos + positionList[i][j]].bIsMain = false;
					m_fishSlots[shapePos + positionList[i][j]].bShape = shapeType;
				}
			}

			break;
		}
	}
}

void CHARACTER::FishEventCheckEnd()
{
	bool isComplete = true;

	for (int i = 0; i < FISH_EVENT_SLOTS_NUM; i++)
	{
		if (m_fishSlots[i].bShape == 0)
		{
			isComplete = false;
			break;
		}
	}

	if (isComplete)
	{
		DWORD dwUseCount = GetFishEventUseCount();
		DWORD dwRewardVnum = dwUseCount <= 10 ? 25113 : (dwUseCount <= 24 ? 25112 : 25111);

		for (int i = 0; i < FISH_EVENT_SLOTS_NUM; i++)
		{
			m_fishSlots[i].bIsMain = false;
			m_fishSlots[i].bShape = 0;
		}

		AutoGiveItem(dwRewardVnum);
		PointChange(POINT_EXP, 30000);

		m_dwFishUseCount = 0;
		SetFishAttachedShape(0);

		TPacketGCFishEventInfo packFishEvent;
		packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
		packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_GC_REWARD;
		packFishEvent.dwFirstArg = dwRewardVnum;
		packFishEvent.dwSecondArg = 0;
		GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));
	}
}

void CHARACTER::FishEventAddShape(BYTE shapePos)
{
	if (!GetDesc())
		return;

	if (shapePos >= FISH_EVENT_SLOTS_NUM)
		return;

	int lastAttachedShape = GetFishAttachedShape();

	if (lastAttachedShape == 0 || (lastAttachedShape > ITEM_FISH_EVENT_BOX_SPECIAL))
		return;

	if (!FishEventIsValidPosition(shapePos, lastAttachedShape))
	{
		TPacketGCFishEventInfo packFishEvent;
		packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
		packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_BOX_USE;
		packFishEvent.dwFirstArg = GetFishAttachedShape();
		packFishEvent.dwSecondArg = GetFishEventUseCount();
		GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("That shape can not fit in this position."));
		return;
	}

	FishEventPlaceShape(shapePos, lastAttachedShape);

	TPacketGCFishEventInfo packFishEvent;
	packFishEvent.bHeader = HEADER_GC_FISH_EVENT_INFO;
	packFishEvent.bSubheader = FISH_EVENT_SUBHEADER_SHAPE_ADD;
	packFishEvent.dwFirstArg = shapePos;
	packFishEvent.dwSecondArg = lastAttachedShape;
	GetDesc()->Packet(&packFishEvent, sizeof(TPacketGCFishEventInfo));

	FishEventCheckEnd();
}
#endif

#ifdef ENABLE_IMPROVE_DRAGON_COIN
DWORD CHARACTER::GetDragonCoin()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT cash FROM account.account WHERE id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD dc = 0;
	str_to_number(dc, row[0]);
	return dc;
}

DWORD CHARACTER::GetDragonMark()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT coins FROM account.account WHERE id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD mark = 0;
	str_to_number(mark, row[0]);
	return mark;
}

void CHARACTER::SetDragonCoin(DWORD amount)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE account.account SET cash = '%d' WHERE id = '%d';", amount, GetDesc()->GetAccountTable().id));
	RefreshDragonCoin();
}

void CHARACTER::SetDragonMark(DWORD amount)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE account.account SET coins = '%d' WHERE id = '%d';", amount, GetDesc()->GetAccountTable().id));
	RefreshDragonCoin();
}

void CHARACTER::RefreshDragonCoin()
{
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshDragonCoin %d", GetDragonCoin());
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshDragonMark %d", GetDragonMark());
}
#endif

#ifdef ENABLE_EXTENDED_PET_SYSTEM
void CHARACTER::CheckPet()
{
	LPITEM item = GetWear(WEAR_PET);
	if (item)
	{
		if (!item->IsPetItem())
		{
			return;
		}
		else
		{
			CPetSystem* petSystem = GetPetSystem();
			if (petSystem)
				petSystem->Summon(item->GetValue(0), item, 0, false);
		}
	}
	else
		return;
}
#endif

#ifdef ENABLE_GEM_SYSTEM
void CHARACTER::OpenGemShopFirstTime()
{
#ifdef ENABLE_BOSS_GEM_SYSTEM
	return;
#else
	for (int i = 0; i < GEM_SLOTS_MAX_NUM; ++i)
	{
		m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId((i / 3) + 1);
		m_gemItems[i].bSlotStatus = i > 2 ? 1 : 0;
	}

	m_dwGemNextRefresh = time(nullptr) + (GEM_REFRESH_PULSE * 3600);
#endif
}

void CHARACTER::RefreshGemShopItems()
{
	for (int i = 0; i < GEM_SLOTS_MAX_NUM; ++i)
	{
#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (m_bGemShopType == true)
		{
			m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetID(i, true);
			// if (i >= 0 && i < 3)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(1, m_bGemShopType);
			// else if (i >= 3 && i < 6)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(2, m_bGemShopType);
			// else if (i >= 6 && i < 9)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(3, m_bGemShopType);
			// else if (i >= 9 && i < 12)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(4, m_bGemShopType);
			// else if (i >= 12 && i < 15)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(5, m_bGemShopType);
			// else if (i >= 15 && i < 18)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(6, m_bGemShopType);
			// else if (i >= 18 && i < 21)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(7, m_bGemShopType);
			// else if (i >= 21 && i < 24)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(8, m_bGemShopType);
			// else if (i >= 24 && i < 27)
				// m_gemBossItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(9, m_bGemShopType);
		}
		else
		{
			m_gemItems[i].bItemId = CShopManager::instance().GemShopGetID(i, false);
			// if (i >= 0 && i < 3)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(1, m_bGemShopType);
			// else if (i >= 3 && i < 6)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(2, m_bGemShopType);
			// else if (i >= 6 && i < 9)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(3, m_bGemShopType);
			// else if (i >= 9 && i < 12)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(4, m_bGemShopType);
			// else if (i >= 12 && i < 15)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(5, m_bGemShopType);
			// else if (i >= 15 && i < 18)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(6, m_bGemShopType);
			// else if (i >= 18 && i < 21)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(7, m_bGemShopType);
			// else if (i >= 21 && i < 24)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(8, m_bGemShopType);
			// else if (i >= 24 && i < 27)
				// m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId(9, m_bGemShopType);
		}
#else
		m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId((i / 3) + 1);
#endif
	}

#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (m_bGemShopType == true)
		m_dwGemBossNextRefresh = time(nullptr) + (GEM_REFRESH_PULSE * 3600);
	else
		m_dwGemNextRefresh = time(nullptr) + (GEM_REFRESH_PULSE * 3600);
#else
	m_dwGemNextRefresh = time(nullptr) + (GEM_REFRESH_PULSE * 3600);
#endif
}

void CHARACTER::RefreshGemShopWithItem()
{
#ifdef ENABLE_BOSS_GEM_SYSTEM
	return;
#else
	if (CountSpecifyItem(GEM_REFRESH_ITEM_VNUM) < 1) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough Refresh Gaya Market items."));
		return;
	}

	RemoveSpecifyItem(GEM_REFRESH_ITEM_VNUM, 1);

	for (int i = 0; i < GEM_SLOTS_MAX_NUM; ++i)
	{
		m_gemItems[i].bItemId = CShopManager::instance().GemShopGetRandomId((i / 3) + 1);
	}

	m_dwGemNextRefresh = time(nullptr) + (GEM_REFRESH_PULSE * 3600);

	RefreshGemShop();
#endif
}

#ifdef ENABLE_BOSS_GEM_SYSTEM
void CHARACTER::GemShopBuy(BYTE bPos)
#else
void CHARACTER::GemShopBuy(BYTE bPos)
#endif
{
	if (bPos >= GEM_SLOTS_MAX_NUM) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] Slot overflow."));
		return;
	}

	DWORD dwVnum = 0;
	BYTE bCount = 0;
	DWORD dwPrice = 0;

#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (m_bGemShopType == true)
	{
		if (!CShopManager::instance().GemShopGetInfoById(m_gemItems[bPos].bItemId, dwVnum, bCount, dwPrice, true))
		{
			sys_err("[Gaya] Can't get info by id %d", m_gemItems[bPos].bItemId);
			return;
		}
	}
	else
	{
		if (!CShopManager::instance().GemShopGetInfoById(m_gemItems[bPos].bItemId, dwVnum, bCount, dwPrice, false))
		{
			sys_err("[Gaya] Can't get info by id %d", m_gemItems[bPos].bItemId);
			return;
		}
	}
#else
	if (!CShopManager::instance().GemShopGetInfoById(m_gemItems[bPos].bItemId, dwVnum, bCount, dwPrice))
	{
		sys_err("[Gaya] Can't get info by id %d", m_gemItems[bPos].bItemId);
		return;
	}
#endif
	
#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (m_bGemShopType == true)
	{
		int countgaya = CountSpecifyItem(50927);
		if (countgaya < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough Gaya."));
			return;
		}
	}
	else
	{
		int countgaya = CountSpecifyItem(50926);
		if (countgaya < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough Gaya."));
			return;
		}
	}
#else
	if (GetGem() < (int)dwPrice) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough Gaya."));
		return;
	}
#endif

	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwVnum, bCount);

	if (item)
	{
		int iEmptyPos;
		if (item->IsDragonSoul())
		{
			iEmptyPos = GetEmptyDragonSoulInventory(item);
		}
		else
		{
			iEmptyPos = GetEmptyInventory(item->GetSize());
		}

		if (iEmptyPos < 0) {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough space."));
			return;
		}

#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (m_bGemShopType == true)
			RemoveSpecifyItem(50927, dwPrice);
		else
			RemoveSpecifyItem(50926, dwPrice);
#else
		PointChange(POINT_GEM, -dwPrice, false);
#endif

		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyPos));

		ITEM_MANAGER::instance().FlushDelayedSave(item);
	}
}

void CHARACTER::GemShopAdd(BYTE bPos)
{
#ifdef ENABLE_BOSS_GEM_SYSTEM
	return;
#endif
	BYTE needCount[] = { 0, 0, 0, 1, 2, 4, 8, 8, 8 };

	if (bPos >= GEM_SLOTS_MAX_NUM) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] Slot overflow."));
		return;
	}

	if (CountSpecifyItem(GEM_UNLOCK_ITEM_VNUM) < needCount[bPos]) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] You don't have enough Gaya Market Expansion items. You need more %d for unlock new slot."), needCount[bPos] - CountSpecifyItem(GEM_UNLOCK_ITEM_VNUM));
		return;
	}

	{
		if (m_gemItems[bPos].bSlotStatus == 0) {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Gaya] No more slots can be unlocked."));
			return;
		}
	}

	RemoveSpecifyItem(GEM_UNLOCK_ITEM_VNUM, needCount[bPos]);

	m_gemItems[bPos].bSlotStatus = 0;
	RefreshGemShop();
}

void CHARACTER::RefreshGemShop()
{
	TPacketGCGemShopRefresh pack;
	pack.header = HEADER_GC_GEM_SHOP_REFRESH;
	pack.nextRefreshTime = GetGemNextRefresh() - time(nullptr);
#ifdef ENABLE_BOSS_GEM_SYSTEM
	pack.bossGem = m_bGemShopType;
#endif

	for (int i = 0; i < GEM_SLOTS_MAX_NUM; ++i)
	{
		pack.shopItems[i].slotIndex = i;
		pack.shopItems[i].status = m_gemItems[i].bSlotStatus;

		DWORD dwVnum = 0;
		BYTE bCount = 0;
		DWORD dwPrice = 0;

#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (m_bGemShopType == true)
		{
			pack.shopItems[i].dwVnum = CShopManager::instance().GemShopGetVnumById(m_gemBossItems[i].bItemId, true);
			pack.shopItems[i].bCount = CShopManager::instance().GemShopGetCountById(m_gemBossItems[i].bItemId, true);
			pack.shopItems[i].dwPrice = CShopManager::instance().GemShopGetPriceById(m_gemBossItems[i].bItemId, true);
		}
		else
		{
			pack.shopItems[i].dwVnum = CShopManager::instance().GemShopGetVnumById(m_gemItems[i].bItemId, false);
			pack.shopItems[i].bCount = CShopManager::instance().GemShopGetCountById(m_gemItems[i].bItemId, false);
			pack.shopItems[i].dwPrice = CShopManager::instance().GemShopGetPriceById(m_gemItems[i].bItemId, false);
		}
#else
		if (!CShopManager::instance().GemShopGetInfoById(m_gemItems[i].bItemId, dwVnum, bCount, dwPrice))
		{
			sys_err("[Gaya] Can't get info by id %d", m_gemItems[i].bItemId);
			continue;
		}
#endif

		// pack.shopItems[i].dwVnum = dwVnum;
		// pack.shopItems[i].bCount = bCount;
		// pack.shopItems[i].dwPrice = dwPrice;
	}

	GetDesc()->Packet(&pack, sizeof(TPacketGCGemShopRefresh));
}

#ifdef ENABLE_BOSS_GEM_SYSTEM
void CHARACTER::OpenGemShop(bool gemShopType)
#else
void CHARACTER::OpenGemShop()
#endif
{
	m_bGemShopType = gemShopType;

#ifndef ENABLE_BOSS_GEM_SYSTEM
	if (GetGemNextRefresh() == 0)
		OpenGemShopFirstTime();
#endif

	RefreshGemShopItems();

	TPacketGCGemShopOpen pack;
	pack.header = HEADER_GC_GEM_SHOP_OPEN;
	pack.nextRefreshTime = GetGemNextRefresh() - time(nullptr);
#ifdef ENABLE_BOSS_GEM_SYSTEM
	pack.bossGem = m_bGemShopType;
#endif

	for (int i = 0; i < GEM_SLOTS_MAX_NUM; ++i)
	{
		pack.shopItems[i].slotIndex = i;
		pack.shopItems[i].status = m_gemItems[i].bSlotStatus;

#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (m_bGemShopType == true)
		{
			pack.shopItems[i].dwVnum = CShopManager::instance().GemShopGetVnumById(m_gemBossItems[i].bItemId, true);
			pack.shopItems[i].bCount = CShopManager::instance().GemShopGetCountById(m_gemBossItems[i].bItemId, true);
			pack.shopItems[i].dwPrice = CShopManager::instance().GemShopGetPriceById(m_gemBossItems[i].bItemId, true);
		}
		else
		{
			pack.shopItems[i].dwVnum = CShopManager::instance().GemShopGetVnumById(m_gemItems[i].bItemId, false);
			pack.shopItems[i].bCount = CShopManager::instance().GemShopGetCountById(m_gemItems[i].bItemId, false);
			pack.shopItems[i].dwPrice = CShopManager::instance().GemShopGetPriceById(m_gemItems[i].bItemId, false);
		}
#else
		if (!CShopManager::instance().GemShopGetInfoById(m_gemItems[i].bItemId, dwVnum, bCount, dwPrice))
		{
			sys_err("[Gaya] Can't get info by id %d", m_gemItems[i].bItemId);
			continue;
		}
#endif
	}

	GetDesc()->Packet(&pack, sizeof(TPacketGCGemShopOpen));
}

bool CHARACTER::CreateGaya(int glimmerstone_count, LPITEM metinstone_item, int cost, int pct)
{
	if (!metinstone_item)
		return false;

	if (CountSpecifyItem(GEM_GLIMMERSTONE_VNUM) < glimmerstone_count)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGHT_COUNT"));
		return false;
	}

	if (this->GetGold() < cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGHT_YANG"));
		return false;
	}

	RemoveSpecifyItem(GEM_GLIMMERSTONE_VNUM, glimmerstone_count);

	if (metinstone_item)
		//ITEM_MANAGER::instance().RemoveItem(metinstone_item, "REMOVE (GAYA)");
		metinstone_item->SetCount(metinstone_item->GetCount() - 1); 			// For stackable stones

	PointChange(POINT_GOLD, -cost, false);

	if (number(1, 100) <= pct)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SUCCESS_GAYA_CREATE"));
		PointChange(POINT_GEM, 1, false);
		return true;
	}
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("FAIL_SUCCESS_GAYA_CREATE"));

	return false;
}
#endif
#ifdef ENABLE_AFK_MODE_SYSTEM
EVENTFUNC(update_character_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("update_character_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (nullptr == ch || ch->IsNPC())
		return 0;

	if (get_dword_time() - ch->GetLastMoveTime() > 60000 * 5)
	{
#ifdef ENABLE_AFK_MODE_SYSTEM
		if (!ch->IsGM())
		{
			if (!ch->IsAway())
				ch->SetAway(true);
			if (!ch->IsAffectFlag(AFF_AFK))
				ch->AddAffect(AFFECT_AFK, POINT_NONE, 0, AFF_AFK, INFINITE_AFFECT_DURATION, 0, true, true);
		}
#endif
	}

	return PASSES_PER_SEC(15);
}

void CHARACTER::StartUpdateCharacterEvent()
{
	if (m_pkUpdateCharacter)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkUpdateCharacter = event_create(update_character_event, info, PASSES_PER_SEC(15));
}
#endif
#ifdef ENABLE_HALLOWEEN_EVENT
void CHARACTER::GetHalloweenRank(char* buffer, size_t buflen)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT name, collected_pumpkin FROM log.halloween_event ORDER BY collected_pumpkin DESC LIMIT 10"));
	if (pMsg->Get()->uiNumRows == 0)
		return;

	MYSQL_ROW row;
	int pos = 1;
	int len = 0, len2;
	*buffer = '\0';
	while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		int points = 0;
		str_to_number(points, row[1]);
		len2 = snprintf(buffer + len, buflen - len, "[ENTER]");

		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;

		len2 = snprintf(buffer + len, buflen - len, "%d. %s - %s - %d %s.", pos, LC_TEXT("HALLOWEEN_PLACE"), row[0], points, LC_TEXT("HALLOWEEN_POINTS"));

		++pos;
		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;
	}
}
#endif
LPREGEN CHARACTER::GetRegen()
{
	return m_pkRegen;
}
#ifdef ENABLE_COSTUME_SET_SYSTEM
int CHARACTER::IsHalloweenSetCount()
{
	int count = 0;
	if (GetWear(WEAR_COSTUME_BODY) && GetWear(WEAR_COSTUME_BODY)->IsSetHalloweenCostume() == true)
		count++;
	if (GetWear(WEAR_COSTUME_HAIR) && GetWear(WEAR_COSTUME_HAIR)->IsSetHalloweenHair() == true)
		count++;
	if (GetWear(WEAR_COSTUME_WEAPON) && GetWear(WEAR_COSTUME_WEAPON)->IsSetHalloweenWeapon() == true)
		count++;
	if (GetWear(WEAR_COSTUME_MOUNT) && GetWear(WEAR_COSTUME_MOUNT)->IsSetHalloweenMount() == true)
		count++;

	return count;
}

void CHARACTER::UpdateHalloweenSetStatus(int iAdd)
{
	m_bHalloweenSetStatus = iAdd;
}
#endif

#ifdef ENABLE_FALL_FIX
bool CHARACTER::CanFall()
{

	if (IsAffectFlag(AFF_CHEONGEUN) && !IsAffectFlag(AFF_CHEONGEUN_WITH_FALL)) //Taichi skill
		return false;

	if (IsImmune(IMMUNE_FALL)) //Immune flag
		return false;

	if (!IsPC() &&
		GetRaceNum() == 1097 &&
		GetRaceNum() == 1098 &&
		GetRaceNum() == 1099 &&
		GetRaceNum() == 2496 &&
		GetRaceNum() == 2497 &&
		GetRaceNum() == 2498
		)
		return false;
	
	return true;
}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
///////////////////////////////////////////////////////////////////////////////////////////////
void CHARACTER::SetYellowmark(int amount)
{
	if (amount > 0)
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET yellowmark = yellowmark + '%d' WHERE id = '%d';", amount, GetPlayerID());
	else
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET yellowmark = yellowmark - '%d' WHERE id = '%d';", amount, GetPlayerID());
}

void CHARACTER::ClearYellowMark()
{
	DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET yellowmark = '0' WHERE id = '%d';", GetPlayerID());
}

int CHARACTER::GetYellowmark()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT yellowmark FROM player.zodiac_point WHERE id = '%d';", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int iMark = 0;
	str_to_number(iMark, row[0]);
	return iMark;
}

void CHARACTER::SetGreenmark(int amount)
{
	if (amount > 0)
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET greenmark = greenmark + '%d' WHERE id = '%d';", amount, GetPlayerID());
	else
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET greenmark = greenmark - '%d' WHERE id = '%d';", amount, GetPlayerID());
}

void CHARACTER::ClearGreenMark()
{
	DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET greenmark = '0' WHERE id = '%d';", GetPlayerID());
}

int CHARACTER::GetGreenmark()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT greenmark FROM player.zodiac_point WHERE id = '%d';", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int iMark = 0;
	str_to_number(iMark, row[0]);
	return iMark;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CHARACTER::SetYellowreward(int amount)
{
	if (amount > 0)
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET yellowreward = yellowreward + '%d' WHERE id = '%d';", amount, GetPlayerID());
	else
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET yellowreward = yellowreward - '%d' WHERE id = '%d';", amount, GetPlayerID());
}

int CHARACTER::GetYellowreward()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT yellowreward FROM player.zodiac_point WHERE id = '%d';", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int iMark = 0;
	str_to_number(iMark, row[0]);
	return iMark;
}

void CHARACTER::SetGreenreward(int amount)
{
	if (amount > 0)
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET greenreward = greenreward + '%d' WHERE id = '%d';", amount, GetPlayerID());
	else
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET greenreward = greenreward - '%d' WHERE id = '%d';", amount, GetPlayerID());
}

int CHARACTER::GetGreenreward()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT greenreward FROM player.zodiac_point WHERE id = '%d';", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int iMark = 0;
	str_to_number(iMark, row[0]);
	return iMark;
}

void CHARACTER::SetGoldreward(int amount)
{
	if (amount > 0)
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET goldreward = goldreward + '%d' WHERE id = '%d';", amount, GetPlayerID());
	else
		DBManager::instance().DirectQuery("UPDATE player.zodiac_point SET goldreward = goldreward - '%d' WHERE id = '%d';", amount, GetPlayerID());
}

int CHARACTER::GetGoldreward()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT goldreward FROM player.zodiac_point WHERE id = '%d';", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int iMark = 0;
	str_to_number(iMark, row[0]);
	return iMark;
}

void CHARACTER::IsZodiacEffectMob()
{
	if (!IsMonster())
		return;

	if (IsDead())
		return;

	DWORD Monster = GetRaceNum();

	if (!Monster || Monster == 0)
		return;

	if (Monster == 2750 || Monster == 2860)
	{
		if (number(1, 2) == 1)
		{
			EffectPacket(SE_SKILL_DAMAGE_ZONE);
		}
		else
		{
			EffectPacket(SE_SKILL_SAFE_ZONE);
		}
	}
}

void CHARACTER::IsZodiacEffectPC(DWORD Monster)
{
	if (!IsPC())
		return;

	if (IsDead())
		return;

	if (!Monster || Monster == 0)
		return;

	if (!GetDesc() || !GetDesc()->GetCharacter())
	{
		sys_err("Dusmeyi Fixledik a.q");
		return;
	}

	if (Monster == 20464)
		EffectPacket(SE_DEAPO_BOOM);
	else if (Monster == 2770 || Monster == 2771 || Monster == 2772)
		EffectPacket(SE_METEOR);
	else if (Monster == 2790 || Monster == 2791 || Monster == 2792)
		EffectPacket(SE_BEAD_RAIN);
	else if (Monster == 2830 || Monster == 2831 || Monster == 2832)
		EffectPacket(SE_FALL_ROCK);
	else if (Monster == 2800 || Monster == 2801 || Monster == 2802)
		EffectPacket(SE_ARROW_RAIN);
	else if (Monster == 2810 || Monster == 2811 || Monster == 2812)
		EffectPacket(SE_HORSE_DROP);
	else if (Monster == 2840 || Monster == 2841 || Monster == 2842)
		EffectPacket(SE_EGG_DROP);
}

void CHARACTER::ZodiacFloorMessage(BYTE Floor)
{
	if (!IsPC())
		return;

	if (Floor == 1)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 2)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 3)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 4)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 5)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 6)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Zodyak patronunu hic olmeden alt et."));
	else if (Floor == 7)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bir metin tasi yok et! Basarili olursan bir bonus guclendirici kazanirsin."));
	else if (Floor == 8)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum metin taslarini imha et."));
	else if (Floor == 9)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 10)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 11)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Zodyak patronunu alt et."));
	else if (Floor == 12)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari hic olmeden alt et."));
	else if (Floor == 13)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Metin taslarini imha et."));
	else if (Floor == 14)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bonus Kati: Metin taslarini imha et."));
	else if (Floor == 15)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Canavarlari yen."));
	else if (Floor == 16)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 17)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Zodyak patronunu alt et."));
	else if (Floor == 18)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum metin taslarini imha et."));
	else if (Floor == 19)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari hic olmeden alt et."));
	else if (Floor == 20)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 21)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bir metin tasi yok et! Basarili olursan bir bonus guclendirici kazanirsin."));
	else if (Floor == 22)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum Zodyak Patonlarini alt et."));
	else if (Floor == 23)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 24)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari hic olmeden alt et."));
	else if (Floor == 25)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 26)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 27)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum metin taslarini imha et."));
	else if (Floor == 28)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bonus Kati: Metin taslarini imha et."));
	else if (Floor == 29)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Metin taslarini imha et."));
	else if (Floor == 30)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Metin taslarini imha et."));
	else if (Floor == 31)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 32)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 33)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Tum canavarlari alt et."));
	else if (Floor == 34)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Canavarlari yen."));
	else if (Floor >= 35 && Floor <= 39)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bonus Kati: Bir metin tasi imha et."));
	else if (Floor == 40)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bonus Kati: Simdi 5 dakika boyunca tuccardan alisveris yaparak stoklarinizi doldurabilirsiniz."));
	//-------------------------------------------------------------------------------------------------------//
	else if (Floor == 41)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Misyon basarisiz oldu."));
	else if (Floor == 42)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Bonus katini kil payiyla kacirdin."));
	else if (Floor == 43)
		ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, "Zaman doldu.");
}

void CHARACTER::EffectZodiacPacket(long X, long Y, int enumEffectType, int enumEffectType2)
{
	TPacketGCSpecialZodiacEffect p;

	p.header = HEADER_GC_SEPCIAL_ZODIAC_EFFECT;
	p.type = enumEffectType;
	p.type2 = enumEffectType2;
	p.vid = GetVID();
	p.x = X;
	p.y = Y;

	PacketAround(&p, sizeof(p));
}

DWORD CHARACTER::CountZodiacItems(DWORD Vnum)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT count FROM player.zodiac_npc WHERE item_vnum = '%u' and owner_id = '%d'", Vnum, GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD dwCount = 0;
	str_to_number(dwCount, row[0]);
	return dwCount;
}

void CHARACTER::SetZodiacItems(DWORD Vnum, int Count)//COUNT(*)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT owner_id FROM player.zodiac_npc WHERE item_vnum = '%u' and owner_id = '%u'", Vnum, GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
	{
		char szQuery[512];
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO player.zodiac_npc(owner_id, item_vnum, count) VALUES(%u, %u, %d)", GetPlayerID(), Vnum, Count);
		DBManager::Instance().DirectQuery(szQuery);
		return;
	}
	else
	{
		char szQuery2[512];
		snprintf(szQuery2, sizeof(szQuery2), "UPDATE player.zodiac_npc SET count = '%d' WHERE item_vnum = %u and owner_id = '%u'", Count, Vnum, GetPlayerID());
		DBManager::Instance().DirectQuery(szQuery2);
	}
}

DWORD CHARACTER::PurchaseCountZodiacItems(DWORD Vnum)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT count FROM player.zodiac_npc_sold WHERE item_vnum = '%u' and owner_id = '%d'", Vnum, GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD dwCount = 0;
	str_to_number(dwCount, row[0]);
	return dwCount;
}

void CHARACTER::SetPurchaseZodiacItems(DWORD Vnum, int Count)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT owner_id FROM player.zodiac_npc_sold WHERE item_vnum = '%u' and owner_id = '%u'", Vnum, GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
	{
		char szQuery[512];
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO player.zodiac_npc_sold(owner_id, item_vnum, count) VALUES(%u, %u, %d)", GetPlayerID(), Vnum, Count);
		DBManager::Instance().DirectQuery(szQuery);
		return;
	}
	else
	{
		char szQuery2[512];
		snprintf(szQuery2, sizeof(szQuery2), "UPDATE player.zodiac_npc_sold SET count = '%d' WHERE item_vnum = '%u' and owner_id = '%u'", Count, Vnum, GetPlayerID());
		DBManager::Instance().DirectQuery(szQuery2);
	}
}

void CHARACTER::CzCheckBox()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT id FROM player.zodiac_point WHERE id = '%d'", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
	{
		char szQuery[512];
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO player.zodiac_point(id, yellowmark, greenmark, yellowreward, greenreward, goldreward) VALUES(%u, %u, %u, %u, %u, %u)", GetPlayerID(), 0, 0, 0, 0, 0);
		DBManager::Instance().DirectQuery(szQuery);
	}
}
#endif

#ifdef ENABLE_WALLHACK_FIX
/*
Wallhack Detection
*/
EVENTFUNC(wallhack_check)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("wallhack_check <Factor> nullptr pointer");
		return 0;
	}

	CHARACTER * Player = info->ch;

	if (!Player)
		return 0;

	LPDESC d = Player->GetDesc();

	if (!d)
		return 0;

	if (d->IsPhase(PHASE_HANDSHAKE) || d->IsPhase(PHASE_LOGIN) || d->IsPhase(PHASE_SELECT) || d->IsPhase(PHASE_DEAD) || d->IsPhase(PHASE_LOADING))
		return PASSES_PER_SEC(1);

	sys_log(2, "WALLHACK_CHECK: %s", Player->GetName());



	LPSECTREE CurrentTree = Player->GetSectree();
	if (!CurrentTree)
		return PASSES_PER_SEC(1);

	bool isWallHack = false;
	for (int32_t dx = -300; dx < 300; ++dx)
	{
		for (int32_t dy = -300; dy < 300; ++dy)
		{
			if (CurrentTree->IsAttr(Player->GetX() + dx, Player->GetY() + dy, ATTR_BLOCK | ATTR_OBJECT))
			{
				isWallHack = true;
				break;
			}
		}
	}

	if (isWallHack)
	{
		if (Player->GetLastMoveAblePosition().x == 0 || Player->GetLastMoveAblePosition().y == 0 || Player->GetLastMoveableMapIndex() == 0)
		{
			Player->GoHome(); //you're drunk!
			return 0;
		}

		std::string HackInfo = "S_WALL_HACK x" + std::to_string(Player->GetX());
		HackInfo += " y" + std::to_string(Player->GetY());
		HackInfo += " z" + std::to_string(Player->GetZ());
		HackInfo += " m" + std::to_string(Player->GetMapIndex());
		LogManager::instance().HackLog(HackInfo.c_str(), Player);
		Player->WarpSet(Player->GetLastMoveAblePosition().x, Player->GetLastMoveAblePosition().y, Player->GetLastMoveableMapIndex());
	}
	else
	{
		Player->SetLastMoveAblePosition(Player->GetXYZ());
		Player->SetLastMoveableMapIndex();
	}

	return PASSES_PER_SEC(1);
}

void CHARACTER::StartCheckWallhack()
{
	if (m_pkCheckWallHackEvent)
		return;

	// temporary disable wallhack check
	if (true)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkCheckWallHackEvent = event_create(wallhack_check, info, PASSES_PER_SEC(1));
}
#endif

#ifdef ENABLE_FISHBOT_FIX
void CHARACTER::SetLastFishTime(int32_t time)
{
	m_iLastFishTime = time;
}

int32_t CHARACTER::GetLastFishTime()
{
	return m_iLastFishTime;
}

int32_t CHARACTER::GetFishHackCount()
{
	return m_iFishHackCount;
}

void CHARACTER::UpdateFishHackCount()
{
	m_iFishHackCount += 1;
}

void CHARACTER::ResetFishHackCount()
{
	m_iFishHackCount = 0;
}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
void CHARACTER::MountSummon(LPITEM mountItem)
{
	if (GetMapIndex() == 113)
		return;
	
	if (CWarMapManager::instance().IsWarMap(GetMapIndex()))
		return;

	CMountSystem* mountSystem = GetMountSystem();
	DWORD mobVnum = 0;

	if (!mountSystem || !mountItem)
		return;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(mountItem->GetTransmutation());
	if (pkItemTransmutation)
	{
		for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
		{
			BYTE bType = pkItemTransmutation->aApplies[i].bType;
			long value = pkItemTransmutation->aApplies[i].lValue;
			if (bType == APPLY_MOUNT)
				mobVnum = value;
		}
	}
	else
	{
		if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
			mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
	}
#else
	if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
		mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
#endif
	if (IsHorseRiding())
		StopRiding();

	if (GetHorse())
		HorseSummon(false);

	mountSystem->Summon(mobVnum, mountItem, false);
}

void CHARACTER::MountUnsummon(LPITEM mountItem)
{
	CMountSystem* mountSystem = GetMountSystem();
	DWORD mobVnum = 0;

	if (!mountSystem || !mountItem)
		return;

#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(mountItem->GetTransmutation());
	if (pkItemTransmutation)
	{
		for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
		{
			BYTE bType = pkItemTransmutation->aApplies[i].bType;
			long value = pkItemTransmutation->aApplies[i].lValue;
			if (bType == APPLY_MOUNT)
				mobVnum = value;
		}
	}
	else
	{
		if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
			mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
	}
#else
	if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
		mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
#endif

	if (GetMountVnum() == mobVnum)
		mountSystem->Unmount(mobVnum);

	mountSystem->Unsummon(mobVnum);
}

void CHARACTER::CheckMount()
{
	if (GetMapIndex() == 113)
		return;
	
	if (CWarMapManager::instance().IsWarMap(GetMapIndex()))
		return;

	CMountSystem* mountSystem = GetMountSystem();
	LPITEM mountItem = GetWear(WEAR_COSTUME_MOUNT);
	DWORD mobVnum = 0;

	if (!mountSystem || !mountItem)
		return;

#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(mountItem->GetTransmutation());
	if (pkItemTransmutation)
	{
		for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
		{
			BYTE bType = pkItemTransmutation->aApplies[i].bType;
			long value = pkItemTransmutation->aApplies[i].lValue;
			if (bType == APPLY_MOUNT)
				mobVnum = value;
		}
	}
	else
	{
		if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
			mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
	}
#else
	if (mountItem->FindApplyValue(APPLY_MOUNT) != 0)
		mobVnum = mountItem->FindApplyValue(APPLY_MOUNT);
#endif

	if (mountSystem->CountSummoned() == 0)
	{
		mountSystem->Summon(mobVnum, mountItem, false);
	}
}

bool CHARACTER::IsRidingMount()
{
	return (GetWear(WEAR_COSTUME_MOUNT) || FindAffect(AFFECT_MOUNT));
}
#endif

#ifdef ENABLE_AURA_SYSTEM
void CHARACTER::OpenAura(bool bCombination)
{
	if (isAuraOpened(bCombination))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The aura window it's already opened."));
		return;
	}

	if (bCombination)
	{
		if (m_bAuraAbsorption)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the aura absorption window."));
			return;
		}

		m_bAuraRefine = true;
	}
	else
	{
		if (m_bAuraRefine)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the aura combine window."));
			return;
		}

		m_bAuraAbsorption = true;
	}

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_OPEN;
	sPacket.bWindow = bCombination;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));

	ClearAuraMaterials();
}

void CHARACTER::CloseAura()
{
	if ((!m_bAuraRefine) && (!m_bAuraAbsorption))
		return;

	bool bWindow = (m_bAuraRefine == true ? true : false);

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_CLOSE;
	sPacket.bWindow = bWindow;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));

	if (bWindow)
		m_bAuraRefine = false;
	else
		m_bAuraAbsorption = false;

	ClearAuraMaterials();
}

void CHARACTER::ClearAuraMaterials()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			continue;

		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = nullptr;
	}
}

bool CHARACTER::AuraIsSameGrade(long lGrade)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	if (!pkItemMaterial[0])
		return false;

	bool bReturn = (pkItemMaterial[0]->GetValue(AURA_GRADE_VALUE_FIELD) == lGrade ? true : false);
	return bReturn;
}

DWORD CHARACTER::GetAuraCombinePrice(long lGrade)
{
	DWORD dwPrice = AURA_REFINE_PRICE;
	return dwPrice;
}

BYTE CHARACTER::CheckAuraEmptyMaterialSlot()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			return i;
	}

	return 255;
}

void CHARACTER::GetAuraCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	if (m_bAuraRefine)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			long lVal = pkItemMaterial[0]->GetValue(AURA_TYPE_VALUE_FIELD);
			if (lVal == 6)
			{
				dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			}
			else
			{
				DWORD dwMaskVnum = pkItemMaterial[0]->GetOriginalVnum();
				TItemTable* pTable = ITEM_MANAGER::instance().GetTable(dwMaskVnum + 1);
				if (pTable)
					dwMaskVnum += 1;

				dwItemVnum = dwMaskVnum;
			}
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
	else
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			dwMinAbs = pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET);
			dwMaxAbs = dwMinAbs;
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
}

void CHARACTER::AddAuraMaterial(TItemPos tPos, BYTE bPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
	{
		if (bPos == 255)
		{
			bPos = CheckEmptyMaterialSlot();
			if (bPos >= AURA_WINDOW_MAX_MATERIALS)
				return;
		}
		else
			return;
	}

	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	else if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (pkItem->IsEquipped()) || (tPos.IsBeltInventoryPosition()) || (pkItem->IsDragonSoul()))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraRefine))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraAbsorption))
		return;
	else if (pkItem->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't add locked items."));
		return;
	}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	else if ((pkItem->IsSealed()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Aura] You can't add binded items."));
		return;
	}
#endif
	else if (pkItem->IsBasicItem())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		return;
	}
	else if ((m_bAuraRefine) && (pkItem->GetSocket(AURA_ABSORPTION_SOCKET) >= AURA_MAX_ABS))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This aura got already maximum absorption chance."));
		return;
	}
	else if ((bPos == 1) && (m_bAuraAbsorption))
	{
		if ((pkItem->GetType() != ITEM_ARMOR))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You can absorb just the bonuses from armors and weapons."));
			return;
		}
		else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_SHIELD) && (pkItem->GetSubType() != ARMOR_EAR) && (pkItem->GetSubType() != ARMOR_WRIST) && (pkItem->GetSubType() != ARMOR_NECK))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You can absorb just the bonuses from armors and weapons."));
			return;
		}
	}
	else if ((bPos == 1) && (m_bAuraRefine))
	{
		if ((pkItem->GetVnum() != AURA_ICE_RUNIC))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Only Refine Material Ice Runic"));
			return;
		}
	}
	else if ((pkItem->GetSubType() != COSTUME_AURA) && (m_bAuraAbsorption))
		return;
	else if ((pkItem->GetSubType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraAbsorption))
		return;
	else if ((pkItem->GetSocket(AURA_ABSORBED_SOCKET) > 0) && (bPos == 0) && (m_bAuraAbsorption))
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;

	if (pkItemMaterial[bPos])
		return;

	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetAuraCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_ADDED;
	sPacket.bWindow = m_bAuraRefine == true ? true : false;
	sPacket.dwPrice = GetAuraCombinePrice(pkItem->GetValue(AURA_GRADE_VALUE_FIELD));
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = dwItemVnum;
	sPacket.dwMinAbs = dwMinAbs;
	sPacket.dwMaxAbs = dwMaxAbs;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
}

void CHARACTER::RemoveAuraMaterial(BYTE bPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	DWORD dwPrice = 0;

	if (bPos == 1)
	{
		if (pkItemMaterial[bPos])
		{
			pkItemMaterial[bPos]->Lock(false);
			pkItemMaterial[bPos] = nullptr;
		}

		if (pkItemMaterial[0])
			dwPrice = GetAuraCombinePrice(pkItemMaterial[0]->GetValue(AURA_GRADE_VALUE_FIELD));
	}
	else
		ClearAuraMaterials();

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_REMOVED;
	sPacket.bWindow = m_bAuraRefine == true ? true : false;
	sPacket.dwPrice = dwPrice;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
}

BYTE CHARACTER::CanRefineAuraMaterials()
{
	BYTE bReturn = 0;
	LPITEM* pkItemMaterial = GetAuraMaterials();
	if (m_bAuraRefine)
	{
		for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
		{
			if (pkItemMaterial[i] != nullptr)
			{
				if ((pkItemMaterial[i]->GetType() == ITEM_COSTUME) && (pkItemMaterial[i]->GetSubType() == COSTUME_AURA))
					bReturn = 1;
				else if ((pkItemMaterial[i]->GetVnum() == AURA_ICE_RUNIC))
					bReturn = 1;
				else
				{
					bReturn = 0;
					break;
				}
			}
			else
			{
				bReturn = 0;
				break;
			}
		}
	}
	else if (m_bAuraAbsorption)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			if ((pkItemMaterial[0]->GetType() == ITEM_COSTUME) && (pkItemMaterial[0]->GetSubType() == COSTUME_AURA))
				bReturn = 2;
			else
				bReturn = 0;

			if ((pkItemMaterial[1]->GetType() == ITEM_ARMOR) && ((pkItemMaterial[1]->GetSubType() == ARMOR_EAR) || (pkItemMaterial[1]->GetSubType() == ARMOR_WRIST) || (pkItemMaterial[1]->GetSubType() == ARMOR_NECK) || (pkItemMaterial[1]->GetSubType() == ARMOR_SHIELD)))
				bReturn = 2;
			else
				bReturn = 0;

			if (pkItemMaterial[0]->GetSocket(AURA_ABSORBED_SOCKET) > 0)
				bReturn = 0;
		}
		else
			bReturn = 0;
	}

	return bReturn;
}

void CHARACTER::RefineAuraMaterials()
{
	BYTE bCan = CanRefineAuraMaterials();
	if (bCan == 0)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	if (!pkItemMaterial[0] || !pkItemMaterial[1])
		return;

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetAuraCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
	DWORD dwPrice = AURA_REFINE_PRICE;

	if (bCan == 1)
	{
		int iSuccessChance = 0;
		long lVal = pkItemMaterial[0]->GetValue(AURA_TYPE_VALUE_FIELD);
		switch (lVal)
		{
		case 2:
		{
			iSuccessChance = AURA_GRADE_2_REFINE;
		}
		break;
		case 3:
		{
			iSuccessChance = AURA_GRADE_3_REFINE;
		}
		break;
		case 4:
		{
			iSuccessChance = AURA_GRADE_4_REFINE;
		}
		break;
		case 5:
		{
			iSuccessChance = AURA_GRADE_5_REFINE;
		}
		break;
		case 6:
		{
			iSuccessChance = AURA_GRADE_6_REFINE;
		}
		break;
		default:
		{
			iSuccessChance = AURA_GRADE_1_REFINE;
		}
		break;
		}

		if (GetGold() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You don't have enough Yang."));
			return;
		}

		int iChance = number(1, 100);
		bool bSucces = (iChance <= iSuccessChance ? true : false);
		if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 49 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 99 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 149 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 199 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 249)
			bSucces = true;
		int oldGradeValue = pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD);
		bool bPacket = true;
		if (bSucces)
		{
			if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 49 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 99 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 149 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 199 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 249)
			{
				PointChange(POINT_GOLD, -dwPrice);
				pkItemMaterial[0]->SetSocket(AURA_ABSORPTION_SOCKET, pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET) + 1);
				pkItemMaterial[0]->SetSocket(AURA_GRADE_VALUE_FIELD, pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) + 1);
				bool bDelete = false;
				if (pkItemMaterial[1]->GetCount() - 1 < 1)
					bDelete = true;
				pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount() - 1);
				if (bDelete == true)
					pkItemMaterial[1] = NULL;
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));
			}
			else
			{
				LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwItemVnum, 1, 0, false);
				if (!pkItem)
				{
					sys_err("%d can't be created.", dwItemVnum);
					return;
				}

				ITEM_MANAGER::CopyAllAttrTo(pkItemMaterial[0], pkItem);
				LogManager::instance().ItemLog(this, pkItem, "AURA REFINE SUCCESS", pkItem->GetName());
				pkItem->SetSocket(AURA_ABSORPTION_SOCKET, pkItem->GetSocket(AURA_ABSORPTION_SOCKET) + 1);
				pkItem->SetSocket(AURA_ABSORBED_SOCKET, pkItemMaterial[0]->GetSocket(AURA_ABSORBED_SOCKET));
				pkItem->SetSocket(AURA_GRADE_VALUE_FIELD, pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) + 1);

				PointChange(POINT_GOLD, -dwPrice);
				DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

				WORD wCell = pkItemMaterial[0]->GetCell();
				ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[0], "AURA (REFINE SUCCESS)");
				pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount() - 1);

				pkItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
				ITEM_MANAGER::instance().FlushDelayedSave(pkItem);

				if (lVal == 6)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura New absorption rate: %2f%"), float(pkItem->GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));

				ClearAuraMaterials();
				bPacket = false;
			}
		}
		else
		{
			PointChange(POINT_GOLD, -dwPrice);
			pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount()-1);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

			//ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE FAIL)");

			if (lVal == 6)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura New absorption rate: %d%"), pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET));
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Failed."));

			if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 50 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 100 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 150 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 200 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 250)
				ClearAuraMaterials();
			bPacket = false;
		}

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketAura sPacket;
		sPacket.header = HEADER_GC_AURA;
		sPacket.subheader = AURA_SUBHEADER_GC_REFINED;
		sPacket.bWindow = m_bAuraRefine == true ? true : false;
		sPacket.dwPrice = dwPrice;
		if (pkItemMaterial[0])
		{
			if (bPacket)
				if (oldGradeValue != 49 && oldGradeValue != 99 && oldGradeValue != 149 && oldGradeValue != 199 && oldGradeValue != 249)
					sPacket.bPos = 255;
				else
					sPacket.bPos = 0;
			else
				sPacket.bPos = 0;
		}
		else
			sPacket.bPos = 0;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		if (bSucces)
			sPacket.dwMaxAbs = 100;
		else
			sPacket.dwMaxAbs = 0;

		GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
	}
	else
	{
		pkItemMaterial[1]->CopyAttributeTo(pkItemMaterial[0]);
		LogManager::instance().ItemLog(this, pkItemMaterial[0], "ABSORB (REFINE SUCCESS)", pkItemMaterial[0]->GetName());
		pkItemMaterial[0]->SetSocket(AURA_ABSORBED_SOCKET, pkItemMaterial[1]->GetOriginalVnum());
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (pkItemMaterial[0]->GetAttributeValue(i) < 0)
				pkItemMaterial[0]->SetForceAttribute(i, pkItemMaterial[0]->GetAttributeType(i), 0);
		}

		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "ABSORBED (REFINE SUCCESS)");

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemMaterial[0]);
		pkItemMaterial[0]->AttrLog();

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));

		ClearAuraMaterials();

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketAura sPacket;
		sPacket.header = HEADER_GC_AURA;
		sPacket.subheader = AURA_SUBHEADER_GC_REFINED;
		sPacket.bWindow = m_bAuraRefine == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 255;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		sPacket.dwMaxAbs = 1;
		GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
	}
}

bool CHARACTER::CleanAuraAttr(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_COSTUME) && (pkTarget->GetSubType() != COSTUME_AURA))
		return false;

	if (pkTarget->GetSocket(AURA_ABSORBED_SOCKET) <= 0)
		return false;
	
	pkTarget->SetSocket(AURA_ABSORBED_SOCKET, 0);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		pkTarget->SetForceAttribute(i, 0, 0);

	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN ATTR)", pkTarget->GetName());
	return true;
}
#endif

#ifdef ENABLE_GUILD_RANKING_SYSTEM
bool CHARACTER::IsGuildRankedSystem()
{
	if (quest::CQuestManager::instance().GetEventFlag("enable_guildranked_system") == 1)
		return false;

	return true;
}

void CHARACTER::SendGuildRankList()
{
	if (!this->GetDesc())
		return;
	
	if (IsOpenSafebox() || GetShop() || IsCubeOpen() || IsDead() || GetExchange() || GetMyShop() || IsCombOpen() || isSashOpened(true) || isSashOpened(false)){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUILD_RANK_SYSTEM_ERR_WINDOW"));
		return;
	}
	if (IsGuildRankedSystem() == false){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUILD_RANK_SYSTEM_DEACTIVE"));
		return;
	}
	
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT `name`,`master`,`win`,`loss` FROM player.guild ORDER BY `win` DESC LIMIT 10"));
	
	if (pMsg->Get()->uiNumRows == 0){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUILD_RANK_SYSTEM_NO_GUILD"));
		return;
	}
	
	TPacketGCGuildRankSystem pGuildRankPacket;
	pGuildRankPacket.header = HEADER_GC_GUILD_RANK_SYSTEM;

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		strlcpy(pGuildRankPacket.guild_name, row[0], sizeof(pGuildRankPacket.guild_name));
		str_to_number(pGuildRankPacket.master, row[1]);
		str_to_number(pGuildRankPacket.win, row[2]);
		str_to_number(pGuildRankPacket.loss, row[3]);
		GetDesc()->Packet(&pGuildRankPacket, sizeof(pGuildRankPacket));
	}
	ChatPacket(CHAT_TYPE_COMMAND, "BINARY_OpenGuildRanking");
}
#endif

#ifdef ENABLE_ALIGNMENT_SYSTEM
void CHARACTER::OnAlignUpdate(long lOldAlignment, bool manuelUpdate)
{
	if (GetRealAlignment() >= 300000)
	{
		if (lOldAlignment < 300000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 5000, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_STONE, 20, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS3, POINT_ATTBONUS_MONSTER, 20, 0, 60*60*60*365, 0, true);
	}
	else if (GetRealAlignment() >= 250000)
	{
		if (lOldAlignment < 250000 && lOldAlignment >= 300000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 3000, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 18, 0, 60*60*60*365, 0, true);		
	}
	else if (GetRealAlignment() >= 200000)
	{
		if (lOldAlignment < 200000 && lOldAlignment >= 250000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 2500, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 16, 0, 60*60*60*365, 0, true);		
	}
	else if (GetRealAlignment() >= 180000)
	{
		if (lOldAlignment < 180000 && lOldAlignment >= 200000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 2000, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 14, 0, 60*60*60*365, 0, true);
	}
	else if (GetRealAlignment() >= 120000)
	{
		if (lOldAlignment < 120000 && lOldAlignment >= 180000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 1500, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 12, 0, 60*60*60*365, 0, true);		
	}
	else if (GetRealAlignment() >= 80000)
	{
		if (lOldAlignment < 80000 && lOldAlignment >= 120000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 1000, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 10, 0, 60*60*60*365, 0, true);		
	}
	else if (GetRealAlignment() >= 40000)
	{
		if (lOldAlignment < 40000 && lOldAlignment >= 80000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 750, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 5, 0, 60*60*60*365, 0, true);		
	}
	else if (GetRealAlignment() >= 10000)
	{
		if (lOldAlignment < 10000 && lOldAlignment >= 40000)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		AddAffect(AFFECT_ALIGN_BONUS1, POINT_MAX_HP, 500, 0, 60*60*60*365, 0, true);
		AddAffect(AFFECT_ALIGN_BONUS2, POINT_ATTBONUS_MONSTER, 3, 0, 60*60*60*365, 0, true);		
	}
	else
	{
		if (lOldAlignment >= 10000 && GetRealAlignment() >= 0)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);			
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
		else if (lOldAlignment >= 0 && GetRealAlignment() < 0)
		{
			RemoveAffect(AFFECT_ALIGN_BONUS1);
			RemoveAffect(AFFECT_ALIGN_BONUS2);				
			RemoveAffect(AFFECT_ALIGN_BONUS3);
		}
	}
}
#endif