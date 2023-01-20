/* Interface includes. */
#include "freertos_barectf_tracer_platform_linux.h"

static int isBackendFull( void * const data )
{
    ( void ) data;
    return 0;
}
/*-----------------------------------------------------------*/

static void openPacket( void * const data )
{
    FreeRTOSBarectfTracer_PlatformLinuxContext_t * const pPlatformContext =
        ( FreeRTOSBarectfTracer_PlatformLinuxContext_t * )data;

    freertos_barectf_kernel_open_packet( &( pPlatformContext->barectfContext ), 0 );
}
/*-----------------------------------------------------------*/

static void closePacket( void * const data )
{
    FreeRTOSBarectfTracer_PlatformLinuxContext_t * const pPlatformContext =
        ( FreeRTOSBarectfTracer_PlatformLinuxContext_t * )data;

    freertos_barectf_kernel_close_packet( &( pPlatformContext->barectfContext ) );

    /* Write packet to the stream file. */
    ( void )fwrite( freertos_barectf_packet_buf( &( pPlatformContext->barectfContext ) ),
                    freertos_barectf_packet_buf_size( &( pPlatformContext->barectfContext ) ),
                    1,
                    pPlatformContext->pTraceFile );
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformLinuxInit( FreeRTOSBarectfTracer_PlatformLinuxContext_t *pPlatformContext,
                                              uint8_t *pTraceBuffer,
                                              size_t traceBufferSize,
                                              FreeRTOSBarectfTracer_GetClock_t getClockFunction,
                                              const char *pDataStreamFilePath )
{
    struct freertos_barectf_platform_callbacks barectfCallbacks;

    barectfCallbacks.freertos_barectf_clock_get_value = getClockFunction;
    barectfCallbacks.is_backend_full = isBackendFull;
    barectfCallbacks.open_packet = openPacket;
    barectfCallbacks.close_packet = closePacket;

    pPlatformContext->pTraceFile = fopen( pDataStreamFilePath, "wb" );

    freertos_barectf_init( &( pPlatformContext->barectfContext ),
                           pTraceBuffer,
                           traceBufferSize,
                           barectfCallbacks,
                           pPlatformContext );
    openPacket( pPlatformContext );
}
/*-----------------------------------------------------------*/

void FreeRTOSBarectfTracer_PlatformLinuxDeInit( FreeRTOSBarectfTracer_PlatformLinuxContext_t *pPlatformContext )
{
    if( freertos_barectf_packet_is_open( &( pPlatformContext->barectfContext ) ) )
    {
        if( freertos_barectf_packet_is_empty( &( pPlatformContext->barectfContext ) ) != 0 )
        {
            closePacket( pPlatformContext );
        }
    }

    fclose( pPlatformContext->pTraceFile );
}
/*-----------------------------------------------------------*/
