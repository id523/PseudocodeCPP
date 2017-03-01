#include "stdafx.h"
#include "RuntimeError.h"


RuntimeError::RuntimeError()
{
	message = "An error occurred.";
}

RuntimeError::RuntimeError(const char * msg)
{
	message = msg;
}


RuntimeError::~RuntimeError()
{
}

const char* RuntimeError::what() const
{
	return message;
}
