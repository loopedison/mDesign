/**
  ******************************************************************************
  * @file    commander.h
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Commander
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMANDER_H__
#define __COMMANDER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CMD_OK                (0X00)
#define CMD_ERR_PARAM         (0X01)
#define CMD_ERR_BUSY          (0X02)
#define CMD_ERR_UNSUPPORT     (0X03)
#define CMD_ERR_PERM          (0X10)
#define CMD_ERR_TRANSFER      (0XFF)

#define CMD_TYPE_STATUS       (0X0)
#define CMD_TYPE_READ         (0X1)
#define CMD_TYPE_WRITE        (0X2)
#define CMD_TYPE_READX        (0X3)
#define CMD_TYPE_WRITEX       (0X4)
#define CMD_TYPE_UPLOAD       (0X5)

/* Exported functions ------------------------------------------------------- */
void Commander_Init(void);
void Commander_Task(void const * argument);
uint32_t Commander_AppSend(uint8_t *pBuff, uint32_t pLen);
uint32_t Commander_AppRecv(uint8_t *msgBuff, uint32_t msgLen);

#ifdef __cplusplus
}
#endif

#endif  /* __COMMANDER_H__ */

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
