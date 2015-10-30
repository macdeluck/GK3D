#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Shader.h"
#include <glm/glm.hpp>

namespace GK
{
	struct Vertex
	{
	public:
		static const int VERTEX_SIZE = 3;

		Vertex();
		Vertex(GLfloat x, GLfloat y, GLfloat z);
		Vertex(GLfloat vertexData[VERTEX_SIZE]);
		GLfloat x;
		GLfloat y;
		GLfloat z;

		std::vector<GLfloat> toVector();
	};

	struct DrawableInstance
	{
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 scale;
		GLfloat angleX;
		GLfloat angleY;
		GLfloat angleZ;

		DrawableInstance(
			glm::vec3 color = glm::vec3(0, 0, 0),
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);
	};

	class Drawable
	{
	public:
		Drawable(std::vector<Vertex> vertices,
			std::vector<DrawableInstance> instances,
			std::shared_ptr<ShaderProgram> shaderProgram);
		Drawable(std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			std::vector<DrawableInstance> instances,
			std::shared_ptr<ShaderProgram> shaderProgram);
		virtual ~Drawable();
		std::vector<DrawableInstance> getInstances();
		void render();
		std::shared_ptr<ShaderProgram> getShader();
	private:
		void init();

		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		std::vector<DrawableInstance> instances;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::vector<GLfloat> getVertexData();
	};
}

#endif