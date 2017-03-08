/**
  ******************************************************************************
  * @file    bsp_blueset.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth Set Control
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_BLUESET_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_BLUESET_GPIO_CLK_ENABLE
  #define BSP_BLUESET_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#endif /* BSP_BLUESET_GPIO_CLK_ENABLE */

#ifndef BSP_BLUESET_GPIO_PORT
  #define BSP_BLUESET_GPIO_PORT          (GPIOA)
#endif /* BSP_BLUESET_GPIO_PORT */

#ifndef BSP_BLUESET_GPIO_PIN
  #define BSP_BLUESET_GPIO_PIN           (GPIO_PIN_8)
#endif /* BSP_BLUESET_GPIO_PIN */

#ifndef BSP_BLUESET_LEVEL_TOGGLE
  #define BSP_BLUESET_LEVEL_TOGGLE       (0)
#endif /* BSP_BLUESET_LEVEL_TOGGLE */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial BlueTooth Set Control
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUESET_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  BSP_BLUESET_GPIO_CLK_ENABLE();
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = BSP_BLUESET_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_BLUESET_GPIO_PORT, &GPIO_InitStruct);
  
#if BSP_BLUESET_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_BLUESET_LEVEL_TOGGLE */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn BlueTooth to Enable
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUESET_Enable(void)
{
#if BSP_BLUESET_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_BLUESET_LEVEL_TOGGLE */
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn BlueTooth to Disable
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUESET_Disable(void)
{
#if BSP_BLUESET_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(BSP_BLUESET_GPIO_PORT, BSP_BLUESET_GPIO_PIN, GPIO_PIN_SET);
#endif /* BSP_BLUESET_LEVEL_TOGGLE */
  return (BSP_OK);
}

#endif /* BSP_BLUESET_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
