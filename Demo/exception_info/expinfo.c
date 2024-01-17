/* Standard includes. */
#include <stdbool.h>
#include <stdint.h>

/* Interface includes. */
#include "flash_write.h"
#include "expinfo.h"

/*-----------------------------------------------------------*/

#define FLASH_USER_FILE_EXIST_MAGIC     ( 0xAABBAABB )
#define EXCEPTION_INFO_MEMORY_REGIONS   ( 2 )

/*-----------------------------------------------------------*/

/* The exception information is organized in the following blocks:
 *
 * +--------+----------------+----------+---------+------------+
 * | Header | Registers Dump | Data RAM | BSS RAM | End Marker |
 * +--------+----------------+----------+---------+------------+
 *
 * 1. Header - Contains the information about the remaining blocks.
 * 2. Register Dump - Values contained in all the registers at the time of
 *    exception.
 * 3. Data RAM - The content of data RAM at the time of exception.
 * 4. BSS RAM - The content of the BSS RAM at the time of exception.
 * 5. End Marker - The presence of this marker indicates that a complete
 *    exception information is present.
 *
 * Each of the above block is padded to ensure that its size is a multiple of
 * 32 bytes.
 */

typedef struct MemoryRegion
{
    uint32_t uxOffset;      /* Offset from the start of the exception information. */
    uint32_t uxLength;      /* Length in bytes. */
    uint32_t uxEndAddress;  /* Last address in the memory region. */
} MemoryRegion_t;

typedef struct ExceptionHeader
{
    uint32_t uxStartMarker;     /* Tells the presence of a valid exception information. */
    uint32_t uxTotalLength;     /* Total length of all 5 blocks in bytes. */

    uint32_t uxRegDumpOffset;   /* Offset of the starting of Register Dump block. */
    uint32_t uxRegDumpLength;   /* Length of the Register Dump block. */

    uint32_t uxMemoryRegionNum; /* Number of memory regions - currently 2, Data and BSS. */
    MemoryRegion_t xMemoryRegions[ EXCEPTION_INFO_MEMORY_REGIONS ];
} ExceptionHeader_t;

typedef struct CoreRegisters
{
    uint32_t ulRegR0;
    uint32_t ulRegR1;
    uint32_t ulRegR2;
    uint32_t ulRegR3;
    uint32_t ulRegR4;
    uint32_t ulRegR5;
    uint32_t ulRegR6;
    uint32_t ulRegR7;
    uint32_t ulRegR8;
    uint32_t ulRegR9;
    uint32_t ulRegR10;
    uint32_t ulRegR11;
    uint32_t ulRegR12;
    uint32_t ulRegSp;
    uint32_t ulRegLr;
    uint32_t ulRegPc;
    uint32_t ulRegXpsr;
    uint32_t ulRegD0;
    uint32_t ulRegD1;
    uint32_t ulRegD2;
    uint32_t ulRegD3;
    uint32_t ulRegD4;
    uint32_t ulRegD5;
    uint32_t ulRegD6;
    uint32_t ulRegD7;
    uint32_t ulRegD8;
    uint32_t ulRegD9;
    uint32_t ulRegD10;
    uint32_t ulRegD11;
    uint32_t ulRegD12;
    uint32_t ulRegD13;
    uint32_t ulRegD14;
    uint32_t ulRegD15;
    uint32_t ulRegFpscr;
    uint32_t ulRegPrimask;
    uint32_t ulRegBasepri;
    uint32_t ulRegFaultmask;
    uint32_t ulRegControl;
    uint32_t ulRegMsp;
    uint32_t ulRegPsp;
} CoreRegisters_t;

/*-----------------------------------------------------------*/

static ExceptionHeader_t gExceptionHeader;
static CoreRegisters_t gCoreRegisters;
static const uint32_t endMarker[ 8 ] = { FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC,
                                         FLASH_USER_FILE_EXIST_MAGIC };

/*-----------------------------------------------------------*/

