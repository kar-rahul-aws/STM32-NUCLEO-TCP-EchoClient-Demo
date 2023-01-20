#include <string.h>

/* Interface includes. */
#include "freertos_barectf_tracer_platform_in_mem.h"

static int isBackendFull( void * const data )
{
    uint32_t pktSize;
    int full;
    FreeRTOSBarectfTracer_PlatformInMemContext_t * const pPlatformContext =
        ( FreeRTOSBarectfTracer_PlatformInMemContext_t * )data;

    pktSize = freertos_barectf_packet_buf_size( &( pPlatformContext->barectfContext ) );

    if( pktSize > pPlatformContext->remainingLength )
    {
        full = 1;
    }
    else
    {
        full = 0;
    }

    return full;
}
/*-----------------------------------------------------------*/

static void openPacket( void * const data )
{
    FreeRTOSBarectfTracer_PlatformInMemContext_t * const pPlatformContext =
        ( FreeRTOSBarectfTracer_PlatformInMemContext_t * )data;

    freertos_barectf_kernel_open_packet( &( pPlatformContext->barectfContext ), 0 );
}
/*-----------------------------------------------------------*/

static void closePacket( void * const data )
{
    uint32_t pktSize;
    FreeRTOSBarectfTracer_PlatformInMemContext_t * const pPlatformContext =
        ( FreeRTOSBarectfTracer_PlatformInMemContext_t * )data;

    freertos_barectf_kernel_close_packet( &( pPlatformContext->barectfContext ) );

    /* Write packet to the stream file. */
    pktSize = freertos_barectf_packet_buf_size( &( pPlatformContext->barectfContext ) );
    memcpy( pPlatformContext->pCurLoc,
            freertos_barectf_packet_buf( &( pPlatformContext->barectfContext ) ),
            pktSize );

    pPlatformContext->pCurLoc += pktSize;
    pPlatformContext->remainingLength -= pktSize;
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformInMemInit( FreeRTOSBarectfTracer_PlatformInMemContext_t *pPlatformContext,
                                              uint8_t *pTraceBuffer,
                                              size_t traceBufferSize,
                                              FreeRTOSBarectfTracer_GetClock_t getClockFunction,
                                              uint8_t *pTraceCaptureBuf,
                                              uint32_t traceCaptureBufLength )
{
    struct freertos_barectf_platform_callbacks barectfCallbacks;

    barectfCallbacks.freertos_barectf_clock_get_value = getClockFunction;
    barectfCallbacks.is_backend_full = isBackendFull;
    barectfCallbacks.open_packet = openPacket;
    barectfCallbacks.close_packet = closePacket;

    pPlatformContext->pTraceCaptureBuf = pTraceCaptureBuf;
    pPlatformContext->traceCaptureBufLength = traceCaptureBufLength;
    pPlatformContext->pCurLoc = pTraceCaptureBuf;
    pPlatformContext->remainingLength = traceCaptureBufLength;

    memset( pTraceCaptureBuf, 0, traceCaptureBufLength );

    freertos_barectf_init( &( pPlatformContext->barectfContext ),
                           pTraceBuffer,
                           traceBufferSize,
                           barectfCallbacks,
                           pPlatformContext );
    openPacket( pPlatformContext );
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformInMemDeInit( FreeRTOSBarectfTracer_PlatformInMemContext_t *pPlatformContext )
{
    if( freertos_barectf_packet_is_open( &( pPlatformContext->barectfContext ) ) )
    {
        if( freertos_barectf_packet_is_empty( &( pPlatformContext->barectfContext ) ) != 0 )
        {
            closePacket( pPlatformContext );
        }
    }
}
/*-----------------------------------------------------------*/

