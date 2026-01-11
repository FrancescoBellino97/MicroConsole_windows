/**
 ******************************************************************************
 * @file    cpu.c
 * @author  Bellino Francesco
 * @brief   CPU module source code:
 * 				- fetch instructions.
 * 				- decode instructions.
 * 				- execute instructions.
 * 				- emulate CPU operations
 *
 ******************************************************************************
 */

#include "cpu.h"
#include "bus.h"


#ifndef UNIT_TEST
static void decode(uint8_t op_code);
static void execute();

/*Emulated functions*/
static uint8_t ADC_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t ADD_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint16_t ADD_U16_S8_BIT(uint16_t num1, int8_t num2);
static uint16_t ADD_U16_U16_BIT(uint16_t num1, uint16_t num2);
static uint8_t SUB_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t SBC_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t AND_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t OR_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t XOR_U8_U8_BIT(uint8_t num1, uint8_t num2);
static uint8_t INC_U8_BIT(uint8_t num);
static uint16_t INC_U16_BIT(uint16_t num);
static uint8_t DEC_U8_BIT(uint8_t num);
static uint16_t DEC_U16_BIT(uint16_t num);
#endif


#ifdef UNIT_TEST
cpu_context cpu_ctx;
#else
static cpu_context cpu_ctx;
#endif


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
#ifdef UNIT_TEST
void decode(uint8_t op_code)
#else
static void decode(uint8_t op_code)
#endif
{
	switch (op_code)
	{
	case 0x00:	/*NOP*/
		cpu_ctx.instruction.type = TYPE_NOP;
		cpu_ctx.instruction.cycles = 1U;
		break;

	case 0x03:	/*INC BC*/
		cpu_ctx.instruction.type = TYPE_INC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.BC;
		break;
	case 0x04:	/*INC B*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;
		break;
	case 0x05:	/*DEC B*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;
		break;

	case 0x09:	/*ADD HL,BC*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = cpu_ctx.registers.BC;
		break;

	case 0x0B:	/*DEC BC*/
		cpu_ctx.instruction.type = TYPE_DEC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.BC;
		break;
	case 0x0C:	/*INC C*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;
		break;
	case 0x0D:	/*DEC C*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;
		break;

	case 0x13:	/*INC DE*/
		cpu_ctx.instruction.type = TYPE_INC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.DE;
		break;
	case 0x14:	/*INC D*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;
		break;
	case 0x15:	/*DEC D*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;
		break;

	case 0x19:	/*ADD HL,DE*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = cpu_ctx.registers.DE;
		break;

	case 0x1B:	/*DEC DE*/
		cpu_ctx.instruction.type = TYPE_DEC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.DE;
		break;
	case 0x1C:	/*INC E*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;
		break;
	case 0x1D:	/*DEC E*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;
		break;

	case 0x23:	/*INC HL*/
		cpu_ctx.instruction.type = TYPE_INC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.HL;
		break;
	case 0x24:	/*INC H*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;
		break;
	case 0x25:	/*DEC H*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;
		break;

	case 0x29:	/*ADD HL,HL*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = cpu_ctx.registers.HL;
		break;

	case 0x2B:	/*DEC HL*/
		cpu_ctx.instruction.type = TYPE_DEC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.HL;
		break;
	case 0x2C:	/*INC L*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;
		break;
	case 0x2D:	/*DEC L*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;
		break;

	case 0x33:	/*INC SP*/
		cpu_ctx.instruction.type = TYPE_INC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.SP;
		break;
	case 0x34:	/*INC (HL)*/
		cpu_ctx.instruction.type = TYPE_INC_HL;
		cpu_ctx.instruction.cycles = 3U;
		break;
	case 0x35:	/*DEC (HL)*/
		cpu_ctx.instruction.type = TYPE_DEC_HL;
		cpu_ctx.instruction.cycles = 3U;
		break;

	case 0x39:	/*ADD HL,SP*/
		cpu_ctx.instruction.type = TYPE_ADD_HL_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = cpu_ctx.registers.SP;
		break;

	case 0x3B:	/*DEC SP*/
		cpu_ctx.instruction.type = TYPE_DEC_R16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.reg_16bit = &cpu_ctx.registers.SP;
		break;
	case 0x3C:	/*INC A*/
		cpu_ctx.instruction.type = TYPE_INC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;
		break;
	case 0x3D:	/*DEC A*/
		cpu_ctx.instruction.type = TYPE_DEC_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;
		break;

	case 0x40:	/*LD B,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x41:	/*LD B,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x42:	/*LD B,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x43:	/*LD B,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x44:	/*LD B,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x45:	/*LD B,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x47:	/*LD B,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.B;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;
	case 0x48:	/*LD C,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x49:	/*LD C,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x4A:	/*LD C,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x4B:	/*LD C,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x4C:	/*LD C,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x4D:	/*LD C,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x4F:	/*LD C,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.C;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;
	case 0x50:	/*LD D,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x51:	/*LD D,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x52:	/*LD D,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x53:	/*LD D,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x54:	/*LD D,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x55:	/*LD D,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x57:	/*LD D,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.D;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;
	case 0x58:	/*LD E,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x59:	/*LD E,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x5A:	/*LD E,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x5B:	/*LD E,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x5C:	/*LD E,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x5D:	/*LD E,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x5F:	/*LD E,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.E;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;
	case 0x60:	/*LD H,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x61:	/*LD H,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x62:	/*LD H,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x63:	/*LD H,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x64:	/*LD H,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x65:	/*LD H,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x67:	/*LD H,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.H;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;
	case 0x68:	/*LD L,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x69:	/*LD L,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x6A:	/*LD L,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x6B:	/*LD L,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x6C:	/*LD L,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x6D:	/*LD L,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x6F:	/*LD L,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.L;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;

	case 0x78:	/*LD A,B*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.B;			/*Source*/
		break;
	case 0x79:	/*LD A,C*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.C;			/*Source*/
		break;
	case 0x7A:	/*LD A,D*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.D;			/*Source*/
		break;
	case 0x7B:	/*LD A,E*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.E;			/*Source*/
		break;
	case 0x7C:	/*LD A,H*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.H;			/*Source*/
		break;
	case 0x7D:	/*LD A,L*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.L;			/*Source*/
		break;
	case 0x7F:	/*LD A,A*/
		cpu_ctx.instruction.type = TYPE_LD_R8_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.reg_8bit = &cpu_ctx.registers.A;	/*Destination*/
		cpu_ctx.instruction.data = cpu_ctx.registers.A;			/*Source*/
		break;

	case 0x80:	/*ADD A,B*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0x81:	/*ADD A,C*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0x82:	/*ADD A,D*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0x83:	/*ADD A,E*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0x84:	/*ADD A,H*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0x85:	/*ADD A,L*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0x86:	/*ADD A,(HL)*/
		cpu_ctx.instruction.type = TYPE_ADD_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0x87:	/*ADD A,A*/
		cpu_ctx.instruction.type = TYPE_ADD_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0x88:	/*ADC A,B*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0x89:	/*ADC A,C*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0x8A:	/*ADC A,D*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0x8B:	/*ADC A,E*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0x8C:	/*ADC A,H*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0x8D:	/*ADC A,L*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0x8E:	/*ADC A,(HL)*/
		cpu_ctx.instruction.type = TYPE_ADC_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0x8F:	/*ADC A,A*/
		cpu_ctx.instruction.type = TYPE_ADC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0x90:	/*SUB B*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0x91:	/*SUB C*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0x92:	/*SUB D*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0x93:	/*SUB E*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0x94:	/*SUB H*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0x95:	/*SUB L*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0x96:	/*SUB HL*/
		cpu_ctx.instruction.type = TYPE_SUB_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0x97:	/*SUB A*/
		cpu_ctx.instruction.type = TYPE_SUB_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0x98:	/*SBC A,B*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0x99:	/*SBC A,C*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0x9A:	/*SBC A,D*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0x9B:	/*SBC A,E*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0x9C:	/*SBC A,H*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0x9D:	/*SBC A,L*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0x9E:	/*SBC A,(HL)*/
		cpu_ctx.instruction.type = TYPE_SBC_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0x9F:	/*SBC A,A*/
		cpu_ctx.instruction.type = TYPE_SBC_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;

	case 0xA0:	/*AND B*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0xA1:	/*AND C*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0xA2:	/*AND D*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0xA3:	/*AND E*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0xA4:	/*AND H*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0xA5:	/*AND L*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0xA6:	/*AND (HL)*/
		cpu_ctx.instruction.type = TYPE_AND_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0xA7:	/*AND A*/
		cpu_ctx.instruction.type = TYPE_AND_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0xA8:	/*XOR B*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0xA9:	/*XOR C*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0xAA:	/*XOR D*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0xAB:	/*XOR E*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0xAC:	/*XOR H*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0xAD:	/*XOR L*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0xAE:	/*XOR (HL)*/
		cpu_ctx.instruction.type = TYPE_XOR_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0xAF:	/*XOR A*/
		cpu_ctx.instruction.type = TYPE_XOR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0xB0:	/*OR B*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0xB1:	/*OR C*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0xB2:	/*OR D*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0xB3:	/*OR E*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0xB4:	/*OR H*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0xB5:	/*OR L*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0xB6:	/*OR (HL)*/
		cpu_ctx.instruction.type = TYPE_OR_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0xB7:	/*OR A*/
		cpu_ctx.instruction.type = TYPE_OR_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;
	case 0xB8:	/*CP B*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.B;
		break;
	case 0xB9:	/*CP C*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.C;
		break;
	case 0xBA:	/*CP D*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.D;
		break;
	case 0xBB:	/*CP E*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.E;
		break;
	case 0xBC:	/*CP H*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.H;
		break;
	case 0xBD:	/*CP L*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.L;
		break;
	case 0xBE:	/*CP (HL)*/
		cpu_ctx.instruction.type = TYPE_CP_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
		break;
	case 0xBF:	/*CP A*/
		cpu_ctx.instruction.type = TYPE_CP_A_R8;
		cpu_ctx.instruction.cycles = 1U;
		cpu_ctx.instruction.data = cpu_ctx.registers.A;
		break;

	case 0xC6:	/*ADD A,u8*/
		cpu_ctx.instruction.type = TYPE_ADD_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xCE:	/*ADC A,u8*/
		cpu_ctx.instruction.type = TYPE_ADC_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xD6:	/*SUB A,u8*/
		cpu_ctx.instruction.type = TYPE_SUB_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xDE:	/*SBC A,u8*/
		cpu_ctx.instruction.type = TYPE_SBC_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xE6:	/*AND u8*/
		cpu_ctx.instruction.type = TYPE_AND_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xE8:	/*ADD SP,s8*/
		cpu_ctx.instruction.type = TYPE_ADD_SP_S8;
		cpu_ctx.instruction.cycles = 4U;
		break;

	case 0xEE:	/*XOR u8*/
		cpu_ctx.instruction.type = TYPE_XOR_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xF6:	/*OR u8*/
		cpu_ctx.instruction.type = TYPE_OR_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	case 0xFE:	/*CP u8*/
		cpu_ctx.instruction.type = TYPE_CP_A_A16;
		cpu_ctx.instruction.cycles = 2U;
		cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
		break;

	default:
		cpu_ctx.instruction.type = TYPE_NOP;	/*Default case is NOP*/
		cpu_ctx.instruction.cycles = 1U;
		break;
	}
}


