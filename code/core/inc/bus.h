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


#endif /* CORE_INC_BUS_H_ */
