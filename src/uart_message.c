#include <stdio.h>
#include <avr/interrupt.h>
#include "message.h"
#include "uart.h"


static void parsePreamble(void);
static void parseAddress(void);
static void parseLoadSize(void);
static void parsePayload(void);

typedef void (*callbacktype)(void);
static callbacktype callback[4] = {parsePreamble, parseAddress, parseLoadSize, parsePayload};

static volatile uint8_t *data;
volatile uint8_t step = 0;
const uint8_t required_preamble[4] = {0xAA, 0xBB, 0xCC, 0xDD};
static MessagePacket packet;


MessagePacket* message_init(volatile uint8_t *reg) {
	data = reg;

	return &packet;
}


static void parsePreamble() {
	if (step == 0) {
		static int counter;

		packet.preamble[counter] = *data;

		if (packet.preamble[counter] == required_preamble[counter]) {
			counter++;
		}
		else {
			counter = 0;
		}

		// go to next step if 4-byte preamble is read.
		if (counter == 4) {
			counter = 0;
			step = 1;
		}
	}
}


static void parseAddress() {
	if (step == 1) {
		static int counter;

		packet.address[counter++] = *data;

		// go to next step if 2-byte address is read.
		if (counter == 2) {
			counter = 0;
			step = 2;
		}
	}
}


static void parseLoadSize() {
	if (step == 2) {
		packet.payloadSize = *data;
		step = 3;
	}
}


static void parsePayload() {
	if (step == 3) {
		static int counter;

		packet.payload[counter++] = *data;

		if (counter == packet.payloadSize) {
			counter = 0;
			step = 4;
		}
	}
}



ISR(USART_RX_vect) {
	if (step < 4) {
		callback[step]();
	}
}