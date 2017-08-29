/**
  ******************************************************************************
  * @file    pid.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for PID Controller 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SIGMA 0.000001f

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  PID Init
  * @param  pid     :Handler
  * @param  mode    :
  * @param  dt_min  :
  * @retval status
  */
int PID_Init(PID_TypeDef *pid, PID_ModeDef mode, float dt_min)
{
	pid->mode           = mode;
	pid->dt_min         = dt_min;
	pid->kp             = 0.0f;
	pid->ki             = 0.0f;
	pid->kd             = 0.0f;
	pid->integral       = 0.0f;
	pid->integral_limit = 0.0f;
	pid->output_limit   = 0.0f;
	pid->error_previous = 0.0f;
	pid->last_output    = 0.0f;
  return 0;
}

//==============================================================================
/**
  * @brief  PID Set Parameters
  * @param  pid     :Handler
  * @param  kp      :
  * @param  ki      :
  * @param  kd      :
  * @param  integral_limit  :
  * @param  output_limit    :
  * @retval status
  */
int PID_SetParameters(PID_TypeDef *pid, float kp, float ki, float kd, float integral_limit, float output_limit)
{
  if ((!isfinite(kp)) || (!isfinite(ki)) || (!isfinite(kd)) || (!isfinite(integral_limit)) || (!isfinite(output_limit)))
  {
		return -1;
	}
  
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->integral_limit = integral_limit;
  pid->output_limit   = output_limit;
  
  return 0;
}

//==============================================================================
/**
  * @brief  PID Set Parameters
  * @param  pid     :Handler
  * @param  sp      :
  * @param  val     :
  * @param  val_dot :
  * @param  dt      :
  * @retval status
  */
float PID_Calculate(PID_TypeDef *pid, float sp, float val, float val_dot, float dt)
{
	if (!isfinite(sp) || !isfinite(val) || !isfinite(val_dot) || !isfinite(dt))
  {
		return pid->last_output;
	}
  
	float i, d;
  float error;
  float output;
  
	/* current error value */
	error = sp - val;
  
	/* calculate P output */
	output = (error * pid->kp);
  
	/* current error derivative */
	if (pid->mode == PID_MODE_DERIVATIV_CALC)
  {
		d = (error - pid->error_previous) / fmaxf(dt, pid->dt_min);
		pid->error_previous = error;
	}
  else if (pid->mode == PID_MODE_DERIVATIV_CALC_NO_SP)
  {
		d = (-val - pid->error_previous) / fmaxf(dt, pid->dt_min);
		pid->error_previous = -val;
	}
  else if (pid->mode == PID_MODE_DERIVATIV_SET)
  {
		d = -val_dot;
	}
  else
  {
		d = 0.0f;
	}
  
  if(!isfinite(d))
  {
    d = 0.0f;
  }
  
	/* calculate PD output */
	output += (d * pid->kd);
  
	if (pid->ki > SIGMA)
  {
		// Calculate the error integral and check for saturation
		i = pid->integral + (error * dt);
    
		/* check for saturation */
		if (isfinite(i))
    {
			if ((pid->output_limit < SIGMA || (fabsf(output + (i * pid->ki)) <= pid->output_limit)) &&
			    fabsf(i) <= pid->integral_limit)
      {
				/* not saturated, use new integral value */
				pid->integral = i;
			}
		}
    
		/* add I component to output */
		output += pid->integral * pid->ki;
	}
  
	/* limit output */
  if (isfinite(output))
  {
    if (pid->output_limit > SIGMA)
    {
      if (output > pid->output_limit)
      {
        output = pid->output_limit;
      }
      else if (output < -pid->output_limit)
      {
        output = -pid->output_limit;
      }
    }
  }
  
  pid->last_output = output;
  
	return pid->last_output;
}

//==============================================================================
/**
  * @brief  PID Reset Integral
  * @param  pid     :Handler
  * @retval status
  */
int PID_ResetIntegral(PID_TypeDef *pid)
{
	pid->integral = 0.0f;
  return 0;
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
