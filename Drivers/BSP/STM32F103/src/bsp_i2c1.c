/**
  ******************************************************************************
  * @file    bsp_i2c1.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for I2C1
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_I2C1_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef I2C1_DEFAULT_SPEED
  #define I2C1_DEFAULT_SPEED            (400000)
#endif /* I2C1_DEFAULT_SPEED */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/
//==============================================================================
/**
  * @brief  BSP_I2C1_Init
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_I2C1_Init(void)
{
  /* !!! HARDWARE_BUG: Enable I2C CLOCK Before Initialize !!! */
  __HAL_RCC_I2C1_CLK_ENABLE();
  
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /**I2C1 GPIO Configuration    
  PB6     ------> I2C1_SCL
  PB7     ------> I2C1_SDA 
  */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  
  /* Peripheral clock enable */
  __HAL_RCC_I2C1_CLK_ENABLE();
  
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = I2C1_DEFAULT_SPEED;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_I2C1_SetSpeed
  * @param  nSpeed: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_I2C1_SetSpeed(uint32_t nSpeed)
{
  hi2c1.Init.ClockSpeed = nSpeed;
  HAL_I2C_Init(&hi2c1);
  return (BSP_OK);
}

#endif /* BSP_I2C1_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
