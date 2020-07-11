#ifndef __INC_METIN_II_CHAR_H__
#define __INC_METIN_II_CHAR_H__

#include <unordered_map>

#include "../../common/stl.h"
#include "../../common/service.h"
#include "entity.h"
#include "FSM.h"
#include "horse_rider.h"
#include "vid.h"
#include "constants.h"
#include "affect.h"
#include "affect_flag.h"
#include "cube.h"
#include "mining.h"
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
#include "item_combination.h"
#endif
#include "packet.h"
#ifdef ENABLE_ABUSE_SYSTEM
#include "abuse.h"
#endif

class CBuffOnAttributes;
class CPetSystem;
#ifdef ENABLE_GROWTH_PET_SYSTEM
class CNewPetSystem;
#endif
#ifdef ENABLE_BUFFI_SYSTEM
class CSupportShaman;
#endif
class CInventory;
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
class CMountSystem;
#endif

enum EInstantFlags
{
	INSTANT_FLAG_DEATH_PENALTY = (1 << 0),
	INSTANT_FLAG_SHOP = (1 << 1),
	INSTANT_FLAG_EXCHANGE = (1 << 2),
	INSTANT_FLAG_STUN = (1 << 3),
	INSTANT_FLAG_NO_REWARD = (1 << 4),
};

enum EAiFlags
{
	AI_FLAG_NPC = (1 << 0),
	AI_FLAG_AGGRESSIVE = (1 << 1),
	AI_FLAG_HELPER = (1 << 2),
	AI_FLAG_STAYZONE = (1 << 3),
};

#ifdef ENABLE_MINIGAME_RUMI_EVENT
#define MAX_CARDS_IN_HAND			5
#define MAX_CARDS_IN_FIELD			3
#endif

extern int g_nPortalLimitTime;

enum RaceTypes
{
	MAIN_RACE_WARRIOR_M,
	MAIN_RACE_ASSASSIN_W,
	MAIN_RACE_SURA_M,
	MAIN_RACE_SHAMAN_W,
	MAIN_RACE_WARRIOR_W,
	MAIN_RACE_ASSASSIN_M,
	MAIN_RACE_SURA_W,
	MAIN_RACE_SHAMAN_M,
#ifdef ENABLE_WOLFMAN_CHARACTER
	MAIN_RACE_WOLFMAN_M,
#endif
	MAIN_RACE_MAX_NUM,
};

enum EOther
{
	POISON_LENGTH = 30,
#ifdef ENABLE_WOLFMAN_CHARACTER
	BLEEDING_LENGTH = 30,
#endif
	STAMINA_PER_STEP = 1,
	SAFEBOX_PAGE_SIZE = 9,
	AI_CHANGE_ATTACK_POISITION_TIME_NEAR = 10000,
	AI_CHANGE_ATTACK_POISITION_TIME_FAR = 1000,
	AI_CHANGE_ATTACK_POISITION_DISTANCE = 100,
	SUMMON_MONSTER_COUNT = 3,
};

enum FlyTypes
{
	FLY_NONE,
	FLY_EXP,
	FLY_HP_MEDIUM,
	FLY_HP_BIG,
	FLY_SP_SMALL,
	FLY_SP_MEDIUM,
	FLY_SP_BIG,
	FLY_FIREWORK1,
	FLY_FIREWORK2,
	FLY_FIREWORK3,
	FLY_FIREWORK4,
	FLY_FIREWORK5,
	FLY_FIREWORK6,
	FLY_FIREWORK_CHRISTMAS,
	FLY_CHAIN_LIGHTNING,
	FLY_HP_SMALL,
	FLY_SKILL_MUYEONG,
#ifdef ENABLE_QUIVER_SYSTEM
	FLY_QUIVER_ATTACK_NORMAL,
#endif
};

enum EDamageType
{
	DAMAGE_TYPE_NONE,
	DAMAGE_TYPE_NORMAL,
	DAMAGE_TYPE_NORMAL_RANGE,
	//스킬
	DAMAGE_TYPE_MELEE,
	DAMAGE_TYPE_RANGE,
	DAMAGE_TYPE_FIRE,
	DAMAGE_TYPE_ICE,
	DAMAGE_TYPE_ELEC,
	DAMAGE_TYPE_MAGIC,
	DAMAGE_TYPE_POISON,
	DAMAGE_TYPE_SPECIAL,
#ifdef ENABLE_WOLFMAN_CHARACTER
	DAMAGE_TYPE_BLEEDING,
#endif
};
enum DamageFlag
{
	DAMAGE_NORMAL = (1 << 0),
	DAMAGE_POISON = (1 << 1),
	DAMAGE_DODGE = (1 << 2),
	DAMAGE_BLOCK = (1 << 3),
	DAMAGE_PENETRATE = (1 << 4),
	DAMAGE_CRITICAL = (1 << 5),
#if defined(ENABLE_WOLFMAN_CHARACTER) && !defined(USE_MOB_BLEEDING_AS_POISON)
	DAMAGE_BLEEDING = (1 << 6),
#endif
	DAMAGE_FIRE		= (1 << 7),
};

enum EPointTypes
{
	POINT_NONE,						// point_value = 0
	POINT_LEVEL,					// point_value = 1
	POINT_VOICE,					// point_value = 2
	POINT_EXP,						// point_value = 3
	POINT_NEXT_EXP,					// point_value = 4
	POINT_HP,						// point_value = 5
	POINT_MAX_HP,					// point_value = 6
	POINT_SP,						// point_value = 7
	POINT_MAX_SP,					// point_value = 8
	POINT_STAMINA,					// point_value = 9
	POINT_MAX_STAMINA,				// point_value = 10
	POINT_GOLD,						// point_value = 11
	POINT_ST,						// point_value = 12
	POINT_HT,						// point_value = 13
	POINT_DX,						// point_value = 14
	POINT_IQ,						// point_value = 15
	POINT_DEF_GRADE,				// point_value = 16
	POINT_ATT_SPEED,				// point_value = 17
	POINT_ATT_GRADE,				// point_value = 18
	POINT_MOV_SPEED,				// point_value = 19
	POINT_CLIENT_DEF_GRADE,			// point_value = 20
	POINT_CASTING_SPEED,			// point_value = 21
	POINT_MAGIC_ATT_GRADE,			// point_value = 22
	POINT_MAGIC_DEF_GRADE,			// point_value = 23
	POINT_EMPIRE_POINT,				// point_value = 24
	POINT_LEVEL_STEP,				// point_value = 25
	POINT_STAT,						// point_value = 26
	POINT_SUB_SKILL,				// point_value = 27
	POINT_SKILL,					// point_value = 28
	POINT_WEAPON_MIN,				// point_value = 29
	POINT_WEAPON_MAX,				// point_value = 30
	POINT_PLAYTIME,					// point_value = 31
	POINT_HP_REGEN,					// point_value = 32
	POINT_SP_REGEN,					// point_value = 33
	POINT_BOW_DISTANCE,				// point_value = 34
	POINT_HP_RECOVERY,				// point_value = 35
	POINT_SP_RECOVERY,				// point_value = 36
	POINT_POISON_PCT,				// point_value = 37
	POINT_STUN_PCT,					// point_value = 38
	POINT_SLOW_PCT,					// point_value = 39
	POINT_CRITICAL_PCT,				// point_value = 40
	POINT_PENETRATE_PCT,			// point_value = 41
	POINT_CURSE_PCT,				// point_value = 42
	POINT_ATTBONUS_HUMAN,			// point_value = 43
	POINT_ATTBONUS_ANIMAL,			// point_value = 44
	POINT_ATTBONUS_ORC,				// point_value = 45
	POINT_ATTBONUS_MILGYO,			// point_value = 46
	POINT_ATTBONUS_UNDEAD,			// point_value = 47
	POINT_ATTBONUS_DEVIL,			// point_value = 48
	POINT_ATTBONUS_INSECT,			// point_value = 49
	POINT_ATTBONUS_FIRE,			// point_value = 50
	POINT_ATTBONUS_ICE,				// point_value = 51
	POINT_ATTBONUS_DESERT,			// point_value = 52
	POINT_ATTBONUS_MONSTER,			// point_value = 53
	POINT_ATTBONUS_WARRIOR,			// point_value = 54
	POINT_ATTBONUS_ASSASSIN,		// point_value = 55
	POINT_ATTBONUS_SURA,			// point_value = 56
	POINT_ATTBONUS_SHAMAN,			// point_value = 57
	POINT_ATTBONUS_TREE,			// point_value = 58
	POINT_RESIST_WARRIOR,			// point_value = 59
	POINT_RESIST_ASSASSIN,			// point_value = 60
	POINT_RESIST_SURA,				// point_value = 61
	POINT_RESIST_SHAMAN,			// point_value = 62
	POINT_STEAL_HP,					// point_value = 63
	POINT_STEAL_SP,					// point_value = 64
	POINT_MANA_BURN_PCT,			// point_value = 65
	POINT_DAMAGE_SP_RECOVER,		// point_value = 66
	POINT_BLOCK,					// point_value = 67
	POINT_DODGE,					// point_value = 68
	POINT_RESIST_SWORD,				// point_value = 69
	POINT_RESIST_TWOHAND,			// point_value = 70
	POINT_RESIST_DAGGER,			// point_value = 71
	POINT_RESIST_BELL,				// point_value = 72
	POINT_RESIST_FAN,				// point_value = 73
	POINT_RESIST_BOW,				// point_value = 74
	POINT_RESIST_FIRE,				// point_value = 75
	POINT_RESIST_ELEC,				// point_value = 76
	POINT_RESIST_MAGIC,				// point_value = 77
	POINT_RESIST_WIND,				// point_value = 78
	POINT_REFLECT_MELEE,			// point_value = 79
	POINT_REFLECT_CURSE,			// point_value = 80
	POINT_POISON_REDUCE,			// point_value = 81
	POINT_KILL_SP_RECOVER,			// point_value = 82
	POINT_EXP_DOUBLE_BONUS,			// point_value = 83
	POINT_GOLD_DOUBLE_BONUS,		// point_value = 84
	POINT_ITEM_DROP_BONUS,			// point_value = 85
	POINT_POTION_BONUS,				// point_value = 86
	POINT_KILL_HP_RECOVERY,			// point_value = 87
	POINT_IMMUNE_STUN,				// point_value = 88
	POINT_IMMUNE_SLOW,				// point_value = 89
	POINT_IMMUNE_FALL,				// point_value = 90
	POINT_PARTY_ATTACKER_BONUS,		// point_value = 91
	POINT_PARTY_TANKER_BONUS,		// point_value = 92
	POINT_ATT_BONUS,				// point_value = 93
	POINT_DEF_BONUS,				// point_value = 94
	POINT_ATT_GRADE_BONUS,			// point_value = 95
	POINT_DEF_GRADE_BONUS,			// point_value = 96
	POINT_MAGIC_ATT_GRADE_BONUS,	// point_value = 97
	POINT_MAGIC_DEF_GRADE_BONUS,	// point_value = 98
	POINT_RESIST_NORMAL_DAMAGE,		// point_value = 99
	POINT_HIT_HP_RECOVERY,			// point_value = 100
	POINT_HIT_SP_RECOVERY,			// point_value = 101
	POINT_MANASHIELD,				// point_value = 102
	POINT_PARTY_BUFFER_BONUS,		// point_value = 103
	POINT_PARTY_SKILL_MASTER_BONUS,	// point_value = 104
	POINT_HP_RECOVER_CONTINUE,		// point_value = 105
	POINT_SP_RECOVER_CONTINUE,		// point_value = 106
	POINT_STEAL_GOLD,				// point_value = 107
	POINT_POLYMORPH,				// point_value = 108
	POINT_MOUNT,					// point_value = 109
	POINT_PARTY_HASTE_BONUS,		// point_value = 110
	POINT_PARTY_DEFENDER_BONUS,		// point_value = 111
	POINT_STAT_RESET_COUNT,			// point_value = 112
	POINT_HORSE_SKILL,				// point_value = 113
	POINT_MALL_ATTBONUS,			// point_value = 114
	POINT_MALL_DEFBONUS,			// point_value = 115
	POINT_MALL_EXPBONUS,			// point_value = 116
	POINT_MALL_ITEMBONUS,			// point_value = 117
	POINT_MALL_GOLDBONUS,			// point_value = 118
	POINT_MAX_HP_PCT,				// point_value = 119
	POINT_MAX_SP_PCT,				// point_value = 120
	POINT_SKILL_DAMAGE_BONUS,		// point_value = 121
	POINT_NORMAL_HIT_DAMAGE_BONUS,	// point_value = 122
	POINT_SKILL_DEFEND_BONUS,		// point_value = 123
	POINT_NORMAL_HIT_DEFEND_BONUS,	// point_value = 124
	POINT_RAMADAN_CANDY_BONUS_EXP,	// point_value = 127
	POINT_ENERGY,					// point_value = 128
	POINT_ENERGY_END_TIME,			// point_value = 129
	POINT_COSTUME_ATTR_BONUS,		// point_value = 130
	POINT_MAGIC_ATT_BONUS_PER,		// point_value = 131
	POINT_MELEE_MAGIC_ATT_BONUS_PER,// point_value = 132
	POINT_RESIST_ICE,				// point_value = 133
	POINT_RESIST_EARTH,				// point_value = 134
	POINT_RESIST_DARK,				// point_value = 135
	POINT_RESIST_CRITICAL,			// point_value = 136
	POINT_RESIST_PENETRATE,			// point_value = 137
#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_BLEEDING_PCT,				// point_value = 138
	POINT_BLEEDING_REDUCE,			// point_value = 139
	POINT_ATTBONUS_WOLFMAN,			// point_value = 140
	POINT_RESIST_WOLFMAN,			// point_value = 141
	POINT_RESIST_CLAW,				// point_value = 142
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	POINT_ANTI_RESIST_MAGIC = 143,		// Anti Magic Resistance;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
	POINT_CHEQUE = 145,
#endif
#ifdef ENABLE_GEM_SYSTEM
	POINT_GEM = 146,
#endif
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	POINT_ATTBONUS_SWORD = 147,
	POINT_ATTBONUS_TWOHANDED = 149,
	POINT_ATTBONUS_DAGGER = 150,
	POINT_ATTBONUS_BELL = 151,
	POINT_ATTBONUS_FAN = 152,
	POINT_ATTBONUS_BOW = 153,
#endif
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	POINT_ATTBONUS_ELEC = 154,
	POINT_ATTBONUS_WIND = 155,
	POINT_ATTBONUS_EARTH = 156,
	POINT_ATTBONUS_DARK = 157,
#endif
	POINT_RESIST_HUMAN = 158,
#ifdef ENABLE_EVENT_SYSTEM
	POINT_YEAR_ROUND = 159,
#endif
	POINT_ATTBONUS_STONE = 160,
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	POINT_ZODIAC = 163,
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	POINT_WORSHIP = 164,
#endif
	POINT_ATTBONUS_CZ = 165,
#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
	POINT_ATTBONUS_RAZADOR = 166,
	POINT_ATTBONUS_NEMERE = 167,
	POINT_ATTBONUS_HYDRA = 168,
	POINT_ATTBONUS_JOTUN_THRYM = 169,
	POINT_ATTBONUS_LUSIFER = 170,
	POINT_ATTBONUS_BERAN_SETAOU = 171,
	POINT_ATTBONUS_AZRAEL = 172,
	POINT_ATTBONUS_MELEY = 173,
#endif
#ifdef ENABLE_MUSHROOM_DUNGEON
	POINT_ATTBONUS_MUSHROOM = 174,
	POINT_RESIST_MUSHROOM = 175,
#endif
#ifdef ENABLE_WATER_DUNGEON
	POINT_ATTBONUS_WATER = 176,
	POINT_RESIST_WATER = 177,
#endif
#ifdef ENABLE_BONUS_TO_BOSS
	POINT_ATTBONUS_BOSS = 178,
#endif
	//POINT_MAX_NUM = 129,	-> common/length.h
};

