#ifndef FREERTOS_BARECTF_TRACER_PLATFORM_SEGGER_RTT_H_
#define FREERTOS_BARECTF_TRACER_PLATFORM_SEGGER_RTT_H_

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>

/* Tracer includes. */
#include "freertos_trace.h"

typedef uint64_t ( *FreeRTOSBarectfTracer_GetClock_t )( void * );

void FreeRTOSBarectfTracer_PlatformSeggerRTTInit( struct freertos_barectf_kernel_ctx * pBarectfContext,
                                                  uint8_t * pRttBuffer,
                                                  size_t rttBufferSize,
                                                  uint8_t * pTraceBuffer,
                                                  size_t traceBufferSize,
                                                  FreeRTOSBarectfTracer_GetClock_t getClockFunction );

void FreeRTOSBarectfTracer_PlatformSeggerRTTDeInit( struct freertos_barectf_kernel_ctx * pBarectfContext );

#endif /* FREERTOS_BARECTF_TRACER_PLATFORM_SEGGER_RTT_H_ */
