/**
  ******************************************************************************
  * @file    bsp.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   board support packet
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H__
#define __BSP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp_def.h"
#include "bsp_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BSP_StatusTypeDef BSP_Init(void);

#ifdef USE_BSP_ASSERT
#define BSP_ASSERT(e) \
        if (e) ;      \
        else BSP_AssertFail( #e, __FILE__, __LINE__ )
void BSP_AssertFail(char *exp, char *file, int line);
#endif /* USE_BSP_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
