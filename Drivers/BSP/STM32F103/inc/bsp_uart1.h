/**
  ******************************************************************************
  * @file    bsp_uart1.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for UART1
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_UART1_H__
#define __BSP_UART1_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef   BSP_UART1_Init(void);
BSP_StatusTypeDef   BSP_UART1_SetBaudRate(uint32_t BaudRate);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART1_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
