#pragma once

typedef struct heart HEART;
typedef struct heart* LPHEART;
typedef void (*HEARTFUNC) (LPHEART heart, int pulse);

struct heart
{
	HEARTFUNC func;
	struct timeval before_sleep;
	struct timeval opt_time;
	struct timeval last_time;

	int passes_per_sec;
	int pulse;
};

extern LPHEART heart_new(int opt_usec, HEARTFUNC func);
extern void heart_delete(LPHEART ht);
extern int heart_idle(LPHEART ht);
extern void heart_beat(LPHEART ht, int pulses);