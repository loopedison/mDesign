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
/* sensor */
Tsensor_TypeDef   tsensor;

//==============================================================================
/* Coin Pluse >=20ms */
static uint32_t   xCntState=0;
static uint32_t   xCntY=0, xCntN=0;
static uint32_t   xCoinCounter=0;

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
  BSP_BUTTON_Init(0);
  BSP_BUTTON_Init(1);
  BSP_BUTTON_Init(2);
  BSP_BUTTON_Init(3);
  BSP_BUTTON_Init(4);
  BSP_BUTTON_Init(5);
  BSP_BUTTON_Init(8);
  BSP_BUTTON_Init(9);
  
  /* Load Param */
  memset(&tsensor.xData, 0, sizeof(Tsensor_DataTypeDef));
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 1)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Update coin pin state */
    if(BSP_BUTTON_Read(8))
    {
      xCntY ++;
      xCntN = 0;
    }
    else
    {
      xCntY = 0;
      xCntN ++;
    }
    
    /* Update coin state */
    if(xCntState == 0)
    {
      if(xCntY >= 2)
      {
        xCntState = 1;
      }
    }
    else if(xCntState == 1)
    {
      if(xCntY >= 16)
      {
        xCntState = 2;
      }
      if(xCntN >= 2)
      {
        xCntState = 0;
      }
    }
    else if(xCntState == 2)
    {
      if(xCntN >= 2)
      {
        xCoinCounter ++;
        xCntState = 0;
      }
    }
    else
    {
      xCntState = 0;
    }
    
    /* Reset if stay busy */
    if(xCntY >= 200)
    {
      xCntState = 0;
    }
    
    /* Update coin counter */
    tsensor.xData.xCoin = xCoinCounter;
    
    /* Update key state */
    tsensor.xData.xButton = 0;
    if(BSP_BUTTON_Read(0))
    {
      tsensor.xData.xButton |= (0x1<<0);
    }
    if(BSP_BUTTON_Read(1))
    {
      tsensor.xData.xButton |= (0x1<<1);
    }
    if(BSP_BUTTON_Read(2))
    {
      tsensor.xData.xButton |= (0x1<<2);
    }
    if(BSP_BUTTON_Read(3))
    {
      tsensor.xData.xButton |= (0x1<<3);
    }
    if(BSP_BUTTON_Read(4))
    {
      tsensor.xData.xButton |= (0x1<<4);
    }
    if(BSP_BUTTON_Read(5))
    {
      tsensor.xData.xButton |= (0x1<<5);
    }
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
