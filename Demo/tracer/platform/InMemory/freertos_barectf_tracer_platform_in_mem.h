#ifndef FREERTOS_BARECTF_TRACER_PLATFORM_In_MEM_H
#define FREERTOS_BARECTF_TRACER_PLATFORM_In_MEM_H

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Baretrace includes. */
#include "freertos_trace.h"

typedef struct FreeRTOSBarectfTracer_PlatformInMemContext
{
    struct freertos_barectf_kernel_ctx barectfContext;
    uint8_t * pTraceCaptureBuf;
    uint32_t traceCaptureBufLength;
    uint8_t * pCurLoc;
    uint32_t remainingLength;
} FreeRTOSBarectfTracer_PlatformInMemContext_t ;

typedef uint64_t ( *FreeRTOSBarectfTracer_GetClock_t )( void * );

void FreeRTOSBarectfTracer_PlatformInMemInit( FreeRTOSBarectfTracer_PlatformInMemContext_t *pPlatformContext,
                                              uint8_t *pTraceBuffer,
                                              size_t traceBufferSize,
                                              FreeRTOSBarectfTracer_GetClock_t getClockFunction,
                                              uint8_t *pTraceCaptureBuf,
                                              uint32_t traceCaptureBufLength );

void FreeRTOSBarectfTracer_PlatformInMemDeInit( FreeRTOSBarectfTracer_PlatformInMemContext_t *pPlatformContext );

void  FreeRTOSBarectfTracer_GetTrace( const uint8_t ** ppucCapturedTrace,
                                      uint32_t * pulCapturedTraceLength );

#endif /* FREERTOS_BARECTF_TRACER_PLATFORM_In_MEM_H */
