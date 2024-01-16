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
    NetworkStats_t stats;
    NetstatResult_t result;

    ( void ) pcCommandString;

    configASSERT( pcWriteBuffer );

    result = Netstat_GetStats( &( stats ) );
    configASSERT( result == NETSTAT_RESULT_OK );

    snprintf( pcWriteBuffer, xWriteBufferLen, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%lu,%lu,%lu,%lu",
                                               stats.udp.rxPackets,
                                               stats.udp.txPackets,
                                               stats.udp.rxDropped,
                                               stats.udp.txDropped,
                                               stats.udp.rxBytes,
                                               stats.udp.txBytes,
                                               stats.tcp.rxPackets,
                                               stats.tcp.txPackets,
                                               stats.tcp.rxDropped,
                                               stats.tcp.txDropped,
                                               stats.tcp.rxBytes,
                                               stats.tcp.txBytes,
                                               stats.icmp.rxPackets,
                                               stats.icmp.txPackets,
                                               stats.icmp.rxDropped,
                                               stats.icmp.txDropped,
                                               stats.icmp.rxBytes,
                                               stats.icmp.txBytes,
                                               ( uint32_t )( ( stats.rxLatency >> 32 ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( stats.rxLatency ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( stats.txLatency >> 32 ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( stats.txLatency ) & 0xFFFFFFFF ) );

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
    /* Register netstat command. */
    FreeRTOS_CLIRegisterCommand( &( xNetStatCommand ) );
}

/*-----------------------------------------------------------*/
