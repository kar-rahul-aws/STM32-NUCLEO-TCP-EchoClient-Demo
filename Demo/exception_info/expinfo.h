#ifndef EXP_INFO_H
#define EXP_INFO_H

/* FreeRTOS includes. */
#include "FreeRTOS.h"

/*-----------------------------------------------------------*/

/**
 * @brief Store the current Assert information.
 *
 * @retval pdTRUE if the store was successful, pdFALSE otherwise.
 */
BaseType_t ExpInfo_StoreAssertInfo( void );

/**
 * @brief Store the current Fault information.
 *
 * @param pulTaskStack Stack pointer of the task that was executing at the time
 * of exception.
 */
void ExpInfo_StoreFaultInfo( uint32_t *pulTaskStack );

/**
 * @brief Get the stored exception information.
 *
 * @param pxExceptionInfo The output parameter to return the exception info.
 * @param pxExceptionInfoLength The output parameter to return the exception info length.
 *
 * @retval pdTRUE if the exception info is retrievd successfully, pdFALSE otherwise.
 */
BaseType_t ExpInfo_GetInfo( const uint8_t ** pxExceptionInfo,
                            uint32_t * pxExceptionInfoLength );

/**
 * @brief Clean the stored exception information.
 *
 * After this function is called, ExpInfo_InfoExist() will return pdFALSE till
 * next exception info is stored using ExpInfo_StoreInfo.
 */
void ExpInfo_CleanInfo( void );

/**
 * @brief Check if exception info exists.
 *
 * @retval pdTRUE if an exception info is present, pdFALSE otherwise.
 */
BaseType_t ExpInfo_InfoExist( void );

/*-----------------------------------------------------------*/

#endif /* #ifndef EXP_INFO_H */
