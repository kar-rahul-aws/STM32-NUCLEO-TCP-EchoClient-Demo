#include <stdbool.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"
#include "cortex_m_regs.h"
#include "expinfo.h"

/*-----------------------------------------------------------*/

/* Use bank 2 for the exception information. */
#define APPLICATION_ADDRESS         ADDR_FLASH_SECTOR_0_BANK2
#define USER_FLASH_END_ADDRESS      FLASH_END_ADDR

#define FLASH_BASE_ADDR      (uint32_t)(FLASH_BASE)
#define FLASH_END_ADDR       (uint32_t)(0x081FFFFF)

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) /* Base @ of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08020000) /* Base @ of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08040000) /* Base @ of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08060000) /* Base @ of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08080000) /* Base @ of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x080A0000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x080C0000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x080E0000) /* Base @ of Sector 7, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_0_BANK2     ((uint32_t)0x08100000) /* Base @ of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1_BANK2     ((uint32_t)0x08120000) /* Base @ of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2_BANK2     ((uint32_t)0x08140000) /* Base @ of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3_BANK2     ((uint32_t)0x08160000) /* Base @ of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4_BANK2     ((uint32_t)0x08180000) /* Base @ of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5_BANK2     ((uint32_t)0x081A0000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6_BANK2     ((uint32_t)0x081C0000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7_BANK2     ((uint32_t)0x081E0000) /* Base @ of Sector 7, 128 Kbytes */

#define  FLASHIF_OK                     ( 0 )
#define  FLASHIF_ERASEKO                ( -1 )
#define  FLASHIF_WRITINGCTRL_ERROR      ( -2 )
#define  FLASHIF_WRITING_ERROR          ( -3 )

#define  ROUND_SIZE_TO_FLASH( x )    ( x % 32 == 0 ? x : ( ( x / 32 ) + 1 ) * 32 )

#define FLASH_USER_FILE_EXIST_MAGIC     ( 0xAABBAABB )
#define EXCEPTION_INFO_MEMORY_REGIONS   ( 2 )

/*-----------------------------------------------------------*/

typedef struct MemoryRegion
{
    uint32_t uxOffset;
    uint32_t uxLength;
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

static uint32_t GetSector(uint32_t Address);

/*-----------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
static void FLASH_If_Init(void)
{
  
  /* Clear pending flags (if any) */  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                         FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);
}
/*-----------------------------------------------------------*/

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
static uint32_t FLASH_If_Erase(uint32_t StartSector)
{
  uint32_t UserStartSector;
  uint32_t SectorError;
  FLASH_EraseInitTypeDef pEraseInit;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock(); 
  FLASH_If_Init();
  
  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(APPLICATION_ADDRESS);
  
  pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
  pEraseInit.Sector = UserStartSector;
  pEraseInit.NbSectors = 8 - UserStartSector;
  pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  
  if (APPLICATION_ADDRESS < ADDR_FLASH_SECTOR_0_BANK2)
  {
    pEraseInit.Banks = FLASH_BANK_1;
    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
      /* Error occurred while sector erase */
      return (1);
    }
    
    /* Mass erase of second bank */
    pEraseInit.TypeErase = FLASH_TYPEERASE_MASSERASE;
    pEraseInit.Banks = FLASH_BANK_2;
    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
      /* Error occurred while sector erase */
      return (1);
    }
  }
  else
  {
    pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    pEraseInit.Banks = FLASH_BANK_2;
    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
      /* Error occurred while sector erase */
      return (1);
    }
  }
  HAL_FLASH_Lock(); 
  return (0);
}
/*-----------------------------------------------------------*/

