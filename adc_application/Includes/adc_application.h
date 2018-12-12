/* Author:   Alex St. Clair
 * Filename: adc_application.h
 * Created:  12-11-18
 * Target:   FRDM-KL25Z
 *
 * This file defines functions for peak level monitoring of
 * the ADC readings
 */

#ifndef ADC_APPLICATION_H
#define ADC_APPLICATION_H

#include <stdbool.h>
#include <stdint.h>

/* slow decay */
#define SCALE_FACTOR	0.9

uint16_t peak_monitor(void);

#endif /* ADC_APPLICATION_H */
