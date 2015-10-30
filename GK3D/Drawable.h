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

	class Drawable
	{
	public:
		Drawable(std::vector<Vertex> vertices,
			glm::vec3 objectColor,
			std::shared_ptr<ShaderProgram> shaderProgram);
		Drawable(std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			glm::vec3 objectColor,
			std::shared_ptr<ShaderProgram> shaderProgram);
		virtual ~Drawable();
		glm::vec3 getObjectColor();
		void render();
		std::shared_ptr<ShaderProgram> getShader();
	private:
		void init();

		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		glm::vec3 objectColor;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::vector<GLfloat> getVertexData();
	};
}

#endif