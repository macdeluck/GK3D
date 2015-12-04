#ifndef _GK_EXCEPTION
#define _GK_EXCEPTION

#include <string>
#include <exception>

namespace GK
{
	class Exception : public std::exception
	{
	private:
		std::string _message;
		std::string _stacktrace;

	public:
		Exception();
		Exception(const Exception& otherException);
		Exception& operator=(const Exception& otherException);
		Exception(std::string message);
		virtual ~Exception();

		virtual std::string getStackTrace() const;
		virtual std::string message() const;
		virtual const char* what() const;
	};
}

#endif