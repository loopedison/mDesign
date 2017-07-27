/**
  ******************************************************************************
  * @file    tsensor.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Tsensor
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "tsensor.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "superled.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* sensor */
Tsensor_TypeDef tsensor;

//==============================================================================
/* adc vref+ /mv */
#define ADC_CHANNEL_NUM     BSP_ADC1_CHANNEL_NUM
/* adc result */
static uint16_t adcValue[4] = {0};
static uint32_t adcUpdateFlag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Tsensor Init
  * @param  None
  * @retval None
  */
void Tsensor_Init(void)
{
  /* Init Device */
  BSP_TIM2_Init();
  BSP_ADC1_Init();
  BSP_EXTI5_Init();
  BSP_EXTI6_Init();
  
  /* Initialize For ADC */
  tsensor.xParam.xADCref    = *(uint16_t *)hStorageMsgData.xUserParam.xParamADCref;
  /* Initialize For tsensor */
  tsensor.xParam.xPitchMin  = *(uint16_t *)hStorageMsgData.xUserParam.xParamPitchMin;
  tsensor.xParam.xPitchMid  = *(uint16_t *)hStorageMsgData.xUserParam.xParamPitchMid;
  tsensor.xParam.xPitchMax  = *(uint16_t *)hStorageMsgData.xUserParam.xParamPitchMax;
  tsensor.xParam.xThroMin   = *(uint16_t *)hStorageMsgData.xUserParam.xParamThroMin;
  tsensor.xParam.xThroMax   = *(uint16_t *)hStorageMsgData.xUserParam.xParamThroMax;
  /* Start ADC */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, ADC_CHANNEL_NUM);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_2);
}

//==============================================================================
/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  * @note   INT mode
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    adcUpdateFlag = 1;
  }
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  /* Update adcValue , and voltage */
  if(adcUpdateFlag != 0)
  {
    adcUpdateFlag = 0;
    for(uint32_t i=0; i<ADC_CHANNEL_NUM; i++)
    {
      /* filter ( aX0+(1-a)X1 ) */
      tsensor.xData.xAdcValue[i] = (0x04*tsensor.xData.xAdcValue[i] + 0x0c*adcValue[i])/0x10;
      tsensor.xData.xAdcVoltage[i] = (tsensor.xParam.xADCref)*(tsensor.xData.xAdcValue[i])/0X1000;
    }
  }
  
  /* Update Pitch */
  if(tsensor.xData.xAdcValue[0] < tsensor.xParam.xPitchMin)
  {
    tsensor.xData.xPitch = -100;
  }
  else if(tsensor.xData.xAdcValue[0] < tsensor.xParam.xPitchMid)
  {
    tsensor.xData.xPitch = -100*(int32_t)(tsensor.xParam.xPitchMid-tsensor.xData.xAdcValue[0])/(tsensor.xParam.xPitchMid-tsensor.xParam.xPitchMin);
  }
  else if(tsensor.xData.xAdcValue[0] < tsensor.xParam.xPitchMax)
  {
    tsensor.xData.xPitch =  100*(int32_t)(tsensor.xData.xAdcValue[0]-tsensor.xParam.xPitchMid)/(tsensor.xParam.xPitchMax-tsensor.xParam.xPitchMid);
  }
  else
  {
    tsensor.xData.xPitch = 100;
  }
  
  /* Update Throttle */
  if(tsensor.xData.xAdcValue[1] < tsensor.xParam.xThroMin)
  {
    tsensor.xData.xThrottle = 0;
  }
  else if(tsensor.xData.xAdcValue[1] < tsensor.xParam.xThroMax)
  {
    tsensor.xData.xThrottle = 100*(int32_t)(tsensor.xData.xAdcValue[1]-tsensor.xParam.xThroMin)/(tsensor.xParam.xThroMax-tsensor.xParam.xThroMin);
  }
  else
  {
    tsensor.xData.xThrottle = 100;
  }
  /* correct */
  tsensor.xData.xThrottle = 100-tsensor.xData.xThrottle;
  
  /* Update Key State */
  tsensor.xData.xButton = 0;
  if(BSP_EXTI5_ReadState() == GPIO_PIN_RESET)
  {
    tsensor.xData.xButton |= (0x1<<0);
  }
  if(BSP_EXTI6_ReadState() == GPIO_PIN_RESET)
  {
    tsensor.xData.xButton |= (0x1<<1);
  }
  
}


//==============================================================================
/**
  * @brief  Tsensor_GetInstance
  * @param  **pSensor
  * @retval none
  */
void Tsensor_GetInstance(Tsensor_TypeDef **pSensor)
{
  *pSensor = &tsensor;
}

//==============================================================================
/**
  * @brief  Tsensor_GetData
  * @param  pData
  * @retval none
  */
void Tsensor_GetData(Tsensor_DataTypeDef *pData)
{
  memcpy(pData, &tsensor.xData, sizeof(Tsensor_DataTypeDef));
}

//==============================================================================
/**
  * @brief  Tsensor_GetParam
  * @param  pParam
  * @retval none
  */
void Tsensor_GetParam(Tsensor_ParamTypeDef *pParam)
{
  memcpy(pParam, &tsensor.xParam, sizeof(Tsensor_ParamTypeDef));
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
