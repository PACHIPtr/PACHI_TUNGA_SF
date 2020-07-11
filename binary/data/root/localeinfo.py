import app
import constInfo
import serverInfo
import dbg

APP_GET_LOCALE_PATH					= app.GetLocalePath()
#APP_GET_LOCALE_SERVICE_NAME 		= app.GetLocaleServiceName()
	
APP_TITLE 							= 'METIN2'

BLEND_POTION_NO_TIME 				= 'BLEND_POTION_NO_TIME'
BLEND_POTION_NO_INFO 				= 'BLEND_POTION_NO_INFO'

LOGIN_FAILURE_WRONG_SOCIALID 		= 'LOGIN_FAILURE_WRONG_SOCIALID'
LOGIN_FAILURE_SHUTDOWN_TIME 		= 'LOGIN_FAILURE_SHUTDOWN_TIME'

GUILD_MEMBER_COUNT_INFINITY 		= 'INFINITY'
GUILD_MARK_MIN_LEVEL 				= '3'
GUILD_BUILDING_LIST_TXT 			= '{:s}/GuildBuildingList.txt'.format(APP_GET_LOCALE_PATH)
FN_GM_MARK 							= '{:s}/effect/gm.mse'.format(APP_GET_LOCALE_PATH)
if app.ENABLE_KING_GUILD_SYSTEM:
	FN_GUILD_MARK					= '{:s}/effect/mg.mse'.format(APP_GET_LOCALE_PATH)

MAP_TREE2 							= 'MAP_TREE2'

ERROR_MARK_UPLOAD_NEED_RECONNECT	= 'UploadMark: Reconnect to game'
ERROR_MARK_CHECK_NEED_RECONNECT		= 'CheckMark: Reconnect to game'

VIRTUAL_KEY_ALPHABET_LOWERS			= r"[1234567890]/qwertyuiop\=asdfghjkl;`'zxcvbnm.,"
VIRTUAL_KEY_ALPHABET_UPPERS			= r"{1234567890}?QWERTYUIOP|+ASDFGHJKL:~'ZXCVBNM<>"
VIRTUAL_KEY_SYMBOLS    				= "!@#$%^&*()_+|{}:'<>?~"
VIRTUAL_KEY_NUMBERS					= "1234567890-=\[];',./`"
VIRTUAL_KEY_SYMBOLS_BR				= "!@#$%^&*()_+|{}:'<>?~áàãâéèêíìóòôõúùç"

TITLE_APPEND = [
	"Myte2 Server Files | PACHI - Tunga |",
	"Myte2 Server Files | PACHI - Tunga |",
	"Myte2 Server Files | PACHI - Tunga |",
	"Myte2 Server Files | PACHI - Tunga |",
	"Myte2 Server Files | PACHI - Tunga |",
]
APP_TITLE = TITLE_APPEND[app.GetRandom(0, len(TITLE_APPEND) - 1)]

# Load locale data by specific path
def LoadLocaleData():
	app.LoadLocaleData(app.GetLocalePath())

# Load locale_game.txt
def LoadLocaleFile(srcFileName, localeDict):
	def SNA(text):
		def f(x):
			return text
		return f

	def SA(text):
		def f(x):
			return text % x
		return f

	def SAN(text):
		def f(x):
			return text % x
		return f

	def SAA(text):
		def f(x):
			return text % x
		return f

	funcDict = {"SA": SA, "SNA": SNA, "SAA": SAA, "SAN": SAN}
	lineIndex = 1

	try:
		lines = pack_open(srcFileName, "r").readlines()
	except IOError:
		dbg.LogBox("LoadLocaleError(%(srcFileName)s)" % locals())
		app.Abort()

	for line in lines:
		if line.count("\t") == 0:
			continue

		try:
			tokens = line[:-1].split("\t")
			if len(tokens) == 2:
				localeDict[tokens[0]] = tokens[1]
			elif len(tokens) >= 3:
				type = tokens[2].strip()
				if type:
					if type in funcDict:
						localeDict[tokens[0]] = funcDict[type](tokens[1])
					else:
						localeDict[tokens[0]] = tokens[1]
				else:
					localeDict[tokens[0]] = tokens[1]
			else:
				raise RuntimeError, "Unknown TokenSize"

			lineIndex += 1
		except:
			dbg.LogBox("%s: line(%d): %s" % (srcFileName, lineIndex, line), "Error")
			raise

