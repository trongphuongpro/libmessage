/** 
 * @file uart.h
 * @brief Function prototypes for UART communication protocol
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2019 Dec 28
 */


#ifndef __UART__
#define __UART__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initialize UART bus with baudrate
 *
 * Enable UART RX interrupt
 *
 * @param baudrate UART baudrate.
 * @return nothing.
 */
void uart_open(uint32_t baudrate);


/**
 * @brief transmit one byte via UART bus
 * @param data one byte data.
 * @return nothing.
 */
void uart_send(uint8_t data);


/**
 * @brief transmit a byte array via UART bus
 * @param data pointer to data.
 * @param len the length of data in byte.
 * @return nothing.
 */
void uart_sendBuffer(const void* data, uint32_t len);


/**
 * @brief print one character to UART terminal
 *
 * character '\\n' or '\r' will be converted to '\r\\n'
 *
 * @param data one byte data.
 * @return nothing.
 */
void uart_putchar(char data);


/**
 * @brief print a string to UART terminal
 * 
 * @param string pointer to data.
 * @return nothing.
 */
void uart_print(const char* string);


/** 
 * @brief get one byte from UART bus
 * @return one byte.
 */
uint8_t uart_receive(void);


/** 
 * @brief get one byte from UART bus
 * @return one byte.
 */
char uart_getchar(void);


/** 
 * @brief flush the receive buffer
 * @return last byte in the receive buffer.
 */
void uart_flush(void);


#ifdef __cplusplus
}
#endif

#endif /* __UART__ */
