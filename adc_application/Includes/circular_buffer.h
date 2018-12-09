/* Author:   Alex St. Clair
 * Filename: circular_buffer.h
 * Created:  11-23-18
 * Targets:  Linux, FRDM-KL25Z
 *
 * This file declares functions to implement a circular
 * buffer for use on the FRDM-KL25Z and to be tested in
 * Linux.
 *
 * This implementation has two extra features:
 *   1) ability to report the number of elements in the buffer
 *   2) does not reserve any unused space when full
 */
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>

#define MAX_RING	2048

typedef struct
{
	char * buffer;
	uint16_t length;
	uint16_t in;
	uint16_t out;
	uint16_t num_elem;
} ring_t;

/* returns a newly allocated and initialized circular buffer that can
 * hold "length" elements.
 */
ring_t * ring_init(uint16_t length);

/* deallocates the given circular buffer */
void ring_destroy(ring_t * ring);

/* adds the element "data" to the given circular buffer and returns the status */
int ring_push(ring_t * ring, char data);

/* removes and the oldest element in the circular buffer and returns the status */
int ring_pop(ring_t * ring, char * data);

/* returns the number of elements in the ring buffer */
uint16_t ring_size(ring_t * ring);

#endif
