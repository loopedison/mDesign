/**
  ******************************************************************************
  * @file    cpu_id.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for CPU Id
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CPU_ID_H__
#define __CPU_ID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//The CPU ID is 12 bytes long
void CPU_getId(uint32_t id[3]);
void CPU_getFlashSize(uint32_t size[1]);

#ifdef __cplusplus
}
#endif

#endif /* _CPU_ID_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
