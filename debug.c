/*===================================================
**Discription	:
**Author		: 	vincenzo
**Date			: 	2015.9.15 0:28
**File			: 	debug.c
**===================================================
*/

#ifdef __DEBUG__
#include <stdarg.h>
void debug_print(const char *fmt, ...)
{
	 va_list ap;
	 va_start(ap, fmt);
	 vprintf(fmt, ap);
	 va_end(ap);
}	
#else
void debug_print(const char *fmt, ...)
{
	
}
#endif