/**
  * @brief  Gets the sector of a given address
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1_BANK1) && (Address >= ADDR_FLASH_SECTOR_0_BANK1))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2_BANK1) && (Address >= ADDR_FLASH_SECTOR_1_BANK1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3_BANK1) && (Address >= ADDR_FLASH_SECTOR_2_BANK1))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4_BANK1) && (Address >= ADDR_FLASH_SECTOR_3_BANK1))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5_BANK1) && (Address >= ADDR_FLASH_SECTOR_4_BANK1))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6_BANK1) && (Address >= ADDR_FLASH_SECTOR_5_BANK1))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7_BANK1) && (Address >= ADDR_FLASH_SECTOR_6_BANK1))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_0_BANK2) && (Address >= ADDR_FLASH_SECTOR_7_BANK1))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_1_BANK2) && (Address >= ADDR_FLASH_SECTOR_0_BANK2))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2_BANK2) && (Address >= ADDR_FLASH_SECTOR_1_BANK2))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3_BANK2) && (Address >= ADDR_FLASH_SECTOR_2_BANK2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4_BANK2) && (Address >= ADDR_FLASH_SECTOR_3_BANK2))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5_BANK2) && (Address >= ADDR_FLASH_SECTOR_4_BANK2))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6_BANK2) && (Address >= ADDR_FLASH_SECTOR_5_BANK2))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7_BANK2) && (Address >= ADDR_FLASH_SECTOR_6_BANK2))
  {
    sector = FLASH_SECTOR_6;  
  }
  else /*if((Address < USER_FLASH_END_ADDRESS) && (Address >= ADDR_FLASH_SECTOR_7_BANK2))*/
  {
    sector = FLASH_SECTOR_7;  
  }

  return sector;
}
/*-----------------------------------------------------------*/

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @param  pNextFlashAddress: Last flash address to write
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
static uint32_t FLASH_If_Write(uint32_t FlashAddress, uint32_t* Data ,uint32_t DataLength, uint32_t *pNextFlashAddress )
{
  uint32_t i = 0;
 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  for (i = 0; (i < DataLength) && (FlashAddress <= (USER_FLASH_END_ADDRESS-32)); i+=8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, FlashAddress, ((uint32_t)(Data+i))) == HAL_OK)
    {
     /* Check the written value */
      if (*(uint32_t*)FlashAddress != *(uint32_t*)(Data+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FLASHIF_WRITINGCTRL_ERROR);
      }
      /* Increment FLASH destination address */
      FlashAddress += 32;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (FLASHIF_WRITING_ERROR);
    }
  }

  HAL_FLASH_Lock();

  if( pNextFlashAddress != NULL )
  {
    *pNextFlashAddress = FlashAddress;
  }

  return (FLASHIF_OK);
}
/*-----------------------------------------------------------*/

