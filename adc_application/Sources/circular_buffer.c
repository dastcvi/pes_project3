/* Author:   Alex St. Clair
 * Filename: circular_buffer.c
 * Created:  11-23-18
 * Targets:  Linux, FRDM-KL25Z
 *
 * This file implements functions to implement a circular
 * buffer for use on the FRDM-KL25Z and to be tested in
 * Linux.
 *
 * This implementation has two extra features:
 *   1) ability to report the number of elements in the buffer
 *   2) does not reserve any unused space when full
 */

#include "circular_buffer.h"
#include <stdlib.h>

/* returns a newly allocated and initialized circular buffer that can
 * hold "length" elements.
 */
ring_t * ring_init(uint16_t length)
{
	ring_t * temp_ring = NULL;

	/* perform bounds checking first */
	if (length == 0 || length > MAX_RING) return NULL;

	/* attempt to allocate the struct */
	temp_ring = (ring_t *) malloc(sizeof(ring_t));
	if (temp_ring == NULL) return NULL;

	/* attempt to allocate the buffer */
	temp_ring->buffer = (char *) malloc(length);
	if (temp_ring->buffer == NULL)
	{
		free(temp_ring);
		return NULL;
	}

	/* set up the rest of the struct */
	temp_ring->length = length;
	temp_ring->in = 0;
	temp_ring->out = 0;
	temp_ring->num_elem = 0;

	return temp_ring;
}

/* deallocates the given circular buffer */
void ring_destroy(ring_t * ring)
{
	free(ring->buffer);
	free(ring);
}

/* adds the element "data" to the given circular buffer */
int ring_push(ring_t * ring, char data)
{
	/* check for full condition */
	if (ring->num_elem == ring->length) return 0;

	/* insert the data */
	ring->buffer[ring->in] = data;

	/* increment the index circularly */
	ring->in = (ring->in + 1) % ring->length;
	ring->num_elem++;

	return 1;
}

/* removes and returns the oldest element in the circular buffer */
int ring_pop(ring_t * ring, char * data)
{
	if (ring->num_elem == 0) return 0;

	/* get the data */
	*data = ring->buffer[ring->out];

	/* increment the index circularly */
	ring->out = (ring->out + 1) % ring->length;
	ring->num_elem--;

	return 1;
}

/* returns the number of elements in the ring buffer */
uint16_t ring_size(ring_t * ring)
{
	return ring->num_elem;
}
