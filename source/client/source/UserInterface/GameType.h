#pragma once
#include "../GameLib/ItemData.h"
#include "Locale_inc.h"
struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD & c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;
};

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Size = 5*9; // x*y
const DWORD c_Inventory_Page_Count = 4;
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;

const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body	= c_Equipment_Start + 0;
const DWORD c_Equipment_Head	= c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes	= c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist	= c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon	= c_Equipment_Start + 4;
const DWORD c_Equipment_Neck	= c_Equipment_Start + 5;
const DWORD c_Equipment_Ear		= c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1	= c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2	= c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow	= c_Equipment_Start + 9;
const DWORD c_Equipment_Shield	= c_Equipment_Start + 10;

#ifdef WJ_TRADABLE_ICON
enum ETopWindowTypes
{
	ON_TOP_WND_NONE,
	ON_TOP_WND_SHOP,
	ON_TOP_WND_EXCHANGE,
	ON_TOP_WND_SAFEBOX,
	ON_TOP_WND_PRIVATE_SHOP,
	ON_TOP_WND_BOOK_COMBINATION,
	ON_TOP_WND_COSTUME_COMBINATION,
	ON_TOP_WND_PET_FEED,
	ON_TOP_WND_ACCE_COMBINE,
	ON_TOP_WND_ACCE_ABSORB,
	ON_TOP_WND_CHANGE_LOOK,
	ON_TOP_WND_AURA,
	ON_TOP_WND_AURA_REFINE,

	ON_TOP_WND_MAX,
};
#endif


// 새로 추가된 신규 반지 & 벨트
// 장착형 아이템에 할당할 수 있는 위치가 기존 장비, 채기랍 퀘스트 보상, 코스튬 시스템 등으로 인해서 공간이 잘려있다.
// 이게 다 채기랍 보상 버프를 장착아이템처럼 구현한 ㅅㄲ 때문에 난리났따... ㅆㅂ
// 
// 정리하면, 기존 장비창들은 서버DB상 아이템 포지션이 90 ~ 102 이고,
// 2013년 초에 새로 추가되는 슬롯들은 111 ~ 부터 시작한다. 착용 장비에서 최대로 사용할 수 있는 값은 121 까지이고, 122부터는 용혼석에서 사용한다.
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	const DWORD c_New_Equipment_Start = c_Equipment_Start + 25;
	const DWORD c_New_Equipment_Count = 5;
	const DWORD c_Equipment_Ring1 	= c_New_Equipment_Start + 0;
	const DWORD c_Equipment_Ring2 	= c_New_Equipment_Start + 1;
	const DWORD c_Equipment_Belt  	= c_New_Equipment_Start + 2;
#ifdef ENABLE_TALISMAN_SYSTEM
	const DWORD c_Equipment_Charm	= c_New_Equipment_Start + 3;
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
	const DWORD c_Equipment_Pet		= c_New_Equipment_Start + 4;
#endif
#endif

enum EDragonSoulDeckType
{
	DS_DECK_1,
	DS_DECK_2,
	DS_DECK_MAX_NUM = 2,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
#ifdef ENABLE_DS_GRADE_MYTH
    DRAGON_SOUL_GRADE_MYTH,
#endif
	DRAGON_SOUL_GRADE_MAX,

};

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

#ifdef ENABLE_COSTUME_SYSTEM
	const DWORD c_Costume_Slot_Start = c_Equipment_Start + 19;
	const DWORD	c_Costume_Slot_Body = c_Costume_Slot_Start + 0;
	const DWORD	c_Costume_Slot_Hair = c_Costume_Slot_Start + 1;
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	const DWORD c_Costume_Slot_Weapon = c_Costume_Slot_Start + 2;
#endif
#ifdef ENABLE_ACCE_SYSTEM
	const DWORD	c_Costume_Slot_Sash = c_Costume_Slot_Start + 3;
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Mount = c_Costume_Slot_Start + 4;
#endif
#ifdef ENABLE_AURA_SYSTEM
	const DWORD c_Costume_Slot_Aura	= c_Costume_Slot_Start + 5;
#endif
	const DWORD c_Costume_Slot_Count = 6;
	const DWORD c_Costume_Slot_End = c_Costume_Slot_Start + c_Costume_Slot_Count;
#endif


