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
#define KEY_NUM                     (4)

#define KEY1_GPIO_PIN               GPIO_PIN_6
#define KEY1_GPIO_PORT              GPIOB
#define KEY1_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define KEY1_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define KEY2_GPIO_PIN               GPIO_PIN_7
#define KEY2_GPIO_PORT              GPIOB
#define KEY2_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define KEY2_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define KEY3_GPIO_PIN               GPIO_PIN_8
#define KEY3_GPIO_PORT              GPIOB
#define KEY3_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define KEY3_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define KEY4_GPIO_PIN               GPIO_PIN_9
#define KEY4_GPIO_PORT              GPIOB
#define KEY4_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define KEY4_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define KEYx_GPIO_CLK_ENABLE(__INDEX__)                       \
        do{                                                   \
            if((__INDEX__) == KEY1) KEY1_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == KEY2) KEY2_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == KEY3) KEY3_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == KEY4) KEY4_GPIO_CLK_ENABLE();   \
        }while(0)

#define KEYx_GPIO_CLK_DISABLE(__INDEX__)                      \
        do{                                                   \
            if((__INDEX__) == KEY1) KEY1_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == KEY2) KEY2_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == KEY3) KEY3_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == KEY4) KEY4_GPIO_CLK_DISABLE();  \
        }while(0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @note : with 0,1,2,3
  */
static GPIO_TypeDef* KEY_GPIO_PORT[KEY_NUM] =
{
  KEY1_GPIO_PORT,
  KEY2_GPIO_PORT,
  KEY3_GPIO_PORT,
  KEY4_GPIO_PORT,
};
static const uint16_t KEY_GPIO_PIN[KEY_NUM] =
{
  KEY1_GPIO_PIN,
  KEY2_GPIO_PIN,
  KEY3_GPIO_PIN,
  KEY4_GPIO_PIN,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial key
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_KEY_Init(KEY_TypeDef keyID)
{
  /* Enable the GPIO_KEY clock */
  KEYx_GPIO_CLK_ENABLE(keyID);
  
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
  * @brief  read key state
  * @param  key, id to read 
  * @param  *state, to store state 
  * @retval BSP_OK: normal
  */
BSP_StatusTypeDef BSP_KEY_ReadState(KEY_TypeDef keyID, uint32_t *state)
{
  *state = (HAL_GPIO_ReadPin(KEY_GPIO_PORT[keyID], KEY_GPIO_PIN[keyID]) == GPIO_PIN_RESET);
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  read key state
  * @param  key, id to read 
  * @retval state
  */
bool BSP_KEY_Read(KEY_TypeDef keyID)
{
  return (HAL_GPIO_ReadPin(KEY_GPIO_PORT[keyID], KEY_GPIO_PIN[keyID]) == GPIO_PIN_RESET);
}

#endif /* BSP_KEY_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
