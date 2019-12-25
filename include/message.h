#ifndef __MESSAGE__
#define __MESSAGE__

#ifdef __cplusplus
extern "C" {
#endif

#include "crc32.h"

typedef struct {
	volatile uint8_t preamble[4];
	volatile uint8_t address[2];
	volatile uint8_t payloadSize;
	volatile uint8_t payload[50];
	volatile crc32_t checksum;
} MessagePacket;


extern volatile uint8_t step;

MessagePacket *message_init(volatile uint8_t*);

#ifdef __cplusplus
}
#endif

#endif