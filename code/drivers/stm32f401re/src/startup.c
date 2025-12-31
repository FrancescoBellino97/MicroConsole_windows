/**
 ******************************************************************************
 * @file    startup.c
 * @author  Bellino Francesco
 * @brief   Startup code called after a reset. It set the handlers and
 * 			initialize the bss section
 *
 *
 ******************************************************************************
 */


#include <stdint.h>
#include "main.h"

#define SRAM_START (0x20000000U)												/* Address of SRAM start */
#define SRAM_SIZE (64U * 1024U)													/* SRAM size (64k) */
#define SRAM_END (SRAM_START + SRAM_SIZE)										/* Address of SRAM end */
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)									/* Address of stack start*/


/* ISR VECTOR */
#define ISR_VECTOR_SIZE_WORDS 114

void Default_handler(void);														/* Default handler for not managed interrupt */

void Reset_handler(void);
void NMI_handler(void) __attribute__((weak, alias("Default_handler")));
void HardFault_handler(void) __attribute__((weak, alias("Default_handler")));
void MemManage_handler(void) __attribute__((weak, alias("Default_handler")));
void BusFault_handler(void) __attribute__((weak, alias("Default_handler")));
void UsageFault_handler(void) __attribute__((weak, alias("Default_handler")));
void SVCall_handler(void) __attribute__((weak, alias("Default_handler")));
void DebugMonitor_handler(void) __attribute__((weak, alias("Default_handler")));
void PendSV_handler(void) __attribute__((weak, alias("Default_handler")));
void Systick_handler(void) __attribute__((weak, alias("Default_handler")));
void WWDG_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI16_PVD_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI21_TAMP_STAMP_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI22_RTC_WKUP_handler(void) __attribute__((weak, alias("Default_handler")));
void FLASH_handler(void) __attribute__((weak, alias("Default_handler")));
void RCC_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI0_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI1_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI2_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI3_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI4_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream0_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream1_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream2_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream3_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream4_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream5_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA_Stream6_handler(void) __attribute__((weak, alias("Default_handler")));
void ADC_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI9_5_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM1_BRK_TIM9_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM1_UP_TIM10_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM1_TRG_COM_TIM11_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM1_CC_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM2_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM3_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM4_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C1_EV_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C1_ER_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C2_EV_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C2_ER_handler(void) __attribute__((weak, alias("Default_handler")));
void SPI1_handler(void) __attribute__((weak, alias("Default_handler")));
void SPI2_handler(void) __attribute__((weak, alias("Default_handler")));
void USART1_handler(void) __attribute__((weak, alias("Default_handler")));
void USART2_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI15_10_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI17_RTC_Alarm_handler(void) __attribute__((weak, alias("Default_handler")));
void EXTI18_OTG_FS_WKUP_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA1_Stream7_handler(void) __attribute__((weak, alias("Default_handler")));
void SDIO_handler(void) __attribute__((weak, alias("Default_handler")));
void TIM5_handler(void) __attribute__((weak, alias("Default_handler")));
void SPI3_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream0_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream1_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream2_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream3_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream4_handler(void) __attribute__((weak, alias("Default_handler")));
void OTG_FS_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream5_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream6_handler(void) __attribute__((weak, alias("Default_handler")));
void DMA2_Stream7_handler(void) __attribute__((weak, alias("Default_handler")));
void USART6_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C3_EV_handler(void) __attribute__((weak, alias("Default_handler")));
void I2C3_ER_handler(void) __attribute__((weak, alias("Default_handler")));
void FPU_handler(void) __attribute__((weak, alias("Default_handler")));
void SPI4_handler(void) __attribute__((weak, alias("Default_handler")));

uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
		STACK_POINTER_INIT_ADDRESS,												/* 0x0000 0000 */
		(uint32_t)&Reset_handler,												/* 0x0000 0004 */
		(uint32_t)&NMI_handler,													/* 0x0000 0008 */
		(uint32_t)&HardFault_handler,											/* 0x0000 000C */
		(uint32_t)&MemManage_handler,											/* 0x0000 0010 */
		(uint32_t)&BusFault_handler,											/* 0x0000 0014 */
		(uint32_t)&UsageFault_handler,											/* 0x0000 0018 */
		0,																		/* 0x0000 001C Reserved */
		0,																		/* 0x0000 0020 Reserved */
		0,																		/* 0x0000 0024 Reserved */
		0,																		/* 0x0000 0028 Reserved */
		(uint32_t)&SVCall_handler,												/* 0x0000 002C */
		(uint32_t)&DebugMonitor_handler,										/* 0x0000 0030 */
		0,																		/* 0x0000 0034 Reserved */
		(uint32_t)&PendSV_handler,												/* 0x0000 0038 */
		(uint32_t)&Systick_handler,												/* 0x0000 003C */
		(uint32_t)&WWDG_handler,												/* 0x0000 0040 */
		(uint32_t)&EXTI16_PVD_handler,											/* 0x0000 0044 */
		(uint32_t)&EXTI21_TAMP_STAMP_handler,									/* 0x0000 0048 */
		(uint32_t)&EXTI22_RTC_WKUP_handler,										/* 0x0000 004C */
		(uint32_t)&FLASH_handler,												/* 0x0000 0050 */
		(uint32_t)&RCC_handler,													/* 0x0000 0054 */
		(uint32_t)&EXTI0_handler,												/* 0x0000 0058 */
		(uint32_t)&EXTI1_handler,												/* 0x0000 005C */
		(uint32_t)&EXTI2_handler,												/* 0x0000 0060 */
		(uint32_t)&EXTI3_handler,												/* 0x0000 0064 */
		(uint32_t)&EXTI4_handler,												/* 0x0000 0068 */
		(uint32_t)&DMA_Stream0_handler,											/* 0x0000 006C */
		(uint32_t)&DMA_Stream1_handler,											/* 0x0000 0070 */
		(uint32_t)&DMA_Stream2_handler,											/* 0x0000 0074 */
		(uint32_t)&DMA_Stream3_handler,											/* 0x0000 0078 */
		(uint32_t)&DMA_Stream4_handler,											/* 0x0000 007C */
		(uint32_t)&DMA_Stream5_handler,											/* 0x0000 0080 */
		(uint32_t)&DMA_Stream6_handler,											/* 0x0000 0084 */
		(uint32_t)&ADC_handler,													/* 0x0000 0088 */
		0,																		/* 0x0000 008C Reserved */
		0,																		/* 0x0000 0090 Reserved */
		0,																		/* 0x0000 0094 Reserved */
		0,																		/* 0x0000 0098 Reserved */
		(uint32_t)&EXTI9_5_handler,												/* 0x0000 009C */
		(uint32_t)&TIM1_BRK_TIM9_handler,										/* 0x0000 00A0 */
		(uint32_t)&TIM1_UP_TIM10_handler,										/* 0x0000 00A4 */
		(uint32_t)&TIM1_TRG_COM_TIM11_handler,									/* 0x0000 00A8 */
		(uint32_t)&TIM1_CC_handler,												/* 0x0000 00AC */
		(uint32_t)&TIM2_handler,												/* 0x0000 00B0 */
		(uint32_t)&TIM3_handler,												/* 0x0000 00B4 */
		(uint32_t)&TIM4_handler,												/* 0x0000 00B8 */
		(uint32_t)&I2C1_EV_handler,												/* 0x0000 00BC */
		(uint32_t)&I2C1_ER_handler,												/* 0x0000 00C0 */
		(uint32_t)&I2C2_EV_handler,												/* 0x0000 00C4 */
		(uint32_t)&I2C2_ER_handler,												/* 0x0000 00C8 */
		(uint32_t)&SPI1_handler,												/* 0x0000 00CC */
		(uint32_t)&SPI2_handler,												/* 0x0000 00D0 */
		(uint32_t)&USART1_handler,												/* 0x0000 00D4 */
		(uint32_t)&USART2_handler,												/* 0x0000 00D8 */
		0,																		/* 0x0000 00DC Reserved */
		(uint32_t)&EXTI15_10_handler,											/* 0x0000 00E0 */
		(uint32_t)&EXTI17_RTC_Alarm_handler,									/* 0x0000 00E4 */
		(uint32_t)&EXTI18_OTG_FS_WKUP_handler,									/* 0x0000 00E8 */
		0,																		/* 0x0000 00EC Reserved */
		0,																		/* 0x0000 00F0 Reserved */
		0,																		/* 0x0000 00F4 Reserved */
		0,																		/* 0x0000 00F8 Reserved */
		(uint32_t)&DMA1_Stream7_handler,										/* 0x0000 00FC */
		0,																		/* 0x0000 0100 Reserved */
		(uint32_t)&SDIO_handler,												/* 0x0000 0104 */
		(uint32_t)&TIM5_handler,												/* 0x0000 0108 */
		(uint32_t)&SPI3_handler,												/* 0x0000 010C */
		0,																		/* 0x0000 0110 Reserved */
		0,																		/* 0x0000 0114 Reserved */
		0,																		/* 0x0000 0118 Reserved */
		0,																		/* 0x0000 011C Reserved */
		(uint32_t)&DMA2_Stream0_handler,										/* 0x0000 0120 */
		(uint32_t)&DMA2_Stream1_handler,										/* 0x0000 0124 */
		(uint32_t)&DMA2_Stream2_handler,										/* 0x0000 0128 */
		(uint32_t)&DMA2_Stream3_handler,										/* 0x0000 012C */
		(uint32_t)&DMA2_Stream4_handler,										/* 0x0000 0130 */
		0,																		/* 0x0000 0134 Reserved */
		0,																		/* 0x0000 0138 Reserved */
		0,																		/* 0x0000 013C Reserved */
		0,																		/* 0x0000 0140 Reserved */
		0,																		/* 0x0000 0144 Reserved */
		0,																		/* 0x0000 0148 Reserved */
		(uint32_t)&OTG_FS_handler,												/* 0x0000 014C */
		(uint32_t)&DMA2_Stream5_handler,										/* 0x0000 0150 */
		(uint32_t)&DMA2_Stream6_handler,										/* 0x0000 0154 */
		(uint32_t)&DMA2_Stream7_handler,										/* 0x0000 0158 */
		(uint32_t)&USART6_handler,												/* 0x0000 015C */
		(uint32_t)&I2C3_EV_handler,												/* 0x0000 0160 */
		(uint32_t)&I2C3_ER_handler,												/* 0x0000 0164 */
		0,																		/* 0x0000 0168 Reserved */
		0,																		/* 0x0000 016C Reserved */
		0,																		/* 0x0000 0170 Reserved */
		0,																		/* 0x0000 0174 Reserved */
		0,																		/* 0x0000 0178 Reserved */
		0,																		/* 0x0000 017C Reserved */
		0,																		/* 0x0000 0180 Reserved */
		(uint32_t)&FPU_handler,													/* 0x0000 0184 */
		0,																		/* 0x0000 0188 Reserved */
		0,																		/* 0x0000 018C Reserved */
		(uint32_t)&SPI4_handler,												/* 0X0000 0190*/
};


/* HANDLER FUNCTIONS */
void Default_handler(void)
{

  while(1);
}

/* Symbols defined in linkerscript file */
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

void Reset_handler(void)
{
  /* Copy .data from FLASH to SRAM */
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *flash_data = (uint8_t*)&_etext;
  uint8_t *sram_data = (uint8_t*) &_sdata;

  /* Copy data section from FLASH to SRAM */
  for (uint32_t i = 0; i < data_size; i++)
  {
    sram_data[i] = flash_data[i];
  }

  /* Zero-fill bss section in SRAM */
  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint32_t *bss = (uint32_t*) &_sbss;

  for (uint32_t i = 0; i < bss_size; i++)
  {
    bss[i] = 0;
  }

  main();
}
