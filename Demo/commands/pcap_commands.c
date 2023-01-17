/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/* Pcap capture includes. */
#include "pcap_capture.h"

/**
 * @brief Interpreter that handles the pcap_start command.
 */
static portBASE_TYPE prvPcapStartCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;

    configASSERT( pcWriteBuffer );

    pcap_capture_start();

    snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );

    /* Return pdFALSE to indicate that the response is complete. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Interpreter that handles the pcap_stop command.
 */
static portBASE_TYPE prvPcapStopCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;

    configASSERT( pcWriteBuffer );

    pcap_capture_stop();

    snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );

    /* Return pdFALSE to indicate that the response is complete. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "pcap_start" command line command.
 */
static const CLI_Command_Definition_t xPcapStartCommand =
{
    ( const char * const ) "pcap_start", /* The command string to type. */
    ( const char * const ) "pcap_start: Starts the packet capture. Returns OK.\r\n",
    prvPcapStartCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "pcap_stop" command line command.
 */
static const CLI_Command_Definition_t xPcapStopCommand =
{
    ( const char * const ) "pcap_stop", /* The command string to type. */
    ( const char * const ) "pcap_stop: Stops the packet capture. Returns OK.\r\n",
    prvPcapStopCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterPcapCommands( void )
{
    FreeRTOS_CLIRegisterCommand( &( xPcapStartCommand ) );
    FreeRTOS_CLIRegisterCommand( &( xPcapStopCommand ) );
}

/*-----------------------------------------------------------*/
