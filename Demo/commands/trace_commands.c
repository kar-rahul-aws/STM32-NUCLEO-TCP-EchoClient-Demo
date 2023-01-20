/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/* Tracer includes. */
#include "freertos_barectf_tracer.h"
#include "freertos_barectf_tracer_platform_in_mem.h"

#define TRACE_BUFFER_LENGTH             ( 1024 )
#define TRACE_CAPTURE_BUFFER_LENGTH     ( 5 * 1024 )

/*-----------------------------------------------------------*/

static FreeRTOSBarectfTracer_PlatformInMemContext_t platformContext;

static uint8_t ucTraceBuffer[ TRACE_BUFFER_LENGTH ];
static uint8_t ucTraceCaptureBuffer[ TRACE_CAPTURE_BUFFER_LENGTH ];

/*-----------------------------------------------------------*/

static uint64_t getClock( void * param )
{
    extern uint32_t ulGetTim7Tick( void );

    ( void ) param;

    return ( uint64_t )ulGetTim7Tick();
}

/*-----------------------------------------------------------*/

/**
 * @brief Interpreter that handles the trace command.
 */
static portBASE_TYPE prvTraceCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char * pcCommandParameter;
    BaseType_t xCommandParameterLength;

    configASSERT( pcWriteBuffer );

    pcCommandParameter = FreeRTOS_CLIGetParameter( pcCommandString, 1, &( xCommandParameterLength ) );

    if( pcCommandParameter != NULL )
    {
        if( strncmp( pcCommandParameter, "start", xCommandParameterLength ) == 0 )
        {
            FreeRTOSBarectfTracer_PlatformInMemInit( &( platformContext ),
                                                     ucTraceBuffer,
                                                     TRACE_BUFFER_LENGTH,
                                                     getClock,
                                                     ucTraceCaptureBuffer,
                                                     TRACE_CAPTURE_BUFFER_LENGTH );

            FreeRTOSBarectfTracer_Init( &( platformContext.barectfContext ) );

            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );
        }
        else if( strncmp( pcCommandParameter, "stop", xCommandParameterLength ) == 0 )
        {
            FreeRTOSBarectfTracer_DeInit();

            FreeRTOSBarectfTracer_PlatformInMemDeInit( &( platformContext ) );

            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );
        }
        else if( strncmp( pcCommandParameter, "get", xCommandParameterLength ) == 0 )
        {
            /* HACK - TRACE data is binary data and therefore, cannot be sent as
             * a null terminated string as payload can have legitimate null
             * bytes. Instead, we write a marker here in the output buffer which
             * the caller of FreeRTOS_CLIProcessCommand checks and sends the
             * actual data instead of this response. */
            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "TRACE-GET" );
        }
        else
        {
            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "Bad Command." );
        }
    }
    else
    {
        snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "Bad Command." );
    }

    /* Return pdFALSE to indicate that the response is complete. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "trace" command line command.
 */
static const CLI_Command_Definition_t xTraceCommand =
{
    ( const char * const ) "trace", /* The command string to type. */
    ( const char * const ) "trace: Starts, stops and gets the trace according to the parameter - start/stop/get.\r\n",
    prvTraceCommandInterpreter, /* The interpreter function for the command. */
    1 /* One parameter - start, stop or get. */
};

/*-----------------------------------------------------------*/

void vRegisterTraceCommand( void )
{
    FreeRTOS_CLIRegisterCommand( &( xTraceCommand ) );
}

/*-----------------------------------------------------------*/

void  FreeRTOSBarectfTracer_GetTrace( const uint8_t ** ppucCapturedTrace,
                                      uint32_t * pulCapturedTraceLength )
{
    *ppucCapturedTrace = platformContext.pTraceCaptureBuf;
    *pulCapturedTraceLength = ( platformContext.traceCaptureBufLength - platformContext.remainingLength );
}

/*-----------------------------------------------------------*/
