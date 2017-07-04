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
static uint32_t   xCntState;
static uint32_t   xCntY, xCntN;

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
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* PreSet */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  
  /* Load Param */
  tcontroller.xTicketTotal = 0;
  tcontroller.xTicketCurrent = 0;
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
    
    /* Update ticket control */
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET)
    {
      xCntY ++;
      xCntN = 0;
    }
    else
    {
      xCntY = 0;
      xCntN ++;
    }
    
    if(tcontroller.xTicketCurrent == 0)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
      xCntState = 0;
    }
    else
    {
      if(xCntState == 0)
      {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
        xCntState = 1;
        xCntY = 0;
        xCntN = 0;
      }
      else if(xCntState == 1)
      {
        if(xCntY >= 4)
        {
          xCntState = 2;
        }
      }
      else if(xCntState == 2)
      {
        if(xCntN >= 4)
        {
          xCntState = 3;
        }
      }
      else if(xCntState == 3)
      {
        xCntState = 0;
        if(tcontroller.xTicketCurrent > 0)
        {
          tcontroller.xTicketCurrent --;
        }
        tcontroller.xTicketTotal ++;
      }
      
      /* If Error */
      if(xCntY >= 200)
      {
        xCntState = 0;
      }
    }
    
  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
