/**
 ******************************************************************************
 * @file    cart_time.c
 * @author  Bellino Francesco
 * @brief   CART module test source file:
 * 				- estimate timing of cart_init().
 * 				- estimate timing of cart_read() (order sequence)
 * 				- estimate timing of cart_read() (random sequence)
 *
 ******************************************************************************
 */

#include "cart.h"
#include "cart_time.h"
#include "time_util.h"


/**
 * @brief	Estimate time of cart_init
 * @param 	None
 * @retval 	None
 */
void cart_init_timing_test()
{
	timing_var time_var;

	time_var.mean_time = 0;

	for (int i=0; i<CART_INIT_TIMING_TEST_RETRIES; i++)
	{
		float time;

		start_time();
		cart_init();	/*Call the tested function*/
		time = estimate_time_oneshot_us();

		manage_time_variables(time, &time_var, i, CART_INIT_TIMING_TEST_RETRIES);

		cart_close();	/*Close module (needed because in cart_init there is a f_open)*/
	}

	print_timing_data_us("cart_init", time_var);
}


/**
 * @brief	Estimate time of cart_read with addresses in order
 * @param  	None
 * @retval 	None
 */
void cart_read_order_timing_test()
{
	timing_var time_var;

	time_var.mean_time = 0;
	cart_init();	/*Initialize module*/

	for (int i=0; i<CART_READ_ORDER_TIMING_TEST_RETRIES; i++)
	{
		float time;

		start_time();
		cart_read(i);	/*Call the tested function*/
		time = estimate_time_oneshot_us();

		manage_time_variables(time, &time_var, i, CART_INIT_TIMING_TEST_RETRIES);
	}

	print_timing_data_us("cart_read (order)", time_var);
	cart_close();	/*Close module*/
}


/**
 * @brief	Estimate time of cart_init
 * @param  	None
 * @retval 	None
 */
void cart_read_random_timing_test()
{
	timing_var time_var;

	time_var.mean_time = 0;
	cart_init();	/*Initialize module*/

	for (int i=0; i<CART_READ_ORDER_TIMING_TEST_RETRIES; i++)
	{
		float time;

		start_time();
		cart_read(i*10);	/*Call the tested function*/
		time = estimate_time_oneshot_us();

		manage_time_variables(time, &time_var, i, CART_INIT_TIMING_TEST_RETRIES);
	}

	print_timing_data_us("cart_read (random)", time_var);
	cart_close();	/*Close module*/
}
