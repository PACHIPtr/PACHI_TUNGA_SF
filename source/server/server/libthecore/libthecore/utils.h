#pragma once

#define LOWER(c)	(((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))
#define UPPER(c)	(((c)>='a'  && (c) <= 'z') ? ((c)+('A'-'a')) : (c))

#define str_cmp strcasecmp

extern char* str_dup(const char* source);
extern void		printdata(const unsigned char* data, int bytes);
extern int		filesize(FILE * fp);

#define core_dump()	core_dump_unix(__FILE__, __LINE__)
extern void core_dump_unix(const char* who, WORD line);

#define TOKEN(string) if (!str_cmp(token_string, string))
extern void parse_token(char* src, char* token, char* value);

extern void trim_and_lower(const char* src, char* dest, size_t dest_size);

extern void lower_string(const char* src, char* dest, size_t dest_len);

extern int is_abbrev(char* arg1, char* arg2);

extern struct timeval* timediff(const struct timeval* a, const struct timeval* b);

extern struct timeval* timeadd(struct timeval* a, struct timeval* b);
extern struct tm* tm_calc(const struct tm* curr_tm, int days);

extern int MAX(int a, int b);
extern int MIN(int a, int b);
extern int MINMAX(int min, int value, int max);

extern int		number_ex(int from, int to, const char* file, int line);
#define number(from, to) number_ex(from, to, __FILE__, __LINE__)

float	fnumber(float from, float to);

extern void		thecore_sleep(struct timeval* timeout);
extern DWORD	thecore_random();

extern float	get_float_time();
extern DWORD	get_dword_time();

extern char* time_str(time_t ct);

#define CREATE(result, type, number)  do {							\
	if (!((result) = (type *) calloc ((number), sizeof(type)))) {	\
		sys_err("calloc failed [%d] %s", errno, strerror(errno));	\
		abort(); } } while(0)

#define INSERT_TO_TW_LIST(item, head, prev, next)	\
    if (!(head))									\
    {												\
	head         = item;							\
	    (head)->prev = (head)->next = NULL;			\
    }												\
    else											\
    {												\
		(head)->prev = item;						\
	    (item)->next = head;						\
	    (item)->prev = NULL;						\
	    head         = item;						\
    }

#define INSERT_TO_LIST(item, head, next)			\
    (item)->next = (head);							\
	(head) = (item);								\

#if !defined(_countof)
#if !defined(__cplusplus)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#else
extern "C++"
{
	template <typename _CountofType, size_t _SizeOfArray>
	char(*__countof_helper(_CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];
#define _countof(_Array) sizeof(*__countof_helper(_Array))
}
#endif
#endif