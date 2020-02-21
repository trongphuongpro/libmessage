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


#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "message.h"
#include "messagebox.h"
#include "uart.h"
#include "crc32.h"


enum steps {	parsingPreamble=0,
				parsingAddress,
				parsingSize,
				parsingPayload,
				parsingChecksum,
				verifyingChecksum
};


/** 
 * @brief Struct contains message frame
 */  
typedef struct MessageFrame {
	uint8_t preamble[MESSAGE_PREAMBLE_SIZE]; /**< @brief preamble of message frame */
	uint8_t address[2]; /**< @brief destination and source address: 2 bytes*/
	uint8_t payloadSize; /**< @brief size of payload: 1 byte */
	uint8_t payload[MESSAGE_MAX_PAYLOAD_SIZE]; /**< @brief payload */
	crc32_t checksum; /**< @brief CRC-32 checksum: 4 bytes */
} __attribute__((packed)) MessageFrame;

typedef MessageFrame* MessageFrame_t;


static void createFrame(const void*, uint8_t, uint8_t, const void*, uint8_t);
static void parsePreamble(void);
static void parseAddress(void);
static void parseSize(void);
static void parsePayload(void);
static void parseChecksum(void);
static int verifyChecksum(void);

static Message extractMessage(MessageFrame_t);


typedef void (*callbacktype)(void);
static callbacktype callback[] = {	parsePreamble, 
									parseAddress, 
									parseSize, 
									parsePayload, 
									parseChecksum };

volatile steps currentStep = parsingPreamble;

static uint8_t validPreamble[MESSAGE_PREAMBLE_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD};

static MessageFrame rxFrame;
static MessageFrame txFrame;
static MessageBox_t messageBuffer;


void uart_messagebox_create(uint32_t baudrate, 
							MessageBox_t box, 
							Message_t data,
							uint8_t num) {
	
	uart_open(baudrate);
	sei();

	messageBuffer = box;
	messagebox_create(messageBuffer, data, num);
}


void message_setPreamble(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
	validPreamble[0] = b1;
	validPreamble[1] = b2;
	validPreamble[2] = b3;
	validPreamble[3] = b4;
}


static void createFrame(	const void* _preamble, 
							uint8_t des, 
							uint8_t src, 
							const void* _data, 
							uint8_t len) 
{
	uint8_t* preamble = (uint8_t*)_preamble;
	uint8_t* data = (uint8_t*)_data;

	// PREAMBLE
	for (uint8_t i = 0; i < MESSAGE_PREAMBLE_SIZE; i++) {
		txFrame.preamble[i] = preamble[i];
	}

	// ADDRESS
	txFrame.address[0] = des;
	txFrame.address[1] = src;

	// PAYLOAD SIZE
	txFrame.payloadSize = (len > MESSAGE_MAX_PAYLOAD_SIZE) ? 
							MESSAGE_MAX_PAYLOAD_SIZE : len;

	// PAYLOAD
	for (uint8_t i = 0; i < txFrame.payloadSize; i++) {
		txFrame.payload[i] = data[i];
	}

	// CHECKSUM CRC32
	crc32_t checksum = crc32_concat(crc32_compute(&txFrame, 
											sizeof(txFrame.preamble) 
											+ sizeof(txFrame.address) 
											+ sizeof(txFrame.payloadSize)),
								txFrame.payload, txFrame.payloadSize);

	txFrame.checksum = checksum;
}


void message_send(	const void* _preamble, 
						uint8_t des, 
						uint8_t src, 
						const void* _data, 
						uint8_t len) 
{
	createFrame(_preamble, des, src, _data, len);

	uart_sendBuffer(txFrame.preamble, 4);
	uart_sendBuffer(txFrame.address, 2);
	uart_send(txFrame.payloadSize);
	uart_sendBuffer(txFrame.payload, txFrame.payloadSize);
	uart_sendBuffer(&txFrame.checksum, sizeof(crc32_t));

	_delay_ms(5);
}


static void parsePreamble() {
	if (currentStep == parsingPreamble) {
		static int counter;

		rxFrame.preamble[counter] = UDR0;

		if (rxFrame.preamble[counter] == validPreamble[counter]) {
			counter++;
		}
		else {
			counter = 0;
		}

		// go to next currentStep if 4-byte preamble is read.
		if (counter == MESSAGE_PREAMBLE_SIZE) {
			counter = 0;
			currentStep = parsingAddress;
		}
	}
}


static void parseAddress() {
	if (currentStep == parsingAddress) {
		static int counter;

		rxFrame.address[counter++] = UDR0;

		// go to next currentStep if 2-byte address is read.
		if (counter == 2) {
			counter = 0;
			currentStep = parsingSize;
		}
	}
}


static void parseSize() {
	if (currentStep == parsingSize) {
		rxFrame.payloadSize = UDR0;

		if (rxFrame.payloadSize > MESSAGE_MAX_PAYLOAD_SIZE) {
			rxFrame.payloadSize = MESSAGE_MAX_PAYLOAD_SIZE;
		}

		currentStep = parsingPayload;
	}
}


static void parsePayload() {
	if (currentStep == parsingPayload) {
		static int counter;

		rxFrame.payload[counter++] = UDR0;

		if (counter == rxFrame.payloadSize) {
			counter = 0;
			currentStep = parsingChecksum;
		}
	}
}


static void parseChecksum() {
	if (currentStep == parsingChecksum) {
		static int counter;

		((uint8_t*)&rxFrame.checksum)[counter++] = UDR0;

		if (counter == sizeof(crc32_t)) {
			counter = 0;
			currentStep = verifyingChecksum;

			if (verifyChecksum() == 0) {
				if (!messagebox_isFull(messageBuffer)) {
					messagebox_push(messageBuffer, extractMessage(&rxFrame));
				}
			}

			currentStep = parsingPreamble;
		}
	}
}


int verifyChecksum() {
	crc32_t ret = crc32_concat(crc32_compute(&rxFrame, 
											sizeof(rxFrame.preamble) 
											+ sizeof(rxFrame.address) 
											+ sizeof(rxFrame.payloadSize)),
								rxFrame.payload, rxFrame.payloadSize);

	if (ret == rxFrame.checksum) {
		return 0;
	}
	else {
		return -1;
	}
}


Message extractMessage(MessageFrame_t frame) {
	Message message;

	message.address = frame->address[1];
	message.payloadSize = frame->payloadSize;

	memcpy(message.payload, frame->payload, message.payloadSize);

	return message;
}


ISR(USART_RX_vect) {
	if (currentStep < verifyingChecksum) {
		callback[currentStep]();
	}
}