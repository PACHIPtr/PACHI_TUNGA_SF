import app
import item
import net
import chrmgr
import player

shop_cost=[]
gift_items={}
MyShops=[]
SHOPNAMES_RANGE = 5000
INPUT_IGNORE = 0
IN_GAME_SHOP_ENABLE = 1
MARKED_SHOP_VID	= 0
CONSOLE_ENABLE = 0
#END
eigenmod=0
WOLF_MAN = "ENABLED"	# ENABLED/DISABLED
WOLF_WOMEN = "DISABLED"	# ENABLED/DISABLED
ME_KEY = 0

PVPMODE_ENABLE = 1
PVPMODE_TEST_ENABLE = 0
PVPMODE_ACCELKEY_ENABLE = 1
PVPMODE_ACCELKEY_DELAY = 0.5
PVPMODE_PROTECTED_LEVEL = 30
ACCOUNT_NAME = "Noname"

FOG_LEVEL0 = 3800.0
FOG_LEVEL1 = 12600.0
FOG_LEVEL2 = 20800.0
FOG_LEVEL = FOG_LEVEL0
FOG_LEVEL_LIST=[FOG_LEVEL0, FOG_LEVEL1, FOG_LEVEL2]		

MuzikKontrol = False

CAMERA_MAX_DISTANCE_SHORT = 2300.0
CAMERA_MAX_DISTANCE_LONG = 3300.0
CAMERA_MAX_DISTANCE_LIST=[CAMERA_MAX_DISTANCE_SHORT, CAMERA_MAX_DISTANCE_LONG]
CAMERA_MAX_DISTANCE = CAMERA_MAX_DISTANCE_SHORT

CHRNAME_COLOR_INDEX = 0

ENVIRONMENT_NIGHT="d:/ymir work/environment/bayblacksand.msenv"

# constant
HIGH_PRICE = 500000
MIDDLE_PRICE = 50000
ERROR_METIN_STONE = 28960
SUB2_LOADING_ENABLE = 1
EXPANDED_COMBO_ENABLE = 1
CONVERT_EMPIRE_LANGUAGE_ENABLE = 1
USE_ITEM_WEAPON_TABLE_ATTACK_BONUS = 0
ADD_DEF_BONUS_ENABLE = 1
LOGIN_COUNT_LIMIT_ENABLE = 0
PET_EVOLUTION = 0
PET_LEVEL = 0
PET_MAIN = 0
FEEDWIND = 0
SKILL_PET3 = 0
SKILL_PET2 = 0
SKILL_PET1 = 0
LASTAFFECT_POINT = 0
LASTAFFECT_VALUE = 0
EVOLUTION = 0

USE_SKILL_EFFECT_UPGRADE_ENABLE = 1

VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = 1
GUILD_MONEY_PER_GSP = 100
GUILD_WAR_TYPE_SELECT_ENABLE = 1
TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE = 0

HAIR_COLOR_ENABLE = 1
ARMOR_SPECULAR_ENABLE = 1
WEAPON_SPECULAR_ENABLE = 1
SEQUENCE_PACKET_ENABLE = 0
KEEP_ACCOUNT_CONNETION_ENABLE = 1
MINIMAP_POSITIONINFO_ENABLE = 0
CONVERT_EMPIRE_LANGUAGE_ENABLE = 0
USE_ITEM_WEAPON_TABLE_ATTACK_BONUS = 0
ADD_DEF_BONUS_ENABLE = 0
LOGIN_COUNT_LIMIT_ENABLE = 0
PVPMODE_PROTECTED_LEVEL = 15
TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE = 10

isItemDropQuestionDialog = 0
isItemQuestionDialog = 0

IS_OPEN_EVENT_INFORMATION = 0

SUPPORTSHAMAN_GUI = 0

## CONFIG
CONFIG_HIDE_HEALTH_BOARD = 0
CONFIG_HIDE_SASH = 0
CONFIG_HIDE_COSTUME = 0
CONFIG_HIDE_COSTUME_W = 0
CONFIG_HIDE_COSTUME_H = 0
CONFIG_DISABLE_GUNSAM = 0
IS_GM = 0
## END OF CONFIG

SKILL_SELECT_POINTER = 0
channel_idx = 0
CLIENT_YOL = "lib/"
SERVER_TYPE = 0

##Youtuber
fav_listim = 0
youtuber_list = 0
youtuber_video_list = 0
LINK_PLUS = ""
video_izle = 0
YOUTUBER_ORNEK = "Ornek: www.youtube.com/watch?v=|cffed475cC3fqJO0hQtM"
##Youtuber

CHEST_DROP_INFO_DATA = {}

INTERFACE_MODULE = 0

