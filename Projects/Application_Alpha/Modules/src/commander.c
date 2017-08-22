/**
  ******************************************************************************
  * @file    commander.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Commander
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "commander.h"
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
#include "commander_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
static USBD_HandleTypeDef *phUsbDev = &hUsbDeviceFS;
static uint32_t usbConnected = 0;

//==============================================================================
/* led connect */
const SuperLed_QItemDef  cLedConnected= 
{
  .counter = SUPERLED_COUNTER_MAX,
  .priority = SUPERLED_PRIO_DEFAULT,
  .message[0] = 900,
  .message[1] = -25,
  .message[2] = 100,
  .message[3] = -25,
  .message[4] = 0,
};

static SuperLed_QItemDef sLedUsb;

//==============================================================================
/* Tx Once */
#define CMD_TXDATASIZE (64)
static uint8_t cmdTxData[CMD_TXDATASIZE];

//==============================================================================
/* Tx DataBuff */
#define CMD_TXBUFFSIZE      (0x200)
typedef struct
{
  uint32_t      mRptr;
  uint32_t      mWptr;
  uint8_t       mBuff[CMD_TXBUFFSIZE];
}Commander_TxBuffDef;
static Commander_TxBuffDef cmdTxBuff;

//==============================================================================
/* Rx DataBuff */
#define CMD_RXBUFFSIZE      (0x200)
typedef struct
{
  uint32_t      mRptr;
  uint32_t      mWptr;
  uint8_t       mBuff[CMD_RXBUFFSIZE];
}Commander_RxBuffDef;
static Commander_RxBuffDef cmdRxBuff;

//==============================================================================
/* Transfer Frame */
#define CMD_MSGHEAD          (0X3C)
#define CMD_FRAMEDATALEN     (0X40)
typedef struct
{
  uint32_t      fLength;
  uint8_t       fData[CMD_FRAMEDATALEN];
}Commander_FrameDef;
static Commander_FrameDef cmdFrameRecv;
static Commander_FrameDef cmdFrameSend;

/* Private function prototypes -----------------------------------------------*/
static uint32_t Commander_TaskRx(void const * argument);
static uint32_t Commander_TaskTx(void const * argument);

/* Private functions ---------------------------------------------------------*/
//==============================================================================
/**
  * @brief  Commander_Init
  * @param  none
  * @retval none
  */
void Commander_Init(void)
{
  /* Initialize connect state */
  usbConnected = 0;
  
  /* Initialize Txbuff and Rxbuff */
  memset(&cmdTxBuff, 0, sizeof(Commander_FrameDef));
  memset(&cmdRxBuff, 0, sizeof(Commander_FrameDef));
  
  /* Init Device */
  BSP_USBEN_Init();
  
  /* Disable USB Connection */
  BSP_USBEN_Off();
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
  USBD_Start(&hUsbDeviceFS);
  /* Enable USB Connection */
  BSP_USBEN_On();
}

//==============================================================================
/**
  * @brief  Commander_Task
  * @param  argument
  * @retval none
  */
void Commander_Task(void const * argument)
{
  //=======================================
  /**
    * @brief  Cmd Task Rx
    */
  Commander_TaskRx(NULL);
  
  //=======================================
  /**
    * @brief  Cmd Task Tx
    */
  Commander_TaskTx(NULL);
  
  //=======================================
  /**
    * @brief  Cmd Task Auto Upload
    */
  Commander_If_AutoUpload(NULL);
  
  //=======================================
  /**
    * @brief  Update usb CONNECT state
    */
  if((usbConnected == 0)&&(phUsbDev->dev_state == USBD_STATE_CONFIGURED))
  {
    usbConnected = 1;
    memcpy(&sLedUsb, &cLedConnected, sizeof(SuperLed_QItemDef));
    SuperLed_Display(&sLedUsb);
  }
  else if((usbConnected == 1)&&(phUsbDev->dev_state != USBD_STATE_CONFIGURED))
  {
    usbConnected = 0;
    SuperLed_Remove(&sLedUsb);
  }
}

//==============================================================================
/**
  * @brief  Commander_AppSend
  * @param  Buf
  * @param  Len
  * @retval none
  * @note   push data to txbuff
  */
