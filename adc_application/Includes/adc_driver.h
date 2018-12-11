/* Author:   Alex St. Clair
 * Filename: adc_driver.h
 * Created:  12-9-18
 * Target:   FRDM-KL25Z
 *
 * This file declares functions to implement a driver for the ADC
 * module in the MKL25Z128VLK4
 */

#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

/* ADCK = Bus Clock = 24 MHz (given system_MKL25Z4.h clock configuration)
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
void init_adc(bool dma_enable);

int16_t read_adc_blocking(void);

#endif /* ADC_DRIVER_H */
