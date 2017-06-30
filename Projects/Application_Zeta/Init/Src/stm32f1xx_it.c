/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Interrupt Handler
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "bsp.h"

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

//==============================================================================
/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  while (1)
  {
  }
}

//==============================================================================
/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

//==============================================================================
/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

//==============================================================================
/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

//==============================================================================
/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

//==============================================================================
/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
}

//==============================================================================
/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
}

//==============================================================================
/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
}

//==============================================================================
/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/


//==============================================================================
//==============================================================================
/**
  * @brief  DMA1_Channel1_IRQHandler
  */
void DMA1_Channel1_IRQHandler(void)
{
#ifdef BSP_ADC1_MODULE_ENABLED
  HAL_DMA_IRQHandler(&hdma_adc1);
#endif /* BSP_ADC1_MODULE_ENABLED */
}

//==============================================================================
__weak void HAL_GPIO_EXTI_Callback_EXTI0(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the exti is needed,
   */
}
__weak void HAL_GPIO_EXTI_Callback_EXTI1(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the exti is needed,
   */
}
__weak void HAL_GPIO_EXTI_Callback_EXTI5(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the exti is needed,
   */
}
__weak void HAL_GPIO_EXTI_Callback_EXTI6(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the exti is needed,
   */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#ifdef BSP_EXTI0_MODULE_ENABLED
  if(GPIO_Pin == GPIO_PIN_0)
  {
    HAL_GPIO_EXTI_Callback_EXTI0(GPIO_Pin);
  }
#endif /* BSP_EXTI0_MODULE_ENABLED */
  
#ifdef BSP_EXTI1_MODULE_ENABLED
  if(GPIO_Pin == GPIO_PIN_1)
  {
    HAL_GPIO_EXTI_Callback_EXTI1(GPIO_Pin);
  }
#endif /* BSP_EXTI1_MODULE_ENABLED */
  
#ifdef BSP_EXTI_MODULE_ENABLED
  if(GPIO_Pin == GPIO_PIN_5)
  {
    HAL_GPIO_EXTI_Callback_EXTI5(GPIO_Pin);
  }
  else if(GPIO_Pin == GPIO_PIN_6)
  {
    HAL_GPIO_EXTI_Callback_EXTI6(GPIO_Pin);
  }
#endif /* BSP_EXTI_MODULE_ENABLED */
}

//==============================================================================
/**
  * @brief  OTG_FS_IRQHandler
  */
extern PCD_HandleTypeDef hpcd_USB_FS;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

//==============================================================================

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
