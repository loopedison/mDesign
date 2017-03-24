/**
  ******************************************************************************
  * @file    application.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Application
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "cpu_id.h"
/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
/* Includes ------------------------------------------------------------------*/
#include "storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define FIRMWARE_UPGRADE_FLAG_ADDR    (0x20001f00)
#define FIRMWARE_UPGRADE_FLAG_KEYWORD (0XCDEF89AB)
volatile uint32_t firmwareUpgradeFlag __attribute__((at(FIRMWARE_UPGRADE_FLAG_ADDR)));

//==============================================================================
/* Sys Run Level at Flash 0x08003400 */
#define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
#define SYS_RUN_LEVEL_ADDR            (0x0800a000)
const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;

//==============================================================================
/* cpu id */
static uint32_t sysCpuId[3] = {0};

//==============================================================================
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;

//==============================================================================
typedef struct
{
  int32_t   xDirection;
  int32_t   xValue;
}Tsensor_CounterDef;
static Tsensor_CounterDef tsensorCounter;

//==============================================================================
typedef struct
{
  int32_t   iFlag;
  int32_t   xVal;
}Tsensor_RateDataDef;

typedef struct
{
  uint32_t              xPeriod;
  uint32_t              xDivision;
  int32_t               xValue;
  Tsensor_RateDataDef   *pDataBuff;
}Tsensor_RateDef;
static Tsensor_RateDef tsensorRate;

//==============================================================================
typedef struct
{
  uint32_t              xPeriod;
  uint8_t               *pDataBuff;
}Tsensor_MsgDef;
static Tsensor_MsgDef tsensorMsg;

/* Private function prototypes -----------------------------------------------*/
uint32_t Tsensor_RateInit(Tsensor_RateDef *pRate);
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue);
uint32_t BlueTooth_InitConfig(void);

/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  Application Deal with message
  * @param  None
  * @retval None
  */
