#ifndef __UART__
#define __UART__

#ifdef __cplusplus
extern "C" {
#endif


void uart_open(uint32_t);

void uart_write(uint8_t);
void uart_writeBuffer(const void*, uint32_t);

void uart_putchar(char c);
void uart_print(const char*);

uint8_t uart_read(void);
uint8_t* uart_readBuffer(uint32_t);
char uart_getchar();

uint8_t uart_flush(void);


#ifdef __cplusplus
}
#endif

#endif /* __UART__ */