enum EPKModes
{
	PK_MODE_PEACE,
	PK_MODE_REVENGE,
	PK_MODE_FREE,
	PK_MODE_PROTECT,
	PK_MODE_GUILD,
	PK_MODE_MAX_NUM
};

enum EPositions
{
	POS_DEAD,
	POS_SLEEPING,
	POS_RESTING,
	POS_SITTING,
	POS_FISHING,
	POS_FIGHTING,
	POS_MOUNTING,
	POS_STANDING
};

enum EBlockAction
{
	BLOCK_EXCHANGE = (1 << 0),
	BLOCK_PARTY_INVITE = (1 << 1),
	BLOCK_GUILD_INVITE = (1 << 2),
	BLOCK_WHISPER = (1 << 3),
	BLOCK_MESSENGER_INVITE = (1 << 4),
	BLOCK_PARTY_REQUEST = (1 << 5),
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	BLOCK_EQUIPMENT_REQUEST = (1 << 6),
#endif
};

enum ESkillUpBy
{
	SKILL_UP_BY_POINT = 0,
	SKILL_UP_BY_BOOK = 1,
	SKILL_UP_BY_TRAIN = 2,
	SKILL_UP_BY_QUEST = 3,
};

// <Factor> Dynamically evaluated CHARACTER* equivalent.
// Referring to SCharDeadEventInfo.
struct DynamicCharacterPtr {
	DynamicCharacterPtr() : is_pc(false), id(0) {}
	DynamicCharacterPtr(const DynamicCharacterPtr& o)
		: is_pc(o.is_pc), id(o.id) {}

	// Returns the LPCHARACTER found in CHARACTER_MANAGER.
	LPCHARACTER Get() const;
	// Clears the current settings.
	void Reset() {
		is_pc = false;
		id = 0;
	}

	// Basic assignment operator.
	DynamicCharacterPtr& operator=(const DynamicCharacterPtr& rhs) {
		is_pc = rhs.is_pc;
		id = rhs.id;
		return *this;
	}
	// Supports assignment with LPCHARACTER type.
	DynamicCharacterPtr& operator=(LPCHARACTER character);
	// Supports type casting to LPCHARACTER.
	operator LPCHARACTER() const {
		return Get();
	}

	bool is_pc;
	uint32_t id;
};

/* 저장하는 데이터 */
typedef struct character_point
{
	long			points[POINT_MAX_NUM];

	BYTE			job;
	BYTE			voice;

	BYTE			level;
	DWORD			exp;
	long			gold;

#ifdef ENABLE_CHEQUE_SYSTEM
	int				cheque;
#endif
#ifdef ENABLE_GEM_SYSTEM
	long			gem;
#endif
	int				hp;
	int				sp;

	int				iRandomHP;
	int				iRandomSP;

	int				stamina;

	BYTE			skill_group;

#ifdef ENABLE_EVENT_SYSTEM
	int				year_round_point;
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	short			extend_inventory;
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	WORD			zodiac_point;
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	WORD			lWorshipPoint;
#endif
#ifdef ENABLE_DUNGEON_BACK_SYSTEM
	long			dungeonindex;
#endif
} CHARACTER_POINT;

/* 저장되지 않는 캐릭터 데이터 */
typedef struct character_point_instant
{
	long			points[POINT_MAX_NUM];

	float			fRot;

	int				iMaxHP;
	int				iMaxSP;

	long			position;

	long			instant_flag;
	DWORD			dwAIFlag;
	DWORD			dwImmuneFlag;
	DWORD			dwLastShoutPulse;

	WORD			parts[PART_MAX_NUM];

	LPITEM			pItems[INVENTORY_AND_EQUIP_SLOT_MAX];
	UINT			bItemGrid[INVENTORY_AND_EQUIP_SLOT_MAX];

	// 용혼석 인벤토리.
	LPITEM			pDSItems[DRAGON_SOUL_INVENTORY_MAX_NUM];
	WORD			wDSItemGrid[DRAGON_SOUL_INVENTORY_MAX_NUM];

	// by mhh
	LPITEM			pCubeItems[CUBE_MAX_NUM];
	LPCHARACTER		pCubeNpc;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	LPITEM			pClMaterials[CL_WINDOW_MAX_MATERIALS];
#endif
#ifdef ENABLE_ACCE_SYSTEM
	LPITEM			pSashMaterials[SASH_WINDOW_MAX_MATERIALS];
#endif
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	LPITEM			pCombItems[COMB_MAX_NUM];
	LPCHARACTER		pCombNpc;
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	LPITEM			pSkillBookItems[SKILL_BOOK_INVENTORY_MAX_NUM];
	WORD			wSkillBookItemsGrid[SKILL_BOOK_INVENTORY_MAX_NUM];

	LPITEM			pUpgradeItems[UPGRADE_ITEMS_INVENTORY_MAX_NUM];
	WORD			pUpgradeItemsGrid[UPGRADE_ITEMS_INVENTORY_MAX_NUM];

	LPITEM			pStoneItems[STONE_ITEMS_INVENTORY_MAX_NUM];
	WORD			pStoneItemsGrid[STONE_ITEMS_INVENTORY_MAX_NUM];

	LPITEM			pChestItems[CHEST_ITEMS_INVENTORY_MAX_NUM];
	WORD			pChestItemsGrid[CHEST_ITEMS_INVENTORY_MAX_NUM];
	
	LPITEM			pAttrItems[ATTR_ITEMS_INVENTORY_MAX_NUM];
	WORD			pAttrItemsGrid[ATTR_ITEMS_INVENTORY_MAX_NUM];
	
	LPITEM			pFlowerItems[FLOWER_ITEMS_INVENTORY_MAX_NUM];
	WORD			pFlowerItemsGrid[FLOWER_ITEMS_INVENTORY_MAX_NUM];
#endif
#ifdef ENABLE_AURA_SYSTEM
	LPITEM			pAuraMaterials[AURA_WINDOW_MAX_MATERIALS];
#endif

	LPCHARACTER			battle_victim;

	BYTE			gm_level;

	BYTE			bBasePart;	// 평상복 번호

	int				iMaxStamina;

	int				bBlockMode;

	int				iDragonSoulActiveDeck;
	LPENTITY		m_pDragonSoulRefineWindowOpener;
} CHARACTER_POINT_INSTANT;

#define TRIGGERPARAM		LPCHARACTER ch, LPCHARACTER causer

typedef struct trigger
{
	BYTE	type;
	int		(*func) (TRIGGERPARAM);
	long	value;
} TRIGGER;

class CTrigger
{
public:
	CTrigger() : bType(0), pFunc(nullptr)
	{
	}

	BYTE	bType;
	int	(*pFunc) (TRIGGERPARAM);
};

EVENTINFO(char_event_info)
{
	DynamicCharacterPtr ch;
};

struct TSkillUseInfo
{
	int	    iHitCount;
	int	    iMaxHitCount;
	int	    iSplashCount;
	DWORD   dwNextSkillUsableTime;
	int	    iRange;
	bool    bUsed;
	DWORD   dwVID;
	bool    isGrandMaster;

	std::unordered_map<DWORD, size_t> TargetVIDMap;

	TSkillUseInfo()
		: iHitCount(0), iMaxHitCount(0), iSplashCount(0), dwNextSkillUsableTime(0), iRange(0), bUsed(false),
		dwVID(0), isGrandMaster(false)
	{}

	bool    HitOnce(DWORD dwVnum = 0);

	bool    UseSkill(bool isGrandMaster, DWORD vid, DWORD dwCooltime, int splashcount = 1, int hitcount = -1, int range = -1);
	DWORD   GetMainTargetVID() const { return dwVID; }
	void    SetMainTargetVID(DWORD vid) { dwVID = vid; }
	void    ResetHitCount() { if (iSplashCount) { iHitCount = iMaxHitCount; iSplashCount--; } }
};

typedef struct packet_party_update TPacketGCPartyUpdate;
class CExchange;
class CSkillProto;
class CParty;
class CDungeon;
class CWarMap;
class CAffect;
class CGuild;
class CSafebox;
class CArena;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "../../libgame/libgame/grid.h"
typedef struct SPrivShop
{
	DWORD	shop_id;
	DWORD	shop_vid;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	item_count;
	BYTE	rest_count;
	BYTE	days;
	DWORD	date_close;
	long long gold;
	int won;
} TPrivShop;

typedef std::map<DWORD, TPrivShop> PSHOP_MAP;
#endif

#ifdef ENABLE_GIFTBOX_SYSTEM
typedef struct SGiftItem
{
	DWORD	id;
	WORD	pos;
	DWORD count;
	DWORD	vnum;
	long	alSockets[ITEM_SOCKET_MAX_NUM];	// LN?arCL
	DWORD	transmutation;
	char	specific_item_name[64];

	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];
	char szFrom[101];
	char szReason[101];
	DWORD dwDateAdd;
} TGiftItem;
typedef std::map<int, std::vector<TGiftItem> > GIFT_MAP;
#endif

class CShop;
typedef class CShop* LPSHOP;

class CMob;
class CMobInstance;
typedef struct SMobSkillInfo TMobSkillInfo;

//SKILL_POWER_BY_LEVEL
extern int GetSkillPowerByLevelFromType(int job, int skillgroup, int skilllevel);
//END_SKILL_POWER_BY_LEVEL

namespace marriage
{
	class WeddingMap;
}

class CHARACTER : public CEntity, public CFSM, public CHorseRider
{
protected:
	//////////////////////////////////////////////////////////////////////////////////
	// Entity 관련
	virtual void	EncodeInsertPacket(LPENTITY entity);
	virtual void	EncodeRemovePacket(LPENTITY entity);
	//////////////////////////////////////////////////////////////////////////////////

public:
	LPCHARACTER			FindCharacterInView(const char* name, bool bFindPCOnly);
	void				UpdatePacket();
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
	void				UpdateAffectFlag();
#endif

	//////////////////////////////////////////////////////////////////////////////////
	// FSM (Finite State Machine) 관련
protected:
	CStateTemplate<CHARACTER>	m_stateMove;
	CStateTemplate<CHARACTER>	m_stateBattle;
	CStateTemplate<CHARACTER>	m_stateIdle;

public:
	virtual void		StateMove();
	virtual void		StateBattle();
	virtual void		StateIdle();
	virtual void		StateFlag();
	virtual void		StateFlagBase();
	void				StateHorse();

protected:
	// STATE_IDLE_REFACTORING
	void				__StateIdle_Monster();
	void				__StateIdle_Stone();
	void				__StateIdle_NPC();
	// END_OF_STATE_IDLE_REFACTORING

public:
	DWORD GetAIFlag() const { return m_pointsInstant.dwAIFlag; }

	void				SetAggressive();
	bool				IsAggressive() const;

	void				SetCoward();
	bool				IsCoward() const;
	void				CowardEscape();

	void				SetNoAttackShinsu();
	bool				IsNoAttackShinsu() const;

	void				SetNoAttackChunjo();
	bool				IsNoAttackChunjo() const;

	void				SetNoAttackJinno();
	bool				IsNoAttackJinno() const;

	void				SetAttackMob();
	bool				IsAttackMob() const;

	virtual void			BeginStateEmpty();
	virtual void			EndStateEmpty() {}

	void				RestartAtSamePos();

protected:
	DWORD				m_dwStateDuration;
	//////////////////////////////////////////////////////////////////////////////////

public:
	CHARACTER();
	virtual ~CHARACTER();

	void			Create(const char* c_pszName, DWORD vid, bool isPC);
	void			Destroy();

	void			Disconnect(const char* c_pszReason);
	void			ChannelChange(short sChannel);

protected:
	void			Initialize();

	//////////////////////////////////////////////////////////////////////////////////
	// Basic Points

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
private:
	DWORD			dwLastTargetInfoPulse;

public:
	DWORD			GetLastTargetInfoPulse() const { return dwLastTargetInfoPulse; }
	void			SetLastTargetInfoPulse(DWORD pulse) { dwLastTargetInfoPulse = pulse; }
#endif

public:
	DWORD			GetPlayerID() const { return m_dwPlayerID; }

	void			SetPlayerProto(const TPlayerTable* table);
	void			CreatePlayerProto(TPlayerTable& tab);	// 저장 시 사용

	void			SetProto(const CMob* c_pkMob);
	WORD			GetRaceNum() const;

	void			Save();		// DelayedSave
	void			SaveReal();	// 실제 저장
	void			FlushDelayedSaveItem();

	const char* GetName() const;
	const VID& GetVID() const { return m_vid; }

	void			SetName(const std::string& name) { m_stName = name; }

	void			SetRace(BYTE race);
	bool			ChangeSex();

	DWORD			GetAID() const;
	int				GetChangeEmpireCount() const;
	void			SetChangeEmpireCount();
	int				ChangeEmpire(BYTE empire);

	BYTE			GetJob() const;
	BYTE			GetCharType() const;

	bool			IsPC() const { return GetDesc() ? true : false; }
	bool			IsNPC()	const { return m_bCharType != CHAR_TYPE_PC; }
	bool			IsMonster()	const { return m_bCharType == CHAR_TYPE_MONSTER; }
	bool			IsStone() const { return m_bCharType == CHAR_TYPE_STONE; }
	bool			IsDoor() const { return m_bCharType == CHAR_TYPE_DOOR; }
	bool			IsBuilding() const { return m_bCharType == CHAR_TYPE_BUILDING; }
	bool			IsWarp() const { return m_bCharType == CHAR_TYPE_WARP; }
	bool			IsGoto() const { return m_bCharType == CHAR_TYPE_GOTO; }
	//		bool			IsPet() const		{ return m_bCharType == CHAR_TYPE_PET; }

	DWORD			GetLastShoutPulse() const { return m_pointsInstant.dwLastShoutPulse; }
	void			SetLastShoutPulse(DWORD pulse) { m_pointsInstant.dwLastShoutPulse = pulse; }
	int				GetLevel() const { return m_points.level; }
	void			SetLevel(BYTE level);

	BYTE			GetGMLevel() const;
	BOOL 			IsGM() const;
	void			SetGMLevel();
	int				GetChannel() const;

	DWORD			GetExp() const { return m_points.exp; }
	void			SetExp(DWORD exp) { m_points.exp = exp; }
	bool            ModoBatalha;
	DWORD			GetNextExp() const;
#ifdef ENABLE_GROWTH_PET_SYSTEM
	DWORD			PetGetNextExp() const;
#endif
	LPCHARACTER		DistributeExp();	// 제일 많이 때린 사람을 리턴한다.
	void			DistributeHP(LPCHARACTER pkKiller);
	void			DistributeSP(LPCHARACTER pkKiller, int iMethod = 0);

	void			SetPosition(int pos);
	bool			IsPosition(int pos) const { return m_pointsInstant.position == pos ? true : false; }
	int				GetPosition() const { return m_pointsInstant.position; }

	void			SetPart(BYTE bPartPos, WORD wVal);
	WORD			GetPart(BYTE bPartPos) const;
	WORD			GetOriginalPart(BYTE bPartPos) const;

	void			SetHP(int hp) { m_points.hp = hp; }
	int				GetHP() const { return m_points.hp; }