Application_StatusTypeDef Application_Main(void)
{
  /* Get CPU ID */
  CPU_getId(sysCpuId);
  
  
  /* Init BSP SYSLED */
  BSP_Init();
  BSP_SYSLED_Init();
  BSP_UART1_Init();
  BSP_USBEN_Init();
  BSP_EXTI0_Init();
  BSP_EXTI1_Init();
  BSP_BLUEPOWER_Init();
  BSP_BLUESET_Init();
  BSP_BLUESTATE_Init();
  
  
  /* Flash SYSLED Start */
  BSP_SYSLED_On();
  HAL_Delay(25);
  BSP_SYSLED_Off();
  HAL_Delay(25);
  
  
  /* Get Run Level */
  uint32_t *pRunLevel = (uint32_t *)&SysRunLevel;
  if(*pRunLevel == 0xffffffff)
  {
    /* Modify Run Flag */
    HAL_Delay(1);
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (SYS_RUN_LEVEL_ADDR), (SYS_RUN_LEVEL_KEYWORD));
    HAL_FLASH_Lock();
    
    /* DeInit Storage Default */
    HAL_Delay(1);
    Storage_DeInit();
    const Storage_MsgDataTypeDef cDefaultMsgData =
    {
      .xBootFlag            = STORAGE_BOOT_FLAG,
    };
    Storage_WriteIndex(STORAGE_HANDLE, 0);
    Storage_WriteIndexMsgData(STORAGE_HANDLE, 0, (Storage_MsgDataTypeDef *)&cDefaultMsgData);
    
    /* Generate system reset to allow jumping to the user code */
    HAL_Delay(1);
    NVIC_SystemReset();
    while(1);
  }
  else if(*pRunLevel == SYS_RUN_LEVEL_KEYWORD)
  {
    /* Init Storage */
    Storage_StatusTypeDef status = Storage_Init();
    if(status != STORAGE_OK)
    {
      while(1)
      {
        HAL_Delay(1600);
        BSP_SYSLED_On();
        HAL_Delay(250);
        BSP_SYSLED_Off();
        HAL_Delay(50);
        BSP_SYSLED_On();
        HAL_Delay(50);
        BSP_SYSLED_Off();
        HAL_Delay(50);
      }
    }
    
    if(hStorageMsgData.xBootFlag == STORAGE_BOOT_FLAG)
    {
      /* Config BlueTooth */
      BlueTooth_InitConfig();
      
      /*  */
      hStorageMsgData.xBootFlag = STORAGE_BOOT_COMPLETE;
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
      
      /* Generate system reset to allow jumping to the user code */
      HAL_Delay(1);
      NVIC_SystemReset();
      while(1);
    }
    else
    {
      /* Power On Blue */
      BSP_BLUEPOWER_Off();
      BSP_BLUESET_Disable();
      HAL_Delay(1);
      BSP_BLUEPOWER_On();
      HAL_Delay(100);
    }
    
    
    /* Initialize For Counter */
    tsensorCounter.xDirection = 0;
    tsensorCounter.xValue = 0;
    
    /* Initialize For Rate */
    tsensorRate.xPeriod = 100;
    tsensorRate.xDivision = 10;
    tsensorRate.xValue = 0;
    tsensorRate.pDataBuff = malloc(tsensorRate.xDivision*sizeof(Tsensor_RateDataDef));
    ASSERT(tsensorRate.pDataBuff != NULL);
    Tsensor_RateInit(&tsensorRate);
    
    
    /* Initialize For Message to Send */
    tsensorMsg.xPeriod = 100;
    tsensorMsg.pDataBuff = malloc(16);
    ASSERT(tsensorRate.pDataBuff != NULL);
    
    
    uint32_t tickNew = 0;
    uint32_t tickLst = 0;
    uint32_t tickLed = 0;
    uint32_t tickRate = 0;
    uint32_t tickTx = 0;
    uint32_t tickSleep = 0;
    
    
    /* Disable USB Connection */
    BSP_USBEN_Off();
    
    /* Init Device Library,Add Supported Class and Start the library*/
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
    USBD_Start(&hUsbDeviceFS);
    
    /* Enable USB Connection */
    BSP_USBEN_On();
    
    
    while(1)
    {
      /* tick */
      tickNew = HAL_GetTick();
      if(tickNew - tickLst >= 1)
      {
        tickLst = tickNew;
        tickLed++;
        tickRate++;
        tickTx++;
        tickSleep++;
      }
      
      
      /* update led state */
      if(tickLed >= 50)
      {
        tickLed = 0;
        if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
        {
          BSP_SYSLED_On();
        }
        else
        {
          BSP_SYSLED_Toggle();
        }
      }
      
      /* update rate */
      if(tickRate >= tsensorRate.xPeriod/tsensorRate.xDivision)
      {
        tickRate = 0;
        /* calc rate */
        Tsensor_RateCalc(&tsensorRate, tsensorCounter.xValue);
      }
      
      /* send msg */
      if(tickTx >= tsensorMsg.xPeriod)
      {
        tickTx = 0;
        
        tsensorMsg.pDataBuff[0] = 0x10;
        tsensorMsg.pDataBuff[1] = 0x00;
        tsensorMsg.pDataBuff[2] = 0x05;
        tsensorMsg.pDataBuff[3] = 0x00;
        tsensorMsg.pDataBuff[4] = 0x00;
        tsensorMsg.pDataBuff[5] = (tsensorRate.xValue&0x00ff);
        tsensorMsg.pDataBuff[6] = (tsensorRate.xValue&0xff00)>>8;
        tsensorMsg.pDataBuff[7] = 0x00;
        tsensorMsg.pDataBuff[8] = 0xff;
        tsensorMsg.pDataBuff[9] = 0xff;
        
        if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
        {
          CDC_Transmit_FS(tsensorMsg.pDataBuff, 10);
        }
        
        HAL_UART_Transmit(&huart1, tsensorMsg.pDataBuff, 10, 2);
      }
      
      
      /* Upgrade Firmware If Need */
      if(firmwareUpgradeFlag == FIRMWARE_UPGRADE_FLAG_KEYWORD)
      {
        /* Generate system reset to allow jumping to the user code */
        BSP_USBEN_Off();
        HAL_Delay(2);
        NVIC_SystemReset();
        while(1);
      }
      
      
      /* low power control */
      if(tsensorRate.xValue != 0)
      {
        tickSleep = 0;
      }
      
      if(tickSleep == 0)
      {
        BSP_BLUEPOWER_On();
      }
      else if(tickSleep == 300000)
      {
        BSP_BLUEPOWER_Off();
      }
      
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
    }
  }
  else
  {
    /* Failed to Endless */
    while(1)
    {
      BSP_SYSLED_Toggle();
      HAL_Delay(25);
    }
  }
}

