/**
  ******************************************************************************
  * @file    bsp_i2c1.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for I2C1
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_I2C1_H__
#define __BSP_I2C1_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_I2C1_Init(void);
BSP_StatusTypeDef BSP_I2C1_SetSpeed(uint32_t nSpeed);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C1_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
