#ifndef __INC_PACKET_H__
#define __INC_PACKET_H__

enum EPacketClientGameHeaders
{
	HEADER_CG_LOGIN = 1,
	HEADER_CG_ATTACK = 2,
	HEADER_CG_CHAT = 3,
	HEADER_CG_CHARACTER_CREATE = 4,
	HEADER_CG_CHARACTER_DELETE = 5,
	HEADER_CG_CHARACTER_SELECT = 6,
	HEADER_CG_MOVE = 7,
	HEADER_CG_SYNC_POSITION = 8,
	HEADER_CG_DIRECT_ENTER = 9,	/*** Only Client ***/
	HEADER_CG_ENTERGAME = 10,
	HEADER_CG_ITEM_USE = 11,
#ifdef  ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
	HEADER_CG_ITEM_DELETE = 12,
#else
	HEADER_CG_ITEM_DROP = 12,
#endif
	HEADER_CG_ITEM_MOVE = 13,
#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
	HEADER_CG_ITEM_SELL = 14,
#endif
	HEADER_CG_ITEM_PICKUP = 15,
	HEADER_CG_QUICKSLOT_ADD = 16,
	HEADER_CG_QUICKSLOT_DEL = 17,
	HEADER_CG_QUICKSLOT_SWAP = 18,
	HEADER_CG_WHISPER = 19,
#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
	HEADER_CG_ITEM_DROP2 = 20,
#endif
	HEADER_CG_ON_CLICK = 21,
	HEADER_CG_EXCHANGE = 22,
	HEADER_CG_CHARACTER_POSITION = 23,
	HEADER_CG_SCRIPT_ANSWER = 24,
	HEADER_CG_QUEST_INPUT_STRING = 25,
	HEADER_CG_QUEST_CONFIRM = 26,
	HEADER_CG_SHOP = 28,
	HEADER_CG_FLY_TARGETING = 29,
	HEADER_CG_USE_SKILL = 30,
	HEADER_CG_ADD_FLY_TARGETING = 31,
	HEADER_CG_SHOOT = 32,
	HEADER_CG_MYSHOP = 33,
	HEADER_CG_ITEM_USE_TO_ITEM = 34,
	HEADER_CG_TARGET = 35,
	HEADER_CG_TEXT = 36,	/*** Only Server ***/
	HEADER_CG_WARP = 38,
	HEADER_CG_SCRIPT_BUTTON = 39,
	HEADER_CG_MESSENGER = 40,
	HEADER_CG_MALL_CHECKOUT = 41,
	HEADER_CG_SAFEBOX_CHECKIN = 42,
	HEADER_CG_SAFEBOX_CHECKOUT = 43,
	HEADER_CG_PARTY_INVITE = 44,
	HEADER_CG_PARTY_INVITE_ANSWER = 45,
	HEADER_CG_PARTY_REMOVE = 46,
	HEADER_CG_PARTY_SET_STATE = 47,
	HEADER_CG_PARTY_USE_SKILL = 48,
	HEADER_CG_SAFEBOX_ITEM_MOVE = 49,
	HEADER_CG_PARTY_PARAMETER = 50,
	HEADER_CG_GUILD = 51,
	HEADER_CG_ANSWER_MAKE_GUILD = 52,
	HEADER_CG_FISHING = 53,
	HEADER_CG_ITEM_GIVE = 54,
	HEADER_CG_EMPIRE = 55,
	HEADER_CG_REFINE = 56,
	HEADER_CG_MARK_LOGIN = 57,
	HEADER_CG_MARK_CRCLIST = 58,
	HEADER_CG_MARK_UPLOAD = 59,
	HEADER_CG_MARK_IDXLIST = 60,
	HEADER_CG_HACK = 62,
	HEADER_CG_CHANGE_NAME = 63,
	HEADER_CG_LOGIN_KEY = 64,
	HEADER_CG_DUNGEON = 65,
	HEADER_CG_LOGIN3 = 233,	/*** Not Sorted ***/
	HEADER_CG_GUILD_SYMBOL_UPLOAD = 66,
	HEADER_CG_SYMBOL_CRC = 67,
	HEADER_CG_SCRIPT_SELECT_ITEM = 68,
	HEADER_CG_DRAGON_SOUL_REFINE = 70,
	HEADER_CG_STATE_CHECKER = 71,

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	HEADER_CG_SHOP_SEARCH = 72,
	HEADER_CG_SHOP_SEARCH_SUB = 73,
	HEADER_CG_SHOP_SEARCH_BUY = 74,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	HEADER_CG_COMBAT_ZONE_REQUEST_ACTION = 75,
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	HEADER_CG_EXTEND_INVENTORY = 76,
#endif
#ifdef ENABLE_YOUTUBER_SYSTEM
	HEADER_CG_YOUTUBE_PANEL = 77,
	HEADER_CG_YOUTUBE_VIDEO = 78,
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
	HEADER_CG_PetSetName = 88,
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_CG_TARGET_INFO_LOAD = 89,
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	HEADER_CG_CL = 90,
#endif
#ifdef ENABLE_ACCE_SYSTEM
	HEADER_CG_SASH = 91,
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
	HEADER_CG_FISH_EVENT_SEND = 92,
#endif
#ifdef ENABLE_GEM_SYSTEM
	HEADER_CG_GEM_SHOP = 110,
#endif
#ifdef ENABLE_OX_RENEWAL
	HEADER_CG_QUEST_INPUT_LONG_STRING = 111,
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
	HEADER_CG_MAILBOX_SEND = 112,
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	HEADER_CG_SAFEBOX_MONEY = 113,
	HEADER_CG_SAFEBOX_CHEQUE = 114,
#endif
#ifdef ENABLE_AURA_SYSTEM
	HEADER_CG_AURA = 115,
#endif
#ifdef ENABLE_AVERAGE_ITEM_PRICE
	HEADER_CG_AVERAGE_ITEM_PRICE = 116,
#endif
	HEADER_CG_TIME_SYNC = 252,	/* 0xfc */
	HEADER_CG_CLIENT_VERSION = 253,	/* 0xfd */
	HEADER_CG_PONG = 254,	/* 0xfe */
	HEADER_CG_HANDSHAKE = 255,	/* 0xff */
	HEADER_CG_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
};
enum EPacketGameClientHeaders
{
	HEADER_GC_TIME_SYNC = 252,	//252HANDSHAKE_OK
	HEADER_GC_PHASE = 253,	//253
	HEADER_GC_BINDUDP = 254,	//254
	HEADER_GC_HANDSHAKE = 255,	//255

