/**
  ******************************************************************************
  * @file    storage.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   storage
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"
#include "crc16.h"

/* Private typedef -----------------------------------------------------------*/
uint32_t                hStorageIndex = 0;
Storage_MsgDataTypeDef  hStorageMsgData;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
Storage_StatusTypeDef StorageFlash_Erase(void);
Storage_StatusTypeDef StorageFlash_ProgramWords(uint32_t addr, uint32_t *src, uint32_t len);
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Storage DeInit
  * @param  none 
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_DeInit(void)
{
  ASSERT(sizeof(Storage_TypeDef) < STORAGE_SIZE);
  /* erase pages, write Version */
  StorageFlash_Erase();
  Storage_InfoDef info;
  memset(&info, 0xff, sizeof(Storage_InfoDef));
  info.version = STORAGE_VERSION;
  Storage_WriteInfo(STORAGE_HANDLE, &info);
  return (STORAGE_OK);
}

//==============================================================================
/**
  * @brief  Storage Init
  * @param  none 
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_Init(void)
{
  Storage_StatusTypeDef status = STORAGE_OK;
  memset(&hStorageMsgData, 0, sizeof(Storage_MsgDataTypeDef));
  status = Storage_ReadIndex(STORAGE_HANDLE, &hStorageIndex);
  if(status != STORAGE_OK) { return (STORAGE_ERROR); }
  status = Storage_ReadIndexMsgData(STORAGE_HANDLE, hStorageIndex, &hStorageMsgData);
  if(status != STORAGE_OK) { return (STORAGE_ERROR); }
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Erase Flash Sector
  * @param  none 
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef StorageFlash_Erase(void)
{
  Storage_StatusTypeDef status = STORAGE_OK;
  
  HAL_FLASH_Unlock();
  /* Variable contains Flash operation status */
  FLASH_EraseInitTypeDef eraseinitstruct;
  uint32_t sectornb = 0;
  /* Get the number of sector */
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = STORAGE_ADDR;
  eraseinitstruct.NbPages = STORAGE_PAGES;
  if(HAL_FLASHEx_Erase(&eraseinitstruct, &sectornb) != HAL_OK)
  {
    status = STORAGE_ERROR;
  }
  HAL_FLASH_Lock();
  
  return (status);
}

//==============================================================================
/**
  * @brief  StorageFlash_ProgramWords
  * @param  addr 
  * @param  src
  * @param  len
  * @retval STORAGE_ERROR If Failed
  */
