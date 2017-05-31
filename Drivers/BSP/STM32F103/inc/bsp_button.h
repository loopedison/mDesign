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
typedef enum 
{
  BUTTON_1 = 0,
  BUTTON_2 = 1,
  BUTTON_3 = 2,
  BUTTON_4 = 3,
}BUTTON_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_BUTTON_Init(BUTTON_TypeDef buttonID);
BSP_StatusTypeDef BSP_BUTTON_ReadState(BUTTON_TypeDef buttonID, uint32_t *state);
bool              BSP_BUTTON_Read(BUTTON_TypeDef buttonID);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_BUTTON_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
