/*
 * Loader_Src.h
 *
 *  Created on: Apr 15, 2023
 *      Author: Pepe
 */

#ifndef INC_LOADER_SRC_H_
#define INC_LOADER_SRC_H_

#define FLASH_WRITE_ENABLE		0x06
#define FLASH_WRITE_DISABLE		0x04
#define FLASH_READ_STATUS_1		0x05
#define FLASH_READ_STATUS_2		0x35
#define FLASH_PAGE_PROGRAM		0x02
#define FLASH_QUAD_PAGE_PROGRAM	0x32
#define FLASH_BLOCK_ERASE_64	0xD8
#define FLASH_BLOCK_ERASE_32	0x52
#define FLASH_SECTOR_ERASE		0x20
#define FLASH_CHIP_ERASE		0x60
#define FLASH_JEDEC_ID			0x9F
#define FLASH_READ				0x03
#define FLASH_FAST_READ			0x0B
#define FLASH_READ_DUAL_OUTPUT	0x3B
#define FLASH_READ_DUAL_IO		0xBB
#define FLASH_READ_QUAD_OUTPUT	0x6B
#define FLASH_READ_QUAD_IO		0xEB
#define FLASH_DUMMY				0xA5

int Write(uint32_t address, uint16_t size, uint8_t *buffer);
int Read (uint32_t address, uint16_t size, uint8_t* buffer);
int SectorErase(uint32_t eraseStartAddress, uint32_t eraseEndAddress);
int MassErase(void);

#endif /* INC_LOADER_SRC_H_ */
