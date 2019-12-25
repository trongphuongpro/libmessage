#include <avr/interrupt.h>
#include "message.h"


static void parsePreamble();
static void parseAddress();
static void parseLoadSize();
static void parsePayload();

static void (*callback[4])() = {parsePreamble, parseAddress, parseLoadSize, parsePayload};
static volatile uint8_t *data;
uint8_t step = 0;

void message_init(volatile uint8_t *reg) {
	data = reg;
}


static void parsePreamble() {
	if (step == 0) {
		static int counter;

		preamble[counter] = *data;

		if (preamble[counter] == required_preamble[counter]) {
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

		address[counter++] = *data;

		// go to next step if 2-byte address is read.
		if (counter == 2) {
			counter = 0;
			step = 2;
		}
	}
}


static void parseLoadSize() {
	if (step == 2) {
		payloadSize = *data;
		step = 3;
	}
}


static void parsePayload() {
	if (step == 3) {
		static int counter;

		payload[counter++] = *data;

		if (counter == payloadSize) {
			counter = 0;
			step = 4;
		}
	}
}


#ifdef __MESSAGE__

ISR(USART_RX_vect) {

	if (step < 4) {
		callback[step]();
	}
}

#endif