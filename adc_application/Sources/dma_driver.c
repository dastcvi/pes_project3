/* Author:   Alex St. Clair
 * Filename: dma_driver.c
 * Created:  12-11-18
 * Target:   FRDM-KL25Z
 *
 * This file implements functions to implement a driver for the DMA
 * module in the MKL25Z128VLK4. It also manages the DMA destination
 * buffer.
 */

#include "dma_driver.h"
#include "MKL25Z4.h"
#include <stdlib.h>

volatile uint16_t * adc_buffer = NULL;

static void alloc_buffer(void);

/* allocate an aligned 128-byte buffer */
static void alloc_buffer(void)
{
	uint32_t temp = 0;

	adc_buffer = (uint16_t *) malloc(256); /* 2 times what is needed */

	if (adc_buffer != NULL) {
		temp = (uint32_t) adc_buffer;

		temp = temp - (temp % 128) + 128; /* align the pointer with the first 128-bit aligned address */

		adc_buffer = (uint16_t *) temp;

		for (temp=0;temp<64;temp++) {
			adc_buffer[temp] = 0;
		}
	}
}

void init_dma(void)
{
	alloc_buffer();

	/* enable DMAMUX clock */
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* enable DMA clock */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;

	/* assign the destination to be the buffer located here */
	DMA0->DMA[0].DAR = (uint32_t) adc_buffer;

	/* assign the source to be the result register in the ADC */
	DMA0->DMA[0].SAR = (uint32_t) &(ADC0->R[0]);

	/* set up for 64 sample (128 byte) transfers */
	DMA0->DMA[0].DSR_BCR |= 128;

	/* enable DMA CH0 controls */
	DMA0->DMA[0].DCR |= ((uint32_t) DMA_SIZE_16 << DMA_DCR_SSIZE_SHIFT)   /* source 16 bits */
			         |  ((uint32_t) DMA_SIZE_16 << DMA_DCR_DSIZE_SHIFT)   /* destination 16 bits */
					 |   DMA_DCR_DINC_MASK                                /* increment destination pointer */
					 |  ((uint32_t) DMA_BUFFER_128 << DMA_DCR_DMOD_SHIFT) /* destination 128-byte buffer (64 samples) */
					 |   DMA_DCR_CS_MASK                                  /* one transfer per request */
					 |   DMA_DCR_D_REQ_MASK                               /* disable request once block has finished */
					 |   DMA_DCR_ERQ_MASK;                                /* enable peripheral request */

	/* set DMAMUX for ADC0 to CH0 and enable */
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK
			          |  ADC0_DMA_SOURCE;
}
