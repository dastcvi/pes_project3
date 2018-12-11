/* Author:   Alex St. Clair
 * Filename: main.c
 * Created:  11-25-18
 * Target:   FRDM-KL25Z
 *
 * This is the main file for the embedded uart application
 * and driver demos for ECEN 5813 Project 2.
 *
 * Updating the defines will change
 */

#include "uart_driver_nonblocking.h"
#include "adc_driver.h"
#include "dma_driver.h"
#include "MKL25Z4.h"
#include <stdint.h>
#include <stdio.h>

/* choose a run configuration */
//#define PART_2_ADC
//#define PART_3_DMA
#define PART_4_IRQ
//#define PART_5_APP

#define RED_LED_PINB		18	/* red LED is on PB18 */

static char user_char = '\0';

void red_led_init(void)
{
	/* Enable PORTB clock */
	SIM->SCGC5 |= (uint32_t) SIM_SCGC5_PORTB_MASK;

	/* mux */
	PORTB->PCR[RED_LED_PINB] |= (uint32_t) 1 << PORT_PCR_MUX_SHIFT;

	/* set as output */
	GPIOB->PDDR |= (uint32_t) 1 << RED_LED_PINB;

	/* start high (LED off) */
	GPIOB->PSOR |= (uint32_t) 1 << RED_LED_PINB;
}

void red_led_toggle(void)
{
	GPIOB->PTOR |= (uint32_t) 1 << RED_LED_PINB;
}

int main(void)
{
 	red_led_init();
	init_uart_nonblocking();

#ifdef PART_2_ADC
	char adc_string[16] = "";
	int16_t adc_result = 0;
	init_adc(false);

	while (1)
	{
		adc_result = read_adc_blocking();
		sprintf(adc_string, "%d\n\r", adc_result);
		tx_string(adc_string, 16);
	}
#endif

#ifdef PART_3_DMA
	init_dma(false);
	init_adc(true);

	while(1);
#endif

#ifdef PART_4_IRQ
	init_dma(true);
	init_adc(true);

	while(1);
#endif

    return 0;
}
