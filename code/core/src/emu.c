/**
 ******************************************************************************
 * @file    emu.c
 * @author  Bellino Francesco
 * @brief   Emulation module source code: m
 * 				- manage cart sub-module
 * 				- manage cpu sub-module
 * 				- manage ppu sub_module
 * 				- manage apu sub-module.
 *
 ******************************************************************************
 */

#include "emu.h"
#include "common.h"
#include "cart.h"
#include "cpu.h"


/**
  * @brief	It initialize cart module and return error code
  * @param  None
  * @retval uint8_t:	error code (see common.h)
  */
uint8_t emu_init()
{
	uint8_t ret_val = NO_ERROR;

	ret_val = cart_init();

	if (ret_val == NO_ERROR)
	{
		cpu_init();
	}

	return ret_val;
}

/**
  * @brief	It runs a cycle of Emulation module
  * @param  None
  * @retval None
  */
void emu_run()
{
	return;
}
