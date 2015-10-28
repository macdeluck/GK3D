#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Shader.h"

namespace GK
{
	struct Vertex
	{
	public:
		static const int VERTEX_SIZE = 6;

		Vertex();
		Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
		Vertex(GLfloat vertexData[VERTEX_SIZE]);
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat r;
		GLfloat g;
		GLfloat b;

		std::vector<GLfloat> toVector();
	};

	class Drawable
	{
	public:
		Drawable(std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			std::shared_ptr<ShaderProgram> shaderProgram);
		virtual ~Drawable();
		void render();
		std::shared_ptr<ShaderProgram> getShader();
	private:
		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::vector<GLfloat> getVertexData();
	};
}

#endif