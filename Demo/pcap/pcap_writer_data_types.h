#ifndef PCAP_WRITER_DATA_TYPES_H
#define PCAP_WRITER_DATA_TYPES_H

/* Standard includes. */
#include <stdint.h>

/**
 * @brief Same as LINKTYPE_ETHERNET and DLT_EN10MB from libpcap.
 *
 * Need to define here as we do not depend on libpcap.
 */
#define PCAP_WRITER_LINKTYPE_ETHERNET       1

/*-----------------------------------------------------------*/

/**
 * @brief Error codes returned from public APIs.
 */
typedef enum pcap_writer_error
{
    PCAP_WRITER_SUCCESS = 0,
    PCAP_WRITER_ERROR_OUT_OF_MEMORY
} pcap_writer_error_t;

/*-----------------------------------------------------------*/

/**
 * @brief Context for pcap writer.
 *
 * Must be initialized using pcap_writer_initialize and passed
 * to all other APIs.
 */
typedef struct pcap_writer_context
{
    uint8_t * capture_buf;      /**< Buffer in which the pcap capture is stored. */
    uint32_t capture_buf_len;   /**< Length of capture_buf. */
    uint8_t * cur;              /**< Next location to write in capture_buf. */
    uint32_t remaining_len;     /**< Remaining length in capture_buf. */
    uint32_t snaplen;           /**< Maximum length of each packet stored. Anything extra is truncated. */
} pcap_writer_context_t;

/*-----------------------------------------------------------*/

/**
 * @brief Represents a timestamp.
 */
typedef struct pcap_writer_timeval {
    int32_t tv_sec;
    int32_t tv_usec;
} pcap_writer_timeval_t;

/*-----------------------------------------------------------*/

#endif /* #ifndef PCAP_WRITER_DATA_TYPES_H */
