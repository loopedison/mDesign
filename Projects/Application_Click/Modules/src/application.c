/**
  ******************************************************************************
  * @file    application.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Application
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "application.h"
/* Includes ------------------------------------------------------------------*/
#include "superled.h"
#include "tcontrol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* default led off */
const SuperLed_QItemDef cLedDefaultRun = 
{
  .counter = SUPERLED_COUNTER_MAX,
  .priority = SUPERLED_PRIO_DEFAULT,
  .message[0] = 25,
  .message[1] = -1000,
  .message[2] = 0,
};

static SuperLed_QItemDef sLedSys;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Application Deal with message
  * @param  None
  * @retval None
  */
Application_StatusTypeDef Application_Main(void)
{
  //=======================================
  /**
    * @brief  Init BSP
    */
  BSP_Init();
  BSP_SYSLED_Init();
  
  //=======================================
  /**
    * @brief  Normal Loop
    */
  Tcontrol_Init();
  
  /* Init superled */
  SuperLed_Init();
  memcpy(&sLedSys, &cLedDefaultRun, sizeof(SuperLed_QItemDef));
  SuperLed_Display(&sLedSys);
  
  while(1)
  {
    //=======================================
    //Led flash Task
    SuperLed_Task(NULL);
    
    //=======================================
    //Commander Task
    Tcontrol_Task(NULL);
    
  }
}

//==============================================================================
/**
  * @brief  HAL_SYSTICK_Callback
  * @param  none
  * @retval none
  * @note   sys tick INT mode
  */
void HAL_SYSTICK_Callback(void)
{
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
