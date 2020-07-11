#pragma once

#define ishan(ch)		(((ch) & 0xE0) > 0x90)
#define isnhdigit(ch)	(!ishan(ch) && isdigit(ch))
#define isnhspace(ch)	(!ishan(ch) && isspace(ch))

extern const char* first_han(const BYTE* str);
extern int check_han(const char* str);
extern int is_hangul(const BYTE* str);
extern int under_han(const void* orig);