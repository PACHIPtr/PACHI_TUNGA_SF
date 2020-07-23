#include <math.h>
#include "ItemCSVReader.h"

using namespace std;

inline string trim_left(const string& str)
{
    string::size_type n = str.find_first_not_of(" \t\v\n\r");
    return n == string::npos ? str : str.substr(n, str.length());
}

inline string trim_right(const string& str)
{
    string::size_type n = str.find_last_not_of(" \t\v\n\r");
    return n == string::npos ? str : str.substr(0, n + 1);
}

string trim(const string& str){return trim_left(trim_right(str));}

static string* StringSplit(string strOrigin, string strTok)
{
    int     cutAt;
    int     index     = 0;
    string* strResult = new string[30];

    while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos)
    {
       if (cutAt > 0)
       {
            strResult[index++] = strOrigin.substr(0, cutAt);
       }
       strOrigin = strOrigin.substr(cutAt+1);
    }

    if(strOrigin.length() > 0)
    {
        strResult[index++] = strOrigin.substr(0, cutAt);
    }

	for( int i=0;i<index;i++)
	{
		strResult[i] = trim(strResult[i]);
	}

    return strResult;
}

int get_Item_Type_Value(string inputString)
{
	string arType[] = {"ITEM_NONE", "ITEM_WEAPON",
		"ITEM_ARMOR", "ITEM_USE", 
		"ITEM_AUTOUSE", "ITEM_MATERIAL",
		"ITEM_SPECIAL", "ITEM_TOOL", 
		"ITEM_LOTTERY", "ITEM_ELK",					//10개

		"ITEM_METIN", "ITEM_CONTAINER", 
		"ITEM_FISH", "ITEM_ROD", 
		"ITEM_RESOURCE", "ITEM_CAMPFIRE",
		"ITEM_UNIQUE", "ITEM_SKILLBOOK", 
		"ITEM_QUEST", "ITEM_POLYMORPH",				//20개

		"ITEM_TREASURE_BOX", "ITEM_TREASURE_KEY",
		"ITEM_SKILLFORGET", "ITEM_GIFTBOX", 
		"ITEM_PICK", "ITEM_HAIR", 
		"ITEM_TOTEM", "ITEM_BLEND", 
		"ITEM_COSTUME", "ITEM_DS",					//30개
	
		"ITEM_SPECIAL_DS",	"ITEM_EXTRACT",
		"ITEM_SECONDARY_COIN",						//33개

		"ITEM_RING",
		"ITEM_BELT",								//35개 (EItemTypes 값으로 치면 34)
		"ITEM_GACHA",
		"ITEM_SOUL",
		"ITEM_SPECIAL_GACHA",
	};

	int retInt = -1;
	for (int j=0;j<sizeof(arType)/sizeof(arType[0]);j++) {
		string tempString = arType[j];
		if	(inputString.find(tempString)!=string::npos && tempString.find(inputString)!=string::npos) {
			retInt =  j;
			break;
		}
	}

	return retInt;

}

