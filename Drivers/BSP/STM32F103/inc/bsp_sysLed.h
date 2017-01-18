/**
  ******************************************************************************
  * @file    bsp_sysled.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for SYSLED
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_SYSLED_H__
#define __BSP_SYSLED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_SYSLED_Init(void);
BSP_StatusTypeDef BSP_SYSLED_Off(void);
BSP_StatusTypeDef BSP_SYSLED_On(void);
BSP_StatusTypeDef BSP_SYSLED_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_SYSLED_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
