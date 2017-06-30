/**
  ******************************************************************************
  * @file    commander_if.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Commander interface
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMANDER_IF_H__
#define __COMMANDER_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Includes ------------------------------------------------------------------*/
#include "storage_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern const Storage_MsgDataUserConfTypeDef cStorageUserConf;
extern const Storage_MsgDataUserParamTypeDef cStorageUserParam;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t Commander_If_Read(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen);
uint32_t Commander_If_Write(uint32_t pAddr, uint8_t *pBuff, uint32_t pLen);
uint32_t Commander_If_AutoUpload(void const * argument);

#ifdef __cplusplus
}
#endif

#endif  /* __COMMANDER_IF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
