#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include <functional>
#include <stack>
#include <set>

inline void stl_lowers(std::string& rstRet)
{
	for (size_t i = 0; i < rstRet.length(); ++i)
	{
		rstRet[i] = tolower(rstRet[i]);
	}
}

struct stringhash
{
	size_t operator() (const std::string& str) const
	{
		const unsigned char* s = (const unsigned char*)str.c_str();
		const unsigned char* end = s + str.size();
		size_t h = 0;

		while (s < end)
		{
			h *= 16777619;
			h ^= *(s++);
		}

		return h;
	}
};