	void			SetSP(int sp) { m_points.sp = sp; }
	int				GetSP() const { return m_points.sp; }

	void			SetStamina(int stamina) { m_points.stamina = stamina; }
	int				GetStamina() const { return m_points.stamina; }

	void			SetMaxHP(int iVal) { m_pointsInstant.iMaxHP = iVal; }
	int				GetMaxHP() const { return m_pointsInstant.iMaxHP; }

	void			SetMaxSP(int iVal) { m_pointsInstant.iMaxSP = iVal; }
	int				GetMaxSP() const { return m_pointsInstant.iMaxSP; }

	void			SetMaxStamina(int iVal) { m_pointsInstant.iMaxStamina = iVal; }
	int				GetMaxStamina() const { return m_pointsInstant.iMaxStamina; }

	void			SetRandomHP(int v) { m_points.iRandomHP = v; }
	void			SetRandomSP(int v) { m_points.iRandomSP = v; }

	int				GetRandomHP() const { return m_points.iRandomHP; }
	int				GetRandomSP() const { return m_points.iRandomSP; }

	int				GetHPPct() const;

	void			SetRealPoint(BYTE idx, int val);
	int				GetRealPoint(BYTE idx) const;

	void			SetPoint(BYTE idx, int val);
	int				GetPoint(BYTE idx) const;
	int				GetLimitPoint(BYTE idx) const;
	int				GetPolymorphPoint(BYTE idx) const;

	const TMobTable& GetMobTable() const;
	BYTE				GetMobRank() const;
	BYTE				GetMobBattleType() const;
	BYTE				GetMobSize() const;
	DWORD				GetMobDamageMin() const;
	DWORD				GetMobDamageMax() const;
	WORD				GetMobAttackRange() const;
	DWORD				GetMobDropItemVnum() const;
	float				GetMobDamageMultiply() const;

	// NEWAI
	bool			IsBerserker() const;
	bool			IsBerserk() const;
	void			SetBerserk(bool mode);

	bool			IsStoneSkinner() const;

	bool			IsGodSpeeder() const;
	bool			IsGodSpeed() const;
	void			SetGodSpeed(bool mode);

	bool			IsDeathBlower() const;
	bool			IsDeathBlow() const;

	bool			IsReviver() const;
	bool			HasReviverInParty() const;
	bool			IsRevive() const;
	void			SetRevive(bool mode);
	// NEWAI END

	bool			IsRaceFlag(DWORD dwBit) const;
	bool			IsSummonMonster() const;
	DWORD			GetSummonVnum() const;

	DWORD			GetPolymorphItemVnum() const;
	DWORD			GetMonsterDrainSPPoint() const;

	void			MainCharacterPacket();	// 내가 메인캐릭터라고 보내준다.

	void			ComputePoints();
#ifdef ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET
	void			ComputePointsWithoutUpdatePacket();
#endif
	void			ComputeBattlePoints();
	void			PointChange(BYTE type, int amount, bool bAmount = false, bool bBroadcast = false);
	void			PointsPacket();
	void			ApplyPoint(BYTE bApplyType, int iVal);
#ifdef ENABLE_GROWTH_PET_SYSTEM
	void			SendPetLevelUpEffect(int vid, int type, int value, int amount);
#endif
	void			CheckMaximumPoints();	// HP, SP 등의 현재 값이 최대값 보다 높은지 검사하고 높다면 낮춘다.

	bool			Show(long lMapIndex, long x, long y, long z = LONG_MAX, bool bShowSpawnMotion = false, bool bAggressive = false);

	void			Sitdown(int is_ground);
	void			Standup();

	void			SetRotation(float fRot);
	void			SetRotationToXY(long x, long y);
	float			GetRotation() const { return m_pointsInstant.fRot; }

	void			MotionPacketEncode(BYTE motion, LPCHARACTER victim, struct packet_motion* packet);
	void			Motion(BYTE motion, LPCHARACTER victim = nullptr);

	void			ChatPacket(BYTE type, const char* format, ...);
	void			MonsterChat(BYTE bMonsterChatType);

	void			ResetPoint(int iLv);

	void			SetBlockMode(int bFlag);
	void			SetBlockModeForce(int bFlag);
	bool			IsBlockMode(int bFlag) const { return (m_pointsInstant.bBlockMode & bFlag) ? true : false; }

	bool			IsPolymorphed() const { return m_dwPolymorphRace > 0; }
	bool			IsPolyMaintainStat() const { return m_bPolyMaintainStat; } // 이전 스텟을 유지하는 폴리모프.
	void			SetPolymorph(DWORD dwRaceNum, bool bMaintainStat = false);
	DWORD			GetPolymorphVnum() const { return m_dwPolymorphRace; }
	int				GetPolymorphPower() const;
	bool IsNearWater()const;
	// FISING
	void			fishing();
	void			fishing_take();
	// END_OF_FISHING

	// MINING
	void			mining(LPCHARACTER chLoad);
	void			mining_cancel();
	void			mining_take();
	// END_OF_MINING

	void			ResetPlayTime(DWORD dwTimeRemain = 0);

	void			CreateFly(BYTE bType, LPCHARACTER pkVictim);

	void			ResetChatCounter();
	BYTE			IncreaseChatCounter();
	BYTE			GetChatCounter() const;
protected:
	DWORD			m_dwPolymorphRace;
	bool			m_bPolyMaintainStat;
	DWORD			m_dwLoginPlayTime;
	DWORD			m_dwPlayerID;
	VID				m_vid;
	std::string		m_stName;
#ifdef ENABLE_GROWTH_PET_SYSTEM
	BYTE			m_stImmortalSt;
#endif
	BYTE			m_bCharType;
#ifdef ENABLE_GROWTH_PET_SYSTEM
	DWORD			m_newpetskillcd[3];
#endif

	CHARACTER_POINT		m_points;
	CHARACTER_POINT_INSTANT	m_pointsInstant;

	int				m_iMoveCount;
	DWORD			m_dwPlayStartTime;
	BYTE			m_bAddChrState;
	bool			m_bSkipSave;
	BYTE			m_bChatCounter;
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	BYTE m_bMountCounter;
#endif
	// End of Basic Points

	//////////////////////////////////////////////////////////////////////////////////
	// Move & Synchronize Positions
	//////////////////////////////////////////////////////////////////////////////////
public:
	bool			IsStateMove() const { return IsState((CState&)m_stateMove); }
	bool			IsStateIdle() const { return IsState((CState&)m_stateIdle); }
	bool			IsWalking() const { return m_bNowWalking || GetStamina() <= 0; }
	void			SetWalking(bool bWalkFlag) { m_bWalking = bWalkFlag; }
	void			SetNowWalking(bool bWalkFlag);
	void			ResetWalking() { SetNowWalking(m_bWalking); }

	bool			Goto(long x, long y);	// 바로 이동 시키지 않고 목표 위치로 BLENDING 시킨다.
	void			Stop();

	bool			CanMove() const;		// 이동할 수 있는가?

	void			SyncPacket();
	bool			Sync(long x, long y);	// 실제 이 메소드로 이동 한다 (각 종 조건에 의한 이동 불가가 없음)
	bool			Move(long x, long y);	// 조건을 검사하고 Sync 메소드를 통해 이동 한다.
	void			OnMove(bool bIsAttack = false);	// 움직일때 불린다. Move() 메소드 이외에서도 불릴 수 있다.
	DWORD			GetMotionMode() const;
	float			GetMoveMotionSpeed() const;
	float			GetMoveSpeed() const;
	void			CalculateMoveDuration();
	void			SendMovePacket(BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime = 0, int iRot = -1);
#ifdef ENABLE_BUFFI_SYSTEM
	void			SendSupportShamanSkillPacket(DWORD skill_vnum);
	DWORD			SupportShamanGetNextExp() const;
#endif
	DWORD			GetCurrentMoveDuration() const { return m_dwMoveDuration; }
	DWORD			GetWalkStartTime() const { return m_dwWalkStartTime; }
	DWORD			GetLastMoveTime() const { return m_dwLastMoveTime; }
	DWORD			GetLastAttackTime() const { return m_dwLastAttackTime; }

	void			SetLastAttacked(DWORD time);	// 마지막으로 공격받은 시간 및 위치를 저장함

	bool			SetSyncOwner(LPCHARACTER ch, bool bRemoveFromList = true);
	bool			IsSyncOwner(LPCHARACTER ch) const;

	bool			WarpSet(long x, long y, long lRealMapIndex = 0);
#ifdef ENABLE_CHANNEL_SWITCH_SYSTEM
	bool			SwitchChannel(long newAddr, WORD newPort);
	bool			StartChannelSwitch(long newAddr, WORD newPort);
#endif
	void			SetWarpLocation(long lMapIndex, long x, long y);
	void			WarpEnd();
	const PIXEL_POSITION& GetWarpPosition() const { return m_posWarp; }
	bool			WarpToPID(DWORD dwPID);

	void			SaveExitLocation();
	void			ExitToSavedLocation();

	void			StartStaminaConsume();
	void			StopStaminaConsume();
	bool			IsStaminaConsume() const;
	bool			IsStaminaHalfConsume() const;

	void			ResetStopTime();
	DWORD			GetStopTime() const;

#ifdef ENABLE_GUILD_SPY_SYSTEM
	int				m_iWarKillCount;
	int				m_iWarDeadCount;
	int				GetWarKillCount() { return m_iWarKillCount; }
	void			IncWarKillCount() { ++m_iWarKillCount; }
	void			ResetWarKillCount() { m_iWarKillCount = 0; }

	int				GetWarDeadCount() { return m_iWarDeadCount; }
	void			IncWarDeadCount() { ++m_iWarDeadCount; }
	void			ResetWarDeadCount() { m_iWarDeadCount = 0; }
#endif

protected:
	void			ClearSync();

	float			m_fSyncTime;
	LPCHARACTER		m_pkChrSyncOwner;
	CHARACTER_LIST	m_kLst_pkChrSyncOwned;	// 내가 SyncOwner인 자들

	PIXEL_POSITION	m_posDest;
	PIXEL_POSITION	m_posStart;
	PIXEL_POSITION	m_posWarp;
	long			m_lWarpMapIndex;

	PIXEL_POSITION	m_posExit;
	long			m_lExitMapIndex;

	DWORD			m_dwMoveStartTime;
	DWORD			m_dwMoveDuration;

	DWORD			m_dwLastMoveTime;
	DWORD			m_dwLastAttackTime;
	DWORD			m_dwWalkStartTime;
	DWORD			m_dwStopTime;

	bool			m_bWalking;
	bool			m_bNowWalking;
	bool			m_bStaminaConsume;
	// End

	// Quickslot 관련
public:
	void			SyncQuickslot(BYTE bType, UINT bOldPos, UINT bNewPos);
	int32_t			GetQuickslotPosition(uint8_t bType, uint16_t bInventoryPos) const;
	bool			GetQuickslot(UINT pos, TQuickslot** ppSlot);
	bool			SetQuickslot(UINT pos, TQuickslot& rSlot);
	bool			DelQuickslot(UINT pos);
	bool			SwapQuickslot(UINT a, UINT b);
	void			ChainQuickslotItem(LPITEM pItem, BYTE bType, UINT bOldPos);
protected:
	TQuickslot		m_quickslot[QUICKSLOT_MAX_NUM];
#ifdef ENABLE_FISH_JIGSAW_EVENT
	TPlayerFishEventSlot* m_fishSlots;
#endif
#ifdef ENABLE_GEM_SYSTEM
	TPlayerGemItems* m_gemItems;
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	TPlayerGemItems* m_gemBossItems;
	#endif
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Affect
public:
	void			StartAffectEvent();
	void			ClearAffect(bool bSave = false);
	void			ClearAffect_New(bool bSave = false);
	void			ComputeAffect(CAffect* pkAff, bool bAdd);
	bool			AddAffect(DWORD dwType, BYTE bApplyOn, long lApplyValue, DWORD dwFlag, long lDuration, long lSPCost, bool bOverride, bool IsCube = false);
	void			RefreshAffect();
	bool			RemoveAffect(DWORD dwType);
	bool			IsAffectFlag(DWORD dwAff) const;
	
	void			SetAffectStack(CAffect * pkAff, uint8_t value);
	uint8_t			GetAffectStack(CAffect * pkAff);
	void			ClearAffectStack(CAffect * pkAff);
	
	std::unordered_map< uint32_t, uint8_t > m_map_affectStack;

	bool			UpdateAffect();	// called from EVENT
	int				ProcessAffect();

	void			LoadAffect(DWORD dwCount, TPacketAffectElement* pElements);
	void			SaveAffect();

	// Affect loading이 끝난 상태인가?
	bool			IsLoadedAffect() const { return m_bIsLoadedAffect; }

	bool			IsGoodAffect(BYTE bAffectType) const;

	void			RemoveGoodAffect();
	void			RemoveBadAffect();

	CAffect* FindAffect(DWORD dwType, BYTE bApply = APPLY_NONE) const;
	CAffect* FindAffectByFlag(DWORD dwFlag) const;
	const std::list<CAffect*>& GetAffectContainer() const { return m_list_pkAffect; }
	bool			RemoveAffect(CAffect* pkAff);

protected:
	bool			m_bIsLoadedAffect;
	TAffectFlag		m_afAffectFlag;
	std::list<CAffect*>	m_list_pkAffect;

public:
	// PARTY_JOIN_BUG_FIX
	void			SetParty(LPPARTY pkParty);
	LPPARTY			GetParty() const { return m_pkParty; }

	bool			RequestToParty(LPCHARACTER leader);
	void			DenyToParty(LPCHARACTER member);
	void			AcceptToParty(LPCHARACTER member);

	/// 자신의 파티에 다른 character 를 초대한다.
	/**
	 * @param	pchInvitee 초대할 대상 character. 파티에 참여 가능한 상태이어야 한다.
	 *
	 * 양측 character 의 상태가 파티에 초대하고 초대받을 수 있는 상태가 아니라면 초대하는 캐릭터에게 해당하는 채팅 메세지를 전송한다.
	 */
	void			PartyInvite(LPCHARACTER pchInvitee);

	/// 초대했던 character 의 수락을 처리한다.
	/**
	 * @param	pchInvitee 파티에 참여할 character. 파티에 참여가능한 상태이어야 한다.
	 *
	 * pchInvitee 가 파티에 가입할 수 있는 상황이 아니라면 해당하는 채팅 메세지를 전송한다.
	 */
	void			PartyInviteAccept(LPCHARACTER pchInvitee);

	/// 초대했던 character 의 초대 거부를 처리한다.
	/**
	 * @param [in]	dwPID 초대 했던 character 의 PID
	 */
	void			PartyInviteDeny(DWORD dwPID);

	bool			BuildUpdatePartyPacket(TPacketGCPartyUpdate& out);
	int				GetLeadershipSkillLevel() const;

	bool			CanSummon(int iLeaderShip);

	void			SetPartyRequestEvent(LPEVENT pkEvent) { m_pkPartyRequestEvent = pkEvent; }

protected:

	/// 파티에 가입한다.
	/**
	 * @param	pkLeader 가입할 파티의 리더
	 */
	void			PartyJoin(LPCHARACTER pkLeader);

