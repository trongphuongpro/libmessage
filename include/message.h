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
#include <stdint.h>

/** 
 * @brief maximum payload size
 */     
#define MESSAGE_MAX_PAYLOAD_SIZE    64


/** 
 * @brief massage preamble size
 */ 
#define MESSAGE_PREAMBLE_SIZE   4


/**
 * @brief Abstract datatype of struct MessageBox.
 *
 * User can use it w/o the knowledge about struct MessageBox.
 */
typedef struct MessageBox MessageBox_t;


/** 
 * @brief Struct contains message payload
 */  
typedef struct Message {
    uint8_t address; /**< @brief source address: 1 bytes*/
    uint8_t payloadSize; /**< @brief size of payload: 1 byte */
    uint8_t payload[MESSAGE_MAX_PAYLOAD_SIZE]; /**< @brief payload */
} __attribute__((packed)) Message_t;


/** 
 * @brief Create message box
 * 
 * Open UART bus, initialize crc32 checksum, enable interrupt.
 *
 * @param baudrate UART baudrate.
 * @param num max size of FIFO buffer.
 * @return pointer to received MessageFrame.
 */
void uart_messagebox_create(uint32_t baudrate, 
                            MessageBox_t *box, 
                            Message_t *data,
                            uint8_t num);


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
void message_send(const void* preamble, 
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
void message_setPreamble(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);


#ifdef __cplusplus
}
#endif

#endif