/**
  * @brief	Execute the current instruction taking into account cycles needed
  * @param  None
  * @retval None
  */
#ifdef UNIT_TEST
void execute()
#else
static void execute()
#endif
{
	switch(cpu_ctx.instruction.type)
	{
	/* NOP */
	case TYPE_NOP: /*It takes only 1 cycle and do nothing*/
		break;

	/* ADD */
	case TYPE_ADD_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = ADD_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_ADD_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform ADD*/
			cpu_ctx.registers.A = ADD_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	case TYPE_ADD_HL_R16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle perform ADD*/
			cpu_ctx.registers.HL = ADD_U16_U16_BIT(cpu_ctx.registers.HL, cpu_ctx.instruction.data);
			break;
		case 1:	/*Second cycle do nothing */
			break;
		}
		break;

	case TYPE_ADD_SP_S8:
		switch (cpu_ctx.instruction.cycles)
		{
		case 4:	/*First cycle read data at address PC*/
			cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.PC++);
			break;
		case 3:	/*Second cycle do nothing*/
			break;
		case 2:	/*Third cycle perform ADD*/
			cpu_ctx.registers.SP = ADD_U16_S8_BIT(cpu_ctx.registers.SP, cpu_ctx.instruction.data & 0xFF);
			break;
		case 1:	/*Fourth cycle do nothing*/
			break;
		}
		break;
		
	/* ADC */
	case TYPE_ADC_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = ADC_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_ADC_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform ADC*/
			cpu_ctx.registers.A = ADC_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;
		
	/* SUB */
	case TYPE_SUB_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = SUB_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_SUB_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform SUB*/
			cpu_ctx.registers.A = SUB_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* SBC */
	case TYPE_SBC_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = SBC_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_SBC_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform SBC*/
			cpu_ctx.registers.A = SBC_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* AND */
	case TYPE_AND_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = AND_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_AND_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform AND*/
			cpu_ctx.registers.A = ADD_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* OR */
	case TYPE_OR_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = OR_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_OR_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform OR*/
			cpu_ctx.registers.A = OR_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* XOR */
	case TYPE_XOR_A_R8:	/*It takes only 1 cycle*/
		cpu_ctx.registers.A = XOR_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_XOR_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform XOR*/
			cpu_ctx.registers.A = XOR_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* CP */
	case TYPE_CP_A_R8:	/*It takes only 1 cycle*/
		(void) SUB_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
		break;

	case TYPE_CP_A_A16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle do nothing (bus read already done during decode)*/
			break;
		case 1:	/*Second cycle perform CP*/
			(void) SUB_U8_U8_BIT(cpu_ctx.registers.A, (uint8_t)cpu_ctx.instruction.data);
			break;
		}
		break;

	/* INC */
	case TYPE_INC_R8:	/*It takes only 1 cycle*/
		*cpu_ctx.instruction.reg_8bit = INC_U8_BIT(*cpu_ctx.instruction.reg_8bit);
		break;

	case TYPE_INC_HL:
		switch (cpu_ctx.instruction.cycles)
		{
		case 3:	/*First cycle read data at address HL*/
			cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
			break;
		case 2:	/*Second cycle perform INC*/
			cpu_ctx.instruction.data = INC_U8_BIT(cpu_ctx.instruction.data);
			break;
		case 1:	/*Third cycle write value at address HL*/
			bus_write(cpu_ctx.registers.HL, cpu_ctx.instruction.data);
			break;
		}
		break;

	case TYPE_INC_R16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle perform INC*/
			*cpu_ctx.instruction.reg_16bit = INC_U16_BIT(*cpu_ctx.instruction.reg_16bit);
			break;
		case 1:	/*Second cycle do nothing*/
			break;
		}
		break;

	/* DEC */
	case TYPE_DEC_R8:	/*It takes only 1 cycle*/
		*cpu_ctx.instruction.reg_8bit = DEC_U8_BIT(*cpu_ctx.instruction.reg_8bit);
		break;

	case TYPE_DEC_HL:
		switch (cpu_ctx.instruction.cycles)
		{
		case 3:	/*First cycle read data at address HL*/
			cpu_ctx.instruction.data = bus_read(cpu_ctx.registers.HL);
			break;
		case 2:	/*Second cycle perform DEC*/
			cpu_ctx.instruction.data = DEC_U8_BIT(cpu_ctx.instruction.data);
			break;
		case 1:	/*Third cycle write value at address HL*/
			bus_write(cpu_ctx.registers.HL, cpu_ctx.instruction.data);
			break;
		}
		break;

	case TYPE_DEC_R16:
		switch (cpu_ctx.instruction.cycles)
		{
		case 2:	/*First cycle perform DEC*/
			*cpu_ctx.instruction.reg_16bit = DEC_U16_BIT(*cpu_ctx.instruction.reg_16bit);
			break;
		case 1:	/*Second cycle do nothing*/
			break;
		}
		break;

	/* LD */
	case TYPE_LD_R8_R8:	/*It takes only 1 cycle*/
		*cpu_ctx.instruction.reg_8bit = cpu_ctx.instruction.data;
		break;

	default:
		break;
	}

	/* Decrement execute cycles */
	if (cpu_ctx.instruction.cycles > 0U)
	{
		cpu_ctx.instruction.cycles--;
	}

	/* Enable fetch when execute is ended */
	if (cpu_ctx.instruction.cycles == 0U)
	{
		cpu_ctx.variables.fetch_enabled = 1U;
	}
}


