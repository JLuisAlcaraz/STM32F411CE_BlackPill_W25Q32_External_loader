/*
 * Loader_Src.h
 *
 *  Created on: Apr 15, 2023
 *      Author: Pepe
 */

#ifndef INC_LOADER_SRC_H_
#define INC_LOADER_SRC_H_


int Write(uint32_t address, uint16_t size, uint8_t *buffer);
int Read (uint32_t address, uint16_t size, uint8_t* buffer);
int SectorErase(uint32_t eraseStartAddress, uint32_t eraseEndAddress);
int MassErase(void);

#endif /* INC_LOADER_SRC_H_ */
