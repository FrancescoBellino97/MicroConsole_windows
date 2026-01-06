/**
 ******************************************************************************
 * @file    time_util.h
 * @author  Bellino Francesco
 * @brief   Time utility header file.
 *
 ******************************************************************************
 */

#ifndef TEST_TIME_INC_TIME_UTIL_H_
#define TEST_TIME_INC_TIME_UTIL_H_


typedef struct{
	float max_time;
	float min_time;
	float mean_time;
} timing_var;


void start_time();
float estimate_time_oneshot_us();
void manage_time_variables(float current_time, timing_var *time_var, int current_retry, int number_retry);
void print_timing_data_us(char *function_name, timing_var time_var);


#endif /* TEST_TIME_INC_TIME_UTIL_H_ */
