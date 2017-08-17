/**
  ******************************************************************************
  * @file    thandler.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   handler for sensor with control
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __THANDLER_H__
#define __THANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t       xReserved;
}Thandler_DataTypeDef;

typedef struct
{
  uint8_t       xReserved;
}Thandler_ParamTypeDef;

typedef struct
{
  Thandler_DataTypeDef   xData;
  Thandler_ParamTypeDef  xParam;
}Thandler_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern Thandler_TypeDef thandler;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Thandler_Init(void);
void Thandler_Task(void const * argument);

#ifdef __cplusplus
}
#endif

#endif  /* __THANDLER_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
