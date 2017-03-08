/**
  ******************************************************************************
  * @file    storage.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   storage
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STORAGE_H__
#define __STORAGE_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported define -----------------------------------------------------------*/
#define STORAGE_VERSION         (0xff000100)
#define STORAGE_MSG_ZISE        (0x000000f0)
#define STORAGE_INDEX_FLAG      (0x01234567)
#define STORAGE_ADDR            (0x0800e000)
#define STORAGE_PAGES           (0x00000004)
#define STORAGE_SIZE            (0X00001000)
#define STORAGE_HANDLE          ((Storage_TypeDef *) STORAGE_ADDR)
#define STORAGE_BOOT_FLAG       (0x89abcdef)
#define STORAGE_BOOT_COMPLETE   (0x00000000)
#define STORAGE_WRP_PAGES       (OB_WRP_PAGES60TO63)
/* Exported types ------------------------------------------------------------*/
typedef enum {
  STORAGE_OK              = 0X0,
  STORAGE_ERROR           = 0X1,
  STORAGE_WRP_ENABLED     = 0X2,
}Storage_StatusTypeDef;

typedef struct
{
  uint32_t                xId;
  uint32_t                xCRC16;
}Storage_MsgInfoTypeDef;

typedef struct
{
  uint32_t                xBootFlag;
}Storage_MsgDataTypeDef;

typedef struct
{
  Storage_MsgInfoTypeDef  info;
  Storage_MsgDataTypeDef  data;
}Storage_MsgDef;

typedef struct
{
  uint32_t                version;
}Storage_InfoDef;

typedef uint32_t Storage_FlagDef;

typedef struct
{
  Storage_InfoDef         info;
  Storage_FlagDef         flag[STORAGE_MSG_ZISE];
  Storage_MsgDef          msg[STORAGE_MSG_ZISE];
}Storage_TypeDef;

/* Exported constants --------------------------------------------------------*/
extern uint32_t                hStorageIndex;
extern Storage_MsgDataTypeDef  hStorageMsgData;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
Storage_StatusTypeDef Storage_DeInit(void);
Storage_StatusTypeDef Storage_Init(void);
Storage_StatusTypeDef Storage_WriteInfo(Storage_TypeDef *pStorage, Storage_InfoDef *pInfo);
Storage_StatusTypeDef Storage_ReadInfo(Storage_TypeDef *pStorage, Storage_InfoDef *pInfo);
Storage_StatusTypeDef Storage_WriteIndex(Storage_TypeDef *pStorage, uint32_t index);
Storage_StatusTypeDef Storage_ReadIndex(Storage_TypeDef *pStorage, uint32_t *index);
Storage_StatusTypeDef Storage_WriteIndexMsg(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDef *msg);
Storage_StatusTypeDef Storage_ReadIndexMsg(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDef *msg);
Storage_StatusTypeDef Storage_WriteIndexMsgData(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDataTypeDef *msgData);
Storage_StatusTypeDef Storage_ReadIndexMsgData(Storage_TypeDef *pStorage, uint32_t index, Storage_MsgDataTypeDef *msgData);
Storage_StatusTypeDef Storage_WriteData(Storage_TypeDef *pStorage, Storage_MsgDataTypeDef *msgData);
Storage_StatusTypeDef Storage_ReadData(Storage_TypeDef *pStorage, Storage_MsgDataTypeDef *msgData);


#endif  /* __STORAGE_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
