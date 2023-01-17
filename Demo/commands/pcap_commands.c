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

/* Pcap capture includes. */
#include "pcap_capture.h"

/**
 * @brief Interpreter that handles the pcap command.
 */
static portBASE_TYPE prvPcapCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char * pcCommandParameter;
    BaseType_t xCommandParameterLength;

    configASSERT( pcWriteBuffer );

    pcCommandParameter = FreeRTOS_CLIGetParameter( pcCommandString, 1, &( xCommandParameterLength ) );

    if( pcCommandParameter != NULL )
    {
        if( strncmp( pcCommandParameter, "start", xCommandParameterLength ) == 0 )
        {
            pcap_capture_start();
            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );
        }
        else if( strncmp( pcCommandParameter, "stop", xCommandParameterLength ) == 0 )
        {
            pcap_capture_stop();
            snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );
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
 * @brief Structure that defines the "pcap" command line command.
 */
static const CLI_Command_Definition_t xPcapCommand =
{
    ( const char * const ) "pcap", /* The command string to type. */
    ( const char * const ) "pcap: Starts, stops and gets the packet capture according to the parameter - start/stop/get.\r\n",
    prvPcapCommandInterpreter, /* The interpreter function for the command. */
    1 /* One parameter - start, stop or get. */
};

/*-----------------------------------------------------------*/

void vRegisterPcapCommand( void )
{
    FreeRTOS_CLIRegisterCommand( &( xPcapCommand ) );
}

/*-----------------------------------------------------------*/
