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

//==============================================================================
/* Ticks */
static uint32_t   xCntState=0;
static uint32_t   xCntY=0, xCntN=0;

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
  BSP_LEDOUT_Init(6);
  BSP_BUTTON_Init(7);
  /* Load Param */
  memset(&tcontroller, 0, sizeof(Tcontrol_TypeDef));
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
    
    /* Update ticket pin */
    if(BSP_BUTTON_Read(7))
    {
      xCntY ++;
      xCntN = 0;
    }
    else
    {
      xCntY = 0;
      xCntN ++;
    }
    
    /* Update ticket control */
    if(tcontroller.xTicketCurrent == 0)
    {
      BSP_LEDOUT_WriteState(6,1);
      xCntState = 0;
    }
    else
    {
      if(xCntState == 0)          //idle
      {
        BSP_LEDOUT_WriteState(6,0);
        xCntState = 1;
        xCntY = 0;
        xCntN = 0;
      }
      else if(xCntState == 1)     //start
      {
        if(xCntY >= 4)
        {
          xCntState = 2;
        }
      }
      else if(xCntState == 2)     //ON
      {
        if(xCntN >= 4)
        {
          xCntState = 3;
        }
      }
      else if(xCntState == 3)     //stop
      {
        xCntState = 0;
        if(tcontroller.xTicketCurrent > 0)
        {
          tcontroller.xTicketCurrent --;
        }
        tcontroller.xTicketTotal ++;
      }
      
      /* Reset if stay busy */
      if(xCntY >= 200)
      {
        xCntState = 0;
      }
    }
    
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
