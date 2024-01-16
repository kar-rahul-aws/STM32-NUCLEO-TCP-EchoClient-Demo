/*
 * netstat_capture.c
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

/* Standard includes. */
#include <string.h>

/* Interface includes. */
#include "netstat_capture.h"

/* DWT related defines. */
#define ARM_REG_DEMCR         ( *( volatile uint32_t * ) 0xE000EDFC )
#define ARM_REG_DWT_CTRL      ( *( volatile uint32_t * ) 0xE0001000 )
#define ARM_REG_DWT_CYCCNT    ( *( volatile uint32_t * ) 0xE0001004 )
#define DWT_CYCCNTENA_BIT     ( 1UL << 0 )
#define DWT_TRCENA_BIT        ( 1UL << 24 )

/*-----------------------------------------------------------*/

static NetworkStats_t stats;
static uint32_t record = 0;

/*-----------------------------------------------------------*/

NetstatResult_t Netstat_GetStats( NetworkStats_t * pStats )
{
    NetstatResult_t result = NETSTAT_RESULT_OK;

    if( pStats == NULL )
    {
        result = NETSTAT_RESULT_BAD_PARAM;
    }

    if( result == NETSTAT_RESULT_OK )
    {
        memcpy( pStats, &( stats ), sizeof( NetworkStats_t ) );
    }

    return result;
}

/*-----------------------------------------------------------*/

void StartRecording( void )
{
    memset( &( stats ), 0 , sizeof( NetworkStats_t ) );

    /* Initialize DWT for latency measurements. */
    if( ARM_REG_DWT_CTRL != 0 )
    {
        ARM_REG_DEMCR |= DWT_TRCENA_BIT;
        ARM_REG_DWT_CYCCNT = 0;
        ARM_REG_DWT_CTRL |= DWT_CYCCNTENA_BIT;
    }

    record = 1;
}

/*-----------------------------------------------------------*/

void StopRecording( void )
{
    record = 0;

    /* Disable DWT. */
    ARM_REG_DWT_CYCCNT = 0;
    ARM_REG_DWT_CTRL &= ~DWT_CYCCNTENA_BIT;
}

/*-----------------------------------------------------------*/

void ResetStats( void )
{
    memset( &( stats ), 0 , sizeof( NetworkStats_t ) );
}

/*-----------------------------------------------------------*/

void RecordUdpRxPackets()
{
    if( record == 1 )
    {
        stats.udp.rxPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordUdpRxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.udp.rxBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordUdpRxDroppedPackets()
{
    if( record == 1 )
    {
        stats.udp.rxDropped++;
    }
}

/*-----------------------------------------------------------*/

void RecordUdpTxPackets()
{
    if( record == 1 )
    {
        stats.udp.txPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordUdpTxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.udp.txBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordUdpTxDroppedPackets()
{
    if( record == 1 )
    {
        stats.udp.txDropped++;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpRxPackets()
{
    if( record == 1 )
    {
        stats.tcp.rxPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpRxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.tcp.rxBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpRxDroppedPackets()
{
    if( record == 1 )
    {
        stats.tcp.rxDropped++;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpTxPackets()
{
    if( record == 1 )
    {
        stats.tcp.txPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpTxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.tcp.txBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordTcpTxDroppedPackets()
{
    if( record == 1 )
    {
        stats.tcp.txDropped++;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpRxPackets()
{
    if( record == 1 )
    {
        stats.icmp.rxPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpRxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.icmp.rxBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpRxDroppedPackets()
{
    if( record == 1 )
    {
        stats.icmp.rxDropped++;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpTxPackets()
{
    if( record == 1 )
    {
        stats.icmp.txPackets++;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpTxBytes( size_t bytes )
{
    if( record == 1 )
    {
        stats.icmp.txBytes += bytes;
    }
}

/*-----------------------------------------------------------*/

void RecordIcmpTxDroppedPackets()
{
    if( record == 1 )
    {
        stats.icmp.txDropped++;
    }
}

/*-----------------------------------------------------------*/

void UpdateRxLatency( uint64_t rtt )
{
    stats.rxLatency = (  stats.rxLatency + rtt  * 100 ) / 2 ;
}

/*-----------------------------------------------------------*/

void UpdateTxLatency( uint64_t rtt )
{
    stats.txLatency = (  stats.txLatency + rtt  * 100 ) / 2 ;
}

/*-----------------------------------------------------------*/

void GetCurrentCycleCount( uint32_t * cycleCount )
{
    *cycleCount = ARM_REG_DWT_CYCCNT;
}

/*-----------------------------------------------------------*/

uint32_t GetElapsedCycles( uint32_t start )
{
    uint32_t currentCycleCount = ARM_REG_DWT_CYCCNT;
    return currentCycleCount - start;
}

/*-----------------------------------------------------------*/
