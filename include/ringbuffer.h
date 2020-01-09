/** 
 * @file ringbuffer.h
 * @brief Function prototypes for creating ring buffer for embedded systems
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date January 6, 2020
 */


#ifndef __RINGBUFFER__
#define __RINGBUFFER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief datatype for user's usage.
 */
typedef struct MessageBox* MessageBox_t;

typedef struct Message* Message_t;

/**
 * @brief create new ring buffer.
 * @param num max number of element in buffer.
 * @param size size of each element in buffer.
 * @return new ring buffer instance.
 */
MessageBox_t ringbuffer_create(uint8_t num);


/**
 * @brief Destroy a ring buffer.
 *
 * Free all allocated memory for this buffer.
 * @param buffer ring buffer instance.
 * @return nothing.
 */
void ringbuffer_destroy(MessageBox_t buffer);


/**
 * @brief reset existed ring buffer.
 * @param buffer ring buffer instance.
 * @return nothing.
 */
void ringbuffer_clear(MessageBox_t buffer);


/**
 * @brief check if ring buffer is empty
 * @param buffer ring buffer instance.
 * @return state of ring buffer.
 */
bool ringbuffer_isEmpty(MessageBox_t buffer);


/**
 * @brief check if ring buffer is full
 * @param buffer ring buffer instance.
 * @return state of ring buffer.
 */
bool ringbuffer_isFull(MessageBox_t buffer);


/**
 * @brief check the capacity of ring buffer
 * @param buffer ring buffer instance.
 * @return the capacity of buffer
 */
uint8_t ringbuffer_getCapacity(MessageBox_t buffer);


/**
 * @brief get the used space of ring buffer
 * @param buffer ring buffer instance.
 * @return the number of used space of buffer
 */
uint8_t ringbuffer_getUsedSpace(MessageBox_t buffer);


/**
 * @brief get the free space of ring buffer
 * @param buffer ring buffer instance.
 * @return the number of free space of buffer
 */
uint8_t ringbuffer_getFreeSpace(MessageBox_t buffer);


void ringbuffer_push(MessageBox_t buffer, Message_t data);
int ringbuffer_pop(MessageBox_t buffer, Message_t data);


#ifdef __cplusplus
}
#endif

#endif