/**
 ******************************************************************************
 * @file    main.c
 * @author  Bellino Francesco
 * @brief   Main application source code.
 *
 ******************************************************************************
 */

/* STANDARD */
#include "../../core/inc/main.h"

#include <stdint.h>

#include "../../core/inc/cfg.h"
/* APP */
#include "micro.h"


/**
  * @brief  Main function
  * 		Initialize the micro and blink the LED
  * @param  None
  * @retval None
  */
int main(void)
{
	micro_init();

	while(1)
	{
		/* For loop to wait a delay */
		for (uint32_t i = 0; i < CLOCK_FREQ*PERIOD; i++);
		micro_blink();
	}

	return 0;
}
