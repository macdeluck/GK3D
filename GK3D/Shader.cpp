#include "Shader.h"

namespace GK
{
	Shader::Shader(std::string source, ShaderType shaderType) : shaderId(new GLuint(0)), shaderType(shaderType)
	{
		GLint success;
		GLchar infoLog[512];
		*shaderId = glCreateShader(type());
		const GLchar* shaderSource = source.c_str();
		glShaderSource(*shaderId, 1, &shaderSource, NULL);
		glCompileShader(*shaderId);
		glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(*shaderId, 512, NULL, infoLog);
			throw Exception(std::string("Shader compilation failed: ") + std::string(infoLog));
		}
	}

	Shader::~Shader()
	{
		if (shaderId.unique())
			glDeleteShader(*shaderId);
	}

	ShaderType Shader::type()
	{
		return shaderType;
	}

	ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader)
		: programId(new GLuint(0))
	{
		if (vertexShader->type() != ShaderType::Vertex)
			throw Exception("Passed vertex shader type is not really vertex shader");
		if (fragmentShader->type() != ShaderType::Fragment)
			throw Exception("Passed fragment shader type is not really fragment shader");
		*programId = glCreateProgram();
		glAttachShader(*programId, *(vertexShader->shaderId));
		glAttachShader(*programId, *(fragmentShader->shaderId));
		glLinkProgram(*programId);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(*programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(*programId, 512, NULL, infoLog);
			throw Exception(std::string("Shader linking failed: ") + std::string(infoLog));
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		if (programId.unique())
			glDeleteProgram(*programId);
	}

	void ShaderProgram::use()
	{
		glUseProgram(*programId);
	}
}