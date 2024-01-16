#ifndef EXPINFO_H
#define EXPINFO_H

#include "FreeRTOS.h"

/*-----------------------------------------------------------*/

/**
 * @brief  Define theh exception information upload methods.
 */
typedef enum ExpInfoUploadMethod
{
    EXP_INFO_UPLOAD_UART,
    EXP_INFO_UPLOAD_UDP,
    EXP_INFO_UPLOAD_MAX
} ExpInfoUploadMethod_t;

/*-----------------------------------------------------------*/

/**
 * @brief  Store the current exception information.
 * @retval pdFALSE : Store the information failed.
 */
BaseType_t ExpInfo_StoreInfo( void );

/**
 * @brief  Upload the exception information with method specified by uploadMethod.
 * @param  uploadMethod: Upload method.
 * @retval pdFALSE : Upload exception information failed.
 */
BaseType_t ExpInfo_UploadInfo( ExpInfoUploadMethod_t uploadMethod );

/**
 * @brief  Clean the exception information. After this commmand, ExpInfo_InfoExist()
 * should return false.
 */
void ExpInfo_CleanInfo( void );

/**
 * @brief  Check if exception information exist.
 * @retval pdFALSE : No exception information in the system.
 */
BaseType_t ExpInfo_InfoExist( void );

/*-----------------------------------------------------------*/

#endif /* #ifndef EXPINFO_H */
