/*===================================================
**Discription		:
**Author			: 	vincenzo
**Date			: 	2015.9.15 0:28
**File			: 	debug.c
**===================================================
*/

void mono_info(const char *fmt, ...)
{
	 va_list ap;
	 va_start(ap, fmt);
	 vprintf(fmt, ap);
	 va_end(ap);
}

#ifdef __DEBUG__
#include <stdarg.h>
void mono_debug(const char *fmt, ...)
{
	 va_list ap;
	 va_start(ap, fmt);
	 vprintf(fmt, ap);
	 va_end(ap);
}	
#else
void mono_debug(const char *fmt, ...){}
#endif

