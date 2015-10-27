#ifndef _GK_SHADER
#define _GK_SHADER

#include "Common.h"

namespace GK
{
	class ShaderProgram;
	class Drawable;

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
		static Shader FromFile(std::string path, ShaderType type);
	private:
		std::shared_ptr<GLuint> shaderId;
		ShaderType shaderType;
		friend class ShaderProgram;
	};

	class ShaderProgram
	{
	public:
		ShaderProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
		virtual ~ShaderProgram();

	private:
		std::shared_ptr<GLuint> programId;
		void use();
		friend class Drawable;
	};
}

#endif