	/**
	 * 파티 가입을 할 수 없을 경우의 에러코드.
	 * Error code 는 시간에 의존적인가에 따라 변경가능한(mutable) type 과 정적(static) type 으로 나뉜다.
	 * Error code 의 값이 PERR_SEPARATOR 보다 낮으면 변경가능한 type 이고 높으면 정적 type 이다.
	 */
	enum PartyJoinErrCode {
		PERR_NONE = 0,	///< 처리성공
		PERR_SERVER,			///< 서버문제로 파티관련 처리 불가
		PERR_DUNGEON,			///< 캐릭터가 던전에 있음
		PERR_OBSERVER,			///< 관전모드임
		PERR_LVBOUNDARY,		///< 상대 캐릭터와 레벨차이가 남
		PERR_LOWLEVEL,			///< 상대파티의 최고레벨보다 30레벨 낮음
		PERR_HILEVEL,			///< 상대파티의 최저레벨보다 30레벨 높음
		PERR_ALREADYJOIN,		///< 파티가입 대상 캐릭터가 이미 파티중
		PERR_PARTYISFULL,		///< 파티인원 제한 초과
		PERR_SEPARATOR,			///< Error type separator.
		PERR_DIFFEMPIRE,		///< 상대 캐릭터와 다른 제국임
		PERR_MAX				///< Error code 최고치. 이 앞에 Error code 를 추가한다.
	};

	/// 파티 가입이나 결성 가능한 조건을 검사한다.
	/**
	 * @param 	pchLeader 파티의 leader 이거나 초대한 character
	 * @param	pchGuest 초대받는 character
	 * @return	모든 PartyJoinErrCode 가 반환될 수 있다.
	 */
	static PartyJoinErrCode	IsPartyJoinableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest);

	/// 파티 가입이나 결성 가능한 동적인 조건을 검사한다.
	/**
	 * @param 	pchLeader 파티의 leader 이거나 초대한 character
	 * @param	pchGuest 초대받는 character
	 * @return	mutable type 의 code 만 반환한다.
	 */
	static PartyJoinErrCode	IsPartyJoinableMutableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest);

	LPPARTY			m_pkParty;
	DWORD			m_dwLastDeadTime;
	LPEVENT			m_pkPartyRequestEvent;

	/**
	 * 파티초청 Event map.
	 * key: 초대받은 캐릭터의 PID
	 * value: event의 pointer
	 *
	 * 초대한 캐릭터들에 대한 event map.
	 */
	typedef std::map< DWORD, LPEVENT >	EventMap;
	EventMap		m_PartyInviteEventMap;

	// END_OF_PARTY_JOIN_BUG_FIX

	////////////////////////////////////////////////////////////////////////////////////////
	// Dungeon
public:
	void			SetDungeon(LPDUNGEON pkDungeon);
	LPDUNGEON		GetDungeon() const { return m_pkDungeon; }
	LPDUNGEON		GetDungeonForce() const;
protected:
	LPDUNGEON	m_pkDungeon;
	int			m_iEventAttr;

	////////////////////////////////////////////////////////////////////////////////////////
	// Guild
public:
	void			SetGuild(CGuild* pGuild);
	CGuild* GetGuild() const { return m_pGuild; }

	void			SetWarMap(CWarMap* pWarMap);
	CWarMap* GetWarMap() const { return m_pWarMap; }

protected:
	CGuild* m_pGuild;
	DWORD			m_dwUnderGuildWarInfoMessageTime;
	CWarMap* m_pWarMap;

	////////////////////////////////////////////////////////////////////////////////////////
	// Item related
public:
	bool			CanHandleItem(bool bSkipRefineCheck = false, bool bSkipObserver = false); // 아이템 관련 행위를 할 수 있는가?

	bool			IsItemLoaded() const { return m_bItemLoaded; }
	void			SetItemLoaded() { m_bItemLoaded = true; }

	void			ClearItem();

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
	void	SetItem(TItemPos Cell, LPITEM item, bool isHighLight = false);
#else
	void	SetItem(TItemPos Cell, LPITEM item);
#endif
	LPITEM			GetItem(TItemPos Cell) const;
	LPITEM			GetItem_NEW(const TItemPos& Cell) const;
	LPITEM			GetInventoryItem(WORD wCell) const;
	LPITEM			GetDragonSoulInventoryItem(WORD wCell) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	LPITEM			GetSkillBookInventoryItem(WORD wCell) const;
	LPITEM			GetUpgradeItemsInventoryItem(WORD wCell) const;
	LPITEM			GetStoneItemsInventoryItem(WORD wCell) const;
	LPITEM			GetChestItemsInventoryItem(WORD wCell) const;
	LPITEM			GetAttrItemsInventoryItem(WORD wCell) const;
	LPITEM			GetFlowerItemsInventoryItem(WORD wCell) const;
#endif
	bool			IsEmptyItemGrid(TItemPos Cell, BYTE size, int iExceptionCell = -1) const;
	bool			IsEmptyItemGridSpecial(const TItemPos& Cell, BYTE bSize, int iExceptionCell, std::vector<WORD>& vec) const;

	void			SetWear(UINT bCell, LPITEM item);
	LPITEM 			GetWear(UINT bCell) const;

	// MYSHOP_PRICE_LIST
	void			UseSilkBotary(void); 		/// 비단 보따리 아이템의 사용

	/// DB 캐시로 부터 받아온 가격정보 리스트를 유저에게 전송하고 보따리 아이템 사용을 처리한다.
	/**
	 * @param [in] p	가격정보 리스트 패킷
	 *
	 * 접속한 후 처음 비단 보따리 아이템 사용 시 UseSilkBotary 에서 DB 캐시로 가격정보 리스트를 요청하고
	 * 응답받은 시점에 이 함수에서 실제 비단보따리 사용을 처리한다.
	 */
	void			UseSilkBotaryReal(const TPacketMyshopPricelistHeader* p);
	// END_OF_MYSHOP_PRICE_LIST

	bool			UseItemEx(LPITEM item, TItemPos DestCell);
	bool			UseItem(TItemPos Cell, TItemPos DestCell = NPOS);

	// ADD_REFINE_BUILDING
	bool			IsRefineThroughGuild() const;
	CGuild* GetRefineGuild() const;
	int				ComputeRefineFee(int iCost, int iMultiply = 5) const;
	void			PayRefineFee(int iTotalMoney);
	void			SetRefineNPC(LPCHARACTER ch);
	// END_OF_ADD_REFINE_BUILDING

	bool			RefineItem(LPITEM pkItem, LPITEM pkTarget);
#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
	bool			DeleteItem(TItemPos Cell);
	bool			SellItem(TItemPos Cell);
#else
	bool			DropItem(TItemPos Cell, BYTE bCount = 0);
#endif
	bool			GiveRecallItem(LPITEM item);
	void			ProcessRecallItem(LPITEM item);

	//	void			PotionPacket(int iPotionType);
	void			EffectPacket(int enumEffectType);
	void			SpecificEffectPacket(const char filename[128]);

	// ADD_MONSTER_REFINE
	bool			DoRefine(LPITEM item, bool bMoneyOnly = false);
	// END_OF_ADD_MONSTER_REFINE

	bool			DoRefineWithScroll(LPITEM item);
	bool			RefineInformation(UINT bCell, BYTE bType, int iAdditionalCell = -1);

	void			SetRefineMode(int iAdditionalCell = -1);
	void			ClearRefineMode();

	bool			GiveItem(LPCHARACTER victim, TItemPos Cell);
	bool			CanReceiveItem(LPCHARACTER from, LPITEM item) const;
	void			ReceiveItem(LPCHARACTER from, LPITEM item);
	bool			GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector <DWORD>& dwItemVnums, std::vector <DWORD>& dwItemCounts, std::vector <LPITEM>& item_gets, int& count);

	bool			MoveItem(TItemPos pos, TItemPos change_pos, BYTE num);
	bool			PickupItem(DWORD vid);
	bool			EquipItem(LPITEM item, int iCandidateCell = -1);
	bool			UnequipItem(LPITEM item);

	// 현재 item을 착용할 수 있는 지 확인하고, 불가능 하다면 캐릭터에게 이유를 알려주는 함수
	bool			CanEquipNow(const LPITEM item, const TItemPos& srcCell = NPOS, const TItemPos& destCell = NPOS);

	// 착용중인 item을 벗을 수 있는 지 확인하고, 불가능 하다면 캐릭터에게 이유를 알려주는 함수
	bool			CanUnequipNow(const LPITEM item, const TItemPos& srcCell = NPOS, const TItemPos& destCell = NPOS);

	bool			SwapItem(UINT bCell, UINT bDestCell);

	LPITEM			AutoGiveItem(DWORD dwItemVnum, BYTE bCount = 1, int iRarePct = -1, bool bMsg = true);
	void			AutoGiveItem(LPITEM item, bool longOwnerShip = false);

	int				GetEmptyInventory(BYTE size) const;
	int				GetEmptyDragonSoulInventory(LPITEM pItem) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	int				GetEmptySkillBookInventory(BYTE size) const;
	int				GetEmptyUpgradeItemsInventory(BYTE size) const;
	int				GetEmptyStoneInventory(BYTE size) const;
	int				GetEmptyChestInventory(BYTE size) const;
	int				GetEmptyAttrInventory(BYTE size) const;
	int				GetEmptyFlowerInventory(BYTE size) const;
#endif
	void			CopyDragonSoulItemGrid(std::vector<WORD>& vDragonSoulItemGrid) const;
	int				GetEmptyDragonSoulInventoryWithExceptions(LPITEM pItem, std::vector<WORD>& vec /*= -1*/) const;

	int				CountEmptyInventory() const;

	int				CountSpecifyItem(DWORD vnum) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	int				CountSpecifyItemBySkillBookInventory(DWORD vnum) const;
	int				CountSpecifyItemByUpgradeItemsInventory(DWORD vnum) const;
	int				CountSpecifyItemByStoneItemsInventory(DWORD vnum) const;
	int				CountSpecifyItemByChestItemsInventory(DWORD vnum) const;
	int				CountSpecifyItemByAttrItemsInventory(DWORD vnum) const;
	int				CountSpecifyItemByFlowerItemsInventory(DWORD vnum) const;
#endif
	int				BKBul(long skillindex) const;
	void			RemoveSpecifyItem(DWORD vnum, DWORD count = 1);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	void			RemoveSpecifyItemBySkillBookInventory(DWORD vnum, DWORD count);
	void			RemoveSpecifyItemByUpgradeItemsInventory(DWORD vnum, DWORD count);
	void			RemoveSpecifyItemByStoneItemsInventory(DWORD vnum, DWORD count);
	void			RemoveSpecifyItemByChestItemsInventory(DWORD vnum, DWORD count);
	void			RemoveSpecifyItemByAttrItemsInventory(DWORD vnum, DWORD count);
	void			RemoveSpecifyItemByFlowerItemsInventory(DWORD vnum, DWORD count);
#endif
	LPITEM			FindSpecifyItem(DWORD vnum) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	LPITEM			FindSkillBookInventoryItem(DWORD vnum) const;
	LPITEM			FindUpgradeItemsInventoryItem(DWORD vnum) const;
	LPITEM			FindStoneItemsInventoryItem(DWORD vnum) const;
	LPITEM			FindChestItemsInventoryItem(DWORD vnum) const;
	LPITEM			FindAttrItemsInventoryItem(DWORD vnum) const;
	LPITEM			FindFlowerItemsInventoryItem(DWORD vnum) const;
#endif
	LPITEM			FindItemByID(DWORD id) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	LPITEM			FindSkillBookInventoryItemByID(DWORD vnum) const;
	LPITEM			FindUpgradeItemsInventoryItemByID(DWORD vnum) const;
	LPITEM			FindStoneItemsInventoryItemByID(DWORD vnum) const;
	LPITEM			FindChestItemsInventoryItemByID(DWORD vnum) const;
	LPITEM			FindAttrItemsInventoryItemByID(DWORD vnum) const;
	LPITEM			FindFlowerItemsInventoryItemByID(DWORD vnum) const;
#endif

	int				CountSpecifyTypeItem(BYTE type) const;
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	int				CountSpecifyTypeItemBySkillBookInventory(BYTE type) const;
	int				CountSpecifyTypeItemByUpgradeItemsInventory(BYTE type) const;
	int				CountSpecifyTypeItemByStoneItemsInventory(BYTE type) const;
	int				CountSpecifyTypeItemByChestItemsInventory(BYTE type) const;
	int				CountSpecifyTypeItemByAttrItemsInventory(BYTE type) const;
	int				CountSpecifyTypeItemByFlowerItemsInventory(BYTE type) const;
#endif
	void			RemoveSpecifyTypeItem(BYTE type, DWORD count = 1);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	void			RemoveSpecifyTypeItemBySkillBookInventory(BYTE type, DWORD count);
	void			RemoveSpecifyTypeItemByUpgradeItemsInventory(BYTE type, DWORD count);
	void			RemoveSpecifyTypeItemByStoneItemsInventory(BYTE type, DWORD count);
	void			RemoveSpecifyTypeItemByChestItemsInventory(BYTE type, DWORD count);
	void			RemoveSpecifyTypeItemByAttrItemsInventory(BYTE type, DWORD count);
	void			RemoveSpecifyTypeItemByFlowerItemsInventory(BYTE type, DWORD count);
#endif

	bool			IsEquipUniqueItem(DWORD dwItemVnum) const;

	// CHECK_UNIQUE_GROUP
	bool			IsEquipUniqueGroup(DWORD dwGroupVnum) const;
	// END_OF_CHECK_UNIQUE_GROUP
	// End of Item
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	void 			ExtendInventoryItemUse(CHARACTER* ch);
#endif

protected:

	/// 한 아이템에 대한 가격정보를 전송한다.
	/**
	 * @param [in]	dwItemVnum 아이템 vnum
	 * @param [in]	dwItemPrice 아이템 가격
	 */
	void			SendMyShopPriceListCmd(DWORD dwItemVnum, TItemPriceType ItemPrice);

	bool			m_bNoOpenedShop;	///< 이번 접속 후 개인상점을 연 적이 있는지의 여부(열었던 적이 없다면 true)

	bool			m_bItemLoaded;
	int				m_iRefineAdditionalCell;
	bool			m_bUnderRefine;
	DWORD			m_dwRefineNPCVID;
public:
	////////////////////////////////////////////////////////////////////////////////////////
	// Money related
	INT				GetGold() const { return m_points.gold; }
	void			SetGold(INT gold) { m_points.gold = gold; }
	bool			DropGold(INT gold);
	INT				GetAllowedGold() const;
	void			GiveGold(INT iAmount);	// 파티가 있으면 파티 분배, 로그 등의 처리
	// End of Money
#ifdef ENABLE_CHEQUE_SYSTEM
	int				GetCheque() const { return m_points.cheque; }
	void			SetCheque(int cheque) { m_points.cheque = cheque; }
#endif
#ifdef ENABLE_GEM_SYSTEM
	long			GetGem() const { return m_points.gem; }
	void			SetGem(long gem) { m_points.gem = gem; }
#endif
#ifdef ENABLE_EVENT_SYSTEM
	// Year Round Point
	int				GetYearRoundPoint() const { return m_points.year_round_point; }
	void			SetYearRoundPoint(int point) { m_points.year_round_point = point; }
	// End Of Year Round Point
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		/*** Zodiac ***/
public:
	WORD			GetZodiacPoint() const { return m_points.zodiac_point; }
	void			SetZodiacPoint(WORD zodiac_point) { m_points.zodiac_point = zodiac_point; }

	DWORD			GetLastZodiacPointReward() const { return m_dwLastZodiacPointReward; }
	void			SetLastZodiacPointReward(DWORD time) { m_dwLastZodiacPointReward = time; }
protected:
	DWORD			m_dwLastZodiacPointReward;
	/*** END ***/
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
public:
	WORD			GetWorshipPoint() const { return m_points.lWorshipPoint; }
	void			SetWorshipPoint(WORD worship_point) { m_points.lWorshipPoint = worship_point; }
