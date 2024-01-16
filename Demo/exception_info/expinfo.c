#include <stdbool.h>
#include <stdint.h>

#include "platform_media.h"
#include "cortex_m_regs.h"
#include "expinfo.h"

/*-----------------------------------------------------------*/

#define FLASH_USER_FILE_EXIST_MAGIC     ( 0xAABBAABB )
#define EXCEPTION_INFO_MEMORY_REGIONS   ( 2 )

/*-----------------------------------------------------------*/

typedef struct MemoryRegion
{
    uint32_t uxOffset;
    uint32_t uxLength;
    uint32_t uxAddress;
} MemoryRegion_t;


typedef struct ExceptionInfo
{
    uint32_t uxFileExistMagic;
    uint32_t uxTotalLength;
    
    uint32_t uxRegDumpOffset;
    uint32_t uxRegDumpLength;
    
    uint32_t uxMemoryRegionNum;
    MemoryRegion_t xMemoryRegions[ EXCEPTION_INFO_MEMORY_REGIONS ];
} ExceptionInfo_t;

/*-----------------------------------------------------------*/

BaseType_t ExpInfo_StoreInfo( void )
{
    static ExceptionInfo_t gExceptionInfo;
    static CoreReg_t gCoreReg;
    static uint32_t uxNextFlashAddress;
    static const uint32_t puxEndMagic[ 4 ] = { FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC };
    static uint32_t uxRetMedia;

    extern unsigned char _sdata;
    extern unsigned char _edata;
    extern unsigned char _sbss;
    extern unsigned char _ebss;

    /* Store the current regin in gCoreReg. */
    EXPINFO_CAPTURE_REGISTERS( gCoreReg );

    /* Store the header information. */
    gExceptionInfo.uxFileExistMagic = FLASH_USER_FILE_EXIST_MAGIC;
    gExceptionInfo.uxTotalLength = PLATFORM_ROUND_SIZE( sizeof( ExceptionInfo_t ) ) +
                                   PLATFORM_ROUND_SIZE( sizeof( CoreReg_t ) ) +
                                   PLATFORM_ROUND_SIZE( ( uint32_t )( &_edata - &_sdata ) ) +
                                   PLATFORM_ROUND_SIZE( ( uint32_t )( &_ebss - &_sbss ) ) +
                                   PLATFORM_ROUND_SIZE( 4 );

    gExceptionInfo.uxRegDumpOffset = PLATFORM_ROUND_SIZE( sizeof( ExceptionInfo_t ) );
    gExceptionInfo.uxRegDumpLength = sizeof( CoreReg_t );

    gExceptionInfo.uxMemoryRegionNum = EXCEPTION_INFO_MEMORY_REGIONS;

    /* .data section. */
    gExceptionInfo.xMemoryRegions[ 0 ].uxOffset = PLATFORM_ROUND_SIZE( sizeof( ExceptionInfo_t ) ) +
                                                  PLATFORM_ROUND_SIZE( sizeof( CoreReg_t ) );
    gExceptionInfo.xMemoryRegions[ 0 ].uxLength = ( uint32_t )( &_edata - &_sdata );
    gExceptionInfo.xMemoryRegions[ 0 ].uxAddress = ( uint32_t )( &_sdata );

    /* .bss section. */
    gExceptionInfo.xMemoryRegions[ 1 ].uxOffset = PLATFORM_ROUND_SIZE( sizeof( ExceptionInfo_t ) ) +
                                                  PLATFORM_ROUND_SIZE( sizeof( CoreReg_t ) ) +
                                                  PLATFORM_ROUND_SIZE( ( uint32_t )( &_edata - &_sdata ) );
    gExceptionInfo.xMemoryRegions[ 1 ].uxLength = ( uint32_t )( &_ebss - &_sbss );
    gExceptionInfo.xMemoryRegions[ 1 ].uxAddress = ( uint32_t )( &_sbss );

    uxRetMedia = PLATFORM_WRITE_MEDIA( PLATFORM_EXCEPTION_INFO_START_ADDRESS, ( uint32_t * )&gExceptionInfo, sizeof( gExceptionInfo ), &uxNextFlashAddress );

    /* Store the registers. */
    if( uxRetMedia == PLATFORM_MEDIA_OK )
    {
        uxRetMedia = PLATFORM_WRITE_MEDIA( uxNextFlashAddress, ( uint32_t * )&gCoreReg, sizeof( CoreReg_t ), &uxNextFlashAddress );
    }

    /* Store the data section memory. */
    if( uxRetMedia == PLATFORM_MEDIA_OK )
    {
        uxRetMedia = PLATFORM_WRITE_MEDIA( uxNextFlashAddress, ( uint32_t * )&_sdata, ( uint32_t )( &_edata - &_sdata ), &uxNextFlashAddress );
    }

    /* Store the bss section memory. */
    if( uxRetMedia == PLATFORM_MEDIA_OK )
    {
        uxRetMedia = PLATFORM_WRITE_MEDIA( uxNextFlashAddress, ( uint32_t * )&_sbss, ( uint32_t )( &_ebss - &_sbss ), &uxNextFlashAddress );
    }

    /* Store the magic end. */
    if( uxRetMedia == PLATFORM_MEDIA_OK )
    {
        uxRetMedia = PLATFORM_WRITE_MEDIA( uxNextFlashAddress, ( uint32_t * )puxEndMagic, 32, &uxNextFlashAddress );
    }

    return ( uxRetMedia == PLATFORM_MEDIA_OK ) ? pdTRUE: pdFALSE;
}
/*-----------------------------------------------------------*/

void ExpInfo_CleanInfo( void )
{
    /* Erase the whole bank of PLATFORM_EXCEPTION_INFO_START_ADDRESS start from sector 0. */
    PLATFORM_EXCEPTION_INFO_CLEAN();
}
/*-----------------------------------------------------------*/

BaseType_t ExpInfo_InfoExist( void )
{
    BaseType_t xReturn = pdTRUE;
    ExceptionInfo_t * pExceptionInfo = ( ExceptionInfo_t * )( PLATFORM_EXCEPTION_INFO_START_ADDRESS );
    uint32_t * puxImagePtr = ( uint32_t *  )( PLATFORM_EXCEPTION_INFO_START_ADDRESS );

    /* Check the start magic number of the sector. */
    if( pExceptionInfo->uxFileExistMagic != FLASH_USER_FILE_EXIST_MAGIC )
    {
        xReturn = pdFALSE;
    }

    /* Check the end magic number of the sector. */
    if( xReturn != pdFALSE )
    {
        if( puxImagePtr[ ( pExceptionInfo->uxTotalLength / 4 ) - 1 ] != FLASH_USER_FILE_EXIST_MAGIC )
        {
            xReturn = pdFALSE;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/