LoadLocaleFile("{:s}/locale_game.txt".format(APP_GET_LOCALE_PATH), locals())

# Option pvp messages
OPTION_PVPMODE_MESSAGE_DICT = {
	0: PVP_MODE_NORMAL,
	1: PVP_MODE_REVENGE,
	2: PVP_MODE_KILL,
	3: PVP_MODE_PROTECT,
	4: PVP_MODE_GUILD,
}

# Whisper messages
WHISPER_ERROR = {
	1: CANNOT_WHISPER_NOT_LOGON,
	2: CANNOT_WHISPER_DEST_REFUSE,
	3: CANNOT_WHISPER_SELF_REFUSE,
}

# Exception of graphic device.
error = dict(
	CREATE_WINDOW = GAME_INIT_ERROR_MAIN_WINDOW,
	CREATE_CURSOR = GAME_INIT_ERROR_CURSOR,
	CREATE_NETWORK = GAME_INIT_ERROR_NETWORK,
	CREATE_ITEM_PROTO = GAME_INIT_ERROR_ITEM_PROTO,
	CREATE_MOB_PROTO = GAME_INIT_ERROR_MOB_PROTO,
	CREATE_NO_DIRECTX = GAME_INIT_ERROR_DIRECTX,
	CREATE_DEVICE = GAME_INIT_ERROR_GRAPHICS_NOT_EXIST,
	CREATE_NO_APPROPRIATE_DEVICE = GAME_INIT_ERROR_GRAPHICS_BAD_PERFORMANCE,
	CREATE_FORMAT = GAME_INIT_ERROR_GRAPHICS_NOT_SUPPORT_32BIT,
	NO_ERROR = str()
)

# Job information (none, skill_group1, skill_group2)
JOBINFO_TITLE = [
	[JOB_WARRIOR0, JOB_WARRIOR1, JOB_WARRIOR2,],
	[JOB_ASSASSIN0, JOB_ASSASSIN1, JOB_ASSASSIN2,],
	[JOB_SURA0, JOB_SURA1, JOB_SURA2,],
	[JOB_SHAMAN0, JOB_SHAMAN1, JOB_SHAMAN2,],
]

#if app.ENABLE_WOLFMAN_CHARACTER:
	#JOBINFO_TITLE += [[JOB_WOLFMAN0,JOB_WOLFMAN1,JOB_WOLFMAN2,],]

# Guild war description
GUILDWAR_NORMAL_DESCLIST = 					(GUILD_WAR_USE_NORMAL_MAP, GUILD_WAR_LIMIT_30MIN, GUILD_WAR_WIN_CHECK_SCORE)
# Guild war warp description
GUILDWAR_WARP_DESCLIST = 					(GUILD_WAR_USE_BATTLE_MAP, GUILD_WAR_WIN_WIPE_OUT_GUILD, GUILD_WAR_REWARD_POTION)
# Guild war flag description
GUILDWAR_CTF_DESCLIST = 					(GUILD_WAR_USE_BATTLE_MAP, GUILD_WAR_WIN_TAKE_AWAY_FLAG1, GUILD_WAR_WIN_TAKE_AWAY_FLAG2, GUILD_WAR_REWARD_POTION)

# Mode of pvp options
MODE_NAME_LIST = 							(PVP_OPTION_NORMAL, PVP_OPTION_REVENGE, PVP_OPTION_KILL, PVP_OPTION_PROTECT,)
# Title name of alignment
TITLE_NAME_LIST = 							( PVP_LEVEL0, PVP_LEVEL1, PVP_LEVEL2, PVP_LEVEL3, PVP_LEVEL4, PVP_LEVEL5, PVP_LEVEL6, PVP_LEVEL7, PVP_LEVEL8, PVP_LEVEL9, PVP_LEVEL10, PVP_LEVEL11 )

