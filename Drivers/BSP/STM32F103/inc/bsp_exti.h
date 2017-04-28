/**
  ******************************************************************************
  * @file    bsp_exti.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for EXTI
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

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
BSP_StatusTypeDef   BSP_EXTI5_Init(void);
BSP_StatusTypeDef   BSP_EXTI6_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_EXTI_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