#endif
	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	// Shop related
public:
	void			SetShop(LPSHOP pkShop);
	LPSHOP			GetShop() const { return m_pkShop; }
	void			ShopPacket(BYTE bSubHeader);

	void			SetShopOwner(LPCHARACTER ch) { m_pkChrShopOwner = ch; }
	LPCHARACTER		GetShopOwner() const { return m_pkChrShopOwner; }

	LPSHOP			GetMyShop() const { return m_pkMyShop; }
	void			CloseMyShop();

protected:

	LPSHOP			m_pkShop;
	LPSHOP			m_pkMyShop;
	std::string		m_stShopSign;
	LPCHARACTER		m_pkChrShopOwner;

	// End of shop
	////////////////////////////////////////////////////////////////////////////////////////
	// Exchange related
public:
	bool			ExchangeStart(LPCHARACTER victim);
	void			SetExchange(CExchange* pkExchange);
	CExchange* GetExchange() const { return m_pkExchange; }

#ifdef ENABLE_GEM_SYSTEM
	#ifdef ENABLE_BOSS_GEM_SYSTEM
	void			OpenGemShop(bool gemShopType);
	#else
	void			OpenGemShop();
	#endif
	void 			RefreshGemShop();
	void 			OpenGemShopFirstTime();
	void 			RefreshGemShopItems();
	void 			RefreshGemShopWithItem();
	void			GemShopBuy(BYTE bPos);
	void			GemShopAdd(BYTE bPos);
	bool			CreateGaya(int glimmerstone_count, LPITEM metinstone_item, int cost, int pct);
protected:
	bool			m_bGemShopType;
#endif

protected:
	CExchange* m_pkExchange;
	// End of Exchange

	////////////////////////////////////////////////////////////////////////////////////////
	// Battle
public:
	struct TBattleInfo
	{
		int iTotalDamage;
		int iAggro;

		TBattleInfo(int iTot, int iAggr)
			: iTotalDamage(iTot), iAggro(iAggr)
		{}
	};
	typedef std::map<VID, TBattleInfo>	TDamageMap;

	typedef struct SAttackLog
	{
		DWORD	dwVID;
		DWORD	dwTime;
	} AttackLog;

	bool				Damage(LPCHARACTER pAttacker, int dam, EDamageType type = DAMAGE_TYPE_NORMAL);
	bool				__Profile__Damage(LPCHARACTER pAttacker, int dam, EDamageType type = DAMAGE_TYPE_NORMAL);
	void				DeathPenalty(BYTE bExpLossPercent);
	void				ReviveInvisible(int iDur);

	bool				Attack(LPCHARACTER pkVictim, BYTE bType = 0);
	bool				IsAlive() const { return m_pointsInstant.position == POS_DEAD ? false : true; }
	bool				CanFight() const;

	bool				CanBeginFight() const;
	void				BeginFight(LPCHARACTER pkVictim); // pkVictimr과 싸우기 시작한다. (강제적임, 시작할 수 있나 체크하려면 CanBeginFight을 사용)

	bool				CounterAttack(LPCHARACTER pkChr); // 반격하기 (몬스터만 사용)

	bool				IsStun() const;
	void				Stun();
	bool				IsDead() const;
	void				Dead(LPCHARACTER pkKiller = nullptr, bool bImmediateDead = false);
#ifdef ENABLE_GROWTH_PET_SYSTEM
	void				SetImmortal(int st) { m_stImmortalSt = st; };
	bool				IsImmortal() { return 1 == m_stImmortalSt; };
	void				SetNewPetSkillCD(int s, DWORD time) { m_newpetskillcd[s] = time; };
	DWORD				GetNewPetSkillCD(int s) { return m_newpetskillcd[s]; };
#endif

	void				Reward(bool bItemDrop);
	void				RewardGold(LPCHARACTER pkAttacker);
#ifdef ENABLE_ATTENDANCE_EVENT
	void				RewardAttendance();
#endif

	bool				Shoot(BYTE bType);
	void				FlyTarget(DWORD dwTargetVID, long x, long y, BYTE bHeader);

	void				ForgetMyAttacker();
	void				AggregateMonster();
	void				AttractRanger();
	void				PullMonster();

	int					GetArrowAndBow(LPITEM* ppkBow, LPITEM* ppkArrow, int iArrowCount = 1);
	void				UseArrow(LPITEM pkArrow, DWORD dwArrowCount);

	void				AttackedByPoison(LPCHARACTER pkAttacker);
	void				RemovePoison();
#ifdef ENABLE_WOLFMAN_CHARACTER
	void				AttackedByBleeding(LPCHARACTER pkAttacker);
	void				RemoveBleeding();
#endif
	void				AttackedByFire(LPCHARACTER pkAttacker, int amount, int count);
	void				RemoveFire();

#ifdef ENABLE_ALIGNMENT_SYSTEM
	void				UpdateAlignment(long long iAmount, bool manuelUpdate);
	long long			GetAlignment() const;
	long long			GetRealAlignment() const;
	void				ShowAlignment(bool bShow);
#else
	void				UpdateAlignment(int iAmount);
	int					GetAlignment() const;
	int					GetRealAlignment() const;
	void				ShowAlignment(bool bShow);
#endif

	void				SetKillerMode(bool bOn);
	bool				IsKillerMode() const;
	void				UpdateKillerMode();

	BYTE				GetPKMode() const;
	void				SetPKMode(BYTE bPKMode);

	void				ItemDropPenalty(LPCHARACTER pkKiller);

	void				UpdateAggrPoint(LPCHARACTER ch, EDamageType type, int dam);
	//
	// HACK
	//
public:

	void SetLastComboTime(DWORD time);
	DWORD GetLastComboTime() const;

	int GetValidComboInterval() const;
	void SetValidComboInterval(int interval);

	BYTE GetComboIndex() const;

	void SkipComboAttackByTime(int interval);
	DWORD GetSkipComboAttackByTime() const;

protected:
	DWORD m_dwLastComboTime;
	int m_iValidComboInterval;
	BYTE m_bComboIndex;
	DWORD m_dwSkipComboAttackByTime;

protected:
	void				UpdateAggrPointEx(LPCHARACTER ch, EDamageType type, int dam, TBattleInfo& info);
	void				ChangeVictimByAggro(int iNewAggro, LPCHARACTER pNewVictim);

	DWORD				m_dwFlyTargetID;
	std::vector<DWORD>	m_vec_dwFlyTargets;
	TDamageMap			m_map_kDamage;	// 어떤 캐릭터가 나에게 얼마만큼의 데미지를 주었는가?
//		AttackLog			m_kAttackLog;
	DWORD				m_dwKillerPID;

	int					m_iAlignment;		// Lawful/Chaotic value -200000 ~ 200000
	int					m_iRealAlignment;
	int					m_iKillerModePulse;
	BYTE				m_bPKMode;

	// Aggro
	DWORD				m_dwLastVictimSetTime;
	int					m_iMaxAggro;
	// End of Battle

	// Stone
public:
	void				SetStone(LPCHARACTER pkChrStone);
	void				ClearStone();
	void				DetermineDropMetinStone();
	DWORD				GetDropMetinStoneVnum() const { return m_dwDropMetinStone; }
	BYTE				GetDropMetinStonePct() const { return m_bDropMetinStonePct; }

protected:
	LPCHARACTER			m_pkChrStone;		// 나를 스폰한 돌
	CHARACTER_SET		m_set_pkChrSpawnedBy;	// 내가 스폰한 놈들
	DWORD				m_dwDropMetinStone;
	BYTE				m_bDropMetinStonePct;
	// End of Stone

public:

	void				SkillLevelPacket();
#ifdef ENABLE_7AND8TH_SKILLS_SYSTEM
	bool				SkillCanUp(DWORD dwVnum);
#endif
	void				SkillLevelUp(DWORD dwVnum, BYTE bMethod = SKILL_UP_BY_POINT);
	bool				SkillLevelDown(DWORD dwVnum);
	// ADD_GRANDMASTER_SKILL
	bool				UseSkill(DWORD dwVnum, LPCHARACTER pkVictim, bool bUseGrandMaster = true);
	void				ResetSkill();
	void				SetSkillLevel(DWORD dwVnum, BYTE bLev);
	int					GetUsedSkillMasterType(DWORD dwVnum);

	bool				IsLearnableSkill(DWORD dwSkillVnum) const;
	// END_OF_ADD_GRANDMASTER_SKILL

	bool				CheckSkillHitCount(const BYTE SkillID, const VID dwTargetVID);
	const DWORD* GetUsableSkillList() const;
	bool				CanUseSkill(DWORD dwSkillVnum) const;
	bool				IsUsableSkillMotion(DWORD dwMotionIndex) const;
	int					GetSkillLevel(DWORD dwVnum) const;
	int					GetSkillMasterType(DWORD dwVnum) const;
	int					GetSkillPower(DWORD dwVnum, BYTE bLevel = 0) const;

	time_t				GetSkillNextReadTime(DWORD dwVnum) const;
	void				SetSkillNextReadTime(DWORD dwVnum, time_t time);
	void				SkillLearnWaitMoreTimeMessage(DWORD dwVnum);

	void				ComputePassiveSkill(DWORD dwVnum);
	int					ComputeSkill(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel = 0);
#ifdef ENABLE_GYEONGGONG_SKILL
	int					ComputeGyeongGongSkill(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel = 0);
#endif
	int					ComputeSkillAtPosition(DWORD dwVnum, const PIXEL_POSITION& posTarget, BYTE bSkillLevel = 0);
	void				ComputeSkillPoints();

	void				SetSkillGroup(BYTE bSkillGroup);
	BYTE				GetSkillGroup() const { return m_points.skill_group; }

	int					ComputeCooltime(int time);

	void				GiveRandomSkillBook();

	void				DisableCooltime();
	bool				LearnSkillByBook(DWORD dwSkillVnum, BYTE bProb = 0);
	bool				LearnGrandMasterSkill(DWORD dwSkillVnum);

private:
	bool				m_bDisableCooltime;
	DWORD				m_dwLastSkillTime;	///< 마지막으로 skill 을 쓴 시간(millisecond).
	// End of Skill

	// MOB_SKILL
public:
	bool				HasMobSkill() const;
	size_t				CountMobSkill() const;
	const TMobSkillInfo* GetMobSkill(unsigned int idx) const;
	bool				CanUseMobSkill(unsigned int idx) const;
	bool				UseMobSkill(unsigned int idx);
	void				ResetMobSkillCooltime();
protected:
	DWORD				m_adwMobSkillCooltime[MOB_SKILL_MAX_NUM];
	// END_OF_MOB_SKILL

	// for SKILL_MUYEONG
public:
	void				StartMuyeongEvent();
	void				StopMuyeongEvent();
#ifdef ENABLE_GYEONGGONG_SKILL
	void				StartGyeongGongEvent();
	void				StopGyeongGongEvent();
#endif

private:
	LPEVENT				m_pkMuyeongEvent;
#ifdef ENABLE_GYEONGGONG_SKILL
	LPEVENT				m_pkGyeongGongEvent;
#endif

	// for SKILL_CHAIN lighting
public:
	int					GetChainLightningIndex() const { return m_iChainLightingIndex; }
	void				IncChainLightningIndex() { ++m_iChainLightingIndex; }
	void				AddChainLightningExcept(LPCHARACTER ch) { m_setExceptChainLighting.insert(ch); }
	void				ResetChainLightningIndex() { m_iChainLightingIndex = 0; m_setExceptChainLighting.clear(); }
	int					GetChainLightningMaxCount() const;
	const CHARACTER_SET& GetChainLightingExcept() const { return m_setExceptChainLighting; }

private:
	int					m_iChainLightingIndex;
	CHARACTER_SET m_setExceptChainLighting;

	// for SKILL_EUNHYUNG
public:
	void				SetAffectedEunhyung();
	void				ClearAffectedEunhyung() { m_dwAffectedEunhyungLevel = 0; }
	bool				GetAffectedEunhyung() const { return m_dwAffectedEunhyungLevel; }

private:
	DWORD				m_dwAffectedEunhyungLevel;

	//
	// Skill levels
	//
protected:
	TPlayerSkill* m_pSkillLevels;
	std::unordered_map<BYTE, int>		m_SkillDamageBonus;
	std::map<int, TSkillUseInfo>	m_SkillUseInfo;

	///////////////////////// ///////////////////////////////////////////////////////////////
	// AI related
public:
	void			AssignTriggers(const TMobTable* table);
	LPCHARACTER		GetVictim() const;	// 공격할 대상 리턴
	void			SetVictim(LPCHARACTER pkVictim);
	LPCHARACTER		GetNearestVictim(LPCHARACTER pkChr);
	LPCHARACTER		GetProtege() const;	// 보호해야 할 대상 리턴

	bool			Follow(LPCHARACTER pkChr, float fMinimumDistance = 150.0f);
	bool			Return();
	bool			IsGuardNPC() const;
	bool			IsChangeAttackPosition(LPCHARACTER target) const;
	void			ResetChangeAttackPositionTime() { m_dwLastChangeAttackPositionTime = get_dword_time() - AI_CHANGE_ATTACK_POISITION_TIME_NEAR; }
	void			SetChangeAttackPositionTime() { m_dwLastChangeAttackPositionTime = get_dword_time(); }

	bool			OnIdle();

	void			OnAttack(LPCHARACTER pkChrAttacker);
	void			OnClick(LPCHARACTER pkChrCauser);

	VID				m_kVIDVictim;

protected:
	DWORD			m_dwLastChangeAttackPositionTime;
	CTrigger		m_triggerOnClick;
	// End of AI

	////////////////////////////////////////////////////////////////////////////////////////
	// Target
protected:
	LPCHARACTER				m_pkChrTarget;		// 내 타겟
	CHARACTER_SET	m_set_pkChrTargetedBy;	// 나를 타겟으로 가지고 있는 사람들

public:
	void				SetTarget(LPCHARACTER pkChrTarget);
	void				BroadcastTargetPacket();
	void				ClearTarget();
	void				CheckTarget();
	LPCHARACTER			GetTarget() const { return m_pkChrTarget; }

	////////////////////////////////////////////////////////////////////////////////////////
	// Safebox
public:
	int					GetSafeboxSize() const;
	void				QuerySafeboxSize();
	void				SetSafeboxSize(int size);

	CSafebox* GetSafebox() const;
	void 				LoadSafebox(int iSize, long long dwGold, WORD wCheque, int iItemCount, TPlayerItem * pItems);
	void				ChangeSafeboxSize(BYTE bSize);
	void				CloseSafebox();

	/// 창고 열기 요청
	/**
	 * @param [in]	pszPassword 1자 이상 6자 이하의 창고 비밀번호
	 *
	 * DB 에 창고열기를 요청한다.
	 * 창고는 중복으로 열지 못하며, 최근 창고를 닫은 시간으로 부터 10초 이내에는 열 지 못한다.
	 */
	void				ReqSafeboxLoad(const char* pszPassword);

	/// 창고 열기 요청의 취소
	/**
	 * ReqSafeboxLoad 를 호출하고 CloseSafebox 하지 않았을 때 이 함수를 호출하면 창고를 열 수 있다.
	 * 창고열기의 요청이 DB 서버에서 실패응답을 받았을 경우 이 함수를 사용해서 요청을 할 수 있게 해준다.
	 */
	void				CancelSafeboxLoad(void) { m_bOpeningSafebox = false; }

	void				SetMallLoadTime(int t) { m_iMallLoadTime = t; }
	int					GetMallLoadTime() const { return m_iMallLoadTime; }

	CSafebox* GetMall() const;
	void				LoadMall(int iItemCount, TPlayerItem* pItems);
	void				CloseMall();

	void				SetSafeboxOpenPosition();
	float				GetDistanceFromSafeboxOpen() const;

