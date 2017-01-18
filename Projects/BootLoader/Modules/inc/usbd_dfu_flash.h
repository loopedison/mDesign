/**
  ******************************************************************************
  * @file    usbd_dfu_flash.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   USB DFU Flash Function
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_DFU_FLASH_H__
#define __USBD_DFU_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern USBD_DFU_MediaTypeDef    USBD_DFU_fops_FS;
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_DFU_FLASH_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
