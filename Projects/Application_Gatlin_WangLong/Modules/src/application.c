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
/* Sys Run Level at Flash 0x0800ef00 */
#define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
#define SYS_RUN_LEVEL_ADDR            (0x0800a000)
const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;

//==============================================================================
/* cpu id */
static uint32_t sysCpuId[3] = {0};

//==============================================================================
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;

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

static SuperLed_QItemDef sled;

//==============================================================================
/* key status bits */
static uint32_t keyStatus = 0;

static USBD_HandleTypeDef *phUsbDev = &hUsbDeviceFS;

typedef struct
{
  uint32_t    xMLen;      /* len of valid message */
  uint8_t     xM[64];     /* message string */
}Cmd_MsgItemDef;

typedef struct
{
  uint32_t        mBuffRptr;
  uint32_t        mBuffWptr;
  Cmd_MsgItemDef  mBuff[8];
}Cmd_MsgBufDef;

static Cmd_MsgBufDef cmds;

/* Private function prototypes -----------------------------------------------*/
int32_t AppCmd_Task(void const * argument);
int32_t AppCmd_AutoUpload(void const * argument);
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
  CPU_getId(sysCpuId);
  
  
  /* Init BSP */
  BSP_Init();
  BSP_SYSLED_Init();
  BSP_UART1_Init();
  BSP_USBEN_Init();
  
  
  /* Get Run Level */
  uint32_t *pRunLevel = (uint32_t *)&SysRunLevel;
  if(*pRunLevel == 0xffffffff)
  {
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
    BSP_EXTI0_Init();
    BSP_EXTI1_Init();
    BSP_KEY_Init(KEY1);
    BSP_KEY_Init(KEY2);
    BSP_TIM3_Init();
    BSP_MOTOR_Init();
    
    /* Init motor */
    BSP_MOTOR_SetEnable(true);    //turn on
    BSP_MOTOR_SetDir(0, 1);       //right
    BSP_MOTOR_SetDir(1, 1);       //right
    
    /* Init superled */
    SuperLed_Init();
    memcpy(&sled, &cLedDefaultRun, sizeof(SuperLed_QItemDef));
    SuperLed_Display(&sled);
    
    /* Initialize For Counter */
    cmds.mBuffRptr = 0;
    cmds.mBuffWptr = 0;
    uint32_t usbConnected = 0;
    
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
        * @brief  Led flash Task
        */
      SuperLed_Task(NULL);
      
      
      //=======================================
      /**
        * @brief  Scan Keys
        */
      keyStatus = 0;
      if(BSP_KEY_Read(KEY1) == true)
      {
        keyStatus |= (0x1<<0);
      }
      if(BSP_KEY_Read(KEY2) == true)
      {
        keyStatus |= (0x1<<1);
      }
      
      
      //=======================================
      /**
        * @brief  Cmd Handle
        */
      AppCmd_Task(NULL);
      
      
      //=======================================
      /**
        * @brief  Auto Upload key state
        */
      AppCmd_AutoUpload(NULL);
      
      
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
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
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
  * @brief  USBD_CDC_ReceiveCallback
  * @param  Buf
  * @param  Len
  * @retval none
  */
void CMD_send(uint8_t *pBuff, uint32_t xlen)
{
  if(phUsbDev->dev_state == USBD_STATE_CONFIGURED)
  {
    CDC_Transmit_FS(pBuff, xlen);
  }
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
  cmds.mBuff[cmds.mBuffWptr].xMLen = *Len;
  memcpy(cmds.mBuff[cmds.mBuffWptr].xM, Buf, cmds.mBuff[cmds.mBuffWptr].xMLen);
  cmds.mBuffWptr ++;
  if(cmds.mBuffWptr >= 8) { cmds.mBuffWptr = 0; }
  return (0);
}

//==============================================================================
/**
  * @brief  AppCmd_Task
  * @param  argument
  * @retval none
  * @note   analysize this message from buff
  */
int32_t AppCmd_Task(void const * argument)
{
  uint8_t errCode = 0;
  
  uint32_t cmdCnt;
  cmdCnt = 8 + cmds.mBuffWptr - cmds.mBuffRptr;
  if(cmdCnt >= 8) {cmdCnt -= 8;}
  
  /* Enter Handle If not EMPTY */
  if(cmdCnt > 0)
  {
    Cmd_MsgItemDef cur;
    /* Get CMD */
    memcpy(&cur, &cmds.mBuff[cmds.mBuffRptr],sizeof(Cmd_MsgItemDef));
    cmds.mBuffRptr ++;
    if(cmds.mBuffRptr >= 8) { cmds.mBuffRptr = 0; }
    /*  */
    
    if(cur.xMLen >= 5)
    {
      if((cur.xM[0] == 0XAA)&&(cur.xM[1]==0X55))
      {
        if(cur.xMLen == 4+cur.xM[2])
        {
          uint8_t ck = 0;
          for(uint32_t i=0; i<3+cur.xM[2]; i++)
          {
            ck ^= cur.xM[i];
          }
          if(ck == cur.xM[3+cur.xM[2]])
          {
            if(cur.xM[3] == 0x01)
            {
              //read hand
              errCode = 0x00;
              //send message
              uint8_t msg[16];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x0c;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              memcpy(&msg[5], "Gatlin_Gun", 10);
              msg[15] = 0;
              for(uint32_t i=0; i<15; i++)
              {
                msg[15] ^= msg[i];
              }
              CMD_send(msg,16);
            }
            else if(cur.xM[3] == 0x02)
            {
              //read heart
              errCode = 0x00;
              //send message
              uint8_t msg[16];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x02;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
            else if(cur.xM[3] == 0x03)
            {
              //read hard version
              errCode = 0x00;
              //send message
              uint8_t msg[16];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x08;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              memcpy(&msg[5], "V1.1.0", 6);
              msg[11] = 0;
              for(uint32_t i=0; i<11; i++)
              {
                msg[11] ^= msg[i];
              }
              CMD_send(msg,12);
            }
            else if(cur.xM[3] == 0x04)
            {
              //read software version
              errCode = 0x00;
              //send message
              uint8_t msg[16];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x08;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              memcpy(&msg[5], "V2.1.4", 6);
              msg[11] = 0;
              for(uint32_t i=0; i<11; i++)
              {
                msg[11] ^= msg[i];
              }
              CMD_send(msg,12);
            }
            else if(cur.xM[3] == 0x05)
            {
              //read sn
              errCode = 0x00;
              //send message
              uint8_t msg[32];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x12;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              memcpy(&msg[5], hStorageMsgData.sID, 16);
              msg[21] = 0;
              for(uint32_t i=0; i<21; i++)
              {
                msg[21] ^= msg[i];
              }
              CMD_send(msg,22);
            }
            else if(cur.xM[3] == 0x61)
            {
              //write sn
              errCode = 0x00;
              if(cur.xM[2] == 0x11)
              {
                memcpy(hStorageMsgData.sID, &cur.xM[4], 16);
                Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
              }
              else
              {
                //param err
                errCode = 0x04;
              }
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x02;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
            else if(cur.xM[3] == 0x20)
            {
              //motor 1
              errCode = 0x00;
              if(cur.xM[4] <= 100)
              {
                BSP_TIM3_SetChannelRate(0, cur.xM[4]);
              }
              else
              {
                errCode = 0x04;
              }
              
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x02;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
            else if(cur.xM[3] == 0x21)
            {
              //motor 2
              errCode = 0x00;
              if(cur.xM[4] <= 100)
              {
                //=======================================
                /**
                  * @note   High Limit
                  */
                BSP_TIM3_SetChannelRate(1, (cur.xM[4]*0x40/0x64));
              }
              else
              {
                errCode = 0x04;
              }
              
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x02;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
            else if(cur.xM[3] == 0x22)
            {
              //read keys
              errCode = 0x00;
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x03;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = keyStatus;
              msg[6] = 0;
              for(uint32_t i=0; i<6; i++)
              {
                msg[6] ^= msg[i];
              }
              CMD_send(msg,7);
            }
            else if(cur.xM[3] == 0x70)
            {
              //set upgrade
              errCode = 0x00;
              firmwareUpgradeFlag = FIRMWARE_UPGRADE_FLAG_KEYWORD;
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x03;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
            else
            {
              //cmd err
              errCode = 0x03;
              //send message
              uint8_t msg[8];
              msg[0] = 0XAA;
              msg[1] = 0X55;
              msg[2] = 0x02;
              msg[3] = 0X80|cur.xM[3];
              msg[4] = errCode;
              msg[5] = 0;
              for(uint32_t i=0; i<5; i++)
              {
                msg[5] ^= msg[i];
              }
              CMD_send(msg,6);
            }
          }
          else
          {
            //ck err
            errCode = 0x02;
            //send message
            uint8_t msg[8];
            msg[0] = 0XAA;
            msg[1] = 0X55;
            msg[2] = 0x02;
            msg[3] = 0X80|cur.xM[3];
            msg[4] = errCode;
            msg[5] = 0;
            for(uint32_t i=0; i<5; i++)
            {
              msg[5] ^= msg[i];
            }
            CMD_send(msg,6);
          }
        }
        else
        {
          //len err
          errCode = 0x01;
          //send message
          uint8_t msg[8];
          msg[0] = 0XAA;
          msg[1] = 0X55;
          msg[2] = 0x02;
          msg[3] = 0X80|cur.xM[3];
          msg[4] = errCode;
          msg[5] = 0;
          for(uint32_t i=0; i<5; i++)
          {
            msg[5] ^= msg[i];
          }
          CMD_send(msg,6);
        }
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
int32_t AppCmd_AutoUpload(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= 20)
  {
    /* Update tick */
    tickLst = tickNew;
    
    //read keys
    //send message
    uint8_t msg[8];
    msg[0] = 0XAA;
    msg[1] = 0X55;
    msg[2] = 0x03;
    msg[3] = 0Xa2;
    msg[4] = 0x00;
    msg[5] = keyStatus;
    msg[6] = 0;
    for(uint32_t i=0; i<6; i++)
    {
      msg[6] ^= msg[i];
    }
    CMD_send(msg,7);
  }
  
  return (0);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
