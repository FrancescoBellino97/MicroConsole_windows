/**
 ******************************************************************************
 * @file    micro.c
 * @author  Bellino Francesco
 * @brief   Microcontroller Abstraction Layer for STM32F401RE uC.
 *
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "micro.h"

/**
  * @brief  Setup the microcontroller system
  *         Initialize GPIOA AHB and Port mode
  * @param  None
  * @retval None
  */
void micro_init(void)
{
	*RCC_AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN);

	/* do a small for loop after enabling the peripheral clock, as per the errata */
	for(uint8_t i=0; i<INIT_DELAY; i++);

	/* Set GPIO Pin as output */
	*GPIOA_MODER |= (1 << GPIO_MODER_MODER);
}


/**
  * @brief  Toggle the LED PIN
  * @param  None
  * @retval None
  */
void micro_blink(void){
	/* Invert Pin output */
	*GPIOA_ODR ^= (1 << GPIOA_PIN);
}
