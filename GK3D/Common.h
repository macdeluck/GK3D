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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Exception.h"

#define GLRUN(expression) { \
	expression; \
	GLenum glError = glGetError(); \
	if (GL_NO_ERROR != glError) \
	{ \
	throw Exception("OpenGL error: " + std::string((char*)gluErrorString(glError))); \
	} \
}

#define GLRUNR(expression, result) { \
	result = expression; \
	GLenum glError = glGetError(); \
	if (GL_NO_ERROR != glError) \
	{ \
	throw Exception("OpenGL error: " + std::string((char*)gluErrorString(glError))); \
	} \
}

#endif