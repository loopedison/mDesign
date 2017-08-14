/**
  ******************************************************************************
  * @file    bsp_usben.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for USB ENABLE
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_USBEN_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_USBEN_GPIO_CLK_ENABLE
  #define BSP_USBEN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#endif /* BSP_USBEN_GPIO_CLK_ENABLE */

#ifndef BSP_USBEN_GPIO_PORT
  #define BSP_USBEN_GPIO_PORT          (GPIOA)
#endif /* BSP_USBEN_GPIO_PORT */

#ifndef BSP_USBEN_GPIO_PIN
  #define BSP_USBEN_GPIO_PIN           (GPIO_PIN_15)
#endif /* BSP_USBEN_GPIO_PIN */

#ifndef BSP_USBEN_LEVEL_TOGGLE
  #define BSP_USBEN_LEVEL_TOGGLE       (0)
#endif /* BSP_USBEN_LEVEL_TOGGLE */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial USB ENABLE
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_USBEN_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  BSP_USBEN_GPIO_CLK_ENABLE();
  /*Configure GPIO pins */
  GPIO_InitStruct.Pin = BSP_USBEN_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_USBEN_GPIO_PORT, &GPIO_InitStruct);
  
#if BSP_USBEN_LEVEL_TOGGLE != 0
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_SET);
#endif /* BSP_USBEN_LEVEL_TOGGLE */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Disconnect USB 
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_USBEN_Off(void)
{
#if BSP_USBEN_LEVEL_TOGGLE != 0
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_SET);
#endif /* BSP_USBEN_LEVEL_TOGGLE */
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Connect USB 
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_USBEN_On(void)
{
#if BSP_USBEN_LEVEL_TOGGLE != 0
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_USBEN_GPIO_PORT, BSP_USBEN_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_USBEN_LEVEL_TOGGLE */
  return (BSP_OK);
}


#endif /* BSP_USBEN_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
