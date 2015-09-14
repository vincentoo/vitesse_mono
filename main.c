/*=================================================
**Discription	:
**Author		: 	vincenzo
**Date			: 	2015.9.15 0:28
**File			: 	main.c
**=================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "sha1.h"

int main(void)
{
	char buf[128] = {0};
	char *string_test = "";
	
	sha1(string_test, buf);
	
	debug_print("%s\n", buf);
	return 0;
}