Storage_StatusTypeDef StorageFlash_ProgramWords(uint32_t addr, uint32_t *src, uint32_t len)
{
  Storage_StatusTypeDef status = STORAGE_OK;
  
  uint32_t pAddr = addr;
  uint32_t *pSrc = (uint32_t *)(src);
  HAL_FLASH_Unlock();
  for(uint32_t i=0; i<len; i++)
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (pAddr + i*sizeof(uint32_t)), *(pSrc+i)) != HAL_OK)
    {
      status = STORAGE_ERROR;
      break;
    }
  }
  HAL_FLASH_Lock();
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Write version
  * @param  pStorage  :Handle
  * @param  pInfo     :version to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_WriteInfo(Storage_TypeDef *pStorage, Storage_InfoDef *pInfo)
{
  ASSERT(pStorage != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  
  if(pStorage->info.version == 0xffffffff)
  {
    uint32_t xAddr = ((uint32_t)&pStorage->info.version);
    uint32_t xData = pInfo->version;
    uint32_t *pSrc = &xData;
    status = StorageFlash_ProgramWords(xAddr, pSrc, 1);
  }
  else
  {
    status = STORAGE_ERROR;
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Read version
  * @param  pStorage  :Handle
  * @param  pInfo     :version to read
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_ReadInfo(Storage_TypeDef *pStorage, Storage_InfoDef *pInfo)
{
  ASSERT(pStorage != NULL);
  ASSERT(pInfo != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  pInfo->version = pStorage->info.version;
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Write Index
  * @param  pStorage  :Handle
  * @param  index     :index to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_WriteIndex(Storage_TypeDef *pStorage, uint32_t index)
{
  ASSERT(pStorage != NULL);
  ASSERT(index < STORAGE_MSG_ZISE);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  if(pStorage->flag[index] == 0xffffffff)
  {
    uint32_t xAddr = ((uint32_t)&pStorage->flag[index]);
    uint32_t xData = STORAGE_INDEX_FLAG;
    uint32_t *pSrc = &xData;
    status = StorageFlash_ProgramWords(xAddr, pSrc, 1);
  }
  else if(pStorage->flag[index] == STORAGE_INDEX_FLAG)
  {
  }
  else
  {
    status = STORAGE_ERROR;
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Read Index
  * @param  pStorage  :Handle
  * @param  *index    :index to read
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_ReadIndex(Storage_TypeDef *pStorage, uint32_t *index)
{
  ASSERT(pStorage != NULL);
  ASSERT(index != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  for(uint32_t i=0; i<STORAGE_MSG_ZISE; i++)
  {
    if(pStorage->flag[i] == STORAGE_INDEX_FLAG)
    {
      *index = i;
    }
    else if(pStorage->flag[i] == 0xffffffff)
    {
      break;
    }
    else
    {
      status = STORAGE_ERROR;
      break;
    }
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Write Msg (info and data)
  * @param  pStorage  :Handle
  * @param  index     :
  * @param  msg       :Msg to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_WriteIndexMsg(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDef *msg)
{
  ASSERT(pStorage != NULL);
  ASSERT(msg != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  if(index >= STORAGE_MSG_ZISE)
  {
    status = STORAGE_ERROR;
  }
  else
  {
    uint32_t xAddr = (uint32_t)(&pStorage->msg[index]);
    uint32_t *pSrc = (uint32_t *)(msg);
    status = StorageFlash_ProgramWords(xAddr, pSrc, sizeof(Storage_MsgDef)/4);
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Read Msg (info and data)
  * @param  pStorage  :Handle
  * @param  index     :
  * @param  msg       :Msg to read
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_ReadIndexMsg(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDef *msg)
{
  ASSERT(pStorage != NULL);
  ASSERT(msg != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  if(index >= STORAGE_MSG_ZISE)
  {
    status = STORAGE_ERROR;
  }
  else
  {
    memcpy(msg, &pStorage->msg[index], sizeof(Storage_MsgDef));
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Write MsgData
  * @param  pStorage  :Handle
  * @param  index     :index
  * @param  msgData   :MsgData to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_WriteIndexMsgData(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDataTypeDef *msgData)
{
  ASSERT(pStorage != NULL);
  ASSERT(msgData != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  Storage_MsgDef pMsg;
  memcpy(&pMsg.data, msgData, sizeof(Storage_MsgDataTypeDef));
  
  if(hStorageIndex != index)
  {
    if(index >= STORAGE_MSG_ZISE)
    {
      index = 0;
      Storage_DeInit();
    }
    hStorageIndex = index;
    Storage_WriteIndex(pStorage, hStorageIndex);
  }
  pMsg.info.xId = index;
  pMsg.info.xCRC16 = CRC16((uint8_t *)&pMsg.data, sizeof(Storage_MsgDataTypeDef));
  status = Storage_WriteIndexMsg(pStorage, index, &pMsg);
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Read MsgData
  * @param  pStorage  :Handle
  * @param  index     :index
  * @param  msgData   :MsgData to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_ReadIndexMsgData(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDataTypeDef *msgData)
{
  ASSERT(pStorage != NULL);
  ASSERT(msgData != NULL);
  
  Storage_StatusTypeDef status = STORAGE_OK;
  if(index >= STORAGE_MSG_ZISE)
  {
    status = STORAGE_ERROR;
  }
  else
  {
    if(pStorage->msg[index].info.xId != index)
    {
      status = STORAGE_ERROR;
    }
    else
    {
      uint32_t xcrc16 = CRC16((uint8_t *)&pStorage->msg[index].data, sizeof(Storage_MsgDataTypeDef));
      if(pStorage->msg[index].info.xCRC16 != xcrc16)
      {
        status = STORAGE_ERROR;
      }
      else
      {
        memcpy(msgData, &pStorage->msg[index].data, sizeof(Storage_MsgDataTypeDef));
      }
    }
  }
  
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Write Data
  * @param  pStorage  :Handle
  * @param  msgData   :MsgData to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_WriteData(Storage_TypeDef *pStorage, Storage_MsgDataTypeDef *msgData)
{
  Storage_StatusTypeDef status = Storage_WriteIndexMsgData(pStorage, (hStorageIndex+1), msgData);
  return (status);
}

//==============================================================================
/**
  * @brief  Storage Read Data
  * @param  pStorage  :Handle
  * @param  msgData   :MsgData to write
  * @retval STORAGE_ERROR If Error
  */
Storage_StatusTypeDef Storage_ReadData(Storage_TypeDef *pStorage, Storage_MsgDataTypeDef *msgData)
{
  Storage_StatusTypeDef status = Storage_ReadIndexMsgData(pStorage, (hStorageIndex+1), msgData);
  return (status);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
