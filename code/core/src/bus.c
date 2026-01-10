/**
 ******************************************************************************
 * @file    bus.c
 * @author  Bellino Francesco
 * @brief   Bus module source code:
 * 				- manage the communication between modules and memory.
 *
 ******************************************************************************
 */

#include "bus.h"
#include "cart.h"


/**
  * @brief	Read a data at specific address
  * @param  uint16_t:	address to access
  * @retval uint8_t:	data read
  */
uint8_t bus_read(uint16_t address)
{
	uint8_t read_val;
	if (address <= ADDRESS_END_ROM)
	{
		read_val = cart_read(address);
	}
	else
	{	/*Address not managed*/
		read_val = 0xFF;
	}

	return read_val;
}


/**
  * @brief	Write a data at specific address
  * @param  uint16_t:	address to access
  * 		uint8_t:	value to write
  * @retval None
  */
void bus_write(uint16_t address, uint8_t value)
{
	(void)address;
	(void)value;
	/*To be implemented...*/
}