protected:
	CSafebox* m_pkSafebox;
	int					m_iSafeboxSize;
	int					m_iSafeboxLoadTime;
	bool				m_bOpeningSafebox;	///< 창고가 열기 요청 중이거나 열려있는가 여부, true 일 경우 열기요청이거나 열려있음.

	CSafebox* m_pkMall;
	int					m_iMallLoadTime;

	PIXEL_POSITION		m_posSafeboxOpen;

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	// Mounting
public:
	void				MountVnum(DWORD vnum);
	DWORD				GetMountVnum() const { return m_dwMountVnum; }
	DWORD				GetLastMountTime() const { return m_dwMountTime; }

	bool				CanUseHorseSkill();

	// Horse
	virtual	void		SetHorseLevel(int iLevel);
	virtual	bool		StartRiding();
	virtual	bool		StopRiding();

	virtual	DWORD		GetMyHorseVnum() const;

	virtual	void		HorseDie();
	virtual bool		ReviveHorse();

	virtual void		SendHorseInfo();
	virtual	void		ClearHorseInfo();

	void				HorseSummon(bool bSummon, bool bFromFar = false, DWORD dwVnum = 0, const char* name = 0);

	LPCHARACTER			GetHorse() const { return m_chHorse; }	 // 현재 소환중인 말
	LPCHARACTER			GetRider() const; // rider on horse
	void				SetRider(LPCHARACTER ch);

	bool				IsRiding() const;

#ifdef ENABLE_PET_SYSTEM
public:
	CPetSystem* GetPetSystem() { return m_petSystem; }

protected:
	CPetSystem* m_petSystem;

public:
#endif

#ifdef ENABLE_BUFFI_SYSTEM
public:
	CSupportShaman* GetSupportShaman() { return m_SupportShaman; }

protected:
	CSupportShaman* m_SupportShaman;

public:
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
public:
	CNewPetSystem* GetNewPetSystem() { return m_newpetSystem; }

protected:
	CNewPetSystem* m_newpetSystem;

public:
#endif

protected:
	LPCHARACTER			m_chHorse;
	LPCHARACTER			m_chRider;

	DWORD				m_dwMountVnum;
	DWORD				m_dwMountTime;

	BYTE				m_bSendHorseLevel;
	BYTE				m_bSendHorseHealthGrade;
	BYTE				m_bSendHorseStaminaGrade;

	////////////////////////////////////////////////////////////////////////////////////////
	// Detailed Log
public:
	void				DetailLog() { m_bDetailLog = !m_bDetailLog; }
	void				ToggleMonsterLog();
	void				MonsterLog(const char* format, ...);
private:
	bool				m_bDetailLog;
	bool				m_bMonsterLog;

	////////////////////////////////////////////////////////////////////////////////////////
	// Empire

public:
	void 				SetEmpire(BYTE bEmpire);
	BYTE				GetEmpire() const { return m_bEmpire; }

protected:
	BYTE				m_bEmpire;

	////////////////////////////////////////////////////////////////////////////////////////
	// Regen
public:
	void				SetRegen(LPREGEN pkRegen);
	LPREGEN				GetRegen();
protected:
	PIXEL_POSITION			m_posRegen;
	float				m_fRegenAngle;
	LPREGEN				m_pkRegen;
	size_t				regen_id_; // to help dungeon regen identification
	// End of Regen

	////////////////////////////////////////////////////////////////////////////////////////
	// Resists & Proofs
public:
	bool				CannotMoveByAffect() const;	// 특정 효과에 의해 움직일 수 없는 상태인가?
	bool				IsImmune(DWORD dwImmuneFlag);
	void				SetImmuneFlag(DWORD dw) { m_pointsInstant.dwImmuneFlag = dw; }

protected:
	void				ApplyMobAttribute(const TMobTable* table);
	// End of Resists & Proofs

	////////////////////////////////////////////////////////////////////////////////////////
	// QUEST
	//
public:
	void				SetQuestNPCID(DWORD vid);
	DWORD				GetQuestNPCID() const { return m_dwQuestNPCVID; }
	LPCHARACTER			GetQuestNPC() const;

	void				SetQuestItemPtr(LPITEM item);
	void				ClearQuestItemPtr();
	LPITEM				GetQuestItemPtr() const;

	void				SetQuestBy(DWORD dwQuestVnum) { m_dwQuestByVnum = dwQuestVnum; }
	DWORD				GetQuestBy() const { return m_dwQuestByVnum; }

	int					GetQuestFlag(const std::string& flag) const;
	void				SetQuestFlag(const std::string& flag, int value);

	void				ConfirmWithMsg(const char* szMsg, int iTimeout, DWORD dwRequestPID);

private:
	DWORD				m_dwQuestNPCVID;
	DWORD				m_dwQuestByVnum;
	LPITEM				m_pQuestItem;

	// Events
public:
	bool				StartStateMachine(int iPulse = 1);
	void				StopStateMachine();
	void				UpdateStateMachine(DWORD dwPulse);
	void				SetNextStatePulse(int iPulseNext);

	// 캐릭터 인스턴스 업데이트 함수. 기존엔 이상한 상속구조로 CFSM::Update 함수를 호출하거나 UpdateStateMachine 함수를 사용했는데, 별개의 업데이트 함수 추가함.
	void				UpdateCharacter(DWORD dwPulse);

protected:
	DWORD				m_dwNextStatePulse;

	// Marriage
public:
	LPCHARACTER			GetMarryPartner() const;
	void				SetMarryPartner(LPCHARACTER ch);
	int					GetMarriageBonus(DWORD dwItemVnum, bool bSum = true);

	void				SetWeddingMap(marriage::WeddingMap* pMap);
	marriage::WeddingMap* GetWeddingMap() const { return m_pWeddingMap; }

private:
	marriage::WeddingMap* m_pWeddingMap;
	LPCHARACTER			m_pkChrMarried;

	// Warp Character
public:
	void				StartWarpNPCEvent();

public:
	void				StartSaveEvent();
	void				StartRecoveryEvent();
	void				StartDestroyWhenIdleEvent();
#ifdef ENABLE_AFK_MODE_SYSTEM
	void 				StartUpdateCharacterEvent();
	LPEVENT 			m_pkUpdateCharacter;
#endif
	LPEVENT				m_pkDeadEvent;
	LPEVENT				m_pkStunEvent;
	LPEVENT				m_pkSaveEvent;
	LPEVENT				m_pkRecoveryEvent;
	LPEVENT				m_pkTimedEvent;
	LPEVENT				m_pkFishingEvent;
	LPEVENT				m_pkAffectEvent;
	LPEVENT				m_pkPoisonEvent;
#ifdef ENABLE_WOLFMAN_CHARACTER
	LPEVENT				m_pkBleedEvent;
#endif
	LPEVENT				m_pkFireEvent;
	LPEVENT				m_pkWarpNPCEvent;
	//DELAYED_WARP
	//END_DELAYED_WARP

	// MINING
	LPEVENT				m_pkMiningEvent;
	// END_OF_MINING
	LPEVENT				m_pkWarpEvent;
	LPEVENT				m_pkDestroyWhenIdleEvent;
	LPEVENT				m_pkPetSystemUpdateEvent;
#ifdef ENABLE_GROWTH_PET_SYSTEM
	LPEVENT				m_pkNewPetSystemUpdateEvent;
	LPEVENT				m_pkNewPetSystemExpireEvent;
#endif

	bool IsWarping() const { return m_pkWarpEvent ? true : false; }

	bool				m_bHasPoisoned;
#ifdef ENABLE_WOLFMAN_CHARACTER
	bool				m_bHasBleeded;
#endif

	const CMob* m_pkMobData;
	CMobInstance* m_pkMobInst;

	std::map<int, LPEVENT> m_mapMobSkillEvent;

	friend struct FuncSplashDamage;
	friend struct FuncSplashAffect;
	friend class CFuncShoot;

public:
	int				GetPremiumRemainSeconds(BYTE bType) const;

private:
	int				m_aiPremiumTimes[PREMIUM_MAX_NUM];

	// NEW_HAIR_STYLE_ADD
public:
	bool ItemProcess_Hair(LPITEM item, int iDestCell);
	// END_NEW_HAIR_STYLE_ADD
	bool ItemProccess_DS(CItem * item, const TItemPos& DestCell);

public:
	void ClearSkill();
	void ClearSubSkill();

	// RESET_ONE_SKILL
	bool ResetOneSkill(DWORD dwVnum);
	// END_RESET_ONE_SKILL

private:
	void SendDamagePacket(LPCHARACTER pAttacker, int Damage, BYTE DamageFlag);
	void Damage_PirateGoldDrop();

	// ARENA
private:
	CArena* m_pArena;
	bool m_ArenaObserver;
	int m_nPotionLimit;

public:
	void 	SetArena(CArena* pArena) { m_pArena = pArena; }
	void	SetArenaObserverMode(bool flag) { m_ArenaObserver = flag; }

	CArena* GetArena() const { return m_pArena; }
	bool	GetArenaObserverMode() const { return m_ArenaObserver; }

	void	SetPotionLimit(int count) { m_nPotionLimit = count; }
	int		GetPotionLimit() const { return m_nPotionLimit; }
	// END_ARENA

	void	LoginSpecialObserver();
	void	LogoutSpecialObserver();

	//PREVENT_TRADE_WINDOW
public:
	bool	IsOpenSafebox() { return m_isOpenSafebox ? true : false; }
	void 	SetOpenSafebox(bool b) { m_isOpenSafebox = b; }

	int		GetSafeboxLoadTime() const { return m_iSafeboxLoadTime; }
	void	SetSafeboxLoadTime() { m_iSafeboxLoadTime = thecore_pulse(); }
	//END_PREVENT_TRADE_WINDOW
private:
	bool	m_isOpenSafebox;

public:
	int		GetSkillPowerByLevel(int level, bool bMob = false) const;

	//PREVENT_REFINE_HACK
	int		GetRefineTime() const { return m_iRefineTime; }
	void	SetRefineTime() { m_iRefineTime = thecore_pulse(); }
	int		m_iRefineTime;
	//END_PREVENT_REFINE_HACK

	//RESTRICT_USE_SEED_OR_MOONBOTTLE
	int 	GetUseSeedOrMoonBottleTime() const { return m_iSeedTime; }
	void  	SetUseSeedOrMoonBottleTime() { m_iSeedTime = thecore_pulse(); }
	int 	m_iSeedTime;
	//END_RESTRICT_USE_SEED_OR_MOONBOTTLE

	//PREVENT_PORTAL_AFTER_EXCHANGE
	int		GetExchangeTime() const { return m_iExchangeTime; }
	void	SetExchangeTime() { m_iExchangeTime = thecore_pulse(); }
	int		m_iExchangeTime;
	//END_PREVENT_PORTAL_AFTER_EXCHANGE

	int 	m_iMyShopTime;
	int		GetMyShopTime() const { return m_iMyShopTime; }
	void	SetMyShopTime() { m_iMyShopTime = thecore_pulse(); }

	// Hack 방지를 위한 체크.
	bool	IsHack(bool bSendMsg = true, bool bCheckShopOwner = true, int limittime = g_nPortalLimitTime);

	void Say(const std::string& s);
public:
	bool ItemProcess_Polymorph(LPITEM item);

	// by mhh
	LPITEM* GetCubeItem() { return m_pointsInstant.pCubeItems; }
	bool IsCubeOpen() const { return (m_pointsInstant.pCubeNpc ? true : false); }
	void SetCubeNpc(LPCHARACTER npc) { m_pointsInstant.pCubeNpc = npc; }
	bool CanDoCube() const;
public:
	bool IsSiegeNPC() const;

private:
	int		m_deposit_pulse;

public:
	void	UpdateDepositPulse();
	bool	CanDeposit() const;

private:
	void	__OpenPrivateShop();

public:
	struct AttackedLog
	{
		DWORD 	dwPID;
		DWORD	dwAttackedTime;

		AttackedLog() : dwPID(0), dwAttackedTime(0)
		{
		}
	};

	AttackLog	m_kAttackLog;
	AttackedLog m_AttackedLog;
	int			m_speed_hack_count;

private:
	std::string m_strNewName;

public:
	const std::string GetNewName() const { return this->m_strNewName; }
	void SetNewName(const std::string name) { this->m_strNewName = name; }

public:
	void GoHome();

	void GotoWarpNPC();
private:
	std::set<DWORD>	m_known_guild;

public:
	void SendGuildName(CGuild * pGuild);
	void SendGuildName(DWORD dwGuildID);

private:
	DWORD m_dwLogOffInterval;

public:
	DWORD GetLogOffInterval() const { return m_dwLogOffInterval; }

public:
	bool UnEquipSpecialRideUniqueItem();

	bool CanWarp();

private:
	DWORD m_dwLastGoldDropTime;

public:
	void AutoRecoveryItemProcess(const EAffectTypes);

public:
	void BuffOnAttr_AddBuffsFromItem(LPITEM pItem);
	void BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem);

private:
	void BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue);
	void BuffOnAttr_ClearAll();

	typedef std::map <BYTE, CBuffOnAttributes*> TMapBuffOnAttrs;
	TMapBuffOnAttrs m_map_buff_on_attrs;
	// 무적 : 원활한 테스트를 위하여.
public:
	void SetArmada() { cannot_dead = true; }
	void ResetArmada() { cannot_dead = false; }
private:
	bool cannot_dead;

#ifdef ENABLE_PET_SYSTEM
private:
	bool m_bIsPet;
public:
	void SetPet() { m_bIsPet = true; }
	bool IsPet() { return m_bIsPet; }
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
private:
	bool m_bIsNewPet;
	int m_eggvid;
public:
	void SetNewPet() { m_bIsNewPet = true; }
	bool IsNewPet() const { return m_bIsNewPet ? true : false; }
	void SetEggVid(int vid) { m_eggvid = vid; }
	int GetEggVid() { return m_eggvid; }

#endif

	//최종 데미지 보정.
private:
	float m_fAttMul;
	float m_fDamMul;
public:
	float GetAttMul() { return this->m_fAttMul; }
	void SetAttMul(float newAttMul) { this->m_fAttMul = newAttMul; }
	float GetDamMul() { return this->m_fDamMul; }
	void SetDamMul(float newDamMul) { this->m_fDamMul = newDamMul; }

private:
	bool IsValidItemPosition(TItemPos Pos) const;

	//독일 선물 기능 패킷 임시 저장
private:
	unsigned int itemAward_vnum;
	char		 itemAward_cmd[20];
	//bool		 itemAward_flag;
public:
	unsigned int GetItemAward_vnum() { return itemAward_vnum; }
	char* GetItemAward_cmd() { return itemAward_cmd; }
	//bool		 GetItemAward_flag() { return itemAward_flag; }
	void		 SetItemAward_vnum(unsigned int vnum) { itemAward_vnum = vnum; }
	void		 SetItemAward_cmd(char* cmd) { strcpy(itemAward_cmd, cmd); }
	//void		 SetItemAward_flag(bool flag) { itemAward_flag = flag; }

