/**
  ******************************************************************************
  * @file    thandler.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   handler for sensor with control
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "thandler.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "superled.h"
#include "tsensor.h"
#include "tcontrol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* handler */
Thandler_TypeDef thandler;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Thandler_Init
  * @param  None
  * @retval None
  */
void Thandler_Init(void)
{
  /* Loading Param */
  memset(&thandler, 0, sizeof(Thandler_DataTypeDef));
}

//==============================================================================
/**
  * @brief  Thandler_Task
  * @param  argument
  * @retval none
  */
void Thandler_Task(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 10)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Upload Motor */
    if(tsensor.xData.xButton != 0)
    {
      if(tcontroller.xData.xMotorRate[0] <= 90)  {tcontroller.xData.xMotorRate[0] += 2;}
      if(tcontroller.xData.xMotorRate[1] <= 50)  {tcontroller.xData.xMotorRate[1] += 2;}
    }
    else
    {
      if(tcontroller.xData.xMotorRate[0] >=  2)  {tcontroller.xData.xMotorRate[0] -= 2;}
      if(tcontroller.xData.xMotorRate[1] >=  2)  {tcontroller.xData.xMotorRate[1] -= 2;}
    }
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
