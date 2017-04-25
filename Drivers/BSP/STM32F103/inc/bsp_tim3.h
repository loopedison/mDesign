/**
  ******************************************************************************
  * @file    bsp_tim3.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for TIM3
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_TIM3_H__
#define __BSP_TIM3_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_TIM3_Init(void);
BSP_StatusTypeDef BSP_TIM3_SetChannelPulse(uint32_t timCh, uint32_t timPulse);
BSP_StatusTypeDef BSP_TIM3_SetChannelRate(uint32_t timCh, uint32_t rate);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_TIM3_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
