#pragma once
#include <exception>

class RuntimeError :
	public std::exception {
private:
	const char* message;
public:
	RuntimeError();
	RuntimeError(const char* message);
	RuntimeError(const char* message, size_t length);
	~RuntimeError();
	const char* what() const;
};

