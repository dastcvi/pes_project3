/* Author:   Alex St. Clair
 * Filename: dma_driver.h
 * Created:  12-11-18
 * Target:   FRDM-KL25Z
 *
 * This file declares functions to implement a driver for the DMA
 * module in the MKL25Z128VLK4. It also manages the DMA destination
 * buffer.
 */

#ifndef DMA_DRIVER_H
#define DMA_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#define ADC0_DMA_SOURCE	    40
#define DMA_SIZE_16         2
#define DMA_BUFFER_128      4

#define RED_LED_PINB		18	/* red LED is on PB18 */

void init_dma(bool irq);

#endif /* DMA_DRIVER_H */