BaseType_t ExpInfo_StoreInfo( void )
{
    static ExceptionInfo_t gExceptionInfo;
    static CoreReg_t gCoreReg;
    static uint32_t uxNextFlashAddress;
    static const uint32_t puxEndMagic[ 4 ] = { FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC, FLASH_USER_FILE_EXIST_MAGIC };
    static uint32_t uxRetFlash;

    extern unsigned char _sdata;
    extern unsigned char _edata;
    extern unsigned char _sbss;
    extern unsigned char _ebss;

    /* Store the current regin in gCoreReg. */
    EXPINFO_CAPTURE_REGISTERS( gCoreReg );

    /* Store the header information. */
    gExceptionInfo.uxFileExistMagic = FLASH_USER_FILE_EXIST_MAGIC;
    gExceptionInfo.uxTotalLength = ROUND_SIZE_TO_FLASH( sizeof( ExceptionInfo_t ) ) +
                                   ROUND_SIZE_TO_FLASH( sizeof( CoreReg_t ) ) +
                                   ROUND_SIZE_TO_FLASH( ( uint32_t )( &_edata - &_sdata ) ) +
                                   ROUND_SIZE_TO_FLASH( ( uint32_t )( &_ebss - &_sbss ) ) +
                                   ROUND_SIZE_TO_FLASH( 4 );

    gExceptionInfo.uxRegDumpOffset = ROUND_SIZE_TO_FLASH( sizeof( ExceptionInfo_t ) );
    gExceptionInfo.uxRegDumpLength = sizeof( CoreReg_t );

    gExceptionInfo.uxMemoryRegionNum = EXCEPTION_INFO_MEMORY_REGIONS;

    /* .data section. */
    gExceptionInfo.xMemoryRegions[ 0 ].uxOffset = ROUND_SIZE_TO_FLASH( sizeof( ExceptionInfo_t ) ) +
                                                  ROUND_SIZE_TO_FLASH( sizeof( CoreReg_t ) );
    gExceptionInfo.xMemoryRegions[ 0 ].uxLength = ( uint32_t )( &_edata - &_sdata );

    /* .bss section. */
    gExceptionInfo.xMemoryRegions[ 1 ].uxOffset = ROUND_SIZE_TO_FLASH( sizeof( ExceptionInfo_t ) ) +
                                                  ROUND_SIZE_TO_FLASH( sizeof( CoreReg_t ) ) +
                                                  ROUND_SIZE_TO_FLASH( ( uint32_t )( &_edata - &_sdata ) );
    gExceptionInfo.xMemoryRegions[ 1 ].uxLength = ( uint32_t )( &_ebss - &_sbss );
    uxRetFlash = FLASH_If_Write( APPLICATION_ADDRESS, ( uint32_t * )&gExceptionInfo, sizeof( gExceptionInfo ), &uxNextFlashAddress );

    /* Store the registers. */
    if( uxRetFlash == FLASHIF_OK )
    {
        uxRetFlash = FLASH_If_Write( uxNextFlashAddress, ( uint32_t * )&gCoreReg, sizeof( CoreReg_t ), &uxNextFlashAddress );
    }

    /* Store the data section memory. */
    if( uxRetFlash == FLASHIF_OK )
    {
        uxRetFlash = FLASH_If_Write( uxNextFlashAddress, ( uint32_t * )&_sdata, ( uint32_t )( &_edata - &_sdata ), &uxNextFlashAddress );
    }

    /* Store the bss section memory. */
    if( uxRetFlash == FLASHIF_OK )
    {
        uxRetFlash = FLASH_If_Write( uxNextFlashAddress, ( uint32_t * )&_sbss, ( uint32_t )( &_ebss - &_sbss ), &uxNextFlashAddress );
    }

    /* Store the magic end. */
    if( uxRetFlash == FLASHIF_OK )
    {
        uxRetFlash = FLASH_If_Write( uxNextFlashAddress, ( uint32_t * )puxEndMagic, 32, &uxNextFlashAddress );
    }

    return ( uxRetFlash == FLASHIF_OK ) ? pdTRUE: pdFALSE;
}
/*-----------------------------------------------------------*/

void ExpInfo_CleanInfo( void )
{
    /* Erase the whole bank of APPLICATION_ADDRESS start from sector 0. */
    FLASH_If_Erase( 0 );
}
/*-----------------------------------------------------------*/

BaseType_t ExpInfo_InfoExist( void )
{
    BaseType_t xReturn = pdTRUE;
    ExceptionInfo_t * pExceptionInfo = ( ExceptionInfo_t * )( APPLICATION_ADDRESS );
    uint32_t * puxImagePtr = ( uint32_t *  )( APPLICATION_ADDRESS );

    /* Check the start magic number of the sector. */
    if( pExceptionInfo->uxFileExistMagic != FLASH_USER_FILE_EXIST_MAGIC )
    {
        xReturn = pdFALSE;
    }

    /* Check the end magic number of the sector. */
    if( xReturn != pdFALSE )
    {
        if( puxImagePtr[ pExceptionInfo->uxTotalLength - 4 ] != FLASH_USER_FILE_EXIST_MAGIC )
        {
            xReturn = pdFALSE;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/
