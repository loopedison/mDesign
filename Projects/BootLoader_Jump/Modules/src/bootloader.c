/**
  ******************************************************************************
  * @file    bootloader.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   BootLoader
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"
#include "cpu_id.h"

/* Private typedef -----------------------------------------------------------*/
/* Sys Boot Version */
#define SYS_BOOT_VERSION              (0XAA000100)

/* Sys Want to Boot at RAM 0x08020000 */
#define SYS_APP_RUN_ADDR              (0x08008000)

typedef void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  BootLoader Deal with message
  * @param  None
  * @retval None
  */
BootLoader_StatusTypeDef BootLoader_Main(void)
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
  
    /* Test application code at SYS_APP_RUN_ADDR */
    if(((*(__IO uint32_t*)SYS_APP_RUN_ADDR) & 0x2FFE0000 ) == 0x20000000)
    {
      /* application address */
      pFunction JumpToApplication;
      uint32_t JumpAddress;
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (SYS_APP_RUN_ADDR + 4);
      JumpToApplication = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) SYS_APP_RUN_ADDR);
      JumpToApplication();
      /* Load Application Failed */
      while(1);
    }
    
    /* No Load Application */
    while(1)
    {
      HAL_Delay(1700);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
    }
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
