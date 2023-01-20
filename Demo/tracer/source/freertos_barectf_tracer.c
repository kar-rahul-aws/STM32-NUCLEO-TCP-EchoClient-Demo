/* Standard includes. */
#include <stddef.h>

#include "freertos_trace.h"
#include "freertos_barectf_tracer.h"

typedef struct FreeRTOSBarectfTracerTask
{
    const char *pTaskName;
    uint32_t taskNumber;
    uint32_t taskPriority;
} FreeRTOSBarectfTracerTask_t;
/*-----------------------------------------------------------*/

FreeRTOSBarectfTracerTask_t outgoingTask;
struct freertos_barectf_kernel_ctx* pCachedBarectfContext = NULL;
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_Init( struct freertos_barectf_kernel_ctx* pBarectfContext )
{
    /* Ensure that first switch-in event is traced correctly. */
    outgoingTask.pTaskName = "IDLE";
    outgoingTask.taskNumber = 0;
    outgoingTask.taskPriority = 0;

    pCachedBarectfContext = pBarectfContext;
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_DeInit( void )
{
    pCachedBarectfContext = NULL;
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_TaskSwitchedIn( const char *pTaskName,
                                           uint32_t taskNumber,
                                           uint32_t taskPriority )
{
    if( pCachedBarectfContext != NULL )
    {
        /* Emit the event on switched-in event as we now know both the outgoing and
         * the incoming task. */
        freertos_barectf_kernel_trace_sched_switch( pCachedBarectfContext,
                                                    outgoingTask.pTaskName,
                                                    outgoingTask.taskNumber,
                                                    outgoingTask.taskPriority,
                                                    0,
                                                    pTaskName,
                                                    taskNumber,
                                                    taskPriority );
    }
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_TaskSwitchedOut( const char *pTaskName,
                                            uint32_t taskNumber,
                                            uint32_t taskPriority )
{
    if( pCachedBarectfContext != NULL )
    {
        /* Cache the outgoing task information for later use in switched-in event. */
        outgoingTask.pTaskName = pTaskName;
        outgoingTask.taskNumber = taskNumber;
        outgoingTask.taskPriority = taskPriority;
    }
}
/*-----------------------------------------------------------*/