int get_Item_SubType_Value(int type_value, string inputString)
{
	static string arSub1[] = {"WEAPON_SWORD", "WEAPON_DAGGER", "WEAPON_BOW", "WEAPON_TWO_HANDED", "WEAPON_BELL", "WEAPON_FAN", "WEAPON_ARROW", "WEAPON_MOUNT_SPEAR", "WEAPON_CLAW", "WEAPON_QUIVER"};
	static string arSub2[] = {"ARMOR_BODY", "ARMOR_HEAD", "ARMOR_SHIELD", "ARMOR_WRIST", "ARMOR_FOOTS", "ARMOR_NECK", "ARMOR_EAR", "ARMOR_NUM_TYPES"};
	static string arSub3[] = {"USE_POTION", "USE_TALISMAN", "USE_TUNING", "USE_MOVE", "USE_TREASURE_BOX", "USE_MONEYBAG", "USE_BAIT", "USE_ABILITY_UP", "USE_AFFECT", "USE_CREATE_STONE", "USE_SPECIAL", "USE_POTION_NODELAY", "USE_CLEAR", "USE_INVISIBILITY", "USE_DETACHMENT", "USE_BUCKET", "USE_POTION_CONTINUE", "USE_CLEAN_SOCKET", "USE_CHANGE_ATTRIBUTE", "USE_ADD_ATTRIBUTE", "USE_ADD_ACCESSORY_SOCKET", "USE_PUT_INTO_ACCESSORY_SOCKET", "USE_ADD_ATTRIBUTE2", "USE_RECIPE", "USE_CHANGE_ATTRIBUTE2", "USE_TIME_CHARGE_PER", "USE_TIME_CHARGE_FIX", "USE_PUT_INTO_BELT_SOCKET", "USE_PUT_INTO_RING_SOCKET", "USE_CHARM", "USE_CHANGE_COSTUME_ATTR", "USE_RESET_COSTUME_ATTR", "USE_PET"};
	static string arSub4[] = {"AUTOUSE_POTION", "AUTOUSE_ABILITY_UP", "AUTOUSE_BOMB", "AUTOUSE_GOLD", "AUTOUSE_MONEYBAG", "AUTOUSE_TREASURE_BOX"};
	static string arSub5[] = {"MATERIAL_LEATHER", "MATERIAL_BLOOD", "MATERIAL_ROOT", "MATERIAL_NEEDLE", "MATERIAL_JEWEL", "MATERIAL_DS_REFINE_NORMAL", "MATERIAL_DS_REFINE_BLESSED", "MATERIAL_DS_REFINE_HOLLY"};
	static string arSub6[] = {"SPECIAL_MAP", "SPECIAL_KEY", "SPECIAL_DOC", "SPECIAL_SPIRIT"};
	static string arSub7[] = {"TOOL_FISHING_ROD"};
	static string arSub8[] = {"LOTTERY_TICKET", "LOTTERY_INSTANT"};
	static string arSub10[] = {"METIN_NORMAL", "METIN_GOLD"};
	static string arSub12[] = {"FISH_ALIVE", "FISH_DEAD"};
	static string arSub14[] = {"RESOURCE_FISHBONE", "RESOURCE_WATERSTONEPIECE", "RESOURCE_WATERSTONE", "RESOURCE_BLOOD_PEARL", "RESOURCE_BLUE_PEARL", "RESOURCE_WHITE_PEARL", "RESOURCE_BUCKET", "RESOURCE_CRYSTAL", "RESOURCE_GEM", "RESOURCE_STONE", "RESOURCE_METIN", "RESOURCE_ORE", "RESOURCE_GREEN_PEARL", "RESOURCE_PINK_PEARL", "RESOURCE_GOLD_PEARL" };
	static string arSub16[] = 
	{
		"UNIQUE_NONE",
		"UNIQUE_BOOK",
		"UNIQUE_SPECIAL_RIDE",
		"UNIQUE_3",
		"UNIQUE_4",
		"UNIQUE_5",
		"UNIQUE_6",
		"UNIQUE_7",
		"UNIQUE_8",
		"UNIQUE_9",
		"USE_SPECIAL",
		"UNIQUE_11",
		"UNIQUE_12",
		"UNIQUE_13",
		"UNIQUE_14",
		"UNIQUE_15",
		"UNIQUE_16",
		"UNIQUE_17",
		"UNIQUE_18",
		"UNIQUE_19",
		"UNIQUE_20",
		"UNIQUE_21",
		"UNIQUE_22",
		"UNIQUE_23",
		"UNIQUE_24",
		"UNIQUE_25",
		"UNIQUE_26",
		"UNIQUE_27",
		"UNIQUE_28",
		"UNIQUE_29",
		"UNIQUE_30",
		"USE_CHARM",
		"UNIQUE_31",
		"UNIQUE_32",
		"USE_PET"
	};
	
	static string arSub28[] = 
	{
		"COSTUME_BODY", 
		"COSTUME_HAIR", 
		"COSTUME_WEAPON",
		"COSTUME_SASH",
		"COSTUME_MOUNT",
		"COSTUME_AURA"
	};
	static string arSub29[] = {"DS_SLOT1", "DS_SLOT2", "DS_SLOT3", "DS_SLOT4", "DS_SLOT5", "DS_SLOT6"};
	static string arSub31[] = {"EXTRACT_DRAGON_SOUL", "EXTRACT_DRAGON_HEART"};
	static string arSubSoul[] = {"RED_SOUL", "BLUE_SOUL"};
	static string* arSubType[] = 
	{
		0, 
		arSub1, 
		arSub2, 
		arSub3, 
		arSub4, 
		arSub5, 
		arSub6, 
		arSub7, 
		arSub8, 
		0, 
		arSub10, 
		0, 
		arSub12, 
		0, 
		arSub14, 
		0, 
		arSub16, 
		0,
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		arSub28, 
		arSub29, 
		arSub29, 
		arSub31, 
		0,
		0,
		0, 
		0,
		arSubSoul,
		0,
	};
	static int arNumberOfSubtype[_countof(arSubType)] = 
	{
		0, 
		sizeof(arSub1)/sizeof(arSub1[0]), 
		sizeof(arSub2)/sizeof(arSub2[0]), 
		sizeof(arSub3)/sizeof(arSub3[0]), 
		sizeof(arSub4)/sizeof(arSub4[0]), 
		sizeof(arSub5)/sizeof(arSub5[0]), 
		sizeof(arSub6)/sizeof(arSub6[0]), 
		sizeof(arSub7)/sizeof(arSub7[0]),
		sizeof(arSub8)/sizeof(arSub8[0]), 
		0, 
		sizeof(arSub10)/sizeof(arSub10[0]), 
		0, 
		sizeof(arSub12)/sizeof(arSub12[0]), 
		0, 
		sizeof(arSub14)/sizeof(arSub14[0]), 
		0, 
		sizeof(arSub16)/sizeof(arSub16[0]), 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		sizeof(arSub28)/sizeof(arSub28[0]), 
		sizeof(arSub29)/sizeof(arSub29[0]), 
		sizeof(arSub29)/sizeof(arSub29[0]), 
		sizeof(arSub31)/sizeof(arSub31[0]), 
		0, 
		0, 
		0, 
		0,
		sizeof(arSubSoul)/sizeof(arSubSoul[0]),
		0,
	};

	if (arSubType[type_value] == 0)
	{
		return 0;
	}

	int retInt = -1;
	for (int j=0;j<arNumberOfSubtype[type_value];j++)
	{
		string tempString = arSubType[type_value][j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString) == 0)
		{
			retInt =  j;
			break;
		}
	}

	return retInt;
}