	HEADER_GC_CHARACTER_ADD = 1,
	HEADER_GC_CHARACTER_DEL = 2,
	HEADER_GC_MOVE = 3,
	HEADER_GC_CHAT = 4,
	HEADER_GC_SYNC_POSITION = 5,
	HEADER_GC_LOGIN_SUCCESS = 6,
	HEADER_GC_LOGIN_FAILURE = 7,
	HEADER_GC_CHARACTER_CREATE_SUCCESS = 8,
	HEADER_GC_CHARACTER_CREATE_FAILURE = 9,
	HEADER_GC_CHARACTER_DELETE_SUCCESS = 10,
	HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID = 11,
	HEADER_GC_ATTACK = 12,
	HEADER_GC_STUN = 13,
	HEADER_GC_DEAD = 14,

	HEADER_GC_CHARACTER_POINTS = 16,
	HEADER_GC_CHARACTER_POINT_CHANGE = 17,
	HEADER_GC_CHANGE_SPEED = 18,
	HEADER_GC_CHARACTER_UPDATE = 19,
	HEADER_GC_ITEM_DEL = 20,
	HEADER_GC_ITEM_SET = 21,
	HEADER_GC_ITEM_USE = 23,
	HEADER_GC_ITEM_UPDATE = 25,
	HEADER_GC_ITEM_GROUND_ADD = 26,
	HEADER_GC_ITEM_GROUND_DEL = 27,
	HEADER_GC_QUICKSLOT_ADD = 28,
	HEADER_GC_QUICKSLOT_DEL = 29,
	HEADER_GC_QUICKSLOT_SWAP = 30,
	HEADER_GC_ITEM_OWNERSHIP = 31,
	HEADER_GC_WHISPER = 33,
	HEADER_GC_MOTION = 34,
	HEADER_GC_SHOP = 36,
	HEADER_GC_SHOP_SIGN = 37,
	HEADER_GC_DUEL_START = 38,
	HEADER_GC_PVP = 39,
	HEADER_GC_EXCHANGE = 40,
	HEADER_GC_CHARACTER_POSITION = 41,
	HEADER_GC_PING = 42,
	HEADER_GC_SCRIPT = 43,
	HEADER_GC_QUEST_CONFIRM = 44,
	HEADER_GC_OWNERSHIP = 47,
	HEADER_GC_TARGET = 48,
	HEADER_GC_WARP = 49,
	HEADER_GC_ADD_FLY_TARGETING = 50,
	HEADER_GC_CREATE_FLY = 51,
	HEADER_GC_FLY_TARGETING = 52,
	HEADER_GC_SKILL_LEVEL = 54,
	HEADER_GC_MESSENGER = 55,
	HEADER_GC_GUILD = 56,
	HEADER_GC_PARTY_INVITE = 57,
	HEADER_GC_PARTY_ADD = 58,
	HEADER_GC_PARTY_UPDATE = 59,
	HEADER_GC_PARTY_REMOVE = 60,
	HEADER_GC_QUEST_INFO = 61,
	HEADER_GC_REQUEST_MAKE_GUILD = 62,
	HEADER_GC_PARTY_PARAMETER = 63,
	HEADER_GC_SAFEBOX_SET = 64,
	HEADER_GC_SAFEBOX_DEL = 65,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD = 66,
	HEADER_GC_SAFEBOX_SIZE = 67,
	HEADER_GC_FISHING = 68,
	HEADER_GC_EMPIRE = 69,
	HEADER_GC_PARTY_LINK = 70,
	HEADER_GC_PARTY_UNLINK = 71,
	HEADER_GC_OBSERVER_ADD = 73,	/*** Only Client ***/
	HEADER_GC_OBSERVER_REMOVE = 74,	/*** Only Client ***/
	HEADER_GC_OBSERVER_MOVE = 75,	/*** Only Client ***/
	HEADER_GC_MARK_BLOCK = 76,
	HEADER_GC_MARK_DIFF_DATA = 77,	/*** Only Client ***/
	HEADER_GC_MARK_IDXLIST = 78,
	HEADER_GC_TIME = 79,
	HEADER_GC_CHANGE_NAME = 80,
	HEADER_GC_DUNGEON = 81,
	HEADER_GC_WALK_MODE = 82,
	HEADER_GC_SKILL_GROUP = 83,
	HEADER_GC_MAIN_CHARACTER = 84,
	HEADER_GC_SEPCIAL_EFFECT = 85,
	HEADER_GC_NPC_POSITION = 86,

	HEADER_GC_LOGIN_KEY = 88,
	HEADER_GC_REFINE_INFORMATION = 89,

	HEADER_GC_CHANNEL = 90,

