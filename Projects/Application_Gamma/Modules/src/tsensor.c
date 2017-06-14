/**
  ******************************************************************************
  * @file    tsensor.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Tsensor
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "tsensor.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//==============================================================================
/* Button */
static uint32_t   xButtonStatus = 0;

//==============================================================================
/* sensor */
Tsensor_TypeDef   tsensor;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Tsensor Init
  * @param  None
  * @retval None
  */
void Tsensor_Init(void)
{
  /* Init Device */
  BSP_KEY_Init(KEY1);
  BSP_KEY_Init(KEY2);
  BSP_KEY_Init(KEY3);
  BSP_KEY_Init(KEY4);
  
  /* Load Param */
  tsensor.xParam.xPeriod = 1;
}

//==============================================================================
/**
  * @brief  Tsensor_Task
  * @param  argument
  * @retval none
  */
void Tsensor_Task(void const * argument)
{
  static uint32_t tickButton = 0;
  uint32_t tickNew = HAL_GetTick();
  
  if(tickNew - tickButton >= tsensor.xParam.xPeriod)
  {
    tickButton = tickNew;
    
    /* update button */
    xButtonStatus = 0;
    if((BSP_KEY_Read(KEY1)==true))
    {
      xButtonStatus |= (0x1<<0);
    }
    if((BSP_KEY_Read(KEY2)==true))
    {
      xButtonStatus |= (0x1<<1);
    }
    if((BSP_KEY_Read(KEY3)==true))
    {
      xButtonStatus |= (0x1<<2);
    }
    if((BSP_KEY_Read(KEY4)==true))
    {
      xButtonStatus |= (0x1<<3);
    }
    
    /* update sensor */
    tsensor.xData.xKey = xButtonStatus;
  }
}

//==============================================================================
/**
  * @brief  Tsensor_GetInstance
  * @param  **pSensor
  * @retval none
  */
void Tsensor_GetInstance(Tsensor_TypeDef **pSensor)
{
  *pSensor = &tsensor;
}

//==============================================================================
/**
  * @brief  Tsensor_GetData
  * @param  pData
  * @retval none
  */
void Tsensor_GetData(Tsensor_DataTypeDef *pData)
{
  memcpy(pData, &tsensor.xData, sizeof(Tsensor_DataTypeDef));
}

//==============================================================================
/**
  * @brief  Tsensor_GetParam
  * @param  pParam
  * @retval none
  */
void Tsensor_GetParam(Tsensor_ParamTypeDef *pParam)
{
  memcpy(pParam, &tsensor.xParam, sizeof(Tsensor_ParamTypeDef));
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
