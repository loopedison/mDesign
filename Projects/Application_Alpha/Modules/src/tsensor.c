/**
  ******************************************************************************
  * @file    tsensor.c
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
static uint16_t adcValue[ADC_CHANNEL_NUM] = {0};
static uint32_t adcUpdateFlag = 0;

//==============================================================================
/* Counter Rate */
#define TIMING_PERIOD         ( 100)    //ms
#define TIMING_DELAY          (2000)
#define TIMING_TICKBUFF       (   4)
typedef struct
{
  uint32_t              xTickVal[TIMING_TICKBUFF];
  uint32_t              xRPM;
}Timing_TypeDef;
static Timing_TypeDef   xTiming;

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
  BSP_ADC1_Init();
  BSP_TIM2_Init();
  BSP_EXTI5_Init();
  BSP_EXTI6_Init();
  BSP_BUTTON_Init(6);
  BSP_BUTTON_Init(7);
  BSP_BUTTON_Init(8);
  BSP_BUTTON_Init(9);
  
  /* Clear tsensor */
  memset(&tsensor, 0, sizeof(Tsensor_TypeDef));
  /* Initialize For ADC */
  tsensor.xParam.xADCref    = *(uint16_t *)hStorageMsgData.xUserParam.xParamADCref;
  /* Initialize For tsensor */
  tsensor.xParam.xSpdMin    = *(uint16_t *)hStorageMsgData.xUserParam.xParamSpeedMin;
  tsensor.xParam.xSpdMax    = *(uint16_t *)hStorageMsgData.xUserParam.xParamSpeedMax;
  tsensor.xParam.xYawMin    = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMin;
  tsensor.xParam.xYawMid    = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMid;
  tsensor.xParam.xYawMax    = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMax;
  
  /* Initialize For CountRate */
  memset(&xTiming, 0, sizeof(Timing_TypeDef));
  
  /* Start ADC */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, ADC_CHANNEL_NUM);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_2);
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  /* Update adcValue, voltage, and calc Yaw */
  if(adcUpdateFlag != 0)
  {
    adcUpdateFlag = 0;
    for(uint32_t i=0; i<ADC_CHANNEL_NUM; i++)
    {
      /* filter ( aX0+(1-a)X1 ) */
      tsensor.xData.xAdcValue[i] = (uint32_t)(0x04*tsensor.xData.xAdcValue[i] + 0x0c*adcValue[i])/0x10;   //75%
      tsensor.xData.xAdcVoltage[i] = (uint32_t)(tsensor.xParam.xADCref)*(tsensor.xData.xAdcValue[i])/0X1000;
    }
  }
  
  /* Update Yaw */
  if(tsensor.xData.xAdcVoltage[0] < tsensor.xParam.xYawMin)
  {
    tsensor.xData.xYaw = -100;
  }
  else if(tsensor.xData.xAdcVoltage[0] < tsensor.xParam.xYawMid)
  {
    tsensor.xData.xYaw = -100*(int32_t)(tsensor.xParam.xYawMid-tsensor.xData.xAdcVoltage[0])/(tsensor.xParam.xYawMid-tsensor.xParam.xYawMin);
  }
  else if(tsensor.xData.xAdcVoltage[0] < tsensor.xParam.xYawMax)
  {
    tsensor.xData.xYaw = 100*(int32_t)(tsensor.xData.xAdcVoltage[0]-tsensor.xParam.xYawMid)/(tsensor.xParam.xYawMax-tsensor.xParam.xYawMid);
  }
  else
  {
    tsensor.xData.xYaw = 100;
  }
  
  /* Update Buttons */
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 1)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Update Button State */
    tsensor.xData.xButton = 0;
    if(BSP_BUTTON_Read(6))  { tsensor.xData.xButton |= (0x1<<0); }
    if(BSP_BUTTON_Read(7))  { tsensor.xData.xButton |= (0x1<<1); }
    if(BSP_BUTTON_Read(8))  { tsensor.xData.xButton |= (0x1<<2); }
    if(BSP_BUTTON_Read(9))  { tsensor.xData.xButton |= (0x1<<3); }
  }
  
  /* Update rate, Speed */
  static uint32_t tickNew0 = 0;
  static uint32_t tickLst0 = 0;
  
  tickNew0 = HAL_GetTick();
  if(tickNew0 - tickLst0 >= TIMING_PERIOD)
  {
    /* Update tick */
    tickLst0 = tickNew0;
    
    if(tickNew0 - xTiming.xTickVal[0] > TIMING_DELAY)
    {
      if(xTiming.xTickVal[0] != 0)
      {
        xTiming.xRPM = 60000/(tickNew0 - xTiming.xTickVal[0]);
      }
      else
      {
        xTiming.xRPM = 0;
      }
    }
    else
    {
      xTiming.xRPM = 60000/(xTiming.xTickVal[0] - xTiming.xTickVal[1]);
    }
    
    if(xTiming.xRPM < tsensor.xParam.xSpdMin)
    {
      tsensor.xData.xSpeed = 0;
    }
    else if(xTiming.xRPM < tsensor.xParam.xSpdMax)
    {
      tsensor.xData.xSpeed = 100* (xTiming.xRPM-tsensor.xParam.xSpdMin)/(tsensor.xParam.xSpdMax-tsensor.xParam.xSpdMin);
    }
    else
    {
      tsensor.xData.xSpeed = 100;
    }
  }
  
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
  * @brief  EXTI Interrupt
  * @param  GPIO_PIN
  * @retval none
  * @note   INT mode
  */
void HAL_GPIO_EXTI_Callback_EXTI5(uint16_t GPIO_Pin)
{
  uint32_t tickNew = HAL_GetTick();
  if(tickNew - xTiming.xTickVal[0] > 10)
  {
    for(uint32_t i=TIMING_TICKBUFF; i>0; i--)
    {
      xTiming.xTickVal[i] = xTiming.xTickVal[i-1];
    }
    xTiming.xTickVal[0] = tickNew;
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
