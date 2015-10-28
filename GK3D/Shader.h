#ifndef _GK_SHADER
#define _GK_SHADER

#include "Common.h"

namespace GK
{
	class ShaderProgram;

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
		virtual void update();
		static std::string FromFile(std::string path);
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
		std::shared_ptr<Shader> getVertexShader();
		std::shared_ptr<Shader> getFragmentShader();
		void use();
	private:
		std::shared_ptr<GLuint> programId;
		std::shared_ptr<Shader> vertexShader;
		std::shared_ptr<Shader> fragmentShader;
	};
}

#endif