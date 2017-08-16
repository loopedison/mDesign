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
#include "crc16.h"
/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "superled.h"
#include "commander.h"
#include "commander_if.h"
#include "tsensor.h"
#include "tcontrol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//##############################################################################
#define STORAGE_INFO_NAME           "Mixed Control"
#define STORAGE_SYS_PID             (0XFF010300)
#define STORAGE_SYS_UID             (0XFFFFFFFF)
#define STORAGE_SYS_SOFT_VER        (0XF110)
#define STORAGE_SYS_HARD_VER        (0XF120)
//##############################################################################
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* Firmware Upgrade Flag */
#if (APPLICATION_OFFSET == 0x00008000)
  #define FIRMWARE_UPGRADE_FLAG_ADDR    (0x20001f00)
  #define FIRMWARE_UPGRADE_FLAG_KEYWORD (0XCDEF89AB)
  volatile uint32_t firmwareUpgradeFlag __attribute__((at(FIRMWARE_UPGRADE_FLAG_ADDR)));
#endif

//==============================================================================
/* Sys Run Level */
#if (APPLICATION_OFFSET == 0x00008000)
  /* Sys Run Level at Flash 0x0800a000 */
  #define SYS_RUN_LEVEL_ADDR            (0x0800a000)
  #define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
  const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;
#else
  #define SYS_RUN_LEVEL_ADDR            (0x08004000)
  #define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
  const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;
#endif

//==============================================================================
/* cpu id */
static uint32_t sysCpuId[3] = {0};

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

//##############################################################################
const Storage_MsgDataSysTypeDef cStorageSys =
{
  .xReserved[0]   = 0x00,
  .xMode[0]       = 0x00,
  .xPID[0]        = (uint8_t)((STORAGE_SYS_PID)),
  .xPID[1]        = (uint8_t)((STORAGE_SYS_PID)>>8),
  .xPID[2]        = (uint8_t)((STORAGE_SYS_PID)>>16),
  .xPID[3]        = (uint8_t)((STORAGE_SYS_PID)>>24),
  .xUID[0]        = (uint8_t)((STORAGE_SYS_UID)),
  .xUID[1]        = (uint8_t)((STORAGE_SYS_UID)>>8),
  .xUID[2]        = (uint8_t)((STORAGE_SYS_UID)>>16),
  .xUID[3]        = (uint8_t)((STORAGE_SYS_UID)>>24),
  .xSoftVer[0]    = (uint8_t)((STORAGE_SYS_SOFT_VER)),
  .xSoftVer[1]    = (uint8_t)((STORAGE_SYS_SOFT_VER)>>8),
  .xHardVer[0]    = (uint8_t)((STORAGE_SYS_HARD_VER)),
  .xHardVer[1]    = (uint8_t)((STORAGE_SYS_HARD_VER)>>8),
  .xUpgrade[0]    = 0x00,
  .xRestore[0]    = 0x00,
};

const Storage_MsgDataSuperTypeDef cStorageSuper =
{
  .xKey[0]        = 0xef,
  .xKey[1]        = 0xcd,
  .xKey[2]        = 0xab,
  .xKey[3]        = 0x89,
  .xState[0]      = 0x00,
};
//##############################################################################

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
    * @brief  Get CPU ID
    */
  CPU_getId(sysCpuId);
  
  
  //=======================================
  /**
    * @brief  Init BSP
    */
  BSP_Init();
  BSP_SYSLED_Init();
  
  
  //=======================================
  /**
    * @brief  Get Run Level
    */
  uint32_t *pRunLevel = (uint32_t *)&SysRunLevel;
  if(*pRunLevel == 0xffffffff)
  {
    //=======================================
    /**
      * @brief  First Run To Prepare
      */
    
    /* Init Storage get Info */
    Storage_StatusTypeDef status = Storage_ReadInfo(STORAGE_HANDLE, &hStorageInfo);
    /* DeInit Storage Default If storage was EMPTY */
    if(hStorageInfo.version != STORAGE_VERSION)
    {
      HAL_Delay(1);
      const Storage_MsgDataTypeDef cDefaultMsgData = { .xFlag = STORAGE_XFLAG_SET, };
      const uint32_t cDefaultIndex = 0;
      const Storage_InfoDef cDefaultInfo = { .version = STORAGE_VERSION, };
      Storage_DeInit();
      Storage_WriteInfo(STORAGE_HANDLE, (Storage_InfoDef *)&cDefaultInfo);
      Storage_WriteIndex(STORAGE_HANDLE, cDefaultIndex);
      Storage_WriteIndexMsgData(STORAGE_HANDLE, 0, (Storage_MsgDataTypeDef *)&cDefaultMsgData);
    }
    
    /* Modify Run Flag */
    HAL_Delay(1);
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (SYS_RUN_LEVEL_ADDR), (SYS_RUN_LEVEL_KEYWORD));
    HAL_FLASH_Lock();
    
    /* Generate system reset to allow jumping to the user code */
    HAL_Delay(1);
    NVIC_SystemReset();
    while(1);
  }
  else if(*pRunLevel == SYS_RUN_LEVEL_KEYWORD)
  {
    //=======================================
    /**
      * @brief  Check Storage Module
      */
    /* Init Storage */
    Storage_StatusTypeDef status = Storage_Init();
    
    //=======================================
    /**
      * @brief  Check If any task If
      */
    if(hStorageMsgData.xFlag == STORAGE_XFLAG_SET)
    {
      /* Default Settings */
      hStorageMsgData.xFlag = STORAGE_XFLAG_RESET;
      memcpy(&hStorageMsgData.xInfo.xName, STORAGE_INFO_NAME, 16);
      memcpy(&hStorageMsgData.xSys, &cStorageSys, sizeof(Storage_MsgDataSysTypeDef));
      memcpy(&hStorageMsgData.xSuper, &cStorageSuper, sizeof(Storage_MsgDataSuperTypeDef));
      memcpy(&hStorageMsgData.xUserConf, &cStorageUserConf, sizeof(Storage_MsgDataUserConfTypeDef));
      memcpy(&hStorageMsgData.xUserParam, &cStorageUserParam, sizeof(Storage_MsgDataUserParamTypeDef));
      memcpy(&hStorageMsgData.xSys.xUID, &sysCpuId[0], sizeof(uint32_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      
      /* Generate system reset to allow jumping to the user code */
      HAL_Delay(10);
      NVIC_SystemReset();
      while(1);
    }
    
    
    //=======================================
    /**
      * @brief  Normal Loop
      */
    /* Initialize Commander */
    Commander_Init();
    
    /* Initialize Tsensor */
    Tsensor_Init();
    
    /* Initialize Tcontrol */
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
      Commander_Task(NULL);
      
      //=======================================
      //Tsensor Task
      Tsensor_Task(NULL);
      
      //=======================================
      //Tcontrol Task
      Tcontrol_Task(NULL);
      
      //=======================================
      //Upgrade Firmware If Need
      if(hStorageMsgData.xSys.xUpgrade[0] != 0x0)
      {
/* Firmware Upgrade Flag */
#if (APPLICATION_OFFSET != 0x00000000)
        firmwareUpgradeFlag = FIRMWARE_UPGRADE_FLAG_KEYWORD;
#endif
        HAL_Delay(100);
        /* Generate system reset to allow jumping to the user code */
        NVIC_SystemReset();
        while(1);
      }
      
      //=======================================
      //Enter SLEEP Mode, wakeup by interrupts
//      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
    }
  }
  else
  {
    /* Failed to Endless */
    while(1)
    {
      BSP_SYSLED_Toggle();  HAL_Delay(25);
    }
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
