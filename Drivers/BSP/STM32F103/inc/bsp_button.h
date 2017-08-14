/**
  ******************************************************************************
  * @file    bsp_button.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BUTTONs
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BUTTON_H__
#define __BSP_BUTTON_H__

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
BSP_StatusTypeDef BSP_BUTTON_Init(uint32_t buttonID);
BSP_StatusTypeDef BSP_BUTTON_ReadState(uint32_t buttonID, uint32_t *state);
bool              BSP_BUTTON_Read(uint32_t buttonID);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BUTTON_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
