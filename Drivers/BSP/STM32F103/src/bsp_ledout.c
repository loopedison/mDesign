/**
  ******************************************************************************
  * @file    bsp_ledout.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for LEDs
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_LEDOUT_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef LEDOUT_0_CONFIG
  #define LEDOUT_0_GPIO_PIN               GPIO_PIN_1
  #define LEDOUT_0_GPIO_PORT              GPIOA
#endif /* LEDOUT_0_CONFIG */

#ifndef LEDOUT_1_CONFIG
  #define LEDOUT_1_GPIO_PIN               GPIO_PIN_2
  #define LEDOUT_1_GPIO_PORT              GPIOA
#endif /* LEDOUT_1_CONFIG */

#ifndef LEDOUT_2_CONFIG
  #define LEDOUT_2_GPIO_PIN               GPIO_PIN_3
  #define LEDOUT_2_GPIO_PORT              GPIOA
#endif /* LEDOUT_2_CONFIG */

#ifndef LEDOUT_3_CONFIG
  #define LEDOUT_3_GPIO_PIN               GPIO_PIN_4
  #define LEDOUT_3_GPIO_PORT              GPIOA
#endif /* LEDOUT_3_CONFIG */

#ifndef LEDOUT_4_CONFIG
  #define LEDOUT_4_GPIO_PIN               GPIO_PIN_5
  #define LEDOUT_4_GPIO_PORT              GPIOA
#endif /* LEDOUT_4_CONFIG */

#ifndef LEDOUT_5_CONFIG
  #define LEDOUT_5_GPIO_PIN               GPIO_PIN_6
  #define LEDOUT_5_GPIO_PORT              GPIOA
#endif /* LEDOUT_5_CONFIG */

#ifndef LEDOUT_6_CONFIG
  #define LEDOUT_6_GPIO_PIN               GPIO_PIN_4
  #define LEDOUT_6_GPIO_PORT              GPIOB
#endif /* LEDOUT_6_CONFIG */

#ifndef LEDOUT_7_CONFIG
  #define LEDOUT_7_GPIO_PIN               GPIO_PIN_5
  #define LEDOUT_7_GPIO_PORT              GPIOB
#endif /* LEDOUT_7_CONFIG */

#ifndef LEDOUT_8_CONFIG
  #define LEDOUT_8_GPIO_PIN               GPIO_PIN_0
  #define LEDOUT_8_GPIO_PORT              GPIOB
#endif /* LEDOUT_8_CONFIG */

#ifndef LEDOUT_9_CONFIG
  #define LEDOUT_9_GPIO_PIN               GPIO_PIN_1
  #define LEDOUT_9_GPIO_PORT              GPIOB
#endif /* LEDOUT_9_CONFIG */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @note : with 0,1,2,3
  */
static GPIO_TypeDef* LEDOUT_GPIO_PORT[10] =
{
  LEDOUT_0_GPIO_PORT,
  LEDOUT_1_GPIO_PORT,
  LEDOUT_2_GPIO_PORT,
  LEDOUT_3_GPIO_PORT,
  LEDOUT_4_GPIO_PORT,
  LEDOUT_5_GPIO_PORT,
  LEDOUT_6_GPIO_PORT,
  LEDOUT_7_GPIO_PORT,
  LEDOUT_8_GPIO_PORT,
  LEDOUT_9_GPIO_PORT,
};
static const uint16_t LEDOUT_GPIO_PIN[10] =
{
  LEDOUT_0_GPIO_PIN,
  LEDOUT_1_GPIO_PIN,
  LEDOUT_2_GPIO_PIN,
  LEDOUT_3_GPIO_PIN,
  LEDOUT_4_GPIO_PIN,
  LEDOUT_5_GPIO_PIN,
  LEDOUT_6_GPIO_PIN,
  LEDOUT_7_GPIO_PIN,
  LEDOUT_8_GPIO_PIN,
  LEDOUT_9_GPIO_PIN,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  BSP_LEDOUT_Init
  * @param  ledID: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_LEDOUT_Init(uint32_t ledID)
{
  if(ledID > 9)  {return (BSP_ERROR);}
  
  /* Enable the GPIO_KEY clock */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  
  /* Configure the GPIO_KEY pin */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LEDOUT_GPIO_PIN[ledID];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LEDOUT_GPIO_PORT[ledID], &GPIO_InitStruct);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_LEDOUT_WriteState
  * @param  ledID
  * @param  ledState
  * @retval BSP_OK: normal
  */
BSP_StatusTypeDef BSP_LEDOUT_WriteState(uint32_t ledID, uint32_t ledState)
{
  if(ledID > 9)  {return (BSP_ERROR);}
  HAL_GPIO_WritePin(LEDOUT_GPIO_PORT[ledID], LEDOUT_GPIO_PIN[ledID], (ledState!=0)?(GPIO_PIN_SET):(GPIO_PIN_RESET));
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_LEDOUT_Toggle
  * @param  ledID
  * @retval state
  */
BSP_StatusTypeDef BSP_LEDOUT_Toggle(uint32_t ledID)
{
  if(ledID > 9)  {return (BSP_ERROR);}
  HAL_GPIO_TogglePin(LEDOUT_GPIO_PORT[ledID], LEDOUT_GPIO_PIN[ledID]);
  return (BSP_OK);
}


#endif /* BSP_LEDOUT_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
