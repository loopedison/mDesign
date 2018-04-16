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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* Controller */
Tcontrol_TypeDef tcontroller;

//==============================================================================
/* Click */
typedef struct
{
  uint8_t       xStatus;
}Click_TypeDef;
static Click_TypeDef tClick;

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* Configure GPIO pins */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* PreSet */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*Configure GPIO pins : PA5*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  
  
  /* Load Param */
  tClick.xStatus = 0;
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
  static uint32_t tickL=0,tickH=0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 1)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Update click */
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
    {
      tickL ++;
      if(tickL >= 2)  { tickH = 0; }
    }
    else
    {
      tickH ++;
      if(tickH >= 2)  { tickL = 0; }
    }
    
    
    /* Update click */
    if(tClick.xStatus == 0)
    {
      if(tickL > 50)
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(0), GPIO_PIN_RESET);
        HAL_Delay(400);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(0), GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(0), GPIO_PIN_RESET);
        HAL_Delay(300);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(0), GPIO_PIN_SET);
        HAL_Delay(200);
        tClick.xStatus = 1;
      }
    }
    else if(tClick.xStatus == 1)
    {
      if(tickH > 50)
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_RESET);
        HAL_Delay(400);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_RESET);
        HAL_Delay(400);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_RESET);
        HAL_Delay(400);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1<<(2), GPIO_PIN_SET);
        HAL_Delay(200);
        tClick.xStatus = 0;
      }
    }
  }
}

//==============================================================================
/**
  * @brief  This function handles EXTI line95 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void HAL_GPIO_EXTI_Callback_EXTI5(uint16_t GPIO_Pin)
{
//  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
//  {
//    tClick.xStatus = 1;
//  }
//  else
//  {
//    tClick.xStatus = 2;
//  }
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
