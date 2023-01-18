/*
 * netstat_capture.c
 *
 *  Created on: Jan 18, 2023
 *      Author: karahulx
 */

/* Standard includes. */
#include <string.h>
#include <stdbool.h>

/* Interface includes. */
#include "netstat_capture.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* STM32 HAL includes. */
#include "stm32h7xx_hal.h"

