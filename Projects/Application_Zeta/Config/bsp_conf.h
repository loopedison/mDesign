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
#define BSP_EXTI_MODULE_ENABLED
#define BSP_ADC1_MODULE_ENABLED
#define BSP_TIM2_MODULE_ENABLED
#define BSP_TIM3_MODULE_ENABLED
#define BSP_I2C1_MODULE_ENABLED
#define BSP_BUTTON_MODULE_ENABLED
#define BSP_LEDOUT_MODULE_ENABLED
#define BSP_DRIVER_MOTOR_MODULE_ENABLED

/* Module Configure --------------------------------------------------------- */
#ifdef BSP_SYSLED_MODULE_ENABLED
  #include "bsp_sysLed.h"
  #define BSP_SYSLED_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
  #define BSP_SYSLED_GPIO_PORT          (GPIOC)
  #define BSP_SYSLED_GPIO_PIN           (GPIO_PIN_13)
  #define BSP_SYSLED_LEVEL_TOGGLE       (0)
#endif /* BSP_SYSLED_MODULE_ENABLED */

#ifdef BSP_UART1_MODULE_ENABLED
  #include "bsp_uart1.h"
  #define BSP_UART1_USING_INT           (1)
  #define BSP_UART1_USING_DMA           (1)
  #define BSP_UART1_DEFAULT_BAUDRATE    (115200)
  #define BSP_UART1_DEFAULT_PRIO        (6)
  #define BSP_UART1_DMA_RX_PRIO         (6)
  #define BSP_UART1_DMA_TX_PRIO         (6)
#endif /* BSP_UART1_MODULE_ENABLED */

#ifdef BSP_USBEN_MODULE_ENABLED
  #include "bsp_usben.h"
  #define BSP_USBEN_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
  #define BSP_USBEN_GPIO_PORT           (GPIOA)
  #define BSP_USBEN_GPIO_PIN            (GPIO_PIN_15)
  #define BSP_USBEN_LEVEL_TOGGLE        (0)
#endif /* BSP_USBEN_MODULE_ENABLED */

#ifdef BSP_EXTI_MODULE_ENABLED
  #include "bsp_exti.h"
  #define BSP_EXTI95_USING_INT          (1)
  #define BSP_EXTI95_PRIO               (6)
#endif /* BSP_EXTI_MODULE_ENABLED */

#ifdef BSP_ADC1_MODULE_ENABLED
  #include "bsp_adc1.h"
  #define BSP_ADC1_USING_DMA            (1)
  #define BSP_ADC1_DMA_PRIO             (6)
  #define BSP_ADC1_CHANNEL_NUM          (2)         //channel
  #define BSP_ADC1_SAMPLETIME           (ADC_SAMPLETIME_7CYCLES_5)
#endif /* BSP_ADC1_MODULE_ENABLED */

#ifdef BSP_TIM2_MODULE_ENABLED
  #include "bsp_tim2.h"
  #define BSP_TIM2_DEFAULT_PRESCALER    (72-1)
  #define BSP_TIM2_DEFAULT_PERIOD       (10000-1)   //10ms
#endif /* BSP_TIM2_MODULE_ENABLED */

#ifdef BSP_TIM3_MODULE_ENABLED
  #include "bsp_tim3.h"
  #define TIM3_PWM_DEFAULT_PRESCALER    (72-1)
  #define TIM3_PWM_DEFAULT_PERIOD       (200-1)     //5k hz
  #define TIM3_PWM_DEFAULT_PULSE        (2)
  #define TIM3_PWM_CHANNEL_NUM          (2)         //channel
#endif /* BSP_TIM3_MODULE_ENABLED */

#ifdef BSP_I2C1_MODULE_ENABLED
  #include "bsp_i2c1.h"
  #define I2C1_DEFAULT_SPEED            (400000)
#endif /* BSP_I2C1_MODULE_ENABLED */

