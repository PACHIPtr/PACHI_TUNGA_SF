#pragma once

#include "../gamelib/RaceData.h"
#include "Locale_inc.h"
typedef BYTE TPacketHeader;

enum HeaderClientGame
{
	HEADER_CG_LOGIN							= 1,
	HEADER_CG_ATTACK						= 2,
	HEADER_CG_CHAT							= 3,
	HEADER_CG_PLAYER_CREATE					= 4,
	HEADER_CG_PLAYER_DESTROY				= 5,
	HEADER_CG_PLAYER_SELECT					= 6,
	HEADER_CG_CHARACTER_MOVE				= 7,
	HEADER_CG_SYNC_POSITION					= 8,
	HEADER_CG_DIRECT_ENTER					= 9,	/*** Only Client ***/
	HEADER_CG_ENTERGAME						= 10,
	HEADER_CG_ITEM_USE						= 11,
#ifdef  ENABLE_DROP_DIALOG_EX_SYSTEM
	HEADER_CG_ITEM_DELETE					= 12,
#else
	HEADER_CG_ITEM_DROP						= 12,
#endif
	HEADER_CG_ITEM_MOVE						= 13,
#ifdef ENABLE_DROP_DIALOG_EX_SYSTEM
	HEADER_CG_ITEM_SELL						= 14,
#endif
	HEADER_CG_ITEM_PICKUP					= 15,
	HEADER_CG_QUICKSLOT_ADD					= 16,
	HEADER_CG_QUICKSLOT_DEL					= 17,
	HEADER_CG_QUICKSLOT_SWAP				= 18,
	HEADER_CG_WHISPER						= 19,
#ifndef ENABLE_DROP_DIALOG_EX_SYSTEM
	HEADER_CG_ITEM_DROP2					= 20,
#endif
	HEADER_CG_ON_CLICK						= 21,
	HEADER_CG_EXCHANGE						= 22,
	HEADER_CG_CHARACTER_POSITION			= 23,
	HEADER_CG_SCRIPT_ANSWER					= 24,
	HEADER_CG_QUEST_INPUT_STRING			= 25,
	HEADER_CG_QUEST_CONFIRM					= 26,
	HEADER_CG_SHOP							= 28,
	HEADER_CG_FLY_TARGETING					= 29,
	HEADER_CG_USE_SKILL						= 30,
	HEADER_CG_ADD_FLY_TARGETING				= 31,
	HEADER_CG_SHOOT							= 32,
	HEADER_CG_MYSHOP						= 33,
	HEADER_CG_ITEM_USE_TO_ITEM				= 34,
	HEADER_CG_TARGET			 			= 35,
	HEADER_CG_TEXT							= 36,	/*** Only Server ***/
	HEADER_CG_WARP							= 38,
	HEADER_CG_SCRIPT_BUTTON					= 39,
	HEADER_CG_MESSENGER						= 40,
	HEADER_CG_MALL_CHECKOUT					= 41,
	HEADER_CG_SAFEBOX_CHECKIN				= 42,
	HEADER_CG_SAFEBOX_CHECKOUT				= 43,
	HEADER_CG_PARTY_INVITE					= 44,
	HEADER_CG_PARTY_INVITE_ANSWER			= 45,
	HEADER_CG_PARTY_REMOVE					= 46,
	HEADER_CG_PARTY_SET_STATE				= 47,
	HEADER_CG_PARTY_USE_SKILL				= 48,
	HEADER_CG_SAFEBOX_ITEM_MOVE				= 49,
	HEADER_CG_PARTY_PARAMETER				= 50,
	HEADER_CG_GUILD							= 51,
	HEADER_CG_ANSWER_MAKE_GUILD				= 52,
	HEADER_CG_FISHING						= 53,
	HEADER_CG_GIVE_ITEM						= 54,
	HEADER_CG_EMPIRE						= 55,
	HEADER_CG_REFINE						= 56,
	HEADER_CG_MARK_LOGIN					= 57,
	HEADER_CG_MARK_CRCLIST					= 58,
	HEADER_CG_MARK_UPLOAD					= 59,
	HEADER_CG_MARK_IDXLIST					= 60,
	HEADER_CG_HACK							= 62,
	HEADER_CG_CHANGE_NAME					= 63,
	HEADER_CG_LOGIN2						= 64,
	HEADER_CG_DUNGEON						= 65,
	HEADER_CG_LOGIN3						= 233,	/*** Not Sorted ***/
	HEADER_CG_GUILD_SYMBOL_UPLOAD			= 66,
	HEADER_CG_GUILD_SYMBOL_CRC				= 67,
	HEADER_CG_SCRIPT_SELECT_ITEM			= 68,
	HEADER_CG_DRAGON_SOUL_REFINE			= 70,
	HEADER_CG_STATE_CHECKER					= 71,
#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	HEADER_CG_SHOP_SEARCH					= 72,
	HEADER_CG_SHOP_SEARCH_SUB				= 73,
	HEADER_CG_SHOP_SEARCH_BUY				= 74,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	HEADER_CG_COMBAT_ZONE_REQUEST_ACTION	= 75,
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	HEADER_CG_EXTEND_INVENTORY 				= 76,
#endif
#ifdef ENABLE_YOUTUBER_SYSTEM
	HEADER_CG_YOUTUBE_PANEL					= 77,
	HEADER_CG_YOUTUBE_VIDEO					= 78,
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
	HEADER_CG_PetSetName 					= 88,
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_CG_TARGET_INFO_LOAD				= 89,
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	HEADER_CG_CL 							= 90,
#endif
#ifdef ENABLE_ACCE_SYSTEM
	HEADER_CG_SASH 							= 91,
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
	HEADER_CG_FISH_EVENT_SEND 				= 92,
#endif
#ifdef ENABLE_GEM_SYSTEM
	HEADER_CG_GEM_SHOP = 110,
#endif
#ifdef ENABLE_OX_RENEWAL
	HEADER_CG_QUEST_INPUT_LONG_STRING		= 111,
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
    HEADER_CG_MAILBOX_SEND						= 112,
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	HEADER_CG_SAFEBOX_MONEY 				= 113,
	HEADER_CG_SAFEBOX_CHEQUE	 			= 114,
#endif
#ifdef ENABLE_AURA_SYSTEM
	HEADER_CG_AURA							= 115,
#endif
#ifdef ENABLE_AVERAGE_ITEM_PRICE
	HEADER_CG_AVERAGE_ITEM_PRICE			= 116,
#endif
	HEADER_CG_TIME_SYNC							= 252,	/* 0xfc */
	HEADER_CG_CLIENT_VERSION					= 253,	/* 0xfd */
	HEADER_CG_PONG								= 254,	/* 0xfe */
	HEADER_CG_HANDSHAKE							= 255,	/* 0xff */
	HEADER_CG_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
};
enum EPacketHeaderGameClient
{

	HEADER_GC_HANDSHAKE_OK						= 252,	//252TIME_SYNC
	HEADER_GC_PHASE								= 253,	//253
    HEADER_GC_BINDUDP							= 254,	//254
    HEADER_GC_HANDSHAKE							= 255,	//255

	HEADER_GC_CHARACTER_ADD								= 1,
	HEADER_GC_CHARACTER_DEL								= 2,
	HEADER_GC_CHARACTER_MOVE							= 3,
	HEADER_GC_CHAT										= 4,
	HEADER_GC_SYNC_POSITION								= 5,
	HEADER_GC_LOGIN_SUCCESS								= 6,
	HEADER_GC_LOGIN_FAILURE								= 7,
	HEADER_GC_PLAYER_CREATE_SUCCESS						= 8,
	HEADER_GC_PLAYER_CREATE_FAILURE						= 9,
	HEADER_GC_PLAYER_DELETE_SUCCESS						= 10,
	HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID				= 11,
	HEADER_GC_STUN										= 13,
	HEADER_GC_DEAD										= 14,
	HEADER_GC_PLAYER_POINTS								= 16,
	HEADER_GC_PLAYER_POINT_CHANGE						= 17,
	HEADER_GC_CHANGE_SPEED								= 18,
	HEADER_GC_CHARACTER_UPDATE							= 19,
	HEADER_GC_ITEM_SET									= 20,
	HEADER_GC_ITEM_SET2									= 21,
	HEADER_GC_ITEM_USE									= 23,
	HEADER_GC_ITEM_UPDATE								= 25,
	HEADER_GC_ITEM_GROUND_ADD							= 26,
	HEADER_GC_ITEM_GROUND_DEL							= 27,
	HEADER_GC_QUICKSLOT_ADD								= 28,
	HEADER_GC_QUICKSLOT_DEL								= 29,
	HEADER_GC_QUICKSLOT_SWAP							= 30,
	HEADER_GC_ITEM_OWNERSHIP							= 31,
	HEADER_GC_WHISPER									= 33,
	HEADER_GC_MOTION									= 34,
	HEADER_GC_SHOP										= 36,
	HEADER_GC_SHOP_SIGN									= 37,
	HEADER_GC_DUEL_START								= 38,
	HEADER_GC_PVP										= 39,
	HEADER_GC_EXCHANGE									= 40,
	HEADER_GC_CHARACTER_POSITION						= 41,
	HEADER_GC_PING										= 42,
	HEADER_GC_SCRIPT									= 43,
	HEADER_GC_QUEST_CONFIRM								= 44,
	HEADER_GC_MOUNT										= 46,
	HEADER_GC_OWNERSHIP									= 47,
	HEADER_GC_TARGET			 						= 48,
	HEADER_GC_WARP										= 49,
	HEADER_GC_ADD_FLY_TARGETING							= 50,
	HEADER_GC_CREATE_FLY								= 51,
	HEADER_GC_FLY_TARGETING								= 52,
	HEADER_GC_SKILL_LEVEL								= 54,
	HEADER_GC_MESSENGER									= 55,
	HEADER_GC_GUILD										= 56,
	HEADER_GC_PARTY_INVITE								= 57,
	HEADER_GC_PARTY_ADD									= 58,
	HEADER_GC_PARTY_UPDATE								= 59,
	HEADER_GC_PARTY_REMOVE								= 60,
	HEADER_GC_QUEST_INFO								= 61,
	HEADER_GC_REQUEST_MAKE_GUILD						= 62,
	HEADER_GC_PARTY_PARAMETER							= 63,
	HEADER_GC_SAFEBOX_SET								= 64,
	HEADER_GC_SAFEBOX_DEL								= 65,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD					= 66,
	HEADER_GC_SAFEBOX_SIZE								= 67,
	HEADER_GC_FISHING									= 68,
	HEADER_GC_EMPIRE									= 69,
	HEADER_GC_PARTY_LINK								= 70,
	HEADER_GC_PARTY_UNLINK								= 71,
	HEADER_GC_OBSERVER_ADD								= 73,	/*** Only Client ***/
	HEADER_GC_OBSERVER_REMOVE							= 74,	/*** Only Client ***/
	HEADER_GC_OBSERVER_MOVE								= 75,	/*** Only Client ***/
	HEADER_GC_MARK_BLOCK								= 76,
	HEADER_GC_MARK_DIFF_DATA							= 77,	/*** Only Client ***/
	HEADER_GC_MARK_IDXLIST								= 78,
	HEADER_GC_TIME										= 79,
	HEADER_GC_CHANGE_NAME								= 80,
	HEADER_GC_DUNGEON									= 81,
	HEADER_GC_WALK_MODE									= 82,
	HEADER_GC_CHANGE_SKILL_GROUP						= 83,
	HEADER_GC_MAIN_CHARACTER2_EMPIRE					= 84,
	HEADER_GC_SEPCIAL_EFFECT							= 85,
	HEADER_GC_NPC_POSITION								= 86,
	HEADER_GC_LOGIN_KEY									= 88,
	HEADER_GC_REFINE_INFORMATION           				= 89,
	HEADER_GC_CHANNEL									= 90,

