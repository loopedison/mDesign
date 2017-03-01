/**
  ******************************************************************************
  * @file    bootloader.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   BootLoader
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "bsp.h"
/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"
#include "cpu_id.h"
#include "xxtea.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_dfu.h"
#include "usbd_dfu_flash.h"
#include "storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Sys Boot Version */
#define SYS_BOOT_VERSION              (0XAA000100)

/* Sys Want to Boot at RAM 0x08020000 */
#define SYS_APP_RUN_ADDR              (0x08008000)

/* Sys Run Level at Flash 0x08003400 */
#define SYS_RUN_LEVEL_KEYWORD         (0XCDEF89AB)
#define SYS_RUN_LEVEL_ADDR            (0x08004000)
const uint32_t SysRunLevel __attribute__((at(SYS_RUN_LEVEL_ADDR))) = 0xffffffff;

/* Upgrade Flag at RAM 0x20001f00 */
#define FIRMWARE_UPGRADE_FLAG_KEYWORD (0XCDEF89AB)
#define FIRMWARE_UPGRADE_FLAG_ADDR    (0x20001f00)
volatile uint32_t firmwareUpgradeFlag __attribute__((at(FIRMWARE_UPGRADE_FLAG_ADDR)));

/* cpuid */
#define SYS_CPU_ID_KEYWORD            (0xa5a5a5a5)
#define SYS_CPU_ID_ADDR               (0x08004004)
const uint32_t SysCpuId[3] __attribute__((at(SYS_CPU_ID_ADDR))) = {0xffffffff, 0xffffffff, 0xffffffff};

/* Encrypt */
#define ENCRYPT_KEY_ADDR              (0x08004010)
const uint32_t ENCRYPT_KeyWord[4] __attribute__((at(ENCRYPT_KEY_ADDR))) =   {0XE9F78F0B,0X43F215FD,0X77E68673,0XD86C3B82};
#define ENCRYPT_ARRAY_SIZE            (0x8)

/* Must size 8*uint32 */
#define BOOT_DFU_FILE_HEADER_SIZE     (0x8)
typedef struct
{
  uint32_t bootVer;
  uint32_t xDataSize;
  uint32_t reserved[6];
}BOOT_DfuFileInfoType;

/* boot flag */
#define SYS_BOOT_FLAG_KEYWORD         (0XCDEF89AB)

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef hUsbDeviceFS;
/* Private function prototypes -----------------------------------------------*/
BootLoader_StatusTypeDef BootLoader_FirmwareUpgrade(uint32_t xAddrDest, uint32_t xAddrSrc, uint32_t xDataSize);
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  BootLoader Deal with message
  * @param  None
  * @retval None
  */
