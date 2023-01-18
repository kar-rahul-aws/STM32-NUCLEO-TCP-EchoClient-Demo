/*
 * netstat_capture.h
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

#ifndef NETSTAT_NETSTAT_CAPTURE_H
#define NETSTAT_NETSTAT_CAPTURE_H


/* Standard includes. */
#include <stdint.h>

/**
 * @brief For Demo purpose.
 */

typedef struct network_stats
{
    uint32_t pckt_rx;
    uint32_t pckt_tx;
    uint32_t pcket_drop_rx;
    uint32_t pcket_drop_tx;
    uint32_t bytes_rx;
    uint32_t bytes_tx;
} network_stats_t;

typedef struct tcp_stats
{
    network_stats_t unicast_stats;
    network_stats_t multicast_stats;
    uint32_t out_ack;
    uint32_t in_ack;
} tcp_stats_t;

typedef struct icmp_stats
{
    network_stats_t unicast_stats;
    network_stats_t multicast_stats;
} icmp_stats_t;

typedef struct udp_stats
{
    network_stats_t unicast_stats;
    network_stats_t multicast_stats;
} udp_stats_t;

typedef struct all_stats
{
    tcp_stats_t tcp_stats;
    udp_stats_t udp_stats;
    icmp_stats_t icmp_stats;
    uint64_t rx_latency;
    uint64_t tx_latency;
} all_stats_t;

/**
 * @brief Get all network stats.
 */
void netstat_get_all_stats( all_stats_t * p_all_stats );

#endif /* NETSTAT_NETSTAT_CAPTURE_H_ */