	HEADER_GC_TARGET_UPDATE = 92,
	HEADER_GC_TARGET_DELETE = 93,
	HEADER_GC_TARGET_CREATE = 94,
	HEADER_GC_AFFECT_ADD = 95,
	HEADER_GC_AFFECT_REMOVE = 96,
	HEADER_GC_MALL_OPEN = 97,
	HEADER_GC_MALL_SET = 98,
	HEADER_GC_MALL_DEL = 99,
	HEADER_GC_LAND_LIST = 100,
	HEADER_GC_LOVER_INFO = 101,
	HEADER_GC_LOVE_POINT_UPDATE = 102,
	HEADER_GC_SYMBOL_DATA = 103,
	HEADER_GC_DIG_MOTION = 104,
	HEADER_GC_DAMAGE_INFO = 105,
	HEADER_GC_CHAR_ADDITIONAL_INFO = 106,
	HEADER_GC_MAIN_CHARACTER3_BGM = 107,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL = 108,
	HEADER_GC_AUTH_SUCCESS = 109,
	HEADER_GC_SPECIFIC_EFFECT = 112,
	HEADER_GC_DRAGON_SOUL_REFINE = 113,
	HEADER_GC_RESPOND_CHANNELSTATUS = 114,
#ifdef ENABLE_EVENT_SYSTEM
	HEADER_GC_EVENT_INFO = 115,
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	HEADER_GC_RANKING_COMBAT_ZONE = 116,
	HEADER_GC_SEND_COMBAT_ZONE = 117,
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	HEADER_GC_SUPPORT_SHAMAN_SKILL = 118,
#endif
#ifdef ENABLE_HEALTH_BOARD_SYSTEM
	HEADER_GC_HEALTH_BOARD = 119,
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
	HEADER_GC_EXTEND_INVENTORY = 120,
#endif
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
	HEADER_GC_EXCHANGE_INFO = 121,
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_GC_TARGET_INFO = 128,
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	HEADER_GC_VIEW_EQUIP = 129,
#endif
#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	HEADER_GC_SHOPSEARCH_SET = 130,
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	HEADER_GC_CL = 131,
#endif
#ifdef ENABLE_ACCE_SYSTEM
	HEADER_GC_SASH = 132,
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
	HEADER_GC_FISH_EVENT_INFO = 133,
#endif
#ifdef ENABLE_GEM_SYSTEM
	HEADER_GC_GEM_SHOP_OPEN = 135,
	HEADER_GC_GEM_SHOP_REFRESH = 136,
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
	HEADER_GC_MAILBOX_RECEIVE = 150,
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	HEADER_GC_SEPCIAL_ZODIAC_EFFECT = 151,
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
	HEADER_GC_SAFEBOX_MONEY_CHANGE = 152,
#endif

#ifdef ENABLE_AURA_SYSTEM
	HEADER_GC_AURA = 154,
#endif
#ifdef ENABLE_SKILL_RENEWAL
	HEADER_GC_SKILL_BOOK_COUNT = 156,
#endif
#ifdef ENABLE_AVERAGE_ITEM_PRICE
	HEADER_GC_AVERAGE_ITEM_PRICE = 157,
#endif
#ifdef ENABLE_GUILD_RANKING_SYSTEM
	HEADER_GC_GUILD_RANK_SYSTEM = 158,
#endif
#ifdef ENABLE_BOSS_MANAGER_SYSTEM
	HEADER_GC_BOSS_DATA	= 159,
#endif
	HEADER_GC_FEATURE_ENABLE = 155,
	HEADER_GC_CHARACTER_UPDATE2 = 138,	/*** Only Client ***/
	HEADER_GC_CHARACTER_ADD2 = 139,	/*** Only Client ***/
	HEADER_GC_SKILL_LEVEL_NEW = 141,	/*** Only Client ***/
	
	HEADER_GC_KEY_AGREEMENT_COMPLETED = 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
};
enum EPacketGameGameHeaders
{
	HEADER_GG_LOGIN 						= 1,
	HEADER_GG_LOGOUT 						= 2,
	HEADER_GG_RELAY 						= 3,
	HEADER_GG_NOTICE 						= 4,
	HEADER_GG_SHUTDOWN 						= 5,
	HEADER_GG_GUILD 						= 6,
	HEADER_GG_DISCONNECT 					= 7,
	HEADER_GG_SHOUT 						= 8,
	HEADER_GG_SETUP 						= 9,
	HEADER_GG_MESSENGER_ADD 				= 10,
	HEADER_GG_MESSENGER_REMOVE 				= 11,
	HEADER_GG_FIND_POSITION 				= 12,
	HEADER_GG_WARP_CHARACTER 				= 13,
	HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX 		= 14,
	HEADER_GG_TRANSFER 						= 15,
	HEADER_GG_XMAS_WARP_SANTA 				= 16,
	HEADER_GG_XMAS_WARP_SANTA_REPLY 		= 17,
	HEADER_GG_LOGIN_PING 					= 18,
	HEADER_GG_CHECK_CLIENT_VERSION 			= 19,
	HEADER_GG_BLOCK_CHAT 					= 20,
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	HEADER_GG_MESSENGER_BLOCK_ADD 			= 21,
	HEADER_GG_MESSENGER_BLOCK_REMOVE 		= 22,
#endif
#ifdef ENABLE_GUILD_NOTICE_SYSTEM
	HEADER_GG_GUILD_NOTICE 					= 23,
#endif
#ifdef ENABLE_RELOAD_COMMAND_ALL_CORES
	HEADER_GG_RELOAD_COMMAND 				= 24,
#endif
#ifdef ENABLE_FULL_NOTICE
	HEADER_GG_BIG_NOTICE 					= 25,
#endif
#ifdef ENABLE_OFFLINESHOP_MESSAGE_SYSTEM
	HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE		= 27,
#endif
	HEADER_GG_FEATURE_ENABLE 				= 28,
#ifdef ENABLE_BOSS_MANAGER_SYSTEM
	HEADER_GG_BOSS_DATA						= 29,
#endif
};

#pragma pack(1)
typedef struct SPacketGGSetup
{
	BYTE	bHeader;
	WORD	wPort;
	BYTE	bChannel;
} TPacketGGSetup;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
typedef struct SPacketCGCombatZoneAction
{
	BYTE	header;
	int		action;
	int		value;
} TPacketCGCombatZoneRequestAction;

