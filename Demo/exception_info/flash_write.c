/* Interface includes. */
#include "flash_write.h"

uint32_t FLASH_Erase( void )
{
    uint32_t Error;
    FLASH_EraseInitTypeDef pEraseInit;

    /* Unlock the Flash to enable the flash control register access. */
    HAL_FLASH_Unlock();

    /* Clear pending flags (if any). */
    __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_EOP |
                            FLASH_FLAG_OPERR |
                            FLASH_FLAG_WRPERR |
                            FLASH_FLAG_PGSERR |
                            FLASH_FLAG_WRPERR );

    /* Mass erase of the flash bank containing the exception info. */
    pEraseInit.TypeErase = FLASH_TYPEERASE_MASSERASE;
    pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    pEraseInit.Banks = EXCEPTION_INFO_BANK;

    if( HAL_FLASHEx_Erase( &( pEraseInit ), &( Error ) ) != HAL_OK )
    {
         return FLASH_ERASE_ERROR;
    }

    HAL_FLASH_Lock();

    return FLASH_OPERATION_OK;
}

/*-----------------------------------------------------------*/

uint32_t FLASH_Write( uint32_t FlashAddress,
                      uint32_t* Data,
                      uint32_t DataLengthInWords,
                      uint32_t *pNextFlashAddress )
{
    uint32_t i = 0;

    /* Unlock the Flash to enable the flash control register access. */
    HAL_FLASH_Unlock();

    for( i = 0; ( i < DataLengthInWords ) && ( FlashAddress <= ( FLASH_END_ADDR - 32 ) ); i += 8 )
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
         * be done by word. */
        if( HAL_FLASH_Program( FLASH_TYPEPROGRAM_FLASHWORD,
                               FlashAddress,
                               ( ( uint32_t )( Data + i ) ) ) == HAL_OK )
        {
            /* Check the written value. */
            if( *( uint32_t * )FlashAddress != *( uint32_t * )( Data + i ) )
            {
                /* Flash content doesn't match SRAM content. */
                return FLASH_WRITE_DATA_MISMATCH;
            }
            /* Increment FLASH destination address. */
            FlashAddress += 32;
        }
        else
        {
            /* Error occurred while writing data in Flash memory. */
            return FLASH_WRITE_ERROR;
        }
    }

    HAL_FLASH_Lock();

    if( pNextFlashAddress != NULL )
    {
        *pNextFlashAddress = FlashAddress;
    }

  return FLASH_OPERATION_OK;
}

/*-----------------------------------------------------------*/
