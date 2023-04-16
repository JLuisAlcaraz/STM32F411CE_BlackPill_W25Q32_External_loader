/*
 * Loader_Src.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Pepe
 */
#include "main.h"
#include "gpio.h"
#include "Loader_Src.h"

extern void SystemClock_Config(void);
extern void MX_SPI1_Init(void);
extern void MX_GPIO_Init(void);

int Init(void);
int CheckBusy(void);
int WriteEnable(void);

#define LOADER_OK	0x01
#define LOADER_FAIL	0x00

#define CS_L HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
#define CS_H HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

extern SPI_HandleTypeDef hspi1;

uint8_t tx_Buff[0x1005];
uint8_t rx_Buff[8];

int Init(void)
{
	SystemInit();
	SCB->VTOR = 0x20000000 | 0x200;
	HAL_DeInit();
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/*Initialize all configured peripherals */
	MX_GPIO_Init();
	CS_H;
	MX_SPI1_Init();

	__set_PRIMASK(0);	//enable interrupts
	HAL_SPI_Init(&hspi1);
	__set_PRIMASK(1);	//disable interrupts

	return LOADER_OK;
}

int Write(uint32_t address, uint16_t size, uint8_t *buffer)
{
	uint32_t addrEnd = address + size;
	uint16_t sizeWrite;

	while(address < addrEnd)
	{
		if(!WriteEnable())
		{
			return LOADER_FAIL;
		}
		else
		{
			sizeWrite = 0x100;
			if(addrEnd - address < 0x100)
			{
				sizeWrite = addrEnd - address;
			}
			CS_L;
			tx_Buff[0] = FLASH_PAGE_PROGRAM;
			tx_Buff[3] = (uint8_t)(address & 0x000000FF);
			tx_Buff[2] = (uint8_t)((address >> 8) & 0x000000FF);
			tx_Buff[1] = (uint8_t)((address >> 16) & 0x000000FF);
			HAL_SPI_Transmit(&hspi1, tx_Buff, 4, 500);
			HAL_SPI_Transmit(&hspi1, buffer, sizeWrite, 500);
			CS_H;
			address += sizeWrite;
			while(CheckBusy())
			{
				;
			}
		}
	}
	return LOADER_OK;
}

int Read (uint32_t address, uint16_t size, uint8_t* buffer)
{
	CS_L;
	tx_Buff[0] = FLASH_READ;
	tx_Buff[3] = (uint8_t)(address & 0x000000FF);
	tx_Buff[2] = (uint8_t)((address >> 8) & 0x000000FF);
	tx_Buff[1] = (uint8_t)((address >> 16) & 0x000000FF);
	HAL_SPI_Transmit(&hspi1, tx_Buff, 4, 100);
	HAL_SPI_Receive(&hspi1, buffer, size, 500);
	CS_H;
	return LOADER_OK;
}

int MassErase(void)
{
	if(!WriteEnable())
	{
		return LOADER_FAIL;
	}
	else
	{
		CS_L;
		tx_Buff[0] = FLASH_CHIP_ERASE;
		HAL_SPI_Transmit(&hspi1, tx_Buff, 1, 100);
		CS_H;
		while(CheckBusy())
		{
			;
		}
	}
	return LOADER_OK;
}

int SectorErase(uint32_t eraseStartAddress, uint32_t eraseEndAddress)
{
	eraseStartAddress = eraseStartAddress - eraseStartAddress % 0x1000;
	//do
	{
		while(CheckBusy())
		{
			;
		}
		if(!WriteEnable())
		{
			return LOADER_FAIL;
		}
		else
		{
			CS_L;
			tx_Buff[0] = FLASH_SECTOR_ERASE;
			tx_Buff[3] = (uint8_t)(eraseStartAddress & 0x000000FF);
			tx_Buff[2] = (uint8_t)((eraseStartAddress >> 8) & 0x000000FF);
			tx_Buff[1] = (uint8_t)((eraseStartAddress >> 16) & 0x000000FF);
			HAL_SPI_Transmit(&hspi1, tx_Buff, 4, 500);
			CS_H;
		}
		eraseStartAddress += 0x1000;
	}
	return LOADER_OK;
}

int CheckBusy(void)
{
	CS_L;
	tx_Buff[0] = FLASH_READ_STATUS_1;
	HAL_SPI_Transmit(&hspi1, tx_Buff, 1, 100);
	HAL_SPI_Receive(&hspi1, rx_Buff, 1, 100);
	CS_H;

	return rx_Buff[0] & 0x01;
}

int WriteEnable(void)
{
	CS_L;
	tx_Buff[0] = FLASH_WRITE_ENABLE;
	HAL_SPI_Transmit(&hspi1, tx_Buff, 1, 100);
	CS_H;

	CS_L
	tx_Buff[0] = FLASH_READ_STATUS_1;
	HAL_SPI_Transmit(&hspi1, tx_Buff, 1, 100);
	HAL_SPI_Receive(&hspi1, rx_Buff, 1, 100);
	CS_H;

	if(rx_Buff[0] & 0x02)
	{
		return LOADER_OK;
	}
	else
	{
		return LOADER_FAIL;
	}
}
