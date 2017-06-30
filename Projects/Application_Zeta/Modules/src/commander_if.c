/**
  ******************************************************************************
  * @file    commander_if.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Commander interface
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "commander.h"
#include "commander_if.h"
#include "cpu_id.h"
#include "crc16.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "tsensor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define CONF_PERIOD         (  50UL)   /* MS */

const Storage_MsgDataUserConfTypeDef cStorageUserConf =
{
  .xOE                = 0xff,
  .xPeriod[0]         = (uint8_t)((CONF_PERIOD)),
  .xPeriod[1]         = (uint8_t)((CONF_PERIOD)>>8),
};

const Storage_MsgDataUserParamTypeDef cStorageUserParam =
{
  .xReserved[0]       = (uint8_t)((0)),
};

#define ADDR_SYS_RESERVED           (0X00)
#define ADDR_SYS_MODE               (0X01)
#define ADDR_SYS_PID                (0X02)
#define ADDR_SYS_UID                (0X06)
#define ADDR_SYS_SOFTVER            (0X0A)
#define ADDR_SYS_HARDVER            (0X0C)
#define ADDR_SYS_UPGRADE            (0X0E)
#define ADDR_SYS_RESTORE            (0X0F)

#define ADDR_USER_CONF_OE           (0X10)
#define ADDR_USER_CONF_PERIOD       (0X11)

#define ADDR_USER_DATA_KEY         (0X20)


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Commander_If_Read
  * @param  pAddr
  * @param  pBuff
  * @param  pLen
  * @retval 0 if success
  */
uint32_t Commander_If_Read(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen)
{
  uint32_t errStatus = CMD_OK;
  
  if(pAddr == ADDR_SYS_RESERVED)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xReserved, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_MODE)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xMode, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_PID)
  {
    if(pLen == sizeof(uint32_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xPID, sizeof(uint32_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_UID)
  {
    if(pLen == sizeof(uint32_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xUID, sizeof(uint32_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_SOFTVER)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xSoftVer, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_HARDVER)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xHardVer, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_UPGRADE)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xUpgrade, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_RESTORE)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xSys.xRestore, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else if(pAddr == ADDR_USER_CONF_OE)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xUserConf.xOE, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_CONF_PERIOD)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &hStorageMsgData.xUserConf.xPeriod, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else if(pAddr == ADDR_USER_DATA_KEY)
  {
    if(pLen == sizeof(uint8_t))
    {
      uint8_t xKeys = (uint8_t)tsensor.xData.xKey;
      memcpy(pBuff, &xKeys, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else
  {
    errStatus = CMD_ERR_UNSUPPORT;
  }
  
  return (errStatus);
}

//==============================================================================
/**
  * @brief  Commander_If_Write
  * @param  pAddr
  * @param  pBuff
  * @param  pLen
  * @retval 0 if success
  */
uint32_t Commander_If_Write(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen)
{
  uint32_t errStatus = CMD_OK;
  
  if(pAddr == ADDR_SYS_RESERVED)
  {
    errStatus = CMD_ERR_UNSUPPORT;
  }
  else if(pAddr == ADDR_SYS_MODE)
  {
    errStatus = CMD_ERR_PERM;
  }
  else if(pAddr == ADDR_SYS_PID)
  {
    if(pLen == sizeof(uint32_t))
    {
      memcpy(&hStorageMsgData.xSys.xPID, pBuff, sizeof(uint32_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_UID)
  {
    if(pLen == sizeof(uint32_t))
    {
      memcpy(&hStorageMsgData.xSys.xUID, pBuff, sizeof(uint32_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_SOFTVER)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(&hStorageMsgData.xSys.xSoftVer, pBuff, sizeof(uint16_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_HARDVER)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(&hStorageMsgData.xSys.xHardVer, pBuff, sizeof(uint16_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_UPGRADE)
  {
    if(pLen == sizeof(uint8_t))
    {
      //UPGRADE
      memcpy(&hStorageMsgData.xSys.xUpgrade, pBuff, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_SYS_RESTORE)
  {
    if(pLen == sizeof(uint8_t))
    {
      //RESTORE
      memcpy(&hStorageMsgData.xUserConf, &cStorageUserConf, sizeof(Storage_MsgDataUserConfTypeDef));
      memcpy(&hStorageMsgData.xUserParam, &cStorageUserParam, sizeof(Storage_MsgDataUserParamTypeDef));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_CONF_OE)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(&hStorageMsgData.xUserConf.xOE, pBuff, sizeof(uint8_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_CONF_PERIOD)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(&hStorageMsgData.xUserConf.xPeriod, pBuff, sizeof(uint16_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else
  {
    errStatus = CMD_ERR_UNSUPPORT;
  }
  
  return (errStatus);
}


//##############################################################################
//##############################################################################
//==============================================================================
/**
  * @brief  Commander_If_ReadX
  * @param  pAddr
  * @param  pBuff
  * @param  pLen
  * @retval 0 if success
  */
uint32_t Commander_If_ReadX(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen)
{
  uint32_t errStatus = CMD_OK;
  
  if((pAddr <= 0x0f))
  {
    memcpy(pBuff, (uint8_t *)&hStorageMsgData.xSys+pAddr, pLen*sizeof(uint8_t));
  }
  
  return (errStatus);
}

//==============================================================================
/**
  * @brief  Commander_If_ReadX
  * @param  pAddr
  * @param  pBuff
  * @param  pLen
  * @retval 0 if success
  */
uint32_t Commander_If_WriteX(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen)
{
  uint32_t errStatus = CMD_OK;
  
  return (errStatus);
}


//##############################################################################
//##############################################################################
//##############################################################################
//==============================================================================
/**
  * @brief  Commander_TaskAutoUpload
  * @param  argument
  * @retval none
  * @note   auto upload
  */
uint32_t Commander_If_AutoUpload(void const * argument)
{
  static uint32_t tickNew = 0;
  static uint32_t tickLst = 0;
  
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= *(uint16_t *)hStorageMsgData.xUserConf.xPeriod)
  {
    /* Update tick */
    tickLst = tickNew;
    
    if(hStorageMsgData.xUserConf.xOE[0] == 0xff)
    {
      /* Upload Message If Connected */
      uint8_t atxMsg[16];
      uint32_t iCnt = 0;
      atxMsg[iCnt++] = 0xaa;
      atxMsg[iCnt++] = 0x55;
      atxMsg[iCnt++] = tsensor.xData.xKey;
      uint8_t ckByte = 0;
      for(uint32_t i=0; i<iCnt; i++)
      {
        ckByte ^= atxMsg[i];
      }
      atxMsg[iCnt++] = ckByte;
      
      /* Transfer via usb */
/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
      extern USBD_HandleTypeDef hUsbDeviceFS;
      if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
      {
        CDC_Transmit_FS(atxMsg, iCnt);
      }
      
      /* Transfer via uart1 */
      HAL_UART_Transmit(&huart1, atxMsg, iCnt, 10);
    }
    else if(hStorageMsgData.xUserConf.xOE[0] == 0xfe)
    {
    }
    else if(hStorageMsgData.xUserConf.xOE[0] >= 0x01)
    {
      /* Upload Message If Connected */
      uint8_t atxMsg[8];
      uint32_t iCnt = 0;
      atxMsg[iCnt++] = (0XF<<4)|CMD_TYPE_UPLOAD;
      atxMsg[iCnt++] = tsensor.xData.xKey;
      Commander_AppSend(atxMsg, iCnt);
    }
  }
  
  return (0);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
