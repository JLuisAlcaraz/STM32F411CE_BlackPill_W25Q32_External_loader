/*
 * Loader_Src.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Pepe
 */
#include <stdio.h>
#include "gpio.h"
#include "w25q32.h"

extern SPI_HandleTypeDef hspi1;

int Init(void)
{
	return 0;
}

int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
	return 0;
}