KInventorymode = 0

IS_AUTO_REFINE = False
AUTO_REFINE_TYPE = 0
AUTO_REFINE_DATA = {
	"ITEM" : [-1, -1],
	"NPC" : [0, -1, -1, 0]
}

OPEN_BIO_WND=0
gerekli = 0
kalan = 0
verilen = 0
toplam = 0

BOSS_TRACKING = None
DATA_KEY_BOSS_TRACKING = 0

if app.ENABLE_MAIL_BOX_SYSTEM:
	FRIENDS_LIST_MAIL = []

SYSTEMS_WINDOW_KAPAT = 0

if app.ENABLE_DUNGEON_INFO_SYSTEM:
	INPUT_IGNORE = 0
	dungeonInfo = []
	dungeonData = {
		"quest_index" : 0,
		"quest_cmd" : "",
	}
	DUNGEON_TRACKING = None
	
GUILD_BOARD_KEY = 0

if app.ENABLE_GUILD_RANKING_SYSTEM:
	DATA_KEY_GUILD_RANK = 0
	IS_OPEN_GUILD_RANKED = 0
	
OTO_TOPLAMA_NEW = 0

def GET_ITEM_QUESTION_DIALOG_STATUS():
	global isItemQuestionDialog
	return isItemQuestionDialog

def SET_ITEM_QUESTION_DIALOG_STATUS(flag):
	global isItemQuestionDialog
	isItemQuestionDialog = flag

def GET_ITEM_DROP_QUESTION_DIALOG_STATUS():
	global isItemDropQuestionDialog
	return isItemDropQuestionDialog

def SET_ITEM_DROP_QUESTION_DIALOG_STATUS(flag):
	global isItemDropQuestionDialog
	isItemDropQuestionDialog = flag

########################

def SET_DEFAULT_FOG_LEVEL():
	global FOG_LEVEL
	app.SetMinFog(FOG_LEVEL)

def SET_FOG_LEVEL_INDEX(index):
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	try:
		FOG_LEVEL=FOG_LEVEL_LIST[index]
	except IndexError:
		FOG_LEVEL=FOG_LEVEL_LIST[0]
	app.SetMinFog(FOG_LEVEL)

def GET_FOG_LEVEL_INDEX():
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	return FOG_LEVEL_LIST.index(FOG_LEVEL)

########################

def SET_DEFAULT_CAMERA_MAX_DISTANCE():
	global CAMERA_MAX_DISTANCE
	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def SET_CAMERA_MAX_DISTANCE_INDEX(index):
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	try:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[index]
	except:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[0]

	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def GET_CAMERA_MAX_DISTANCE_INDEX():
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	return CAMERA_MAX_DISTANCE_LIST.index(CAMERA_MAX_DISTANCE)

########################

if app.ENABLE_TARGET_INFORMATION_SYSTEM:
	MONSTER_INFO_DATA = {}
	
def SET_DEFAULT_CHRNAME_COLOR():
	global CHRNAME_COLOR_INDEX
	chrmgr.SetEmpireNameMode(CHRNAME_COLOR_INDEX)

def SET_CHRNAME_COLOR_INDEX(index):
	global CHRNAME_COLOR_INDEX
	CHRNAME_COLOR_INDEX=index
	chrmgr.SetEmpireNameMode(index)

def GET_CHRNAME_COLOR_INDEX():
	global CHRNAME_COLOR_INDEX
	return CHRNAME_COLOR_INDEX

def SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(index):
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = index

def GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	return VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD

def SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE():
	global CONVERT_EMPIRE_LANGUAGE_ENABLE
	net.SetEmpireLanguageMode(CONVERT_EMPIRE_LANGUAGE_ENABLE)

def SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS():
	global USE_ITEM_WEAPON_TABLE_ATTACK_BONUS
	player.SetWeaponAttackBonusFlag(USE_ITEM_WEAPON_TABLE_ATTACK_BONUS)

def SET_DEFAULT_USE_SKILL_EFFECT_ENABLE():
	global USE_SKILL_EFFECT_UPGRADE_ENABLE
	app.SetSkillEffectUpgradeEnable(USE_SKILL_EFFECT_UPGRADE_ENABLE)

def SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE():
	global TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE
	app.SetTwoHandedWeaponAttSpeedDecreaseValue(TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE)

