/** 
 * @file ringbuffer.c
 * @brief Implementation for creating ring buffer for embedded systems
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date January 6, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ringbuffer.h"
#include "message.h"


/** 
 * @brief Struct contains FIFO buffer containing received messages.
 */ 
typedef struct MessageBox {
	Message_t *data; /**< @brief Array of pointers to messages */
	uint8_t readPoint; /**< @brief Reading point */
	uint8_t writePoint; /**< @brief Writting point */
	uint8_t capacity; /**< @brief The capacity of FIFO buffer */
	bool isFull; /**< @brief State of buffer, full or not */
} __attribute__((packed)) MessageBox;


MessageBox_t ringbuffer_create(uint8_t num) {
	assert(num);

	MessageBox_t newbuffer = (MessageBox_t)malloc(sizeof(MessageBox));
	assert(newbuffer);

	newbuffer->data = calloc(num, sizeof(Message_t*));
	assert(newbuffer->data);

	newbuffer->capacity = num;
	newbuffer->readPoint = 0;
	newbuffer->writePoint = 0;
	newbuffer->isFull = false;

	assert(ringbuffer_isEmpty(newbuffer));

	return newbuffer;
}


void ringbuffer_destroy(MessageBox_t buffer) {
	assert(buffer);

	ringbuffer_clear(buffer);

	free(buffer->data);
	free(buffer);
}


void ringbuffer_clear(MessageBox_t buffer) {
	Message dump;
	dump.payload = calloc(MESSAGE_MAX_PAYLOAD_SIZE, 1);

	// Remove all data until empty
	while (!ringbuffer_isEmpty(buffer)) {
		ringbuffer_pop(buffer, &dump);
	}

	free(dump.payload);
}


bool ringbuffer_isEmpty(MessageBox_t buffer) {
	assert(buffer);

	return (!buffer->isFull && (buffer->readPoint == buffer->writePoint));
}


bool ringbuffer_isFull(MessageBox_t buffer) {
	assert(buffer);

	return buffer->isFull;
}


uint8_t ringbuffer_getCapacity(MessageBox_t buffer) {
	assert(buffer);

	return buffer->capacity;
}


uint8_t ringbuffer_getUsedSpace(MessageBox_t buffer) {
	assert(buffer);

	uint8_t number = buffer->capacity;

	if (!buffer->isFull) {
		if (buffer->readPoint <= buffer->writePoint) {
			number = buffer->writePoint - buffer->readPoint;
		}
		else {
			number -= buffer->readPoint - buffer->writePoint;
		}
	}

	return number;
}


uint8_t ringbuffer_getFreeSpace(MessageBox_t buffer) {
	return ringbuffer_getCapacity(buffer) - ringbuffer_getUsedSpace(buffer);
}


/*void increase_checkpoints(MessageBox_t buffer) {
	assert(buffer);

	if (buffer->isFull) {
		buffer->readPoint = (buffer->readPoint + 1) % buffer->capacity;
	}

	buffer->writePoint = (buffer->writePoint + 1) % buffer->capacity;
	buffer->isFull = (buffer->readPoint == buffer->writePoint);
}


void decrease_checkpoints(MessageBox_t buffer) {
	assert(buffer);

	buffer->readPoint = (buffer->readPoint + 1) % buffer->capacity;
	buffer->isFull = false;
}*/


void ringbuffer_push(MessageBox_t buffer, Message_t data) {
	assert(buffer && buffer->data);

	if (!buffer->isFull) {
		buffer->data[buffer->writePoint] = data;

		buffer->writePoint = (buffer->writePoint + 1) % buffer->capacity;
		buffer->isFull = (buffer->readPoint == buffer->writePoint);
	}
}


int ringbuffer_pop(MessageBox_t buffer, Message_t data) {
	assert(buffer && buffer->data && data);

	int ret = -1;

	if (!ringbuffer_isEmpty(buffer)) {
		data->address = buffer->data[buffer->readPoint]->address;
		data->payloadSize = buffer->data[buffer->readPoint]->payloadSize;
		memcpy(data->payload, buffer->data[buffer->readPoint]->payload, data->payloadSize);

		/**
		 * free memory allocated for packet's payload
		 */
		free(buffer->data[buffer->readPoint]->payload);
		free(buffer->data[buffer->readPoint]);

		/**
		 * update checkpoints after getting data into buffer
		 */
		buffer->readPoint = (buffer->readPoint + 1) % buffer->capacity;
		buffer->isFull = false;

		ret = 0;
	}

	return ret;
}