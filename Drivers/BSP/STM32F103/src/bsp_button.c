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
#define BUTTON_NUM                     (4)

#define BUTTON_1_GPIO_PIN               GPIO_PIN_4
#define BUTTON_1_GPIO_PORT              GPIOB
#define BUTTON_1_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define BUTTON_1_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define BUTTON_2_GPIO_PIN               GPIO_PIN_5
#define BUTTON_2_GPIO_PORT              GPIOB
#define BUTTON_2_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define BUTTON_2_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define BUTTON_3_GPIO_PIN               GPIO_PIN_0
#define BUTTON_3_GPIO_PORT              GPIOB
#define BUTTON_3_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define BUTTON_3_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define BUTTON_4_GPIO_PIN               GPIO_PIN_1
#define BUTTON_4_GPIO_PORT              GPIOB
#define BUTTON_4_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define BUTTON_4_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)                            \
        do{                                                           \
            if((__INDEX__) == BUTTON_1) BUTTON_1_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == BUTTON_2) BUTTON_2_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == BUTTON_3) BUTTON_3_GPIO_CLK_ENABLE();   \
            if((__INDEX__) == BUTTON_4) BUTTON_4_GPIO_CLK_ENABLE();   \
        }while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)                           \
        do{                                                           \
            if((__INDEX__) == BUTTON_1) BUTTON_1_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == BUTTON_2) BUTTON_2_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == BUTTON_3) BUTTON_3_GPIO_CLK_DISABLE();  \
            if((__INDEX__) == BUTTON_4) BUTTON_4_GPIO_CLK_DISABLE();  \
        }while(0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @note : with 0,1,2,3
  */
static GPIO_TypeDef* BUTTON_GPIO_PORT[BUTTON_NUM] =
{
  BUTTON_1_GPIO_PORT,
  BUTTON_2_GPIO_PORT,
  BUTTON_3_GPIO_PORT,
  BUTTON_4_GPIO_PORT,
};
static const uint16_t BUTTON_GPIO_PIN[BUTTON_NUM] =
{
  BUTTON_1_GPIO_PIN,
  BUTTON_2_GPIO_PIN,
  BUTTON_3_GPIO_PIN,
  BUTTON_4_GPIO_PIN,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial BUTTON
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_BUTTON_Init(BUTTON_TypeDef buttonID)
{
  /* Enable the GPIO_KEY clock */
  BUTTONx_GPIO_CLK_ENABLE(buttonID);
  
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
BSP_StatusTypeDef BSP_BUTTON_ReadState(BUTTON_TypeDef buttonID, uint32_t *state)
{
  *state = (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT[buttonID], BUTTON_GPIO_PIN[buttonID]) == GPIO_PIN_RESET);
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP_BUTTON_Read
  * @param  key, id to read 
  * @retval state
  */
bool BSP_BUTTON_Read(BUTTON_TypeDef buttonID)
{
  return (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT[buttonID], BUTTON_GPIO_PIN[buttonID]) == GPIO_PIN_RESET);
}

#endif /* BSP_BUTTON_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
