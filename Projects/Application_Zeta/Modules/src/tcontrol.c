/**
  ******************************************************************************
  * @file    tcontrol.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   tcontrol
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
  BSP_DRIVER_MOTOR_Init();
  /* Loading Param */
  memset(&tcontroller.xData.xMotorRate, 0, sizeof(Tcontrol_DataTypeDef));
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
  if(tickNew - tickLst >= 1)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Update Motor rate */
    for(uint32_t i=0; i<2; i++)
    {
      BSP_DRIVER_MOTOR_Set(i, -tcontroller.xData.xMotorRate[i]);
    }
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
