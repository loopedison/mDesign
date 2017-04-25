/**
  ******************************************************************************
  * @file    config.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Main configuration file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//==============================================================================
//std
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==============================================================================
//ASSERT
#define USE_ASSERT                  (1)

//==============================================================================
//DEBUG
#define USE_DEBUG_PRINT

//==============================================================================
//PI
#define M_PI                        (3.14159265359)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef USE_ASSERT
#define ASSERT(expr) ((expr) ? (void)0 : AssertFail(__FILE__, __LINE__))
void AssertFail(char *file, int line);
#endif /* USE_ASSERT */


#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