# Horse levels
LEVEL_LIST = 								(str(), HORSE_LEVEL1, HORSE_LEVEL2, HORSE_LEVEL3)
# Horse health
HEALTH_LIST = 								(HORSE_HEALTH0, HORSE_HEALTH1, HORSE_HEALTH2, HORSE_HEALTH3)

# Use-skill messages
USE_SKILL_ERROR_TAIL_DICT = {	
	'IN_SAFE':								CANNOT_SKILL_SELF_IN_SAFE,
	'NEED_TARGET': 							CANNOT_SKILL_NEED_TARGET,
	'NEED_EMPTY_BOTTLE': 					CANNOT_SKILL_NEED_EMPTY_BOTTLE,
	'NEED_POISON_BOTTLE': 					CANNOT_SKILL_NEED_POISON_BOTTLE,
	'REMOVE_FISHING_ROD': 					CANNOT_SKILL_REMOVE_FISHING_ROD,
	'NOT_YET_LEARN': 						CANNOT_SKILL_NOT_YET_LEARN,
	'NOT_MATCHABLE_WEAPON':					CANNOT_SKILL_NOT_MATCHABLE_WEAPON,
	'WAIT_COOLTIME':						CANNOT_SKILL_WAIT_COOLTIME,
	'NOT_ENOUGH_HP':						CANNOT_SKILL_NOT_ENOUGH_HP,
	'NOT_ENOUGH_SP':						CANNOT_SKILL_NOT_ENOUGH_SP,
	'CANNOT_USE_SELF':						CANNOT_SKILL_USE_SELF,
	'ONLY_FOR_ALLIANCE': 					CANNOT_SKILL_ONLY_FOR_ALLIANCE,
	'CANNOT_ATTACK_ENEMY_IN_SAFE_AREA':		CANNOT_SKILL_DEST_IN_SAFE,
	'CANNOT_APPROACH':						CANNOT_SKILL_APPROACH,
	'CANNOT_ATTACK':						CANNOT_SKILL_ATTACK,
	'ONLY_FOR_CORPSE':						CANNOT_SKILL_ONLY_FOR_CORPSE,
	'EQUIP_FISHING_ROD':					CANNOT_SKILL_EQUIP_FISHING_ROD, 
	'NOT_HORSE_SKILL':						CANNOT_SKILL_NOT_HORSE_SKILL,
	'HAVE_TO_RIDE':							CANNOT_SKILL_HAVE_TO_RIDE,
}

# Notify messages
NOTIFY_MESSAGE = {
	'CANNOT_EQUIP_SHOP': 					CANNOT_EQUIP_IN_SHOP,
	'CANNOT_EQUIP_EXCHANGE': 				CANNOT_EQUIP_IN_EXCHANGE,
}

# Attack messages
ATTACK_ERROR_TAIL_DICT = {
	'IN_SAFE': 								CANNOT_ATTACK_SELF_IN_SAFE,
	'DEST_IN_SAFE': 						CANNOT_ATTACK_DEST_IN_SAFE,
}

# Shot messages
SHOT_ERROR_TAIL_DICT = {
	'EMPTY_ARROW': 							CANNOT_SHOOT_EMPTY_ARROW,
	'IN_SAFE':								CANNOT_SHOOT_SELF_IN_SAFE,
	'DEST_IN_SAFE':							CANNOT_SHOOT_DEST_IN_SAFE,
}

# Skill messages
USE_SKILL_ERROR_CHAT_DICT = {	
	'NEED_EMPTY_BOTTLE': 					SKILL_NEED_EMPTY_BOTTLE,
	'NEED_POISON_BOTTLE': 					SKILL_NEED_POISON_BOTTLE, 
	'ONLY_FOR_GUILD_WAR': 					SKILL_ONLY_FOR_GUILD_WAR,
}

