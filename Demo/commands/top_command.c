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
 * @brief Interpreter that handles the top command.
 */
static portBASE_TYPE prvTopCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;

    configASSERT( pcWriteBuffer );

    /* TODO - We need to call uxTaskGetSystemState and return the
     * results in a format suitable for our CLI. */
    vTaskGetRunTimeStats( pcWriteBuffer );

    /* Return pdFALSE to indicate that the response is complete. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "top" command line command.
 */
static const CLI_Command_Definition_t xTopCommand =
{
    ( const char * const ) "top", /* The command string to type. */
    ( const char * const ) "top: Returns various stats about all tasks.\r\n",
    prvTopCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterTopCommand( void )
{
    /* Register top command. */
    FreeRTOS_CLIRegisterCommand( &( xTopCommand ) );
}

/*-----------------------------------------------------------*/

