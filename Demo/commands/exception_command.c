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

/* Exception info. */
#include "expinfo.h"

/**
 * @brief Interpreter that handles the pcap command.
 */
static portBASE_TYPE prvExceptionCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char * pcCommandParameter;
    BaseType_t xCommandParameterLength;

    configASSERT( pcWriteBuffer );

    pcCommandParameter = FreeRTOS_CLIGetParameter( pcCommandString, 1, &( xCommandParameterLength ) );

    if( pcCommandParameter != NULL )
    {
        if( strncmp( pcCommandParameter, "dump", xCommandParameterLength ) == 0 )
        {
            /* HACK - exception data is binary data and therefore, cannot be sent as
             * a null terminated string as payload can have legitimate null
             * bytes. Instead, we write a marker here in the output buffer which
             * the caller of FreeRTOS_CLIProcessCommand checks and sends the
             * actual data instead of this response. */
            BaseType_t xIsExceptionDumpExist;
            xIsExceptionDumpExist = ExpInfo_InfoExist();

            if( xIsExceptionDumpExist == pdTRUE )
            {
                snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "EXCEPTION-DUMP" );
            }
            else
            {
                snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "No exception dump available." );
            }
        }
        else if( strncmp( pcCommandParameter, "assert", xCommandParameterLength ) == 0 )
        {
            configASSERT( pdFALSE );
        }
        else if( strncmp( pcCommandParameter, "check", xCommandParameterLength ) == 0 )
        {
            BaseType_t xIsExceptionDumpExist;
            xIsExceptionDumpExist = ExpInfo_InfoExist();

            if( xIsExceptionDumpExist == pdTRUE )
            {
                snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "TRUE" );
            }
            else
            {
                snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "FALSE" );
            }
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
 * @brief Structure that defines the "exception" command line command.
 */
static const CLI_Command_Definition_t xExceptionCommand =
{
    ( const char * const ) "exception", /* The command string to type. */
    ( const char * const ) "exception: Checks if exception dump exists or starts dumpingaccording to the parameter - check/dump.\r\n",
    prvExceptionCommandInterpreter, /* The interpreter function for the command. */
    1 /* One parameter - start, stop or get. */
};

/*-----------------------------------------------------------*/

void vRegisterExceptionCommand( void )
{
    FreeRTOS_CLIRegisterCommand( &( xExceptionCommand ) );
}

/*-----------------------------------------------------------*/
