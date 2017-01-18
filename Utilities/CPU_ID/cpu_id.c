/**
  ******************************************************************************
  * @file    cpu_id.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for CPU Id
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cpu_id.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Get CPU IDs
  * @param  id[3], to store data 
  * @retval None
  */
void CPU_getId(uint32_t id[3])
{
  id[0] = *(uint32_t *)(0X1FFFF7E8);
  id[1] = *(uint32_t *)(0X1FFFF7EC);
  id[2] = *(uint32_t *)(0X1FFFF7F0);
}

//==============================================================================
/**
  * @brief  Get Internal Flash Size
  * @param  size[1], to store data 
  * @retval None
  */
void CPU_getFlashSize(uint32_t size[1])
{
  size[0] = *(short*)(0X1FFFF7E0);
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
