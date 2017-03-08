/**
  ******************************************************************************
  * @file    bsp_bluepower.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for BlueTooth Power Control
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BLUEPOWER_H__
#define __BSP_BLUEPOWER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_BLUEPOWER_Init(void);
BSP_StatusTypeDef BSP_BLUEPOWER_Off(void);
BSP_StatusTypeDef BSP_BLUEPOWER_On(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BLUEPOWER_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
