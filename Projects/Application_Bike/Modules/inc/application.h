/**
  ******************************************************************************
  * @file    application.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Application
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLICATION_OK            = 0X0,
  APPLICATION_ERROR         = 0X1,
}Application_StatusTypeDef;



/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
Application_StatusTypeDef Application_Main(void);

#endif  /* __APPLICATION_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