	HEADER_GC_TARGET_UPDATE								= 92,
	HEADER_GC_TARGET_DELETE								= 93,
	HEADER_GC_TARGET_CREATE								= 94,
	HEADER_GC_AFFECT_ADD								= 95,
	HEADER_GC_AFFECT_REMOVE								= 96,
	HEADER_GC_MALL_OPEN									= 97,
	HEADER_GC_MALL_SET									= 98,
	HEADER_GC_MALL_DEL									= 99,
	HEADER_GC_LAND_LIST									= 100,
	HEADER_GC_LOVER_INFO								= 101,
	HEADER_GC_LOVE_POINT_UPDATE							= 102,
	HEADER_GC_GUILD_SYMBOL_DATA							= 103,
	HEADER_GC_DIG_MOTION								= 104,
	HEADER_GC_DAMAGE_INFO           					= 105,
	HEADER_GC_CHAR_ADDITIONAL_INFO						= 106,
	HEADER_GC_MAIN_CHARACTER3_BGM						= 107,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL					= 108,
	HEADER_GC_AUTH_SUCCESS								= 109,
	HEADER_GC_SPECIFIC_EFFECT							= 112,
	HEADER_GC_DRAGON_SOUL_REFINE						= 113,
	HEADER_GC_RESPOND_CHANNELSTATUS						= 114,
#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
	HEADER_GC_EVENT_INFO								= 115,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	HEADER_GC_RANKING_COMBAT_ZONE						= 116,
	HEADER_GC_SEND_COMBAT_ZONE							= 117,
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	HEADER_GC_SUPPORT_SHAMAN_SKILL						= 118,
#endif
#ifdef ENABLE_HEALTH_BOARD_SYSTEM
	HEADER_GC_HEALTH_BOARD								= 119,
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	HEADER_GC_EXTEND_INVENTORY							= 120,
#endif
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	HEADER_GC_EXCHANGE_INFO								= 121,
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_GC_TARGET_INFO								= 128,
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	HEADER_GC_VIEW_EQUIP								= 129,
#endif
#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	HEADER_GC_SHOPSEARCH_SET							= 130,
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	HEADER_GC_CL 										= 131,
#endif	
#ifdef ENABLE_ACCE_SYSTEM
	HEADER_GC_SASH 										= 132,
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
	HEADER_GC_FISH_EVENT_INFO 							= 133,
#endif
#ifdef ENABLE_GEM_SYSTEM
	HEADER_GC_GEM_SHOP_OPEN								= 135,
	HEADER_GC_GEM_SHOP_REFRESH							= 136,
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
    HEADER_GC_MAILBOX_RECEIVE							= 150,
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	HEADER_GC_SEPCIAL_ZODIAC_EFFECT						= 151,
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	HEADER_GC_SAFEBOX_MONEY_CHANGE 						= 152,
#endif

#ifdef ENABLE_AURA_SYSTEM
	HEADER_GC_AURA										= 154,
#endif
#ifdef ENABLE_SKILL_RENEWAL
	HEADER_GC_SKILL_BOOK_COUNT 							= 156,
#endif
#ifdef ENABLE_AVERAGE_ITEM_PRICE
	HEADER_GC_AVERAGE_ITEM_PRICE 						= 157,
#endif
#ifdef ENABLE_GUILD_RANKING_SYSTEM
	HEADER_GC_GUILD_RANK_SYSTEM							= 158,
#endif
#ifdef ENABLE_BOSS_MANAGER_SYSTEM
	HEADER_GC_BOSS_DATA									= 159,
#endif
	HEADER_GC_FEATURE_ENABLE 							= 155,
	HEADER_GC_CHARACTER_UPDATE2					= 138,	/*** Only Client ***/
	HEADER_GC_CHARACTER_ADD2                  	= 139,	/*** Only Client ***/
	HEADER_GC_SKILL_LEVEL_NEW					= 141,	/*** Only Client ***/
	HEADER_GC_KEY_AGREEMENT_COMPLETED			= 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
};

enum
{
	ID_MAX_NUM = 30,
	PASS_MAX_NUM = 16,
	SHOP_SIGN_MAX_LEN = 32,

	PLAYER_PER_ACCOUNT = 5,

	QUICKSLOT_MAX_LINE = 4,
	QUICKSLOT_MAX_COUNT_PER_LINE = 8, // 클라이언트 임의 결정값
	QUICKSLOT_MAX_COUNT = QUICKSLOT_MAX_LINE * QUICKSLOT_MAX_COUNT_PER_LINE,

	QUICKSLOT_MAX_NUM = 36, // 서버와 맞춰져 있는 값

	SHOP_HOST_ITEM_MAX_NUM = 45,
	METIN_SOCKET_COUNT = 6,

	PARTY_AFFECT_SLOT_MAX_NUM = 7,

	GUILD_GRADE_NAME_MAX_LEN = 8,
	GUILD_NAME_MAX_LEN = 12,
	GULID_COMMENT_MAX_LEN = 50,


	QUEST_INPUT_STRING_MAX_NUM = 64,
#ifdef ENABLE_OX_RENEWAL
	QUEST_INPUT_LONG_STRING_MAX_NUM = 128,
#endif

	PRIVATE_CODE_LENGTH = 8,

	REFINE_MATERIAL_MAX_NUM = 5,

	SHOP_TAB_NAME_MAX = 32,
	SHOP_TAB_COUNT_MAX = 3,
	
	/*** Security ***/
#ifdef ENABLE_HWID_BAN_SYSTEM
	CPU_ID_MAX_NUM = 128,
	HDD_MODEL_MAX_NUM = 128,
	MACHINE_GUID_MAX_NUM = 128,
	MAC_ADDR_MAX_NUM = 128,
	HDD_SERIAL_MAX_NUM = 128,
#endif
};

#pragma pack(push)
#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mark
typedef struct command_mark_login
{
    BYTE    header;
    DWORD   handle;
    DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
    BYTE    header;
    DWORD   gid;
    BYTE    image[16*12*4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
    BYTE    header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
    BYTE    header;
    BYTE    imgIdx;
    DWORD   crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
    BYTE    header;
	DWORD	bufSize;
    WORD    count;
    //뒤에 size * (WORD + WORD)만큼 데이터 붙음
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
    BYTE    header;
    DWORD   bufSize;
	BYTE	imgIdx;
    DWORD   count;
    // 뒤에 64 x 48 x 픽셀크기(4바이트) = 12288만큼 데이터 붙음
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	handle;
} TPacketCGSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE	header;
	DWORD	dwGuildID;
	DWORD	dwCRC;
	DWORD	dwSize;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
    BYTE header;
    WORD size;
    DWORD guild_id;
} TPacketGCGuildSymbolData;

//
//
//
typedef struct packet_observer_add
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverAdd;

typedef struct packet_observer_move
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverMove;

//server
typedef struct command_login
{
    BYTE header;
    char name[ID_MAX_NUM + 1];
    char pwd[PASS_MAX_NUM + 1];
} TPacketCGLogin;

// start - 권한 서버 접속을 위한 패킷들
typedef struct command_login2
{
	BYTE	header;
	char	name[ID_MAX_NUM + 1];
	DWORD	login_key;
    DWORD	adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
    BYTE	header;
    char	name[ID_MAX_NUM + 1];
    char	pwd[PASS_MAX_NUM + 1];
    DWORD	adwClientKey[4];
#ifdef ENABLE_HWID_BAN_SYSTEM
	char	cpu_id[CPU_ID_MAX_NUM + 1];
	char	hdd_model[HDD_MODEL_MAX_NUM + 1];
	char	machine_guid[MACHINE_GUID_MAX_NUM + 1];
	char	mac_addr[MAC_ADDR_MAX_NUM + 1];
	char	hdd_serial[HDD_SERIAL_MAX_NUM + 1];
#endif
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
	char	clientversion[PASS_MAX_NUM + 1];
	char	pin[PASS_MAX_NUM + 1];
#endif
#ifdef URIEL_AC_VERIFY
	DWORD	uriel1;
	DWORD	uriel2;
	DWORD	uriel3;
	DWORD	uriel4;
#endif
} TPacketCGLogin3;

#ifdef ENABLE_GROWTH_PET_SYSTEM
typedef struct packet_RequestPetName
{
	BYTE byHeader;
	char petname[13];
} TPacketCGRequestPetName;
#endif

typedef struct command_direct_enter
{
    BYTE        bHeader;
    char        login[ID_MAX_NUM + 1];
    char        passwd[PASS_MAX_NUM + 1];
    BYTE        index;
} TPacketCGDirectEnter;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	player_index;
} TPacketCGSelectCharacter;

