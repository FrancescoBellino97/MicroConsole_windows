/**
 ******************************************************************************
 * @file    main.c
 * @author  Bellino Francesco
 * @brief   Main application source code:
 * 				- manage emulation module.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"
#include "emu.h"


/**
  * @brief	This function initialize and run emulation module.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t ret_val = NO_ERROR;

	ret_val = emu_init();		/*Initialize emulation module*/

	if (ret_val == NO_ERROR)
	{
		emu_run();		/*Run emulation module*/
	}
	else
	{
		fprintf(stderr, "An error occurred. Error code: %d\n", ret_val);
	}

	return 0;
}
