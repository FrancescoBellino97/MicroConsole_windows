/**
 ******************************************************************************
 * @file    cpu.c
 * @author  Bellino Francesco
 * @brief   CPU module source code:
 * 				- fetch instructions.
 * 				- decode instructions.
 * 				- execute instructions.
 *
 ******************************************************************************
 */

#include "cpu.h"
#include "bus.h"


static void decode(uint8_t op_code);
static void execute();

/*Emulated functions*/
static uint8_t ADC(uint8_t num1, uint8_t num2);

cpu_context cpu_ctx;


/**
  * @brief	Initialize CPU module
  * @param  None
  * @retval None
  */
void cpu_init()
{
	/* The init value of register are taken from documentation */
	cpu_ctx.registers.AF = 0x01B0;
	cpu_ctx.registers.BC = 0x0013;
	cpu_ctx.registers.DE = 0x00D8;
	cpu_ctx.registers.HL = 0x014D;
	cpu_ctx.registers.SP = 0xFFFE;
	cpu_ctx.registers.PC = 0x0100;	/*Skip boot ROM*/
}


/**
  * @brief	Run a cycle of CPU module
  * @param  None
  * @retval None
  */
void cpu_run()
{
	/** From documentation the CPU can perform overlapped execute
	 * and fetch so the execute is performed every cycle while the
	 * fetch is performed only when execute ended.
	 * This is to ensure max 1 memory access per CPU cycle.
	 */

	execute();	/*Execute the current instruction*/

	if (cpu_ctx.variables.fetch_enabled == 1U)
	{	/*Fetch and decode the Op Code read from ROM*/
		uint8_t op_code;
		cpu_ctx.variables.fetch_enabled = 0U;	/*Clear fetch flag*/
		op_code = bus_read(cpu_ctx.registers.PC++);	/*Fetch next instruction from ROM*/
		decode(op_code);	/*Decode instruction*/
	}
}


/**
  * @brief	Decode an OpCode and update context variable
  * @param  uint8_t:	op_code to decode
  * @retval None
  */
static void decode(uint8_t op_code)
{
	switch (op_code)
	{
	case 0x00:	/*NOP*/
		cpu_ctx.instruction.type = TYPE_NOP;
		cpu_ctx.instruction.cycles = 1;
		break;

	case 0x09:	/*ADD HL,BC*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		break;

	case 0x19:	/*ADD HL,DE*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		break;

	case 0x29:	/*ADD HL,HL*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		break;

	case 0x39:	/*ADD HL,SP*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		break;

	case 0x80:	/*ADD A,B*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x81:	/*ADD A,C*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x82:	/*ADD A,D*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x83:	/*ADD A,E*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x84:	/*ADD A,H*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x85:	/*ADD A,L*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;
	case 0x86:	/*ADD A,(HL)*/
		cpu_ctx.instruction.type = TYPE_ADD_A_HL;
		break;
	case 0x87:	/*ADD A,A*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		break;


	case 0x88:	/*ADC A,B*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0x89:	/*ADC A,C*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0x8A:	/*ADC A,D*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0x8B:	/*ADC A,E*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0x8C:	/*ADC A,H*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0x8D:	/*ADC A,L*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0x8E:	/*ADC A,(HL)*/
		cpu_ctx.instruction.type = TYPE_ADC_A_HL;
		cpu_ctx.instruction.cycles = 2;
		cpu_ctx.instruction.data = cpu_ctx.registers.HL;
		break;
	case 0x8F:	/*ADC A,A*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;

	case 0xA0:	/*AND B*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA1:	/*AND C*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA2:	/*AND D*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA3:	/*AND E*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA4:	/*AND H*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA5:	/*AND L*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;
	case 0xA6:	/*AND (HL)*/
		cpu_ctx.instruction.type = TYPE_AND_A_HL;
		break;
	case 0xA7:	/*AND A*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		break;

	case 0xC6:	/*ADD A,u8*/
		cpu_ctx.instruction.type = TYPE_ADD_A_U8;
		break;

	case 0xE6:	/*AND u8*/
		cpu_ctx.instruction.type = TYPE_AND_A_U8;
		break;


	case 0xCE:	/*ADC A,u8*/
		cpu_ctx.instruction.type = TYPE_ADC_A_U8;
		cpu_ctx.instruction.cycles = 2;
		break;

	case 0xE8:	/*ADD SP,s8*/
		cpu_ctx.instruction.type = TYPE_ADD_SP_S8;
		break;

	default:
		cpu_ctx.instruction.type = TYPE_NOP;	/*Default case is NOP*/
		break;
	}
}


/**
  * @brief	Execute the current instruction taking into account cycles needed
  * @param  None
  * @retval None
  */
static void execute()
{
	switch(cpu_ctx.instruction.type)
	{
	case TYPE_NOP:
		break;

	case TYPE_ADC_A_R8:
		cpu_ctx.registers.A = ADC(cpu_ctx.registers.A, cpu_ctx.instruction.data);
		break;

	case TYPE_ADC_A_HL:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle read data at address HL*/
			cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
			break;
		case 1:	/*Second cycle perform ADC*/
			cpu_ctx.registers.A = ADC(cpu_ctx.registers.A, cpu_ctx.instruction.data);
			break;
		}
		break;

	case TYPE_ADC_A_U8:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle read data at address PC*/
			cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
			break;
		case 1:	/*Second cycle perform ADC*/
			cpu_ctx.registers.A = ADC(cpu_ctx.registers.A, cpu_ctx.instruction.data);
			break;
		}
		break;

	default:
		break;
	}

	/* Decrement execute cycles */
	cpu_ctx.instruction.cycles--;

	/* Enable fetch when execute is ended */
	if (cpu_ctx.instruction.cycles == 0U)
	{
		cpu_ctx.variables.fetch_enabled = 1U;
	}
}


/**
  * @brief	Execute ADC between num1 and num2 and update flags
  * @param  num1:		first number
  * 		num2:		second number
  * @retval uint8_t:	result
  */
static uint8_t ADC(uint8_t num1, uint8_t num2)
{
	uint16_t result;

	result = num1 + num2 + cpu_ctx.registers.c_flag;

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (result > 0xFF)		cpu_ctx.registers.c_flag = 1U;
	if ((uint16_t)((num1 & 0xF) + (num2 & 0xF)) > 0xF)		cpu_ctx.registers.h_flag = 1U;
	if (result == 0U)		cpu_ctx.registers.z_flag = 1U;

	return result;
}