public:
	//용혼석

	// 캐릭터의 affect, quest가 load 되기 전에 DragonSoul_Initialize를 호출하면 안된다.
	// affect가 가장 마지막에 로드되어 LoadAffect에서 호출함.
	void	DragonSoul_Initialize();

	bool	DragonSoul_IsQualified() const;
	void	DragonSoul_GiveQualification();

	int		DragonSoul_GetActiveDeck() const;
	bool	DragonSoul_IsDeckActivated() const;
	bool	DragonSoul_ActivateDeck(int deck_idx);

	void	DragonSoul_DeactivateAll();
	// 반드시 ClearItem 전에 불러야 한다.
	// 왜냐하면....
	// 용혼석 하나 하나를 deactivate할 때마다 덱에 active인 용혼석이 있는지 확인하고,
	// active인 용혼석이 하나도 없다면, 캐릭터의 용혼석 affect와, 활성 상태를 제거한다.
	//
	// 하지만 ClearItem 시, 캐릭터가 착용하고 있는 모든 아이템을 unequip하는 바람에,
	// 용혼석 Affect가 제거되고, 결국 로그인 시, 용혼석이 활성화되지 않는다.
	// (Unequip할 때에는 로그아웃 상태인지, 아닌지 알 수 없다.)
	// 용혼석만 deactivate시키고 캐릭터의 용혼석 덱 활성 상태는 건드리지 않는다.
	void	DragonSoul_CleanUp();
	// 용혼석 강화창
public:
	bool		DragonSoul_RefineWindow_Open(LPENTITY pEntity);
	bool		DragonSoul_RefineWindow_Close();
	LPENTITY	DragonSoul_RefineWindow_GetOpener() { return  m_pointsInstant.m_pDragonSoulRefineWindowOpener; }
	bool		DragonSoul_RefineWindow_CanRefine();

private:
	// SyncPosition을 악용하여 타유저를 이상한 곳으로 보내는 핵 방어하기 위하여,
	// SyncPosition이 일어날 때를 기록.
	timeval		m_tvLastSyncTime;
public:
	void			SetLastSyncTime(const timeval & tv) { memcpy(&m_tvLastSyncTime, &tv, sizeof(timeval)); }
protected:
#ifdef ENABLE_DROP_HACK_FIX
	int					LastDropTime;
	int					CountDrops;
#endif

#ifdef ENABLE_MINIGAME_RUMI_EVENT
public:
	struct S_CARD
	{
		DWORD	type;
		DWORD	value;
	};

	struct CARDS_INFO
	{
		S_CARD cards_in_hand[MAX_CARDS_IN_HAND];
		S_CARD cards_in_field[MAX_CARDS_IN_FIELD];
		DWORD	cards_left;
		DWORD	field_points;
		DWORD	points;
	};

	void			Cards_open(DWORD safemode);
	void			Cards_clean_list();
	DWORD			GetEmptySpaceInHand();
	void			Cards_pullout();
	void			RandomizeCards();
	bool			CardWasRandomized(DWORD type, DWORD value);
	void			SendUpdatedInformations();
	void			SendReward();
	void			CardsDestroy(DWORD reject_index);
	void			CardsAccept(DWORD accept_index);
	void			CardsRestore(DWORD restore_index);
	DWORD			GetEmptySpaceInField();
	DWORD			GetAllCardsCount();
	bool			TypesAreSame();
	bool			ValuesAreSame();
	bool			CardsMatch();
	DWORD			GetLowestCard();
	bool			CheckReward();
	void			CheckCards();
	void			RestoreField();
	void			ResetField();
	void			CardsEnd();
	void			GetGlobalRank(char* buffer, size_t buflen);
	void			GetRundRank(char* buffer, size_t buflen);
protected:
	CARDS_INFO	character_cards;
	S_CARD	randomized_cards[24];
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
public:
	void			OpenMyShop(const char* c_pszSign, TShopItemTable * pTable, BYTE bItemCount, DWORD days, DWORD model);
	void			SendShops(bool isGm = false);
	void			OpenShop(DWORD id, const char* name, DWORD npc, DWORD level, DWORD align, bool onboot = false);
	void			SetPrivShop(DWORD shop_id) { bprivShop = shop_id; }
	BOOL			IsPrivShop(void)  const { return bprivShop > 0; }
	DWORD			GetPrivShop()  const { return bprivShop; }
	void			SetPrivShopOwner(DWORD id) { bprivShopOwner = id; }
	DWORD			GetPrivShopOwner()  const { return bprivShopOwner; }
	void			DeleteMyShop();
	DWORD			GetShopTime()  const { return dw_ShopTime; }
	void			SetShopTime(DWORD time) { dw_ShopTime = time; }
	void			SetShopSign(const char* name);
	void			LoadPrivShops();
	TPrivShop		GetPrivShopTable(DWORD id);
	void			RemovePrivShopTable(DWORD id);
	void			UpdatePrivShopTable(DWORD id, TPrivShop shop);
	void			UpdateShopItems();
	void			SendShopCost();
private:
	PSHOP_MAP		m_mapshops;
	DWORD			bprivShop;
	DWORD			bprivShopOwner;
	DWORD			dw_ShopTime;
public:
	void			StartRefreshShopEvent();
protected:
	LPEVENT			m_pkRefreshShopEvent;
public:
	void			SetShopValidPos(bool value) { m_bShopValidPos = value; }
	bool			GetShopValidPos() { return m_bShopValidPos; }
protected:
	bool			m_bShopValidPos;

#endif

#ifdef ENABLE_GIFTBOX_SYSTEM
protected:
	void			AddGiftGrid(int page);
	int				AddGiftGridItem(int page, int size);
	GIFT_MAP		m_mapGiftGrid;
	LPEVENT			m_pkGiftRefresh;
	DWORD			m_dwLastGiftPage;
public:
	void			StartRefreshGift();
	void			LoadGiftPage(int page);
	void			RefreshGift();
	int				GetGiftPages() { return m_mapGiftGrid.size(); }
	int				GetLastGiftPage() { return m_dwLastGiftPage; }
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
protected:
	bool	m_bChangeLook;
	#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	bool	m_bMountChangeLook;
	#endif
public:
	bool	isChangeLookOpened() { return m_bChangeLook; }
	#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	void	ChangeLookWindow(bool bOpen = false, bool bRequest = false, bool bMount = false);
	#else
	void	ChangeLookWindow(bool bOpen = false, bool bRequest = false);
	#endif
	void	ClearClWindowMaterials();
	LPITEM * GetClWindowMaterials() { return m_pointsInstant.pClMaterials; }
	BYTE	CheckClEmptyMaterialSlot();
	void	AddClMaterial(TItemPos tPos, BYTE bPos);
	void	RemoveClMaterial(BYTE bPos);
	void	RefineClMaterials();
	bool	CleanTransmutation(LPITEM pkItem, LPITEM pkTarget);
#endif
	CEntity::ENTITY_MAP GetViewMap() const { return m_map_view; }
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
private:
	DWORD m_iCombatZonePoints;
	DWORD m_iCombatZoneDeaths;

public:
	LPEVENT m_pkCombatZoneLeaveEvent;
	LPEVENT m_pkCombatZoneWarpEvent;

	BYTE	GetCombatZoneRank();

	DWORD	GetRealCombatZonePoints();
	void	SetRealCombatZonePoints(DWORD dwValue);

	void	UpdateCombatZoneRankings(const char* memberName, DWORD memberEmpire, DWORD memberPoints);

	DWORD	GetCombatZonePoints() { return m_iCombatZonePoints; }
	void	SetCombatZonePoints(DWORD dwValue) { m_iCombatZonePoints = dwValue; }
	DWORD	GetCombatZoneDeaths() { return m_iCombatZoneDeaths; }
	void	SetCombatZoneDeaths(DWORD dwValue) { m_iCombatZoneDeaths = dwValue; }
#endif
#ifdef ENABLE_BUFFI_SYSTEM
private:
	bool m_bIsSupportShaman;
	LPCHARACTER m_Owner;
	DWORD m_dwSupportShamanCostume;
	DWORD m_dwSupportShamanWeapon;
#ifdef ENABLE_ACCE_SYSTEM
	DWORD m_dwSupportShamanSash;
#endif
	DWORD m_dwSupportShamanHair;
	DWORD m_dwSupportShamanInt;
public:
	void SetSupportShaman() { m_bIsSupportShaman = true; }
	bool IsSupportShaman() { return m_bIsSupportShaman; }
	void SetSupportShamanArmor(int vnum) { m_dwSupportShamanCostume = vnum; }
	void SetSupportShamanWeapon(int vnum) { m_dwSupportShamanWeapon = vnum; }
#ifdef ENABLE_ACCE_SYSTEM
	void SetSupportShamanSash(int vnum) { m_dwSupportShamanSash = vnum; }
#endif
	void SetSupportShamanHair(int vnum) { m_dwSupportShamanHair = vnum; }
	DWORD GetSupportShamanArmor() { return m_dwSupportShamanCostume; }
	DWORD GetSupportShamanWeapon() { return m_dwSupportShamanWeapon; }
#ifdef ENABLE_ACCE_SYSTEM
	DWORD GetSupportShamanSash() { return m_dwSupportShamanSash; }
#endif
	DWORD GetSupportShamanHair() { return m_dwSupportShamanHair; }
	void SetOwner(LPCHARACTER owner) { m_Owner = owner; }
	LPCHARACTER GetOwner() { return m_Owner; }
	void SetSupportShamanInt(int count) { m_dwSupportShamanInt = count; }
	DWORD GetSupportShamanInt() { return m_dwSupportShamanInt; }
#endif
#ifdef ENABLE_ACCE_SYSTEM
protected:
	bool	m_bSashCombination, m_bSashAbsorption;

public:
	bool	isSashOpened(bool bCombination) { return bCombination ? m_bSashCombination : m_bSashAbsorption; }
	void	OpenSash(bool bCombination);
	void	CloseSash();
	void	ClearSashMaterials();
	bool	CleanSashAttr(LPITEM pkItem, LPITEM pkTarget);
	LPITEM* GetSashMaterials() { return m_pointsInstant.pSashMaterials; }
	bool	SashIsSameGrade(long lGrade);
	DWORD	GetSashCombinePrice(long lGrade);
	void	GetSashCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);
	BYTE	CheckEmptyMaterialSlot();
	void	AddSashMaterial(TItemPos tPos, BYTE bPos);
	void	RemoveSashMaterial(BYTE bPos);
	BYTE	CanRefineSashMaterials();
	void	RefineSashMaterials();
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
public:
	bool	RequestToViewEquip(LPCHARACTER pTarget);
	void	DenyToViewEquip(LPCHARACTER guest);
	void	AcceptToViewEquip(LPCHARACTER guest);
	void	SetViewEquipRequestEvent(LPEVENT pkEvent) { m_pkViewEquipRequestEvent = pkEvent; }
	void	SendEquipment(LPCHARACTER ch);

protected:
	LPEVENT	m_pkViewEquipRequestEvent;
#endif
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
public:
	LPITEM* GetCombItem() { return m_pointsInstant.pCombItems; }
	bool			IsCombOpen() const { return (m_pointsInstant.pCombNpc ? true : false); }
	void			SetCombNpc(LPCHARACTER npc) { m_pointsInstant.pCombNpc = npc; }
	bool			CanDoComb() const;
#endif
#ifdef ENABLE_HEALTH_BOARD_SYSTEM
public:
	void	HealthBoardPacket();
#endif
public:
	bool IsInSafezone() const;
public:
#ifdef ENABLE_CAMP_FIRE_FIX
protected:
	int				LastCampFireUse;
#endif
#ifdef ENABLE_ANTI_CMD_FLOOD
private:
	int m_dwCmdAntiFloodPulse;
	DWORD m_dwCmdAntiFloodCount;
public:
	int GetCmdAntiFloodPulse() { return m_dwCmdAntiFloodPulse; }
	DWORD GetCmdAntiFloodCount() { return m_dwCmdAntiFloodCount; }
	DWORD IncreaseCmdAntiFloodCount() { return ++m_dwCmdAntiFloodCount; }
	void SetCmdAntiFloodPulse(int dwPulse) { m_dwCmdAntiFloodPulse = dwPulse; }
	void SetCmdAntiFloodCount(DWORD dwCount) { m_dwCmdAntiFloodCount = dwCount; }
#endif

#ifdef __ENABLE_ACTIVITY__
public:
	spActivityHandler GetActivityHandler() { return m_activityHandler; }
	int		GetActivity() const;
	void	LoadActivity(TActivityTable * data);

private:
	spActivityHandler m_activityHandler;
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
public:
	int		GetExtendInvenMax() const { return EXTEND_INVENTORY_SLOT_START + m_points.extend_inventory; }
	int		GetExtendInvenStage() const { return m_points.extend_inventory / 5; }
	void	SetExtendedInventory(short extend_inventory) { m_points.extend_inventory = extend_inventory; };
	void	SendExtendInventoryPacket() const;
	void	UpgradeExInventory(CHARACTER * ch);
	int CheckExtendAvailableSlots3();
	int CheckExtendAvailableSlots4();
	int CheckExtendAvailableSlots5();
	bool IsAvailableCell(int cell, short ExtendSlots);
	int GetExtendItemNeedCount();
#endif
public:
	int				waitHackCounter;
#ifdef ENABLE_YOUTUBER_SYSTEM
public:
	void YoutubeYonetimPaneli(const char* arg1, const char* arg2, const char* arg3);
	void YoutubeVideoListesi(const char* arg1, const char* arg2);
	void Yayinci_Liste();

private:
	std::vector<std::string> Youtuberr;
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
public:
	bool			DoRefine_Rarity(LPITEM item, LPITEM second_item, LPITEM third_item);
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
public:
	void 			FishEventGeneralInfo();
	void			FishEventUseBox(TItemPos itemPos);
	bool 			FishEventIsValidPosition(BYTE shapePos, BYTE shapeType);
	void 			FishEventPlaceShape(BYTE shapePos, BYTE shapeType);
	void 			FishEventAddShape(BYTE shapePos);
	void 			FishEventCheckEnd();
private:
	DWORD m_dwFishUseCount;
	BYTE m_bFishAttachedShape;
public:
	DWORD GetFishEventUseCount() const { return m_dwFishUseCount; }
	void FishEventIncreaseUseCount() { m_dwFishUseCount++; }

	BYTE GetFishAttachedShape() const { return m_bFishAttachedShape; }
	void SetFishAttachedShape(BYTE bShape) { m_bFishAttachedShape = bShape; }
#endif
#ifdef ENABLE_IMPROVE_DRAGON_COIN
public:
	DWORD			GetDragonCoin();
	DWORD			GetDragonMark();
	void			SetDragonCoin(DWORD amount);
	void			SetDragonMark(DWORD amount);
	void			RefreshDragonCoin();
#endif
#ifdef ENABLE_CAPE_EFFECT_FIX
protected:
	int				m_iCapeEffectPulse;
public:
	void			SetCapeEffectPulse(int i) { m_iCapeEffectPulse = i; }
	int				GetCapeEffectPulse() { return m_iCapeEffectPulse; }
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
public:
	void				CheckPet();
#endif
#ifdef ENABLE_GEM_SYSTEM
private:
	DWORD m_dwGemNextRefresh;
public:
	DWORD GetGemNextRefresh() const { return m_dwGemNextRefresh; }
#ifdef ENABLE_BOSS_GEM_SYSTEM
private:
	DWORD m_dwGemBossNextRefresh;
public:
	DWORD GetGemBossNextRefresh() const { return m_dwGemBossNextRefresh; }
