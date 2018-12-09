/* Author:   Alex St. Clair
 * Filename: adc_driver.c
 * Created:  12-9-18
 * Target:   FRDM-KL25Z
 *
 * This file implements functions to implement a driver for the ADC
 * module in the MKL25Z128VLK4
 */

#include "adc_driver.h"
#include "MKL25Z4.h"

/* ADCK = Bus Clock = 24 MHz (given system_MKL25Z4.h clock configuration and ADC reset default)
 * ADCK cycle = 1 / 24 MHz ~= 42 ns
 *
 * After the first continuous conversion:
 * ConversionTime = AvgNum*(BCT + LSTAdder + HSCAdder)
 *    We don't need fast conversion and want long samples, so choose:
 *    AvgNum = 32
 *    BCT = 34 * ADCK cycles (16b differential)
 *    LSTAdder = 20 * ADCK cycles
 *    HSCAdder = 0 * ADCK cycles
 * ConversionTime = 32 * 54 * ADCK cycles = 72 us
 * ConversionFrequency = 1 / 72 us ~= 13.9 kHz > 8 kHz minimum
 */
void init_adc(void)
{
	/* Enable PORT E clock */
	SIM->SCGC5 |= (uint32_t) SIM_SCGC5_PORTE_MASK;

	/* Pin 13, PTE20, ADC0_DP0 (ALT0, default, analog) */
	PORTE->PCR[20] &= ~(PORT_PCR_MUX_MASK);

	/* Pin 14, PTE21, ADC0_DM0 (ALT0, default, analog) */
	PORTE->PCR[21] &= ~(PORT_PCR_MUX_MASK);

	/* enable clock to ADC0 */
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	/* CFG1, note defaults to BUS_CLOCK / 1 */
	ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK    /* enable long samples */
			    | ADC_CFG1_MODE_MASK;     /* 16-bit resolution */

	/* CFG2, note defaults to LSTAdded = 20 */
	/* SC2, note defaults to, DMA disabled, compare disabled, software trigger, external reference pins */

	/* SC3 */
	ADC0->SC3 |= ADC_SC3_ADCO_MASK        /* enable continuous mode */
			   | ADC_SC3_AVGE_MASK        /* enable hardware averaging */
			   | ADC_SC3_AVGS_MASK;       /* 32 sample averaging */

	/* SC1, note defaults to interrupt disabled */
	ADC0->SC1[0] |= ADC_SC1_DIFF_MASK;    /* enable differential mode */
	ADC0->SC1[0] &= ~(ADC_SC1_ADCH_MASK); /* select channel 0 and enable ADC */
}

int16_t read_adc_blocking(void)
{
	int16_t result = 0;

	while (ADC0->SC1[0] & ADC_SC1_COCO_MASK == 0); /* wait for conversion to complete */

	result = (int16_t) (ADC0->R[0] & 0xFFFF);

	return result;
}
