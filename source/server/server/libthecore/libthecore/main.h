#pragma once
#include "heart.h"

extern volatile int tics;
extern volatile int shutdowned;
extern LPHEART thecore_heart;

enum ProffilerEnum
{
	PF_IDLE = 0,
	PF_HEARTBEAT = 1,
	NUM_PF = 3,
};

extern unsigned int thecore_profiler[NUM_PF];
extern int thecore_init(int fps, HEARTFUNC heartbeat_func);
extern int thecore_idle(void);
extern void thecore_shutdown(void);
extern void thecore_destroy(void);
extern int thecore_pulse(void);
extern float thecore_time(void);
extern float thecore_pulse_per_second(void);
extern int thecore_is_shutdowned(void);
extern void thecore_tick(void);