/**
  * @brief	Execute ADC between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t ADC_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t ADC_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint16_t result;

	result = (uint16_t)(num1 + num2 + cpu_ctx.registers.c_flag);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint16_t)((num1 & 0xF) + (num2 & 0xF) +  cpu_ctx.registers.c_flag) > 0xF)		cpu_ctx.registers.h_flag = 1U;
	if (result > 0xFF)		cpu_ctx.registers.c_flag = 1U;
	if (result == 0U)		cpu_ctx.registers.z_flag = 1U;

	return result;
}


/**
  * @brief	Execute ADD between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t ADD_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t ADD_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint16_t result;

	result = (uint16_t)(num1 + num2);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint16_t)((num1 & 0xF) + (num2 & 0xF)) > 0xF)		cpu_ctx.registers.h_flag = 1U;
	if (result > 0xFF)		cpu_ctx.registers.c_flag = 1U;
	if (result == 0U)		cpu_ctx.registers.z_flag = 1U;

	return (uint8_t)result;
}


/**
  * @brief	Execute ADD between unsigned 16bit and signed 8bit and update flags
  * @param  num1:		first number unsigned 16bit
  * 		num2:		second number signed 8bit
  * @retval uint16_t:	result unsigned 16bit
  */
#ifdef UNIT_TEST
uint16_t ADD_U16_S8_BIT(uint16_t num1, int8_t num2)
#else
static uint16_t ADD_U16_S8_BIT(uint16_t num1, int8_t num2)
#endif
{
	uint32_t result;

	result = (uint32_t)(num1 + num2);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint16_t)((num1 & 0xF) + (num2 & 0xF)) > 0xF)		cpu_ctx.registers.h_flag = 1U;
	if (result > 0xFF)		cpu_ctx.registers.c_flag = 1U;

	return (uint16_t)result;
}


