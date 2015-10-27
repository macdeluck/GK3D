#include "Drawable.h"

namespace GK
{
	Drawable::Drawable(std::vector<GLfloat> vertices, std::vector<GLuint> indices, int stride, std::shared_ptr<ShaderProgram> shaderProgram)
		: vertices(vertices), indices(indices), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0)), shaderProgram(shaderProgram)
	{
		{
			GLuint l_vao, l_vbo, l_ebo;
			glGenVertexArrays(1, &l_vao);
			glGenBuffers(1, &l_vbo);
			glGenBuffers(1, &l_ebo);
			*vao = l_vao; *vbo = l_vbo; *ebo = l_ebo;
		}
		glBindVertexArray(*vao);

		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	Drawable::~Drawable()
	{
		if (vao.unique())
			glDeleteVertexArrays(1, &(*vao));
		if (vbo.unique())
			glDeleteBuffers(1, &(*vbo));
		if (ebo.unique())
			glDeleteBuffers(1, &(*ebo));
	}

	void Drawable::render()
	{
		shaderProgram->use();
		glBindVertexArray(*vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}