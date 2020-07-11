#include <string.h>
void* (*thecore_memcpy) (void* to, const void* from, size_t len) = memcpy;