# Shop/private-shop messages
SHOP_ERROR_DICT = {
	"NOT_ENOUGH_MONEY" 						: SHOP_NOT_ENOUGH_MONEY,
	"SOLDOUT" 								: SHOP_SOLDOUT,
	"INVENTORY_FULL" 						: SHOP_INVENTORY_FULL,
	"INVALID_POS" 							: SHOP_INVALID_POS,
	"NOT_ENOUGH_YEAR_ROUND_COIN" 			: SHOP_NOT_ENOUGH_YEAR_ROUND_COIN,
	"NOT_ENOUGH_CHEQUE" 					: SHOP_NOT_ENOUGH_CHEQUE,
	"NOT_ENOUGH_GAYA_POINT"					: SHOP_NOT_ENOUGH_GAYA_POINT,
	"NOT_ENOUGH_ZODIAC_POINT" 				: SHOP_NOT_ENOUGH_ZODIAC_POINT,
	"NOT_ENOUGH_WORSHIP_POINT" 				: SHOP_NOT_ENOUGH_WORSHIP_POINT,
	"NOT_ENOUGH_EXP" 						: SHOP_NOT_ENOUGH_EXP,
	"NOT_ENOUGH_DRAGON_COIN" 				: SHOP_NOT_ENOUGH_DRAGON_COIN,
	"NOT_ENOUGH_DRAGON_MARK" 				: SHOP_NOT_ENOUGH_DRAGON_MARK,
	"MAX_COUNT" 							: SHOP_MAX_COUNT,
	"NOT_ENOUGH_MONEY_EX"					: SHOP_NOT_ENOUGH_MONEY_EX,
	"NOT_ENOUGH_ZODIAC_ITEMS" 				: SHOP_NOT_ENOUGH_ZODIAC_ITEMS,
}

if app.ENABLE_BUY_WITH_ITEM:
	SHOP_ERROR_DICT.update({ "NOT_ENOUGH_ITEM" : SHOP_NOT_ENOUGH_ITEM, })

if (app.ENABLE_BATTLE_ZONE_SYSTEM):
	SHOP_ERROR_DICT.update({
		"NOT_ENOUGH_POINTS" : COMBAT_ZONE_SHOP_NOT_ENOUGH_BATTLE_POINT,
		"MAX_LIMIT_POINTS" : COMBAT_ZONE_SHOP_EXCEED_LIMIT_TODAY,
		"OVERFLOW_LIMIT_POINTS" : COMBAT_ZONE_SHOP_OVERFLOW_LIMIT_TODAY
		}
	)

