/*
 * Dev_Inf.c
 *
 *  Created on: 15 abr. 2023
 *      Author: Pepe
 */


/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo __attribute__((section(".Dev_info"))) /*const*/ StorageInfo  = {
#endif
	"W25Q32_STM32F411CE_BlackPill", 	 	         // Device Name + version number
	SPI_FLASH,                  		 // Device Type
	0x00000000,                			 // Device Start Address
	0x00400000,		                 	 // Device Size in Bytes
	0x100,				                 // Programming Page Size
	0xFF,                                // Initial Content of Erased Memory

	// Specify Size and Address of Sectors (view example below)
	{{0x00000400, 0x00001000}, 			// Sector Numbers, Sector Size
	{0x00000000, 0x00000000}}
};
