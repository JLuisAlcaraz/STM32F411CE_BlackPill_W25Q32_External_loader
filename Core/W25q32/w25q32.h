#ifndef _W25QXX_H
#define _W25QXX_H


#define FLASH_CS_PORT	GPIOA
#define FLASH_CS_PIN	GPIO_PIN_4

#define FLASH_ORDER_OK		0
#define FLASH_ORDER_FAIL	1
#define FLASH_BUSY			1
#define FLASH_READY			0

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

uint8_t FlashJedecID(uint8_t *buffer);
uint8_t FlashRead (uint32_t address, uint16_t size, uint8_t* buffer);
uint8_t FlashWrite(uint32_t address, uint16_t size, uint8_t *buffer);
uint8_t FlashSectorErase(uint32_t sector);
uint8_t FlashBlockErase32K(uint32_t sector);
uint8_t FlashBlockErase64K(uint32_t sector);
uint8_t FlashChipErase(void);
uint8_t FlashWriteEnable(void);
uint8_t FlashWriteDisable(void);



#endif
