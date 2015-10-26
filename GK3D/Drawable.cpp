#include "Drawable.h"

namespace GK
{
	Drawable::Drawable(std::vector<GLfloat> vertices) : vertices(vertices)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	Drawable::~Drawable()
	{
		glDeleteBuffers(1, &vbo);
	}
}