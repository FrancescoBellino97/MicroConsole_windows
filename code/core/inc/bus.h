/**
 ******************************************************************************
 * @file    bus.h
 * @author  Bellino Francesco
 * @brief   Bus module header file.
 *
 ******************************************************************************
 */

#ifndef CORE_INC_BUS_H_
#define CORE_INC_BUS_H_


#include <stdint.h>


#define ADDRESS_END_ROM		0x7FFF


uint8_t bus_read(uint16_t address);
void bus_write(uint16_t address, uint8_t value);


#endif /* CORE_INC_BUS_H_ */