########################
if app.ENABLE_PERMA_BLEND_SYSTEM:
	ACCESSORY_MATERIAL_LIST = [50623, 50624, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 50634, 50635, 50636, 50637, 50638, 50639, 50641, 50642, 50643 , 50644, 50645, 50646, 50647]
	#ACCESSORY_MATERIAL_LIST = [50623, 50623, 50624, 50624, 50625, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 
	#			    50623, 50623, 50624, 50624, ]
	JewelAccessoryInfos = [
			# cevher		bileklik	kolye	küpe
			[ 50634,	14220,	16220,	17220 ],
			[ 50635,	14500,	16500,	17500 ],
			[ 50636,	14520,	16520,	17520 ],
			[ 50637,	14540,	16540,	17540 ],
			[ 50638,	14560,	16560,	17560 ],
			[ 50639,	14570,	16570,	17570 ],
			[ 50640,	14230,	16230,	17230 ],
			[ 50635,	24500,	19500,	18500 ],
			[ 50636,	24520,	19520,	18520 ],
			[ 50637,	24540,	19540,	18540 ],
			[ 50638,	24560,	19560,	18560 ],
			[ 50639,	24570,	19570,	18570 ],


		]
	JewelAccessoryInfosdict =	{
		50634:	50641, #OK
		50635:	50642, #OK
		50636:	50643, #OK
		50637:	50644, #OK
		50638:	50645, #OK
		50639:	50646, #OK
		50640:	50647, #OK
		18900:	18901, #OK
		50633:	50633,
	}
else:
	ACCESSORY_MATERIAL_LIST = [50623, 50624, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 50634, 50635, 50636, 50637, 50638, 50639]
	#ACCESSORY_MATERIAL_LIST = [50623, 50623, 50624, 50624, 50625, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 
	#			    50623, 50623, 50624, 50624, ]
	JewelAccessoryInfos = [
			# jewel		wrist	neck	ear
			[ 50634,	14420,	16220,	17220 ],	
			[ 50635,	14500,	16500,	17500 ],	
			[ 50636,	14520,	16520,	17520 ],	
			[ 50637,	14540,	16540,	17540 ],	
			[ 50638,	14560,	16560,	17560 ],	
			[ 50639,	14570,	16570,	17570 ],	
		]
def GET_ACCESSORY_MATERIAL_VNUM(vnum, subType):
	ret = vnum
	item_base = (vnum / 10) * 10
	for info in JewelAccessoryInfos:
		if item.ARMOR_WRIST == subType:	
			if info[1] == item_base:
				return info[0]
		elif item.ARMOR_NECK == subType:	
			if info[2] == item_base:
				return info[0]
		elif item.ARMOR_EAR == subType:	
			if info[3] == item_base:
				return info[0]
 
	if vnum >= 16210 and vnum <= 16219:
		return 50625

	if item.ARMOR_WRIST == subType:	
		WRIST_ITEM_VNUM_BASE = 14000
		ret -= WRIST_ITEM_VNUM_BASE
	elif item.ARMOR_NECK == subType:
		NECK_ITEM_VNUM_BASE = 16000
		ret -= NECK_ITEM_VNUM_BASE
	elif item.ARMOR_EAR == subType:
		EAR_ITEM_VNUM_BASE = 17000
		ret -= EAR_ITEM_VNUM_BASE

	type = ret/20

	if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
		type = (ret-170) / 20
		if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
			return 0

	return ACCESSORY_MATERIAL_LIST[type]

##################################################################
## »õ·Î Ãß°¡µÈ 'º§Æ®' ¾ÆÀÌÅÛ Å¸ÀÔ°ú, º§Æ®ÀÇ ¼ÒÄÏ¿¡ ²ÈÀ» ¾ÆÀÌÅÛ °ü·Ã.. 
## º§Æ®ÀÇ ¼ÒÄÏ½Ã½ºÅÛÀº ¾Ç¼¼¼­¸®¿Í µ¿ÀÏÇÏ±â ¶§¹®¿¡, À§ ¾Ç¼¼¼­¸® °ü·Ã ÇÏµåÄÚµùÃ³·³ ÀÌ·±½ÄÀ¸·Î ÇÒ ¼ö¹Û¿¡ ¾ø´Ù..

def GET_BELT_MATERIAL_VNUM(vnum, subType = 0):
	# ÇöÀç´Â ¸ğµç º§Æ®¿¡´Â ÇÏ³ªÀÇ ¾ÆÀÌÅÛ(#18900)¸¸ »ğÀÔ °¡´É
	return 18900
	
	
## ÀÚµ¿¹°¾à (HP: #72723 ~ #72726, SP: #72727 ~ #72730)

# ÇØ´ç vnumÀÌ ÀÚµ¿¹°¾àÀÎ°¡?
def IS_AUTO_POTION(itemVnum):
	return IS_AUTO_POTION_HP(itemVnum) or IS_AUTO_POTION_SP(itemVnum)
	