static void ReadRegisters( CoreRegisters_t * pRegs )
{
    register uint32_t regVal;

    register int regR0 asm("r0");
    register int regR1 asm("r1");
    register int regR2 asm("r2");
    register int regR3 asm("r3");
    register int regR4 asm("r4");
    register int regR5 asm("r5");
    register int regR6 asm("r6");
    register int regR7 asm("r7");
    register int regR8 asm("r8");
    register int regR9 asm("r9");
    register int regR10 asm("r10");
    register int regR11 asm("r11");
    register int regR12 asm("r12");
    register int regSp asm("sp");
    register int regLr asm("lr");
    register int regD0 asm("d0");
    register int regD1 asm("d1");
    register int regD2 asm("d2");
    register int regD3 asm("d3");
    register int regD4 asm("d4");
    register int regD5 asm("d5");
    register int regD6 asm("d6");
    register int regD7 asm("d7");
    register int regD8 asm("d8");
    register int regD9 asm("d9");
    register int regD10 asm("d10");
    register int regD11 asm("d11");
    register int regD12 asm("d12");
    register int regD13 asm("d13");
    register int regD14 asm("d14");
    register int regD15 asm("d15");

    pRegs->ulRegR0 = regR0;
    pRegs->ulRegR1 = regR1;
    pRegs->ulRegR2 = regR2;
    pRegs->ulRegR3 = regR3;
    pRegs->ulRegR4 = regR4;
    pRegs->ulRegR5 = regR5;
    pRegs->ulRegR6 = regR6;
    pRegs->ulRegR7 = regR7;
    pRegs->ulRegR8 = regR8;
    pRegs->ulRegR9 = regR9;
    pRegs->ulRegR10 = regR10;
    pRegs->ulRegR11 = regR11;
    pRegs->ulRegR12 = regR12;
    pRegs->ulRegSp = regSp;
    pRegs->ulRegLr = regLr;
    pRegs->ulRegD0 = regD0;
    pRegs->ulRegD1 = regD1;
    pRegs->ulRegD2 = regD2;
    pRegs->ulRegD3 = regD3;
    pRegs->ulRegD4 = regD4;
    pRegs->ulRegD5 = regD5;
    pRegs->ulRegD6 = regD6;
    pRegs->ulRegD7 = regD7;
    pRegs->ulRegD8 = regD8;
    pRegs->ulRegD9 = regD9;
    pRegs->ulRegD10 = regD10;
    pRegs->ulRegD11 = regD11;
    pRegs->ulRegD12 = regD12;
    pRegs->ulRegD13 = regD13;
    pRegs->ulRegD14 = regD14;
    pRegs->ulRegD15 = regD15;

    __asm volatile( "mov %0, pc" : "=r" ( regVal ) );
    pRegs->ulRegPc = regVal;

    pRegs->ulRegXpsr = __get_xPSR();
    pRegs->ulRegFpscr = __get_FPSCR();
    pRegs->ulRegPrimask = __get_PRIMASK();
    pRegs->ulRegBasepri = __get_BASEPRI();
    pRegs->ulRegFaultmask = __get_FAULTMASK();
    pRegs->ulRegControl = __get_CONTROL();
    pRegs->ulRegMsp = __get_MSP();
    pRegs->ulRegPsp = __get_PSP();
}

/*-----------------------------------------------------------*/

