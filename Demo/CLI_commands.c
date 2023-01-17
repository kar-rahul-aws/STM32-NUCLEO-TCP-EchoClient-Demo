/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/* FreeRTOS+UDP includes, just to make the stats available to the CLI
commands. */
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_Sockets.h"

/*
 * Defines a command that sends a Hello message on receiving a UDP packet.
 */
static portBASE_TYPE prvPingReplyCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf( ( char * ) pcWriteBuffer, "\r\nHello !! " );

	return pdTRUE;
}


/* Structure that defines the "task-stats" command line command.  This generates
a table that gives information on each task in the system. */
static const CLI_Command_Definition_t xPingReplyCommand =
{
	( const int8_t * const ) "ping", /* The command string to type. */
	( const int8_t * const ) "ping:\r\n Displays Hello on receiving a ping\r\n\r\n",
	prvPingReplyCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterCLICommands( void )
{
	/* Register all the command line commands */
	FreeRTOS_CLIRegisterCommand( &xPingReplyCommand );

}

