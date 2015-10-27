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
}