/**
  ******************************************************************************
  * @file    bsp_motor.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for Motor Control, with TB6612FNG
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

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
BSP_StatusTypeDef BSP_MOTOR_Init(void);
BSP_StatusTypeDef BSP_MOTOR_SetEnable(bool nEn);
BSP_StatusTypeDef BSP_MOTOR_SetDir(int32_t nMotor, int32_t nDir);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_MOTOR_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
