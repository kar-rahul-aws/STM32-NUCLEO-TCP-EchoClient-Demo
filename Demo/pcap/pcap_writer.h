#ifndef PCAP_WRITER_H
#define PCAP_WRITER_H

/* Data types. */
#include "pcap_writer_data_types.h"

/**
 * @brief Initialize a pcap writer context.
 *
 * @param[in] p_ctx The context to initialize.
 * @param[in] snaplen Maximum length of each packet stored. Anything extra is truncated.
 * @param[in] link_type Network linktype. Only Ethernet is supported.
 * @param[in] capture_buf The buffer in which to store the pcap capture.
 * @param[in] capture_buf_len Length of capture_buf.
 *
 * @return PCAP_WRITER_SUCCESS is returned if the context is successfully initialized.
 * Otherwise an error code indicating the reason of the error is returned.
 */
pcap_writer_error_t pcap_writer_initialize( pcap_writer_context_t * p_ctx,
                                            uint32_t snaplen,
                                            uint32_t link_type,
                                            uint8_t * capture_buf,
                                            uint32_t capture_buf_len );

/*-----------------------------------------------------------*/

/**
 * @brief Write a packet.
 *
 * @param[in] p_ctx The initialized context.
 * @param[in] packet The packet to write.
 * @param[in] packet_len Length of the packet.
 * @param[in] p_timestamp Timestamp associated with the packet.
 *
 * @return PCAP_WRITER_SUCCESS is returned if the packet is successfully written.
 * Otherwise an error code indicating the reason of the error is returned.
 */
pcap_writer_error_t pcap_writer_write_packet( pcap_writer_context_t * p_ctx,
                                              const uint8_t * packet,
                                              uint32_t packet_len,
                                              const pcap_writer_timeval_t * p_timestamp );

/*-----------------------------------------------------------*/

/**
 * @brief Get the length of the capture.
 *
 * @param[in] p_ctx The initialized context.
 *
 * @return The length of the capture.
 */
uint32_t pcap_writer_get_capture_len( const pcap_writer_context_t * p_ctx );

/*-----------------------------------------------------------*/

/**
 * @brief Get the remaining length of the capture buffer.
 *
 * @param[in] p_ctx The initialized context.
 *
 * @return The remaining length of the capture buffer.
 */
uint32_t pcap_writer_get_remaining_len( const pcap_writer_context_t * p_ctx );

/*-----------------------------------------------------------*/

#endif /* #ifndef PCAP_WRITER_H */