# ÇØ´ç vnumÀÌ HP ÀÚµ¿¹°¾àÀÎ°¡?
def IS_AUTO_POTION_HP(itemVnum):
	if 72723 <= itemVnum and 72726 >= itemVnum:
		return 1
	elif itemVnum >= 76021 and itemVnum <= 76022:		## »õ·Î µé¾î°£ ¼±¹°¿ë È­·æÀÇ Ãàº¹
		return 1
	elif itemVnum == 79012:
		return 1
		
	return 0
	
# ÇØ´ç vnumÀÌ SP ÀÚµ¿¹°¾àÀÎ°¡?
def IS_AUTO_POTION_SP(itemVnum):
	if 72727 <= itemVnum and 72730 >= itemVnum:
		return 1
	elif itemVnum >= 76004 and itemVnum <= 76005:		## »õ·Î µé¾î°£ ¼±¹°¿ë ¼ö·æÀÇ Ãàº¹
		return 1
	elif itemVnum == 79013:
		return 1
	elif itemVnum == 55701 or itemVnum == 55702 or itemVnum == 55703 or itemVnum == 55704 or itemVnum == 55705 or itemVnum == 55706:
		return 1
				
	return 0

def IS_BRAVERY_CAPE(itemVnum):
	if 70038 == itemVnum:
		return 1
	if 70057 == itemVnum:
		return 1
	if 76007 == itemVnum:
		return 1
	if 39006 == itemVnum:
		return 1
		
	return 0
	
def IS_PET_ITEM(itemVnum):
	if itemVnum >= 53001 and itemVnum <= 53302:
		return 1
		
	if itemVnum == 38200 or itemVnum == 38201:
		return 1
		
	if itemVnum >= 56004 and itemVnum <= 56010:
		return 1
		
	if itemVnum == 40497:
		return 1
		
	if itemVnum >= 56070 and itemVnum <= 56074:
		return 1
		
	return 0
	
def IS_CHARM_ITEM(itemVnum):
	if itemVnum >= 9600 and itemVnum <= 10950:
		return 1
	
	return 0
	
def IS_MOUNT_ITEM(itemVnum):
	if itemVnum >= 52000 and itemVnum <= 52220:
		return 1
	
	if itemVnum >= 71182 and itemVnum <= 71187:
		return 1
		
	if itemVnum >= 71220 and itemVnum <= 71243:
		return 1
	
	if itemVnum == 71192:
		return 1
		
	if itemVnum == 71193:
		return 1
		
	if itemVnum == 71197:
		return 1
		
	if itemVnum == 71198:
		return 1
		
	if itemVnum >= 71161 and itemVnum <= 71172:
		return 1
		
	if itemVnum >= 71137 and itemVnum <= 71142:
		return 1
		
	if itemVnum >= 71124 and itemVnum <= 71128:
		return 1
		
	if itemVnum >= 71131 and itemVnum <= 71134:
		return 1
		
	if itemVnum == 71115:
		return 1
		
	if itemVnum == 71117:
		return 1
		
	if itemVnum == 71119:
		return 1
		
	if itemVnum == 71121:
		return 1

	if itemVnum >= 56011 and itemVnum <= 56039:
		return 1
		
	return 0

def IS_SUPPORT_SHAMAN_ITEM(itemVnum):
	if itemVnum == 81001 or itemVnum == 81002 or itemVnum == 81003 or itemVnum == 81004 or itemVnum == 81005:
		return 1
		
	return 0
	
def IS_NEW_PET_ITEM(itemVnum):
	if itemVnum == 55701 or 55702 == itemVnum or 55703 == itemVnum or 55704 == itemVnum or 55705 == itemVnum or itemVnum == 55706 or itemVnum == 55707 or itemVnum == 55708 or itemVnum == 55709 or itemVnum == 55710:
		return 1
		
	return 0
	
def COUNT_SPECIFY_ITEM(itemVnum):
	finalCount = 0
	
	for i in xrange(player.INVENTORY_PAGE_SIZE*player.INVENTORY_PAGE_COUNT):
		if player.GetItemIndex(player.INVENTORY, i) == itemVnum:
			finalCount = finalCount + player.GetItemCount(player.INVENTORY, i)
			
	return finalCount
	
def IS_NEW_TYPE_POTION(itemVnum):
	if itemVnum == 18383 or itemVnum == 18384 or itemVnum == 18385 or itemVnum == 18386 or itemVnum == 18387 or itemVnum == 18388 or itemVnum == 18389 or itemVnum == 18390:
		return 1
	if itemVnum == 18391 or itemVnum == 18392 or itemVnum == 18393 or itemVnum == 18394 or itemVnum == 18395 or itemVnum == 13131:
		return 1
		
	return 0


