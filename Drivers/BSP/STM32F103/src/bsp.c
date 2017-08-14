/**
  ******************************************************************************
  * @file    bsp.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   board support packet
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  BSP Init
  * @param  none 
  * @retval BSP_OK: normal
  */
BSP_StatusTypeDef BSP_Init(void)
{
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  BSP assert fail
  * @param  none 
  * @retval none
  */
#if USE_BSP_ASSERT == 1
__weak void BSP_AssertFail(char *exp, char *file, int line)
{
  while(1)
  {
  }
}
#endif //USE_BSP_ASSERT


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
