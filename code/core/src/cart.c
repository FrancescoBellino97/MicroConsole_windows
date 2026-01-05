/**
 ******************************************************************************
 * @file    cart.c
 * @author  Bellino Francesco
 * @brief   Cartridge module source code:
 * 				- perform check on ROM data.
 * 				- manage the communication with the ROM data.
 *
 ******************************************************************************
 */

#include "cart.h"
#include "common.h"


static cart_context cart_ctx;


/**
  * @brief	Access ROM and perform initialization and checks
  * @param  None
  * @retval uint8_t:	error code (see common.h)
  */
uint8_t cart_init()
{
	uint8_t ret_val = NO_ERROR;
    uint8_t checksum = 0U;
    uint8_t rom_checksum;

    /* Initialize context variables */
    cart_ctx.last_address = 0U;
    cart_ctx.memory_pointer = fopen(CART_LOCATION, "r");


    /* Check if ROM is found */
    if (cart_ctx.memory_pointer == NULL)
    {
    	ret_val = CART_NOT_FOUND;
    }


    uint8_t nintendo_logo_reference[ADDRESS_NINTENDO_LOGO_SIZE] = {
    		0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
    };	/*Nintendo logo reference from documentation*/

    /* Nintendo logo check */
    for (uint16_t index = 0U;
    		(index < ADDRESS_NINTENDO_LOGO_SIZE) && (ret_val == NO_ERROR) ;
    		index++)
    {
    	uint8_t rom_data;

    	rom_data = cart_read(ADDRESS_ROM_START + ADDRESS_NINTENDO_LOGO_OFFSET + index);

    	if (rom_data != nintendo_logo_reference[index])
    	{
    		ret_val = CART_NINTENDO_LOGO_FAILED;
    	}
    }


    /* Header section checksum calculation*/
    if (ret_val == NO_ERROR)
    {
    	/* Perform checksum on ROM data starting from TITLE to MASK_VERSION_NUMER addresses */
    	for (uint16_t address = ADDRESS_ROM_START + ADDRESS_TITLE_OFFSET;
    			address <= ADDRESS_ROM_START + ADDRESS_MASK_ROM_VERSION_NUMBER_OFFSET;
    			address++)
    	{
    		uint8_t rom_data;

    		rom_data = cart_read(address);
    		checksum = checksum - rom_data - 1;	/*From documentation*/
    	}

    	/* Read checksum in ROM */
    	rom_checksum = cart_read(ADDRESS_ROM_START + ADDRESS_HEADER_CHECKSUM_OFFSET);

    	if (checksum != rom_checksum)
    	{
    		return CART_CHECKSUM_FAILED;
    	}
    }

    return ret_val;
}


/**
  * @brief	Read a Byte from ROM at specific address
  * @param  None
  * @retval uint8_t:	value read
  */
uint8_t cart_read(uint16_t address)
{
	uint8_t value_read;

	/* Check if the new address is next to previous one */
	if (address != cart_ctx.last_address+1)
	{
		fseek(cart_ctx.memory_pointer, address, SEEK_SET);	/*Change file pointer*/
	}

	/* Read value from ROM */
	value_read = fgetc(cart_ctx.memory_pointer);
	cart_ctx.last_address = address;

	return value_read;
}
