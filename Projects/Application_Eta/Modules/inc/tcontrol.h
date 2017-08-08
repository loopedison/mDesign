/**
  ******************************************************************************
  * @file    tcontrol.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   tcontrol
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCONTROL_H__
#define __TCONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t      xReserved[1];
}Tcontrol_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern Tcontrol_TypeDef tcontroller;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Tcontrol_Init(void);
void Tcontrol_Task(void const * argument);

#ifdef __cplusplus
}
#endif

#endif  /* __TCONTROL_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
