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
/* adc vref+ /mv */
#define ADC_CHANNEL_NUM     BSP_ADC1_CHANNEL_NUM
/* adc result */
static uint16_t adcValue[4] = {0};
static uint32_t adcUpdateFlag = 0;

//==============================================================================
/* keys value */
static uint8_t  buttonState = 0;

//==============================================================================
/* Counter */
static int32_t  counterA = 0;

//==============================================================================
/* rate */
#define RATE_PERIOD     (1000)    //ms
#define RATE_DIV        ( 100)
#define RATE_DB_SIZE    (RATE_DIV)

typedef struct
{
  int32_t               iFlag;
  int32_t               xCnt;
}Rate_DataDef;
typedef struct
{
  Rate_DataDef          pDB[RATE_DB_SIZE];
}Rate_TypeDef;
static Rate_TypeDef     xRate;

//==============================================================================
/* sensor */
Tsensor_TypeDef tsensor;

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
  BSP_BUTTON_Init(BUTTON_1);
  BSP_BUTTON_Init(BUTTON_2);
  BSP_BUTTON_Init(BUTTON_3);
  BSP_BUTTON_Init(BUTTON_4);
  
  /* Clear tsensor */
  memset(&tsensor, 0, sizeof(Tsensor_TypeDef));
  
  /* Initialize For ADC */
  tsensor.xParam.xADCref    = *(uint16_t *)hStorageMsgData.xUserParam.xParamADCref;
  /* Initialize For tsensor */
  tsensor.xParam.xSpeedMin   = *(uint16_t *)hStorageMsgData.xUserParam.xParamSpeedMin;
  tsensor.xParam.xSpeedMax   = *(uint16_t *)hStorageMsgData.xUserParam.xParamSpeedMax;
  tsensor.xParam.xYawMin  = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMin;
  tsensor.xParam.xYawMid  = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMid;
  tsensor.xParam.xYawMax  = *(uint16_t *)hStorageMsgData.xUserParam.xParamYawMax;
  /* Start ADC */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, ADC_CHANNEL_NUM);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_2);
  
  /* Initialize For Rate */
  for(uint32_t i=0; i<RATE_DB_SIZE; i++)
  {
    xRate.pDB[i].iFlag = i;
    xRate.pDB[i].xCnt = 0;
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
  if(BSP_EXTI6_ReadState() == GPIO_PIN_RESET)
  { counterA -= 1; }
  else
  { counterA += 1; }
}
void HAL_GPIO_EXTI_Callback_EXTI6(uint16_t GPIO_Pin)
{
}

//==============================================================================
/**
  * @brief  Tsensor Rate Calc
  * @param  pRate   :
  * @param  pCounter  :
  * @param  pValue  :
  * @retval rate value
  */
uint32_t Tsensor_RateCalc(Rate_TypeDef *pRate, int32_t *pCounter, int32_t *pValue)
{
  int32_t xVal = 0;
  for(uint32_t i=0; i<RATE_DB_SIZE; i++)
  {
    pRate->pDB[i].iFlag += 1;
    if(pRate->pDB[i].iFlag >= RATE_DB_SIZE)
    {
      pRate->pDB[i].iFlag = 0;
      xVal = *pCounter - pRate->pDB[i].xCnt;
      pRate->pDB[i].xCnt = *pCounter;
    }
  }
  *pValue = xVal;
  return (0);
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  /* Update Key State */
  buttonState = 0;
  if(BSP_BUTTON_Read(BUTTON_1) == true)
  {
    buttonState |= (0x1<<0);
  }
  if(BSP_BUTTON_Read(BUTTON_2) == true)
  {
    buttonState |= (0x1<<1);
  }
  if(BSP_BUTTON_Read(BUTTON_3) == true)
  {
    buttonState |= (0x1<<2);
  }
  if(BSP_BUTTON_Read(BUTTON_4) == true)
  {
    buttonState |= (0x1<<3);
  }
  tsensor.xData.xButton = buttonState;
  
  /* Update adcValue, voltage, and calc Yaw */
  if(adcUpdateFlag != 0)
  {
    adcUpdateFlag = 0;
    
    for(uint32_t i=0; i<ADC_CHANNEL_NUM; i++)
    {
      /* filter ( aX0+(1-a)X1 ) */
      tsensor.xData.xAdcValue[i] = (0x04*tsensor.xData.xAdcValue[i] + 0x0c*adcValue[i])/0x10;
      tsensor.xData.xAdcVoltage[i] = (tsensor.xParam.xADCref)*(tsensor.xData.xAdcValue[i])/0X1000;
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
  }
  
  /* Update rate, Speed */
  static uint32_t tickRate = 0;
  uint32_t tickNew = HAL_GetTick();
  if(tickNew - tickRate >= RATE_PERIOD/RATE_DIV)
  {
    tickRate = tickNew;
    Tsensor_RateCalc(&xRate, &counterA, &tsensor.xData.xRateValue);
    
    if(tsensor.xData.xRateValue < (-1)*tsensor.xParam.xSpeedMax)
    {
      tsensor.xData.xSpeed = -100;
    }
    else if(tsensor.xData.xRateValue < (-1)*tsensor.xParam.xSpeedMin)
    {
      tsensor.xData.xSpeed = -100*(-tsensor.xData.xRateValue-tsensor.xParam.xSpeedMin) \
                                /(tsensor.xParam.xSpeedMax-tsensor.xParam.xSpeedMin);
    }
    else if(tsensor.xData.xRateValue < (1)*tsensor.xParam.xSpeedMin)
    {
      tsensor.xData.xSpeed = 0;
    }
    else if(tsensor.xData.xRateValue < (1)*tsensor.xParam.xSpeedMax)
    {
      tsensor.xData.xSpeed = 100*(tsensor.xData.xRateValue-tsensor.xParam.xSpeedMin)  \
                                /(tsensor.xParam.xSpeedMax-tsensor.xParam.xSpeedMin);
    }
    else
    {
      tsensor.xData.xSpeed = 100;
    }
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