uint32_t Commander_AppSend(uint8_t *pBuff, uint32_t pLen)
{
  cmdFrameSend.fLength = 0;
  cmdFrameSend.fData[cmdFrameSend.fLength++] = CMD_MSGHEAD;
  cmdFrameSend.fData[cmdFrameSend.fLength++] = pLen;
  for(uint32_t i=0; ((i<(pLen))&&(i<CMD_FRAMEDATALEN-4)); i++)
  {
    cmdFrameSend.fData[cmdFrameSend.fLength++] = pBuff[i];
  }
  uint16_t ck = CRC16_MODEBUS(&cmdFrameSend.fData[0], cmdFrameSend.fLength);
  cmdFrameSend.fData[cmdFrameSend.fLength++] = ck&0xff;
  cmdFrameSend.fData[cmdFrameSend.fLength++] = ck>>8;
  
  for(uint32_t i=0; (i<cmdFrameSend.fLength); i++)
  {
    /* Push to Buff */
    cmdTxBuff.mBuff[cmdTxBuff.mWptr] = cmdFrameSend.fData[i];
    /* Move to Next */
    cmdTxBuff.mWptr ++;
    if(cmdTxBuff.mWptr >= CMD_TXBUFFSIZE) { cmdTxBuff.mWptr -= CMD_TXBUFFSIZE; }
  }
  
  return (0);
}

//==============================================================================
/**
  * @brief  Commander_TaskTx
  * @param  argument
  * @retval none
  * @note   transfer message if not EMPTY
  */
uint32_t Commander_TaskTx(void const * argument)
{
  /* Count msg Buff */
  uint32_t mCnt = CMD_TXBUFFSIZE + cmdTxBuff.mWptr - cmdTxBuff.mRptr;
  if(mCnt >= CMD_TXBUFFSIZE) { mCnt -= CMD_TXBUFFSIZE; }
  
  /* Transfer if not EMPTY */
  if(mCnt > 0)
  {
    uint32_t iCnt = 0;
    /* Copy to txbuff */
    for(iCnt=0; ((iCnt<mCnt)&&(iCnt<CMD_TXDATASIZE)); iCnt++)
    {
      cmdTxData[iCnt] = cmdTxBuff.mBuff[cmdTxBuff.mRptr];
      cmdTxBuff.mRptr ++;
      if(cmdTxBuff.mRptr >= CMD_TXBUFFSIZE) { cmdTxBuff.mRptr -= CMD_TXBUFFSIZE; }
    }
    
    /* Transfer via usb */
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
    {
      CDC_Transmit_FS(cmdTxData, iCnt);
    }
    
    mCnt -= iCnt;
  }
  
  return (0);
}

//==============================================================================
/**
  * @brief  Commander_PutRecv
  * @param  buff
  * @param  len
  * @retval none
  * @note   push data to rxbuff
  */
int8_t Commander_PutRecv(uint8_t * buff, uint32_t len)
{
  for(uint32_t i=0; i<len; i++)
  {
    /* Push to Buff */
    cmdRxBuff.mBuff[cmdRxBuff.mWptr] = buff[i];
    /* Move to Next */
    cmdRxBuff.mWptr ++;
    if(cmdRxBuff.mWptr >= CMD_RXBUFFSIZE) { cmdRxBuff.mWptr -= CMD_RXBUFFSIZE; }
  }
  
  return (0);
}

//==============================================================================
/**
  * @brief  Commander_TaskRx
  * @param  argument
  * @retval none
  * @note   transfer message to app layer
  */