BootLoader_StatusTypeDef BootLoader_Main(void)
{
  /* Get CPU ID */
  uint32_t sysCpuId[3];
  CPU_getId(sysCpuId);
  
  /* Init BSP SYSLED */
  BSP_Init();
  BSP_SYSLED_Init();
  BSP_USBEN_Init();
  
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
    
    /* Modify Cpu ID */
    HAL_Delay(1);
    HAL_FLASH_Unlock();
    for(uint32_t i=0; i<3; i++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (SYS_CPU_ID_ADDR + i*sizeof(uint32_t)), (sysCpuId[i]^SYS_CPU_ID_KEYWORD));
    }
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
    /* check cpu id */
    uint32_t *pCpuID = (uint32_t *)SysCpuId;
    for(uint32_t i=0; i<3; i++)
    {
      ASSERT((sysCpuId[i]^SYS_CPU_ID_KEYWORD) == *(pCpuID+i));
    }
    
    /* sys boot flag */
    uint32_t bootFlag = 0;
    
    /* Check Upgrade Flag On Button PA0 */
    /* Init key PA0 */
    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOA_CLK_ENABLE();
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Read Button */
    HAL_Delay(2);
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
      bootFlag = SYS_BOOT_FLAG_KEYWORD;
    }
    
    /* Check Upgrade Flag On RAM */
    if(firmwareUpgradeFlag == FIRMWARE_UPGRADE_FLAG_KEYWORD)
    {
      firmwareUpgradeFlag = 0xffffffff;
      bootFlag = SYS_BOOT_FLAG_KEYWORD;
    }
    
    /* Init Storage */
    Storage_StatusTypeDef status = Storage_Init();
    if(status != STORAGE_OK)
    {
      while(1)
      {
        HAL_Delay(1800);
        BSP_SYSLED_On();
        HAL_Delay(50);
        BSP_SYSLED_Off();
        HAL_Delay(50);
        BSP_SYSLED_On();
        HAL_Delay(50);
        BSP_SYSLED_Off();
        HAL_Delay(50);
      }
    }
    
    /* Add Upgrade Flag If Need */
    if((bootFlag == SYS_BOOT_FLAG_KEYWORD)&&(hStorageMsgData.xBootFlag != STORAGE_BOOT_FLAG))
    {
      hStorageMsgData.xBootFlag = STORAGE_BOOT_FLAG;
      Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
    }
    
    /* Check Boot Flag On Storage Msg */
    /* Upgrade Firmware If Complete */
    if(hStorageMsgData.xBootFlag == STORAGE_BOOT_COMPLETE)
    {
      /* Unsecure Code From Download */
      uint32_t fileInfo[BOOT_DFU_FILE_HEADER_SIZE];
      memcpy(&fileInfo, (uint32_t *)(USBD_DFU_APP_DEFAULT_ADD), sizeof(uint32_t)*BOOT_DFU_FILE_HEADER_SIZE);
      btea((uint32_t *)&fileInfo, (-1)*(BOOT_DFU_FILE_HEADER_SIZE), ENCRYPT_KeyWord);
      BOOT_DfuFileInfoType *pFileInfo = (BOOT_DfuFileInfoType *)fileInfo;
      if(pFileInfo->bootVer == SYS_BOOT_VERSION)
      {
        uint32_t xAddrDest = SYS_APP_RUN_ADDR;
        uint32_t xAddrSrc  = USBD_DFU_APP_DEFAULT_ADD + sizeof(uint32_t)*BOOT_DFU_FILE_HEADER_SIZE;
        uint32_t xDataSize = sizeof(uint32_t)*pFileInfo->xDataSize;
        if(BootLoader_FirmwareUpgrade(xAddrDest, xAddrSrc, xDataSize) == BOOTLOADER_OK)
        {
          /* Change Boot Flag */
          hStorageMsgData.xBootFlag = 0xffffffff;
          Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
        }
      }
    }
    
    /* Check Boot Flag On Storage Msg */
    /* DownLoad Firmware If Need */
    if(hStorageMsgData.xBootFlag == STORAGE_BOOT_FLAG)
    {
      /* Otherwise enters DFU mode to allow user programing his application */
      /* Init Device Library */
      USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
      /* Add Supported Class */
      USBD_RegisterClass(&hUsbDeviceFS, &USBD_DFU);
      /* Add DFU Media interface */
      USBD_DFU_RegisterMedia(&hUsbDeviceFS, &USBD_DFU_fops_FS);
      /* Start Device Process */
      USBD_Start(&hUsbDeviceFS);
      /* Enable USB Connect */
      BSP_USBEN_On();
      
      while(1)
      {
        HAL_Delay(1950);
        BSP_SYSLED_On();
        HAL_Delay(25);
        BSP_SYSLED_Off();
        HAL_Delay(25);
      }
    }
    
    /* Test application code at SYS_APP_RUN_ADDR */
    if(((*(__IO uint32_t*)SYS_APP_RUN_ADDR) & 0x2FFE0000 ) == 0x20000000)
    {
      /* application address */
      pFunction JumpToApplication;
      uint32_t JumpAddress;
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (SYS_APP_RUN_ADDR + 4);
      JumpToApplication = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) SYS_APP_RUN_ADDR);
      JumpToApplication();
      /* Load Application Failed */
      while(1);
    }
    
    /* No Load Application */
    while(1)
    {
      HAL_Delay(1700);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
      BSP_SYSLED_On();
      HAL_Delay(50);
      BSP_SYSLED_Off();
      HAL_Delay(50);
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
  * @brief  Upgrade Firmware
  * @param  None
  * @retval None
  */
BootLoader_StatusTypeDef BootLoader_FirmwareUpgrade(uint32_t xAddrDest, uint32_t xAddrSrc, uint32_t xDataSize)
{
  /* Check Param */  
  if((xAddrDest%sizeof(uint32_t) != 0)||(xAddrSrc%sizeof(uint32_t) != 0)||(xDataSize%sizeof(uint32_t) != 0))
  {
    return (BOOTLOADER_ERROR);
  }
  
  BootLoader_StatusTypeDef status = BOOTLOADER_OK;
  
  /* Unlock Flash Begin */
  HAL_FLASH_Unlock();
  
  /* Decrypt Firmware And Program */
  uint32_t pBuf[FLASH_PAGE_SIZE/sizeof(uint32_t)];
  uint32_t xMsg[ENCRYPT_ARRAY_SIZE];
  uint32_t iDataLen = 0;
  uint32_t iDataBlock = 0;
  while((status == BOOTLOADER_OK)&&(iDataLen < xDataSize))
  {
    /* datasize to program */
    if(iDataLen + FLASH_PAGE_SIZE < xDataSize)  { iDataBlock = FLASH_PAGE_SIZE; }
    else                                        { iDataBlock = xDataSize - iDataLen; }
    
    /* Copy to RAM */
    memcpy(pBuf, (void*)(xAddrSrc + iDataLen), iDataBlock);
    
    /* Erase Select Flash */
    uint32_t SectorError;
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = xAddrDest + iDataLen;
    EraseInitStruct.NbPages = 1;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
      status = BOOTLOADER_ERROR;
      break;
    }
    
    /* Decrypt, and Program to Flash */
    for(uint32_t i=0; ((status == BOOTLOADER_OK)&&(i<(iDataBlock/sizeof(uint32_t)/ENCRYPT_ARRAY_SIZE))); i++)
    {
      memcpy(xMsg, (void *)(pBuf+(i*ENCRYPT_ARRAY_SIZE)), sizeof(uint32_t)*ENCRYPT_ARRAY_SIZE);
      btea(xMsg, (-1*ENCRYPT_ARRAY_SIZE), ENCRYPT_KeyWord);
      for(uint32_t j=0; ((status == BOOTLOADER_OK)&&(j<(ENCRYPT_ARRAY_SIZE))); j++)
      {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (xAddrDest + iDataLen + sizeof(uint32_t)*(i*ENCRYPT_ARRAY_SIZE+j)), xMsg[j]) != HAL_OK)
        {
          status = BOOTLOADER_ERROR;
          break;
        }
      }
    }
    
    /* For Next */
    iDataLen += iDataBlock;
  }
  
  /* Lock Flash End */
  HAL_FLASH_Lock();
  
  return (status);
}


/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
