#pragma once
#include "Locale_inc.h"

unsigned GetCodePage();
int StringCompareCI (LPCSTR szStringLeft, LPCSTR szStringRight, size_t sizeLength);
void LoadConfig (const char* fileName);
unsigned GetGuildLastExp (int level);
int GetSkillPower (unsigned level);

const char*	LocaleService_GetLocalePath();
const char*	LocaleService_GetSecurityKey();
// END_OF_CHEONMA