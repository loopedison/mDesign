/**
  ******************************************************************************
  * @file    crc16.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for CRC 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC16_H__
#define __CRC16_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t CRC16(uint8_t * pData, uint32_t iLenIn);

#ifdef __cplusplus
}
#endif

#endif /* __CRC16_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
