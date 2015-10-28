#include "Shader.h"
#include <fstream>
#include <sstream>

namespace GK
{
	std::string Shader::FromFile(std::string path)
	{
		std::ifstream inputStream = std::ifstream(path);
		std::stringstream stringBuilder = std::stringstream("");
		if (!inputStream.good())
		{
			inputStream.close();
			throw Exception("Error occured while reading shader file");
		}
		std::string tmp;
		while (std::getline(inputStream, tmp))
		{
			stringBuilder << tmp;
			stringBuilder << "\n";
		}
		inputStream.close();
		return stringBuilder.str();
	}

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

	GLuint Shader::getShaderId()
	{
		return *shaderId;
	}

	ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader)
		: programId(new GLuint(0)), vertexShader(vertexShader), fragmentShader(fragmentShader)
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

	std::shared_ptr<Shader> ShaderProgram::getVertexShader()
	{
		return vertexShader;
	}
	std::shared_ptr<Shader> ShaderProgram::getFragmentShader()
	{
		return fragmentShader;
	}

	void ShaderProgram::update()
	{
	}

	GLuint ShaderProgram::getProgramId()
	{
		return *programId;
	}
}