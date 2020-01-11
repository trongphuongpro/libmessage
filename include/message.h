/** 
 * @file message.h
 * @brief Function prototypes for message protocol
 *  
 * This C library is used to create Data Link Layer for existed Physical Layers,
 * such as UART, SPI, I2C,...

 * This file contains the prototypes, struct, enum and extern variable
 * that you will need, in order to use this library.

 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date December 28, 2019 
 */


#ifndef __MESSAGE__
#define __MESSAGE__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "crc32.h"


/** 
 * @brief maximum payload size
 */		
#define MESSAGE_MAX_PAYLOAD_SIZE	32


/** 
 * @brief enum contains code for each step of transmitting/receiving procedure
 */  
typedef enum steps steps;


typedef struct MessageBox* MessageBox_t;
typedef struct Message* Message_t;


/** 
 * @brief Struct contains message payload
 */  
struct Message {
	uint8_t address; /**< @brief destination and source address: 2 bytes*/
	uint8_t payloadSize; /**< @brief size of payload: 1 byte */
	uint8_t *payload; /**< @brief payload */
} __attribute__((packed));


/** 
 * @brief global variable contains current state of procedure
 */
extern volatile steps currentStep;


/** 
 * @brief Create message box
 * 
 * Open UART bus, initialize crc32 checksum, enable interrupt.
 *
 * @param baudrate UART baudrate.
 * @param num max size of FIFO buffer.
 * @return pointer to received MessageFrame.
 */
MessageBox_t uart_messagebox_create(uint32_t baudrate, uint8_t num);


/** 
 * @brief Destroy message box
 * 
 * Free all allocated memory.
 *
 * @return nothing.
 */
void messagebox_destroy(void);


/** 
 * @brief Send message frame
 *
 * @param preamble UART baudrate.
 * @param destination Receiver's address.
 * @param source Transmitter's address.
 * @param payload message need to be sent.
 * @param len length of message. 
 * @return nothing.
 */
void messagebox_send(const void* preamble, 
						uint8_t destination, 
						uint8_t source, 
						const void* payload, 
						uint8_t len);


/** 
 * @brief Set valid preamble (4 bytes) for incoming frame
 *
 * @param b1 first byte.
 * @param b2 second byte.
 * @param b3 third byte.
 * @param b4 last byte.
 * @return nothing.
 */
void messagebox_setPreamble(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);

bool messagebox_isAvailable(MessageBox_t buffer);

int messagebox_pop(MessageBox_t buffer, Message_t data);

uint8_t messagebox_getCapacity(MessageBox_t buffer);


uint8_t messagebox_getUsedSpace(MessageBox_t buffer);


uint8_t messagebox_getFreeSpace(MessageBox_t buffer);

#ifdef __cplusplus
}
#endif

#endif