/**
  ******************************************************************************
  * @file    bsp_tim2.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for TIM2
  * @note    TIM2 For Trig ADC1
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_TIM2_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_TIM2_DEFAULT_PRESCALER
  #define BSP_TIM2_DEFAULT_PRESCALER      (72-1)
#endif /* BSP_TIM2_DEFAULT_PRESCALER */

#ifndef BSP_TIM2_DEFAULT_PERIOD
  #define BSP_TIM2_DEFAULT_PERIOD         (10000-1)
#endif /* BSP_TIM2_DEFAULT_PERIOD */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial TIM2
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_TIM2_Init(void)
{
  /* Peripheral clock enable */
  __HAL_RCC_TIM2_CLK_ENABLE();
  
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = BSP_TIM2_DEFAULT_PRESCALER;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = BSP_TIM2_DEFAULT_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);
  
  TIM_ClockConfigTypeDef sClockSourceConfig;
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  
  HAL_TIM_OC_Init(&htim2);
  
  TIM_MasterConfigTypeDef sMasterConfig;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
  
  TIM_OC_InitTypeDef sConfigOC;
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
  
  return (BSP_OK);
}


#endif /* BSP_TIM2_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
