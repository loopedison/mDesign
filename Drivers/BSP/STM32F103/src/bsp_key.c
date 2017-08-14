/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for KEYs
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_KEY_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef KEY_0_CONFIG
  #define KEY_0_GPIO_PIN            GPIO_PIN_6
  #define KEY_0_GPIO_PORT           GPIOB
#endif /* KEY_0_CONFIG */

#ifndef KEY_1_CONFIG
  #define KEY_1_GPIO_PIN            GPIO_PIN_7
  #define KEY_1_GPIO_PORT           GPIOB
#endif /* KEY_1_CONFIG */

#ifndef KEY_2_CONFIG
  #define KEY_2_GPIO_PIN            GPIO_PIN_8
  #define KEY_2_GPIO_PORT           GPIOB
#endif /* KEY_2_CONFIG */

#ifndef KEY_3_CONFIG
  #define KEY_3_GPIO_PIN            GPIO_PIN_9
  #define KEY_3_GPIO_PORT           GPIOB
#endif /* KEY_3_CONFIG */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @note : with 0,1,2,3
  */
static GPIO_TypeDef* KEY_GPIO_PORT[4] =
{
  KEY_0_GPIO_PORT,
  KEY_1_GPIO_PORT,
  KEY_2_GPIO_PORT,
  KEY_3_GPIO_PORT,
};
static const uint16_t KEY_GPIO_PIN[4] =
{
  KEY_0_GPIO_PIN,
  KEY_1_GPIO_PIN,
  KEY_2_GPIO_PIN,
  KEY_3_GPIO_PIN,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial key
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_KEY_Init(uint32_t keyID)
{
  if(keyID > 4)  {return (BSP_ERROR);}
  
  /* Enable the GPIO_KEY clock */
  __GPIOB_CLK_ENABLE();
  
  /* Configure the GPIO_KEY pin */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = KEY_GPIO_PIN[keyID];
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY_GPIO_PORT[keyID], &GPIO_InitStruct);
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_KEY_ReadState
  * @param  key, id to read 
  * @param  *state, to store state 
  * @retval BSP_OK: normal
  */
BSP_StatusTypeDef BSP_KEY_ReadState(uint32_t keyID, uint32_t *state)
{
  if(keyID > 4)  {return (BSP_ERROR);}
  *state = (HAL_GPIO_ReadPin(KEY_GPIO_PORT[keyID], KEY_GPIO_PIN[keyID]) == GPIO_PIN_RESET);
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  read key state
  * @param  key, id to read 
  * @retval state
  */
bool BSP_KEY_Read(uint32_t keyID)
{
  if(keyID > 4)  {return (false);}
  return (HAL_GPIO_ReadPin(KEY_GPIO_PORT[keyID], KEY_GPIO_PIN[keyID]) == GPIO_PIN_RESET);
}

#endif /* BSP_KEY_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
