/**
  ******************************************************************************
  * @file    tsensor.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Tsensor
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSENSOR_H__
#define __TSENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t      xCoin;
}Tsensor_DataTypeDef;

typedef struct
{
  uint16_t      xPeriod;
}Tsensor_ParamTypeDef;

typedef struct
{
  Tsensor_DataTypeDef   xData;
  Tsensor_ParamTypeDef  xParam;
}Tsensor_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern Tsensor_TypeDef tsensor;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Tsensor_Init(void);
void Tsensor_Task(void const * argument);
void Tsensor_GetInstance(Tsensor_TypeDef **pSensor);
void Tsensor_GetData(Tsensor_DataTypeDef *pData);
void Tsensor_GetParam(Tsensor_ParamTypeDef *pParam);

#ifdef __cplusplus
}
#endif

#endif  /* __TSENSOR_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
