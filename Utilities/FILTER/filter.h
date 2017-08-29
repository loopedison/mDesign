/**
  ******************************************************************************
  * @file    filter.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for filter
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILTER_H__
#define __FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
#define IIR_SHIFT     (8)
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int16_t iirLPFilterSingle(int32_t in, int32_t attenuation, int32_t* filt);

#ifdef __cplusplus
}
#endif

#endif /* __FILTER_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
