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
#define TYPE_NOP			(0U)
/*ADC*/
#define TYPE_ADC_A_R8		(1U)
#define TYPE_ADC_A_HL		(2U)
#define TYPE_ADC_A_U8		(3U)
/*ADD*/
#define TYPE_ADD_A_R8		(4U)
#define TYPE_ADD_A_HL		(5U)
#define TYPE_ADD_A_U8		(6U)
#define TYPE_ADD_HL_R16		(7U)
#define TYPE_ADD_SP_S8		(8U)
/*AND*/
#define TYPE_AND_A_R8		(9U)
#define TYPE_AND_A_HL		(10U)
#define TYPE_AND_A_U8		(11U)

/*LD*/
#define LD_R16_U16			(12U)


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
		uint16_t *reg;			/*Pointer to 16bit register of the instruction*/
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
uint8_t AND_U8_U8_BIT(uint8_t num1, uint8_t num2);
#endif


#endif /* CORE_INC_CPU_H_ */
