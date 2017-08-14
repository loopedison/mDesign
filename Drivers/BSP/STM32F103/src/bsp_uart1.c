/**
  ******************************************************************************
  * @file    bsp_uart1.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Function for UART1
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#ifdef BSP_UART1_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if BSP_UART1_USING_DMA == 1
  #if BSP_UART1_USING_INT != 1
    #define BSP_UART1_USING_INT       (1)
  #endif /* BSP_UART1_USING_INT */
#endif /* BSP_UART1_USING_DMA */

#ifndef BSP_UART1_DEFAULT_BAUDRATE
  #define BSP_UART1_DEFAULT_BAUDRATE  (115200)
#endif /* BSP_UART1_DEFAULT_BAUDRATE */

#ifndef BSP_UART1_DEFAULT_PRIO
  #define BSP_UART1_DEFAULT_PRIO      (6)
#endif /* BSP_UART1_DEFAULT_PRIO */

#ifndef BSP_UART1_DMA_RX_PRIO
  #define BSP_UART1_DMA_RX_PRIO       (6)
#endif /* BSP_UART1_DMA_RX_PRIO */

#ifndef BSP_UART1_DMA_TX_PRIO
  #define BSP_UART1_DMA_TX_PRIO       (6)
#endif /* BSP_UART1_DMA_TX_PRIO */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Export variables ----------------------------------------------------------*/
UART_HandleTypeDef huart1;

#ifdef BSP_UART1_USING_DMA
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
#endif /* BSP_UART1_USING_DMA */

/* Export function prototypes ------------------------------------------------*/

//==============================================================================
/**
  * @brief  Initial UART1 with DMA
  * @param  none: 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_UART1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  
  /* Peripheral clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX
  */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
#if BSP_UART1_USING_DMA == 1
  /* Peripheral DMA init*/
  
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  hdma_usart1_rx.Instance = DMA1_Channel5;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_NORMAL;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_usart1_rx);
  
  __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
  
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, BSP_UART1_DMA_RX_PRIO, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  hdma_usart1_tx.Instance = DMA1_Channel4;
  hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_tx.Init.Mode = DMA_NORMAL;
  hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_usart1_tx);
  
  __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
  
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, BSP_UART1_DMA_TX_PRIO, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  
#endif /* BSP_UART1_USING_DMA */
  
  /* Peripheral clock enable */
  __HAL_RCC_USART1_CLK_ENABLE();
  
  huart1.Instance = USART1;
  huart1.Init.BaudRate = BSP_UART1_DEFAULT_BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
  
#if BSP_UART1_USING_INT == 1
  /* Peripheral interrupt init */
  HAL_NVIC_SetPriority(USART1_IRQn, BSP_UART1_DEFAULT_PRIO, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif /* BSP_UART1_USING_INT */
  
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  Set UART1 BaudRate
  * @param  BaudRate  : 
  * @retval BSP_OK    : normal
  */
BSP_StatusTypeDef BSP_UART1_SetBaudRate(uint32_t BaudRate)
{
  huart1.Init.BaudRate = BaudRate;
  HAL_UART_Init(&huart1);
  return (BSP_OK);
}

//==============================================================================
/**
  * @brief  This function handles USART1 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
#if BSP_UART1_USING_INT == 1
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}
#endif /* BSP_UART1_USING_INT */

//==============================================================================
/**
  * @brief  This function handles DMA1_Channel5 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
#if BSP_UART1_USING_DMA == 1
void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}
#endif /* BSP_UART1_USING_DMA */

//==============================================================================
/**
  * @brief  This function handles DMA1_Channel4 global interrupt.
  * @param  none 
  * @retval none
  * @note   interrupt
  */
#if BSP_UART1_USING_DMA == 1
void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
#endif /* BSP_UART1_USING_DMA */


#endif /* BSP_UART1_MODULE_ENABLED */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
