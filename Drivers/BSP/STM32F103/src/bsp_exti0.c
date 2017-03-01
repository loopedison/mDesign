/**
  ******************************************************************************
  * @file    bsp_exti0.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for EXTI
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_EXTI0_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_EXTI0_PRIO
  #define BSP_EXTI0_PRIO              (5)
#endif /* BSP_EXTI0_PRIO */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial EXTI line0 with INT
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_EXTI0_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /*Configure GPIO pins : PB0*/
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(EXTI0_IRQn, BSP_EXTI0_PRIO, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  This function handles EXTI line0 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

#endif /* BSP_EXTI0_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
