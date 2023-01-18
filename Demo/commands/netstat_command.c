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

/* Netstat includes. */
#include "netstat_capture.h"

/**
 * @brief Interpreter that handles the netstat command.
 */
static portBASE_TYPE prvNetStatCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
        all_stats_t all_network_stats;

        ( void ) pcCommandString;

        configASSERT( pcWriteBuffer );

        netstat_get_all_stats( &( all_network_stats ) );
        snprintf( pcWriteBuffer, xWriteBufferLen, "%ld,%ld,%ld,%ld,%ld,%ld",
                                                  all_network_stats.udp_stats.unicast_stats.pckt_rx,
                                                  all_network_stats.udp_stats.unicast_stats.pckt_tx,
                                                  all_network_stats.udp_stats.unicast_stats.pcket_drop_rx,
                                                  all_network_stats.udp_stats.unicast_stats.pcket_drop_tx,
                                                  all_network_stats.udp_stats.unicast_stats.bytes_rx,
                                                  all_network_stats.udp_stats.unicast_stats.bytes_tx );

        /* Return pdFALSE to indicate that the response is complete. */
        return pdFALSE;
}

/*-----------------------------------------------------------*/

/**
 * @brief Structure that defines the "netstat" command line command.
 */
static const CLI_Command_Definition_t xNetStatCommand =
{
    ( const char * const ) "netstat", /* The command string to type. */
    ( const char * const ) "netstat: Get the Network Statistics.\r\n",
    prvNetStatCommandInterpreter, /* The interpreter function for the command. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterNetStatCommand( void )
{
    /* Register ping command. */
    FreeRTOS_CLIRegisterCommand( &( xNetStatCommand ) );
}

/*-----------------------------------------------------------*/
