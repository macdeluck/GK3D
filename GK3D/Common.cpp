#include "Common.h"

namespace GK
{
	Exception::Exception() : _message()
	{
	}
	Exception::Exception(const Exception& otherException)
	{
		this->_message = otherException._message;
	}
	Exception& Exception::operator=(const Exception& otherException)
	{
		this->_message = otherException._message;
		return *this;
	}
	Exception::Exception(std::string message) : _message(message)
	{
	}

	Exception::~Exception()
	{
	}

	std::string Exception::message() const
	{
		return this->_message;
	}
	const char* Exception::what() const
	{
		return this->_message.c_str();
	}
}