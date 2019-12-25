#ifndef __MESSAGE__
#define __MESSAGE__

#ifdef __cplusplus
extern "C" {
#endif

#include "crc32.h"

extern volatile uint8_t preamble[4];
extern volatile uint8_t address[2];
extern volatile uint8_t payloadSize;
extern volatile uint8_t payload[50];
extern volatile crc32_t checksum;
extern uint8_t step;

const uint8_t required_preamble[4] = {0xAA, 0xBB, 0xCC, 0xDD};


void message_init(volatile uint8_t*);

#ifdef __cplusplus
}
#endif

#endif