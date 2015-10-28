#include "Drawable.h"
#include <algorithm>

namespace GK
{
	Drawable::Drawable(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::shared_ptr<ShaderProgram> shaderProgram)
		: vertices(vertices), indices(indices), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0)), shaderProgram(shaderProgram)
	{
		init();
	}

	Drawable::Drawable(std::vector<Vertex> vertices, std::shared_ptr<ShaderProgram> shaderProgram)
		: vertices(vertices), indices(0), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0)), shaderProgram(shaderProgram)
	{
		init();
	}

	void Drawable::init()
	{
		{
			GLuint l_vao, l_vbo, l_ebo;
			glGenVertexArrays(1, &l_vao);
			*vao = l_vao;
			glGenBuffers(1, &l_vbo);
			*vbo = l_vbo;
			if (indices.size() > 0)
			{
				glGenBuffers(1, &l_ebo);
				*ebo = l_ebo;
			}
		}
		glBindVertexArray(*vao);

		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		std::vector<GLfloat> vertexData = getVertexData();
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		if (indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	Drawable::~Drawable()
	{
		if (vao.unique())
			glDeleteVertexArrays(1, &(*vao));
		if (vbo.unique())
			glDeleteBuffers(1, &(*vbo));
		if (indices.size() > 0)
		{
			if (ebo.unique())
				glDeleteBuffers(1, &(*ebo));
		}
	}

	void Drawable::render()
	{
		shaderProgram->use();
		glBindVertexArray(*vao);
		if (indices.size() > 0)
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		else glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}

	std::shared_ptr<ShaderProgram> Drawable::getShader()
	{
		return shaderProgram;
	}

	std::vector<GLfloat> Drawable::getVertexData()
	{
		std::vector<GLfloat> result = std::vector<GLfloat>(vertices.size() * Vertex::VERTEX_SIZE);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			std::copy_n(vertices[i].toVector().begin(), Vertex::VERTEX_SIZE, result.begin() + i*Vertex::VERTEX_SIZE);
		}
		return result;
	}

	Vertex::Vertex() : x(0), y(0), z(0), r(0), g(0), b(0) {}
	Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
		: x(x), y(y), z(z), r(r), g(g), b(b) {}
	Vertex::Vertex(GLfloat vertexData[VERTEX_SIZE])
		: x(vertexData[0]), y(vertexData[1]), z(vertexData[2]), r(vertexData[3]), g(vertexData[4]), b(vertexData[5]) {}

	std::vector<GLfloat> Vertex::toVector()
	{
		return std::vector < GLfloat > { x, y, z, r, g, b };
	}
}