MINIMAP_ZONE_NAME_DICT_BY_IDX = {
	0 : "",
	1 : MAP_A1,
	3 : MAP_A3,
	4 : MAP_GUILD_01,
	5 : MAP_MONKEY_DUNGEON_11,
	6 : GUILD_VILLAGE_01,
	21 : MAP_B1,
	23 : MAP_B3,
	24 : MAP_GUILD_02,
	25 : MAP_MONKEY_DUNGEON_12,
	26 : GUILD_VILLAGE_02,
	41 : MAP_C1,
	43 : MAP_C3,
	44 : MAP_GUILD_03,
	45 : MAP_MONKEY_DUNGEON_13,
	46 : GUILD_VILLAGE_03,
	61 : MAP_N_SNOWM_01,
	62 : MAP_N_FLAME_01,
	63 : MAP_N_DESERT_01,
	64 : MAP_N_THREEWAY,
	65 : MAP_MILGYO,
	66 : MAP_DEVILTOWER1,
	67 : MAP_TRENT,
	68 : MAP_TRENT02,
	69 : MAP_WL_01,
	70 : MAP_NUSLUCK01,
	71 : MAP_SPIDERDUNGEON_02,
	72 : MAP_SKIPIA_DUNGEON_01,
	73 : MAP_SKIPIA_DUNGEON_02,
	81 : MAP_WEDDING_01,
	100 : MAP_FIELD_DUNGEON,
	101 : MAP_RESOURCE_ZONE,
	103 : MAP_T1,
	104 : MAP_SPIDERDUNGEON,
	105 : MAP_T2,
	107 : MAP_MONKEY_DUNGEON,
	108 : MAP_MONKEY_DUNGEON2,
	109 : MAP_MONKEY_DUNGEON3,
	110 : MAP_T3,
	111 : MAP_T4,
	112 : MAP_DUEL,
	113 : MAP_OXEVENT,
	114 : MAP_SUNGZI,
	118 : MAP_SUNGZI_FLAME_HILL_01,
	119 : MAP_SUNGZI_FLAME_HILL_02,
	120 : MAP_SUNGZI_FLAME_HILL_03,
	121 : MAP_SUNGZI_SNOW,
	122 : MAP_SUNGZI_SNOW_PASS01,
	123 : MAP_SUNGZI_SNOW_PASS02,
	124 : MAP_SUNGZI_SNOW_PASS03,
	125 : MAP_SUNGZI_DESERT_01,
	126 : MAP_SUNGZI_DESERT_HILL_01,
	127 : MAP_SUNGZI_DESERT_HILL_02,
	128 : MAP_SUNGZI_DESERT_HILL_03,
	181 : MAP_EMPIREWAR01,
	182 : MAP_EMPIREWAR02,
	183 : MAP_EMPIREWAR03,
	200 : MAP_GM_GUILD,
	207 : MAP_SKIPIA_DUNGEON_BOSS,
	212 : MAP_N_FLAME_DRAGON,
	216 : MAP_DEVILCATACOMB,
	217 : MAP_SPIDERDUNGEON_03,
	246 : MAP_BATTLEFIELD,
	301 : MAP_CAPEDRAGONHEAD,
	302 : MAP_DAWNMISTWOOD,
	303 : MAP_BAYBLACKSAND,
	304 : MAP_MT_THUNDER,
	305 : MAP_SUNGZI_FLAME_PASS_01,
	306 : MAP_SUNGZI_FLAME_PASS_02,
	307 : MAP_SUNGZI_FLAME_PASS_03,
	351 : MAP_N_FLAME_DUNGEON_01,
	352 : MAP_N_SNOW_DUNGEON_01,
	353 : MAP_DAWNMIST_DUNGEON_01,
	354 : MAP_MT_TH_DUNGEON_01,
	355 : MAP_DAWNMIST_DUNGEON_02,
	356 : MAP_12ZI_STAGE,
}

# Character status description
STAT_MINUS_DESCRIPTION = {
	'HTH-': 								STAT_MINUS_CON,
	'INT-': 								STAT_MINUS_INT,
	'STR-': 								STAT_MINUS_STR,
	'DEX-': 								STAT_MINUS_DEX,
}

MINIMAP_ZONE_NAME_DICT = {
	"metin2_map_a1"  : MAP_A1,
	"map_a2"         : MAP_A2,
	"metin2_map_a3"  : MAP_A3,
	"metin2_map_b1"  : MAP_B1,
	"map_b2"         : MAP_B2,
	"metin2_map_b3"  : MAP_B3,
	"metin2_map_c1"  : MAP_C1,
	"map_c2"         : MAP_C2,
	"metin2_map_c3"  : MAP_C3,
	"map_n_snowm_01" : MAP_SNOW,
	"metin2_map_n_flame_01" : MAP_FLAME,
	"metin2_map_n_desert_01" : MAP_DESERT,
	"metin2_map_milgyo" : MAP_TEMPLE,
	"metin2_map_spiderdungeon" : MAP_SPIDER,
	"metin2_map_deviltower1" : MAP_SKELTOWER,
	"metin2_map_guild_01" : MAP_AG,
	"metin2_map_guild_02" : MAP_BG,
	"metin2_map_guild_03" : MAP_CG,
	"metin2_map_trent" : MAP_TREE,
	"metin2_map_trent02" : MAP_TREE2,
	"season1/metin2_map_WL_01" : MAP_WL,
	"season1/metin2_map_nusluck01" : MAP_NUSLUCK,
    "Metin2_map_CapeDragonHead" : MAP_CAPE,
    "metin2_map_Mt_Thunder" : MAP_THUNDER,
    "metin2_map_dawnmistwood" : MAP_DAWN,
    "metin2_map_BayBlackSand" :	MAP_BAY,
}