typedef struct SPacketGCCombatZone
{
	int		rank;
	char	name[12 + 1];
	int		empire;
	int		points;
} TPacketGCCombatZoneRanking;

typedef struct SPacketGCCombatZoneData
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

typedef struct SPacketGGLogin
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	BYTE	bEmpire;
	long	lMapIndex;
	BYTE	bChannel;
	int		iLevel;
} TPacketGGLogin;

typedef struct SPacketGGLogout
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGLogout;

typedef struct SPacketGGRelay
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lSize;
} TPacketGGRelay;

typedef struct SPacketGGNotice
{
	BYTE	bHeader;
	long	lSize;
} TPacketGGNotice;

typedef struct SPacketGGGuildNotice
{
	BYTE	bHeader;
	DWORD	dwGuildID;
	long	lSize;
} TPacketGGGuildNotice;

typedef struct SPacketGGShutdown
{
	BYTE	bHeader;
} TPacketGGShutdown;

typedef struct SPacketGGGuild
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
} TPacketGGGuild;

enum GuildSubheaderGameGame
{
	GUILD_SUBHEADER_GG_CHAT,
	GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS,
};

typedef struct SPacketGGGuildChat
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGGuildChat;

typedef struct SPacketGGParty
{
	BYTE	header;
	BYTE	subheader;
	DWORD	pid;
	DWORD	leaderpid;
} TPacketGGParty;

typedef struct SPacketGGDisconnect
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGDisconnect;

typedef struct SPacketGGShout
{
	BYTE	bHeader;
	BYTE	bEmpire;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGShout;

typedef struct SPacketGGXmasWarpSanta
{
	BYTE	bHeader;
	BYTE	bChannel;
	long	lMapIndex;
} TPacketGGXmasWarpSanta;

typedef struct SPacketGGXmasWarpSantaReply
{
	BYTE	bHeader;
	BYTE	bChannel;
} TPacketGGXmasWarpSantaReply;

typedef struct SPacketGGMessenger
{
	BYTE        bHeader;
	char        szAccount[CHARACTER_NAME_MAX_LEN + 1];
	char        szCompanion[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGMessenger;

typedef struct SPacketGGFindPosition
{
	BYTE header;
	DWORD dwFromPID; // 저 위치로 워프하려는 사람
	DWORD dwTargetPID; // 찾는 사람
} TPacketGGFindPosition;

typedef struct SPacketGGWarpCharacter
{
	BYTE header;
	DWORD pid;
	long x;
	long y;
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	int mapIndex;
#endif
} TPacketGGWarpCharacter;

//  HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX	    = 15,

typedef struct SPacketGGGuildWarMapIndex
{
	BYTE bHeader;
	DWORD dwGuildID1;
	DWORD dwGuildID2;
	long lMapIndex;
} TPacketGGGuildWarMapIndex;

typedef struct SPacketGGTransfer
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lX, lY;
} TPacketGGTransfer;

typedef struct SPacketGGLoginPing
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGLoginPing;

typedef struct SPacketGGBlockChat
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lBlockDuration;
} TPacketGGBlockChat;

typedef struct command_text
{
	BYTE	bHeader;
} TPacketCGText;

typedef struct command_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketCGHandshake;

typedef struct command_login
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
} TPacketCGLognByKey;

typedef struct command_login3
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	DWORD	adwClientKey[4];
#ifdef ENABLE_HWID_BAN_SYSTEM
	char	cpu_id[CPU_ID_MAX_LEN + 1];
	char	hdd_model[HDD_MODEL_MAX_LEN + 1];
	char	machine_guid[MACHINE_GUID_MAX_LEN + 1];
	char	mac_addr[MAC_ADDR_MAX_LEN + 1];
	char	hdd_serial[HDD_SERIAL_MAX_LEN + 1];
#endif
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
	char	clientversion[PASSWD_MAX_LEN + 1];
	char	pin[PASSWD_MAX_LEN + 1];
#endif
#ifdef URIEL_AC_VERIFY
	unsigned long	uriel1;
	unsigned long	uriel2;
	unsigned long	uriel3;
	unsigned long	uriel4;
#endif
} TPacketCGLogin3;

#ifdef ENABLE_GROWTH_PET_SYSTEM
typedef struct packet_RequestPetName
{
	BYTE byHeader;
	char petname[13];
}TPacketCGRequestPetName;
#endif

typedef struct command_player_select
{
	BYTE	header;
	BYTE	index;
} TPacketCGPlayerSelect;

typedef struct command_player_delete
{
	BYTE	header;
	BYTE	index;
	char	private_code[8];
} TPacketCGPlayerDelete;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE	shape;
	BYTE	Con;
	BYTE	Int;
	BYTE	Str;
	BYTE	Dex;
} TPacketCGPlayerCreate;