int get_Item_AntiFlag_Value(string inputString) 
{
	string arAntiFlag[] = {"ANTI_FEMALE", "ANTI_MALE", "ANTI_MUSA", "ANTI_ASSASSIN", "ANTI_SURA", "ANTI_MUDANG", "ANTI_GET", "ANTI_DROP", "ANTI_SELL", "ANTI_EMPIRE_A", "ANTI_EMPIRE_B", "ANTI_EMPIRE_C", "ANTI_SAVE", "ANTI_GIVE", "ANTI_PKDROP", "ANTI_STACK", "ANTI_MYSHOP", "ANTI_SAFEBOX", "ANTI_WOLFMAN", "ANTI_MYOFFLINESHOP"};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|");
	for(int i =0;i<sizeof(arAntiFlag)/sizeof(arAntiFlag[0]);i++) {
		string tempString = arAntiFlag[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}

int get_Item_Flag_Value(string inputString) 
{
	string arFlag[] = 
	{
		"ITEM_TUNABLE"
		,"ITEM_STACKABLE"
		,"COUNT_PER_1GOLD"
		,"ITEM_SLOW_QUERY"
		,"ITEM_MAKECOUNT"
		,"ITEM_IRREMOVABLE"
		,"CONFIRM_WHEN_USE"
		,"QUEST_USE_MULTIPLE"
		,"ITEM_RARE_ABILITY"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|");
	for(int i =0;i<sizeof(arFlag)/sizeof(arFlag[0]);i++) {
		string tempString = arFlag[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}

int get_Item_WearFlag_Value(string inputString)
{
	string arWearrFlag[] = {"WEAR_BODY", "WEAR_HEAD", "WEAR_FOOTS", "WEAR_WRIST", "WEAR_WEAPON", "WEAR_NECK", "WEAR_EAR", "WEAR_SHIELD", "WEAR_UNIQUE", "WEAR_ARROW", "WEAR_HAIR", "WEAR_ABILITY", "WEAR_COSTUME_BODY", "WEAR_COSTUME_HAIR", "WEAR_COSTUME_WEAPON", "WEAR_COSTUME_MOUNT", "WEAR_CHARM", "WEAR_COSTUME_SASH"};
	
	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|");
	for(int i = 0; i<sizeof(arWearrFlag)/sizeof(arWearrFlag[0]);i++)
	{
		string tempString = arWearrFlag[i];
		for (int j = 0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0)
			{
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	
	delete []arInputString;
	return retValue;
}

int get_Item_Immune_Value(string inputString) 
{
	string arImmune[] = {
		"PARA",
		"CURSE",
		"STUN",
		"SLEEP",
		"SLOW",
		"POISON",
		"TERROR"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|");
	for(int i =0;i<sizeof(arImmune)/sizeof(arImmune[0]);i++) {
		string tempString = arImmune[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}

int get_Item_LimitType_Value(string inputString)
{
	string arLimitType[] = {
		"LIMIT_NONE", 
		"LEVEL", 
		"STR", 
		"DEX", 
		"INT", 
		"CON", 
		"REAL_TIME", 
		"REAL_TIME_FIRST_USE", 
		"TIMER_BASED_ON_WEAR",
	};

	int retInt = -1;
	for (int j=0;j<sizeof(arLimitType)/sizeof(arLimitType[0]);j++) {
		string tempString = arLimitType[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0)
		{
			retInt =  j;
			break;
		}
	}

	return retInt;
}

int get_Item_ApplyType_Value(string inputString)
{
	string arApplyType[] = { "APPLY_NONE", "APPLY_MAX_HP", "APPLY_MAX_SP", "APPLY_CON", "APPLY_INT", "APPLY_STR", "APPLY_DEX", "APPLY_ATT_SPEED", "APPLY_MOV_SPEED", "APPLY_CAST_SPEED", "APPLY_HP_REGEN", "APPLY_SP_REGEN", "APPLY_POISON_PCT", "APPLY_STUN_PCT", "APPLY_SLOW_PCT", "APPLY_CRITICAL_PCT", "APPLY_PENETRATE_PCT", "APPLY_ATTBONUS_HUMAN", "APPLY_ATTBONUS_ANIMAL", "APPLY_ATTBONUS_ORC", "APPLY_ATTBONUS_MILGYO", "APPLY_ATTBONUS_UNDEAD", "APPLY_ATTBONUS_DEVIL", "APPLY_STEAL_HP", "APPLY_STEAL_SP", "APPLY_MANA_BURN_PCT", "APPLY_DAMAGE_SP_RECOVER", "APPLY_BLOCK", "APPLY_DODGE", "APPLY_RESIST_SWORD", "APPLY_RESIST_TWOHAND", "APPLY_RESIST_DAGGER", "APPLY_RESIST_BELL", "APPLY_RESIST_FAN", "APPLY_RESIST_BOW", "APPLY_RESIST_FIRE", "APPLY_RESIST_ELEC", "APPLY_RESIST_MAGIC", "APPLY_RESIST_WIND", "APPLY_REFLECT_MELEE", "APPLY_REFLECT_CURSE", "APPLY_POISON_REDUCE", "APPLY_KILL_SP_RECOVER", "APPLY_EXP_DOUBLE_BONUS", "APPLY_GOLD_DOUBLE_BONUS", "APPLY_ITEM_DROP_BONUS", "APPLY_POTION_BONUS", "APPLY_KILL_HP_RECOVER", "APPLY_IMMUNE_STUN", "APPLY_IMMUNE_SLOW", "APPLY_IMMUNE_FALL", "APPLY_SKILL", "APPLY_BOW_DISTANCE", "APPLY_ATT_GRADE_BONUS", "APPLY_DEF_GRADE_BONUS", "APPLY_MAGIC_ATT_GRADE", "APPLY_MAGIC_DEF_GRADE", "APPLY_CURSE_PCT", "APPLY_MAX_STAMINA", "APPLY_ATTBONUS_WARRIOR", "APPLY_ATTBONUS_ASSASSIN", "APPLY_ATTBONUS_SURA", "APPLY_ATTBONUS_SHAMAN", "APPLY_ATTBONUS_MONSTER", "APPLY_MALL_ATTBONUS", "APPLY_MALL_DEFBONUS", "APPLY_MALL_EXPBONUS", "APPLY_MALL_ITEMBONUS", "APPLY_MALL_GOLDBONUS", "APPLY_MAX_HP_PCT", "APPLY_MAX_SP_PCT", "APPLY_SKILL_DAMAGE_BONUS", "APPLY_NORMAL_HIT_DAMAGE_BONUS", "APPLY_SKILL_DEFEND_BONUS", "APPLY_NORMAL_HIT_DEFEND_BONUS", "APPLY_EXTRACT_HP_PCT", "APPLY_RESIST_WARRIOR", "APPLY_RESIST_ASSASSIN", "APPLY_RESIST_SURA", "APPLY_RESIST_SHAMAN", "APPLY_ENERGY",	"APPLY_DEF_GRADE", "APPLY_COSTUME_ATTR_BONUS", "APPLY_MAGIC_ATTBONUS_PER", "APPLY_MELEE_MAGIC_ATTBONUS_PER", "APPLY_RESIST_ICE", "APPLY_RESIST_EARTH", "APPLY_RESIST_DARK", "APPLY_ANTI_CRITICAL_PCT", "APPLY_ANTI_PENETRATE_PCT", "APPLY_BLEEDING_PCT", "APPLY_BLEEDING_REDUCE", "APPLY_ATTBONUS_WOLFMAN", "APPLY_RESIST_WOLFMAN", "APPLY_RESIST_CLAW"	, "APPLY_ANTI_RESIST_MAGIC","APPLY_ATTBONUS_SWORD","APPLY_ATTBONUS_TWOHANDED","APPLY_ATTBONUS_DAGGER","APPLY_ATTBONUS_BELL","APPLY_ATTBONUS_FAN","APPLY_ATTBONUS_BOW","APPLY_ATTBONUS_ELECT", "APPLY_ATTBONUS_FIRE", "APPLY_ATTBONUS_ICE", "APPLY_ATTBONUS_WIND", "APPLY_ATTBONUS_EARTH", "APPLY_ATTBONUS_DARK","APPLY_RESIST_HUMAN", "APPLY_ATTBONUS_STONE"
	,"APPLY_ATTBONUS_DESERT", "APPLY_ATTBONUS_ZODIAC", "APPLY_ATTBONUS_RAZADOR", "APPLY_ATTBONUS_NEMERE", "APPLY_ATTBONUS_HYDRA", "APPLY_ATTBONUS_JOTUN_THRYM", "APPLY_ATTBONUS_LUSIFER", "APPLY_ATTBONUS_BERAN_SETAOU", "APPLY_ATTBONUS_AZRAEL", "APPLY_ATTBONUS_MELEY", "APPLY_MOUNT", "APPLY_ATTBONUS_MUSHROOM", "APPLY_RESIST_MUSHROOM"
	,"APPLY_ATTBONUS_WATER","APPLY_RESIST_WATER", "APPLY_ATTBONUS_BOSS"
	};


	int retInt = -1;
	for (int j=0;j<sizeof(arApplyType)/sizeof(arApplyType[0]);j++) {
		string tempString = arApplyType[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0)
		{ 
			retInt =  j;
			break;
		}
	}

	return retInt;

}

int get_Mob_Rank_Value(string inputString) 
{
	string arRank[] = {
		"PAWN", 
		"S_PAWN", 
		"KNIGHT", 
		"S_KNIGHT", 
		"BOSS", 
		"KING"
	};

	int retInt = -1;
	for (int j=0;j<sizeof(arRank)/sizeof(arRank[0]);j++) {
		string tempString = arRank[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0) 
		{
			retInt =  j;
			break;
		}
	}

	return retInt;
}

int get_Mob_Type_Value(string inputString)
{
	string arType[] = {
		"MONSTER", 
		"NPC", 
		"STONE", 
		"WARP", 
		"DOOR", 
		"BUILDING", 
		"PC", 
		"POLYMORPH_PC", 
		"HORSE", 
		"GOTO"
	};

	int retInt = -1;
	for (int j=0;j<sizeof(arType)/sizeof(arType[0]);j++) {
		string tempString = arType[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0) 
		{
			retInt =  j;
			break;
		}
	}

	return retInt;
}

int get_Mob_BattleType_Value(string inputString) 
{
	string arBattleType[] = {
		"MELEE", 
		"RANGE", 
		"MAGIC", 
		"SPECIAL", 
		"POWER", 
		"TANKER", 
		"SUPER_POWER", 
		"SUPER_TANKER"
	};

	int retInt = -1;
	for (int j=0;j<sizeof(arBattleType)/sizeof(arBattleType[0]);j++) {
		string tempString = arBattleType[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0) 
		{ 
			retInt =  j;
			break;
		}
	}

	return retInt;
}

int get_Mob_Size_Value(string inputString)
{
	string arSize[] = {
		"SAMLL", 
		"MEDIUM", 
		"BIG"
	};

	int retInt = 0;
	for (int j=0;j<sizeof(arSize)/sizeof(arSize[0]);j++) {
		string tempString = arSize[j];
		string tempInputString = trim(inputString);
		if	(tempInputString.compare(tempString)==0) 
		{
			retInt =  j + 1;
			break;
		}
	}

	return retInt;
}

int get_Mob_AIFlag_Value(string inputString)
{
	string arAIFlag[] = {
		"AGGR",
		"NOMOVE",
		"COWARD",
		"NOATTSHINSU",
		"NOATTCHUNJO",
		"NOATTJINNO",
		"ATTMOB",
		"BERSERK",
		"STONESKIN",
		"GODSPEED",
		"DEATHBLOW",
		"REVIVE"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, ",");
	for(int i =0;i<sizeof(arAIFlag)/sizeof(arAIFlag[0]);i++) {
		string tempString = arAIFlag[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}

int get_Mob_RaceFlag_Value(string inputString)
{
	string arRaceFlag[] = {
		"ANIMAL",
		"UNDEAD",
		"DEVIL",
		"HUMAN",
		"ORC",
		"MILGYO",
		"INSECT",
		"FIRE",
		"ICE",
		"DESERT",
		"TREE",
		"ATT_ELEC",
		"ATT_FIRE",
		"ATT_ICE",
		"ATT_WIND",
		"ATT_EARTH",
		"ATT_DARK",
		"ATT_CZ"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|");
	for(int i =0;i<sizeof(arRaceFlag)/sizeof(arRaceFlag[0]);i++) {
		string tempString = arRaceFlag[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}

int get_Mob_ImmuneFlag_Value(string inputString)
{
	string arImmuneFlag[] = {
		"STUN",
		"SLOW",
		"FALL",
		"CURSE",
		"POISON",
		"TERROR",
		"REFLECT"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, ",");
	for(int i =0;i<sizeof(arImmuneFlag)/sizeof(arImmuneFlag[0]);i++) {
		string tempString = arImmuneFlag[i];
		for (int j=0; j<30 ; j++)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString)==0) {
				retValue = retValue + pow((float)2,(float)i);
			}
			
			if(tempString2.compare("") == 0)
				break;
		}
	}
	delete []arInputString;

	return retValue;
}