if app.ENABLE_PARTY_UPDATE:
	MINIMAP_ZONE_NAME_DICT_BY_IDX = {
		0 : "",
		1 : MAP_A1,
		3 : MAP_A3,
		4 : MAP_GUILD_01,
		5 : MAP_MONKEY_DUNGEON_11,
		6 : GUILD_VILLAGE_01,
		21 : MAP_B1,
		23 : MAP_B3,
		24 : MAP_GUILD_02,
		25 : MAP_MONKEY_DUNGEON_12,
		26 : GUILD_VILLAGE_02,
		41 : MAP_C1,
		43 : MAP_C3,
		44 : MAP_GUILD_03,
		45 : MAP_MONKEY_DUNGEON_13,
		46 : GUILD_VILLAGE_03,
		61 : MAP_N_SNOWM_01,
		62 : MAP_N_FLAME_01,
		63 : MAP_N_DESERT_01,
		64 : MAP_N_THREEWAY,
		65 : MAP_MILGYO,
		66 : MAP_DEVILTOWER1,
		67 : MAP_TRENT,
		68 : MAP_TRENT02,
		69 : MAP_WL_01,
		70 : MAP_NUSLUCK01,
		71 : MAP_SPIDERDUNGEON_02,
		72 : MAP_SKIPIA_DUNGEON_01,
		73 : MAP_SKIPIA_DUNGEON_02,
		81 : MAP_WEDDING_01,
		100 : MAP_FIELD_DUNGEON,
		103 : MAP_T1,
		104 : MAP_SPIDERDUNGEON,
		105 : MAP_T2,
		107 : MAP_MONKEY_DUNGEON,
		108 : MAP_MONKEY_DUNGEON2,
		109 : MAP_MONKEY_DUNGEON3,
		110 : MAP_T3,
		111 : MAP_T4,
		112 : MAP_DUEL,
		113 : MAP_OXEVENT,
		114 : MAP_SUNGZI,
		118 : MAP_SUNGZI_FLAME_HILL_01,
		119 : MAP_SUNGZI_FLAME_HILL_02,
		120 : MAP_SUNGZI_FLAME_HILL_03,
		121 : MAP_SUNGZI_SNOW,
		122 : MAP_SUNGZI_SNOW_PASS01,
		123 : MAP_SUNGZI_SNOW_PASS02,
		124 : MAP_SUNGZI_SNOW_PASS03,
		125 : MAP_SUNGZI_DESERT_01,
		126 : MAP_SUNGZI_DESERT_HILL_01,
		127 : MAP_SUNGZI_DESERT_HILL_02,
		128 : MAP_SUNGZI_DESERT_HILL_03,
		181 : MAP_EMPIREWAR01,
		182 : MAP_EMPIREWAR02,
		183 : MAP_EMPIREWAR03,
		200 : MAP_GM_GUILD,
		207 : MAP_SKIPIA_DUNGEON_BOSS,
		212 : MAP_N_FLAME_DRAGON,
		216 : MAP_DEVILCATACOMB,
		217 : MAP_SPIDERDUNGEON_03,
		246 : MAP_BATTLEFIELD,
		301 : MAP_CAPEDRAGONHEAD,
		302 : MAP_DAWNMISTWOOD,
		303 : MAP_BAYBLACKSAND,
		304 : MAP_MT_THUNDER,
		305 : MAP_SUNGZI_FLAME_PASS_01,
		306 : MAP_SUNGZI_FLAME_PASS_02,
		307 : MAP_SUNGZI_FLAME_PASS_03,
		351 : MAP_N_FLAME_DUNGEON_01,
		352 : MAP_N_SNOW_DUNGEON_01,
		353 : MAP_DAWNMIST_DUNGEON_01,
		354 : MAP_MT_TH_DUNGEON_01,
		355 : MAP_DAWNMIST_DUNGEON_02,
		356 : MAP_12ZI_STAGE,
	}

