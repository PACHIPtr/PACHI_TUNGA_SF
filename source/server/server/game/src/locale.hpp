#pragma once
#define LC_TEXT(str) locale_find(str)

extern "C"
{
	void locale_init (const char* filename);
	const char* locale_find (const char* string);
};