//==============================================================================
/**
  * @brief  
  * @param  
  * @retval 
  */
uint32_t HAL_UART_TransmitString(const char *s)
{
  uint32_t slen = strlen(s);
  HAL_UART_Transmit(&huart1, (uint8_t *)s, slen, (1+slen/5));
  return (0);
}

//==============================================================================
/**
  * @brief  
  * @param  
  * @retval 
  */
uint32_t BlueTooth_InitConfig(void)
{
  /* Set UART Com Baud */
  BSP_UART1_SetBaudRate(38400);
  
  /* Turn to Set Mode , and Power On*/
  BSP_BLUEPOWER_Off();
  BSP_BLUESET_Enable();
  HAL_Delay(10);
  BSP_BLUEPOWER_On();
  HAL_Delay(500);
  
  /*Configure this module*/
  HAL_UART_TransmitString("AT\r\n");
  HAL_Delay(100);
  HAL_UART_TransmitString("AT\r\n");
  HAL_Delay(100);
  HAL_UART_TransmitString("AT+ROLE=0\r\n");
  HAL_Delay(100);
  HAL_UART_TransmitString("AT+UART=115200,0,0\r\n");
  HAL_Delay(100);
  HAL_UART_TransmitString("AT+PSWD=JISHIVR_WALKER\r\n");
  HAL_Delay(100);
  HAL_UART_TransmitString("AT+NAME=JISHIVR_");
  uint8_t kid[8];
  sprintf((char *)kid, "%08X", sysCpuId[0]);
  HAL_UART_Transmit(&huart1, kid, 8, 2);
  HAL_UART_TransmitString("\r\n");
  HAL_Delay(100);
  
  /* Power Off */
  BSP_BLUEPOWER_Off();
  HAL_Delay(500);
  
  return (0);
}

//==============================================================================
/**
  * @brief  USBD_CDC_ReceiveCallback
  * @param  Buf
  * @param  Len
  * @retval none
  */
int8_t USBD_CDC_ReceiveCallback(uint8_t* Buf, uint32_t *Len)
{
  if(*Len >= 4)
  {
    if(Buf[0] == 0XAB)
    {
      uint8_t ck = 0;
      for(uint32_t i=0; i<3; i++)
      {
        ck ^= Buf[i];
      }
      if(ck == Buf[3])
      {
        if(Buf[1] == 0x00)
        {
          if(Buf[2] == 0x01)
          {
            firmwareUpgradeFlag = FIRMWARE_UPGRADE_FLAG_KEYWORD;
          }
        }
      }
    }
  }
  return (0);
}

//==============================================================================
/**
  * @brief  EXTI Interrupt
  * @param  GPIO_PIN
  * @retval none
  * @note   INT mode
  */
void HAL_GPIO_EXTI_Callback_EXTI0(uint16_t GPIO_Pin)
{
  if(tsensorCounter.xDirection == 0)
  {
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    { tsensorCounter.xValue -= 1; }
    else
    { tsensorCounter.xValue += 1; }
  }
  else
  {
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    { tsensorCounter.xValue += 1; }
    else
    { tsensorCounter.xValue -= 1; }
  }
}

//==============================================================================
/**
  * @brief  Tsensor Rate Init
  * @param  pRate   :
  * @retval none
  */
uint32_t Tsensor_RateInit(Tsensor_RateDef *pRate)
{
  for(uint32_t i=0; i<pRate->xDivision; i++)
  {
    pRate->pDataBuff[i].iFlag = i;
    pRate->pDataBuff[i].xVal = 0;
  }
  return (0);
}

//==============================================================================
/**
  * @brief  Tsensor Rate Calc
  * @param  pRate   :
  * @param  iValue  :
  * @retval rate value
  */
uint32_t Tsensor_RateCalc(Tsensor_RateDef *pRate, int32_t iValue)
{
  for(uint32_t i=0; i<pRate->xDivision; i++)
  {
    pRate->pDataBuff[i].iFlag += 1;
    if(pRate->pDataBuff[i].iFlag >= pRate->xDivision)
    {
      pRate->pDataBuff[i].iFlag = 0;
      pRate->xValue = iValue - pRate->pDataBuff[i].xVal;
      pRate->pDataBuff[i].xVal = iValue;
    }
  }
  return (0);
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
