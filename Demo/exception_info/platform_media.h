#ifndef PLATFORM_MDEIA_H
#define PLATFORM_MDEIA_H

#include <stdint.h>

#define PLATFORM_EXCEPTION_INFO_START_ADDRESS         ADDR_FLASH_SECTOR_0_BANK2
#define PLATFORM_EXCEPTION_INFO_MAX_SIZE              ( USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS )

#define PLATFORM_BYTES_TO_WRITE_UNIT( x )             ( ( x ) % 4 == 0 ? ( x / 4 ) : ( ( x ) / 4 ) + 1 )

#define PLATFORM_WRITE_MEDIA( xFlashAddress, pData, xDataLength, pNextFlashAddress ) \
    FLASH_If_Write( xFlashAddress, pData, PLATFORM_BYTES_TO_WRITE_UNIT( xDataLength ), pNextFlashAddress )

#define PLATFORM_EXCEPTION_INFO_CLEAN()               FLASH_If_Erase( 0 )
#define PLATFORM_ROUND_SIZE                           ROUND_SIZE_TO_FLASH

#define PLATFORM_MEDIA_OK                             ( 0 )

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

uint32_t FLASH_If_Erase(uint32_t StartSector);
uint32_t FLASH_If_Write(uint32_t FlashAddress, uint32_t* Data ,uint32_t DataLength, uint32_t *pNextFlashAddress );


#endif


