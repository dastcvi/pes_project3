/* Author:   Alex St. Clair
 * Filename: uart_driver_nonblocking.c
 * Created:  11-28-18
 * Target:   FRDM-KL25Z
 *
 * This file implements functions to implement a nonblocking driver for the
 * UART module in the MKL25Z128VLK4
 */

#include "uart_driver_nonblocking.h"
#include "circular_buffer.h"
#include "MKL25Z4.h"

ring_t * rx_buffer;
ring_t * tx_buffer;

bool rx_overrun = false;

void init_uart_nonblocking(void)
{
	/* initialize circular buffers for tx and rx */
	tx_buffer = ring_init(TX_BUFFER_SIZE);
	rx_buffer = ring_init(RX_BUFFER_SIZE);

	/* Enable PORTA clock */
	SIM->SCGC5 |= (uint32_t) SIM_SCGC5_PORTA_MASK;

	/* UART0_RX: PTA1, ALT2 */
	PORTA->PCR[1] |= (uint32_t) PORT_ALT_2 << PORT_PCR_MUX_SHIFT;

	/* UART0_TX: PTA2, ALT2 */
	PORTA->PCR[2] |= (uint32_t) PORT_ALT_2 << PORT_PCR_MUX_SHIFT;

	/* set UART0 clock to MCGPLLCLK (48 MHz) */
	SIM->SOPT2 |= (uint32_t) UART_CLK_PLL << SIM_SOPT2_UART0SRC_SHIFT;

	/* enable MCGPLLCLK */
	MCG->C5 |= (uint8_t) 1 << MCG_C5_PLLCLKEN0_SHIFT;

	/* enable UART0 clock */
	SIM->SCGC4 |= (uint32_t) 1 << SIM_SCGC4_UART0_SHIFT;

	/* ensure receiver and transmitter not enabled */
	UART0->C2 &= ~((uint8_t) (UART_C2_TE_MASK | UART_C2_RE_MASK));

	/* baud_prescale = baud_clock / (OSR * baud)
	 * OSR = 16 (reset default)
	 * baud_clock = MCGPLLCLK / 2 = 48 MHz (startup code initializes MCGPLLCLK)
	 * for baud = 115200, then baud_prescale = 26.04
	 */
	UART0->BDH &= ~((uint8_t) UART_BDH_SBR_MASK); /* clear upper bits since 26 < 0xFF */
	UART0->BDL = (uint8_t) 26;

	/* disable the transmit data register empty interrupt (enabled when there are chars to transmit) */
	UART0->C2 &= (uint8_t) ~(1 << UART_C2_TIE_SHIFT);

	/* enable the receive data register full interrupt */
	UART0->C2 |= (uint8_t) 1 << UART_C2_RIE_SHIFT;

	/* module defaults to 8-bit, no parity, one stop bit */
	/* enable receiver and transmitter */
	UART0->C2 |= (uint8_t) (UART_C2_TE_MASK | UART_C2_RE_MASK);

	/* enable the general UART0 IRQ */
	NVIC_EnableIRQ(UART0_IRQn);

	/* enable interrupts */
	__enable_irq();
}

/* transmits a char assuming the transmitter is available */
void tx_char(char data)
{
	UART0->D = (uint8_t) data;
}

/* reads back the data from the receiver */
char rx_char(void)
{
	char data = (char) UART0->D;

	return data;
}

/* transmit a char by pushing it onto the tx buffer and enabling the tx interrupt */
bool tx_nonblocking(char data)
{
	/* take the coarse/naive approach to critical section protection */
	__disable_irq();

	/* attempt to add the char to the buffer */
	if (!ring_push(tx_buffer, data))
	{
		__enable_irq();
		return false;
	}

	/* enable the transmit data register empty interrupt */
	UART0->C2 |= (uint8_t) 1 << UART_C2_TIE_SHIFT;

	__enable_irq();
	return true;
}

/* check if a character is ready to be received, and grab it if so */
bool rx_nonblocking(char * data)
{
	bool available = false;

	__disable_irq();

	available = ring_pop(rx_buffer, data);

	__enable_irq();

	return available;
}

/* function called by the UART0 ISR for tx */
void isr_tx(void)
{
	char temp_char = '\0';

	/* take the coarse/naive approach to critical section protection */
	__disable_irq();

	if (ring_pop(tx_buffer, &temp_char))
	{
		tx_char(temp_char);
	}

	if(0 == ring_size(tx_buffer))
	{
		/* disable the transmit data register empty interrupt (enabled when there are chars to transmit) */
		UART0->C2 &= (uint8_t) ~(1 << UART_C2_TIE_SHIFT);
	}

	__enable_irq();
}

/* function called by the UART0 ISR upon rx */
void isr_rx(void)
{
	char temp_char = '\0';

	temp_char = rx_char();

	__disable_irq();

	if (!ring_push(rx_buffer, temp_char))
	{
		rx_overrun = true;
	}

	__enable_irq();
}

/* IRQ handler for all UART0 interrupts */
void UART0_IRQHandler(void)
{
	uint8_t uart_status = UART0->S1;

	/* handle the transmit data register empty interrupt (should only be enabled when there are chars to tx) */
	if (uart_status & UART_S1_TDRE_MASK)
	{
		isr_tx();
	}

	/* handle the receive data register full interrupt (always enabled) */
	if (uart_status & UART_S1_RDRF_MASK)
	{
		isr_rx();
	}
}
