/*
 * netstat_capture.c
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

/* Standard includes. */
#include <string.h>
#include <stdbool.h>

/* Interface includes. */
#include "netstat_capture.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* STM32 HAL includes. */
#include "stm32h7xx_hal.h"

/*-----------------------------------------------------------*/

void netstat_get_all_stats( all_stats_t * p_all_stats )
{
    memset( p_all_stats, 0, sizeof( all_stats_t ) );

    p_all_stats->udp_stats.unicast_stats.pckt_rx = 12;
    p_all_stats->udp_stats.unicast_stats.pckt_tx = 14;
    p_all_stats->udp_stats.unicast_stats.pcket_drop_rx = 16;
    p_all_stats->udp_stats.unicast_stats.pcket_drop_tx = 18;
    p_all_stats->udp_stats.unicast_stats.bytes_rx = 10;
    p_all_stats->udp_stats.unicast_stats.bytes_tx = 20;
}

/*-----------------------------------------------------------*/