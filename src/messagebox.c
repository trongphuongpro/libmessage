/** 
 * @file messagebox.c
 * @brief Implementation for creating ring buffer for embedded systems
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date January 6, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "messagebox.h"


void messagebox_create(MessageBox *box, Message *data, uint8_t num) {
	assert(num);
	assert(box);

	box->data = data;
	assert(box->data);

	box->capacity = num;
	box->readPoint = 0;
	box->writePoint = 0;
	box->isFull = false;

	assert(messagebox_isEmpty(box));
}


void messagebox_clear(MessageBox_t buffer) {
	Message dump;

	while (!messagebox_isEmpty(buffer)) {
		messagebox_pop(buffer, &dump);
	}
}


bool messagebox_isAvailable(MessageBox_t buffer) {
	return !messagebox_isEmpty(buffer);
}


bool messagebox_isEmpty(MessageBox_t buffer) {
	assert(buffer);

	return (!buffer->isFull && (buffer->readPoint == buffer->writePoint));
}


bool messagebox_isFull(MessageBox_t buffer) {
	assert(buffer);

	return buffer->isFull;
}


uint8_t messagebox_getCapacity(MessageBox_t buffer) {
	assert(buffer);

	return buffer->capacity;
}


uint8_t messagebox_getUsedSpace(MessageBox_t buffer) {
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


uint8_t messagebox_getFreeSpace(MessageBox_t buffer) {
	return messagebox_getCapacity(buffer) - messagebox_getUsedSpace(buffer);
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


void messagebox_push(MessageBox_t buffer, Message data) {
	assert(buffer && buffer->data);

	if (!buffer->isFull) {
		buffer->data[buffer->writePoint] = data;

		buffer->writePoint = (buffer->writePoint + 1) % buffer->capacity;
		buffer->isFull = (buffer->readPoint == buffer->writePoint);
	}
}


int messagebox_pop(MessageBox_t buffer, Message_t data) {
	assert(buffer && buffer->data && data);

	int ret = -1;

	if (!messagebox_isEmpty(buffer)) {
		data->address = buffer->data[buffer->readPoint].address;
		data->payloadSize = buffer->data[buffer->readPoint].payloadSize;
		memcpy(data->payload, buffer->data[buffer->readPoint].payload, data->payloadSize);

		/**
		 * update checkpoints after getting data into buffer
		 */
		buffer->readPoint = (buffer->readPoint + 1) % buffer->capacity;
		buffer->isFull = false;

		ret = 0;
	}

	return ret;
}