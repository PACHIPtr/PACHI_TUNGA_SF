/*********************************************************************
 * date        : 2007.11.16
 * file        : ani.cpp
 * author      : mhh
 * description :
 */

#define _ani_cpp_

#include "stdafx.h"
#include "char.h"
#include "item.h"
#include "ani.h"

const char* FN_race_name(int race)
{
#define FN_NAME(race)	case race: return #race
	switch (race)
	{
		FN_NAME(MAIN_RACE_WARRIOR_M);
		FN_NAME(MAIN_RACE_ASSASSIN_W);
		FN_NAME(MAIN_RACE_SURA_M);
		FN_NAME(MAIN_RACE_SHAMAN_W);
		FN_NAME(MAIN_RACE_WARRIOR_W);
		FN_NAME(MAIN_RACE_ASSASSIN_M);
		FN_NAME(MAIN_RACE_SURA_W);
		FN_NAME(MAIN_RACE_SHAMAN_M);
#ifdef ENABLE_WOLFMAN_CHARACTER
		FN_NAME(MAIN_RACE_WOLFMAN_M);
#endif
		FN_NAME(MAIN_RACE_MAX_NUM);
	}

	return "UNKNOWN";
#undef FN_NAME
}

const char* FN_weapon_type(int weapon)
{
#define FN_NAME(weapon)	case weapon: return #weapon
	switch (weapon)
	{
		FN_NAME(WEAPON_SWORD);
		FN_NAME(WEAPON_DAGGER);
		FN_NAME(WEAPON_BOW);
		FN_NAME(WEAPON_TWO_HANDED);
		FN_NAME(WEAPON_BELL);
		FN_NAME(WEAPON_FAN);
		FN_NAME(WEAPON_ARROW);
		FN_NAME(WEAPON_MOUNT_SPEAR);
#ifdef ENABLE_WOLFMAN_CHARACTER
		FN_NAME(WEAPON_CLAW);
#endif
#ifdef ENABLE_QUIVER_SYSTEM
		FN_NAME(WEAPON_QUIVER);
#endif
		FN_NAME(WEAPON_NUM_TYPES);
	}

	return "UNKNOWN";
#undef FN_NAME
}

class ANI
{
protected:
	// [종족][일반0탈것1][무기][콤보]
	DWORD m_speed[MAIN_RACE_MAX_NUM][2][WEAPON_NUM_TYPES][10];

public:
	ANI();

public:
	bool	load();
	bool	load_one_race(int race, const char* dir_name);
	DWORD	load_one_weapon(const char* dir_name, int weapon, BYTE combo, bool horse);
	DWORD	attack_speed(int race, int weapon, BYTE combo = 0, bool horse = false);

	void	print_attack_speed();
};

static class ANI s_ANI;

DWORD FN_attack_speed_from_file(const char* file)
{
	FILE* fp = fopen(file, "r");

	if (nullptr == fp)
		return 0;

	int speed = 1000;

	const char* key = "DirectInputTime";
	const char* delim = " \t\r\n";
	const char* field, *value;

	char buf[1024];

	while (fgets(buf, 1024, fp))
	{
		field = strtok(buf, delim);
		value = strtok(nullptr, delim);

		if (field && value)
		{
			if (0 == strcasecmp(field, key))
			{
				float f_speed = strtof(value, nullptr);
				speed = (int)(f_speed * 1000.0);
				break;
			}
		}
	}

	fclose(fp);
	return speed;
}

ANI::ANI()
{
	// set default value
	for (int race = 0; race < MAIN_RACE_MAX_NUM; ++race)
	{
		for (int weapon = 0; weapon < WEAPON_NUM_TYPES; ++weapon)
		{
			for (BYTE combo = 0; combo <= 9; ++combo)
			{
				m_speed[race][0][weapon][combo] = 1000;
				m_speed[race][1][weapon][combo] = 1000;
			}
		}
	}
}

