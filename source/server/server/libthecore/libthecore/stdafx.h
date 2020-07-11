#pragma once

#define INLINE __inline__

#define false	0
#define true	(!false)

#define FALSE	false
#define TRUE	(!FALSE)

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#include <sys/signal.h>
#include <sys/wait.h>

#include <pthread.h>
#include <semaphore.h>
#include <sys/event.h>

#include "typedef.h"
#include "heart.h"
#include "fdwatch.h"
#include "socket.h"
#include "kstbl.h"
#include "hangul.h"
#include "buffer.h"
#include "signal.h"
#include "log.h"
#include "main.h"
#include "utils.h"
#include "crypt.h"
#include "memcpy.h"