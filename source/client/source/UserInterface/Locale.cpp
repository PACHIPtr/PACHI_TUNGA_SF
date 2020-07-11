#include "StdAfx.h"
#include "Locale.h"
#include "PythonApplication.h"
#include "resource.h"
#include "../eterBase/CRC32.h"
#include "../eterpack/EterPackManager.h"
#include "../eterLocale/Japanese.h"
#include <windowsx.h>

std::string __SECURITY_KEY_STRING__ = LSS_SECURITY_KEY;

char MULTI_LOCALE_PATH[256] = "locale/tr";
char MULTI_LOCALE_NAME[256] = "tr";

int LOCALE_CODEPAGE = 1254;
void LoadConfig (const char* fileName)
{
	auto fp = fopen(fileName, "rt");
	if (fp)
	{
		char line[256];
		char name[256];
		int code;
		if (fgets(line, sizeof(line) - 1, fp))
		{
			line[sizeof(line) - 1] = '\0';
			sscanf(line, "%d %s", &code, name);

			LOCALE_CODEPAGE = code;
			strcpy(MULTI_LOCALE_NAME, name);
			sprintf(MULTI_LOCALE_PATH, "locale/%s", MULTI_LOCALE_NAME);
		}
		fclose(fp);
	}
}

unsigned GetGuildLastExp (int level)
{
	static const int GUILD_LEVEL_MAX = 20;
	static DWORD GUILD_EXP_LIST[GUILD_LEVEL_MAX + 1] =
	{
		0,
		6000UL,
		18000UL,
		36000UL,
		64000UL,
		94000UL,
		130000UL,
		172000UL,
		220000UL,
		274000UL,
		334000UL,
		400000UL,
		600000UL,
		840000UL,
		1120000UL,
		1440000UL,
		1800000UL,
		2600000UL,
		3200000UL,
		4000000UL,
		16800000UL,
	};

	if (level < 0 && level >= GUILD_LEVEL_MAX)
	{
		return 0;
	}

	return GUILD_EXP_LIST[level];
}

int GetSkillPower (unsigned level)
{
	static const unsigned SKILL_POWER_NUM = 50;
	if (level >= SKILL_POWER_NUM)
	{
		return 0;
	}

	static unsigned SKILL_POWERS[SKILL_POWER_NUM] =
	{
		0,
		5,  6,  8, 10, 12,
		14, 16, 18, 20, 22,
		24, 26, 28, 30, 32,
		34, 36, 38, 40, 50,
		52, 54, 56, 58, 60,
		63, 66, 69, 72, 82,
		85, 88, 91, 94, 98,
		102, 106, 110, 115, 125,
		125,
	};

	return SKILL_POWERS[level];
}

unsigned int GetCodePage()
{
	return LOCALE_CODEPAGE;
}

int StringCompareCI (LPCSTR szStringLeft, LPCSTR szStringRight, size_t sizeLength)
{
	return strnicmp (szStringLeft, szStringRight, sizeLength);
}

const char*	LocaleService_GetLocalePath()
{
	return MULTI_LOCALE_PATH;
}

const char*	LocaleService_GetSecurityKey()
{
	return __SECURITY_KEY_STRING__.c_str();
}