/**
  * @brief	Execute ADD between unsigned 16bit and unsigned 16bit and update flags
  * @param  num1:		first number unsigned 16bit
  * 		num2:		second number unsigned 16bit
  * @retval uint16_t:	result unsigned 16bit
  */
#ifdef UNIT_TEST
uint16_t ADD_U16_U16_BIT(uint16_t num1, uint16_t num2)
#else
static uint16_t ADD_U16_U16_BIT(uint16_t num1, uint16_t num2)
#endif
{
	uint32_t result;

	result = (uint32_t)(num1 + num2);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint16_t)((num1 & 0xFF) + (num2 & 0xFF)) > 0xFF)	cpu_ctx.registers.h_flag = 1U;
	if (result > 0xFFFF)	cpu_ctx.registers.c_flag = 1U;
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint16_t)result;
}


/**
  * @brief	Execute SUB between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t SUB_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t SUB_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint16_t result;

	result = (uint16_t)(num1 - num2);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 1U;	/*Always 1*/
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (((num1 & 0xF) - (num2 & 0xF)) < 0x0)	cpu_ctx.registers.h_flag = 1U;
	if ((num1 - num2) < 0x0)	cpu_ctx.registers.c_flag = 1U;
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint8_t)result;
}


/**
  * @brief	Execute SBC between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t SBC_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t SBC_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint16_t result;

	result = (uint16_t)(num1 - num2 - cpu_ctx.registers.c_flag);

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 1U;	/*Always 1*/
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (((num1 & 0xF) - (num2 & 0xF) - cpu_ctx.registers.c_flag) < 0x0)	cpu_ctx.registers.h_flag = 1U;
	if ((num1 - num2 - cpu_ctx.registers.c_flag) < 0x0)	cpu_ctx.registers.c_flag = 1U;
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint8_t)result;
}


