/** 
 * @file uart_message_atmega.c
 * @brief Implementations for message protocol
 *  
 * This library is used to create Data Link Layer for existed Physical Layers,
 * such as UART, SPI, I2C,...
 *
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2019 Dec 28
 */


#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart_message.h"
#include "uart.h"


static void createPacket(const void*, uint8_t, uint8_t, const void*, uint8_t);

static void parsePreamble(void);
static void parseAddress(void);
static void parseSize(void);
static void parsePayload(void);
static void parseChecksum(void);


typedef void (*callbacktype)(void);
static callbacktype callback[] = {	parsePreamble, 
									parseAddress, 
									parseSize, 
									parsePayload, 
									parseChecksum };

volatile steps currentStep = parsingPreamble;

const uint8_t required_preamble[MESSAGE_PREAMBLE_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD};

static volatile MessagePacket rxPacket;
static MessagePacket txPacket;


volatile MessagePacket* uart_message_create(uint32_t baudrate) {
	
	uart_open(baudrate);
	crc32_init();
	sei();

	return &rxPacket;
}


static void createPacket(	const void* _preamble, 
							uint8_t des, 
							uint8_t src, 
							const void* _data, 
							uint8_t len) 
{
	uint8_t* preamble = (uint8_t*)_preamble;
	uint8_t* data = (uint8_t*)_data;

	// PREAMBLE
	for (uint8_t i = 0; i < MESSAGE_PREAMBLE_SIZE; i++) {
		txPacket.preamble[i] = preamble[i];
	}

	// ADDRESS
	txPacket.address[0] = des;
	txPacket.address[1] = src;

	// PAYLOAD SIZE
	txPacket.payloadSize = (len > MESSAGE_MAX_PAYLOAD_SIZE) ? MESSAGE_MAX_PAYLOAD_SIZE : len;

	// PAYLOAD 
	for (uint8_t i = 0; i < txPacket.payloadSize; i++) {
		txPacket.payload[i] = data[i];
	}

	// CHECKSUM CRC32
	crc32_t checksum = crc32_compute(&txPacket, sizeof(txPacket.preamble) 
											+ sizeof(txPacket.address) 
											+ sizeof(txPacket.payloadSize)
											+ txPacket.payloadSize);

	txPacket.checksum = checksum;
}


void uart_message_send(	const void* _preamble, 
						uint8_t des, 
						uint8_t src, 
						const void* _data, 
						uint8_t len) 
{
	createPacket(_preamble, des, src, _data, len);

	uart_writeBuffer(txPacket.preamble, 4);
	uart_writeBuffer(txPacket.address, 2);
	uart_write(txPacket.payloadSize);
	uart_writeBuffer(txPacket.payload, txPacket.payloadSize);
	uart_writeBuffer(&txPacket.checksum, sizeof(crc32_t));
	_delay_ms(5);
}


static void parsePreamble() {
	if (currentStep == parsingPreamble) {
		static int counter;

		rxPacket.preamble[counter] = UDR0;

		if (rxPacket.preamble[counter] == required_preamble[counter]) {
			counter++;
		}
		else {
			counter = 0;
		}

		// go to next currentStep if 4-byte preamble is read.
		if (counter == MESSAGE_PREAMBLE_SIZE) {
			counter = 0;
			currentStep++;
		}
	}
}


static void parseAddress() {
	if (currentStep == parsingAddress) {
		static int counter;

		rxPacket.address[counter++] = UDR0;

		// go to next currentStep if 2-byte address is read.
		if (counter == 2) {
			counter = 0;
			currentStep++;
		}
	}
}


static void parseSize() {
	if (currentStep == parsingSize) {
		rxPacket.payloadSize = UDR0;
		currentStep++;
	}
}


static void parsePayload() {
	if (currentStep == parsingPayload) {
		static int counter;

		rxPacket.payload[counter++] = UDR0;

		if (counter == rxPacket.payloadSize || counter == MESSAGE_MAX_PAYLOAD_SIZE) {
			counter = 0;
			currentStep++;
		}
	}
}


static void parseChecksum() {
	if (currentStep == parsingChecksum) {
		static int counter;

		*((uint8_t*)(&rxPacket.checksum)+counter) = UDR0;
		counter++;

		if (counter == sizeof(crc32_t)) {
			counter = 0;
			currentStep = idle;
		}
	}
}


int uart_message_verifyChecksum() {
	if (currentStep == idle) {
		/*crc32_t ret = crc32_compute(&rxPacket, sizeof(rxPacket.preamble) 
											+ sizeof(rxPacket.address) 
											+ sizeof(rxPacket.payloadSize)
											+ rxPacket.payloadSize);*/

		crc32_t ret = crc32_compute(rxPacket.payload, rxPacket.payloadSize);

		if (ret == rxPacket.checksum) {
			return 0;
		}
		else {
			return -1;
		}
	}
	return -1;
}


ISR(USART_RX_vect) {
	if (currentStep < idle) {
		callback[currentStep]();
	}
}