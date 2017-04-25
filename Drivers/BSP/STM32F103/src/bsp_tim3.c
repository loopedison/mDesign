/**
  ******************************************************************************
  * @file    bsp_tim3.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for TIM3
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_TIM3_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef TIM3_PWM_DEFAULT_PRESCALER
  #define TIM3_PWM_DEFAULT_PRESCALER    (72-1)
#endif /* TIM3_PWM_DEFAULT_PRESCALER */

#ifndef TIM3_PWM_DEFAULT_PERIOD
  #define TIM3_PWM_DEFAULT_PERIOD       (1000)    //1k hz
#endif /* TIM3_PWM_DEFAULT_PERIOD */

#ifndef TIM3_PWM_DEFAULT_PULSE
  #define TIM3_PWM_DEFAULT_PULSE        (2)
#endif /* TIM3_PWM_DEFAULT_PULSE */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_OC_InitTypeDef sConfigOC;

/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Init TIM3 
  * @param  none 
  * @retval BSP_OK, normal
  */
BSP_StatusTypeDef BSP_TIM3_Init(void)
{
  BSP_StatusTypeDef status = BSP_OK;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /**TIM3 GPIO Configuration    
  PB4     ------> TIM3_CH1
  PB5     ------> TIM3_CH2
  PB0     ------> TIM3_CH3
  PB1     ------> TIM3_CH4
  */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  __HAL_AFIO_REMAP_TIM3_PARTIAL();
  
  /* Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();
  
  
  /* Initializes TIM Base */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = TIM3_PWM_DEFAULT_PRESCALER;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIM3_PWM_DEFAULT_PERIOD;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);
  
  
  /* Initializes TIM PWM */
  HAL_TIM_PWM_Init(&htim3);
  
  
  /* Initializes TIM clock source */
  TIM_ClockConfigTypeDef sClockSourceConfig;
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);
  
  
  /* Initializes TIM Master Slave */
  TIM_MasterConfigTypeDef sMasterConfig;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
  
  
  /* Configure the PWM channels */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  
  /* Set the pulse value for channel 1 */
  sConfigOC.Pulse = TIM3_PWM_DEFAULT_PULSE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
  /* Set the pulse value for channel 2 */
  sConfigOC.Pulse = TIM3_PWM_DEFAULT_PULSE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
  /* Set the pulse value for channel 3 */
  sConfigOC.Pulse = TIM3_PWM_DEFAULT_PULSE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
  /* Set the pulse value for channel 4 */
  sConfigOC.Pulse = TIM3_PWM_DEFAULT_PULSE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
  
  
  /* Start PWM channels */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  
  
  return (status);
}

//==============================================================================
/**
  * @brief  set TIM3 PWM pulse
  * @param  timCh 
  * @param  timPulse 
  * @retval BSP_OK, normal
  */
BSP_StatusTypeDef BSP_TIM3_SetChannelPulse(uint32_t timCh, uint32_t timPulse)
{
  BSP_StatusTypeDef status = BSP_OK;
  
  if(timPulse < TIM3_PWM_DEFAULT_PERIOD)
  {
    sConfigOC.Pulse = timPulse;
  }
  else
  {
    sConfigOC.Pulse = TIM3_PWM_DEFAULT_PERIOD;
  }
  
  if     (timCh == 0x0)
  {
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  }
  else if(timCh == 0x1)
  {
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  }
  else if(timCh == 0x2)
  {
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  }
  else if(timCh == 0x3)
  {
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  set TIM3 PWM rate
  * @param  timCh 
  * @param  rate 
  * @retval BSP_OK, normal
  */
BSP_StatusTypeDef BSP_TIM3_SetChannelRate(uint32_t timCh, uint32_t rate)
{
  BSP_StatusTypeDef status = BSP_OK;
  uint32_t pulse = 0;
  pulse = rate*TIM3_PWM_DEFAULT_PERIOD/100;
  status = BSP_TIM3_SetChannelPulse(timCh, pulse);
  return (status);
}


#endif /* BSP_TIM3_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
