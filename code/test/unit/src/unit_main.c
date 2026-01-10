/**
 ******************************************************************************
 * @file    unit_main.c
 * @author  Bellino Francesco
 * @brief   Unit test main application source code:
 * 				- run unit tests on CPU module.
 *
 ******************************************************************************
 */

#include <stdio.h>

#include "unity.h"
#include "cpu_unit.h"


/**
  * @brief	Runs unit tests for modules
  * @param  None
  * @retval None
  */
int main(void)
{
	/* UNIT TESTS */
	UNITY_BEGIN();	/*Start Unit Tests Framework*/

	/* CPU module */
	RUN_TEST(cpu_ADD_U8_U8_BIT_unit_test);
	RUN_TEST(cpu_ADC_U8_U8_BIT_unit_test);
	RUN_TEST(cpu_ADD_U16_S8_BIT_unit_test);
	RUN_TEST(cpu_ADD_U16_U16_BIT_unit_test);
	RUN_TEST(cpu_SUB_U8_U8_BIT_unit_test);
	RUN_TEST(cpu_SBC_U8_U8_BIT_unit_test);
	RUN_TEST(cpu_OR_U8_U8_BIT_unit_test);

	return UNITY_END();		/*End Unit Tests Framework*/
}


/**
  * @brief	Run before each test
  * @param  None
  * @retval None
  */
void setUp(void)
{

}


/**
  * @brief	Run after each test
  * @param  None
  * @retval None
  */
void tearDown()
{

}
