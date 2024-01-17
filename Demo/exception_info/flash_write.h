#ifndef FLASH_WRITE_H
#define FLASH_WRITE_H

/* Standard includes. */
#include <stdint.h>

/* ST HAL includes. */
#include "stm32h7xx_hal.h"

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0_BANK1   ( ( uint32_t ) 0x08000000 ) /* Base @ of Sector 0, 128 KBytes. */
#define ADDR_FLASH_SECTOR_1_BANK1   ( ( uint32_t ) 0x08020000 ) /* Base @ of Sector 1, 128 KBytes. */
#define ADDR_FLASH_SECTOR_2_BANK1   ( ( uint32_t ) 0x08040000 ) /* Base @ of Sector 2, 128 KBytes. */
#define ADDR_FLASH_SECTOR_3_BANK1   ( ( uint32_t ) 0x08060000 ) /* Base @ of Sector 3, 128 KBytes. */
#define ADDR_FLASH_SECTOR_4_BANK1   ( ( uint32_t ) 0x08080000 ) /* Base @ of Sector 4, 128 KBytes. */
#define ADDR_FLASH_SECTOR_5_BANK1   ( ( uint32_t ) 0x080A0000 ) /* Base @ of Sector 5, 128 KBytes. */
#define ADDR_FLASH_SECTOR_6_BANK1   ( ( uint32_t ) 0x080C0000 ) /* Base @ of Sector 6, 128 KBytes. */
#define ADDR_FLASH_SECTOR_7_BANK1   ( ( uint32_t ) 0x080E0000 ) /* Base @ of Sector 7, 128 KBytes. */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_0_BANK2   ( ( uint32_t ) 0x08100000 ) /* Base @ of Sector 0, 128 KBytes. */
#define ADDR_FLASH_SECTOR_1_BANK2   ( ( uint32_t ) 0x08120000 ) /* Base @ of Sector 1, 128 KBytes. */
#define ADDR_FLASH_SECTOR_2_BANK2   ( ( uint32_t ) 0x08140000 ) /* Base @ of Sector 2, 128 KBytes. */
#define ADDR_FLASH_SECTOR_3_BANK2   ( ( uint32_t ) 0x08160000 ) /* Base @ of Sector 3, 128 KBytes. */
#define ADDR_FLASH_SECTOR_4_BANK2   ( ( uint32_t ) 0x08180000 ) /* Base @ of Sector 4, 128 KBytes. */
#define ADDR_FLASH_SECTOR_5_BANK2   ( ( uint32_t ) 0x081A0000 ) /* Base @ of Sector 5, 128 KBytes. */
#define ADDR_FLASH_SECTOR_6_BANK2   ( ( uint32_t ) 0x081C0000 ) /* Base @ of Sector 6, 128 KBytes. */
#define ADDR_FLASH_SECTOR_7_BANK2   ( ( uint32_t ) 0x081E0000 ) /* Base @ of Sector 7, 128 KBytes. */

#define FLASH_END_ADDR              ( ( uint32_t ) 0x081FFFFF )

#define APPLICATION_BANK            FLASH_BANK_1
#define EXCEPTION_INFO_BANK         FLASH_BANK_2

#define EXCEPTION_INFO_START_ADDR   ADDR_FLASH_SECTOR_0_BANK2
/*-----------------------------------------------------------*/

/* Round up to ensure that x is a multiple of 32.
 * 32 --> 32.
 * 33 --> 64.
 */
#define ROUND_UP_32( x )     ( ( ( x ) % 32 == 0 ) ? ( x ) : ( ( ( ( x ) / 32 ) + 1 ) * 32 ) )

/* Convert bytes to words. */
#define BYTES_TO_WORDS( x )  ( ( ( x ) % 4 == 0 ) ? ( ( x ) / 4 ) : ( ( ( x ) / 4 ) + 1 ) )

/*-----------------------------------------------------------*/

/* Return code from APIs. */
#define FLASH_OPERATION_OK          0
#define FLASH_ERASE_ERROR           1
#define FLASH_WRITE_ERROR           2
#define FLASH_WRITE_DATA_MISMATCH   3

/*-----------------------------------------------------------*/

/**
 * @brief Erase flash.
 *
 * @return FLASH_OPERATION_OK if successful, an error code otherwise.
 */
uint32_t FLASH_Erase( void );

/**
 * @brief Write to flash.
 *
 * Data is always written in chunks of 8 words. This means if the DataLengthInWords
 * is 9, 16 words would still be written.
 *
 * @param FlashAddress The flash address to program - must be aligned to 32 bytes (8 words).
 * @param Data The address of the data to write to flash.
 * @param DataLengthInWords The length of the data in words.
 * @param pNextFlashAddress The next flash address to write.
 *
 * @return FLASH_OPERATION_OK if successful, an error code otherwise.
 */
uint32_t FLASH_Write( uint32_t FlashAddress,
                      uint32_t* Data,
                      uint32_t DataLengthInWords,
                      uint32_t *pNextFlashAddress );

/*-----------------------------------------------------------*/

#endif /* FLASH_WRITE_H */
