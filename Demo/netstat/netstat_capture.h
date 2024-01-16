/*
 * netstat_capture.h
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

#ifndef NETSTAT_CAPTURE_H
#define NETSTAT_CAPTURE_H


/* Standard includes. */
#include <stdint.h>
#include <stddef.h>

#define CLOCK_SPEED_HTZ                         ( ( uint64_t ) 64000000 )
#define CYCLE_COUNT_TO_MICRO_SECONDS( cycles )  ( ( ( uint64_t )( cycles ) * ( uint64_t ) 1000000 ) / CLOCK_SPEED_HTZ )

/*-----------------------------------------------------------*/

typedef enum NetstatResult
{
    NETSTAT_RESULT_OK,
    NETSTAT_RESULT_BAD_PARAM
} NetstatResult_t;

typedef struct ProtocolStats
{
    uint32_t rxPackets;
    uint32_t txPackets;
    uint32_t rxBytes;
    uint32_t txBytes;
    uint32_t rxDropped;
    uint32_t txDropped;
} ProtocolStats_t;

typedef struct NetworkStats
{
    ProtocolStats_t tcp;
    ProtocolStats_t udp;
    ProtocolStats_t icmp;
    uint64_t rxLatency;
    uint64_t txLatency;
} NetworkStats_t;

/*-----------------------------------------------------------*/

/**
 * @brief Obtain network stats.
 *
 * @param pStats Output parameter to return the network stats in.
 *
 * @return NETSTAT_RESULT_OK if successful, error code otherwise.
 */
NetstatResult_t Netstat_GetStats( NetworkStats_t * pStats );

/*-----------------------------------------------------------*/

void StartRecording( void );
void StopRecording( void );
void ResetStats( void );

void RecordUdpRxPackets();
void RecordUdpRxBytes( size_t bytes );
void RecordUdpRxDroppedPackets();
void RecordUdpTxPackets();
void RecordUdpTxBytes( size_t bytes );
void RecordUdpTxDroppedPackets();

void RecordTcpRxPackets();
void RecordTcpRxBytes( size_t bytes );
void RecordTcpRxDroppedPackets();
void RecordTcpTxPackets();
void RecordTcpTxBytes( size_t bytes );
void RecordTcpTxDroppedPackets();

void RecordIcmpRxPackets();
void RecordIcmpRxBytes( size_t bytes );
void RecordIcmpRxDroppedPackets();
void RecordIcmpTxPackets();
void RecordIcmpTxBytes( size_t bytes );
void RecordIcmpTxDroppedPackets();

void UpdateRxLatency( uint64_t rtt );
void UpdateTxLatency( uint64_t rtt );

void GetCurrentCycleCount( uint32_t * cycleCount );
uint32_t GetElapsedCycles( uint32_t start );

/*-----------------------------------------------------------*/

#define iptraceUDP_PACKET_RECEIVE( networkBuffer, result )                  \
    RecordUdpRxPackets();                                                   \
    RecordUdpRxBytes( ( networkBuffer )->xDataLength );                     \
    if( ( result ) == eReleaseBuffer )                                      \
    {                                                                       \
        RecordUdpRxDroppedPackets();                                        \
    }

#define iptraceTCP_PACKET_RECEIVE( networkBuffer, result )                  \
    RecordTcpRxPackets();                                                   \
    RecordTcpRxBytes( ( networkBuffer )->xDataLength );                     \
    if( ( result ) == eReleaseBuffer )                                      \
    {                                                                       \
        RecordTcpRxDroppedPackets();                                        \
    }

#define iptraceICMP_PACKET_RECEIVE( networkBuffer, result )                 \
    RecordIcmpRxPackets();                                                  \
    RecordIcmpRxBytes( ( networkBuffer )->xDataLength );                    \
    if( ( result ) == eReleaseBuffer )                                      \
    {                                                                       \
        RecordIcmpRxDroppedPackets();                                       \
    }

#define iptraceUDP_PACKET_SEND( networkBuffer )                             \
    RecordUdpTxPackets();                                                   \
    RecordUdpTxBytes( ( networkBuffer )->xDataLength );

#define iptraceUDP_TX_PACKET_DROP( networkBuffer )                          \
    RecordUdpTxDroppedPackets();

#define iptraceTCP_PACKET_SEND( packetLength, sentLength )                  \
    RecordTcpTxPackets();                                                   \
    RecordTcpTxBytes( ( packetLength ) );                                   \
    if( ( sentLength ) < 0 )                                                \
    {                                                                       \
        RecordTcpTxDroppedPackets();                                        \
    }

#define iptraceICMP_PACKET_SEND( packetLength, result )                     \
    RecordIcmpTxPackets();                                                  \
    RecordIcmpTxBytes( ( packetLength ) );                                  \
    if( ( result ) == pdFAIL )                                              \
    {                                                                       \
        RecordIcmpTxDroppedPackets();                                       \
    }

#define iptracePACKET_RECEIVE_START()                                       \
    uint32_t packetReceiveStart;                                            \
    GetCurrentCycleCount( &( packetReceiveStart ) );

#define iptracePACKET_RECEIVE_END()                                         \
    uint32_t packetReceiveCycles;                                           \
    packetReceiveCycles = GetElapsedCycles( packetReceiveStart );           \
    UpdateRxLatency( CYCLE_COUNT_TO_MICRO_SECONDS( packetReceiveCycles) );

#define iptraceSTACK_PACKET_SEND_START()                                    \
    uint32_t stackPacketSendStart;                                          \
    GetCurrentCycleCount( &( stackPacketSendStart ) );

#define iptraceSTACK_PACKET_SEND_END()                                      \
    uint32_t stackPacketSendCycles;                                         \
    stackPacketSendCycles = GetElapsedCycles( stackPacketSendStart );       \
    UpdateTxLatency( CYCLE_COUNT_TO_MICRO_SECONDS( stackPacketSendCycles) );

#define iptracePING_SEND_START()                                            \
    uint32_t pingSendStart;                                                 \
    GetCurrentCycleCount( &( pingSendStart ) );

#define iptracePING_SEND_END()                                              \
    uint32_t pingSendCycles;                                                \
    pingSendCycles = GetElapsedCycles( pingSendStart );                     \
    UpdateTxLatency( CYCLE_COUNT_TO_MICRO_SECONDS( pingSendCycles) );

#define iptraceTCP_SEND_START()                                             \
    uint32_t tcpSendStart;                                                  \
    GetCurrentCycleCount( &( tcpSendStart ) );

#define iptraceTCP_SEND_END()                                               \
    uint32_t tcpSendCycles;                                                 \
    tcpSendCycles = GetElapsedCycles( tcpSendStart );                       \
    UpdateTxLatency( CYCLE_COUNT_TO_MICRO_SECONDS( tcpSendCycles) );


#define iptraceIP_TASK_STARTING()                                           \
    StartRecording();
/*-----------------------------------------------------------*/

#endif /* NETSTAT_CAPTURE_H */
