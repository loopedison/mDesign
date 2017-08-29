/**
  ******************************************************************************
  * @file    filter.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for filter
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "filter.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  iir LPFilter Single function 
  * @param  in          : 
  * @param  attenuation : 
  * @param  filt        : 
  * @retval result
  */
int16_t iirLPFilterSingle(int32_t in, int32_t attenuation, int32_t* filt)
{
  int32_t inScaled;
  int32_t filttmp = *filt;
  int16_t out;
  
  if (attenuation > (1<<IIR_SHIFT))
  {
    attenuation = (1<<IIR_SHIFT);
  }
  else if (attenuation < 1)
  {
    attenuation = 1;
  }
  
  // Shift to keep accuracy
  inScaled = in << IIR_SHIFT;
  // Calculate IIR filter
  filttmp = filttmp + (((inScaled-filttmp) >> IIR_SHIFT) * attenuation);
  // Scale and round
  out = (filttmp >> 8) + ((filttmp & (1 << (IIR_SHIFT - 1))) >> (IIR_SHIFT - 1));
  *filt = filttmp;
  
  return out;
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
