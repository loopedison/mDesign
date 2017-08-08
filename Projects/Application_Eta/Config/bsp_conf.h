/**
  ******************************************************************************
  * @file    bsp_conf.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Config for BSP
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_CONF_H__
#define __BSP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define USE_BSP_ASSERT    (1)
  
/* Exported functions ------------------------------------------------------- */
#define BSP_SYSLED_MODULE_ENABLED
#define BSP_UART1_MODULE_ENABLED
#define BSP_USBEN_MODULE_ENABLED

#ifdef BSP_SYSLED_MODULE_ENABLED
  #include "bsp_sysLed.h"
//  #define BSP_SYSLED_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
//  #define BSP_SYSLED_GPIO_PORT          (GPIOB)
//  #define BSP_SYSLED_GPIO_PIN           (GPIO_PIN_12)
//  #define BSP_SYSLED_LEVEL_TOGGLE       (0)
#endif /* BSP_SYSLED_MODULE_ENABLED */

#ifdef BSP_UART1_MODULE_ENABLED
  #include "bsp_uart1.h"
  #define BSP_UART1_USING_INT
  #define BSP_UART1_USING_DMA
#endif /* BSP_UART1_MODULE_ENABLED */

#ifdef BSP_USBEN_MODULE_ENABLED
  #include "bsp_usben.h"
//  #define BSP_USBEN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
//  #define BSP_USBEN_GPIO_PORT          (GPIOB)
//  #define BSP_USBEN_GPIO_PIN           (GPIO_PIN_12)
//  #define BSP_USBEN_LEVEL_TOGGLE       (0)
#endif /* BSP_USBEN_MODULE_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CONF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
