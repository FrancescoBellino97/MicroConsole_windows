/**
 ******************************************************************************
 * @file    emu.c
 * @author  Bellino Francesco
 * @brief   Emulation module source code: it manage the sub-modules cart, cpu,
 * 			ppu and apu.
 *
 ******************************************************************************
 */

#include "emu.h"
#include "cart.h"
#include "common.h"


/**
  * @brief	It initialize cart module and return error code
  * @param  None
  * @retval uint8_t:	error code (see common.h)
  */
uint8_t emu_init()
{
	uint8_t ret_val = NO_ERROR;

	ret_val = cart_init();

	return ret_val;
}

/**
  * @brief	TODO
  * @param  None
  * @retval None
  */
void emu_run()
{
	return;
}
