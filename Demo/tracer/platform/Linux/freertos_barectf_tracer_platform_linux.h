#ifndef FREERTOS_BARECTF_TRACER_PLATFORM_LINUX_H_
#define FREERTOS_BARECTF_TRACER_PLATFORM_LINUX_H_

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Baretrace includes. */
#include "freertos_trace.h"

typedef struct FreeRTOSBarectfTracer_PlatformLinuxContext
{
    struct freertos_barectf_kernel_ctx barectfContext;
    FILE * pTraceFile;
} FreeRTOSBarectfTracer_PlatformLinuxContext_t ;

typedef uint64_t ( *FreeRTOSBarectfTracer_GetClock_t )( void * );

void FreeRTOSBarectfTracer_PlatformLinuxInit( FreeRTOSBarectfTracer_PlatformLinuxContext_t *pPlatformContext,
                                              uint8_t *pTraceBuffer,
                                              size_t traceBufferSize,
                                              FreeRTOSBarectfTracer_GetClock_t getClockFunction,
                                              const char *pDataStreamFilePath );

void FreeRTOSBarectfTracer_PlatformLinuxDeInit( FreeRTOSBarectfTracer_PlatformLinuxContext_t *pPlatformContext );

#endif /* FREERTOS_BARECTF_TRACER_PLATFORM_LINUX_H_ */
