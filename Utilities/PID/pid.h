/**
  ******************************************************************************
  * @file    pid.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for PID Controller 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	/* Use PID_MODE_DERIVATIV_NONE for a PI controller (vs PID) */
	PID_MODE_DERIVATIV_NONE = 0,
	/* PID_MODE_DERIVATIV_CALC calculates discrete derivative from previous error,
	 * val_dot in PID_Calculate() will be ignored */
	PID_MODE_DERIVATIV_CALC,
	/* PID_MODE_DERIVATIV_CALC_NO_SP calculates discrete derivative from previous value,
	 * setpoint derivative will be ignored, val_dot in PID_Calculate() will be ignored */
	PID_MODE_DERIVATIV_CALC_NO_SP,
	/* Use PID_MODE_DERIVATIV_SET if you have the derivative already (Gyros, Kalman) */
	PID_MODE_DERIVATIV_SET
} PID_ModeDef;

typedef struct
{
	PID_ModeDef mode;
	float dt_min;
	float kp;
	float ki;
	float kd;
	float integral;
	float integral_limit;
	float output_limit;
	float error_previous;
	float last_output;
} PID_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int PID_Init(PID_TypeDef *pid, PID_ModeDef mode, float dt_min);
int PID_SetParameters(PID_TypeDef *pid, float kp, float ki, float kd, float integral_limit, float output_limit);
float PID_Calculate(PID_TypeDef *pid, float sp, float val, float val_dot, float dt);
int PID_ResetIntegral(PID_TypeDef *pid);


#ifdef __cplusplus
}
#endif

#endif /* __PID_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