BaseType_t ExpInfo_StoreInfo( void )
{
    uint32_t uxNextFlashAddress;
    uint32_t result;

    extern unsigned char _sdata;
    extern unsigned char _edata;
    extern unsigned char _sbss;
    extern unsigned char _ebss;

    /* Store the current registers in in gCoreRegisters. */
    ReadRegisters( &( gCoreRegisters ) );

    /* Store the header information. */
    gExceptionHeader.uxStartMarker = FLASH_USER_FILE_EXIST_MAGIC;
    gExceptionHeader.uxTotalLength = ROUND_UP_32( sizeof( ExceptionHeader_t ) ) +
                                     ROUND_UP_32( sizeof( CoreRegisters_t ) ) +
                                     ROUND_UP_32( ( uint32_t )( &_edata - &_sdata ) ) +
                                     ROUND_UP_32( ( uint32_t )( &_ebss - &_sbss ) ) +
                                     ROUND_UP_32( sizeof( endMarker ) );

    gExceptionHeader.uxRegDumpOffset = ROUND_UP_32( sizeof( ExceptionHeader_t ) );
    gExceptionHeader.uxRegDumpLength = sizeof( CoreRegisters_t );

    gExceptionHeader.uxMemoryRegionNum = EXCEPTION_INFO_MEMORY_REGIONS;

    /* .data section. */
    gExceptionHeader.xMemoryRegions[ 0 ].uxOffset = ROUND_UP_32( sizeof( ExceptionHeader_t ) ) +
                                                  ROUND_UP_32( sizeof( CoreRegisters_t ) );
    gExceptionHeader.xMemoryRegions[ 0 ].uxLength = ( uint32_t )( &_edata - &_sdata );
    gExceptionHeader.xMemoryRegions[ 0 ].uxEndAddress = ( uint32_t )( &_sdata );

    /* .bss section. */
    gExceptionHeader.xMemoryRegions[ 1 ].uxOffset = ROUND_UP_32( sizeof( ExceptionHeader_t ) ) +
                                                  ROUND_UP_32( sizeof( CoreRegisters_t ) ) +
                                                  ROUND_UP_32( ( uint32_t )( &_edata - &_sdata ) );
    gExceptionHeader.xMemoryRegions[ 1 ].uxLength = ( uint32_t )( &_ebss - &_sbss );
    gExceptionHeader.xMemoryRegions[ 1 ].uxEndAddress = ( uint32_t )( &_sbss );

    /* Store exception header. */
    result = FLASH_Write( EXCEPTION_INFO_START_ADDR,
                          ( uint32_t * )( &( gExceptionHeader ) ),
                          BYTES_TO_WORDS( sizeof( gExceptionHeader ) ),
                          &( uxNextFlashAddress ) );

    /* Store the registers. */
    if( result == FLASH_OPERATION_OK )
    {
        result = FLASH_Write( uxNextFlashAddress,
                              ( uint32_t * )( &( gCoreRegisters ) ),
                              BYTES_TO_WORDS( sizeof( CoreRegisters_t ) ),
                              &( uxNextFlashAddress ) );
    }

    /* Store the data section memory. */
    if( result == FLASH_OPERATION_OK )
    {
        result = FLASH_Write( uxNextFlashAddress,
                              ( uint32_t * )( &_sdata ),
                              BYTES_TO_WORDS( ( uint32_t )( &_edata - &_sdata ) ),
                              &( uxNextFlashAddress ) );
    }

    /* Store the bss section memory. */
    if( result == FLASH_OPERATION_OK )
    {
        result = FLASH_Write( uxNextFlashAddress,
                              ( uint32_t * )( &_sbss ),
                              BYTES_TO_WORDS( ( uint32_t )( &_ebss - &_sbss ) ),
                              &( uxNextFlashAddress ) );
    }

    /* Store the end marker. */
    if( result == FLASH_OPERATION_OK )
    {
        result = FLASH_Write( uxNextFlashAddress,
                              ( uint32_t * )endMarker,
                              BYTES_TO_WORDS( sizeof( endMarker ) ),
                              &( uxNextFlashAddress ) );
    }

    return ( result == FLASH_OPERATION_OK ) ? pdTRUE : pdFALSE;
}

/*-----------------------------------------------------------*/

void ExpInfo_CleanInfo( void )
{
    FLASH_Erase();
}

/*-----------------------------------------------------------*/

BaseType_t ExpInfo_InfoExist( void )
{
    BaseType_t xReturn = pdTRUE;
    ExceptionHeader_t * pExceptionHeader = ( ExceptionHeader_t * )( EXCEPTION_INFO_START_ADDR );
    uint32_t * puxExceptionInfoImage = ( uint32_t * )( EXCEPTION_INFO_START_ADDR );

    /* Check the start marker. */
    if( pExceptionHeader->uxStartMarker != FLASH_USER_FILE_EXIST_MAGIC )
    {
        xReturn = pdFALSE;
    }

    /* Check the end marker. */
    if( xReturn != pdFALSE )
    {
        if( puxExceptionInfoImage[ ( pExceptionHeader->uxTotalLength / 4 ) - 1 ] != FLASH_USER_FILE_EXIST_MAGIC )
        {
            xReturn = pdFALSE;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t ExpInfo_GetInfo( const uint8_t ** pxExceptionInfo,
                            uint32_t * pxExceptionInfoLength )
{
    BaseType_t xReturn;
    ExceptionHeader_t * pExceptionHeader = ( ExceptionHeader_t * )( EXCEPTION_INFO_START_ADDR );

    if( ( pxExceptionInfo == NULL ) || ( pxExceptionInfoLength == NULL ) )
    {
        xReturn = pdFALSE;
    }
    else
    {
        xReturn = ExpInfo_InfoExist();

        if( xReturn == pdTRUE )
        {
            *pxExceptionInfo = ( const uint8_t * )EXCEPTION_INFO_START_ADDR;
            *pxExceptionInfoLength = pExceptionHeader->uxTotalLength;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/
