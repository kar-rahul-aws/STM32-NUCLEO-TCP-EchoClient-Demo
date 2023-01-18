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

#include "netstat_capture.h"

/**
 * @brief Interpreter that handles the ping command.
 */
/**
 * @brief Buffer which contains all the network Statistics.
 */


static portBASE_TYPE prvNetStatCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
		const char * pcCommandParameter;
	    BaseType_t xCommandParameterLength;
	    allstat result;
	    configASSERT( pcWriteBuffer );

	    pcCommandParameter = FreeRTOS_CLIGetParameter( pcCommandString, 1, &( xCommandParameterLength ) );

	    if( pcCommandParameter != NULL )
	    {
	    	if( strncmp( pcCommandParameter, "start", xCommandParameterLength ) == 0 )
	    	{
	    		netstat_capture_start();
	    		snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK." );
	    	}
	    	else if( strncmp( pcCommandParameter, "stop", xCommandParameterLength ) == 0 )
	    	{
	    		netstat_capture_stop();
	    		snprintf( ( char * ) pcWriteBuffer, xWriteBufferLen, "OK." );

	  	    }
	    	else if( strncmp( pcCommandParameter, "get", xCommandParameterLength ) == 0 )
	    	{
	    		vGetNetStat(3,0,0,0,0,&result);
	    		configPRINTF(("%d\n",result.udp_stat.unicast_stat.bytes_rx));
	    		snprintf( ( char * ) pcWriteBuffer, "%d,", (char*)result.udp_stat.unicast_stat.bytes_rx);

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
 * @brief Structure that defines the "ping" command line command.
 */
static const CLI_Command_Definition_t xNetStatCommand =
{
    ( const char * const ) "netstat", /* The command string to type. */
    ( const char * const ) "netstat: Starts, stops and gets the Network Statistics according to the parameter - start,stop,get.\r\n",
    prvNetStatCommandInterpreter, /* The interpreter function for the command. */
    1 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterNetStatCommand( void )
{
	/* Register ping command. */
	FreeRTOS_CLIRegisterCommand( &( xNetStatCommand ) );
}

/*-----------------------------------------------------------*/
