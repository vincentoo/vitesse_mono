/*=================================================
**Discription		:
**Author			: 	vincenzo
**Email                  :      <liyulinga@qq.com>
**Date			: 	2015.9.20 9:28
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
#define MEMCMP memcmp

#define PID_T pthread_t
#define PTHREAD_CREATE pthread_create
#define PTHREAD_JOIN pthread_join
#define EXIT exit

#define HTONS htons
#define NTOHS ntohs

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;

#ifdef __cplusplus
}
#endif
#endif
