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

/*FreeRTOS+TCP includes.*/
#include "FreeRTOS_Net_Stat.h"

/**
 * @brief Interpreter that handles the netstat command.
 */
static portBASE_TYPE prvNetStatCommandInterpreter( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    allstat all_network_stats;
    eErrorType_t xResult = eIncorrectStat;

    ( void ) pcCommandString;

    configASSERT( pcWriteBuffer );

    xResult = vGetNetStat( eGetStat, &( all_network_stats ) );
    configASSERT( xResult == eSuccessStat );

    snprintf( pcWriteBuffer, xWriteBufferLen, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%lu,%lu,%lu,%lu",
                                               all_network_stats.udp_stat.stat.pckt_rx,
                                               all_network_stats.udp_stat.stat.pckt_tx,
                                               all_network_stats.udp_stat.stat.pcket_drop_rx,
                                               all_network_stats.udp_stat.stat.pcket_drop_tx,
                                               all_network_stats.udp_stat.stat.bytes_rx,
                                               all_network_stats.udp_stat.stat.bytes_tx,
                                               all_network_stats.tcp_stat.stat.pckt_rx,
                                               all_network_stats.tcp_stat.stat.pckt_tx,
                                               all_network_stats.tcp_stat.stat.pcket_drop_rx,
                                               all_network_stats.tcp_stat.stat.pcket_drop_tx,
                                               all_network_stats.tcp_stat.stat.bytes_rx,
                                               all_network_stats.tcp_stat.stat.bytes_tx,
                                               all_network_stats.icmp_stat.stat.pckt_rx,
                                               all_network_stats.icmp_stat.stat.pckt_tx,
                                               all_network_stats.icmp_stat.stat.pcket_drop_rx,
                                               all_network_stats.icmp_stat.stat.pcket_drop_tx,
                                               all_network_stats.icmp_stat.stat.bytes_rx,
                                               all_network_stats.icmp_stat.stat.bytes_tx,
                                               ( uint32_t )( ( all_network_stats.rx_latency >> 32 ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( all_network_stats.rx_latency ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( all_network_stats.tx_latency >> 32 ) & 0xFFFFFFFF ),
                                               ( uint32_t )( ( all_network_stats.tx_latency ) & 0xFFFFFFFF ) );

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