typedef struct command_player_create_success
{
	BYTE		header;
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketGCPlayerCreateSuccess;

// 공격
typedef struct command_attack
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwVID;
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

enum EMoveFuncType
{
	FUNC_WAIT,
	FUNC_MOVE,
	FUNC_ATTACK,
	FUNC_COMBO,
	FUNC_MOB_SKILL,
	FUNC_MAX_NUM,
	FUNC_SKILL = 0x80,
};

// 이동
typedef struct command_move
{
	BYTE	bHeader;
	BYTE	bFunc;
	BYTE	bArg;
	BYTE	bRot;
	long	lX;
	long	lY;
	DWORD	dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketCGSyncPositionElement;

// 위치 동기화
typedef struct command_sync_position	// 가변 패킷
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketCGSyncPosition;

/* 채팅 (3) */
typedef struct command_chat	// 가변 패킷
{
	BYTE	header;
	WORD	size;
	BYTE	type;
} TPacketCGChat;

/* 귓속말 */
typedef struct command_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	char 	szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_entergame
{
	BYTE	header;
} TPacketCGEnterGame;

typedef struct command_item_use
{
	BYTE 	header;
	TItemPos 	Cell;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE	header;
	TItemPos	Cell;
	TItemPos	TargetCell;
} TPacketCGItemUseToItem;

#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
typedef struct command_item_drop
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
	BYTE	count;
} TPacketCGItemDrop2;
#endif

typedef struct command_item_move
{
	BYTE 	header;
	TItemPos	Cell;
	TItemPos	CellTo;
	BYTE	count;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE 	header;
	DWORD	vid;
} TPacketCGItemPickup;

typedef struct command_quickslot_add
{
	BYTE	header;
	UINT    pos;
	TQuickslot	slot;
} TPacketCGQuickslotAdd;

typedef struct command_quickslot_del
{
	BYTE	header;
	UINT    pos;
} TPacketCGQuickslotDel;

typedef struct command_quickslot_swap
{
	BYTE	header;
	UINT    pos;
	UINT	change_pos;
} TPacketCGQuickslotSwap;

enum EShopSubheaderClientGame
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop
{
	BYTE	header;
	BYTE	subheader;
} TPacketCGShop;

typedef struct command_on_click
{
	BYTE	header;
	DWORD	vid;
} TPacketCGOnClick;

enum ExchangeSubheader
{
	EXCHANGE_SUBHEADER_CG_START,	/* arg1 == vid of target character */
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ELK_ADD,	/* arg1 == amount of gold */
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,	/* arg1 == amount of cheque */
#endif
	EXCHANGE_SUBHEADER_CG_ACCEPT,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_CG_CANCEL,	/* arg1 == not used */
};

typedef struct command_exchange
{
	BYTE	header;
	BYTE	sub_header;
	DWORD	arg1;
	BYTE	arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE	header;
	BYTE	position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE	header;
	BYTE	answer;
	//char	file[32 + 1];
	//BYTE	answer[16 + 1];
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
	BYTE        header;
	unsigned int	idx;
} TPacketCGScriptButton;

typedef struct command_quest_input_string
{
	BYTE header;
	char msg[64 + 1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;

/*
 * 서버 측에서 보내는 패킷
 */
typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64 + 1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketGCHandshake;

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_LOGIN,
	PHASE_SELECT,
	PHASE_LOADING,
	PHASE_GAME,
	PHASE_DEAD,

	PHASE_CLIENT_CONNECTING,
	PHASE_DBCLIENT,
	PHASE_P2P,
	PHASE_AUTH,
};

typedef struct packet_phase
{
	BYTE	header;
	BYTE	phase;
} TPacketGCPhase;

typedef struct packet_bindudp
{
	BYTE	header;
	DWORD	addr;
	WORD	port;
} TPacketGCBindUDP;

typedef struct packet_login_success
{
	BYTE		bHeader;
	TSimplePlayer	players[PLAYER_PER_ACCOUNT];
	DWORD		guild_id[PLAYER_PER_ACCOUNT];
	char		guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN + 1];

	DWORD		handle;
	DWORD		random_key;
} TPacketGCLoginSuccess;

typedef struct packet_auth_success
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
	BYTE	bResult;
} TPacketGCAuthSuccess;

typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct packet_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

enum ECharAdd
{
	ADD_CHARACTER_STATE_DEAD = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY = (1 << 4),
};

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

typedef struct packet_add_char
{
	BYTE	header;
	DWORD	dwVID;
#if defined(WJ_SHOP_MOB_INFO)
	DWORD	dwLevel;
	DWORD	dwAIFlag;
#endif

	float	angle;
	long	x;
	long	y;
	long	z;

	BYTE	bType;
	WORD	wRaceNum;
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[2];	// 효과
} TPacketGCCharacterAdd;

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
	BYTE	bPKMode;
	DWORD	dwMountVnum;
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

typedef struct packet_update_char
{
	BYTE	header;
	DWORD	dwVID;

	WORD        awPart[CHR_EQUIPPART_NUM];
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[2];

	DWORD	dwGuildID;
#ifdef ENABLE_ALIGNMENT_SYSTEM
	long long	sAlignment;
#else
	int	sAlignment;
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
	DWORD   dwLevel;
#endif
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	DWORD	combat_zone_points;
#endif
#ifdef ENABLE_BUFFI_SYSTEM
	bool bIsSupportShaman;
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
} TPacketGCCharacterUpdate;

typedef struct packet_del_char
{
	BYTE	header;
	DWORD	id;
} TPacketGCCharacterDelete;

typedef struct packet_chat	// 가변 패킷
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	id;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper	// 가변 패킷
{
	BYTE	bHeader;
	WORD	wSize;
	BYTE	bType;
	char	szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
	BYTE	bLevel;
} TPacketGCWhisper;

