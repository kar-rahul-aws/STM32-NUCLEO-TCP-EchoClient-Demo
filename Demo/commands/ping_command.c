/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/**
 * @brief Interpreter that handles the ping command.
 */
static portBASE_TYPE prvPingCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;

    configASSERT( pcWriteBuffer );

    snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK" );

    /* Return pdFALSE to indicate that the response is complete. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "ping" command line command.
 */
static const CLI_Command_Definition_t xPingCommand =
{
    ( const char * const ) "ping", /* The command string to type. */
    ( const char * const ) "ping: Returns OK.\r\n",
    prvPingCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterPingCommand( void )
{
    /* Register ping command. */
    FreeRTOS_CLIRegisterCommand( &( xPingCommand ) );
}

/*-----------------------------------------------------------*/

