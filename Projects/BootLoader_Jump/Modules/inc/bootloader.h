/**
  ******************************************************************************
  * @file    bootloader.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   BootLoader
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
  BOOTLOADER_OK            = 0X0,
  BOOTLOADER_ERROR         = 0X1,
}BootLoader_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BootLoader_StatusTypeDef BootLoader_Main(void);

#endif  /* __BOOTLOADER_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
