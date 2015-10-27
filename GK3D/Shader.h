#ifndef _GK_SHADER
#define _GK_SHADER

#include "Common.h"

namespace GK
{
	enum ShaderType
	{
		Unknown = 0,
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	class Shader
	{
	public:
		Shader(std::string source, ShaderType type);
		virtual ~Shader();

		virtual ShaderType type();
	private:
		std::shared_ptr<GLuint> shaderId;
		ShaderType shaderType;
	};
}

#endif