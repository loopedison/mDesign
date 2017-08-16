/**
  ******************************************************************************
  * @file    bsp_driver_motor.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for Motor Control, with XY-160D
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DRIVER_MOTOR_H__
#define __BSP_DRIVER_MOTOR_H__

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
BSP_StatusTypeDef BSP_DRIVER_MOTOR_Init(void);
BSP_StatusTypeDef BSP_DRIVER_MOTOR_Set(int32_t nMotor, int32_t nSpd);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DRIVER_MOTOR_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
