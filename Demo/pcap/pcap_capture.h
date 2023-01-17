#ifndef PCAP_CAPTURE_H
#define PCAP_CAPTURE_H

/* Standard includes. */
#include <stdint.h>

/**
 * @brief Start capturing packets.
 */
void pcap_capture_start( void );

/**
 * @brief Stop capturing packets.
 */
void pcap_capture_stop( void );

/**
 * @brief Get the captured data.
 *
 * @param[out] pp_captured_data Out parameter to return captured data.
 * @param[out] p_captured_data_length Out parameter to return captured data length.
 */
void pcap_capture_get_captured_data( const uint8_t ** pp_captured_data,
                                     uint32_t * p_captured_data_length );

/**
 * @brief Capture a packet.
 *
 * @param p_packet Packet to capture.
 * @param packet_length Length of the packet.
 */
void pcap_capture_record_packet( const uint8_t * p_packet, uint32_t packet_length );

#endif /* #ifndef PCAP_CAPTURE_H */