/**
  * @brief	Execute AND between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t AND_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t AND_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint8_t result;

	result = num1 & num2;

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 1U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint16_t)result;
}


/**
  * @brief	Execute OR between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t OR_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t OR_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint8_t result;

	result = num1 | num2;

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint16_t)result;
}


/**
  * @brief	Execute XOR between unsigned 8bit and unsigned 8bit and update flags
  * @param  num1:		first number unsigned 8bit
  * 		num2:		second number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t XOR_U8_U8_BIT(uint8_t num1, uint8_t num2)
#else
static uint8_t XOR_U8_U8_BIT(uint8_t num1, uint8_t num2)
#endif
{
	uint8_t result;

	result = num1 ^ num2;

	/* Reset cpu flags */
	cpu_ctx.registers.c_flag = 0U;
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if (result == 0U)	cpu_ctx.registers.z_flag = 1U;

	return (uint16_t)result;
}


/**
  * @brief	Execute INC on unsigned 8bit and update flags
  * @param  num:		number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t INC_U8_BIT(uint8_t num)
#else
static uint8_t INC_U8_BIT(uint8_t num)
#endif
{
	uint16_t result;

	result = (uint16_t)(num + 1);

	/* Reset cpu flags */
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint8_t)result == 0U)	cpu_ctx.registers.z_flag = 1U;
	if (((uint8_t)result & 0xF) == 0x0)	cpu_ctx.registers.h_flag = 1U;

	return (uint8_t) result;
}


