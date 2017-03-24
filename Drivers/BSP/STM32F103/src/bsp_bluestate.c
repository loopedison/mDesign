/**
  ******************************************************************************
  * @file    bsp_bluestate.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth state Control
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_BLUESTATE_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_BLUESTATE_GPIO_CLK_ENABLE
  #define BSP_BLUESTATE_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#endif /* BSP_BLUESTATE_GPIO_CLK_ENABLE */

#ifndef BSP_BLUESTATE_GPIO_PORT
  #define BSP_BLUESTATE_GPIO_PORT          (GPIOB)
#endif /* BSP_BLUESTATE_GPIO_PORT */

#ifndef BSP_BLUESTATE_GPIO_PIN
  #define BSP_BLUESTATE_GPIO_PIN           (GPIO_PIN_14)
#endif /* BSP_BLUESTATE_GPIO_PIN */

#ifndef BSP_BLUESTATE_LEVEL_TOGGLE
  #define BSP_BLUESTATE_LEVEL_TOGGLE       (0)
#endif /* BSP_BLUESTATE_LEVEL_TOGGLE */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External function prototypes -----------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial BlueTooth State Control
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BLUESTATE_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  BSP_BLUESTATE_GPIO_CLK_ENABLE();
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = BSP_BLUESTATE_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_BLUESTATE_GPIO_PORT, &GPIO_InitStruct);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BlueTooth Connect State
  * @param  none: 
  * @retval true    : if connected
  */
bool BSP_BLUESTATE_Get(void)
{
#if BSP_BLUESTATE_LEVEL_TOGGLE == 0
  return (HAL_GPIO_ReadPin(BSP_BLUESTATE_GPIO_PORT, BSP_BLUESTATE_GPIO_PIN) == GPIO_PIN_RESET);
#else
  return (HAL_GPIO_ReadPin(BSP_BLUESTATE_GPIO_PORT, BSP_BLUESTATE_GPIO_PIN) == GPIO_PIN_SET);
#endif /* BSP_BLUESTATE_LEVEL_TOGGLE */
}

#endif /* BSP_BLUESTATE_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
