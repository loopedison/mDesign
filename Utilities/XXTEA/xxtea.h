/**
  ******************************************************************************
  * @file    xxtea.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for XXTEA
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XXTEA_H__
#define __XXTEA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void btea(uint32_t *v, int n, uint32_t const key[4]);

#ifdef __cplusplus
}
#endif

#endif /* __XXTEA_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
