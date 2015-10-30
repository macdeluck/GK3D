#include "Drawable.h"
#include "Scene.h"
#include "Shader.h"
#include <algorithm>

namespace GK
{
	DrawableInstance::DrawableInstance(std::shared_ptr<ShaderProgram> shaderProgram,
		glm::vec3 color,
		glm::vec3 position,
		glm::vec3 scale,
		GLfloat angleX, GLfloat angleY, GLfloat angleZ)
		: shaderProgram(shaderProgram),
		color(color), position(position), scale(scale), angleX(angleX), angleY(angleY), angleZ(angleZ)
	{
	}

	Drawable::Drawable(std::vector<Vertex> vertices, std::vector<GLuint> indices,
		std::vector<DrawableInstance> instances)
		: vertices(vertices), instances(instances), indices(indices), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0))
	{
		init();
	}

	Drawable::Drawable(std::vector<Vertex> vertices,
		std::vector<DrawableInstance> instances)
		: vertices(vertices), instances(instances), indices(0), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0))
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

	void Drawable::render(std::weak_ptr<Scene> scene)
	{
		glBindVertexArray(*vao);
		for (size_t i = 0; i < instances.size(); i++)
		{
			instances[i].shaderProgram->render(instances[i], scene);
			if (indices.size() > 0)
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			else glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		}
		glBindVertexArray(0);
	}

	std::vector<DrawableInstance> Drawable::getInstances()
	{
		return instances;
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

	Vertex::Vertex() : x(0), y(0), z(0) {}
	Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z)
		: x(x), y(y), z(z) {}
	Vertex::Vertex(GLfloat vertexData[VERTEX_SIZE])
		: x(vertexData[0]), y(vertexData[1]), z(vertexData[2]) {}

	std::vector<GLfloat> Vertex::toVector()
	{
		return std::vector < GLfloat > { x, y, z };
	}
}