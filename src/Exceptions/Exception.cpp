#include "Exception.h"

Exception::Exception(string message) :
	message(message)
{
}

const char* Exception::what() const throw() {
	return message.c_str();
}