#endif
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
public:
	std::vector<THitCountInfo> m_hitCount;
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
public:
	void			MiniGameCatchKingSetFieldCards(std::vector<TCatchKingCard> vec) { m_vecCatchKingFieldCards = vec; }

	DWORD			MiniGameCatchKingGetScore() const { return dwCatchKingTotalScore; }
	void			MiniGameCatchKingSetScore(DWORD dwScore) { dwCatchKingTotalScore = dwScore; }

	DWORD			MiniGameCatchKingGetBetNumber() const { return bCatchKingBetSetNumber; }
	void			MiniGameCatchKingSetBetNumber(BYTE bSetNr) { bCatchKingBetSetNumber = bSetNr; }

	BYTE			MiniGameCatchKingGetHandCard() const { return bCatchKingHandCard; }
	void			MiniGameCatchKingSetHandCard(BYTE bKingCard) { bCatchKingHandCard = bKingCard; }

	BYTE			MiniGameCatchKingGetHandCardLeft() const { return bCatchKingHandCardLeft; }
	void			MiniGameCatchKingSetHandCardLeft(BYTE bHandCard) { bCatchKingHandCardLeft = bHandCard; }

	bool			MiniGameCatchKingGetGameStatus() const { return dwCatchKingGameStatus; }
	void			MiniGameCatchKingSetGameStatus(bool bStatus) { dwCatchKingGameStatus = bStatus; }

	std::vector<TCatchKingCard> m_vecCatchKingFieldCards;
protected:
	BYTE bCatchKingHandCard;
	BYTE bCatchKingHandCardLeft;
	bool dwCatchKingGameStatus;

	BYTE bCatchKingBetSetNumber;
	DWORD dwCatchKingTotalScore;
#endif
#ifdef ENABLE_HALLOWEEN_EVENT
public:
	void GetHalloweenRank(char* buffer, size_t buflen);
#endif
#ifdef ENABLE_AFK_MODE_SYSTEM
	void SetAway(bool f) { m_isAway = f; }
	bool IsAway() const { return m_isAway; }
protected:
	bool m_isAway;
#endif
#ifdef ENABLE_TOGGLE_BUFF_SYSTEM
public:
	void			CheckPotions();
	void			CheckTeleportItems();
	bool			CheckPotionItem(int affectID);
#endif
#ifdef ENABLE_ITEM_SOUL_SYSTEM
	void			CheckSoul();
	float			GetSoulDamVal(bool bMelee);
	bool			CheckSoulItem(bool bBlue);
#endif
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
public:
	bool IsDefanceWaweMastAttackMob(DWORD dwMobVnum) const { return (dwMobVnum >= 3950 && dwMobVnum <= 3964); }
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
	//MailBox Functions

	struct MailboxValues
	{
		int id_mail;
		int id_receive_mail;
		char nombre_pj[12];
		char asunto[25];
		char descrip[100];
		int item_vnum;
		int item_count;
#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
		int transmutation;
#endif
		int gold;
#ifdef USE_WON_SYSTEM_MAILBOX
		int won;
#endif
		int check_acept;
		int id_item;
		DWORD time;
		int check;
	}save_mail, copy_mail;

public:
	void 	OpenMailBox();
	void 	LOAD_CONFIG_MAILS();

	bool	IsOpenMailBox() const { return m_isOpenMailBox ? true : false; }
	void 	SetOpenMailBox(bool b) { m_isOpenMailBox = b; }
	void 	LoadingMails();
	void 	LoadMails();
	void 	SendMInfoMap();
#ifdef USE_WON_SYSTEM_MAILBOX
	void 	SendMailBox(const char* name, const char* titulo, const char* descrip, int item_vnum, int gold, int won);
	void 	SendMailBoxPackets(BYTE subheader, int id_mail = 0, char nombre_pj[12] = nullptr, char asunto[25] = nullptr, char descrip[100] = nullptr, int item_vnum = 0, int item_count = 0, int gold = 0, int won = 0, int check_acept = 0, int id_item = 0, DWORD time = 0, int check_view = 0);
#else
	void 	SendMailBox(const char* name, const char* titulo, const char* descrip, int item_vnum, int gold);
	void 	SendMailBoxPackets(BYTE subheader, int id_mail = 0, char nombre_pj[12] = "", char asunto[25] = "", char descrip[100] = "", int item_vnum = 0, int item_count = 0, int gold = 0, int check_acept = 0, int id_item = 0, DWORD time = 0, int check_view = 0);
#endif
	void 	CheckNameMailBox(const char* name);
	void 	AcceptMail(int id_mail);
	void 	DeleteMail(int id_mail);
	void 	DeleteAllMails();
	void 	AcceptAllMails();
	void 	CheckViewMail(int id_mail);

	void 	InsertItemMailBox(LPITEM item);
	bool 	FindPlayerName(const char* name);
	int 	GetIdPlayerReceive(const char* name);
	int 	GetCountMails(int id_player_receive);
	bool 	FindIDMail(int id_mail);
	bool 	FindIDReceiveMail();
	void 	ReceiveItem(int id_item);
	void 	ClearLIST(int func);

	int 	get_check_acept_mailbox(int id_mail);
	int		get_item_vnum_mailbox(int id_mail);
	int 	get_id_item_mailbox(int id_mail);
	int 	get_gold_mailbox(int id_mail);
#ifdef USE_WON_SYSTEM_MAILBOX
	int 	get_won_mailbox(int id_mail);
#endif

	//Function Extras
	void 	StartCheckTimeMailBox();
	int 	get_mailbox_size() { return info_mailbox_values.size(); }
	int 	get_mailbox_time(int size) { return info_mailbox_values[size].time; }
	int 	get_mailbox_id_mail(int size) { return info_mailbox_values[size].id_mail; }
	bool 	get_mailbox_time_extra();

	LPEVENT  MailBoxUpdateTime;

private:
	bool	m_isOpenMailBox;

	std::vector<MailboxValues> info_mailbox_values;
	std::vector<MailboxValues> info_mailbox_values_extra;

	int 	tasa_porciento;  //Porciendo de envio de yang
	int 	gold_envio;		 //Yang costo envio de mensaje
	int 	limite_mensajes; //Limite de mensajes en buzon.
	int 	send_mail_to_me; //Enviar mensajes a usted mismo.
#endif
#ifdef ENABLE_COSTUME_SET_SYSTEM
public:
	int			IsHalloweenSetCount();
	void		UpdateHalloweenSetStatus(int iAdd);
	int			IsHalloweenSetStatus() { return m_bHalloweenSetStatus; }
protected:
	int			m_bHalloweenSetStatus;
#endif
#ifdef ENABLE_FALL_FIX
public:
	bool			CanFall();
#endif
	// Inventory
private:
	std::shared_ptr<CInventory> m_inventory;

public:
	std::shared_ptr<CInventory> GetInventory() { return m_inventory; };
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
public:
	void SetLastZodiacCzLastTime(int time) { m_dwZodiacCzLastTime = time; }
	int GetLastZodiacCzLastTime() const	{ return m_dwZodiacCzLastTime; }
protected:
	int m_dwZodiacCzLastTime;
public:
	void SetZodiac(LPZODIAC pkZodiac);
	LPZODIAC GetZodiac() const	{ return m_pkZodiac; }
	LPZODIAC GetZodiacForce() const;
protected:
	LPZODIAC m_pkZodiac;
public:
	void CzCheckBox();
	void SetYellowmark(int amount);
	void ClearYellowMark();
	int GetYellowmark();
	void SetGreenmark(int amount);
	void ClearGreenMark();
	int GetGreenmark();
	void SetYellowreward(int amount);
	int GetYellowreward();
	void SetGreenreward(int amount);
	int GetGreenreward();
	void SetGoldreward(int amount);
	int GetGoldreward();
	void IsZodiacEffectMob();
	void IsZodiacEffectPC(DWORD Monster);
	void ZodiacFloorMessage(BYTE Floor);
	void SetLastZodiacAttackTime(DWORD time) { m_dwLastZodiacAttackTime = time; }
	DWORD GetLastZodiacAttackTime() const	{ return m_dwLastZodiacAttackTime; }
	void IncDeadCount() { m_dwDeadCount++; }
	void SetDeadCount(DWORD dead) { m_dwDeadCount = dead; }
	DWORD GetDeadCount() const	{ return m_dwDeadCount; }
	void EffectZodiacPacket(long X, long Y, int enumEffectType, int enumEffectType2 = 0);
	bool IsZodiacBoss();
	bool IsZodiacSubays();
	bool IsZodiacStone();
	WORD GetStoneNum() const;
	LPEVENT m_pkZodiacSkill1;
	LPEVENT m_pkZodiacSkill2;
	LPEVENT m_pkZodiacSkill3;
	LPEVENT m_pkZodiacSkill4;
	LPEVENT m_pkZodiacSkill5;
	LPEVENT m_pkZodiacSkill6;
	LPEVENT m_pkZodiacSkill7;
	LPEVENT m_pkZodiacSkill8;
	LPEVENT m_pkZodiacSkill9;
	LPEVENT m_pkZodiacSkill10;
	LPEVENT m_pkZodiacSkill11;
	void ZodiacDamage(BYTE Type, LPCHARACTER Victim = nullptr);
	int ZodiacSkillAttack(LPCHARACTER Victim, DWORD dwVnum, BYTE bSkillLevel);
	DWORD CountZodiacItems(DWORD Vnum);
	void SetZodiacItems(DWORD Vnum, int Count);
	DWORD PurchaseCountZodiacItems(DWORD Vnum);
	void SetPurchaseZodiacItems(DWORD Vnum, int Count);
	void KanoneDamage();
	void SpawnZodiacGroup(LPZODIAC pZodiac);
	void SpawnZodiacStone(LPZODIAC pZodiac);
private:
	DWORD m_dwLastZodiacAttackTime;
	DWORD m_dwDeadCount;
#endif
#ifdef ENABLE_CAPTCHA_SYSTEM
public:
	long	m_captcha;
	void	SetCaptcha(long captcha)	{ m_captcha = captcha; }
	long	GetCaptcha() const	{ return m_captcha; }
#endif
#ifdef ENABLE_ABUSE_SYSTEM
public:
	bool IsSuspectWaitDmg() const { return m_bIsSuspectWaitDmg; }
	void SetSuspectWaitDmg() { m_bIsSuspectWaitDmg = true; }
	spAbuseController GetAbuseController() const { return m_abuse; }
protected:
	bool m_bIsSuspectWaitDmg;
	spAbuseController m_abuse;
#endif
#ifdef ENABLE_WALLHACK_FIX
public:
	void StartCheckWallhack();
	const PIXEL_POSITION& GetLastMoveAblePosition(int32_t index = 0) { return m_lastMoveAblePos; }
	void SetLastMoveAblePosition(const PIXEL_POSITION& lastPos) { m_lastMoveAblePos = lastPos; }
	void SetLastMoveableMapIndex() { m_lastMoveableMapIndex = GetMapIndex(); }
	int32_t GetLastMoveableMapIndex() { return m_lastMoveableMapIndex; }
protected:
	LPEVENT m_pkCheckWallHackEvent;
	PIXEL_POSITION m_lastMoveAblePos;
	int32_t m_lastMoveableMapIndex;
#endif
#ifdef ENABLE_ANTINEARWATER_FIX
public:
	bool IS_VALID_FISHING_POSITION(int32_t *returnPosx, int32_t *returnPosy) const;
#endif
#ifdef ENABLE_FISHBOT_FIX
public:
	int32_t GetFishHackCount();
	void UpdateFishHackCount();
	void ResetFishHackCount();
	void SetLastFishTime(int32_t time);
	int32_t GetLastFishTime();
protected:
	int32_t m_iFishHackCount;
	int32_t m_iLastFishTime;
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
public:
	CMountSystem* GetMountSystem() { return m_mountSystem; }
	void MountSummon(LPITEM mountItem);
	void MountUnsummon(LPITEM mountItem);
	void CheckMount();
	bool IsRidingMount();
protected:
	CMountSystem* m_mountSystem;
private:
	bool m_bIsMount;
public:
	void SetMount() { m_bIsMount = true; }
	bool IsMount() { return m_bIsMount; }
#endif
#ifdef ENABLE_DEBUG_MODE_SYSTEM
public:
	BOOL IsDebug() const{return (IsPC() ? m_bDebug : false);}
	void SetDebug(bool d_gDebug){m_bDebug=d_gDebug;}
private:
	bool m_bDebug;
#endif
#ifdef ENABLE_AURA_SYSTEM
protected:
	bool	m_bAuraRefine, m_bAuraAbsorption;

public:
	bool	isAuraOpened(bool bCombination) { return bCombination ? m_bAuraRefine : m_bAuraAbsorption; }
	void	OpenAura(bool bCombination);
	void	CloseAura();
	void	ClearAuraMaterials();
	bool	CleanAuraAttr(LPITEM pkItem, LPITEM pkTarget);
	LPITEM* GetAuraMaterials() { return m_pointsInstant.pAuraMaterials; }
	bool	AuraIsSameGrade(long lGrade);
	DWORD	GetAuraCombinePrice(long lGrade);
	void	GetAuraCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);
	BYTE	CheckAuraEmptyMaterialSlot();
	void	AddAuraMaterial(TItemPos tPos, BYTE bPos);
	void	RemoveAuraMaterial(BYTE bPos);
	BYTE	CanRefineAuraMaterials();
	void	RefineAuraMaterials();
#endif
#ifdef ENABLE_DUNGEON_BACK_SYSTEM
public:
	long GetPlayerDungeonIndex()	{ return m_points.dungeonindex; }
	void SetPlayerDungeonIndex(long vnum)	{ m_points.dungeonindex = vnum; }
#endif
#ifdef ENABLE_DUNGEON_INFO_SYSTEM
	public:
		void			SetQuestDamage(int iDmg) { iQuestDamage = iDmg; }
		int				GetQuestDamage() { return iQuestDamage; }

	private:
		int				iQuestDamage;
#endif
#ifdef ENABLE_SKILL_RENEWAL
public:
	int GetSkillBookReadCount(DWORD skillVnum);
	int GetSkillRemainCount(DWORD skillVnum);
	void SendSkillBookCount();
#endif
#ifdef ENABLE_GUILD_RANKING_SYSTEM
public:
	void SendGuildRankList();
	bool IsGuildRankedSystem();
#endif
#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
public:
	void SetSpecialGachaItemVnum(DWORD vnum)  { m_dwSpecialGachaVnum = vnum; }
	DWORD GetSpecialGachaItemVnum() { return m_dwSpecialGachaVnum; }
	void SetSpecialGachaItemCount(DWORD count)  { m_dwSpecialGachaCount = count; }
	DWORD GetSpecialGachaItemCount() { return m_dwSpecialGachaCount; }
	void SetSpecialGachaItemVnumGive(DWORD vnum) { m_dwSpecialGachaGiveVnum = vnum; }
	DWORD GetSpecialGachaItemVnumGive() { return m_dwSpecialGachaGiveVnum; }
	void SetSpecialGachaItemCountGive(DWORD count) { m_dwSpecialGachaGiveCount = count; }
	DWORD GetSpecialGachaItemCountGive() { return m_dwSpecialGachaGiveCount; }
protected:
	DWORD	m_dwSpecialGachaVnum;
	DWORD	m_dwSpecialGachaCount;
	DWORD	m_dwSpecialGachaGiveVnum;
	DWORD	m_dwSpecialGachaGiveCount;
#endif
#ifdef ENABLE_BLEND_ICON_SYSTEM
public:
	bool SetAffectPotion(LPITEM item);
#endif
#ifdef ENABLE_ALIGNMENT_SYSTEM
public:
	void OnAlignUpdate(long lOldAlignment, bool manuelUpdate);
#endif
};

ESex GET_SEX(LPCHARACTER ch);

#endif
