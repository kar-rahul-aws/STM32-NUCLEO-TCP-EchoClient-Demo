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

/**
 * @brief Indicates whether we are currently capturing packets or not.
 */
static uint32_t capture_running = 0;

struct allstat* result ;

void vGetNetStat(eState_t state, bool cast,
						eStatVal_t stat_type,
						eProtocol_t protocol,
						allstat *result )
{
	if(state > eStartStat )
	{
		result->udp_stat.unicast_stat.bytes_rx = 1500;
		result->udp_stat.unicast_stat.bytes_tx = 1200;
		result->udp_stat.unicast_stat.pcket_drop_rx = 80;
		result->udp_stat.unicast_stat.pcket_drop_tx = 70;
		result->udp_stat.unicast_stat.pckt_rx = 100;
		result->udp_stat.unicast_stat.pckt_tx = 150;
	}
}


/*-----------------------------------------------------------*/


void netstat_capture_start( void )
{
    /* Start fresh. */
    memset( result, 0, sizeof(struct all_stats) );

    vGetNetStat(0,0,0,0,result);

    /* Record that the capture is now running. */
    capture_running = 1;
}

/*-----------------------------------------------------------*/

void netstat_capture_stop( void )
{
	vGetNetStat(1,0,0,0,result);
    capture_running = 0;
}

/*-----------------------------------------------------------*/

void netstat_capture_record_packet( void )
{
	vGetNetStat(2,0,0,0,result);
}

/*-----------------------------------------------------------*/
