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
#include "cpu_id.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define FIRMWARE_UPGRADE_FLAG_ADDR    (0x20001f00)
#define FIRMWARE_UPGRADE_FLAG_KEYWORD (0XCDEF89AB)
volatile uint32_t firmwareUpgradeFlag __attribute__((at(FIRMWARE_UPGRADE_FLAG_ADDR)));
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
  /* Get CPU ID */
  uint32_t sysCpuId[3];
  CPU_getId(sysCpuId);
  
  /* Init BSP SYSLED */
  BSP_Init();
  BSP_SYSLED_Init();
  
  /* Flash SYSLED Start */
  BSP_SYSLED_On();
  HAL_Delay(25);
  BSP_SYSLED_Off();
  HAL_Delay(25);
  uint32_t flashCnt = 0;
  while(1)
  {
    HAL_Delay(2000);
    flashCnt++;
    for(uint32_t i=0; i<flashCnt%10; i++)
    {
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
    }
    
    if(flashCnt > 30)
    {
      HAL_Delay(10);
      firmwareUpgradeFlag=FIRMWARE_UPGRADE_FLAG_KEYWORD;
      /* Generate system reset to allow jumping to the user code */
      NVIC_SystemReset();
      while(1);
    }
  }
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
