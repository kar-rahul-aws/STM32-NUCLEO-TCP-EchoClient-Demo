/*
 * netstat_capture.h
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

#ifndef NETSTAT_NETSTAT_CAPTURE_H_
#define NETSTAT_NETSTAT_CAPTURE_H_


/* Standard includes. */
#include <stdint.h>

/**
 * @brief For Demo purpose
 */
/*NetStat static structures - FreeRTOS+TCP*/

typedef enum
{
    eStartStat = 0,
    eStopStat,
    eGetStat
} eState_t;

typedef enum
{
    eDataStat = 0,
    ePerfState,
    eAll
} eStatVal_t;

typedef enum
{
    eUDP = 0,
    eTCP,
    eICMP,
    eDefault
} eProtocol_t;


typedef struct network_stats
{
    uint32_t pckt_rx;
    uint32_t pckt_tx;
    uint32_t pcket_drop_rx;
    uint32_t pcket_drop_tx;
    uint32_t bytes_rx;
    uint32_t bytes_tx;
}stats;


typedef struct TCP_stats
{
    stats unicast_stat;
    stats multicast_stat;
    uint32_t out_ack;
    uint32_t in_ack;
}tcp;

typedef struct ICMP_stats
{
    stats unicast_stat;
    stats multicast_stat;
}icmp;

typedef struct UDP_stats
{
    stats unicast_stat;
    stats multicast_stat;
}udp;

typedef struct all_stats
{
    tcp tcp_stat;
    udp udp_stat;
    icmp icmp_stat;
    uint64_t rx_latency;
    uint64_t tx_latency;
}allstat;




/*NetStat static structures - FreeRTOS+TCP*/

/**
 * @brief Start capturing packets.
 */
void netstat_capture_start(void);

/**
 * @brief Stop capturing packets.
 */
void netstat_capture_stop(void);


/**
 * @brief Capture a packet.
 *
 * @param
 *
 */
void netstat_capture_record_packet(void);


#endif /* NETSTAT_NETSTAT_CAPTURE_H_ */