typedef struct command_attack
{
	BYTE	header;
	BYTE	bType;			// 공격 유형
	DWORD	dwVictimVID;	// 적 VID
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

typedef struct command_chat
{
	BYTE	header;
	WORD	length;
	BYTE	type;
} TPacketCGChat;

typedef struct command_whisper
{
    BYTE        bHeader;
    WORD        wSize;
    char        szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_EnterFrontGame
{
	BYTE header;
} TPacketCGEnterFrontGame;

typedef struct command_item_use
{
	BYTE header;
	TItemPos pos;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE header;
	TItemPos source_pos;
	TItemPos target_pos;
} TPacketCGItemUseToItem;

#ifdef ENABLE_DROP_DIALOG_EX_SYSTEM
typedef struct command_item_delete
{
    BYTE		header;
	TItemPos	item_pos;
} TPacketCGItemDelete;

typedef struct command_item_sell
{
    BYTE		header;
	TItemPos	item_pos;
} TPacketCGItemSell;
#else
typedef struct command_item_drop
{
	BYTE  header;
	TItemPos pos;
	DWORD elk;
} TPacketCGItemDrop;
#endif

typedef struct command_item_move
{
	BYTE header;
	TItemPos pos;
	TItemPos change_pos;
	BYTE num;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE header;
	DWORD vid;
} TPacketCGItemPickUp;

typedef struct command_quickslot_add
{
    BYTE        header;
    UINT        pos;
	TQuickSlot	slot;
}TPacketCGQuickSlotAdd;

typedef struct command_quickslot_del
{
    BYTE        header;
    UINT        pos;
}TPacketCGQuickSlotDel;

typedef struct command_quickslot_swap
{
    BYTE        header;
    UINT        pos;
    UINT        change_pos;
}TPacketCGQuickSlotSwap;

typedef struct command_on_click
{
	BYTE		header;
	DWORD		vid;
} TPacketCGOnClick;

enum EShopSubheaderCG
{
    SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop
{
	BYTE        header;
	BYTE		subheader;
} TPacketCGShop;


enum
{
	EXCHANGE_SUBHEADER_CG_START,			// arg1 == vid of target character
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ELK_ADD,			// arg1 == amount of elk
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,	/* arg1 == amount of cheque */
#endif
	EXCHANGE_SUBHEADER_CG_ACCEPT,			// arg1 == not used
	EXCHANGE_SUBHEADER_CG_CANCEL,			// arg1 == not used
};

typedef struct command_exchange
{
	BYTE		header;
	BYTE		subheader;
	DWORD		arg1;
	BYTE		arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{   
    BYTE        header;
    BYTE        position;
} TPacketCGPosition;

typedef struct command_script_answer
{
    BYTE        header;
	BYTE		answer;
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
    BYTE        header;
	unsigned int			idx;
} TPacketCGScriptButton;

typedef struct command_target
{
    BYTE        header;
    DWORD       dwVID;
} TPacketCGTarget;

typedef struct command_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;	
} TPacketCGMove;

typedef struct command_sync_position_element 
{ 
    DWORD       dwVID; 
    long        lX; 
    long        lY; 
} TPacketCGSyncPositionElement; 

typedef struct command_sync_position
{ 
    BYTE        bHeader;
	WORD		wSize;
} TPacketCGSyncPosition; 

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
    BYTE        bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{   
    BYTE		bHeader;                
    BYTE		bType;
} TPacketCGShoot;

enum
{
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_GC_BLOCK_LIST,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGIN,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT,
	MESSENGER_SUBHEADER_GC_BLOCK_INVITE,//not used
#endif	
#ifdef ENABLE_MESSENGER_TEAM
	MESSENGER_SUBHEADER_GC_TEAM_LIST,
	MESSENGER_SUBHEADER_GC_TEAM_LOGIN,
	MESSENGER_SUBHEADER_GC_TEAM_LOGOUT,
#endif
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_REMOVE_FRIEND,
};

typedef struct packet_messenger
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_list_offline
{
    BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

#ifdef ENABLE_MESSENGER_BLOCK
typedef struct packet_messenger_block_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerBlockListOffline;

typedef struct packet_messenger_block_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerBlockListOnline;
#endif

#ifdef ENABLE_MESSENGER_TEAM
typedef struct packet_messenger_team_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerTeamListOffline;

typedef struct packet_messenger_team_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerTeamListOnline;
#endif

enum EMessengerState
{
	MESSENGER_CONNECTED_STATE_OFFLINE,
	MESSENGER_CONNECTED_STATE_ONLINE,
};

typedef struct packet_messenger_list_online
{
    BYTE connected;
	BYTE length;
	//BYTE length_char_name;
} TPacketGCMessengerListOnline;

typedef struct packet_messenger_login
{
	//BYTE length_login;
	//BYTE length_char_name;
	BYTE length;
} TPacketGCMessengerLogin;

typedef struct packet_messenger_logout
{
	BYTE length;
} TPacketGCMessengerLogout;

enum
{
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_VID,
    MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_NAME,
    MESSENGER_SUBHEADER_CG_REMOVE_BLOCK,
#endif
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
    MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
    MESSENGER_SUBHEADER_CG_REMOVE,
};

typedef struct command_messenger
{
    BYTE header;
    BYTE subheader;
} TPacketCGMessenger;


typedef struct command_safebox_checkout
{
    BYTE        bHeader;
    UINT        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
    BYTE        bHeader;
    UINT        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

typedef struct command_mall_checkout
{
    BYTE        bHeader;
    UINT        bMallPos;
    TItemPos	ItemPos;
} TPacketCGMallCheckout;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_use_skill
{
    BYTE                bHeader;
    DWORD               dwVnum;
	DWORD				dwTargetVID;
} TPacketCGUseSkill;

typedef struct command_party_invite
{
    BYTE header;
    DWORD vid;
} TPacketCGPartyInvite;

typedef struct command_party_invite_answer
{
    BYTE header;
    DWORD leader_pid;
    BYTE accept;
} TPacketCGPartyInviteAnswer;

typedef struct command_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
    BYTE byHeader;
    DWORD dwVID;
	BYTE byState;
    BYTE byFlag;
} TPacketCGPartySetState;

typedef struct packet_party_link
{
    BYTE header;
    DWORD pid;
    DWORD vid;
#ifdef ENABLE_PARTY_UPDATE
	long mapIdx;
	long channel;

	packet_party_link() {
		mapIdx = 0;
		channel = 0;
	}
#endif
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
    BYTE header;
    DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_use_skill
{
    BYTE byHeader;
	BYTE bySkillIndex;
    DWORD dwTargetVID;
} TPacketCGPartyUseSkill;

enum
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct command_guild
{
    BYTE byHeader;
	BYTE bySubHeader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild; 

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	BYTE byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
    BYTE        bHeader;
    char        szBuf[255 + 1];
} TPacketCGHack;


// Private Shop
typedef struct SShopItemTable
{
    DWORD		vnum;
    BYTE		count;

    TItemPos	pos;			// PC 상점에만 이용
    DWORD		price;			// PC 상점에만 이용
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		price_cheque;
#endif
    BYTE		display_pos;	//	PC 상점에만 이용, 보일 위치.
#ifdef ENABLE_BUY_WITH_ITEM
	DWORD		witemVnum;
#endif
} TShopItemTable;

typedef struct SPacketCGMyShop
{
    BYTE        bHeader;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
    int	        bCount;	// count of TShopItemTable, max 39
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	BYTE		days;
#endif
#ifdef ENABLE_SHOP_DECORATION_SYSTEM
	DWORD		model;
#endif
} TPacketCGMyShop;

typedef struct SPacketCGRefine
{
	BYTE		header;
	UINT		pos;
	BYTE		type;
} TPacketCGRefine;

typedef struct SPacketCGChangeName
{
    BYTE header;
    BYTE index;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct command_client_version
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion;

enum EPartyExpDistributionType
{
    PARTY_EXP_DISTRIBUTION_NON_PARITY,
    PARTY_EXP_DISTRIBUTION_PARITY,
};

typedef struct command_party_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketCGPartyParameter;

typedef struct command_quest_input_string
{
    BYTE        bHeader;
    char		szString[QUEST_INPUT_STRING_MAX_NUM+1];
} TPacketCGQuestInputString;

#ifdef ENABLE_OX_RENEWAL
typedef struct command_quest_input_long_string
{
	BYTE		bHeader;
	char		szString[QUEST_INPUT_LONG_STRING_MAX_NUM + 1];
} TPacketCGQuestInputLongString;
#endif

typedef struct command_quest_confirm
{
    BYTE header;
    BYTE answer;
    DWORD requestPID;
} TPacketCGQuestConfirm;

typedef struct command_script_select_item
{
    BYTE header;
    TItemPos selection;
} TPacketCGScriptSelectItem;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From Server
enum EPhase
{
    PHASE_CLOSE,				// 끊기는 상태 (또는 끊기 전 상태)
    PHASE_HANDSHAKE,			// 악수..;;
    PHASE_LOGIN,				// 로그인 중
    PHASE_SELECT,				// 캐릭터 선택 화면
    PHASE_LOADING,				// 선택 후 로딩 화면
    PHASE_GAME,					// 게임 화면
    PHASE_DEAD,					// 죽었을 때.. (게임 안에 있는 것일 수도..)

	PHASE_DBCLIENT_CONNECTING,	// 서버용
    PHASE_DBCLIENT,				// 서버용
    PHASE_P2P,					// 서버용
    PHASE_AUTH,					// 로그인 인증 용
};

typedef struct packet_phase
{
    BYTE        header;
    BYTE        phase;
} TPacketGCPhase;

typedef struct packet_blank		// 공백패킷.
{
	BYTE		header;
} TPacketGCBlank;

typedef struct packet_blank_dynamic
{
	BYTE		header;
	WORD		size;
} TPacketGCBlankDynamic;

typedef struct packet_header_handshake
{
	BYTE		header;
	DWORD		dwHandshake;
	DWORD		dwTime;
	LONG		lDelta;
} TPacketGCHandshake;

typedef struct packet_header_bindudp
{
	BYTE		header;
	DWORD		addr;
	WORD		port;
} TPacketGCBindUDP;

typedef struct packet_header_dynamic_size
{
	BYTE		header;
	WORD		size;
} TDynamicSizePacketHeader;

typedef struct SSimplePlayerInformation
{
    DWORD	dwID;
    char	szName[CHARACTER_NAME_MAX_LEN + 1];
    BYTE	byJob;
    BYTE	byLevel;
    DWORD	dwPlayMinutes;
    BYTE	byST, byHT, byDX, byIQ;
    WORD	wMainPart;
    BYTE	bChangeName;
	WORD	wHairPart;
#ifdef ENABLE_ACCE_SYSTEM
	WORD	wSashPart;
#endif
    BYTE	bDummy[4];
	long	x, y;
	LONG	lAddr;
	WORD	wPort;
	BYTE	bySkillGroup;
	DWORD		last_play;
} TSimplePlayerInformation;

typedef struct packet_login_success
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT];
    DWORD						guild_id[PLAYER_PER_ACCOUNT];
    char						guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess;



enum { LOGIN_STATUS_MAX_LEN = 8 };
typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[LOGIN_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE		shape;
	BYTE		CON;
	BYTE		INT;
	BYTE		STR;
	BYTE		DEX;
} TPacketCGCreateCharacter;

typedef struct command_player_create_success
{
    BYTE						header;
    BYTE						bAccountCharacterSlot;
    TSimplePlayerInformation	kSimplePlayerInfomation;
} TPacketGCPlayerCreateSuccess;

typedef struct command_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

typedef struct command_player_delete
{
	BYTE        header;
	BYTE        index;
	char		szPrivateCode[PRIVATE_CODE_LENGTH];
} TPacketCGDestroyCharacter;

typedef struct packet_player_delete_success
{
	BYTE        header;
	BYTE        account_index;
} TPacketGCDestroyCharacterSuccess;

enum
{
	ADD_CHARACTER_STATE_DEAD   = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN  = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY  = (1 << 4),
};

enum EPKModes
{
	PK_MODE_PEACE,
	PK_MODE_REVENGE,
	PK_MODE_FREE,
	PK_MODE_PROTECT,
	PK_MODE_GUILD,
	PK_MODE_MAX_NUM,
};

// 2004.11.20.myevan.CRaceData::PART_MAX_NUM 사용안하게 수정 - 서버에서 사용하는것과 일치하지 않음
enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,
#ifdef ENABLE_ACCE_SYSTEM
	CHR_EQUIPPART_SASH,
#endif
#ifdef ENABLE_AURA_SYSTEM
	CHR_EQUIPPART_AURA,
#endif
	CHR_EQUIPPART_NUM,		
};

typedef struct packet_char_additional_info
{
	BYTE    header;
	DWORD   dwVID;
	char    name[CHARACTER_NAME_MAX_LEN + 1];
	WORD    awPart[CHR_EQUIPPART_NUM];
	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
#ifdef ENABLE_ALIGNMENT_SYSTEM
	long long	sAlignment;
#else
	int	sAlignment;
#endif
	BYTE    bPKMode;
	DWORD   dwMountVnum;
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	BYTE	combat_zone_rank;
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	bool	bIsSupportShaman;
#endif
#ifdef ENABLE_GUILD_LEADER_SYSTEM
	BYTE	bMemberType;
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
	DWORD	dwWeaponRareLv;
#endif
#ifdef ENABLE_EFFECT_STONE_SYSTEM
	DWORD	bStoneEffect;
	DWORD	bWeaponStoneEffect;
#endif
#ifdef ENABLE_QUIVER_SYSTEM
	DWORD	dwArrow;
#endif
} TPacketGCCharacterAdditionalInfo;

typedef struct packet_add_char
{
    BYTE        header;
    DWORD       dwVID;
#if defined(WJ_SHOW_MOB_INFO)
	DWORD		dwLevel;
	DWORD		dwAIFlag;
#endif

    //char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    //WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    //BYTE      bEmpire;
    //DWORD     dwGuild;
    //short     sAlignment;	
	//BYTE		bPKMode;
	//DWORD		dwMountVnum;
} TPacketGCCharacterAdd;

typedef struct packet_add_char2
{
    BYTE        header;

    DWORD       dwVID;

    char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    BYTE        bEmpire;

    DWORD       dwGuild;
	int	sAlignment;
	DWORD		dwLevel;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
} TPacketGCCharacterAdd2;

typedef struct packet_update_char
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
#ifdef ENABLE_ALIGNMENT_SYSTEM
	long long	sAlignment;
#else
	int	sAlignment;
#endif
	DWORD		dwLevel;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	DWORD		combat_zone_points;
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	bool		bIsSupportShaman;
#endif
#ifdef ENABLE_GUILD_LEADER_SYSTEM
	BYTE		bMemberType;
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
	DWORD	dwWeaponRareLv;
#endif
#ifdef ENABLE_EFFECT_STONE_SYSTEM
	DWORD	bStoneEffect;
	DWORD	bWeaponStoneEffect;
#endif
#ifdef ENABLE_QUIVER_SYSTEM
	DWORD	dwArrow;
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_update_char2
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
    int       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
} TPacketGCCharacterUpdate2;
typedef struct packet_del_char
{
	BYTE	header;
    DWORD	dwVID;
} TPacketGCCharacterDelete;

enum EChatType
{
	CHAT_TYPE_TALKING,  /* 그냥 채팅 */
	CHAT_TYPE_INFO,     /* 정보 (아이템을 집었다, 경험치를 얻었다. 등) */
	CHAT_TYPE_NOTICE,   /* 공지사항 */
	CHAT_TYPE_PARTY,    /* 파티말 */
	CHAT_TYPE_GUILD,    /* 길드말 */
	CHAT_TYPE_COMMAND,	/* 명령 */
	CHAT_TYPE_SHOUT,	/* 외치기 */
	CHAT_TYPE_WHISPER,	// 서버와는 연동되지 않는 Only Client Enum
	CHAT_TYPE_BIG_NOTICE,
#ifdef ENABLE_DICE_SYSTEM
	CHAT_TYPE_DICE_INFO, //11
#endif
#ifdef ENABLE_OX_RENEWAL
	CHAT_TYPE_CONTROL_NOTICE,
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_CHAT
	CHAT_TYPE_ZODIAC_NOTICE,
	CHAT_TYPE_ZODIAC_SUB_NOTICE,
	CHAT_TYPE_ZODIAC_NOTICE_CLEAR,
#endif
	CHAT_TYPE_MAX_NUM,
};

typedef struct packet_chatting
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	dwVID;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper   // 가변 패킷    
{   
    BYTE        bHeader;                        
    WORD        wSize;
    BYTE        bType;
    char        szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
	BYTE		bLevel;
} TPacketGCWhisper;

typedef struct packet_stun
{
	BYTE		header;
	DWORD		vid;
} TPacketGCStun;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
enum EPacketCGCombatZoneSubHeaderType
{
	COMBAT_ZONE_SUB_HEADER_NONE,
	COMBAT_ZONE_SUB_HEADER_ADD_LEAVING_TARGET,
	COMBAT_ZONE_SUB_HEADER_REMOVE_LEAVING_TARGET,	
	COMBAT_ZONE_SUB_HEADER_FLASH_ON_MINIMAP,
	COMBAT_ZONE_SUB_HEADER_OPEN_RANKING,
	COMBAT_ZONE_SUB_HEADER_REFRESH_SHOP,
};

typedef struct SPacketCGCombatZoneRequestAction
{
	BYTE	header;
	int		action;
	int		value;
} TPacketCGCombatZoneRequestAction;

typedef struct SPacketGCCombatZoneRanking
{
	int		rank;
	char	name[12 + 1];
	int		empire;
	int		points;
} TPacketGCCombatZoneRanking;

typedef struct SPacketGCCombatZoneRankingData
{
	BYTE header;
	TPacketGCCombatZoneRanking rankingData[11];
} TPacketGCCombatZoneRankingData;

typedef struct SPacketGCSendCombatZone
{
	BYTE	header;
	DWORD	sub_header;
	DWORD	m_pInfoData[4];
	DWORD	m_pDataDays[7][8];
	bool	isRunning;
} TPacketGCSendCombatZone;
#endif

typedef struct packet_dead
{
	BYTE		header;
	DWORD		vid;
} TPacketGCDead;

// SUPPORT_BGM
typedef struct packet_main_character2_empire
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter2_EMPIRE;

typedef struct packet_main_character3_bgm
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[24 + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[24 + 1];
	float		fBGMVol;
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

enum EPointTypes
{
	POINT_NONE						= 0,
	POINT_LEVEL						= 1,
	POINT_VOICE						= 2,
	POINT_EXP						= 3,
	POINT_NEXT_EXP					= 4,
	POINT_HP						= 5,
	POINT_MAX_HP					= 6,
	POINT_SP						= 7,
	POINT_MAX_SP					= 8,
	POINT_STAMINA					= 9,
	POINT_MAX_STAMINA				= 10,
	POINT_GOLD						= 11,
	POINT_ST						= 12,
	POINT_HT						= 13,
	POINT_DX						= 14,
	POINT_IQ						= 15,
	POINT_ATT_POWER					= 16,
	POINT_ATT_SPEED					= 17,
	POINT_EVADE_RATE				= 18,
	POINT_MOV_SPEED					= 19,
	POINT_DEF_GRADE					= 20,
	POINT_CASTING_SPEED				= 21,
	POINT_MAGIC_ATT_GRADE			= 22,
	POINT_MAGIC_DEF_GRADE			= 23,
	POINT_EMPIRE_POINT				= 24,
	POINT_LEVEL_STEP				= 25,
	POINT_STAT						= 26,
	POINT_SUB_SKILL					= 27,
	POINT_SKILL						= 28,
	POINT_MIN_ATK					= 29,
	POINT_MAX_ATK					= 30,
	POINT_PLAYTIME					= 31,
	POINT_HP_REGEN					= 32,
	POINT_SP_REGEN					= 33,
	POINT_BOW_DISTANCE				= 34,
	POINT_HP_RECOVERY				= 35,
	POINT_SP_RECOVERY				= 36,
	POINT_POISON_PCT				= 37,
	POINT_STUN_PCT					= 38,
	POINT_SLOW_PCT					= 39,
	POINT_CRITICAL_PCT				= 40,
	POINT_PENETRATE_PCT				= 41,
	POINT_CURSE_PCT					= 42,
	POINT_ATTBONUS_HUMAN			= 43,
	POINT_ATTBONUS_ANIMAL			= 44,
	POINT_ATTBONUS_ORC				= 45,
	POINT_ATTBONUS_MILGYO			= 46,
	POINT_ATTBONUS_UNDEAD			= 47,
	POINT_ATTBONUS_DEVIL			= 48,
	POINT_ATTBONUS_INSECT			= 49,
	POINT_ATTBONUS_FIRE				= 50,
	POINT_ATTBONUS_ICE				= 51,
	POINT_ATTBONUS_DESERT			= 52,
	POINT_ATTBONUS_UNUSED0			= 53,
	POINT_ATTBONUS_UNUSED1			= 54,
	POINT_ATTBONUS_UNUSED2			= 55,
	POINT_ATTBONUS_UNUSED3			= 56,
	POINT_ATTBONUS_UNUSED4			= 57,
	POINT_ATTBONUS_UNUSED5			= 58,
	POINT_ATTBONUS_UNUSED6			= 59,
	POINT_ATTBONUS_UNUSED7			= 60,
	POINT_ATTBONUS_UNUSED8			= 61,
	POINT_ATTBONUS_UNUSED9			= 62,
	POINT_STEAL_HP					= 63,
	POINT_STEAL_SP					= 64,
	POINT_MANA_BURN_PCT				= 65,
	POINT_DAMAGE_SP_RECOVER			= 66,
	POINT_BLOCK						= 67,
	POINT_DODGE						= 68,
	POINT_RESIST_SWORD				= 69,
	POINT_RESIST_TWOHAND			= 70,
	POINT_RESIST_DAGGER				= 71,
	POINT_RESIST_BELL				= 72,
	POINT_RESIST_FAN				= 73,
	POINT_RESIST_BOW				= 74,
	POINT_RESIST_FIRE				= 75,
	POINT_RESIST_ELEC				= 76,
	POINT_RESIST_MAGIC				= 77,
	POINT_RESIST_WIND				= 78,
	POINT_REFLECT_MELEE				= 79,
	POINT_REFLECT_CURSE				= 80,
	POINT_POISON_REDUCE				= 81,
	POINT_KILL_SP_RECOVER			= 82,
	POINT_EXP_DOUBLE_BONUS			= 83,
	POINT_GOLD_DOUBLE_BONUS			= 84,
	POINT_ITEM_DROP_BONUS			= 85,
	POINT_POTION_BONUS				= 86,
	POINT_KILL_HP_RECOVERY			= 87,
	POINT_IMMUNE_STUN				= 88,
	POINT_IMMUNE_SLOW				= 89,
	POINT_IMMUNE_FALL				= 90,
	POINT_PARTY_ATT_GRADE			= 91,
	POINT_PARTY_DEF_GRADE			= 92,
	POINT_ATT_BONUS					= 93,
	POINT_DEF_BONUS					= 94,
	POINT_ATT_GRADE_BONUS			= 95,
	POINT_DEF_GRADE_BONUS			= 96,
	POINT_MAGIC_ATT_GRADE_BONUS		= 97,
	POINT_MAGIC_DEF_GRADE_BONUS		= 98,
	POINT_RESIST_NORMAL_DAMAGE		= 99,
	POINT_HIT_HP_RECOVERY			= 100,
	POINT_HIT_SP_RECOVERY			= 101,
	POINT_MANASHIELD				= 102,
	POINT_PARTY_BUFFER_BONUS		= 103,
	POINT_PARTY_SKILL_MASTER_BONUS	= 104,
	POINT_HP_RECOVER_CONTINUE		= 105,
	POINT_SP_RECOVER_CONTINUE		= 106,
	POINT_STEAL_GOLD				= 107,
	POINT_POLYMORPH					= 108,
	POINT_MOUNT						= 109,
	POINT_PARTY_HASTE_BONUS			= 110,
	POINT_PARTY_DEFENDER_BONUS		= 111,
	POINT_STAT_RESET_COUNT			= 112,
	POINT_HORSE_SKILL				= 113,
	POINT_MALL_ATTBONUS				= 114,
	POINT_MALL_DEFBONUS				= 115,
	POINT_MALL_EXPBONUS				= 116,
	POINT_MALL_ITEMBONUS			= 117,
	POINT_MALL_GOLDBONUS			= 118,
	POINT_MAX_HP_PCT				= 119,
	POINT_MAX_SP_PCT				= 120,
	POINT_SKILL_DAMAGE_BONUS		= 121,
	POINT_NORMAL_HIT_DAMAGE_BONUS	= 122,
	POINT_SKILL_DEFEND_BONUS		= 123,
	POINT_NORMAL_HIT_DEFEND_BONUS	= 124,
	POINT_RAMADAN_CANDY_BONUS_EXP	= 125,
	POINT_ENERGY 					= 126,
	POINT_ENERGY_END_TIME 			= 127,
	POINT_COSTUME_ATTR_BONUS		= 128,
	POINT_MAGIC_ATT_BONUS_PER		= 129,
	POINT_MELEE_MAGIC_ATT_BONUS_PER = 130,
	POINT_RESIST_ICE				= 131,
	POINT_RESIST_EARTH				= 132,
	POINT_RESIST_DARK				= 133,
	POINT_RESIST_CRITICAL			= 134,
	POINT_RESIST_PENETRATE			= 135,
	POINT_BLEEDING_PCT				= 136,
	POINT_BLEEDING_REDUCE			= 137,
	POINT_ATTBONUS_WOLFMAN			= 138,
	POINT_RESIST_WOLFMAN			= 139,
	POINT_RESIST_CLAW				= 140,
	POINT_ANTI_RESIST_MAGIC 		= 143,
#ifdef ENABLE_CHEQUE_SYSTEM
	POINT_CHEQUE 					= 145,
#endif
#ifdef ENABLE_GEM_SYSTEM
	POINT_GEM 						= 146,
#endif
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	POINT_ATTBONUS_SWORD	 		= 147,
	POINT_ATTBONUS_TWOHANDED 		= 148,
	POINT_ATTBONUS_DAGGER 			= 149,
	POINT_ATTBONUS_BELL 			= 150,
	POINT_ATTBONUS_FAN 				= 151,
	POINT_ATTBONUS_BOW 				= 152,
#endif
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	POINT_ATTBONUS_ELEC				= 154,
	POINT_ATTBONUS_WIND				= 155,
	POINT_ATTBONUS_EARTH			= 156,
	POINT_ATTBONUS_DARK				= 157,
#endif
	POINT_RESIST_HUMAN 				= 158,
#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
	POINT_YEAR_ROUND				= 159,
#endif
	POINT_ATTBONUS_STONE			= 160,
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	POINT_ZODIAC					= 163,
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	POINT_WORSHIP					= 164,
#endif
	POINT_ATTBONUS_CZ				= 165,
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
	POINT_MIN_WEP = 200,
	POINT_MAX_WEP,
	POINT_MIN_MAGIC_WEP,
	POINT_MAX_MAGIC_WEP,
	POINT_HIT_RATE,
};

typedef struct packet_points
{
    BYTE        header;
    long        points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_point_change
{
    int         header;

	DWORD		dwVID;
	BYTE		Type;

	long        amount; // 바뀐 값
    long        value;  // 현재 값
} TPacketGCPointChange;

typedef struct packet_motion
{
	BYTE		header;
	DWORD		vid;
	DWORD		victim_vid;
	WORD		motion;
} TPacketGCMotion;




typedef struct packet_set_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	BYTE		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD		transmutation;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet;

typedef struct packet_set_item2
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	BYTE		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD		transmutation;
#endif
	DWORD		flags;
	DWORD		anti_flags;
	bool		highlight;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	long		sealbind;
#endif
	bool		is_basic;
} TPacketGCItemSet2;

typedef struct packet_item_del
{
    BYTE        header;
    UINT        pos;
} TPacketGCItemDel;

typedef struct packet_use_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		ch_vid;
	DWORD		victim_vid;

	DWORD		vnum;
} TPacketGCItemUse;

typedef struct packet_update_item
{
	BYTE		header;
	TItemPos	Cell;
	BYTE		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD		transmutation;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	long		sealbind;
#endif
	mutable bool		is_basic;
} TPacketGCItemUpdate;

typedef struct packet_ground_add_item
{
    BYTE        bHeader;
    long        lX;
	long		lY;
	long		lZ;

    DWORD       dwVID;
    DWORD       dwVnum;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	DWORD		count;
} TPacketGCItemGroundAdd;

typedef struct packet_ground_del_item
{
	BYTE		header;
	DWORD		vid;
} TPacketGCItemGroundDel;

typedef struct packet_item_ownership
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_quickslot_add
{
    BYTE        header;
    UINT        pos;
	TQuickSlot	slot;
} TPacketGCQuickSlotAdd;

typedef struct packet_quickslot_del
{
    BYTE        header;
    UINT        pos;
} TPacketGCQuickSlotDel;

typedef struct packet_quickslot_swap
{
    BYTE        header;
    UINT        pos;
    UINT        change_pos;
} TPacketGCQuickSlotSwap;

typedef struct packet_shop_start
{
	DWORD	owner_vid;
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
#ifdef ENABLE_SHOP_SEEN
	DWORD	bakanlar;
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	bool	islimiteditemshop;
#endif
} TPacketGCShopStart;

typedef struct packet_shop_start_ex // 다음에 TSubPacketShopTab* shop_tabs 이 따라옴.
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	DWORD points;
	DWORD curLimit;
	DWORD maxLimit;
#endif
} TPacketGCShopStartEx;


typedef struct packet_shop_update_item
{
	BYTE						pos;
	struct packet_shop_item		item;
} TPacketGCShopUpdateItem;

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
	SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
	SHOP_SUBHEADER_GC_OK,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
	SHOP_SUBHEADER_GC_SOLDOUT,
	SHOP_SUBHEADER_GC_INVENTORY_FULL,
	SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_YEAR_ROUND_COIN,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	SHOP_SUBHEADER_GC_NOT_ENOUGH_POINTS,
	SHOP_SUBHEADER_GC_MAX_LIMIT_POINTS,
	SHOP_SUBHEADER_GC_OVERFLOW_LIMIT_POINTS,
#endif
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_GAYA_POINT,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ZODIAC_POINT,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_EXP,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_DRAGON_COIN,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_DRAGON_MARK,
	SHOP_SUBHEADER_GC_MAX_COUNT,
	SHOP_SUBHEADER_GC_ENOUGH_STOCK,
	SHOP_SUBHEADER_GC_WAIT_BUY_TIME,
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ZODIAC_ITEM,
#endif
#ifdef ENABLE_WORSHIP_SYSTEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_WORSHIP_POINT,
#endif
#ifdef ENABLE_BUY_WITH_ITEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM,
#endif
};

typedef struct packet_shop
{
	BYTE        header;
	WORD		size;
	BYTE        subheader;
} TPacketGCShop;

typedef struct packet_exchange
{
    BYTE        header;
    BYTE        subheader;
    BYTE        is_me;
    DWORD       arg1;
    TItemPos       arg2;
    DWORD       arg3;
#ifdef WJ_TRADABLE_ICON
    TItemPos       arg4;
#endif
	long		alValues[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	dwTransmutation;
#endif
} TPacketGCExchange;

enum
{
	EXCHANGE_SUBHEADER_GC_START,			// arg1 == vid
	EXCHANGE_SUBHEADER_GC_ITEM_ADD,		// arg1 == vnum  arg2 == pos  arg3 == count
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,		// arg1 == pos
	EXCHANGE_SUBHEADER_GC_ELK_ADD,			// arg1 == elk
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,	/* arg1 == cheque */
#endif
	EXCHANGE_SUBHEADER_GC_ACCEPT,			// arg1 == accept
	EXCHANGE_SUBHEADER_GC_END,				// arg1 == not used
	EXCHANGE_SUBHEADER_GC_ALREADY,			// arg1 == not used
	EXCHANGE_SUBHEADER_GC_LESS_ELK,		// arg1 == not used
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,	/* arg1 == not used */
#endif
};

typedef struct packet_position
{
    BYTE        header;
	DWORD		vid;
    BYTE        position;
} TPacketGCPosition;

typedef struct packet_ping
{
	BYTE		header;
} TPacketGCPing;

typedef struct packet_pong
{
	BYTE		bHeader;
} TPacketCGPong;

typedef struct packet_script
{
    BYTE		header;
    WORD        size;
	BYTE		skin;
    WORD        src_size;
} TPacketGCScript;

typedef struct packet_target
{
    BYTE        header;
    DWORD       dwVID;
    BYTE        bHPPercent;
#ifdef ENABLE_VIEW_TARGET_MONSTER_HP
	int			iMinHP;
	int			iMaxHP;
#endif
	BYTE	bElement;
} TPacketGCTarget;

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
typedef struct packet_target_info
{
	BYTE	header;
	DWORD	dwVID;
	DWORD	race;
	DWORD	dwVnum;
	BYTE	count;
} TPacketGCTargetInfo;

typedef struct packet_target_info_load
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTargetInfoLoad;
#endif

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int  damage;
} TPacketGCDamageInfo;

typedef struct packet_mount
{
    BYTE        header;
    DWORD       vid;
    DWORD       mount_vid;
    BYTE        pos;
	DWORD		_x, _y;
} TPacketGCMount;

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

typedef struct packet_move
{	
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

enum
{
	QUEST_SEND_IS_BEGIN         = 1 << 0,
    QUEST_SEND_TITLE            = 1 << 1,  // 28자 까지
    QUEST_SEND_CLOCK_NAME       = 1 << 2,  // 16자 까지
    QUEST_SEND_CLOCK_VALUE      = 1 << 3,
    QUEST_SEND_COUNTER_NAME     = 1 << 4,  // 16자 까지
    QUEST_SEND_COUNTER_VALUE    = 1 << 5,
	QUEST_SEND_ICON_FILE		= 1 << 6,  // 24자 까지 
};

typedef struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
	WORD c_index;
	BYTE flag;
} TPacketGCQuestInfo;

typedef struct packet_quest_confirm
{
    BYTE header;
    char msg[64+1];
    long timeout;
    DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_c2c
{
	BYTE		header;
	WORD		wSize;
} TPacketGCC2C;

typedef struct packetd_sync_position_element 
{ 
    DWORD       dwVID; 
    long        lX; 
    long        lY; 
} TPacketGCSyncPositionElement; 

typedef struct packetd_sync_position
{ 
    BYTE        bHeader; 
    WORD		wSize;
} TPacketGCSyncPosition; 

typedef struct packet_ownership 
{ 
    BYTE                bHeader; 
    DWORD               dwOwnerVID; 
    DWORD               dwVictimVID; 
} TPacketGCOwnership; 

#define	SKILL_MAX_NUM 255

typedef struct packet_skill_level
{
    BYTE        bHeader;
    BYTE        abSkillLevels[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct SPlayerSkill
{
	BYTE bMasterType;
	BYTE bLevel;
	time_t tNextRead;
} TPlayerSkill;

typedef struct packet_skill_level_new
{
	BYTE bHeader;
	TPlayerSkill skills[SKILL_MAX_NUM];
} TPacketGCSkillLevelNew;

// fly
typedef struct packet_fly
{
    BYTE        bHeader;
    BYTE        bType;
    DWORD       dwStartVID;
    DWORD       dwEndVID;
} TPacketGCCreateFly;

enum EPVPModes
{
	PVP_MODE_NONE,
    PVP_MODE_AGREE,
    PVP_MODE_FIGHT,
    PVP_MODE_REVENGE,
};

typedef struct packet_duel_start
{
    BYTE	header ;
    WORD	wSize ;	// DWORD가 몇개? 개수 = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart ;

typedef struct packet_pvp
{
	BYTE		header;
	DWORD		dwVIDSrc;
	DWORD		dwVIDDst;
	BYTE		bMode;
} TPacketGCPVP;

typedef struct packet_skill_cooltime_end
{
	BYTE		header;
	BYTE		bSkill;
} TPacketGCSkillCoolTimeEnd;

typedef struct packet_warp
{
	BYTE			bHeader;
	LONG			lX;
	LONG			lY;
	LONG			lAddr;
	WORD			wPort;
} TPacketGCWarp;

typedef struct packet_party_invite
{
    BYTE header;
    DWORD leader_pid;
} TPacketGCPartyInvite;

typedef struct packet_party_add
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
#ifdef ENABLE_PARTY_UPDATE
	long mapIdx;
	long channel;

	packet_party_add() {
		mapIdx = 0;
		channel = 0;
	}
#endif
} TPacketGCPartyAdd;

typedef struct packet_party_update
{
    BYTE header;
    DWORD pid;
    BYTE state;
    BYTE percent_hp;
    short affects[PARTY_AFFECT_SLOT_MAX_NUM];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketGCPartyRemove;


typedef struct packet_safebox_wrong_password
{
    BYTE        bHeader;
} TPacketGCSafeboxWrongPassword;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCSafeboxSize; 


typedef struct command_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketGCEmpire;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
    FISHING_SUBHEADER_GC_FISH,
};

typedef struct packet_fishing
{
    BYTE header;
    BYTE subheader;
    DWORD info;
    BYTE dir;
} TPacketGCFishing;

typedef struct paryt_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketGCPartyParameter;

//////////////////////////////////////////////////////////////////////////
// Guild

enum
{
    GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH, 
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
    GUILD_SUBHEADER_GC_CHANGE_EXP,
    GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
    GUILD_SUBHEADER_GC_WAR,
    GUILD_SUBHEADER_GC_GUILD_NAME,
    GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
    GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
    GUILD_SUBHEADER_GC_WAR_POINT,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
	GUILD_SUBHEADER_GC_WAR_SKILL,
};

typedef struct packet_guild
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCGuild;

// SubHeader - Grade
enum
{
    GUILD_AUTH_ADD_MEMBER       = (1 << 0),
    GUILD_AUTH_REMOVE_MEMBER    = (1 << 1),
    GUILD_AUTH_NOTICE           = (1 << 2),
    GUILD_AUTH_SKILL            = (1 << 3),
	GUILD_AUTH_WAR				= (1 << 4),
};

typedef struct packet_guild_sub_grade
{
	char grade_name[GUILD_GRADE_NAME_MAX_LEN+1]; // 8+1 길드장, 길드원 등의 이름
	BYTE auth_flag;
} TPacketGCGuildSubGrade;

typedef struct packet_guild_sub_member
{
	DWORD pid;
	BYTE byGrade;
	BYTE byIsGeneral;
	BYTE byJob;
	BYTE byLevel;
	DWORD dwOffer;
	BYTE byNameFlag;
// if NameFlag is TRUE, name is sent from server.
//	char szName[CHARACTER_ME_MAX_LEN+1];
} TPacketGCGuildSubMember;

typedef struct packet_guild_sub_info
{
    WORD member_count;
    WORD max_member_count;
	DWORD guild_id;
    DWORD master_pid;
    DWORD exp;
    BYTE level;
    char name[GUILD_NAME_MAX_LEN+1];
	DWORD gold;
	BYTE hasLand;
} TPacketGCGuildInfo;

enum EGuildWarState
{
    GUILD_WAR_NONE,
    GUILD_WAR_SEND_DECLARE,
    GUILD_WAR_REFUSE,
    GUILD_WAR_RECV_DECLARE,
    GUILD_WAR_WAIT_START,
    GUILD_WAR_CANCEL,
    GUILD_WAR_ON_WAR,
    GUILD_WAR_END,

    GUILD_WAR_DURATION = 2*60*60, // 2시간
};

typedef struct packet_guild_war
{
    DWORD       dwGuildSelf;
    DWORD       dwGuildOpp;
    BYTE        bType;
    BYTE        bWarState;
#ifdef ENABLE_IMPROVED_GUILD_WAR
	int		iMaxPlayer;
	int		iMaxScore;
#endif
} TPacketGCGuildWar;

typedef struct SPacketGuildWarPoint
{
    DWORD dwGainGuildID;
    DWORD dwOpponentGuildID;
    long lPoint;
} TPacketGuildWarPoint;

typedef struct SPacketGuildWarSkill
{
	DWORD guild_id;
	BYTE skillone;
	BYTE skilltwo;
	BYTE skillthree;
	BYTE skillfour;
	BYTE skillfive;
	BYTE skillsix;
} TPacketGuildWarSkill;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE		bHeader;
    WORD		size;
    BYTE		subheader;
} TPacketGCDungeon;

// Private Shop
typedef struct SPacketGCShopSign
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketGCTime
{
    BYTE        bHeader;
    time_t      time;
} TPacketGCTime;

enum
{
    WALKMODE_RUN,
    WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
    BYTE        header;
    DWORD       vid;
    BYTE        mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
    BYTE        header;
    BYTE        skill_group;
} TPacketGCChangeSkillGroup;

struct TMaterial
{
    DWORD vnum;
    DWORD count;
};

typedef struct SRefineTable
{
    DWORD src_vnum;
    DWORD result_vnum;
    BYTE material_count;
    int cost; // 소요 비용
    int prob; // 확률
    TMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TRefineTable;


typedef struct SPacketGCRefineInformation
{
	BYTE			header;
	BYTE			type;
	UINT			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformation;

enum ESpecialEffect
{
	SE_NONE,
	SE_HPUP_RED,
	SE_SPUP_BLUE,
	SE_SPEEDUP_GREEN,
	SE_DXUP_PURPLE,
	SE_CRITICAL,
	SE_PENETRATE,
	SE_BLOCK,
	SE_DODGE,
	SE_CHINA_FIREWORK,
	SE_SPIN_TOP,
	SE_SUCCESS,
	SE_FAIL,
    SE_LEVELUP_ON_14_FOR_GERMANY,	//레벨업 14일때 ( 독일전용 )
    SE_LEVELUP_UNDER_15_FOR_GERMANY,//레벨업 15일때 ( 독일전용 )
    SE_PERCENT_DAMAGE1,
    SE_PERCENT_DAMAGE2,
    SE_PERCENT_DAMAGE3,    
	SE_AUTO_HPUP,
	SE_AUTO_SPUP,
	SE_EQUIP_RAMADAN_RING,			// 초승달의 반지를 착용하는 순간에 발동하는 이펙트
	SE_EQUIP_HALLOWEEN_CANDY,		// 할로윈 사탕을 착용(-_-;)한 순간에 발동하는 이펙트
	SE_EQUIP_HAPPINESS_RING,		// 크리스마스 행복의 반지를 착용하는 순간에 발동하는 이펙트
	SE_EQUIP_LOVE_PENDANT,		// 발렌타인 사랑의 팬던트(71145) 착용할 때 이펙트 (발동이펙트임, 지속이펙트 아님)
#ifdef ENABLE_VERSION_162_ENABLED
	SE_EFFECT_HEALER,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	SE_COMBAT_ZONE_POTION,
#endif
#ifdef ENABLE_ACCE_SYSTEM
	SE_EFFECT_SASH_SUCCEDED,
	SE_EFFECT_SASH_EQUIP,
#endif
	// NEW_EFFECTS
	SE_PVP_WIN,
	SE_PVP_OPEN1,
	SE_PVP_OPEN2,

	SE_PVP_BEGIN1,
	SE_PVP_BEGIN2,

	SE_FR_SUCCESS,
	SE_FR_FAIL,
	// NEW_EFFECTS
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	SE_SKILL_DAMAGE_ZONE,
	SE_SKILL_DAMAGE_ZONE_BUYUK,
	SE_SKILL_DAMAGE_ZONE_ORTA,
	SE_SKILL_DAMAGE_ZONE_KUCUK,
	SE_SKILL_SAFE_ZONE,
	SE_SKILL_SAFE_ZONE_BUYUK,
	SE_SKILL_SAFE_ZONE_ORTA,
	SE_SKILL_SAFE_ZONE_KUCUK,
	SE_METEOR,
	SE_BEAD_RAIN,
	SE_FALL_ROCK,
	SE_ARROW_RAIN,
	SE_HORSE_DROP,
	SE_EGG_DROP,
	SE_DEAPO_BOOM,
#endif
};

typedef struct SPacketGCSpecialEffect
{
    BYTE header;
    BYTE type;
    DWORD vid;
} TPacketGCSpecialEffect;

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
typedef struct SPacketGCSpecialZodiacEffect
{
	BYTE header;
	BYTE type;
	BYTE type2;
	DWORD vid;
	long x;
	long y;
} TPacketGCSpecialZodiacEffect;
#endif

typedef struct SPacketGCNPCPosition
{
    BYTE header;
	WORD size;
    WORD count;
} TPacketGCNPCPosition;

struct TNPCPosition
{
    BYTE bType;
    char name[CHARACTER_NAME_MAX_LEN+1];
    long x;
    long y;
};

typedef struct SPacketGCChangeName
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;

enum EBlockAction
{
    BLOCK_EXCHANGE              = (1 << 0),
    BLOCK_PARTY_INVITE          = (1 << 1),
    BLOCK_GUILD_INVITE          = (1 << 2),
    BLOCK_WHISPER               = (1 << 3),
    BLOCK_MESSENGER_INVITE      = (1 << 4),
    BLOCK_PARTY_REQUEST         = (1 << 5),
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	BLOCK_EQUIPMENT_REQUEST 	= (1 << 6),
#endif
};

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct packet_auth_success
{
    BYTE        bHeader;
    DWORD       dwLoginKey;
    BYTE        bResult;
} TPacketGCAuthSuccess;

typedef struct packet_channel
{
    BYTE header;
    BYTE channel;
} TPacketGCChannel;

#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
typedef struct SEquipmentItemSet
{
	DWORD   vnum;
	BYTE    count;
	long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
	long	sealbind;
	DWORD	transmutation;
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
    BYTE    header;
    DWORD   dwVID;
    TEquipmentItemSet equips[20];
} TPacketGCViewEquip;
#endif

typedef struct
{
    DWORD       dwID;
    long        x, y;
    long        width, height;
    DWORD       dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
    BYTE        header;
    WORD        size;
} TPacketGCLandList;

typedef struct
{
	BYTE		bHeader;
	long		lID;
	char		szTargetName[32+1];
	DWORD		dwVID;
	BYTE		byType;
} TPacketGCTargetCreate;

enum
{
	CREATE_TARGET_TYPE_NONE,
	CREATE_TARGET_TYPE_LOCATION,
	CREATE_TARGET_TYPE_CHARACTER,
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)	
	CREATE_TARGET_TYPE_COMBAT_ZONE = 4,
#endif
};

typedef struct
{
    BYTE        bHeader;
    long        lID;
    long        lX, lY;
} TPacketGCTargetUpdate;

typedef struct
{
    BYTE        bHeader;
    long        lID;
} TPacketGCTargetDelete;

typedef struct
{
    DWORD       dwType;
    BYTE        bPointIdxApplyOn;
    long        lApplyValue;
    DWORD       dwFlag;
    long        lDuration;
    long        lSPCost;
} TPacketAffectElement;

typedef struct 
{
    BYTE bHeader;
    TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
    BYTE bHeader;
    DWORD dwType;
    BYTE bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_mall_open
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCMallOpen;

typedef struct packet_lover_info
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE byLovePoint;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE bHeader;
	BYTE byLovePoint;
} TPacketGCLovePointUpdate;

typedef struct packet_dig_motion
{
    BYTE header;
    DWORD vid;
    DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client To Client


typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[128];
} TPacketGCSpecificEffect;

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_UPGRADE,
	DS_SUB_HEADER_DO_IMPROVEMENT,
	DS_SUB_HEADER_DO_REFINE,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
};

typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine(): header (HEADER_CG_DRAGON_SOUL_REFINE) {}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DS_REFINE_WINDOW_MAX_NUM];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine(): header (HEADER_GC_DRAGON_SOUL_REFINE) {}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_CHANGELOOK_SYSTEM
enum EChangeLookInfo
{
	CL_WINDOW_MAX_MATERIALS = 3,
	CL_CLEAN_ATTR_VALUE0 = 8,
	CL_SCROLL_VNUM = 72326,
};

enum
{
	CL_SUBHEADER_OPEN = 0,
	CL_SUBHEADER_CLOSE,
	CL_SUBHEADER_ADD,
	CL_SUBHEADER_REMOVE,
	CL_SUBHEADER_REFINE,
};

typedef struct SPacketChangeLook
{
	BYTE	header;
	BYTE	subheader;
	DWORD	dwCost;
	BYTE	bPos;
	TItemPos	tPos;
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
	mutable	bool	bMount;
#endif
} TPacketChangeLook;

typedef struct SChangeLookMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TChangeLookMaterial;
#endif

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
typedef struct packet_priv_shop_item_set
{
	BYTE header;
	DWORD vid;
	BYTE Cell;
	DWORD price;
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD price2;
#endif
	DWORD vnum;
	BYTE count;
	DWORD flags;
	DWORD anti_flags;
	long alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD  transmutation;
#endif
	char	name[CHARACTER_NAME_MAX_LEN];
} TPacketGCShopSearchItemSet;
typedef struct command_shop_search
{
	BYTE        header;
	WORD  Race;
	int   ItemCat;
	int   SubCat;
	int   MinLevel;
	int   MaxLevel;
	int   MinRefine;
	int   MaxRefine;
	unsigned int   MinGold;
	unsigned int   MaxGold;
#ifdef ENABLE_CHEQUE_SYSTEM
	unsigned int   MinCheque;
	unsigned int   MaxCheque;
#endif
	char ItemName[SHOP_TAB_NAME_MAX + 1];
} TPacketCGShopSearch;

typedef struct command_shop_search_buy
{
	BYTE        header;
	int	shopVid;
	BYTE   shopItemPos;
} TPacketCGShopSearchBuy;
#endif

#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
typedef struct packet_event_info
{
	BYTE		bHeader;
	BYTE		event_id;
	bool		isActivate;
	DWORD		event_time;
} TPacketGCEventInfo;
#endif

#ifdef ENABLE_BUFFI_SYSTEM
typedef struct TPacketSupportShamanSkill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVid;
	DWORD	dwLevel;
}TPacketGCSupportShamanSkill;
#endif

#ifdef ENABLE_GUILD_LEADER_SYSTEM
enum EGuildMemberTypes {
	GUILD_MEMBER,
	GUILD_MEMBER_LEADER,
	GUILD_MEMBER_GENERAL,
};
#endif

#ifdef ENABLE_ACCE_SYSTEM
enum ESashInfo
{
	SASH_ABSORPTION_SOCKET = 0,
	SASH_ABSORBED_SOCKET = 1,
	SASH_CLEAN_ATTR_VALUE0 = 7,
	SASH_WINDOW_MAX_MATERIALS = 2,
};

enum
{
	SASH_SUBHEADER_GC_OPEN = 0,
	SASH_SUBHEADER_GC_CLOSE,
	SASH_SUBHEADER_GC_ADDED,
	SASH_SUBHEADER_GC_REMOVED,
	SASH_SUBHEADER_CG_REFINED,
	SASH_SUBHEADER_CG_CLOSE = 0,
	SASH_SUBHEADER_CG_ADD,
	SASH_SUBHEADER_CG_REMOVE,
	SASH_SUBHEADER_CG_REFINE,
};

typedef struct SPacketSash
{
	BYTE	header;
	BYTE	subheader;
	bool	bWindow;
	DWORD	dwPrice;
	BYTE	bPos;
	TItemPos	tPos;
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TPacketSash;

typedef struct SSashMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TSashMaterial;

typedef struct SSashResult
{
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TSashResult;
#endif

#ifdef ENABLE_HEALTH_BOARD_SYSTEM
typedef struct SPacketGCHealthBoard
{
	BYTE	bHeader;
	DWORD	dwVID;
	BYTE	bHPPercent;
} TPacketGCHealthBoard;
#endif

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
typedef struct game_extend_inventory_packet
{
	BYTE		header;
	WORD		size;
	BYTE		subheader;
	int			id;
	int			id1;
	int			id2;
	short		dwExtend_inventory_max;
	short		dwExtend_inventory_stage;
} TPacketGCExtendInventory;

enum EExtendInvenTypes
{
	SET_STAGE_EXTEND_INVEN			= 0,
	NOTIFY_EXTEND_INVEN_ITEM_USE	= 1,
	SET_MAX_EXTEND_INVENTORY		= 2,
};

typedef struct command_extend_inventory
{
	BYTE	header;
	BYTE	sub_header;
} TPacketCGExtendInventory;
#endif

#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
typedef struct packet_exchange_info   // 가변 패킷
{
	BYTE        bHeader;
	WORD        wSize;
	bool        bError;
	int			iUnixTime;
} TPacketGCExchageInfo;
#endif

#ifdef ENABLE_YOUTUBER_SYSTEM
typedef struct SPacketYoutuberPanel
{
	BYTE	header;
	char	arg1[32];
	char	arg2[32];
	char	arg3[32];
} TPacketYoutuberPanel;

typedef struct SPacketYoutubeVideo
{
	BYTE	header;
	char	arg1[32];
	char	arg2[32];
} TPacketYoutubeVideo;
#endif

#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
enum
{
	HEADER_CG_CHEST_DROP_INFO = 235,
	HEADER_GC_CHEST_DROP_INFO = 236,
};

typedef struct SPacketCGChestDropInfo {
	BYTE	header;
	WORD	wInventoryCell;
} TPacketCGChestDropInfo;

typedef struct SChestDropInfoTable {
	BYTE	bPageIndex;
	BYTE	bSlotIndex;
	DWORD	dwItemVnum;
	BYTE	bItemCount;
} TChestDropInfoTable;

typedef struct SPacketGCChestDropInfo {
	BYTE	bHeader;
	WORD	wSize;
	DWORD	dwChestVnum;
} TPacketGCChestDropInfo;
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
enum
{
	FISH_EVENT_SUBHEADER_BOX_USE,
	FISH_EVENT_SUBHEADER_SHAPE_ADD,
	FISH_EVENT_SUBHEADER_GC_REWARD,
	FISH_EVENT_SUBHEADER_GC_ENABLE,
};

typedef struct SPacketGCFishEvent {
	BYTE	bHeader;
	BYTE	bSubheader;
} TPacketCGFishEvent;

typedef struct SPacketGCFishEventInfo {
	BYTE	bHeader;
	BYTE	bSubheader;
	DWORD	dwFirstArg;
	DWORD	dwSecondArg;
} TPacketGCFishEventInfo;
#endif

typedef struct SPacketGCCharacterUpdatePart
{
	BYTE bHeader;
	BYTE bPart;
	DWORD dwVID;
	DWORD dwVnum;
	DWORD dwEvoPoint;
} TPacketGCCharacterUpdatePart;

#ifdef ENABLE_GEM_SYSTEM
typedef struct command_gem_shop
{
	BYTE        header;
	BYTE		subheader;
} TPacketCGGemShop;

typedef struct command_gem_shop_open
{
	BYTE	header;
	int     nextRefreshTime;
	TGemShopItem shopItems[27];
#ifdef ENABLE_BOSS_GEM_SYSTEM
	mutable	bool	gemShopType;
#endif
} TPacketGCGemShopOpen;

typedef struct command_gem_shop_refresh
{
	BYTE	header;
	int     nextRefreshTime;
	TGemShopItem shopItems[27];
#ifdef ENABLE_BOSS_GEM_SYSTEM
	mutable	bool	gemShopType;
#endif
} TPacketGCGemShopRefresh;

enum
{
	GEM_SHOP_SUBHEADER_CG_BUY,
	GEM_SHOP_SUBHEADER_CG_ADD,
	GEM_SHOP_SUBHEADER_CG_REFRESH,
};
#endif

#ifdef ENABLE_ATTENDANCE_EVENT
enum
{
	HEADER_CG_ATTENDANCE_REWARD = 220,
	
	HEADER_GC_HIT_COUNT_INFO = 224,
	HEADER_GC_ATTENDANCE_EVENT = 225,
	HEADER_GC_ATTENDANCE_EVENT_INFO = 226,
};

typedef struct SHitCountInfo
{
	SHitCountInfo(){ dwVid = dwCount = 0;}
	SHitCountInfo(DWORD vid, DWORD count)
	{
		dwVid = vid;
		dwCount = count;
	}
	DWORD	dwVid;
	DWORD	dwCount;
} THitCountInfo;

typedef struct SRewardItem
{
	BYTE bDay;
	DWORD dwVnum;
	DWORD dwCount;
} TRewardItem;

typedef struct SPacketGCHitCountInfo {
	BYTE	bHeader;
	DWORD	dwVid;
	DWORD	dwCount;
} TPacketGCHitCountInfo;

typedef struct SPacketGCAttendanceEventInfo {
	BYTE	bHeader;
	WORD	wSize;
	bool	bIsEnable;
	BYTE	bDay;
	BYTE	bClear;
	BYTE	bReward;
} TPacketGCAttendanceEventInfo;

typedef struct SPacketGCAttendanceEvent {
	BYTE	bHeader;
	BYTE	bType;
	BYTE	bValue;
} TPacketGCAttendanceEvent;
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
enum
{
	HEADER_CG_MINI_GAME_CATCH_KING = 226,
	HEADER_GC_MINI_GAME_CATCH_KING = 238,
};

enum
{
	SUBHEADER_GC_CATCH_KING_EVENT_INFO,
	SUBHEADER_GC_CATCH_KING_START,
	SUBHEADER_GC_CATCH_KING_SET_CARD,
	SUBHEADER_GC_CATCH_KING_RESULT_FIELD,
	SUBHEADER_GC_CATCH_KING_SET_END_CARD,
	SUBHEADER_GC_CATCH_KING_REWARD
};

typedef struct SPacketCGMiniGameCatchKing {
	BYTE	bHeader;
	BYTE	bSubheader;
	BYTE	bSubArgument;
} TPacketCGMiniGameCatchKing;

typedef struct SPacketGCMiniGameCatchKing {
	BYTE	bHeader;
	WORD	wSize;
	BYTE	bSubheader;
} TPacketGCMiniGameCatchKing;

typedef struct SPacketGCCatchKingEventInfo {
	BYTE	bHeader;
	bool	bIsEnable;
} TPacketGCCatchKingEventInfo;

typedef struct SPacketGCMiniGameCatchKingResult {
	DWORD	dwPoints;
	BYTE	bRowType;
	BYTE	bCardPos;
	BYTE	bCardValue;
	bool	bKeepFieldCard;
	bool	bDestroyHandCard;
	bool	bGetReward;
	bool 	bIsFiveNearBy;
} TPacketGCMiniGameCatchKingResult;

typedef struct SPacketGCMiniGameCatchKingSetEndCard {
	BYTE	bCardPos;
	BYTE	bCardValue;
} TPacketGCMiniGameCatchKingSetEndCard;
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM
enum
{
	MAILBOX_SUB_HEADER_OPEN, //abre gui mailbox
	MAILBOX_SUB_HEADER_CLOSE, //cierra gui mailbox
	MAILBOX_SUB_HEADER_RECEIVE_INFO, // recibe informacion de mensajes a abrir el gui
	MAILBOX_SUB_HEADER_SEND_MAIL, // envia email
	MAILBOX_SUB_HEADER_CHECK_NAME, // envia nombre y lo comprueba
	MAILBOX_SUB_HEADER_CHECK_NAME_SUCCESSFUL, // el nombre es exitoso
	MAILBOX_SUB_HEADER_SEND_MAIL_SUCCESSFUL, // envio email exitoso.
	MAILBOX_SUB_HEADER_ACCEPT_MAIL, //acepta el mensaje 
	MAILBOX_SUB_HEADER_ACCEPT_ALL_MAIL, //acepta todos los mensajes
	MAILBOX_SUB_HEADER_DELETE_MAIL, //elimina el mensaje
	MAILBOX_SUB_HEADER_DELETE_ALL_MAIL, //elimina todos los mensajes
	MAILBOX_SUB_HEADER_CHECK_ACCEPT_SUCCESSFUL, //mensaje aceptado con exito.
	MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL, //mensaje eliminado con exito.
	MAILBOX_SUB_HEADER_LOADING_ACCEPT, //carga la lista de mensajes aceptados
	MAILBOX_SUB_HEADER_LOADING_DELETE, // carga la lista de mensajes eliminados
	MAILBOX_SUB_HEADER_VIEW_MAIL, //envia que vio el mensaje
	MAILBOX_SUB_HEADER_CHECK_VIEW_SUCCESSFUL,  //se ve el mensaje con exito
};

//Packet mailbox que envia informacion
typedef struct packet_send_mail_box
{
	BYTE header;
	BYTE subheader;
	int id_mail;
	char nombre_pj[12];
	char asunto[25];
	char descrip[100];
	int slot_item;
	int yang;
#ifdef USE_WON_SYSTEM_MAILBOX
	int won;
#endif
} TPacketCGMailBoxSend;

//Packet mailbox que recibe informacion
typedef struct SPacketGCMailBoxReceive
{
	SPacketGCMailBoxReceive() : header(HEADER_GC_MAILBOX_RECEIVE)
	{}
	BYTE header;
	BYTE bSubType;
	int id_mail;
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
	DWORD time;
	int check_view;
	//Values Extras
	long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

} TPacketGCMailBoxReceive;
#endif

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
enum
{
	SAFEBOX_MONEY_STATE_SAVE = 0,
	SAFEBOX_MONEY_STATE_WITHDRAW = 1,
};

typedef struct command_safebox_money
{
	BYTE bHeader;
	BYTE bState;
	long long llMoney;
} TPacketCGSafeboxMoney;

enum
{
	SAFEBOX_CHEQUE_STATE_SAVE = 0,
	SAFEBOX_CHEQUE_STATE_WITHDRAW = 1,
};

typedef struct command_safebox_cheque
{
	BYTE bHeader;
	BYTE bState;
	WORD wCheque;
} TPacketCGSafeboxCheque;

typedef struct packet_safebox_money_change
{
	BYTE bHeader;
	long long llMoney;
	WORD wCheque;
} TPacketGCSafeboxMoneyChange;
#endif


#ifdef _IMPROVED_PACKET_ENCRYPTION_
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};
#endif // _IMPROVED_PACKET_ENCRYPTION_

#ifdef ENABLE_AURA_SYSTEM
enum EAuraInfo
{
	AURA_ABSORPTION_SOCKET = 0,
	AURA_ABSORBED_SOCKET = 1,
	AURA_LEVEL_SOCKET = 2,
	AURA_WINDOW_MAX_MATERIALS = 2,
	AURA_ICE_RUNIC = 49990,
};

enum
{
	AURA_SUBHEADER_GC_OPEN = 0,
	AURA_SUBHEADER_GC_CLOSE,
	AURA_SUBHEADER_GC_ADDED,
	AURA_SUBHEADER_GC_REMOVED,
	AURA_SUBHEADER_GC_REFINED,
	AURA_SUBHEADER_CG_CLOSE = 0,
	AURA_SUBHEADER_CG_ADD,
	AURA_SUBHEADER_CG_REMOVE,
	AURA_SUBHEADER_CG_REFINE,
};

typedef struct SPacketAura
{
	BYTE	header;
	BYTE	subheader;
	bool	bWindow;
	DWORD	dwPrice;
	BYTE	bPos;
	TItemPos	tPos;
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TPacketAura;

typedef struct SAuraMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TAuraMaterial;

typedef struct SAuraResult
{
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TAuraResult;
#endif

typedef struct SPacketGCFeatureEnable
{
	BYTE header;
	BYTE feature_index;
	bool enabled;
} TPacketGCFeatureEnable;

#ifdef ENABLE_SKILL_RENEWAL
typedef struct SPacketGCSkillBookCount
{
	BYTE header;
	BYTE readCount[5];
	BYTE leftCount[5];
} TPacketGCSkillBookCount;
#endif

#ifdef ENABLE_AVERAGE_ITEM_PRICE
typedef struct command_send_average
{
	BYTE header;
	DWORD vnum;
} TPacketCGAverageSearch;

typedef struct command_recv_average
{
	BYTE header;
	DWORD vnum;
	DWORD averagePrice, averageCheque;
} TPacketGCAverageSearch;
#endif

#ifdef ENABLE_GUILD_RANKING_SYSTEM
typedef struct packet_guildrank_system
{
	BYTE	header;
	char	guild_name[64];
	DWORD	master;
	DWORD	win;
	DWORD	loss;
} TPacketGCGuildRankSystem;
#endif

#ifdef ENABLE_BOSS_MANAGER_SYSTEM
typedef struct packet_boss_data
{
	BYTE header;
	DWORD regen_time;
	DWORD dead_time;
	BYTE channel;
	DWORD boss_vnum;
} TPacketGCBossData;
#endif

#pragma pack(pop)
