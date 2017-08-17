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
  BSP_BUTTON_Init(4);
  BSP_BUTTON_Init(5);
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
    
    /* Update button */
    tsensor.xData.xButton = 0;
    if(BSP_BUTTON_Read(4))
    {
      tsensor.xData.xButton |= (0x1<<0);
    }
    if(BSP_BUTTON_Read(5))
    {
      tsensor.xData.xButton |= (0x1<<1);
    }
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
