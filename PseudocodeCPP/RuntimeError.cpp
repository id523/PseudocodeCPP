#include "stdafx.h"
#include "RuntimeError.h"

#include <string.h> // strlen

RuntimeError::RuntimeError() : RuntimeError("An error occurred.") {}

RuntimeError::RuntimeError(const char * msg) : RuntimeError(msg, strlen(msg)) {}

RuntimeError::RuntimeError(const char * msg, size_t length) {
	// Make a copy of the given message
	size_t copylen = length + 1;
	char* msgcopy = new char[copylen];
	memcpy(msgcopy, msg, length);
	msgcopy[length] = 0;
	message = msgcopy;
}

RuntimeError::~RuntimeError() {
	// Clean up the created copy
	delete[] message;
}

const char* RuntimeError::what() const {
	return message;
}
