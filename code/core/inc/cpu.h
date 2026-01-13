/**
 ******************************************************************************
 * @file    cpu.h
 * @author  Bellino Francesco
 * @brief   CPU module header file.
 *
 ******************************************************************************
 */

#ifndef CORE_INC_CPU_H_
#define CORE_INC_CPU_H_


#include <stdint.h>


/* Instruction type group */
#define TYPE_NOP			( 0U)
/*ADD*/
#define TYPE_ADD_A_R8		( 1U)
#define TYPE_ADD_A_A16		( 2U)
#define TYPE_ADD_HL_R16		( 3U)
#define TYPE_ADD_SP_S8		( 4U)
/*ADC*/
#define TYPE_ADC_A_R8		( 5U)
#define TYPE_ADC_A_A16		( 6U)
/*SUB*/
#define TYPE_SUB_A_R8		( 7U)
#define TYPE_SUB_A_A16		( 8U)
/*SBC*/
#define TYPE_SBC_A_R8		( 9U)
#define TYPE_SBC_A_A16		(10U)
/*AND*/
#define TYPE_AND_A_R8		(11U)
#define TYPE_AND_A_A16		(12U)
/*OR*/
#define TYPE_OR_A_R8		(13U)
#define TYPE_OR_A_A16		(14U)
/*XOR*/
#define TYPE_XOR_A_R8		(15U)
#define TYPE_XOR_A_A16		(16U)
/*CP*/
#define TYPE_CP_A_R8		(17U)
#define TYPE_CP_A_A16		(18U)
/*INC*/
#define TYPE_INC_R8			(19U)
#define TYPE_INC_HL			(20U)
#define TYPE_INC_R16		(21U)
/*DEC*/
#define TYPE_DEC_R8			(22U)
#define TYPE_DEC_HL			(23U)
#define TYPE_DEC_R16		(24U)
/*LD*/
#define TYPE_LD_R8_R8		(25U)
#define TYPE_LD_R8_U8		(26U)
#define TYPE_LD_A16_R8		(27U)
#define TYPE_LD_HLI_R8		(28U)
#define TYPE_LD_HLD_R8		(29U)
#define TYPE_LD_R16_R16		(30U)


typedef struct{
	/* CPU registers */
	struct{
		union{
			uint16_t AF;
			struct{
				union{
					struct{
						uint8_t unused	:4;
						uint8_t c_flag	:1;		/*Carry flag*/
						uint8_t h_flag	:1;		/*Half carry flag*/
						uint8_t n_flag	:1;		/*Subtraction flag*/
						uint8_t z_flag	:1;		/*Zero flag*/
					};
					uint8_t F;
				};
				uint8_t A;
			};
		};
		union{
			uint16_t BC;
			struct{
				uint8_t C;
				uint8_t B;
			};
		};
		union{
			uint16_t DE;
			struct{
				uint8_t E;
				uint8_t D;
			};
		};
		union{
			uint16_t HL;
			struct{
				uint8_t L;
				uint8_t H;
			};
		};
		uint16_t SP;	/*Stack pointer*/
		uint16_t PC;	/*Program counter*/
	} registers;


	/* CPU instruction */
	struct{
		uint8_t type;			/*Type of instruction*/
		uint8_t cycles;			/*Number of cycle needed by the instruction*/
		uint16_t data;			/*It depends on the instruction type*/
		uint8_t *reg_8bit;		/*Pointer to 8bit register of the instruction*/
		uint16_t *reg_16bit;	/*Pointer to 16bit register of the instruction*/
		uint8_t condition;		/*Condition used by CALL or JP*/
	}instruction;

	/* CPU variables */
	struct{
		uint8_t IME;				/*Interrupt flag status*/
		uint8_t state;				/*CPU status (1: running, 0: halted)*/
		uint8_t fetch_enabled;		/*Flag to enable fetch of next instruction*/
		uint8_t interrupt_enabled;	/*Flag to enable interrupt on next cycle*/
	} variables;
}cpu_context;


#ifdef UNIT_TEST
extern cpu_context cpu_ctx;
#endif


void cpu_init();
void cpu_run();
#ifdef UNIT_TEST
void decode(uint8_t op_code);
void execute();
uint8_t ADC_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t ADD_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint16_t ADD_U16_S8_BIT(uint16_t num1, int8_t num2);
uint16_t ADD_U16_U16_BIT(uint16_t num1, uint16_t num2);
uint8_t SUB_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t SBC_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t AND_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t OR_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t XOR_U8_U8_BIT(uint8_t num1, uint8_t num2);
uint8_t INC_U8_BIT(uint8_t num);
uint16_t INC_U16_BIT(uint16_t num);
uint8_t DEC_U8_BIT(uint8_t num);
uint16_t DEC_U16_BIT(uint16_t num);
#endif


#endif /* CORE_INC_CPU_H_ */
