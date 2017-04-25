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
#define STORAGE_VERSION         (0xff000101)
#define STORAGE_MSG_ZISE        (0x00000010)

#define STORAGE_XFLAG_SET       (0x89abcdef)
#define STORAGE_XFLAG_RESET     (0x00000000)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t                xFlag;
  uint32_t                uID;
  uint8_t                 sID[16];
}Storage_MsgDataTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif  /* __STORAGE_CONF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
