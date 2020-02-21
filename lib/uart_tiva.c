/** 
 * @file uart_tiva.c
 * @brief Functions for UART communication protocol Tiva C.
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2019 Dec 28
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "driverlib/uart.h"
#include "uart.h"


static uint32_t UARTbase;

void uart_open(uint32_t base) {
	UARTbase = base;
}


void uart_send(uint8_t data) {
	UARTCharPut(UARTbase, data);
}


void uart_sendBuffer(const void* buffer, uint32_t len) {
	const uint8_t *data = (uint8_t*)buffer;

	for (uint32_t i = 0; i < len; i++) {
		UARTCharPut(UARTbase, data[i]);
	}
}


void uart_putchar(char c) {
	if (c == '\n') {
		UARTCharPut(UARTbase, '\r');
	}

	UARTCharPut(UARTbase, c);

	if (c == '\r') {
		UARTCharPut(UARTbase, '\n');
	}
}


void uart_print(const char* string) {
	for (uint32_t i = 0; i < strlen(string); i++) {
		uart_putchar(string[i]);
	}
}


uint8_t uart_receive(void) {
	return UARTCharGet(UARTbase);
}


void uart_flush(void) {
	while (UARTCharsAvail(UARTbase)) {
		UARTCharGetNonBlocking(UARTbase);
	}
}