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
#define CONF_PERIOD         (  20UL)   /* MS */
#define PARAM_ADC_REF       (3320UL)
#define PARAM_SPEED_MIN     (   2UL)
#define PARAM_SPEED_MAX     (  20UL)
#define PARAM_YAW_MIN       ( 620UL)
#define PARAM_YAW_MID       (1660UL)
#define PARAM_YAW_MAX       (2280UL)

const Storage_MsgDataUserConfTypeDef cStorageUserConf =
{
  .xOE                = 0x01,
  .xPeriod[0]         = (uint8_t)((CONF_PERIOD)),
  .xPeriod[1]         = (uint8_t)((CONF_PERIOD)>>8),
  .xCalcMode          = 0x00,
};

const Storage_MsgDataUserParamTypeDef cStorageUserParam =
{
  .xParamADCref[0]    = (uint8_t)((PARAM_ADC_REF)),
  .xParamADCref[1]    = (uint8_t)((PARAM_ADC_REF)>>8),
  .xParamYawMin[0]    = (uint8_t)((PARAM_YAW_MIN)),
  .xParamYawMin[1]    = (uint8_t)((PARAM_YAW_MIN)>>8),
  .xParamYawMid[0]    = (uint8_t)((PARAM_YAW_MID)),
  .xParamYawMid[1]    = (uint8_t)((PARAM_YAW_MID)>>8),
  .xParamYawMax[0]    = (uint8_t)((PARAM_YAW_MAX)),
  .xParamYawMax[1]    = (uint8_t)((PARAM_YAW_MAX)>>8),
  .xParamSpeedMin[0]  = (uint8_t)((PARAM_SPEED_MIN)),
  .xParamSpeedMin[1]  = (uint8_t)((PARAM_SPEED_MIN)>>8),
  .xParamSpeedMax[0]  = (uint8_t)((PARAM_SPEED_MAX)),
  .xParamSpeedMax[1]  = (uint8_t)((PARAM_SPEED_MAX)>>8),
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
#define ADDR_USER_CONF_CALC         (0X13)

#define ADDR_USER_DATA_ADC1         (0X20)
#define ADDR_USER_DATA_ADC2         (0X22)
#define ADDR_USER_DATA_ADC3         (0X24)
#define ADDR_USER_DATA_ADC4         (0X26)
#define ADDR_USER_DATA_VOLT1        (0X28)
#define ADDR_USER_DATA_VOLT2        (0X2A)
#define ADDR_USER_DATA_VOLT3        (0X2C)
#define ADDR_USER_DATA_VOLT4        (0X2E)
#define ADDR_USER_DATA_RESERVED     (0X30)
#define ADDR_USER_DATA_BUTTON       (0X31)
#define ADDR_USER_DATA_SPEED        (0X32)
#define ADDR_USER_DATA_YAW          (0X33)
#define ADDR_USER_PARAM_ADC_VREF    (0X34)
#define ADDR_USER_PARAM_SPEED_MIN   (0X36)
#define ADDR_USER_PARAM_SPEED_MAX   (0X38)
#define ADDR_USER_PARAM_YAW_MIN     (0X3A)
#define ADDR_USER_PARAM_YAW_MID     (0X3C)
#define ADDR_USER_PARAM_YAW_MAX     (0X3E)


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
  else if(pAddr == ADDR_USER_CONF_CALC)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &hStorageMsgData.xUserConf.xCalcMode, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else if(pAddr == ADDR_USER_DATA_ADC1)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcValue[0], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_ADC2)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcValue[1], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_ADC3)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcValue[2], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_ADC4)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcValue[3], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_VOLT1)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcVoltage[0], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_VOLT2)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcVoltage[1], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_VOLT3)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcVoltage[2], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_VOLT4)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xData.xAdcVoltage[3], sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_BUTTON)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(pBuff, &tsensor.xData.xButton, sizeof(uint8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_SPEED)
  {
    if(pLen == sizeof(int8_t))
    {
      memcpy(pBuff, &tsensor.xData.xSpeed, sizeof(int8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_DATA_YAW)
  {
    if(pLen == sizeof(int8_t))
    {
      memcpy(pBuff, &tsensor.xData.xYaw, sizeof(int8_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else if(pAddr == ADDR_USER_PARAM_ADC_VREF)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xADCref, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_SPEED_MIN)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xSpeedMin, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_SPEED_MAX)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xSpeedMax, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MIN)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xYawMin, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MID)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xYawMid, sizeof(uint16_t));
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MAX)
  {
    if(pLen == sizeof(uint16_t))
    {
      memcpy(pBuff, &tsensor.xParam.xYawMax, sizeof(uint16_t));
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
  else if(pAddr == ADDR_USER_CONF_CALC)
  {
    if(pLen == sizeof(uint8_t))
    {
      memcpy(&hStorageMsgData.xUserConf.xCalcMode, pBuff, sizeof(uint8_t));
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      errStatus = CMD_OK;
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  
  else if(pAddr == ADDR_USER_PARAM_ADC_VREF)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        memcpy(&hStorageMsgData.xUserParam.xParamADCref, pBuff, sizeof(uint16_t));
        Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
        errStatus = CMD_OK;
        
        memcpy(&tsensor.xParam.xADCref, &hStorageMsgData.xUserParam.xParamADCref, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_SPEED_MIN)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        if((pBuff[0]==0xff)&&(pBuff[1]==0xff))
        {
          memcpy(&hStorageMsgData.xUserParam.xParamSpeedMin, &tsensor.xData.xAdcValue[1], sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        else
        {
          memcpy(&hStorageMsgData.xUserParam.xParamSpeedMin, pBuff, sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        memcpy(&tsensor.xParam.xSpeedMin, &hStorageMsgData.xUserParam.xParamSpeedMin, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_SPEED_MAX)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        if((pBuff[0]==0xff)&&(pBuff[1]==0xff))
        {
          memcpy(&hStorageMsgData.xUserParam.xParamSpeedMax, &tsensor.xData.xAdcValue[1], sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        else
        {
          memcpy(&hStorageMsgData.xUserParam.xParamSpeedMax, pBuff, sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        memcpy(&tsensor.xParam.xSpeedMax, &hStorageMsgData.xUserParam.xParamSpeedMax, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MIN)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        if((pBuff[0]==0xff)&&(pBuff[1]==0xff))
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMin, &tsensor.xData.xAdcValue[0], sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        else
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMin, pBuff, sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        memcpy(&tsensor.xParam.xYawMin, &hStorageMsgData.xUserParam.xParamYawMin, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MID)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        if((pBuff[0]==0xff)&&(pBuff[1]==0xff))
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMid, &tsensor.xData.xAdcValue[0], sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        else
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMid, pBuff, sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        memcpy(&tsensor.xParam.xYawMid, &hStorageMsgData.xUserParam.xParamYawMid, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
    }
    else
    {
      errStatus = CMD_ERR_PARAM;
    }
  }
  else if(pAddr == ADDR_USER_PARAM_YAW_MAX)
  {
    if(pLen == sizeof(uint16_t))
    {
      if(hStorageMsgData.xUserConf.xCalcMode != 0)
      {
        if((pBuff[0]==0xff)&&(pBuff[1]==0xff))
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMax, &tsensor.xData.xAdcValue[0], sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        else
        {
          memcpy(&hStorageMsgData.xUserParam.xParamYawMax, pBuff, sizeof(uint16_t));
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
          errStatus = CMD_OK;
        }
        memcpy(&tsensor.xParam.xYawMax, &hStorageMsgData.xUserParam.xParamYawMax, sizeof(uint16_t));
      }
      else
      {
        errStatus = CMD_ERR_PERM;
      }
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
      atxMsg[iCnt++] = tsensor.xData.xButton;
      atxMsg[iCnt++] = tsensor.xData.xSpeed;
      atxMsg[iCnt++] = tsensor.xData.xYaw;
      Commander_AppSend(atxMsg, iCnt);
    }
  }
  
  return (0);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
