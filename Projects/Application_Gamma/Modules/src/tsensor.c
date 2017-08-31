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
  
  if(tickNew - tickButton >= 1)
  {
    tickButton = tickNew;
    
    /* update button */
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
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