#ifdef BSP_BUTTON_MODULE_ENABLED
  #include "bsp_button.h"
  #define BUTTON_0_CONFIG               (1)
  #define BUTTON_0_GPIO_PIN             GPIO_PIN_1
  #define BUTTON_0_GPIO_PORT            GPIOA
  #define BUTTON_1_CONFIG               (1)
  #define BUTTON_1_GPIO_PIN             GPIO_PIN_2
  #define BUTTON_1_GPIO_PORT            GPIOA
  #define BUTTON_2_CONFIG               (1)
  #define BUTTON_2_GPIO_PIN             GPIO_PIN_3
  #define BUTTON_2_GPIO_PORT            GPIOA
  #define BUTTON_3_CONFIG               (1)
  #define BUTTON_3_GPIO_PIN             GPIO_PIN_4
  #define BUTTON_3_GPIO_PORT            GPIOA
  #define BUTTON_4_CONFIG               (1)
  #define BUTTON_4_GPIO_PIN             GPIO_PIN_5
  #define BUTTON_4_GPIO_PORT            GPIOA
  #define BUTTON_5_CONFIG               (1)
  #define BUTTON_5_GPIO_PIN             GPIO_PIN_6
  #define BUTTON_5_GPIO_PORT            GPIOA
  #define BUTTON_6_CONFIG               (1)
  #define BUTTON_6_GPIO_PIN             GPIO_PIN_4
  #define BUTTON_6_GPIO_PORT            GPIOB
  #define BUTTON_7_CONFIG               (1)
  #define BUTTON_7_GPIO_PIN             GPIO_PIN_5
  #define BUTTON_7_GPIO_PORT            GPIOB
  #define BUTTON_8_CONFIG               (1)
  #define BUTTON_8_GPIO_PIN             GPIO_PIN_0
  #define BUTTON_8_GPIO_PORT            GPIOB
  #define BUTTON_9_CONFIG               (1)
  #define BUTTON_9_GPIO_PIN             GPIO_PIN_1
  #define BUTTON_9_GPIO_PORT            GPIOB
#endif /* BSP_BUTTON_MODULE_ENABLED */

#ifdef BSP_LEDOUT_MODULE_ENABLED
  #include "bsp_ledout.h"
  #define LEDOUT_0_CONFIG               (1)
  #define LEDOUT_0_GPIO_PIN             GPIO_PIN_1
  #define LEDOUT_0_GPIO_PORT            GPIOA
  #define LEDOUT_1_CONFIG               (1)
  #define LEDOUT_1_GPIO_PIN             GPIO_PIN_2
  #define LEDOUT_1_GPIO_PORT            GPIOA
  #define LEDOUT_2_CONFIG               (1)
  #define LEDOUT_2_GPIO_PIN             GPIO_PIN_3
  #define LEDOUT_2_GPIO_PORT            GPIOA
  #define LEDOUT_3_CONFIG               (1)
  #define LEDOUT_3_GPIO_PIN             GPIO_PIN_4
  #define LEDOUT_3_GPIO_PORT            GPIOA
  #define LEDOUT_4_CONFIG               (1)
  #define LEDOUT_4_GPIO_PIN             GPIO_PIN_5
  #define LEDOUT_4_GPIO_PORT            GPIOA
  #define LEDOUT_5_CONFIG               (1)
  #define LEDOUT_5_GPIO_PIN             GPIO_PIN_6
  #define LEDOUT_5_GPIO_PORT            GPIOA
  #define LEDOUT_6_CONFIG               (1)
  #define LEDOUT_6_GPIO_PIN             GPIO_PIN_4
  #define LEDOUT_6_GPIO_PORT            GPIOB
  #define LEDOUT_7_CONFIG               (1)
  #define LEDOUT_7_GPIO_PIN             GPIO_PIN_5
  #define LEDOUT_7_GPIO_PORT            GPIOB
  #define LEDOUT_8_CONFIG               (1)
  #define LEDOUT_8_GPIO_PIN             GPIO_PIN_0
  #define LEDOUT_8_GPIO_PORT            GPIOB
  #define LEDOUT_9_CONFIG               (1)
  #define LEDOUT_9_GPIO_PIN             GPIO_PIN_1
  #define LEDOUT_9_GPIO_PORT            GPIOB
#endif /* BSP_LEDOUT_MODULE_ENABLED */

#ifdef BSP_DRIVER_MOTOR_MODULE_ENABLED
  #include "bsp_driver_motor.h"
  #define MOTOR_CONFIG_WITH_TIM3        (1)
#endif /* BSP_DRIVER_MOTOR_MODULE_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CONF_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
