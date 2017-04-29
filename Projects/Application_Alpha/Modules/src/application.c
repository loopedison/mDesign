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
/*  */
typedef struct
{
  uint32_t   xValueA;
  uint32_t   xValueB;
}Tsensor_CounterDef;

static Tsensor_CounterDef tsensorCounter;

//==============================================================================
typedef struct
{
  int32_t   iFlag;
  int32_t   xVal;
}Tsensor_RateDataDef;

typedef struct
{
  uint32_t              xPeriod;
  uint32_t              xDivision;
  uint32_t              xRate;
  Tsensor_RateDataDef   *pDataBuff;
}Tsensor_RateDef;

static Tsensor_RateDef tsensorRateA;
static Tsensor_RateDef tsensorRateB;

//==============================================================================
typedef struct
{
  int32_t   xDir;
  int32_t   xSpd;
}Tsensor_InfoDef;

static Tsensor_InfoDef tsensorInfo;

/* Private function prototypes -----------------------------------------------*/
uint32_t AppCmd_Task(void const * argument);
uint32_t AppCmd_AutoUpload(void const * argument);
uint32_t Tsensor_RateInit(Tsensor_RateDef *pRate);
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue);
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
    BSP_EXTI5_Init();
    BSP_EXTI6_Init();
    
    
    /* Initialize For Counter */
    uint32_t usbConnected = 0;
    
    /* Initialize For Counter */
    tsensorCounter.xValueA = 0;
    tsensorCounter.xValueB = 0;
    
    /* Initialize For Rate */
    tsensorRateA.xPeriod = 5000;
    tsensorRateA.xDivision = 50;
    tsensorRateA.xRate = 0;
    tsensorRateA.pDataBuff = malloc(tsensorRateA.xDivision*sizeof(Tsensor_RateDataDef));
    ASSERT(tsensorRateA.pDataBuff != NULL);
    Tsensor_RateInit(&tsensorRateA);
    
    tsensorRateB.xPeriod = 5000;
    tsensorRateB.xDivision = 50;
    tsensorRateB.xRate = 0;
    tsensorRateB.pDataBuff = malloc(tsensorRateB.xDivision*sizeof(Tsensor_RateDataDef));
    ASSERT(tsensorRateB.pDataBuff != NULL);
    Tsensor_RateInit(&tsensorRateB);
    
    /* Initialize For Info */
    tsensorInfo.xDir = 0;
    tsensorInfo.xSpd = 0;
    
    /* Initialize For Message to Send */
    appMsg.xPeriod = 100;
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
    
    /* Make Info */
    tsensorInfo.xSpd = MIN(tsensorRateA.xRate, tsensorRateB.xRate);
    if(tsensorInfo.xSpd >= 1)       {tsensorInfo.xSpd -= 1;}
    else                            {tsensorInfo.xSpd = 0;}
    tsensorInfo.xDir = tsensorRateA.xRate - tsensorRateB.xRate;
    if(tsensorInfo.xDir >= 2)       {tsensorInfo.xDir -= 2;}
    else if(tsensorInfo.xDir <= -2) {tsensorInfo.xDir += 2;}
    else                            {tsensorInfo.xDir = 0;}
    
    /* Upload Message If Connected */
    appMsg.xDataBuffPtr = 0;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = 0xaa;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = 0x55;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = tsensorInfo.xSpd;
    appMsg.pDataBuff[appMsg.xDataBuffPtr++] = tsensorInfo.xDir;
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
  }
  
  return (0);
}

//==============================================================================
/**
  * @brief  EXTI Interrupt
  * @param  GPIO_PIN
  * @retval none
  * @note   INT mode
  */
void HAL_GPIO_EXTI_Callback_EXTI5(uint16_t GPIO_Pin)
{
  tsensorCounter.xValueA += 1;
}
void HAL_GPIO_EXTI_Callback_EXTI6(uint16_t GPIO_Pin)
{
  tsensorCounter.xValueB += 1;
}

//==============================================================================
/**
  * @brief  Tsensor Rate Init
  * @param  pRate   :
  * @retval none
  */
uint32_t Tsensor_RateInit(Tsensor_RateDef *pRate)
{
  for(uint32_t i=0; i<pRate->xDivision; i++)
  {
    pRate->pDataBuff[i].iFlag = i;
    pRate->pDataBuff[i].xVal = 0;
  }
  return (0);
}

//==============================================================================
/**
  * @brief  Tsensor Rate Calc
  * @param  pRate   :
  * @param  iValue  :
  * @retval rate value
  */
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue)
{
  for(uint32_t i=0; i<pRate->xDivision; i++)
  {
    pRate->pDataBuff[i].iFlag += 1;
    if(pRate->pDataBuff[i].iFlag >= pRate->xDivision)
    {
      pRate->pDataBuff[i].iFlag = 0;
      pRate->xRate = iValue - pRate->pDataBuff[i].xVal;
      pRate->pDataBuff[i].xVal = iValue;
    }
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
  static uint32_t tickRateA = 0;
  static uint32_t tickRateB = 0;
  uint32_t tickNew = 0;
  
  tickNew = HAL_GetTick();
  
  /* update rate */
  if(tickNew - tickRateA >= tsensorRateA.xPeriod/tsensorRateA.xDivision)
  {
    tickRateA = tickNew;
    Tsensor_RateCalc(&tsensorRateA, tsensorCounter.xValueA);
  }
  
  /* update rate */
  if(tickNew - tickRateB >= tsensorRateB.xPeriod/tsensorRateB.xDivision)
  {
    tickRateB = tickNew;
    Tsensor_RateCalc(&tsensorRateB, tsensorCounter.xValueB);
  }
  
  return (0);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
