/**
 ******************************************************************************
 * @file    micro.h
 * @author  Bellino Francesco
 * @brief   Microcontroller Abstraction Layer for STM32F401RE uC.
 *
 *
 ******************************************************************************
 */

#ifndef DRIVERS_STM32F401RE_MICRO_H_
#define DRIVERS_STM32F401RE_MICRO_H_


/**	DEFINE **/
#define PERIPHERAL_BASE 	(0x40000000U)											/* Start peripheral address */
#define AHB1_BASE 			(PERIPHERAL_BASE + 0x20000U)							/* AHB1 address */
#define GPIOA_BASE 			(AHB1_BASE + 0x0U)										/* GPIOA base address */
#define RCC_BASE 			(AHB1_BASE + 0x3800U)									/* RCC base address */


#define RCC_AHB1ENR_OFFSET 	(0x30U)													/* RCC enable register offset from RCC base */
#define RCC_AHB1ENR 		((volatile uint32_t*) (RCC_BASE + RCC_AHB1ENR_OFFSET))	/* RCC enable register  */
#define RCC_AHB1ENR_GPIOAEN (0x00U)													/* GPIOA enable bit of RCC enable register */

#define GPIOA_PIN			(0U)													/* GPIOA Pin used */
#define GPIO_MODER_OFFSET 	(0x00U)													/* GPIOA moder offset from GPIOA base*/
#define GPIOA_MODER 		((volatile uint32_t*) (GPIOA_BASE + GPIO_MODER_OFFSET))	/* GPIOA moder register */
#define GPIO_MODER_MODER 	(GPIOA_PIN << 1)										/* Offset for GPIOA Pin */
#define GPIO_ODR_OFFSET 	(0x14U)													/* GPIO Output Data Register offset */
#define GPIOA_ODR 			((volatile uint32_t*) (GPIOA_BASE + GPIO_ODR_OFFSET))	/* GPIOA Output Data Register pointer */

#define INIT_DELAY 10																/* Loop delay at start */

/**	FUNCTIONS **/
void micro_init(void);
void micro_blink(void);

#endif /* DRIVERS_STM32F401RE_MICRO_H_ */
