/**
  ******************************************************************************
  * @file    bsp_ledout.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for LEDs
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LEDOUT_H__
#define __BSP_LEDOUT_H__

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
BSP_StatusTypeDef BSP_LEDOUT_Init(uint32_t ledID);
BSP_StatusTypeDef BSP_LEDOUT_WriteState(uint32_t ledID, uint32_t ledState);
BSP_StatusTypeDef BSP_LEDOUT_Toggle(uint32_t ledID);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LEDOUT_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