# Path of quest icon file
def GetLetterImageName():
	return "locale/tr/icon/scroll_close.tga"
	
def GetLetterOpenImageName():
	return "locale/tr/icon/scroll_open.tga"
	
def GetLetterCloseImageName():
	return "locale/tr/icon/scroll_close.tga"

def DO_YOU_SELL_ITEM(sellItemName, sellItemCount, sellItemPrice):
	if sellItemCount > 1 :
		return DO_YOU_SELL_ITEM2 % (sellItemName, sellItemCount, NumberToMoneyString(sellItemPrice) )
	else:
		return DO_YOU_SELL_ITEM1 % (sellItemName, NumberToMoneyString(sellItemPrice) )
		
def DO_YOU_BUY_ITEM_EX(buyItemCount, buyItemPrice, buyItemCheque) :
	if buyItemCheque > 0:
		return DO_YOU_BUY_ITEM_WITH_CHEQUE % (buyItemCount, buyItemPrice, buyItemCheque )
	else:
		return DO_YOU_BUY_ITEM_WITHOUT_CHEQUE % (buyItemCount, buyItemPrice )
		
def REFINE_FAILURE_CAN_NOT_ATTACH(attachedItemName) :
	return REFINE_FAILURE_CAN_NOT_ATTACH0 % (attachedItemName)

def REFINE_FAILURE_NO_SOCKET(attachedItemName) :
	return REFINE_FAILURE_NO_SOCKET0 % (attachedItemName)

def REFINE_FAILURE_NO_GOLD_SOCKET(attachedItemName) :
	return REFINE_FAILURE_NO_GOLD_SOCKET0 % (attachedItemName)
	
def HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, dropItemCount) :
	if dropItemCount > 1 :
		return HOW_MANY_ITEM_DO_YOU_DROP2 % (dropItemName, dropItemCount)
	else :	
		return HOW_MANY_ITEM_DO_YOU_DROP1 % (dropItemName)
		
def HOW_MANY_ITEM_DO_YOU_DROP_NEW(dropItemName, dropItemCount, sellprice) :
	if dropItemCount > 1 :
		return HOW_MANY_ITEM_DO_YOU_DROP_NEW2 % (dropItemName, dropItemCount, sellprice)
	else :
		return HOW_MANY_ITEM_DO_YOU_DROP_NEW1 % (dropItemName, sellprice)

def FISHING_NOTIFY(isFish, fishName) :
	if isFish :
		return FISHING_NOTIFY1 % ( fishName )
	else :
		return FISHING_NOTIFY2 % ( fishName )

def FISHING_SUCCESS(isFish, fishName) :
	if isFish :
		return FISHING_SUCCESS1 % (fishName)
	else :
		return FISHING_SUCCESS2 % (fishName)
		
def NumberToMoneyString(n) :
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT0)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT0) 
	
def NumberToChequeString(n) :
	if n <= 0 :
		return "0 %s" % (CHEQUE_SYSTEM_UNIT_WON)
	cheque = str(n)
	return "%s %s" % (cheque, CHEQUE_SYSTEM_UNIT_WON)
	
def NumberToString(n) :
	if n <= 0 :
		return "0"

	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ])) 

def NumberToYearRoundPointString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_YEAR_ROUND_COIN)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_YEAR_ROUND_COIN)
	
if (app.ENABLE_BATTLE_ZONE_SYSTEM):
	def NumberToCombatZoneCoinString(n) :
		if n <= 0 :
			return "0 %s" % (MONETARY_COMBAT_ZONE)
		return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_COMBAT_ZONE)

def NumberToGayaString(n):
	if n <= 0 :
		return "0 %s" % (CHEQUE_SYSTEM_UNIT_GEM)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), CHEQUE_SYSTEM_UNIT_GEM)
	
