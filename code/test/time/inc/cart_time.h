/**
 ******************************************************************************
 * @file    cart_time.h
 * @author  Bellino Francesco
 * @brief   CART module test header file.
 *
 ******************************************************************************
 */

#ifndef TEST_TIME_INC_CART_TIME_H_
#define TEST_TIME_INC_CART_TIME_H_


#define CART_INIT_TIMING_TEST_RETRIES			1000
#define CART_READ_ORDER_TIMING_TEST_RETRIES		1000
#define CART_READ_RANDOM_TIMING_TEST_RETRIES	1000


void cart_init_timing_test();
void cart_read_order_timing_test();
void cart_read_random_timing_test();


#endif /* TEST_TIME_INC_CART_TIME_H_ */
