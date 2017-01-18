/**
  ******************************************************************************
  * @file    bsp_def.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   define for BSP
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DEF_H__
#define __BSP_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BSP_OK        = 0,
  BSP_ERROR     = 1,
  
  
  
}BSP_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DEF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
