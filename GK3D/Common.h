#ifndef _GK_COMMON
#define _GK_COMMON

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <exception>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <memory>

namespace GK
{
	class Exception : public std::exception
	{
	private:
		std::string _message;

	public:
		Exception();
		Exception(const Exception& otherException);
		Exception& operator=(const Exception& otherException);
		Exception(std::string message);
		virtual ~Exception();

		virtual std::string message() const;
		virtual const char* what() const;
	};
}

#endif