typedef struct packet_main_character
{
	BYTE        header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character3_bgm
{
	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[24 + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[24 + 1];
	float	fBGMVol;
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

typedef struct packet_points
{
	BYTE	header;
	INT		points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_skill_level
{
	BYTE		bHeader;
	TPlayerSkill	skills[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct packet_point_change
{
	int		header;
	DWORD	dwVID;
	BYTE	type;
	long	amount;
	long	value;
} TPacketGCPointChange;

typedef struct packet_stun
{
	BYTE	header;
	DWORD	vid;
} TPacketGCStun;

typedef struct packet_dead
{
	BYTE	header;
	DWORD	vid;
} TPacketGCDead;

struct TPacketGCItemDelDeprecated
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	BYTE	count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

typedef struct packet_item_set
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	BYTE	count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	DWORD	flags;
	DWORD	anti_flags;
	bool	highlight;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	long	sealbind;
#endif
	bool	is_basic;
} TPacketGCItemSet;

typedef struct packet_item_del
{
	BYTE	header;
	UINT	pos;
} TPacketGCItemDel;

struct packet_item_use
{
	BYTE	header;
	TItemPos Cell;
	DWORD	ch_vid;
	DWORD	victim_vid;
	DWORD	vnum;
};

typedef struct packet_item_update
{
	BYTE	header;
	TItemPos Cell;
	BYTE	count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	long	sealbind;
#endif
	mutable bool	is_basic;
} TPacketGCItemUpdate;

typedef struct packet_item_ground_add
{
	BYTE	bHeader;
	long 	x, y, z;
	DWORD	dwVID;
	DWORD	dwVnum;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	DWORD	count;
} TPacketGCItemGroundAdd;

typedef struct packet_item_ownership
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_item_ground_del
{
	BYTE	bHeader;
	DWORD	dwVID;
} TPacketGCItemGroundDel;

struct packet_quickslot_add
{
	BYTE	header;
	UINT	pos;
	TQuickslot	slot;
};

struct packet_quickslot_del
{
	BYTE	header;
	UINT	pos;
};

struct packet_quickslot_swap
{
	BYTE	header;
	UINT	pos;
	UINT	pos_to;
};

struct packet_motion
{
	BYTE	header;
	DWORD	vid;
	DWORD	victim_vid;
	WORD	motion;
};

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
#ifdef ENABLE_EVENT_SYSTEM
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

struct packet_shop_item
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
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
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
};

typedef struct packet_shop_start
{
	DWORD   owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
#ifdef ENABLE_SHOP_SEEN
	DWORD bakanlar;
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
	BYTE			pos;
	struct packet_shop_item	item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop	// 가변 패킷
{
	BYTE		header;
	WORD	size;
	BYTE		subheader;
} TPacketGCShop;

struct packet_exchange
{
	BYTE	header;
	BYTE	sub_header;
	BYTE	is_me;
	DWORD	arg1;	// vnum
	TItemPos	arg2;	// cell
	DWORD	arg3;	// count
#ifdef WJ_TRADABLE_ICON
	TItemPos	arg4;	// srccell
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	dwTransmutation;
#endif
};

enum EPacketTradeSubHeaders
{
	EXCHANGE_SUBHEADER_GC_START,	/* arg1 == vid */
	EXCHANGE_SUBHEADER_GC_ITEM_ADD,	/* arg1 == vnum  arg2 == pos  arg3 == count */
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,
	EXCHANGE_SUBHEADER_GC_GOLD_ADD,	/* arg1 == gold */
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,	/* arg1 == cheque */
#endif
	EXCHANGE_SUBHEADER_GC_ACCEPT,	/* arg1 == accept */
	EXCHANGE_SUBHEADER_GC_END,		/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_ALREADY,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_LESS_GOLD,	/* arg1 == not used */
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,	/* arg1 == not used */
#endif
};

struct packet_position
{
	BYTE	header;
	DWORD	vid;
	BYTE	position;
};

typedef struct packet_ping
{
	BYTE	header;
} TPacketGCPing;

struct packet_script
{
	BYTE	header;
	WORD	size;
	BYTE	skin;
	WORD	src_size;
};

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	long		lX;
	long		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

// 소유권
typedef struct packet_ownership
{
	BYTE		bHeader;
	DWORD		dwOwnerVID;
	DWORD		dwVictimVID;
} TPacketGCOwnership;

// 위치 동기화
typedef struct packet_sync_position	// 가변 패킷
{
	BYTE	bHeader;
	WORD	wSize;	// 개수 = (wSize - sizeof(TPacketGCSyncPosition)) / sizeof(TPacketGCSyncPositionElement)
} TPacketGCSyncPosition;

typedef struct packet_fly
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwStartVID;
	DWORD	dwEndVID;
} TPacketGCCreateFly;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE		bHeader;
	BYTE		bType;
} TPacketCGShoot;

typedef struct packet_duel_start
{
	BYTE	header;
	WORD	wSize;	// DWORD가 몇개? 개수 = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE
};

typedef struct packet_pvp
{
	BYTE        bHeader;
	DWORD       dwVIDSrc;
	DWORD       dwVIDDst;
	BYTE        bMode;	// 0 이면 끔, 1이면 켬
} TPacketGCPVP;

typedef struct command_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVID;
} TPacketCGUseSkill;

typedef struct command_target
{
	BYTE	header;
	DWORD	dwVID;
} TPacketCGTarget;

typedef struct packet_target
{
	BYTE	header;
	DWORD	dwVID;
	BYTE	bHPPercent;
#ifdef USE_TARGET_DECIMAL_HP
	int		iMinHP;
	int		iMaxHP;
#endif
#ifdef ENABLE_TARGET_ELEMENT_SYSTEM
	BYTE	bElement;
#endif
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

typedef struct packet_warp
{
	BYTE	bHeader;
	long	lX;
	long	lY;
	long	lAddr;
	WORD	wPort;
} TPacketGCWarp;

typedef struct command_warp
{
	BYTE	bHeader;
} TPacketCGWarp;

struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
	WORD c_index;//questcategory
	BYTE flag;
};

enum EMessengerSubheaderGameClient
{
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	MESSENGER_SUBHEADER_GC_BLOCK_LIST,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGIN,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT,
	MESSENGER_SUBHEADER_GC_BLOCK_INVITE,//not used
#endif
#ifdef ENABLE_MESSENGER_TEAM_SYSTEM
	MESSENGER_SUBHEADER_GC_TEAM_LIST,
	MESSENGER_SUBHEADER_GC_TEAM_LOGIN,
	MESSENGER_SUBHEADER_GC_TEAM_LOGOUT,
#endif
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
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

typedef struct packet_messenger_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerListOnline;

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
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

#ifdef ENABLE_MESSENGER_TEAM_SYSTEM
typedef struct packet_messenger_team_list_offline
{
	BYTE connected;
	BYTE length;
} TPacketGCMessengerTeamListOffline;

typedef struct packet_messenger_team_list_online
{
	BYTE connected;
	BYTE length;
} TPacketGCMessengerTeamListOnline;
#endif

enum EMessengerSubheaderClientGame
{
#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE_BLOCK,
#endif
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE,
	MESSENGER_SUBHEADER_CG_INVITE_ANSWER,
};

typedef struct command_messenger
{
	BYTE header;
	BYTE subheader;
} TPacketCGMessenger;

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
typedef struct command_messenger_add_block_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddBlockByVID;

typedef struct command_messenger_add_block_by_name
{
	BYTE length;
} TPacketCGMessengerAddBlockByName;

typedef struct command_messenger_remove_block
{
	char login[LOGIN_MAX_LEN + 1];
} TPacketCGMessengerRemoveBlock;
#endif

typedef struct command_messenger_add_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddByVID;

typedef struct command_safebox_checkout
{
	BYTE	bHeader;
	UINT	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE	bHeader;
	UINT	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_party_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketCGPartyParameter;

typedef struct paryt_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketGCPartyParameter;

typedef struct packet_party_add
{
	BYTE	header;
	DWORD	pid;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_PARTY_UPDATE
	long	mapIdx;
	long	channel;

	packet_party_add() {
		mapIdx = 0;
		channel = 0;
	}
#endif
} TPacketGCPartyAdd;

typedef struct command_party_invite
{
	BYTE	header;
	DWORD	vid;
} TPacketCGPartyInvite;

typedef struct packet_party_invite
{
	BYTE	header;
	DWORD	leader_vid;
} TPacketGCPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE	header;
	DWORD	leader_vid;
	BYTE	accept;
} TPacketCGPartyInviteAnswer;

typedef struct packet_party_update
{
	BYTE	header;
	DWORD	pid;
	BYTE	role;
	BYTE	percent_hp;
	short	affects[7];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

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
typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE header;
	DWORD pid;
	BYTE byRole;
	BYTE flag;
} TPacketCGPartySetState;

enum EPartySkillType
{
	PARTY_SKILL_HEAL = 1,
	PARTY_SKILL_WARP = 2
};

typedef struct command_party_use_skill
{
	BYTE header;
	BYTE bySkillIndex;
	DWORD vid;
} TPacketCGPartyUseSkill;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketCGSafeboxSize;

typedef struct packet_safebox_wrong_password
{
	BYTE	bHeader;
} TPacketCGSafeboxWrongPassword;

typedef struct command_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketGCEmpire;

enum EGuildSubheaderGameClient
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
	GUILD_SUBHEADER_GC_WAR_SCORE,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
	GUILD_SUBHEADER_GC_WAR_SKILL,
};

enum GUILD_SUBHEADER_CG
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

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

typedef struct packet_guild_name_t
{
	BYTE header;
	WORD size;
	BYTE subheader;
	DWORD	guildID;
	char	guildName[GUILD_NAME_MAX_LEN];
} TPacketGCGuildName;

typedef struct packet_guild_war
{
	DWORD	dwGuildSelf;
	DWORD	dwGuildOpp;
	BYTE	bType;
	BYTE 	bWarState;
#ifdef ENABLE_IMPROVED_GUILD_WAR_SYSTEM
	int		iMaxPlayer;
	int		iMaxScore;
#endif
} TPacketGCGuildWar;

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

typedef struct command_guild
{
	BYTE header;
	BYTE subheader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN + 1];
} TPacketCGAnswerMakeGuild;

typedef struct command_guild_use_skill
{
	DWORD	dwVnum;
	DWORD	dwPID;
} TPacketCGGuildUseSkill;

// Guild Mark
typedef struct command_mark_login
{
	BYTE    header;
	DWORD   handle;
	DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE	header;
	DWORD	gid;
	BYTE	image[16 * 12 * 4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
	BYTE	header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
	BYTE	header;
	BYTE	imgIdx;
	DWORD	crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE    header;
	DWORD	bufSize;
	WORD	count;
	//뒤에 size * (WORD + WORD)만큼 데이터 붙음
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE	header;
	DWORD	bufSize;
	BYTE	imgIdx;
	DWORD	count;
	// 뒤에 64 x 48 x 픽셀크기(4바이트) = 12288만큼 데이터 붙음
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	guild_id;
} TPacketCGGuildSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE header;
	DWORD guild_id;
	DWORD crc;
	DWORD size;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

// Fishing

typedef struct command_fishing
{
	BYTE header;
	BYTE dir;
} TPacketCGFishing;

typedef struct packet_fishing
{
	BYTE header;
	BYTE subheader;
	DWORD info;
	BYTE dir;
} TPacketGCFishing;

enum FishingHeader
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
	FISHING_SUBHEADER_GC_FISH,
};

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	BYTE byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE	bHeader;
	char	szBuf[255 + 1];
} TPacketCGHack;

