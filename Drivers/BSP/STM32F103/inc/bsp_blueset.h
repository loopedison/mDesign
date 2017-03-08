/**
  ******************************************************************************
  * @file    bsp_blueset.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth Set Control
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BLUESET_H__
#define __BSP_BLUESET_H__

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
BSP_StatusTypeDef BSP_BLUESET_Init(void);
BSP_StatusTypeDef BSP_BLUESET_Enable(void);
BSP_StatusTypeDef BSP_BLUESET_Disable(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BLUESET_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
