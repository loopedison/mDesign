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
/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "superled.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define FIRMWARE_UPGRADE_FLAG_ADDR    (0x20001f00)
#define FIRMWARE_UPGRADE_FLAG_KEYWORD (0XCDEF89AB)
volatile uint32_t firmwareUpgradeFlag __attribute__((at(FIRMWARE_UPGRADE_FLAG_ADDR)));

//==============================================================================
/* Sys Run Level at Flash 0x0800a000 */
#define SYS_RUN_LEVEL_ADDR            (0x0800a000)
#define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;

//==============================================================================
/* cpu id */
static uint32_t sysCpuId[3] = {0};

//==============================================================================
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
static USBD_HandleTypeDef *phUsbDev = &hUsbDeviceFS;

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

const SuperLed_QItemDef  cLedConnected= 
{
  .counter = SUPERLED_COUNTER_MAX,
  .priority = SUPERLED_PRIO_DEFAULT,
  .message[0] = 1000,
  .message[1] = -25,
  .message[2] = 0,
};

//==============================================================================
/*  */
typedef struct
{
  uint32_t    xMLen;      /* len of valid message */
  uint8_t     xM[64];     /* message string */
}AppCmd_MsgItemDef;

typedef struct
{
  uint32_t            mBuffRptr;
  uint32_t            mBuffWptr;
  AppCmd_MsgItemDef   mBuff[8];
}AppCmd_MsgBuffDef;

static AppCmd_MsgBuffDef appCmds;

//==============================================================================
typedef struct
{
  uint32_t              xPeriod;
  uint8_t               *pDataBuff;
  uint8_t               xDataBuffPtr;
}AppCmd_SendDef;
static AppCmd_SendDef appMsg;

//==============================================================================
/* sensor */
typedef struct
{
  uint32_t      keyState;
  uint32_t      motorRate[2];
}Tsensor_InfoDef;

static Tsensor_InfoDef tsensorInfo;

