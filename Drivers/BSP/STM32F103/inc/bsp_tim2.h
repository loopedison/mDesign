/**
  ******************************************************************************
  * @file    bsp_tim2.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for TIM2
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_TIM2_H__
#define __BSP_TIM2_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef   BSP_TIM2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_TIM2_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