// SubHeader - Dungeon
enum OtherDss
{
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE bHeader;
	WORD size;
	BYTE subheader;
} TPacketGCDungeon;

typedef struct packet_dungeon_dest_position
{
	long x;
	long y;
} TPacketGCDungeonDestPosition;

typedef struct SPacketGCShopSign
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketCGMyShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	int		bCount;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	BYTE	days;
#endif
#ifdef ENABLE_SHOP_DECORATION_SYSTEM
	DWORD	model;
#endif
} TPacketCGMyShop;

typedef struct SPacketGCTime
{
	BYTE	bHeader;
	time_t	time;
} TPacketGCTime;

enum EWalkModes
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE	header;
	DWORD	vid;
	BYTE	mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE        header;
	BYTE        skill_group;
} TPacketGCChangeSkillGroup;

typedef struct SPacketCGRefine
{
	BYTE	header;
	UINT	pos;
	BYTE	type;
} TPacketCGRefine;

typedef struct SPacketGCRefineInformaion
{
	BYTE	header;
	BYTE	type;
	UINT	pos;
	DWORD	src_vnum;
	DWORD	result_vnum;
	BYTE	material_count;
	int		cost; // 소요 비용
	int		prob; // 확률
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TPacketGCRefineInformation;

struct TNPCPosition
{
	BYTE bType;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	long x;
	long y;
};

typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;

	// array of TNPCPosition
} TPacketGCNPCPosition;

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGChangeName;

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCChangeName;

