/**
  ******************************************************************************
  * @file    bsp_button.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BUTTONs
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_BUTTON_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef BUTTON_0_CONFIG
  #define BUTTON_0_GPIO_PIN               GPIO_PIN_1
  #define BUTTON_0_GPIO_PORT              GPIOA
#endif /* BUTTON_0_CONFIG */

#ifndef BUTTON_1_CONFIG
  #define BUTTON_1_GPIO_PIN               GPIO_PIN_2
  #define BUTTON_1_GPIO_PORT              GPIOA
#endif /* BUTTON_1_CONFIG */

#ifndef BUTTON_2_CONFIG
  #define BUTTON_2_GPIO_PIN               GPIO_PIN_3
  #define BUTTON_2_GPIO_PORT              GPIOA
#endif /* BUTTON_2_CONFIG */

#ifndef BUTTON_3_CONFIG
  #define BUTTON_3_GPIO_PIN               GPIO_PIN_4
  #define BUTTON_3_GPIO_PORT              GPIOA
#endif /* BUTTON_3_CONFIG */

#ifndef BUTTON_4_CONFIG
  #define BUTTON_4_GPIO_PIN               GPIO_PIN_5
  #define BUTTON_4_GPIO_PORT              GPIOA
#endif /* BUTTON_4_CONFIG */

#ifndef BUTTON_5_CONFIG
  #define BUTTON_5_GPIO_PIN               GPIO_PIN_6
  #define BUTTON_5_GPIO_PORT              GPIOA
#endif /* BUTTON_5_CONFIG */

#ifndef BUTTON_6_CONFIG
  #define BUTTON_6_GPIO_PIN               GPIO_PIN_0
  #define BUTTON_6_GPIO_PORT              GPIOB
#endif /* BUTTON_6_CONFIG */

#ifndef BUTTON_7_CONFIG
  #define BUTTON_7_GPIO_PIN               GPIO_PIN_1
  #define BUTTON_7_GPIO_PORT              GPIOB
#endif /* BUTTON_7_CONFIG */

#ifndef BUTTON_8_CONFIG
  #define BUTTON_8_GPIO_PIN               GPIO_PIN_4
  #define BUTTON_8_GPIO_PORT              GPIOB
#endif /* BUTTON_8_CONFIG */

#ifndef BUTTON_9_CONFIG
  #define BUTTON_9_GPIO_PIN               GPIO_PIN_5
  #define BUTTON_9_GPIO_PORT              GPIOB
#endif /* BUTTON_9_CONFIG */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @note : with 0,1,2,3
  */
static GPIO_TypeDef* BUTTON_GPIO_PORT[10] =
{
  BUTTON_0_GPIO_PORT,
  BUTTON_1_GPIO_PORT,
  BUTTON_2_GPIO_PORT,
  BUTTON_3_GPIO_PORT,
  BUTTON_4_GPIO_PORT,
  BUTTON_5_GPIO_PORT,
  BUTTON_6_GPIO_PORT,
  BUTTON_7_GPIO_PORT,
  BUTTON_8_GPIO_PORT,
  BUTTON_9_GPIO_PORT,
};
static const uint16_t BUTTON_GPIO_PIN[10] =
{
  BUTTON_0_GPIO_PIN,
  BUTTON_1_GPIO_PIN,
  BUTTON_2_GPIO_PIN,
  BUTTON_3_GPIO_PIN,
  BUTTON_4_GPIO_PIN,
  BUTTON_5_GPIO_PIN,
  BUTTON_6_GPIO_PIN,
  BUTTON_7_GPIO_PIN,
  BUTTON_8_GPIO_PIN,
  BUTTON_9_GPIO_PIN,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial BUTTON
  * @param  buttonID: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BUTTON_Init(uint32_t buttonID)
{
  if(buttonID > 9)  {return (BSP_ERROR);}
  
  /* Enable the GPIO_KEY clock */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  
  /* Configure the GPIO_KEY pin */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = BUTTON_GPIO_PIN[buttonID];
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUTTON_GPIO_PORT[buttonID], &GPIO_InitStruct);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_BUTTON_ReadState
  * @param  key, id to read 
  * @param  *state, to store state 
  * @retval BSP_OK: normal
  */
BSP_StatusTypeDef BSP_BUTTON_ReadState(uint32_t buttonID, uint32_t *state)
{
  if(buttonID > 9)  {return (BSP_ERROR);}
  *state = (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT[buttonID], BUTTON_GPIO_PIN[buttonID]) == GPIO_PIN_RESET);
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_BUTTON_Read
  * @param  key, id to read 
  * @retval state
  */
bool BSP_BUTTON_Read(uint32_t buttonID)
{
  if(buttonID > 9)  {return (false);}
  return (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT[buttonID], BUTTON_GPIO_PIN[buttonID]) == GPIO_PIN_RESET);
}


#endif /* BSP_BUTTON_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