bool ANI::load()
{
	const char* dir_name[MAIN_RACE_MAX_NUM] = {
		"data/pc/warrior",		// 무사(남)
		"data/pc/assassin",		// 자객(여)
		"data/pc/sura",			// 수라(남)
		"data/pc/shaman",		// 무당(여)
		"data/pc2/warrior",		// 무사(여)
		"data/pc2/assassin",	// 자객(남)
		"data/pc2/sura",		// 수라(여)
		"data/pc2/shaman",		// 무당(남)
#ifdef ENABLE_WOLFMAN_CHARACTER
		"data/pc3/wolfman"		// Wolfman(Male)
#endif
	};

	for (int race = 0; race < MAIN_RACE_MAX_NUM; ++race)
	{
		if (false == load_one_race(race, dir_name[race]))
		{
			sys_err("ANI directory = %s", dir_name[race]);
			return false;
		}
	}

	return true;
}

DWORD ANI::load_one_weapon(const char* dir_name, int weapon, BYTE combo, bool horse)
{
	char format[128];
	char filename[256];

	switch (weapon)
	{
	case WEAPON_SWORD:
		strlcpy(format, "%s/%sonehand_sword/combo_%02d.msa", sizeof(format));
		break;

	case WEAPON_DAGGER:
		strlcpy(format, "%s/%sdualhand_sword/combo_%02d.msa", sizeof(format));
		break;

	case WEAPON_BOW:
		strlcpy(format, "%s/%sbow/attack.msa", sizeof(format));
		break;

	case WEAPON_TWO_HANDED:
		strlcpy(format, "%s/%stwohand_sword/combo_%02d.msa", sizeof(format));
		break;

	case WEAPON_BELL:
		strlcpy(format, "%s/%sbell/combo_%02d.msa", sizeof(format));
		break;

	case WEAPON_FAN:
		strlcpy(format, "%s/%sfan/combo_%02d.msa", sizeof(format));
		break;

#ifdef ENABLE_WOLFMAN_CHARACTER
	case WEAPON_CLAW:
		strlcpy(format, "%s/%sclaw/combo_%02d.msa", sizeof(format));
		break;
#endif

	default:
		return 1000;
	}

	snprintf(filename, sizeof(filename), format, dir_name, horse ? "horse_" : "", combo);
	DWORD speed = FN_attack_speed_from_file(filename);

	if (speed == 0)
		return 1000;

	return speed;
}

bool ANI::load_one_race(int race, const char* dir_name)
{
	if (nullptr == dir_name || '\0' == dir_name[0])
		return false;

	for (int weapon = WEAPON_SWORD; weapon < WEAPON_NUM_TYPES; ++weapon)
	{
		if (!(weapon == WEAPON_SWORD || weapon == WEAPON_TWO_HANDED) && (race == MAIN_RACE_WARRIOR_M || race == MAIN_RACE_WARRIOR_W))
			continue;

		if (!(weapon == WEAPON_SWORD) && (race == MAIN_RACE_SURA_M || race == MAIN_RACE_SURA_W))
			continue;

		if (!(weapon == WEAPON_FAN || weapon == WEAPON_BELL) && (race == MAIN_RACE_SHAMAN_M || race == MAIN_RACE_SHAMAN_W))
			continue;

		if (!(weapon == WEAPON_SWORD || weapon == WEAPON_BOW || weapon == WEAPON_DAGGER) && (race == MAIN_RACE_ASSASSIN_M || race == MAIN_RACE_ASSASSIN_W))
			continue;

		if (!(weapon == WEAPON_CLAW || race == MAIN_RACE_WOLFMAN_M))
			continue;

		for (BYTE combo = 1; combo <= 9; ++combo)
		{
			m_speed[race][0][weapon][combo] = load_one_weapon(dir_name, weapon, combo, false);
			m_speed[race][0][weapon][0] = MIN(m_speed[race][0][weapon][0], m_speed[race][0][weapon][combo]);
			m_speed[race][1][weapon][combo] = load_one_weapon(dir_name, weapon, combo, true);
			m_speed[race][1][weapon][0] = MIN(m_speed[race][1][weapon][0], m_speed[race][1][weapon][combo]);
		}
	}

	return true;
}

