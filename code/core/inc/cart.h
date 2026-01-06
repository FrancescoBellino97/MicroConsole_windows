/**
 ******************************************************************************
 * @file    cart.h
 * @author  Bellino Francesco
 * @brief   Cartridge module header file.
 *
 ******************************************************************************
 */

#ifndef CORE_INC_CART_H_
#define CORE_INC_CART_H_


#include <stdio.h>
#include <stdint.h>


/*				----------------------------
 * 				| ROM DATA MAPPING         |
 * 				----------------------------
 * 		0x100	----------------------------
 * 				| ENTRY POINT              |
 * 		0x104	----------------------------
 * 				| NINTENDO LOGO            |
 * 		0x134	----------------------------
 * 				| TITLE                    |
 * 		0x144	----------------------------
 * 				| NEW LICENSEE CODE        |
 * 		0x146	----------------------------
 * 				| SGB FLAG                 |
 * 		0x147	----------------------------
 * 				| CARTRIDGE TYPE           |
 * 		0x148	----------------------------
 * 				| ROM SIZE                 |
 *		0x149	----------------------------
 * 				| RAM SIZE                 |
 *		0x14A	----------------------------
 * 				| DESTINATION CODE         |
 *		0x14B	----------------------------
 * 				| OLD LICENSEE CODE        |
 *		0x14C	----------------------------
 * 				| MASK ROM VERSION NUMBER  |
 *		0x14D	----------------------------
 * 				| HEADER CHECKSUM          |
 *		0x14E	----------------------------
 * 				| GLOBAL CHECKSUM          |
 *		0X14F	----------------------------
 *
 */


#define CART_LOCATION	"../rom/rom.gb"

/* ROM SECTION ADDRESS OFFSETS */
#define ADDRESS_ROM_START						(0x0000U)
#define ADDRESS_ENTRY_POINT_OFFSET				(0x0100U)
#define ADDRESS_NINTENDO_LOGO_OFFSET			(0x0104U)
#define ADDRESS_TITLE_OFFSET					(0x0134U)
#define ADDRESS_NEW_LICENSEE_CODE_OFFSET		(0x0144U)
#define ADDRESS_SGB_FLAG_OFFSET					(0x0146U)
#define ADDRESS_CARTRIDGE_TYPE_OFFSET			(0x0147U)
#define ADDRESS_ROM_SIZE_OFFSET					(0x0148U)
#define ADDRESS_RAM_SIZE_OFFSET					(0x0149U)
#define ADDRESS_DESTINATION_CODE_OFFSET			(0x014AU)
#define ADDRESS_OLD_LICENSEE_CODE_OFFSET		(0x014BU)
#define ADDRESS_MASK_ROM_VERSION_NUMBER_OFFSET	(0x014CU)
#define ADDRESS_HEADER_CHECKSUM_OFFSET			(0x014DU)
#define ADDRESS_GLOBAL_CHECKSUM_OFFSET			(0x014EU)
#define ADDRESS_CARTRIDGE_HEADER_END			(0x0150U)

/* ROM SECTION SIZES */
#define ADDRESS_ENTRY_POINT_SIZE				(ADDRESS_NINTENDO_LOGO_OFFSET - ADDRESS_ENTRY_POINT_OFFSET)
#define ADDRESS_NINTENDO_LOGO_SIZE				(ADDRESS_TITLE_OFFSET - ADDRESS_NINTENDO_LOGO_OFFSET)
#define ADDRESS_TITLE_SIZE						(ADDRESS_NEW_LICENSEE_CODE_OFFSET - ADDRESS_TITLE_OFFSET)
#define ADDRESS_NEW_LICENSEE_CODE_SIZE			(ADDRESS_SGB_FLAG_OFFSET - ADDRESS_NEW_LICENSEE_CODE_OFFSET)
#define ADDRESS_SGB_FLAG_SIZE					(ADDRESS_CARTRIDGE_TYPE_OFFSET - ADDRESS_SGB_FLAG_OFFSET)
#define ADDRESS_CARTRIDGE_TYPE_SIZE				(ADDRESS_ROM_SIZE_OFFSET - ADDRESS_CARTRIDGE_TYPE_OFFSET)
#define ADDRESS_ROM_SIZE_SIZE					(ADDRESS_RAM_SIZE_OFFSET - ADDRESS_ROM_SIZE_OFFSET)
#define ADDRESS_RAM_SIZE_SIZE					(ADDRESS_DESTINATION_CODE_OFFSET - ADDRESS_RAM_SIZE_OFFSET)
#define ADDRESS_DESTINATION_CODE_SIZE			(ADDRESS_OLD_LICENSEE_CODE_OFFSET - ADDRESS_DESTINATION_CODE_OFFSET)
#define ADDRESS_OLD_LICENSEE_CODE_SIZE			(ADDRESS_MASK_ROM_VERSION_NUMBER_OFFSET - ADDRESS_OLD_LICENSEE_CODE_OFFSET)
#define ADDRESS_MASK_ROM_VERSION_NUMBER_SIZE	(ADDRESS_HEADER_CHECKSUM_OFFSET - ADDRESS_MASK_ROM_VERSION_NUMBER_OFFSET)
#define ADDRESS_HEADER_CHECKSUM_SIZE			(ADDRESS_GLOBAL_CHECKSUM_OFFSET - ADDRESS_HEADER_CHECKSUM_OFFSET)
#define ADDRESS_GLOBAL_CHECKSUM_SIZE			(ADDRESS_CARTRIDGE_HEADER_END - ADDRESS_GLOBAL_CHECKSUM_OFFSET)


typedef struct
{
	FILE *memory_pointer;		/*Pointer to ROM memory*/
	uint16_t last_address;		/*Last accessed address*/
}cart_context;


#ifdef UNIT_TEST
extern cart_context cart_ctx;
#endif


uint8_t cart_init();
uint8_t cart_read(uint16_t address);
void cart_close();


#endif /* CORE_INC_CART_H_ */
