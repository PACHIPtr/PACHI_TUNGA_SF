#include "stdafx.h"

static struct timeval null_time = { 0, 0 };

#define ishprint(x)     ((((x) & 0xE0) > 0x90) || isprint(x))

void printdata(const unsigned char* data, int bytes)
{
	int i, j, k;
	const unsigned char* p;

	fprintf(stderr, "------------------------------------------------------------------\n");
	j = bytes;
	while (1)
	{
		k = j >= 16 ? 16 : j;

		p = data;
		for (i = 0; i < 16; i++)
		{
			if (i >= k)
			{
				fprintf(stderr, "   ");
			}
			else
			{
				fprintf(stderr, "%02x ", *p);
			}
			p++;
		}

		fprintf(stderr, "| ");

		p = data;
		for (i = 0; i < k; i++)
		{
			if (i >= k)
			{
				fprintf(stderr, " ");
			}
			else
			{
				fprintf(stderr, "%c", ishprint(*p) && ishprint(*(p + 1)) ? *p : '.');
			}
			p++;
		}

		fprintf(stderr, "\n");

		j -= 16;
		data += 16;

		if (j <= 0)
		{
			break;
		}
	}

	fprintf(stderr, "------------------------------------------------------------------\n");
}

struct timeval* timediff(const struct timeval* a, const struct timeval* b)
{
	static struct timeval rslt;

	if (a->tv_sec < b->tv_sec)
	{
		return &null_time;
	}
	else if (a->tv_sec == b->tv_sec)
	{
		if (a->tv_usec < b->tv_usec)
		{
			return &null_time;
		}
		else
		{
			rslt.tv_sec = 0;
			rslt.tv_usec = a->tv_usec - b->tv_usec;
			return &rslt;
		}
	}
	else
	{
		rslt.tv_sec = a->tv_sec - b->tv_sec;

		if (a->tv_usec < b->tv_usec)
		{
			rslt.tv_usec = a->tv_usec + 1000000 - b->tv_usec;
			rslt.tv_sec--;
		}
		else
		{
			rslt.tv_usec = a->tv_usec - b->tv_usec;
		}

		return &rslt;
	}
}

struct timeval* timeadd(struct timeval* a, struct timeval* b)
{
	static struct timeval rslt;

	rslt.tv_sec = a->tv_sec + b->tv_sec;
	rslt.tv_usec = a->tv_usec + b->tv_usec;

	while (rslt.tv_usec >= 1000000)
	{
		rslt.tv_usec -= 1000000;
		rslt.tv_sec++;
	}

	return &rslt;
}

char* time_str(time_t ct)
{
	static char* time_s;

	time_s = asctime(localtime(&ct));

	time_s[strlen(time_s) - 6] = '\0';
	time_s += 4;

	return (time_s);
}

void trim_and_lower(const char* src, char* dest, size_t dest_size)
{
	const char* tmp = src;
	size_t len = 0;

	if (!dest || dest_size == 0)
	{
		return;
	}

	if (!src)
	{
		*dest = '\0';
		return;
	}

	while (*tmp)
	{
		if (!isnhspace(*tmp))
		{
			break;
		}

		tmp++;
	}

	--dest_size;

	while (*tmp && len < dest_size)
	{
		*(dest++) = LOWER(*tmp);
		++tmp;
		++len;
	}

	*dest = '\0';

	if (len > 0)
	{
		--dest;

		while (*dest && isnhspace(*dest) && len--)
		{
			*(dest--) = '\0';
		}
	}
}

void lower_string(const char* src, char* dest, size_t dest_size)
{
	const char* tmp = src;
	size_t len = 0;

	if (!dest || dest_size == 0)
	{
		return;
	}

	if (!src)
	{
		*dest = '\0';
		return;
	}

	--dest_size;

	while (*tmp && len < dest_size)
	{
		*(dest++) = LOWER(*tmp);
		++tmp;
		++len;
	}

	*dest = '\0';
}

char* str_dup(const char* source)
{
	char* new_line;

	CREATE(new_line, char, strlen(source) + 1);
	return (strcpy(new_line, source));
}

