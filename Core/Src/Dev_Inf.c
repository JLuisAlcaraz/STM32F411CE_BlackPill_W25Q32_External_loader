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
		4194304,		                 	 // Device Size in Bytes
		256				,                    // Programming Page Size
		0xFF,                                // Initial Content of Erased Memory

		// Specify Size and Address of Sectors (view example below)
		{
				{ 0x400,  			// Sector Numbers,
				  0x1000 },        	//Sector Size

				{ 0x00000000, 0x00000000 }
		}
};
