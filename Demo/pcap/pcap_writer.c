/* Standard includes. */
#include <string.h>
#include <assert.h>

/* Interface includes. */
#include "pcap_writer.h"

/**
 * Values used in pcap file header.
*/
#define PCAP_WRITER_TCPDUMP_MAGIC       0xa1b2c3d4
#define PCAP_WRITER_PCAP_VERSION_MAJOR  2
#define PCAP_WRITER_PCAP_VERSION_MINOR  4

/*-----------------------------------------------------------*/

/**
 * @brief Represents pcap file header present at the beginning of the file.
 */
typedef struct pcap_writer_pcap_file_header {
    uint32_t magic;             /**< Helps the reader determine the endianess. */
    uint16_t version_major;     /**< Major version number of pcap file format. */
    uint16_t version_minor;     /**< Minor version number of pcap file format. */
    int32_t this_zone;          /**< 4-byte time zone offset; this is always 0. */
    uint32_t sigfigs;           /**< 4-byte number giving the accuracy of time stamps
                                     in the file; this is always 0. */
    uint32_t snaplen;           /**< Maximum length of each packet stored. Anything extra is truncated. */
    uint32_t link_type;         /**< Network linktype. Only Ethernet is supported. */
} pcap_writer_pcap_file_header_t;

/**
 * @brief Represents packet header present at the beginning of each packet.
 */
typedef struct pcap_writer_packet_header {
    pcap_writer_timeval_t ts;   /**< Timestamp associated with the packet. */
    uint32_t caplen;            /**< Length of the packet actually captured. */
    uint32_t len;               /**< Length of the packet. */
} pcap_writer_packet_header_t;

/*-----------------------------------------------------------*/

/**
 * @brief Write data to the capture buffer.
 *
 * @param[in] p_ctx The initialized context.
 * @param[in] data The data to write.
 * @param[in] data_len The length of the data.
 *
 * @return PCAP_WRITER_SUCCESS is returned if data is successfully written.
 * Otherwise an error code indicating the reason of the error is returned.
 */
static pcap_writer_error_t pcap_writer_write_data( pcap_writer_context_t * p_ctx,
                                                   const uint8_t * data,
                                                   uint32_t data_len );

/*-----------------------------------------------------------*/

static pcap_writer_error_t pcap_writer_write_data( pcap_writer_context_t * p_ctx,
                                                   const uint8_t * data,
                                                   uint32_t data_len )
{
    pcap_writer_error_t ret_val = PCAP_WRITER_SUCCESS;

    if( data_len > p_ctx->remaining_len )
    {
        ret_val = PCAP_WRITER_ERROR_OUT_OF_MEMORY;
    }
    else
    {
        memcpy( p_ctx->cur, data, data_len );
        p_ctx->cur = p_ctx->cur + data_len;
        p_ctx->remaining_len = p_ctx->remaining_len - data_len;
    }

    return ret_val;
}

/*-----------------------------------------------------------*/

pcap_writer_error_t pcap_writer_initialize( pcap_writer_context_t * p_ctx,
                                            uint32_t snaplen,
                                            uint32_t link_type,
                                            uint8_t * capture_buf,
                                            uint32_t capture_buf_len )
{
    pcap_writer_error_t ret_val = PCAP_WRITER_SUCCESS;
    pcap_writer_pcap_file_header_t file_header;

    assert( link_type == PCAP_WRITER_LINKTYPE_ETHERNET );
    assert( capture_buf != NULL );
    assert( capture_buf_len > sizeof( pcap_writer_pcap_file_header_t ) );

    p_ctx->capture_buf = capture_buf;
    p_ctx->capture_buf_len = capture_buf_len;
    p_ctx->cur = capture_buf;
    p_ctx->remaining_len = capture_buf_len;
    p_ctx->snaplen = snaplen;

    file_header.magic = PCAP_WRITER_TCPDUMP_MAGIC;
    file_header.version_major = PCAP_WRITER_PCAP_VERSION_MAJOR;
    file_header.version_minor = PCAP_WRITER_PCAP_VERSION_MINOR;
    file_header.this_zone = 0;
    file_header.sigfigs = 0;
    file_header.snaplen = snaplen;
    file_header.link_type = link_type;

    ret_val = pcap_writer_write_data( p_ctx,
                                      ( const uint8_t * ) &( file_header ),
                                      ( uint32_t ) sizeof( file_header ) );

    return ret_val;
}

/*-----------------------------------------------------------*/

pcap_writer_error_t pcap_writer_write_packet( pcap_writer_context_t * p_ctx,
                                              const uint8_t * packet,
                                              uint32_t packet_len,
                                              const pcap_writer_timeval_t * p_timestamp )
{
    pcap_writer_error_t ret_val = PCAP_WRITER_SUCCESS;
    pcap_writer_packet_header_t packet_header;

    packet_header.ts.tv_sec = p_timestamp->tv_sec;
    packet_header.ts.tv_usec = p_timestamp->tv_usec;
    packet_header.caplen = packet_len;
    packet_header.len = packet_len;

    ret_val = pcap_writer_write_data( p_ctx,
                                      ( const uint8_t * ) &( packet_header ),
                                      ( uint32_t ) sizeof( packet_header ) );

    if( ret_val == PCAP_WRITER_SUCCESS )
    {
        ret_val = pcap_writer_write_data( p_ctx,
                                          packet,
                                          packet_len <= p_ctx->snaplen ? packet_len : p_ctx->snaplen );
    }

    return ret_val;
}

/*-----------------------------------------------------------*/

uint32_t pcap_writer_get_capture_len( const pcap_writer_context_t * p_ctx )
{
    return ( p_ctx->capture_buf_len - p_ctx->remaining_len );
}

/*-----------------------------------------------------------*/

uint32_t pcap_writer_get_remaining_len( const pcap_writer_context_t * p_ctx )
{
    return p_ctx->remaining_len;
}

/*-----------------------------------------------------------*/
