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
#define MIN(a,b)    (((a)<(b))?(a):(b))

//==============================================================================
/* Counter */
static uint32_t   counterA = 0;
static uint32_t   counterB = 0;

//==============================================================================
/* rate */
typedef struct
{
  uint32_t              iFlag;
  uint32_t              xCnt;
}Tsensor_RateDataDef;

typedef struct
{
  uint32_t              xT;
  uint32_t              xD;
  uint32_t              xVal;
  Tsensor_RateDataDef   *pDB;
}Tsensor_RateDef;

static Tsensor_RateDef  tRateA;
static Tsensor_RateDef  tRateB;

//==============================================================================
/* sensor */
Tsensor_TypeDef tsensor;

/* Private function prototypes -----------------------------------------------*/
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue);
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
  BSP_EXTI5_Init();
  BSP_EXTI6_Init();
  
  /* Initialize For Counter */
  counterA = 0;
  counterB = 0;
  
  /* Initialize For Rate */
  tsensor.xParam.xPeriod = 5000;
  
  /* Initialize For Rate */
  tRateA.xT = tsensor.xParam.xPeriod;
  tRateA.xD = 50;
  tRateA.xVal = 0;
  tRateA.pDB = malloc(tRateA.xD*sizeof(Tsensor_RateDataDef));
  ASSERT(tRateA.pDB != NULL);
  for(uint32_t i=0; i<tRateA.xD; i++)
  {
    tRateA.pDB[i].iFlag = i;
    tRateA.pDB[i].xCnt = 0;
  }
  
  tRateB.xT = tsensor.xParam.xPeriod;
  tRateB.xD = 50;
  tRateB.xVal = 0;
  tRateB.pDB = malloc(tRateB.xD*sizeof(Tsensor_RateDataDef));
  ASSERT(tRateB.pDB != NULL);
  for(uint32_t i=0; i<tRateB.xD; i++)
  {
    tRateB.pDB[i].iFlag = i;
    tRateB.pDB[i].xCnt = 0;
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
  static uint32_t tickRateA = 0;
  static uint32_t tickRateB = 0;
  uint32_t tickNew = 0;
  
  tickNew = HAL_GetTick();
  
  /* update rate */
  if(tickNew - tickRateA >= tRateA.xT/tRateA.xD)
  {
    tickRateA = tickNew;
    Tsensor_RateCalc(&tRateA, counterA);
  }
  
  /* update rate */
  if(tickNew - tickRateB >= tRateB.xT/tRateB.xD)
  {
    tickRateB = tickNew;
    Tsensor_RateCalc(&tRateB, counterB);
  }
  
  /* Make Info */
  tsensor.xData.xRate = MIN(tRateA.xVal, tRateB.xVal);
  if(tsensor.xData.xRate >= 1)            {tsensor.xData.xRate -= 1;}
  else                                    {tsensor.xData.xRate = 0;}
  tsensor.xData.xDirection = tRateA.xVal - tRateB.xVal;
  if(tsensor.xData.xDirection >= 2)       {tsensor.xData.xDirection -= 2;}
  else if(tsensor.xData.xDirection <= -2) {tsensor.xData.xDirection += 2;}
  else                                    {tsensor.xData.xDirection = 0;}
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
  counterA += 1;
}
void HAL_GPIO_EXTI_Callback_EXTI6(uint16_t GPIO_Pin)
{
  counterB += 1;
}

//==============================================================================
/**
  * @brief  Tsensor Rate Calc
  * @param  pRate   :
  * @param  iValue  :
  * @retval rate value
  */
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue)
{
  for(uint32_t i=0; i<pRate->xD; i++)
  {
    pRate->pDB[i].iFlag += 1;
    if(pRate->pDB[i].iFlag >= pRate->xD)
    {
      pRate->pDB[i].iFlag = 0;
      pRate->xVal = iValue - pRate->pDB[i].xCnt;
      pRate->pDB[i].xCnt = iValue;
    }
  }
  return (0);
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
