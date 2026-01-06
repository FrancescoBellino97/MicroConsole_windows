/**
 ******************************************************************************
 * @file    time_main.c
 * @author  Bellino Francesco
 * @brief   Timing test main application source code:
 * 				- run time estimation tests on CART module.
 *
 ******************************************************************************
 */

#include <stdio.h>

#include "cart_time.h"


/**
  * @brief	Runs timing tests for modules
  * @param  None
  * @retval None
  */
int main(void)
{
	/* TIMING TESTS */
	/* CART module */
	cart_init_timing_test();
	cart_read_order_timing_test();
	cart_read_random_timing_test();

	return 0;
}
