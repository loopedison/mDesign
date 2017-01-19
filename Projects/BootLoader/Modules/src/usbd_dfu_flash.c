/**
  ******************************************************************************
  * @file    usbd_dfu_flash.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   USB DFU Flash Function
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_flash.h"
#include "storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/32*001Ka,32*001Kg"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USBD_HandleTypeDef hUsbDeviceFS;
/* Private function prototypes -----------------------------------------------*/
static uint16_t MEM_If_Init_FS(void);
static uint16_t MEM_If_Erase_FS (uint32_t Add);
static uint16_t MEM_If_Write_FS (uint8_t *src, uint8_t *dest, uint32_t Len);
static uint8_t *MEM_If_Read_FS  (uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_DeInit_FS(void);
static uint16_t MEM_If_GetStatus_FS (uint32_t Add, uint8_t Cmd, uint8_t *buffer);
static uint16_t MEM_If_Complete (void);

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_FS __ALIGN_END =
{
  (uint8_t*)FLASH_DESC_STR,
  MEM_If_Init_FS,
  MEM_If_DeInit_FS,
  MEM_If_Erase_FS,
  MEM_If_Write_FS,
  MEM_If_Read_FS,
  MEM_If_GetStatus_FS,   
  MEM_If_Complete,
};

/* Private functions ---------------------------------------------------------*/
//==============================================================================
/**
  * @brief  MEM_If_Init_FS
  *         Memory initialization routine.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Init_FS(void)
{
  HAL_FLASH_Unlock();
  return (USBD_OK);
}

//==============================================================================
/**
  * @brief  MEM_If_DeInit_FS
  *         De-Initializes Memory.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_DeInit_FS(void)
{
  HAL_FLASH_Lock();
  return (USBD_OK);
}

//==============================================================================
/**
  * @brief  MEM_If_Erase_FS
  *         Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Erase_FS(uint32_t Add)
{
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  uint32_t sectornb = 0;
  FLASH_EraseInitTypeDef eraseinitstruct;
  /* Get the number of sector */
  eraseinitstruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = Add;
  eraseinitstruct.NbPages     = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &sectornb);
  if (status != HAL_OK)
  {
    return USBD_FAIL;
  }
  
  return (USBD_OK);
}

//==============================================================================
/**
  * @brief  MEM_If_Write_FS
  *         Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  for(uint32_t i=0; i<Len; i+=4)
  {
    if((uint32_t)(dest+i) < USBD_DFU_APP_DEFAULT_ADD)
    {
      return 3;
    }
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word */ 
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(dest+i), *(uint32_t*)(src+i)) == HAL_OK)
    {
     /* Check the written value */
      if(*(uint32_t *)(src + i) != *(uint32_t*)(dest+i))
      {
        return 2;
      }
    }
    else
    {
      return 1;
    }
  }
  
  return (USBD_OK);
}

//==============================================================================
/**
  * @brief  MEM_If_Read_FS
  *         Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *MEM_If_Read_FS (uint8_t *src, uint8_t *dest, uint32_t Len)
{
  for(uint32_t i=0; i<Len; i++)
  {
    dest[i] = 0XFF;
  }
  return (uint8_t*)(dest);
}

//==============================================================================
/**
  * @brief  Flash_If_GetStatus_FS
  *         Get status routine.
  * @param  Add: Address to be read from.
  * @param  Cmd: Number of data to be read (in bytes).
  * @param  buffer: used for returning the time necessary for a program or an erase operation
  * @retval 0 if operation is successful
  */
uint16_t MEM_If_GetStatus_FS (uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  switch (Cmd)
  {
  case DFU_MEDIA_PROGRAM:
    break;
  case DFU_MEDIA_ERASE:
  default:
    break;
  }                             
  return  (USBD_OK);
}

//==============================================================================
/**
  * @brief  MEM_If_Complete
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Complete(void)
{
  hStorageMsgData.xBootFlag = STORAGE_BOOT_COMPLETE;
  Storage_WriteData(STORAGE_HANDLE, &hStorageMsgData);
  return  (USBD_OK);
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
