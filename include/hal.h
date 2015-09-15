/*=================================================
**Discription		:
**Author			: 	vincenzo
**Date			: 	2015.9.15 0:28
**File			: 	hal.h
**=================================================
*/
#ifndef __HAL_H__
#define __HAL_H__

#include <pthread.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MALLOC malloc
#define FREE   free
#define MEMSET memset
#define MEMCPY memcpy

#define PID_T pthread_t
#define PTHREAD_CREATE pthread_create
#define EXIT exit

#define HTONS htons
#define NTOHS ntohs

#ifdef __cplusplus
}
#endif
#endif