def NumberToZodiacPointString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_ZODIAC_POINT)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_ZODIAC_POINT)
	
def NumberToWorshipPointString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_WORSHIP_POINT)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_WORSHIP_POINT)
	
def NumberToExpString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_EXP)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_EXP)
	
def NumberToDragonCoinString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_DRAGON_COIN)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_DRAGON_COIN)
	
def NumberToDragonMarkString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_DRAGON_MARK)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_DRAGON_MARK)
	
def NumberToAlignmentString(n):
	if n <= 0 :
		return "%d %s" % (n, MONETARY_UNIT_ALIGN)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_ALIGN) 
	
def NumberToHealthPointString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_HEALTH_POINT)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_HEALTH_POINT) 
	
def NumberToManaPointString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_MANA_POINT)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_MANA_POINT) 
	
def NumberToSecondaryCoinString(n):
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_SECONDARY_COIN)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_SECONDARY_COIN) 

# Return the title of alignment by points.
def GetAlignmentTitleName(alignment):
	if alignment >= 250000:
		return TITLE_NAME_LIST[0]
	elif alignment >= 100000:
		return TITLE_NAME_LIST[1]
	elif alignment >= 50000:
		return TITLE_NAME_LIST[2]
	elif alignment >= 12000:
		return TITLE_NAME_LIST[3]
	elif alignment >= 8000:
		return TITLE_NAME_LIST[4]
	elif alignment >= 4000:
		return TITLE_NAME_LIST[5]
	elif alignment >= 1000:
		return TITLE_NAME_LIST[6]
	elif alignment >= 0:
		return TITLE_NAME_LIST[7]
	elif alignment > -4000:
		return TITLE_NAME_LIST[8]
	elif alignment > -8000:
		return TITLE_NAME_LIST[9]
	elif alignment > -12000:
		return TITLE_NAME_LIST[10]

	return TITLE_NAME_LIST[11]
	
# Convert seconds to Days-Hours-Minutes
def SecondToDHM(time):
	if time < 60:
		return '0' + MINUTE

	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	day = int(int((time / 60) / 60) / 24)

	text = ''
	if day > 0:
		text += str(day) + DAY
		text += ' '

	if hour > 0:
		text += str(hour) + HOUR
		text += ' '

	if minute > 0:
		text += str(minute) + MINUTE
	return text

# Convert seconds to Hours-Minutes
def SecondToHM(time):
	if time < 60:
		return '0' + MINUTE

	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60)

	text = ''
	if hour > 0:
		text += str(hour) + HOUR
		if hour > 0:
			text += ' '

	if minute > 0:
		text += str(minute) + MINUTE
	return text
	
	
# Pet
def GetNamePetTooltip(vnum):
	vnum += 55700
	if vnum == 55701:
		return PET_NAME_MONKEY
	elif vnum == 55702:
		return PET_NAME_SPIDER
	elif vnum == 55703:
		return PET_NAME_RAZADOR
	elif vnum == 55704:
		return PET_NAME_NEMERE
	elif vnum == 55705:
		return PET_NAME_MINI_DRAGON
	elif vnum == 55706:
		return PET_NAME_MINI_MELEY
	elif vnum == 55707:
		return PET_NAME_MINI_LUCIFER
	elif vnum == 55708:
		return PET_NAME_MINI_EXECUTIONER
	elif vnum == 55708:
		return PET_NAME_MINI_BASSHIDO
							
def SecondToColonTypeHMS(time):			
	if time < 60:
		return "00:00:%02d" % (time)

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60)

	return "%02d:%02d:%02d" % (hour, minute, second)

def SecondToColonTypeMS(time):

	second = int(time % 60)
	minute = int((time / 60) % 60)

	return "%02d:%02d" % (minute, second)
		
if app.ENABLE_EMOJI_IN_TEXTLINE:
	def EmojiTextLine(n) :
		return "|E%s|e" % (n)
		
def FormatTime(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	return "%02d Saat %02d Dakika %02d Saniye" % (hour, minute, second)