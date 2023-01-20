#ifndef FREERTOS_BARECTF_TRACER_H_
#define FREERTOS_BARECTF_TRACER_H_

/* Standard includes. */
#include <stdint.h>

struct freertos_barectf_kernel_ctx;

/* Initialization and cleanup. */
void FreeRTOSBarectfTracer_Init( struct freertos_barectf_kernel_ctx* pBarectfContext );

void FreeRTOSBarectfTracer_DeInit( void );
/*-----------------------------------------------------------*/

/* Context Switch. */
#define traceTASK_SWITCHED_IN()                                                                                                     \
    FreeRTOSBarectfTracer_TaskSwitchedIn( &( pxCurrentTCB->pcTaskName[ 0 ] ),                                                       \
                                          ( ( ( TaskHandle_t )pxCurrentTCB == xIdleTaskHandle ) ? 0 : pxCurrentTCB->uxTCBNumber ),  \
                                          pxCurrentTCB->uxPriority )

#define traceTASK_SWITCHED_OUT()                                                                                                    \
    FreeRTOSBarectfTracer_TaskSwitchedOut( &( pxCurrentTCB->pcTaskName[ 0 ] ),                                                      \
                                           ( ( ( TaskHandle_t )pxCurrentTCB == xIdleTaskHandle ) ? 0 : pxCurrentTCB->uxTCBNumber ), \
                                           pxCurrentTCB->uxPriority )

void FreeRTOSBarectfTracer_TaskSwitchedIn( const char *pTaskName,
                                           uint32_t taskNumber,
                                           uint32_t taskPriority );

void FreeRTOSBarectfTracer_TaskSwitchedOut( const char *pTaskName,
                                            uint32_t taskNumber,
                                            uint32_t taskPriority );
/*-----------------------------------------------------------*/

#endif /* FREERTOS_BARECTF_TRACER_H_ */
