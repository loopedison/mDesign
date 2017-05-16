/**
  ******************************************************************************
  * @file    application.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Application
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "tcontrol.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "superled.h"
#include "tsensor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* Controller */
Tcontrol_TypeDef tcontroller;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//==============================================================================
/**
  * @brief  Tcontrol Init
  * @param  None
  * @retval None
  */
void Tcontrol_Init(void)
{
  /* Init Device */
  BSP_TIM3_Init();
  BSP_MOTOR_Init();
  
  /* Start Motor */
  BSP_MOTOR_SetEnable(true);    //turn on
  BSP_MOTOR_SetDir(0, 1);       //right
  BSP_MOTOR_SetDir(1, 1);       //right
}

//==============================================================================
/**
  * @brief  Tcontrol_Task
  * @param  argument
  * @retval none
  */
void Tcontrol_Task(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 10)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Upload Motor */
    if(tsensor.xData.xKey != 0)
    {
      if(tcontroller.xMotorRate[0] <= 95)  {tcontroller.xMotorRate[0] += 5;}
      if(tcontroller.xMotorRate[1] <= 60)  {tcontroller.xMotorRate[1] += 5;}
    }
    else
    {
      if(tcontroller.xMotorRate[0] >=  5)  {tcontroller.xMotorRate[0] -= 5;}
      if(tcontroller.xMotorRate[1] >=  5)  {tcontroller.xMotorRate[1] -= 5;}
    }
    
    BSP_TIM3_SetChannelRate(0, tcontroller.xMotorRate[0]);
    BSP_TIM3_SetChannelRate(1, tcontroller.xMotorRate[1]);
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
