/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for EXTI
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_EXTI_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if BSP_EXTI95_USING_INT == 1
  #define BSP_EXTI95_USING_INT        (1)
#endif /* BSP_EXTI95_USING_INT */

#ifndef BSP_EXTI95_PRIO
  #define BSP_EXTI95_PRIO             (6)
#endif /* BSP_EXTI95_PRIO */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial EXTI line5 with INT
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_EXTI5_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*Configure GPIO pins : PA5*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
#if BSP_EXTI95_USING_INT == 1
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, BSP_EXTI95_PRIO, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
#endif /* BSP_EXTI95_USING_INT */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Initial EXTI line6 with INT
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_EXTI6_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*Configure GPIO pins : PA6*/
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
#if BSP_EXTI95_USING_INT == 1
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, BSP_EXTI95_PRIO, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
#endif /* BSP_EXTI95_USING_INT */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_EXTI5_Read
  * @param  none
  * @retval GPIO PinState
  */
bool BSP_EXTI5_Read(void)
{
  return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET);
}

//==============================================================================
/**
  * @brief  BSP_EXTI6_Read
  * @param  none
  * @retval GPIO PinState
  */
bool BSP_EXTI6_Read(void)
{
  return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET);
}

//==============================================================================
/**
  * @brief  This function handles EXTI line95 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
#if BSP_EXTI95_USING_INT == 1
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}
#endif /* BSP_EXTI95_USING_INT */


#endif /* BSP_EXTI_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
