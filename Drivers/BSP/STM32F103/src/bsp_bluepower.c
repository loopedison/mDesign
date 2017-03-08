/**
  ******************************************************************************
  * @file    bsp_bluepower.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth Power Control
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_BLUEPOWER_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_BLUEPOWER_GPIO_CLK_ENABLE
  #define BSP_BLUEPOWER_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#endif /* BSP_BLUEPOWER_GPIO_CLK_ENABLE */

#ifndef BSP_BLUEPOWER_GPIO_PORT
  #define BSP_BLUEPOWER_GPIO_PORT          (GPIOB)
#endif /* BSP_BLUEPOWER_GPIO_PORT */

#ifndef BSP_BLUEPOWER_GPIO_PIN
  #define BSP_BLUEPOWER_GPIO_PIN           (GPIO_PIN_15)
#endif /* BSP_BLUEPOWER_GPIO_PIN */

#ifndef BSP_BLUEPOWER_LEVEL_TOGGLE
  #define BSP_BLUEPOWER_LEVEL_TOGGLE       (0)
#endif /* BSP_BLUEPOWER_LEVEL_TOGGLE */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial BlueTooth Power Control
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUEPOWER_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  BSP_BLUEPOWER_GPIO_CLK_ENABLE();
  /*Configure GPIO pins */
  GPIO_InitStruct.Pin = BSP_BLUEPOWER_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_BLUEPOWER_GPIO_PORT, &GPIO_InitStruct);
  
#if BSP_BLUEPOWER_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_BLUEPOWER_LEVEL_TOGGLE */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn Off BlueTooth 
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUEPOWER_Off(void)
{
#if BSP_BLUEPOWER_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_BLUEPOWER_LEVEL_TOGGLE */
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn On BlueTooth 
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUEPOWER_On(void)
{
#if BSP_BLUEPOWER_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(BSP_BLUEPOWER_GPIO_PORT, BSP_BLUEPOWER_GPIO_PIN, GPIO_PIN_SET);
#endif /* BSP_BLUEPOWER_LEVEL_TOGGLE */
  return (BSP_OK);
}

#endif /* BSP_BLUEPOWER_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
