/**
 ******************************************************************************
 * @file    time_util.c
 * @author  Bellino Francesco
 * @brief   Time utility source code:
 * 				- ???.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "time_util.h"


#define US_CONVERSION_FACTOR	1000


struct timespec startTime;


/**
  * @brief	Save in start time structure the current time
  * @param  None
  * @retval None
  */
void start_time()
{
	clock_gettime(CLOCK_MONOTONIC, &startTime);
}


/**
  * @brief	Estimate time between start time and current time
  * @param  None
  * @retval float:	delta time in us
  */
float estimate_time_oneshot_us()
{
	struct timespec endTime;
	clock_gettime(CLOCK_MONOTONIC, &endTime);
	long delta = endTime.tv_nsec-startTime.tv_nsec;

	if (delta < 0)
	{	/*Compensate underflow*/
		delta += LONG_MAX;
	}

	return (float)(delta)/US_CONVERSION_FACTOR;
}


/**
  * @brief	Update timing_var data
  * @param  current_time:	current time value
  * 		time_var:		structure with max, min and mean timing values
  * 		current_retry:	current retry of test (to initialize min and max time values)
  * 		number_retry:	total number of retry (to compute mean_value)
  * @retval None
  */
void manage_time_variables(float current_time, timing_var *time_var, int current_retry, int number_retry)
{
	if (current_retry==0)
	{
		time_var->max_time = current_time;
		time_var->min_time = current_time;
	}
	else
	{
		if (current_time > time_var->max_time)
		{
			time_var->max_time = current_time;
		}
		if (current_time < time_var->min_time)
		{
			time_var->min_time = current_time;
		}
	}
	time_var->mean_time += current_time/number_retry;
}


/**
  * @brief	Print timing data formatted
  * @param  function_name:	name of the function to print
  * 		time_var:		timing structure with max, min and mean time value
  * @retval None
  */
void print_timing_data_us(char *function_name, timing_var time_var)
{
	printf("%s timing test\n"
			"\tMean time = %.1fus\n"
			"\tMax time  = %.1fus\n"
			"\tMin time  = %.1fus\n\n",
			function_name, time_var.mean_time, time_var.max_time, time_var.min_time);
}
