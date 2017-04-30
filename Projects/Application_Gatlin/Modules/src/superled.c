/**
  ******************************************************************************
  * @file    superled.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   superled module
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"

/* Includes ------------------------------------------------------------------*/
#include "superled.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef SUPERLED_MSG_BUFF_LEN
  #define SUPERLED_MSG_BUFF_LEN            (10)
#endif /* SUPERLED_MSG_BUFF_LEN */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//==============================================================================
/* default led off */
const SLED_MsgDef cLedDefaultOff = {(int32_t)(0x70000000), 0};

//==============================================================================
/* superled queue */
typedef struct
{
  uint32_t              qIndexH;
  SuperLed_QItemDef     qBuff[SUPERLED_MSG_BUFF_LEN];
}SuperLed_QueueDef;
static SuperLed_QueueDef SuperLed_QList;

//==============================================================================
/* superled current message to display */
typedef struct
{
  uint32_t          qIndex;
  uint32_t          msgIndex;
  SLED_MsgDef       msg;
}SuperLed_DispDef;
SuperLed_DispDef SuperLed_Cur;

//==============================================================================
/* superled uID to manage */
static SuperLed_IdDef SuperLed_uID = 0;

//==============================================================================
/* superled tick */
static uint32_t tickNew = 0;
static uint32_t tickLst = 0;

/* Private function prototypes -----------------------------------------------*/
void SuperLed_Init(void);
void SuperLed_Task(void const * argument);
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  SuperLed Init
  * @param  None 
  * @retval None 
  */
void SuperLed_Init(void)
{
  /* initial all the drivers and modules */
  BSP_SYSLED_Init();
  
  /* Default */
  SuperLed_QList.qIndexH = 0;
  SuperLed_QList.qBuff[0].counter = SUPERLED_COUNTER_MAX;
  SuperLed_QList.qBuff[0].priority = 0;
  memcpy(&SuperLed_QList.qBuff[0].message, &cLedDefaultOff, sizeof(SLED_MsgDef));
  SuperLed_uID ++;
  
  /* Update tick */
  tickNew = tickLst = HAL_GetTick();
}

//==============================================================================
/**
  * @brief  SuperLed Task
  * @param  argument 
  * @retval None 
  */
void SuperLed_Task(void const * argument)
{
  tickNew = HAL_GetTick();
  if(tickNew - tickLst >= SUPERLED_TICK_PERIOD)
  {
    /* Update tick */
    tickLst = tickNew;
    
    /* Update queue*/
    if(SuperLed_Cur.qIndex != SuperLed_QList.qIndexH)
    {
      SuperLed_Cur.qIndex = SuperLed_QList.qIndexH;
      SuperLed_Cur.msgIndex = 0;
      memcpy(&SuperLed_Cur.msg, &SuperLed_QList.qBuff[SuperLed_QList.qIndexH], sizeof(SLED_MsgDef));
    }
    
    /* Display */
    if(SuperLed_Cur.msg[SuperLed_Cur.msgIndex] > 0)
    {
      BSP_SYSLED_On();
      
      SuperLed_Cur.msg[SuperLed_Cur.msgIndex] --;
      if(SuperLed_Cur.msg[SuperLed_Cur.msgIndex] <= 0)
      {
        SuperLed_Cur.msgIndex ++;
      }
    }
    else if(SuperLed_Cur.msg[SuperLed_Cur.msgIndex] < 0)
    {
      BSP_SYSLED_Off();
      
      SuperLed_Cur.msg[SuperLed_Cur.msgIndex] ++;
      if(SuperLed_Cur.msg[SuperLed_Cur.msgIndex] >= 0)
      {
        SuperLed_Cur.msgIndex ++;
      }
    }
    else
    {
      if(SuperLed_QList.qBuff[SuperLed_QList.qIndexH].counter != SUPERLED_COUNTER_MAX)
      {
        SuperLed_QList.qBuff[SuperLed_QList.qIndexH].counter --;
      }
      if(SuperLed_QList.qBuff[SuperLed_QList.qIndexH].counter > 0)
      {
        SuperLed_Cur.msgIndex = 0;
        memcpy(&SuperLed_Cur.msg, &SuperLed_QList.qBuff[SuperLed_QList.qIndexH].message, sizeof(SuperLed_QItemDef));
      }
      else
      {
        /* Finish This Item */
        SuperLed_QList.qBuff[SuperLed_QList.qIndexH].priority = 0;
        
        /* Find the Next highest */
        for(uint32_t i=0; i<SUPERLED_MSG_BUFF_LEN; i++)
        {
          if(SuperLed_QList.qBuff[i].counter != 0)
          {
            if(SuperLed_QList.qBuff[i].priority > SuperLed_QList.qBuff[SuperLed_QList.qIndexH].priority )
            {
              SuperLed_QList.qIndexH = i;
            }
          }
        }
        
        /* Turn Off */
        BSP_SYSLED_Off();
      }
    }
  }
}

