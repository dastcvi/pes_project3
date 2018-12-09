/* Author:   Alex St. Clair
 * Filename: uart_driver_nonblocking.h
 * Created:  11-28-18
 * Target:   FRDM-KL25Z
 *
 * This file declares functions to implement a nonblocking driver for the
 * UART module in the MKL25Z128VLK4
 */

#ifndef UART_DRIVER_NONBLOCKING_H
#define UART_DRIVER_NONBLOCKING_H

#include <stdbool.h>
#include <stdint.h>

/* determine whether or not this driver echos chars as they're received */
#define ENABLE_ECHO

#define PORT_ALT_2		2
#define UART_CLK_PLL	1

#define TX_BUFFER_SIZE	128
#define RX_BUFFER_SIZE	1

extern bool rx_overrun;

/* initialize the uart in nonblocking mode */
void init_uart_nonblocking(void);

/* transmits a char assuming the transmitter is available */
void tx_char(char data);

/* reads back the data from the receiver */
char rx_char(void);

/* transmit a char by pushing it onto the tx buffer and enabling the tx interrupt */
bool tx_nonblocking(char data);

/* check if a character is ready to be received it, and grab it if so */
bool rx_nonblocking(char * data);

/* transmits a string via TX buffer, will block if the buffer is full */
/* the array size is the size of the *data array, not necessarily the number of elements */
/* the *data array should be null terminated */
void tx_string(char * data, uint8_t array_size);

/* function called by the UART0 ISR for tx */
void isr_tx(void);

/* function called by the UART0 ISR upon rx */
void isr_rx(void);

#endif