DWORD ANI::attack_speed(int race, int weapon, BYTE combo, bool horse)
{
	switch (race)
	{
	case MAIN_RACE_WARRIOR_M:
	case MAIN_RACE_ASSASSIN_W:
	case MAIN_RACE_SURA_M:
	case MAIN_RACE_SHAMAN_W:
	case MAIN_RACE_WARRIOR_W:
	case MAIN_RACE_ASSASSIN_M:
	case MAIN_RACE_SURA_W:
	case MAIN_RACE_SHAMAN_M:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case MAIN_RACE_WOLFMAN_M:
#endif
		break;
	default:
		return 1000;
	}

	switch (weapon)
	{
	case WEAPON_SWORD:
	case WEAPON_DAGGER:
	case WEAPON_BOW:
	case WEAPON_TWO_HANDED:
	case WEAPON_BELL:
	case WEAPON_FAN:
	case WEAPON_ARROW:
	case WEAPON_MOUNT_SPEAR:
#ifdef ENABLE_WOLFMAN_CHARACTER
	case WEAPON_CLAW:
#endif
#ifdef ENABLE_QUIVER_SYSTEM
	case WEAPON_QUIVER:
#endif
		break;
	default:
		return 1000;
	}

	return m_speed[race][horse ? 1 : 0][weapon][combo];
}

const char* FN_race_string(int race)
{
	switch (race)
	{
	case MAIN_RACE_WARRIOR_M:
		return "WARRIOR_M";

	case MAIN_RACE_ASSASSIN_W:
		return "ASSASSIN_W";

	case MAIN_RACE_SURA_M:
		return "SURA_M";

	case MAIN_RACE_SHAMAN_W:
		return "SHAMAN_W";

	case MAIN_RACE_WARRIOR_W:
		return "WARRIOR_W";

	case MAIN_RACE_ASSASSIN_M:
		return "ASSASSIN_M";

	case MAIN_RACE_SURA_W:
		return "SURA_W";

	case MAIN_RACE_SHAMAN_M:
		return "SHAMAN_M";
#ifdef ENABLE_WOLFMAN_CHARACTER
	case MAIN_RACE_WOLFMAN_M:
		return "WOLFMAN_M";
#endif
	}

	return "UNKNOWN_RACE";
}

const char* FN_weapon_string(int weapon)
{
	switch (weapon)
	{
	case WEAPON_SWORD:
		return "SWORD";

	case WEAPON_DAGGER:
		return "DAGGER";

	case WEAPON_BOW:
		return "BOW";

	case WEAPON_TWO_HANDED:
		return "TWO_HANDED";

	case WEAPON_BELL:
		return "BELL";

	case WEAPON_FAN:
		return "FAN";

	case WEAPON_ARROW:
		return "ARROW";

	case WEAPON_MOUNT_SPEAR:
		return "WEAPON_MOUNT_SPEAR";
#ifdef ENABLE_WOLFMAN_CHARACTER
	case WEAPON_CLAW:
		return "CLAW";
#endif
#ifdef ENABLE_QUIVER_SYSTEM
	case WEAPON_QUIVER:		
		return "WEAPON_QUIVER";
#endif
	}

	return "UNKNOWN";
}

void ANI::print_attack_speed()
{
	for (int race = 0; race < MAIN_RACE_MAX_NUM; ++race)
	{
		for (int weapon = 0; weapon < WEAPON_NUM_TYPES; ++weapon)
		{
			printf("[%s][%s] = %u\n",
				FN_race_string(race),
				FN_weapon_string(weapon),
				attack_speed(race, weapon));
		}
		printf("\n");
	}
}

void ani_init()
{
	s_ANI.load();
}

DWORD ani_attack_speed(LPCHARACTER ch)
{
	DWORD speed = 1000;

	if (nullptr == ch)
		return speed;

	LPITEM item = ch->GetWear(WEAR_WEAPON);

	if (nullptr == item)
		return speed;

	if (ITEM_WEAPON != item->GetType())
		return speed;

	int race = ch->GetRaceNum();
	int weapon = item->GetSubType();

	/* 투핸디드 소드의 경우 삼연참공격과 승마시 */
	/* 오류가 많아 한손검 속도로 생각하자       */
	if (weapon == WEAPON_TWO_HANDED)
		weapon = WEAPON_SWORD;

	return s_ANI.attack_speed(race, weapon);
}

DWORD ani_combo_speed(LPCHARACTER ch, BYTE combo)
{
	LPITEM item = ch->GetWear(WEAR_WEAPON);

	if (nullptr == item || combo > 9)
		return 1000;

	return s_ANI.attack_speed(ch->GetRaceNum(), item->GetSubType(), combo, ch->IsRiding());
}

void ani_print_attack_speed()
{
	s_ANI.print_attack_speed();
}

#if 0
int main(int argc, char** argv)
{
	ani_init();
	ani_print_attack_speed();
	exit(0);
}
#endif