typedef struct command_client_version
{
	BYTE header;
	char filename[32 + 1];
	char timestamp[32 + 1];
} TPacketCGClientVersion;

typedef struct packet_channel
{
	BYTE header;
	BYTE channel;
} TPacketGCChannel;

#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
typedef struct SEquipmentItemSet
{
	BYTE  header;
	DWORD vid;
	struct {
		DWORD	vnum;
		BYTE	count;
		long	alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		long	sealbind;
		DWORD	transmutation;
	} equips[20];
} TPacketViewEquip;
#endif

typedef struct
{
	DWORD	dwID;
	long	x, y;
	long	width, height;
	DWORD	dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE	header;
	WORD	size;
} TPacketGCLandList;

typedef struct
{
	BYTE	bHeader;
	long	lID;
	char	szName[32 + 1];
	DWORD	dwVID;
	BYTE	bType;
} TPacketGCTargetCreate;

typedef struct
{
	BYTE	bHeader;
	long	lID;
	long	lX, lY;
} TPacketGCTargetUpdate;

typedef struct
{
	BYTE	bHeader;
	long	lID;
} TPacketGCTargetDelete;

typedef struct
{
	BYTE		bHeader;
	TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
	BYTE	bHeader;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGCAffectRemove;

// SCRIPT_SELECT_ITEM
typedef struct command_script_select_item
{
	BYTE header;
	TItemPos selection;
} TPacketCGScriptSelectItem;
// END_OF_SCRIPT_SELECT_ITEM

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int damage;
} TPacketGCDamageInfo;

typedef struct packet_lover_info
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE love_point;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE header;
	BYTE love_point;
} TPacketGCLovePointUpdate;

// MINING
typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;
// END_OF_MINING

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
	DS_SUB_HEADER_DO_REFINE_GRADE,
	DS_SUB_HEADER_DO_REFINE_STEP,
	DS_SUB_HEADER_DO_REFINE_STRENGTH,
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
	SPacketCGDragonSoulRefine() : header(HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DRAGON_SOUL_REFINE_GRID_SIZE];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

#ifdef ENABLE_CHANGELOOK_SYSTEM
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
	mutable bool	bMount;
#endif
} TPacketChangeLook;
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
	long alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
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
	int32_t  shopVid;
	BYTE   shopItemPos;
} TPacketCGShopSearchBuy;
#endif

#ifdef ENABLE_EVENT_SYSTEM
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

#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
typedef struct command_item_delete
{
	BYTE 	header;
	TItemPos 	Cell;
} TPacketCGItemDelete;

typedef struct command_item_sell
{
	BYTE 		header;
	TItemPos 	Cell;
} TPacketCGItemSell;
#endif

#ifdef ENABLE_ACCE_SYSTEM
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
	BYTE	header;
	WORD	size;
	BYTE	subheader;
	int		id;
	int		id1;
	int		id2;
	short	dwExtend_inventory_max;
	short	dwExtend_inventory_stage;
} TPacketGCExtendInventory;

enum
{
	SET_STAGE_EXTEND_INVEN,
	NOTIFY_EXTEND_INVEN_ITEM_USE,
	SET_MAX_EXTEND_INVENTORY,
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

typedef struct SPacketGGReloadCommand
{
	BYTE	header;
	char	argument[10];
} TPacketGGReloadCommand;

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
	TGemShopItem shopItems[GEM_SLOTS_MAX_NUM];
#ifdef ENABLE_BOSS_GEM_SYSTEM
	mutable bool	bossGem;
#endif
} TPacketGCGemShopOpen;

typedef struct command_gem_shop_refresh
{
	BYTE	header;
	int     nextRefreshTime;
	TGemShopItem shopItems[GEM_SLOTS_MAX_NUM];
#ifdef ENABLE_BOSS_GEM_SYSTEM
	mutable	bool	bossGem;
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
	SHitCountInfo() { dwVid = dwCount = 0; }
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

typedef struct SCatchKingCard
{
	SCatchKingCard() { bIndex = 0; bIsExposed = false; }
	SCatchKingCard(BYTE index, bool isExposed)
	{
		bIndex = index;
		bIsExposed = isExposed;
	}

	BYTE bIndex;
	bool bIsExposed;
} TCatchKingCard;

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
#ifdef ENABLE_OX_RENEWAL
typedef struct command_quest_input_long_string
{
	BYTE header;
	char msg[128 + 1];
} TPacketCGQuestInputLongString;
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
	MAILBOX_SUB_HEADER_CHECK_VIEW_SUCCESSFUL, //se ve el mensaje con exito
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
	long    alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TPacketGCMailBoxReceive;
#endif

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
#endif

#ifdef ENABLE_OFFLINESHOP_MESSAGE_SYSTEM
typedef struct SPacketGGOfflineShopMessage
{
	BYTE bHeader;
	DWORD dwTargetPID;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	char szItemName[ITEM_NAME_MAX_LEN + 1];
} TPacketGGOfflineShopMessage;
#endif

typedef struct SPacketGCFeatureEnable
{
	BYTE header;
	BYTE feature_index;
	bool enabled;
} TPacketGCFeatureEnable;

typedef struct SPacketGGFeatureEnable
{
	BYTE header;
	BYTE feature_index;
	bool enabled;
} TPacketGGFeatureEnable;

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

typedef struct packet_p2p_boss_data
{
	BYTE header;
	DWORD boss_vnum;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGBossData;
#endif

#pragma pack()
#endif
