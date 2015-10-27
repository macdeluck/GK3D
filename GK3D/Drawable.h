#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Shader.h"

namespace GK
{
	class Drawable
	{
	public:
		Drawable(std::vector<GLfloat> vertices,
			std::vector<GLuint> indices,
			int stride,
			std::shared_ptr<ShaderProgram> shaderProgram);
		virtual ~Drawable();
		void render();
	private:
		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		std::vector<GLfloat> vertices;
		std::vector<GLuint> indices;
		std::shared_ptr<ShaderProgram> shaderProgram;
	};
}

#endif