/**
  * @brief	Execute INC on unsigned 16bit (don't update flags)
  * @param  num:		number unsigned 16bit
  * @retval uint8_t:	result unsigned 16bit
  */
#ifdef UNIT_TEST
uint16_t INC_U16_BIT(uint16_t num)
#else
static uint16_t INC_U16_BIT(uint16_t num)
#endif
{
	uint32_t result;

	result = (uint32_t)(num + 1);

	return (uint16_t) result;
}


/**
  * @brief	Execute DEC on unsigned 8bit and update flags
  * @param  num:		number unsigned 8bit
  * @retval uint8_t:	result unsigned 8bit
  */
#ifdef UNIT_TEST
uint8_t DEC_U8_BIT(uint8_t num)
#else
static uint8_t DEC_U8_BIT(uint8_t num)
#endif
{
	uint16_t result;

	result = (uint16_t)(num - 1);

	/* Reset cpu flags */
	cpu_ctx.registers.h_flag = 0U;
	cpu_ctx.registers.n_flag = 0U;
	cpu_ctx.registers.z_flag = 0U;

	/* Update CPU flags */
	if ((uint8_t)result == 0U)	cpu_ctx.registers.z_flag = 1U;
	if (((uint8_t)result & 0xF) == 0xF)	cpu_ctx.registers.h_flag = 1U;

	return (uint8_t) result;
}


/**
  * @brief	Execute DEC on unsigned 16bit (don't update flags)
  * @param  num:		number unsigned 16bit
  * @retval uint8_t:	result unsigned 16bit
  */
#ifdef UNIT_TEST
uint16_t DEC_U16_BIT(uint16_t num)
#else
static uint16_t DEC_U16_BIT(uint16_t num)
#endif
{
	uint32_t result;

	result = (uint32_t)(num - 1);

	return (uint16_t) result;
}
