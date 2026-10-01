#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "config.h"

#define UART_BAUD   9600UL     // set the Proteus virtual terminal to the same baud rate


void uart_init(void);
void uart_putc(char c);
void uart_print(const char *s);
void uart_print_int(int16_t value);

#endif
