#ifndef __INC_ETERPACK_INLINE_H__
#define __INC_ETERPACK_INLINE_H__

inline void inlinePathCreate(const char* path)
{
	char dir[64];
	const char* p, *k;
	
	p = path + 3;
	
	while (nullptr != (k = strchr(p, '/')))
	{
		memset(dir, 0, sizeof(dir));
		strncpy_s(dir, path, k - path);
		CreateDirectory(dir, nullptr);
		p = k + 1;
	}
}

inline void inlineConvertPackFilename(char* name)
{
	char * p = name;

	while (*p)
	{
		if (*p == '\\')
			*p = '/';
		else
			*p = (int) tolower(*p);
		p++;
	}
}

#endif
