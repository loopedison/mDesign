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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* Coin Pluse >=20ms */
#define S_IDLE    (0X0)
#define S_BUSY    (0X1)
#define S_DONE    (0X2)
static uint32_t   xCntState;
static uint32_t   xCntY, xCntN;
static uint32_t   xCoinCounter;
static uint32_t   xCoinError;

//==============================================================================
/* sensor */
Tsensor_TypeDef   tsensor;

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
  BSP_EXTI5_Init();
  BSP_EXTI6_Init();
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Load Param */
  tsensor.xParam.xPeriod = 1;
  
  xCntState = S_IDLE;
  xCntY = 0;
  xCntN = 0;
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  static uint32_t tickButton = 0;
  uint32_t tickNew = HAL_GetTick();
  
  if(tickNew - tickButton >= tsensor.xParam.xPeriod)
  {
    tickButton = tickNew;
    
    /* update coin pin state */
    if(BSP_EXTI5_ReadState() == GPIO_PIN_RESET)
    {
      xCntY ++;
      xCntN = 0;
    }
    else
    {
      xCntY = 0;
      xCntN ++;
    }
    
    if(xCntState == S_IDLE)
    {
      if(xCntY >= 2)
      {
        xCntState = S_BUSY;
      }
    }
    else if(xCntState == S_BUSY)
    {
      if(xCntY >= 15)
      {
        xCntState = S_DONE;
      }
      if(xCntN >= 2)
      {
        xCoinError ++;
        xCntState = S_IDLE;
      }
    }
    else if(xCntState == S_DONE)
    {
      if(xCntN >= 2)
      {
        xCoinCounter ++;
        xCntState = S_IDLE;
      }
    }
    else
    {
      xCntState = S_IDLE;
    }
    
    if(xCntY >= 200)
    {
      xCntState = S_IDLE;
    }
    
    /* update coin counter */
    tsensor.xData.xCoin = xCoinCounter;
    
    /* update key state */
    tsensor.xData.xKey = 0;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
      tsensor.xData.xKey |= (0x1<<0);
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
      tsensor.xData.xKey |= (0x1<<1);
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
