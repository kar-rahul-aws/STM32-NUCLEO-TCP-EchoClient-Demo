/* Standard includes. */
#include <string.h>

/* Interface includes. */
#include "pcap_capture.h"

/* PCAP writer includes. */
#include "pcap_writer.h"

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

/**
 * @brief Pcap writer context.
 */
static pcap_writer_context_t pcap_writer_ctx;

/**
 * @brief Buffer which contains all the captured packets.
 */
static uint8_t capture_buffer[ configPCAP_CAPTURE_BUFFER_LENGTH ];

/*-----------------------------------------------------------*/

void pcap_capture_start( void )
{
    pcap_writer_error_t ret_val;

    /* Start fresh. */
    memset( &( capture_buffer[ 0 ] ), 0, configPCAP_CAPTURE_BUFFER_LENGTH );

    ret_val = pcap_writer_initialize( &( pcap_writer_ctx ),
                                      configPCAP_CAPTURE_PACKET_SNAPLEN,
                                      PCAP_WRITER_LINKTYPE_ETHERNET,
                                      &( capture_buffer[ 0 ] ),
                                      configPCAP_CAPTURE_BUFFER_LENGTH );
    configASSERT( ret_val == PCAP_WRITER_SUCCESS );

    /* Record that the capture is now running. */
    capture_running = 1;
}

/*-----------------------------------------------------------*/

void pcap_capture_stop( void )
{
    capture_running = 0;
}

/*-----------------------------------------------------------*/

void pcap_capture_get_captured_data( const uint8_t ** pp_captured_data,
                                     uint32_t * p_captured_data_length )
{
    *pp_captured_data = &( capture_buffer[ 0 ] );
    *p_captured_data_length = pcap_writer_get_capture_len( &( pcap_writer_ctx ) );
}

/*-----------------------------------------------------------*/

void pcap_capture_record_packet( const uint8_t * p_packet, uint32_t packet_length )
{
    uint32_t milliseconds;
    pcap_writer_timeval_t timestamp;

    if( capture_running == 1 )
    {
        milliseconds = HAL_GetTick();
        timestamp.tv_sec = milliseconds / 1000;
        timestamp.tv_usec = milliseconds % 1000;

        /* Ignoring OOM error for now. */
        pcap_writer_write_packet( &( pcap_writer_ctx ),
                                  p_packet,
                                  packet_length,
                                  &( timestamp ) );
    }
}

/*-----------------------------------------------------------*/
