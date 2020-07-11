#pragma once
#define sys_err(fmt, args...) _sys_err(__FUNCTION__, __LINE__, fmt, ##args)

extern int log_init(void);
extern void log_destroy(void);
extern void log_rotate(void);

extern void log_set_level(unsigned int level);
extern void log_unset_level(unsigned int level);

extern void log_set_expiration_days(unsigned int days);
extern int log_get_expiration_days(void);

extern void _sys_err(const char* func, int line, const char* format, ...);
extern void sys_log_header(const char* header);
extern void sys_log(unsigned int lv, const char* format, ...);
extern void pt_log(const char* format, ...);