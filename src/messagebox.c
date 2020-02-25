/** 
 * @file messagebox.c
 * @brief Implementation for creating ring box for embedded systems
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date January 6, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "messagebox.h"


void messagebox_create(MessageBox_t *box, Message_t *data, uint8_t num) {
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


void messagebox_clear(MessageBox_t *box) {
	Message_t dump;

	while (!messagebox_isEmpty(box)) {
		messagebox_pop(box, &dump);
	}
}


bool messagebox_isAvailable(MessageBox_t *box) {
	return !messagebox_isEmpty(box);
}


bool messagebox_isEmpty(MessageBox_t *box) {
	assert(box);

	return (!box->isFull && (box->readPoint == box->writePoint));
}


bool messagebox_isFull(MessageBox_t *box) {
	assert(box);

	return box->isFull;
}


uint8_t messagebox_getCapacity(MessageBox_t *box) {
	assert(box);

	return box->capacity;
}


uint8_t messagebox_getUsedSpace(MessageBox_t *box) {
	assert(box);

	uint8_t number = box->capacity;

	if (!box->isFull) {
		if (box->readPoint <= box->writePoint) {
			number = box->writePoint - box->readPoint;
		}
		else {
			number -= box->readPoint - box->writePoint;
		}
	}

	return number;
}


uint8_t messagebox_getFreeSpace(MessageBox_t *box) {
	return messagebox_getCapacity(box) - messagebox_getUsedSpace(box);
}


/*void increase_checkpoints(MessageBox_t* box) {
	assert(box);

	if (box->isFull) {
		box->readPoint = (box->readPoint + 1) % box->capacity;
	}

	box->writePoint = (box->writePoint + 1) % box->capacity;
	box->isFull = (box->readPoint == box->writePoint);
}


void decrease_checkpoints(MessageBox_t* box) {
	assert(box);

	box->readPoint = (box->readPoint + 1) % box->capacity;
	box->isFull = false;
}*/


void messagebox_push(MessageBox_t *box, Message_t data) {
	assert(box && box->data);

	if (!box->isFull) {
		box->data[box->writePoint] = data;

		box->writePoint = (box->writePoint + 1) % box->capacity;
		box->isFull = (box->readPoint == box->writePoint);
	}
}


int messagebox_pop(MessageBox_t *box, Message_t *data) {
	assert(box && box->data && data);

	int ret = -1;

	if (!messagebox_isEmpty(box)) {
		data->address = box->data[box->readPoint].address;
		data->payloadSize = box->data[box->readPoint].payloadSize;
		memcpy(data->payload, box->data[box->readPoint].payload, data->payloadSize);

		/**
		 * update checkpoints after getting data into box
		 */
		box->readPoint = (box->readPoint + 1) % box->capacity;
		box->isFull = false;

		ret = 0;
	}

	return ret;
}