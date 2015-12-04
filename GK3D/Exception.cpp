#include "Exception.h"

#ifdef GK3D_DEBUG
#include "call_stack.hpp"
#endif

namespace GK
{
	Exception::Exception() : _message()
	{
#ifdef GK3D_DEBUG
		stacktrace::call_stack cs;
		_stacktrace = cs.to_string();
#endif
	}

	Exception::Exception(const Exception& otherException)
	{
		this->_message = otherException._message;
#ifdef GK3D_DEBUG
		stacktrace::call_stack cs;
		_stacktrace = cs.to_string();
#endif
	}

	Exception& Exception::operator=(const Exception& otherException)
	{
		this->_message = otherException._message;
#ifdef GK3D_DEBUG
		stacktrace::call_stack cs;
		_stacktrace = cs.to_string();
#endif
		return *this;
	}

	Exception::Exception(std::string message) : _message(message)
	{
#ifdef GK3D_DEBUG
		stacktrace::call_stack cs;
		_stacktrace = cs.to_string();
#endif
	}

	Exception::~Exception()
	{
	}

	std::string Exception::getStackTrace() const
	{
		return _stacktrace;
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