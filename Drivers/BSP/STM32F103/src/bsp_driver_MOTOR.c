/**
  ******************************************************************************
  * @file    bsp_driver_motor.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for Motor Control, with XY-160D
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_DRIVER_MOTOR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef MOTOR_CONFIG_WITH_TIM3
  #define MOTOR_CONFIG_WITH_TIM3         (1)
#endif /* MOTOR_CONFIG_WITH_TIM3 */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  BSP_DRIVER_MOTOR_Init
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_DRIVER_MOTOR_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Preset GPIO pins */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Preset GPIO pins */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  
#if MOTOR_CONFIG_WITH_TIM3 == 1
  /* ===== Init PWM ===== */
  BSP_TIM3_Init();
#endif /* MOTOR_CONFIG_WITH_TIM3 */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_DRIVER_MOTOR_Set
  * @param  nMotor, nDir: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_DRIVER_MOTOR_Set(int32_t nMotor, int32_t nSpd)
{
  if(nMotor == 0)
  {
    if(nSpd > 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_RESET);
    }
    else if(nSpd < 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_RESET);
    }
  }
  else if(nMotor == 1)
  {
    if(nSpd > 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    }
    else if(nSpd < 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    }
  }
  
#if MOTOR_CONFIG_WITH_TIM3 == 1
  /* ===== Init PWM ===== */
  BSP_TIM3_SetChannelRate(nMotor, abs(nSpd));
#endif /* MOTOR_CONFIG_WITH_TIM3 */
  
  return (BSP_OK);
}


#endif /* BSP_DRIVER_MOTOR_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
