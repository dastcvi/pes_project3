/* Author:   Alex St. Clair
 * Filename: adc_application.c
 * Created:  12-11-18
 * Target:   FRDM-KL25Z
 *
 * This file implements functions for peak level monitoring of
 * the ADC readings
 */

#include "adc_application.h"
#include "dma_driver.h"
#include <string.h>
#include <stdlib.h>

int16_t peak_buffer[64] = {0};
uint16_t last_peak = 0;

/* return the new peak */
uint16_t peak_monitor(void)
{
	uint8_t itr = 0;
	bool new_peak = false;

	/* copy the adc buffer from dma_driver for calculations */
	memcpy(peak_buffer, adc_buffer, 128);

	new_data = false;

	for (itr=0; itr<64; itr++)
	{
		if (abs(peak_buffer[itr]) > last_peak)
		{
			last_peak = abs(peak_buffer[itr]);
			new_peak = true;
		}
	}

	if (!new_peak)
	{
		last_peak = (uint16_t) (SCALE_FACTOR * (float) last_peak);
	}

	return last_peak;
}
