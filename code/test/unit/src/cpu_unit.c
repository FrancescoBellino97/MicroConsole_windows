/**
 ******************************************************************************
 * @file    cpu_test.c
 * @author  Bellino Francesco
 * @brief   CPU module test source file:
 * 				- perform unit test on emulated operations
 *
 ******************************************************************************
 */

#include "cpu.h"
#include "cpu_unit.h"
#include "unity.h"


/** @brief	Unit test of ADC_U8_U8_BIT() in CPU module */
void cpu_ADC_U8_U8_BIT_unit_test()
{
	TEST_MESSAGE("Normal ADC");
	cpu_ctx.registers.c_flag = 0U;
	TEST_ASSERT_EQUAL(0x3, ADC_U8_U8_BIT(0x1, 0x2));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("ADC with carry=1");
	cpu_ctx.registers.c_flag = 1U;
	TEST_ASSERT_EQUAL(0x4, ADC_U8_U8_BIT(0x1, 0x2));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADC with carry");
	cpu_ctx.registers.c_flag = 0U;
	TEST_ASSERT_EQUAL(0x1, ADC_U8_U8_BIT(0xF0, 0x11));
	TEST_ASSERT_EQUAL(0x10, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=1*/

	TEST_MESSAGE("Normal ADC with half-carry");
	cpu_ctx.registers.c_flag = 0U;
	TEST_ASSERT_EQUAL(0x12, ADC_U8_U8_BIT(0x8, 0xA));
	TEST_ASSERT_EQUAL(0x20, cpu_ctx.registers.F);		/*Z=0, N=0, H=1, C=0*/

	TEST_MESSAGE("Normal ADC with zero");
	cpu_ctx.registers.c_flag = 0U;
	TEST_ASSERT_EQUAL(0x0, ADC_U8_U8_BIT(0x0, 0x0));
	TEST_ASSERT_EQUAL(0x80, cpu_ctx.registers.F);		/*Z=1, N=0, H=0, C=0*/
}


/** @brief	Unit test of ADD_U8_U8_BIT() in CPU module */
void cpu_ADD_U8_U8_BIT_unit_test()
{
	TEST_MESSAGE("Normal ADD");
	TEST_ASSERT_EQUAL(0x3, ADD_U8_U8_BIT(0x1, 0x2));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADD with carry");
	TEST_ASSERT_EQUAL(0x1, ADD_U8_U8_BIT(0xF0, 0x11));
	TEST_ASSERT_EQUAL(0x10, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=1*/

	TEST_MESSAGE("Normal ADD with half-carry");
	TEST_ASSERT_EQUAL(0x12, ADD_U8_U8_BIT(0x8, 0xA));
	TEST_ASSERT_EQUAL(0x20, cpu_ctx.registers.F);		/*Z=0, N=0, H=1, C=0*/

	TEST_MESSAGE("Normal ADD with zero");
	TEST_ASSERT_EQUAL(0x0, ADD_U8_U8_BIT(0x0, 0x0));
	TEST_ASSERT_EQUAL(0x80, cpu_ctx.registers.F);		/*Z=1, N=0, H=0, C=0*/
}


/** @brief	Unit test of ADD_U16_S8_BIT() in CPU module */
void cpu_ADD_U16_S8_BIT_unit_test()
{
	TEST_MESSAGE("Normal ADD");
	TEST_ASSERT_EQUAL(0x3, ADD_U16_S8_BIT(0x1, 0x2));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADD with negative number");
	TEST_ASSERT_EQUAL(0x0, ADD_U16_S8_BIT(0x10, 0xF0));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADD with carry (positive)");
	TEST_ASSERT_EQUAL(0x100, ADD_U16_S8_BIT(0xF0, 0x10));
	TEST_ASSERT_EQUAL(0x10, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=1*/

	TEST_MESSAGE("Normal ADD with carry (negative)");
	TEST_ASSERT_EQUAL(0xFFFF, ADD_U16_S8_BIT(0x01, 0xFE));
	TEST_ASSERT_EQUAL(0x10, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=1*/

	TEST_MESSAGE("Normal ADD with zero (not set by this operation)");
	TEST_ASSERT_EQUAL(0x0, ADD_U16_S8_BIT(0x0, 0x0));
	TEST_ASSERT_EQUAL(0x00, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADD with carry (positive)");
	TEST_ASSERT_EQUAL(0x10, ADD_U16_S8_BIT(0xF, 0x01));
	TEST_ASSERT_EQUAL(0x20, cpu_ctx.registers.F);		/*Z=0, N=0, H=1, C=0*/
}

/** @brief	Unit test of ADD_U16_U16_BIT() in CPU module */
void cpu_ADD_U16_U16_BIT_unit_test()
{
	TEST_MESSAGE("Normal ADD");
	TEST_ASSERT_EQUAL(0x3, ADD_U16_U16_BIT(0x1, 0x2));
	TEST_ASSERT_EQUAL(0x0, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=0*/

	TEST_MESSAGE("Normal ADD with carry");
	TEST_ASSERT_EQUAL(0x100, ADD_U16_U16_BIT(0xF000, 0x1100));
	TEST_ASSERT_EQUAL(0x10, cpu_ctx.registers.F);		/*Z=0, N=0, H=0, C=1*/

	TEST_MESSAGE("Normal ADD with half-carry");
	TEST_ASSERT_EQUAL(0x120, ADD_U16_U16_BIT(0x80, 0xA0));
	TEST_ASSERT_EQUAL(0x20, cpu_ctx.registers.F);		/*Z=0, N=0, H=1, C=0*/

	TEST_MESSAGE("Normal ADD with zero");
	TEST_ASSERT_EQUAL(0x0, ADD_U16_U16_BIT(0x0, 0x0));
	TEST_ASSERT_EQUAL(0x80, cpu_ctx.registers.F);		/*Z=1, N=0, H=0, C=0*/
}