int is_abbrev(char* arg1, char* arg2)
{
	if (!*arg1)
	{
		return 0;
	}

	for (; *arg1 && *arg2; arg1++, arg2++)
		if (LOWER(*arg1) != LOWER(*arg2))
		{
			return 0;
		}

	if (!*arg1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int filesize(FILE * fp)
{
	int pos;
	int size;

	pos = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, pos, SEEK_SET);

	return (size);
}

void parse_token(char* src, char* token, char* value)
{
	char* tmp;

	for (tmp = src; *tmp && *tmp != ':'; tmp++)
	{
		if (isnhspace(*tmp))
		{
			continue;
		}

		*(token++) = LOWER(*tmp);
	}

	*token = '\0';

	for (tmp += 2; *tmp; tmp++)
	{
		if (*tmp == '\n' || *tmp == '\r')
		{
			continue;
		}

		*(value++) = *tmp;
	}

	*value = '\0';
}

struct tm* tm_calc(const struct tm* curr_tm, int days)
{
	char                yoon = FALSE;
	static struct tm    new_tm;
	int                 monthdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (!curr_tm)
	{
		time_t time_s = time(0);
		new_tm = *localtime(&time_s);
	}
	else
	{
		thecore_memcpy(&new_tm, curr_tm, sizeof(struct tm));
	}

	if (new_tm.tm_mon == 1)
	{
		if (!((new_tm.tm_year + 1900) % 4))
		{
			if (!((new_tm.tm_year + 1900) % 100))
			{
				if (!((new_tm.tm_year + 1900) % 400))
				{
					yoon = TRUE;
				}
			}
			else
			{
				yoon = TRUE;
			}
		}

		if (yoon)
		{
			new_tm.tm_mday += 1;
		}
	}

	if (yoon)
	{
		monthdays[1] = 29;
	}
	else
	{
		monthdays[1] = 28;
	}

	new_tm.tm_mday += days;

	if (new_tm.tm_mday <= 0)
	{
		new_tm.tm_mon--;

		if (new_tm.tm_mon < 0)
		{
			new_tm.tm_year--;
			new_tm.tm_mon = 11;
		}

		new_tm.tm_mday = monthdays[new_tm.tm_mon] + new_tm.tm_mday;
	}
	else if (new_tm.tm_mday > monthdays[new_tm.tm_mon])
	{
		new_tm.tm_mon++;

		if (new_tm.tm_mon > 11)
		{
			new_tm.tm_year++;
			new_tm.tm_mon = 0;
		}

		new_tm.tm_mday = monthdays[new_tm.tm_mon] - new_tm.tm_mday;
	}

	return (&new_tm);
}

int MIN(int a, int b)
{
	return a < b ? a : b;
}

int MAX(int a, int b)
{
	return a > b ? a : b;
}

int MINMAX(int min, int value, int max)
{
	int tv;

	tv = (min > value ? min : value);
	return (max < tv) ? max : tv;
}

DWORD thecore_random()
{
	return random();
}

int number_ex(int from, int to, const char* file, int line)
{
	if (from > to)
	{
		int tmp = from;

		sys_err("number(): first argument is bigger than second argument %d -> %d, %s %d", from, to, file, line);

		from = to;
		to = tmp;
	}

	int returnValue = 0;

	if ((to - from + 1) != 0)
	{
		returnValue = ((thecore_random() % (to - from + 1)) + from);
	}
	else
	{
		sys_err("number(): devided by 0");
	}

	return returnValue;
}

float fnumber(float from, float to)
{
	return (((float)thecore_random() / (float)RAND_MAX) * (to - from)) + from;
}

void thecore_sleep(struct timeval* timeout)
{
	if (select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, timeout) < 0)
	{
		if (errno != EINTR)
		{
			sys_err("select sleep %s", strerror(errno));
			return;
		}
	}
}

void thecore_msleep(DWORD dwMillisecond)
{
	static struct timeval tv_sleep;
	tv_sleep.tv_sec = dwMillisecond / 1000;
	tv_sleep.tv_usec = dwMillisecond * 1000;
	thecore_sleep(&tv_sleep);
}

void core_dump_unix(const char* who, WORD line)
{
	sys_err("*** Dumping Core %s:%d ***", who, line);

	fflush(stdout);
	fflush(stderr);

	if (fork() == 0)
	{
		abort();
	}
}

float get_float_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	tv.tv_sec -= 1057699978;
	return ((float)tv.tv_sec + ((float)tv.tv_usec / 1000000.0f));
}

static DWORD get_boot_sec()
{
	static struct timeval tv_boot = { 0L, 0L };

	if (tv_boot.tv_sec == 0)
	{
		gettimeofday(&tv_boot, NULL);
	}

	return tv_boot.tv_sec;
}

DWORD get_dword_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	tv.tv_sec -= get_boot_sec();
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}