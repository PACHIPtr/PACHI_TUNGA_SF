#pragma once

#include "../UserInterface/Locale_inc.h"

#ifdef ENABLE_CRASH_SENDER
extern int SetEterExceptionHandler();
extern int RemoveEterException();
#else
extern void SetEterExceptionHandler();
#endif
