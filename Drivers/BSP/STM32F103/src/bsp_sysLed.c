/**
  ******************************************************************************
  * @file    bsp_sysled.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for sysled
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_SYSLED_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_SYSLED_GPIO_CLK_ENABLE
  #define BSP_SYSLED_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#endif /* BSP_SYSLED_GPIO_CLK_ENABLE */

#ifndef BSP_SYSLED_GPIO_PORT
  #define BSP_SYSLED_GPIO_PORT          (GPIOB)
#endif /* BSP_SYSLED_GPIO_PORT */

#ifndef BSP_SYSLED_GPIO_PIN
  #define BSP_SYSLED_GPIO_PIN           (GPIO_PIN_12)
#endif /* BSP_SYSLED_GPIO_PIN */

#ifndef BSP_SYSLED_LEVEL_TOGGLE
  #define BSP_SYSLED_LEVEL_TOGGLE       (0)
#endif /* BSP_SYSLED_LEVEL_TOGGLE */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial sysled
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_SYSLED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  BSP_SYSLED_GPIO_CLK_ENABLE();
  /*Configure GPIO pins : PB2 */
  GPIO_InitStruct.Pin = BSP_SYSLED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_SYSLED_GPIO_PORT, &GPIO_InitStruct);
  
#if BSP_SYSLED_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_SYSLED_LEVEL_TOGGLE */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn OFF sysled
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_SYSLED_Off(void)
{
#if BSP_SYSLED_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_SET);
#else
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_RESET);
#endif /* BSP_SYSLED_LEVEL_TOGGLE */
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Turn ON sysled
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_SYSLED_On(void)
{
#if BSP_SYSLED_LEVEL_TOGGLE == 0
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN, GPIO_PIN_SET);
#endif /* BSP_SYSLED_LEVEL_TOGGLE */
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Toggle sysled
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_SYSLED_Toggle(void)
{
  HAL_GPIO_TogglePin(BSP_SYSLED_GPIO_PORT, BSP_SYSLED_GPIO_PIN);
  return (BSP_OK);
}

#endif /* BSP_SYSLED_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