/* Private function prototypes -----------------------------------------------*/
uint32_t AppCmd_Task(void const * argument);
uint32_t AppCmd_AutoUpload(void const * argument);
uint32_t Tsensor_Task(void const * argument);

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
  BSP_UART1_Init();
  BSP_USBEN_Init();
  
  
  /* Get Run Level */
  uint32_t *pRunLevel = (uint32_t *)&SysRunLevel;
  if(*pRunLevel == 0xffffffff)
  {
    //=======================================
    /**
      * @brief  First Run To Prepare
      */
    
    /* Modify Run Flag */
    HAL_Delay(1);
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (SYS_RUN_LEVEL_ADDR), (SYS_RUN_LEVEL_KEYWORD));
    HAL_FLASH_Lock();
    
    /* Init Storage get Info */
    Storage_StatusTypeDef status = Storage_ReadInfo(STORAGE_HANDLE, &hStorageInfo);
    /* DeInit Storage Default If storage was EMPTY */
    if(hStorageInfo.version == 0xffffffff)
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
    
    /* Valid Version Check */
    if(STORAGE_VERSION > hStorageInfo.version)
    {
      /* need upgrade */
    }
    else if(STORAGE_VERSION < hStorageInfo.version)
    {
      /* error */
    }
    
    if(status != STORAGE_OK)
    {
      while(1)
      {
        HAL_Delay(1600);
        BSP_SYSLED_On();
        HAL_Delay(250);
        BSP_SYSLED_Off();
        HAL_Delay(50);
        BSP_SYSLED_On();
        HAL_Delay(50);
        BSP_SYSLED_Off();
        HAL_Delay(50);
      }
    }
    
    
    //=======================================
    /**
      * @brief  Check If any task If
      */
    if(hStorageMsgData.xFlag == STORAGE_XFLAG_SET)
    {
////      /*  */
////      hStorageMsgData.xFlag = STORAGE_XFLAG_RESET;
////      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
////      
////      /* Generate system reset to allow jumping to the user code */
////      HAL_Delay(1);
////      NVIC_SystemReset();
////      while(1);
    }
    
    
    //=======================================
    /**
      * @brief  Normal Loop
      */
    /* Init Device */
    BSP_KEY_Init(KEY1);
    BSP_KEY_Init(KEY2);
    BSP_TIM3_Init();
    BSP_MOTOR_Init();
    
    /* Initialize For Counter */
    uint32_t usbConnected = 0;
    
    /* Initialize For tsensor */
    memset(&tsensorInfo, 0, sizeof(Tsensor_InfoDef));
    
    /* Initialize For Message to Send */
    appMsg.xPeriod = 50;
    appMsg.xDataBuffPtr = 0;
    appMsg.pDataBuff = malloc(32);
    ASSERT(appMsg.pDataBuff != NULL);
    
    /* Init superled */
    SuperLed_QItemDef sled;
    
    /* Init superled */
    SuperLed_Init();
    memcpy(&sled, &cLedDefaultRun, sizeof(SuperLed_QItemDef));
    SuperLed_Display(&sled);
    
    
    /* Disable USB Connection */
    BSP_USBEN_Off();
    /* Init Device Library,Add Supported Class and Start the library*/
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
    USBD_Start(&hUsbDeviceFS);
    /* Enable USB Connection */
    BSP_USBEN_On();
    
    
    /* Start Motor */
    BSP_MOTOR_SetEnable(true);    //turn on
    BSP_MOTOR_SetDir(0, 1);       //right
    BSP_MOTOR_SetDir(1, 1);       //right
    
    
    while(1)
    {
      //=======================================
      /**
        * @brief  Led flash Task
        */
      SuperLed_Task(NULL);
      
      
      //=======================================
      /**
        * @brief  Cmd Task
        */
      AppCmd_Task(NULL);
      
      
      //=======================================
      /**
        * @brief  Auto Upload key state
        */
      AppCmd_AutoUpload(NULL);
      
      
      //=======================================
      /**
        * @brief  Auto Upload key state
        */
      Tsensor_Task(NULL);
      
      
      //=======================================
      /**
        * @brief  Led flash Task
        */
      if((usbConnected == 0)&&(phUsbDev->dev_state == USBD_STATE_CONFIGURED))
      {
        usbConnected = 1;
        memcpy(&sled, &cLedConnected, sizeof(SuperLed_QItemDef));
        SuperLed_Display(&sled);
      }
      else if((usbConnected == 1)&&(phUsbDev->dev_state != USBD_STATE_CONFIGURED))
      {
        usbConnected = 0;
        SuperLed_Remove(&sled);
      }
      
      
      //=======================================
      /**
        * @brief  Upgrade Firmware If Need
        */
      if(firmwareUpgradeFlag == FIRMWARE_UPGRADE_FLAG_KEYWORD)
      {
        /* Generate system reset to allow jumping to the user code */
        BSP_USBEN_Off();
        HAL_Delay(10);
        NVIC_SystemReset();
        while(1);
      }
      
      //=======================================
      /**
        * @brief  Enter SLEEP Mode, wakeup by interrupts
        */
//      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
    }
  }
  else
  {
    /* Failed to Endless */
    while(1)
    {
      BSP_SYSLED_Toggle();
      HAL_Delay(25);
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


//==============================================================================
/**
  * @brief  USBD_CDC_ReceiveCallback
  * @param  Buf
  * @param  Len
  * @retval none
  * @note   recode this message to buff
  */
int8_t USBD_CDC_ReceiveCallback(uint8_t* Buf, uint32_t *Len)
{
  /* Push to Buff */
  appCmds.mBuff[appCmds.mBuffWptr].xMLen = *Len;
  memcpy(appCmds.mBuff[appCmds.mBuffWptr].xM, Buf, appCmds.mBuff[appCmds.mBuffWptr].xMLen);
  appCmds.mBuffWptr ++;
  if(appCmds.mBuffWptr >= 8) { appCmds.mBuffWptr = 0; }
  return (0);
}

//==============================================================================
/**
  * @brief  AppCmd_Task
  * @param  argument
  * @retval none
  * @note   analysize this message from buff
  */
uint32_t AppCmd_Task(void const * argument)
{
  uint32_t cmdCnt;
  cmdCnt = 8 + appCmds.mBuffWptr - appCmds.mBuffRptr;
  if(cmdCnt >= 8) {cmdCnt -= 8;}
  
  /* Enter Handle If not EMPTY */
  if(cmdCnt > 0)
  {
    AppCmd_MsgItemDef cur;
    /* Get CMD */
    memcpy(&cur, &appCmds.mBuff[appCmds.mBuffRptr], sizeof(AppCmd_MsgItemDef));
    appCmds.mBuffRptr ++;
    if(appCmds.mBuffRptr >= 8) { appCmds.mBuffRptr = 0; }
    /*  */
    
    if(cur.xMLen >= 4)
    {
      if((cur.xM[0] == 0XAB)&&(cur.xM[1]==0X00)&&(cur.xM[2]==0X01)&&(cur.xM[3]==0XAA))
      {
        firmwareUpgradeFlag = FIRMWARE_UPGRADE_FLAG_KEYWORD;
      }
    }
  }
  return (0);
}

//==============================================================================
/**
  * @brief  AppCmd_AutoUpload
  * @param  argument
  * @retval none
  * @note   auto upload
  */
uint32_t AppCmd_AutoUpload(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= appMsg.xPeriod)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Upload Message If Connected */
    appMsg.xDataBuffPtr = 0;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = 0xaa;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = 0x55;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = tsensorInfo.keyState;
    uint8_t ckByte = 0;
    for(uint32_t i=0; i<appMsg.xDataBuffPtr; i++)
    {
      ckByte ^= appMsg.pDataBuff[i];
    }
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = ckByte;
    
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
    {
      CDC_Transmit_FS(appMsg.pDataBuff, appMsg.xDataBuffPtr);
    }
    
    HAL_UART_Transmit(&huart1, appMsg.pDataBuff, appMsg.xDataBuffPtr, 10);
  }
  
  return (0);
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
uint32_t Tsensor_Task(void const * argument)
{
  tsensorInfo.keyState = 0;
  if(BSP_KEY_Read(KEY1) == true)
  {
    tsensorInfo.keyState |= (0x1<<0);
  }
  if(BSP_KEY_Read(KEY2) == true)
  {
    tsensorInfo.keyState |= (0x1<<1);
  }
  
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 10)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Upload Motor */
    if(tsensorInfo.keyState != 0)
    {
      if(tsensorInfo.motorRate[0] <= 95)  {tsensorInfo.motorRate[0] += 5;}
      if(tsensorInfo.motorRate[1] <= 60)  {tsensorInfo.motorRate[1] += 5;}
    }
    else
    {
      if(tsensorInfo.motorRate[0] >=  5)  {tsensorInfo.motorRate[0] -= 5;}
      if(tsensorInfo.motorRate[1] >=  5)  {tsensorInfo.motorRate[1] -= 5;}
    }
    
    BSP_TIM3_SetChannelRate(0, tsensorInfo.motorRate[0]);
    BSP_TIM3_SetChannelRate(1, tsensorInfo.motorRate[1]);
  }
  
  return (0);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
