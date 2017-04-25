/**
  ******************************************************************************
  * @file    bsp_key.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for KEYs
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum 
{
  KEY1 = 0,
  KEY2 = 1,
  KEY3 = 2,
  KEY4 = 3,
}KEY_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_KEY_Init(KEY_TypeDef keyID);
BSP_StatusTypeDef BSP_KEY_ReadState(KEY_TypeDef keyID, uint32_t *state);
bool              BSP_KEY_Read(KEY_TypeDef keyID);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_KEY_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