// [주의] 숫자(32) 하드코딩 주의. 현재 서버에서 용혼석 슬롯은 32부터임. 
// 서버 common/length.h 파일의 EWearPositions 열거형이 32까지 확장될 것을 염두하고(32 이상은 확장 하기 힘들게 되어있음.), 
// 그 이후부터를 용혼석 장착 슬롯으로 사용.
const DWORD c_Wear_Max = 32;
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

// NOTE: 2013년 2월 5일 현재... 용혼석 데크는 2개가 존재하는데, 향후 확장 가능성이 있어서 3개 데크 여유분을 할당 해 둠. 그 뒤 공간은 벨트 인벤토리로 사용
const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;		

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	// 벨트 아이템이 제공하는 인벤토리
	const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
	const DWORD c_Belt_Inventory_Width = 4;
	const DWORD c_Belt_Inventory_Height= 4;
	const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
	const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;
#endif

const DWORD c_Inventory_Count	= c_Belt_Inventory_Slot_End;

// 용혼석 전용 인벤토리
const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
const DWORD c_Skill_Book_Inventory_Slot_Start = 0;
const DWORD c_Skill_Book_Inventory_Slot_Count = 225;
const DWORD c_Skill_Book_Inventory_Slot_End = c_Skill_Book_Inventory_Slot_Start + c_Skill_Book_Inventory_Slot_Count;

const DWORD c_Upgrade_Items_Inventory_Slot_Start = 0;
const DWORD c_Upgrade_Items_Inventory_Slot_Count = 225;
const DWORD c_Upgrade_Items_Inventory_Slot_End = c_Upgrade_Items_Inventory_Slot_Start + c_Upgrade_Items_Inventory_Slot_Count;
	
const DWORD c_Stone_Items_Inventory_Slot_Start = 0;
const DWORD c_Stone_Items_Inventory_Slot_Count = 225;
const DWORD c_Stone_Items_Inventory_Slot_End = c_Stone_Items_Inventory_Slot_Start + c_Stone_Items_Inventory_Slot_Count;

const DWORD c_Chest_Items_Inventory_Slot_Start = 0;
const DWORD c_Chest_Items_Inventory_Slot_Count = 225;
const DWORD c_Chest_Items_Inventory_Slot_End = c_Chest_Items_Inventory_Slot_Start + c_Chest_Items_Inventory_Slot_Count;

const DWORD c_Attr_Items_Inventory_Slot_Start = 0;
const DWORD c_Attr_Items_Inventory_Slot_Count = 225;
const DWORD c_Attr_Items_Inventory_Slot_End = c_Attr_Items_Inventory_Slot_Start + c_Attr_Items_Inventory_Slot_Count;

const DWORD c_Flower_Items_Inventory_Slot_Start = 0;
const DWORD c_Flower_Items_Inventory_Slot_Count = 225;
const DWORD c_Flower_Items_Inventory_Slot_End = c_Flower_Items_Inventory_Slot_Start + c_Flower_Items_Inventory_Slot_Count;
#endif

enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	SLOT_TYPE_SKILL_BOOK_INVENTORY,
	SLOT_TYPE_UPGRADE_ITEMS_INVENTORY,
	SLOT_TYPE_STONE_INVENTORY,
	SLOT_TYPE_CHEST_INVENTORY,
	SLOT_TYPE_ATTR_INVENTORY,
	SLOT_TYPE_FLOWER_INVENTORY,
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
	SLOT_TYPE_FISH_EVENT,
#endif
#ifdef ENABLE_AUTO_HUNTING_SYSTEM
	SLOT_TYPE_AUTOHUNT_ITEM,
	SLOT_TYPE_AUTOHUNT_SKILL,
#endif
	SLOT_TYPE_MAX,
};

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,				// 기본 인벤토리. (45칸 짜리가 2페이지 존재 = 90칸)
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
	BELT_INVENTORY,			// NOTE: W2.1 버전에 새로 추가되는 벨트 슬롯 아이템이 제공하는 벨트 인벤토리
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	SKILL_BOOK_INVENTORY,
	UPGRADE_ITEMS_INVENTORY,
	STONE_ITEMS_INVENTORY,
	CHEST_ITEMS_INVENTORY,
	ATTR_ITEMS_INVENTORY,
	FLOWER_ITEMS_INVENTORY,
