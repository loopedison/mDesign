/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for usbd config
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Common Config */
/* Maximum number of supported media : Flash */
#define USBD_MAX_NUM_INTERFACES         1
#define USBD_MAX_NUM_CONFIGURATION      1
#define USBD_MAX_STR_DESC_SIZ           0x100
#define USBD_SUPPORT_USER_STRING        1
#define USBD_SELF_POWERED               1
#define USBD_DEBUG_LEVEL                0
  
/* DFU Class Config */
#define USBD_DFU_MAX_ITF_NUM            1
#define USBD_DFU_XFER_SIZE              1024
#define USBD_DFU_APP_DEFAULT_ADD        0x08008000

/* #define for FS and HS identification */
#define DEVICE_FS 		                  0

/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy
#define USBD_Delay                HAL_Delay

 /* DEBUG macros */  
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)         printf(__VA_ARGS__);\
                                  printf("\n");
#else
#define USBD_UsrLog(...)
#endif 

#if (USBD_DEBUG_LEVEL > 1)
#define  USBD_ErrLog(...)         printf("ERROR: ") ;\
                                  printf(__VA_ARGS__);\
                                  printf("\n");
#else
#define USBD_ErrLog(...)   
#endif 

#if (USBD_DEBUG_LEVEL > 2)                         
#define  USBD_DbgLog(...)         printf("DEBUG : ") ;\
                                  printf(__VA_ARGS__);\
                                  printf("\n");
#else
#define USBD_DbgLog(...)                         
#endif

/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* __USBD_CONF__H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
