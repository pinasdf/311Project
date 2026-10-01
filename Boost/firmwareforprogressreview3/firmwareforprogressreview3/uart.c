#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

void uart_init(void)
{
	uint16_t ubrr = (uint16_t)((F_CPU / (16UL * UART_BAUD)) - 1UL);
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1 << TXEN0);                        // transmit only
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8 data bits, no parity, 1 stop bit
}

void uart_putc(char c)
{
	while (!(UCSR0A & (1 << UDRE0))) { }
	UDR0 = c;
}

void uart_print(const char *s)
{
	while (*s) uart_putc(*s++);
}

void uart_print_int(int16_t value)
{
	char buf[8];
	itoa(value, buf, 10);
	uart_print(buf);
}
