/*
 * w25q32.c
 *
 *  Created on: 7 ene. 2023
 *      Author: Pepe
 */
#include "gpio.h"
#include "w25q32.h"

uint8_t FlashCheckBusy(void);

#define CS_L HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET);
#define CS_H HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET);

extern SPI_HandleTypeDef hspi1;
uint8_t txBuff[8];
uint8_t rxBuff[8];

uint8_t FlashCheckBusy(void)
{
	CS_L;
	txBuff[0] = FLASH_READ_STATUS_1;
	HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 2, 100);
	CS_H;

	return rxBuff[1] & 0x01;
}

uint8_t FlashWriteEnable(void)
{
	CS_L;
	txBuff[0] = FLASH_WRITE_ENABLE;
	HAL_SPI_Transmit(&hspi1, txBuff, 1, 100);
	CS_H;

	CS_L
	txBuff[0] = FLASH_READ_STATUS_1;
	HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 2, 100);
	CS_H;

	if(rxBuff[1] & 0x02)
	{
		return FLASH_ORDER_OK;
	}
	else
	{
		return FLASH_ORDER_FAIL;
	}
}

uint8_t FlashWriteDisable(void)
{
	CS_L;
	txBuff[0] = FLASH_WRITE_DISABLE;
	HAL_SPI_Transmit(&hspi1, txBuff, 1, 100);
	CS_H;

	CS_L;
	txBuff[0] = FLASH_READ_STATUS_1;
	HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 2, 100);
	CS_H;

	if(rxBuff[1] & 0x02)
	{
		return FLASH_ORDER_FAIL;
	}
	else
	{
		return FLASH_ORDER_OK;
	}
}

uint8_t FlashJedecID(uint8_t *buffer)
{

	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	CS_L;
	txBuff[0] = FLASH_JEDEC_ID;
	HAL_SPI_TransmitReceive(&hspi1, &txBuff[0], buffer, 4, 100);
	CS_H;
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

uint8_t FlashRead (uint32_t address, uint16_t size, uint8_t* buffer)
{
	uint8_t txBuff[8] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5};

	CS_L;
	txBuff[0] = FLASH_READ;
	txBuff[3] = (uint8_t)(address & 0x000000FF);
	txBuff[2] = (uint8_t)((address >> 8) & 0x000000FF);
	txBuff[1] = (uint8_t)((address >> 16) & 0x000000FF);
	HAL_SPI_Transmit(&hspi1, &txBuff[0], 4, 100);
	HAL_SPI_Receive(&hspi1, buffer, size, 500);
	CS_H;
	return 0;
}

uint8_t FlashWrite(uint32_t address, uint16_t size, uint8_t *buffer)
{
	if(!FlashCheckBusy())
	{
		if(FlashWriteEnable())
		{
			return FLASH_ORDER_FAIL;
		}
		else
		{
			CS_L;
			txBuff[0] = FLASH_PAGE_PROGRAM;
			txBuff[3] = (uint8_t)(address & 0x000000FF);
			txBuff[2] = (uint8_t)((address >> 8) & 0x000000FF);
			txBuff[1] = (uint8_t)((address >> 16) & 0x000000FF);
			HAL_SPI_Transmit(&hspi1, txBuff, 4, 100);
			HAL_SPI_Transmit(&hspi1, buffer, size, 500);
			CS_H;
		}
		return FLASH_ORDER_OK;
	}
	return FLASH_ORDER_FAIL;
}

/*
 * Erase a sector of Flash (4Kb, 16 pages)
 *
 * Param
 * sector:	the 24bits start address of the sector 0 = 0x00, 1 = 0x1000, 2 = 0x2000...
 */
uint8_t FlashSectorErase(uint32_t sector)
{
	if(!FlashCheckBusy())
	{
		if(FlashWriteEnable())
		{
			return FLASH_ORDER_FAIL;
		}
		else
		{
			CS_L;
			txBuff[0] = FLASH_SECTOR_ERASE;
			txBuff[3] = (uint8_t)(sector & 0x000000FF);
			txBuff[2] = (uint8_t)((sector >> 8) & 0x000000FF);
			txBuff[1] = (uint8_t)((sector >> 16) & 0x000000FF);
			HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 4, 100);
			CS_H;
		}
		return FLASH_ORDER_OK;
	}
	return FLASH_ORDER_FAIL;
}

/*
 * Erase a block of Flash (32Kb, 128 pages)
 *
 * Param
 * sector:	the 24bits start address of the sector 0 = 0x00, 1 = 0x1000, 2 = 0x2000...
 */
uint8_t FlashBlockErase32K(uint32_t sector)
{
	if(!FlashCheckBusy())
	{
		if(FlashWriteEnable())
		{
			return FLASH_ORDER_FAIL;
		}
		else
		{
			CS_L;
			txBuff[0] = FLASH_BLOCK_ERASE_32;
			txBuff[3] = (uint8_t)(sector & 0x000000FF);
			txBuff[2] = (uint8_t)((sector >> 8) & 0x000000FF);
			txBuff[1] = (uint8_t)((sector >> 16) & 0x000000FF);
			HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 4, 100);
			CS_H;
		}
		return FLASH_ORDER_OK;
	}
	return FLASH_ORDER_FAIL;
}

/*
 * Erase a block of Flash (64Kb, 256 pages)
 *
 * Param
 * sector:	the 24bits start address of the sector 0 = 0x00, 1 = 0x1000, 2 = 0x2000...
 */
uint8_t FlashBlockErase64K(uint32_t sector)
{
	if(!FlashCheckBusy())
	{
		if(FlashWriteEnable())
		{
			return FLASH_ORDER_FAIL;
		}
		else
		{
			CS_L;
			txBuff[0] = FLASH_BLOCK_ERASE_64;
			txBuff[3] = (uint8_t)(sector & 0x000000FF);
			txBuff[2] = (uint8_t)((sector >> 8) & 0x000000FF);
			txBuff[1] = (uint8_t)((sector >> 16) & 0x000000FF);
			HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 4, 100);
			CS_H;
		}
		return FLASH_ORDER_OK;
	}
	return FLASH_ORDER_FAIL;
}

uint8_t FlashChipErase(void)
{
	if(!FlashCheckBusy())
	{
		if(FlashWriteEnable())
		{
			return FLASH_ORDER_FAIL;
		}
		else
		{
			CS_L;
			txBuff[0] = FLASH_CHIP_ERASE;
			HAL_SPI_Transmit(&hspi1, txBuff, 1, 100);
			CS_H;
		}
		return FLASH_ORDER_OK;
	}
	return FLASH_ORDER_FAIL;
}
