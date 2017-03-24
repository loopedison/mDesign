/**
  ******************************************************************************
  * @file    bsp_bluestate.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth state Control
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BLUESTATE_H__
#define __BSP_BLUESTATE_H__

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
BSP_StatusTypeDef BSP_BLUESTATE_Init(void);
bool              BSP_BLUESTATE_Get(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BLUESTATE_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
