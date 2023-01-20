/* Interface includes. */
#include "freertos_barectf_tracer_platform_segger_rtt.h"

/* SEGGER includes. */
#include "RTT/SEGGER_RTT.h"

/* Index of the RTT buffer to be used. */
#define RTT_UP_BUFFER_INDEX         1
/*-----------------------------------------------------------*/

static int isBackendFull( void * const data )
{
    ( void ) data;
    return 0;
}
/*-----------------------------------------------------------*/

static void openPacket( void * const data )
{
    struct freertos_barectf_kernel_ctx * const pBarectfContext =
        ( struct freertos_barectf_kernel_ctx *  )data;

    freertos_barectf_kernel_open_packet( pBarectfContext, 0 );
}
/*-----------------------------------------------------------*/

static void closePacket( void * const data )
{
    struct freertos_barectf_kernel_ctx * const pBarectfContext =
        ( struct freertos_barectf_kernel_ctx *  )data;

    freertos_barectf_kernel_close_packet( pBarectfContext );

    /* Write packet to the stream file. */
    ( void ) SEGGER_RTT_Write( RTT_UP_BUFFER_INDEX,
                               freertos_barectf_packet_buf( pBarectfContext ),
                               freertos_barectf_packet_buf_size( pBarectfContext ) );
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformSeggerRTTInit( struct freertos_barectf_kernel_ctx * pBarectfContext,
                                                  uint8_t * pRttBuffer,
                                                  size_t rttBufferSize,
                                                  uint8_t * pTraceBuffer,
                                                  size_t traceBufferSize,
                                                  FreeRTOSBarectfTracer_GetClock_t getClockFunction )
{
    struct freertos_barectf_platform_callbacks barectfCallbacks;

    barectfCallbacks.freertos_barectf_clock_get_value = getClockFunction;
    barectfCallbacks.is_backend_full = isBackendFull;
    barectfCallbacks.open_packet = openPacket;
    barectfCallbacks.close_packet = closePacket;

    SEGGER_RTT_ConfigUpBuffer( RTT_UP_BUFFER_INDEX,
                               "Trace_Buffer",
                               pRttBuffer,
                               rttBufferSize,
                               SEGGER_RTT_MODE_NO_BLOCK_SKIP );

    freertos_barectf_init( pBarectfContext,
                           pTraceBuffer,
                           traceBufferSize,
                           barectfCallbacks,
                           pBarectfContext );
    openPacket( pBarectfContext );
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformSeggerRTTDeInit( struct freertos_barectf_kernel_ctx * pBarectfContext )
{
    if( freertos_barectf_packet_is_open( pBarectfContext ) )
    {
        if( freertos_barectf_packet_is_empty( pBarectfContext ) != 0 )
        {
            closePacket( pBarectfContext );
        }
    }
}
/*-----------------------------------------------------------*/
