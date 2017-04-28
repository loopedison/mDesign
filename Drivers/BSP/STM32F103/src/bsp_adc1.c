/**
  ******************************************************************************
  * @file    bsp_adc1.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for ADC1
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_ADC1_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BSP_ADC1_USING_DMA
  #define BSP_ADC1_USING_DMA        (1)
#endif /* BSP_ADC1_USING_DMA */

#ifndef BSP_ADC1_DMA_PRIO
  #define BSP_ADC1_DMA_PRIO         (5) 
#endif /* BSP_ADC1_DMA_PRIO */

#ifndef BSP_ADC1_CHANNEL_NUM
  #define BSP_ADC1_CHANNEL_NUM      (6) 
#endif /* BSP_ADC1_CHANNEL_NUM */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
/* Export function prototypes ------------------------------------------------*/
//==============================================================================
/**
  * @brief  Initial ADC1 with DMA INT
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_ADC1_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /**ADC1 GPIO Configuration    
  PA1     ------> ADC1_IN1
  PA2     ------> ADC1_IN2
  PA3     ------> ADC1_IN3
  PA4     ------> ADC1_IN4 
  */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  
#ifdef BSP_ADC1_USING_DMA
  /* Peripheral DMA init*/
  
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  hdma_adc1.Instance = DMA1_Channel1;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_adc1);
  
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
  
  /* DMA2_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, BSP_ADC1_DMA_PRIO, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  
#endif /* BSP_ADC1_USING_DMA */
  
  /* Peripheral clock enable */
  __HAL_RCC_ADC1_CLK_ENABLE();
  
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = BSP_ADC1_CHANNEL_NUM;
  HAL_ADC_Init(&hadc1);
  
  /**Configure  channels. 
  */
  ADC_ChannelConfTypeDef sConfig;
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 3;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 4;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 5;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  return (BSP_OK);
}


#endif /* BSP_ADC1_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
