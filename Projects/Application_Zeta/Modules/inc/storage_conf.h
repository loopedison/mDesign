/**
  ******************************************************************************
  * @file    storage_conf.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   storage config
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STORAGE_CONF_H__
#define __STORAGE_CONF_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported define -----------------------------------------------------------*/
#define STORAGE_VERSION         (0xff010600)
#define STORAGE_MSG_ZISE        (0x00000010)

#define STORAGE_XFLAG_SET       (0x89abcdef)
#define STORAGE_XFLAG_RESET     (0x00000000)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t                 xName[16];
}Storage_MsgDataNameTypeDef;

typedef struct
{
  uint8_t                 xReserved[1];
  uint8_t                 xMode[1];
  uint8_t                 xPID[4];
  uint8_t                 xUID[4];
  uint8_t                 xSoftVer[2];
  uint8_t                 xHardVer[2];
  uint8_t                 xUpgrade[1];
  uint8_t                 xRestore[1];
}Storage_MsgDataSysTypeDef;

typedef struct
{
  uint8_t                 xKey[4];
  uint8_t                 xState[1];
  uint8_t                 xReserved[11];
}Storage_MsgDataSuperTypeDef;

typedef struct
{
  uint8_t                 xOE[1];
  uint8_t                 xPeriod[2];
  uint8_t                 xReserved[13];
}Storage_MsgDataUserConfTypeDef;
typedef struct
{
  uint8_t                 xReserved[1];
}Storage_MsgDataUserParamTypeDef;

typedef struct
{
  /* First Flag */
  uint32_t                          xFlag;
  /* Info */
  Storage_MsgDataNameTypeDef        xInfo;
  /* System */
  Storage_MsgDataSysTypeDef         xSys;
  /* Super */
  Storage_MsgDataSuperTypeDef       xSuper;
  /* user: AUTO */
  Storage_MsgDataUserConfTypeDef    xUserConf;
  Storage_MsgDataUserParamTypeDef   xUserParam;
}Storage_MsgDataTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif  /* __STORAGE_CONF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
