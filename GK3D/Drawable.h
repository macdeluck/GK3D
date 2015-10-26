#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Common.h"

namespace GK
{
	class Drawable
	{
	public:
		Drawable(std::vector<GLfloat> vertices);
		virtual ~Drawable();

	private:
		GLuint vbo;
		std::vector<GLfloat> vertices;
	};
}

#endif