#endif

	GROUND,					// NOTE: 2013년 2월5일 현재까지 unused.. 왜 있는거지???
	
	WINDOW_TYPE_MAX,
};


#ifdef ENABLE_FISH_JIGSAW_EVENT
enum EFishEventInfo
{
	FISH_EVENT_SHAPE_NONE,
	FISH_EVENT_SHAPE_1,
	FISH_EVENT_SHAPE_2,			
	FISH_EVENT_SHAPE_3,			
	FISH_EVENT_SHAPE_4,			
	FISH_EVENT_SHAPE_5,			
	FISH_EVENT_SHAPE_6,			
	FISH_EVENT_SHAPE_7,
	FISH_EVENT_SHAPE_MAX_NUM,
};
#endif

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#pragma pack (push, 1)
#define WORD_MAX 0xffff

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
    SItemPos ()
    {
		window_type =     INVENTORY;
		cell = WORD_MAX;
    }
	SItemPos (BYTE _window_type, WORD _cell)
    {
        window_type = _window_type;
        cell = _cell;
    }

	// 기존에 cell의 형을 보면 BYTE가 대부분이지만, oi
	// 어떤 부분은 int, 어떤 부분은 WORD로 되어있어,
	// 가장 큰 자료형인 int로 받는다.
  //  int operator=(const int _cell)
  //  {
		//window_type = INVENTORY;
  //      cell = _cell;
  //      return cell;
  //  }
	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;
			break;
		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;
			break;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);
			break;
		case SKILL_BOOK_INVENTORY:
			return cell < c_Skill_Book_Inventory_Slot_End;
			break;
		case UPGRADE_ITEMS_INVENTORY:
			return cell < c_Upgrade_Items_Inventory_Slot_End;
			break;
		case STONE_ITEMS_INVENTORY:
			return cell < c_Stone_Items_Inventory_Slot_End;
			break;
		case CHEST_ITEMS_INVENTORY:
			return cell < c_Chest_Items_Inventory_Slot_End;
			break;
		case ATTR_ITEMS_INVENTORY:
			return cell < c_Attr_Items_Inventory_Slot_End;
			break;
		case FLOWER_ITEMS_INVENTORY:
			return cell < c_Flower_Items_Inventory_Slot_End;
			break;
		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);
			break;

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
		case SKILL_BOOK_INVENTORY:
		case UPGRADE_ITEMS_INVENTORY:
		case STONE_ITEMS_INVENTORY:
		case CHEST_ITEMS_INVENTORY:
		case ATTR_ITEMS_INVENTORY:
		case FLOWER_ITEMS_INVENTORY:
			return false;
			break;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

// Used by PushMove
const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
	ITEM_SOCKET_SLOT_MAX_NUM = 4,
	ITEM_ATTRIBUTE_SLOT_MAX_NUM = 15,
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	UINT Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
    BYTE        bType;
    short       sValue;
} TPlayerItemAttribute;

typedef struct packet_item
{
    DWORD       vnum;
    BYTE        count;
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	long		sealbind;
#endif
	bool		is_basic;
} TItemData;

typedef struct packet_shop_item
{
	DWORD		vnum;
	DWORD		price;
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		price_cheque;
#endif
	BYTE		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	BYTE		display_pos;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_SOLD_ITEM_SYSTEM
	int			isSold;
	char		newOwner[64+1];
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	DWORD		getLimitedCount;
	DWORD		getLimitedPurchaseCount;
#endif
#ifdef ENABLE_BUY_WITH_ITEM
	DWORD witemVnum;
#endif
} TShopItemData;

enum
{
	PET_HATCHING_MONEY 	=	100000,
	PET_NAME_MIN_SIZE	=	4,
	PET_NAME_MAX_SIZE	=	12,
};

#ifdef ENABLE_GEM_SYSTEM
typedef struct SGemShopItem
{
	BYTE	slotIndex;
	BYTE	status;
	
	DWORD	dwVnum;
	BYTE	bCount;
	DWORD	dwPrice;
} TGemShopItem;
#endif

#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2) // By sqrt
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx*dx + dy*dy);
}

// DEFAULT_FONT
void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char * c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();
// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char * c_szPathName);
const char * GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);
BYTE ApplyTypeToPointType(BYTE bApplyType);