uint32_t Commander_TaskRx(void const * argument)
{
  uint32_t cmdMsgState = 0;
  /* Count msg Buff */
  uint32_t cmdMsgCnt = CMD_RXBUFFSIZE + cmdRxBuff.mWptr - cmdRxBuff.mRptr;
  if(cmdMsgCnt >= CMD_RXBUFFSIZE) { cmdMsgCnt -= CMD_RXBUFFSIZE; }
  
  /* Enter Handle If not EMPTY */
  if(cmdMsgCnt >= 4)
  {
    /*  */
    cmdMsgState = 0;
    
    //head check
    if( cmdRxBuff.mBuff[cmdRxBuff.mRptr]  == CMD_MSGHEAD)
    {
      /* Flag Waiting */
      cmdMsgState = 1;
      
      if(cmdRxBuff.mRptr != CMD_RXBUFFSIZE-1)           //normal
      {
        cmdFrameRecv.fLength = 4 + cmdRxBuff.mBuff[cmdRxBuff.mRptr + 1];
      }
      else                                              //tail
      {
        cmdFrameRecv.fLength = 4 + cmdRxBuff.mBuff[0];
      }
      
      //length check
      if(cmdMsgCnt >= cmdFrameRecv.fLength)
      {
        //copy msg to handle
        uint32_t xPtr = cmdRxBuff.mRptr;
        for(uint32_t i=0; ((i<cmdFrameRecv.fLength)&&(i<CMD_FRAMEDATALEN)); i++)
        {
          cmdFrameRecv.fData[i] = cmdRxBuff.mBuff[xPtr];
          xPtr ++;
          if(xPtr >= CMD_RXBUFFSIZE) { xPtr -= CMD_RXBUFFSIZE; }
        }
        
        //crc16 check
        uint16_t ck = CRC16_MODEBUS(&cmdFrameRecv.fData[0], cmdFrameRecv.fLength-2);
        if(((ck&0xff) == cmdFrameRecv.fData[cmdFrameRecv.fLength-2])&&          \
          (((ck>>8) == cmdFrameRecv.fData[cmdFrameRecv.fLength-1])))
        {
          /* Flag Complete */
          cmdMsgState = 2;
          
          /* Found */
          /* POST to app handle */
          Commander_AppRecv(&cmdFrameRecv.fData[2], cmdFrameRecv.fData[1]);
          
        }
        else
        {
          cmdMsgState = 0;
        }
      }
    }
    
    if(cmdMsgState == 0)
    {
      /* Found none */
      /* Move to Next */
      cmdRxBuff.mRptr ++;
      if(cmdRxBuff.mRptr >= CMD_RXBUFFSIZE) { cmdRxBuff.mRptr -= CMD_RXBUFFSIZE; }
      /*  */
      cmdMsgCnt --;
    }
    else if(cmdMsgState == 1)
    {
      /* Waiting */
    }
    else if(cmdMsgState == 2)
    {
      /* Found Complete */
      /* Move to Next */
      cmdRxBuff.mRptr += cmdFrameRecv.fLength;
      if(cmdRxBuff.mRptr >= CMD_RXBUFFSIZE) { cmdRxBuff.mRptr -= CMD_RXBUFFSIZE; }
      /*  */
      cmdMsgCnt -= cmdFrameRecv.fLength;
    }
  }
  
  return (0);
}


//==============================================================================
/**
  * @brief  Commander_AppRecv
  * @param  msgBuff
  * @param  msgLen
  * @retval none
  * @note   
  */
uint32_t Commander_AppRecv(uint8_t *msgBuff, uint32_t msgLen)
{
  uint8_t eStatus;
  uint8_t txMsg[32];
  if(msgLen >= 3)
  {
    if((msgBuff[0]&0X0F) == CMD_TYPE_READ)          //normal read
    {
      /* Read Addr */
      eStatus = Commander_If_Read(msgBuff[1], &txMsg[1], msgBuff[2]);
      if(eStatus == CMD_OK)
      {
        txMsg[0] = (msgBuff[0]&0XF0)|CMD_TYPE_READ;      //read
        Commander_AppSend(txMsg, 1+msgBuff[2]);
      }
      else
      {
        txMsg[0] = (msgBuff[0]&0XF0)|CMD_TYPE_STATUS;    //status
        txMsg[1] = eStatus;
        Commander_AppSend(txMsg, 2);
      }
    }
    else if((msgBuff[0]&0X0F) == CMD_TYPE_WRITE)    //normal write
    {
      if(msgLen == 3+msgBuff[2])
      {
        /* Write Addr */
        eStatus = Commander_If_Write(msgBuff[1], &msgBuff[3], msgBuff[2]);
      }
      else
      {
        eStatus = CMD_ERR_PARAM;
      }
      /* Status */
      txMsg[0] = (msgBuff[0]&0XF0)|CMD_TYPE_STATUS;   //status
      txMsg[1] = eStatus;
      Commander_AppSend(txMsg, 2);
    }
    
  }
  return (0);
}

//==============================================================================
/**
  * @brief  USBD_CDC_ReceiveCallback
  * @param  Buf
  * @param  Len
  * @retval none
  * @note   INT mode
  * @note   push data to rxbuff
  */
int8_t USBD_CDC_ReceiveCallback(uint8_t * Buf, uint32_t * Len)
{
  Commander_PutRecv(Buf, *Len);
  return (0);
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