//==============================================================================
/**
  * @brief  SuperLed_Display
  * @param  *pLed 
  * @param  *pId 
  * @retval None 
  */
SuperLed_StatusTypeDef SuperLed_Display(SuperLed_QItemDef *pLed)
{
  if(pLed == NULL)  { return (SUPERLED_ERROR); }
  
  uint32_t cI = 0;
  for(uint32_t i=0; ((cI==0)&&(i<SUPERLED_MSG_BUFF_LEN)); i++)
  {
    if(SuperLed_QList.qBuff[i].counter == 0)
    {
      cI = 1;
      
      /* Return Handle */
      pLed->uId = SuperLed_uID;
      
      /* Update qBuff */
      SuperLed_QList.qBuff[i].uId = SuperLed_uID;
      SuperLed_QList.qBuff[i].counter = pLed->counter;
      if(pLed->priority == SUPERLED_PRIO_DEFAULT)
      {
        SuperLed_QList.qBuff[i].priority = SuperLed_QList.qBuff[SuperLed_QList.qIndexH].priority + 1;
      }
      else
      {
        SuperLed_QList.qBuff[i].priority = pLed->priority;
      }
      memcpy(&SuperLed_QList.qBuff[i].message, &pLed->message, sizeof(SLED_MsgDef));
      
      /* Update uid */
      SuperLed_uID ++;
      
      /* Update qIndexH */
      SuperLed_QList.qIndexH = 0;
      if(SuperLed_QList.qBuff[i].priority > SuperLed_QList.qBuff[SuperLed_QList.qIndexH].priority)
      {
        SuperLed_QList.qIndexH = i;
      }
    }
  }
  
  if(cI == 0)   { return (SUPERLED_ERROR); }
  return (SUPERLED_OK);
}

SuperLed_StatusTypeDef SuperLed_Remove(SuperLed_QItemDef *pLed)
{
  if(pLed == NULL)  { return (SUPERLED_ERROR); }
  
  uint32_t cI = 0;
  for(uint32_t i=0; ((cI==0)&&(i<SUPERLED_MSG_BUFF_LEN)); i++)
  {
    if(SuperLed_QList.qBuff[i].counter != 0)
    {
      if(SuperLed_QList.qBuff[i].uId == pLed->uId)
      {
        cI = 1;
        /* Remove this message */
        SuperLed_QList.qBuff[i].counter = 0;
        SuperLed_QList.qBuff[i].priority = 0;
      }
    }
  }
  
  /* Find the Next highest */
  for(uint32_t i=0; i<SUPERLED_MSG_BUFF_LEN; i++)
  {
    if(SuperLed_QList.qBuff[i].counter != 0)
    {
      if(SuperLed_QList.qBuff[i].priority > SuperLed_QList.qBuff[SuperLed_QList.qIndexH].priority )
      {
        SuperLed_QList.qIndexH = i;
      }
    }
  }
  
  return (SUPERLED_OK);
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
