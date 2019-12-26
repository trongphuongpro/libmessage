#ifndef __MESSAGE__
#define __MESSAGE__

#ifdef __cplusplus
extern "C" {
#endif

#include "crc32.h"
	
#define MESSAGE_PREAMBLE_SIZE		4
#define MESSAGE_MAX_PAYLOAD_SIZE	100

typedef struct {
	uint8_t preamble[MESSAGE_PREAMBLE_SIZE];
	uint8_t address[2];
	uint8_t payloadSize;
	uint8_t payload[MESSAGE_MAX_PAYLOAD_SIZE];
	crc32_t checksum;
} __attribute__((__packed__)) MessagePacket;


typedef enum {	parsingPreamble=0, 
				parsingAddress, 
				parsingSize, 
				parsingPayload, 
				parsingChecksum,
				idle } steps;

extern volatile steps currentStep;

volatile MessagePacket* uart_message_create(uint32_t);
void uart_message_send(const void*, uint8_t, uint8_t, const void*, uint8_t);
int uart_message_verifyChecksum(void);

#ifdef __cplusplus
}
#endif

#endif