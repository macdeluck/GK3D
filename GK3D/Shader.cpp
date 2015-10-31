#include "Shader.h"
#include "Drawable.h"
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
		: programId(new GLuint(0)), vertexShader(vertexShader), fragmentShader(fragmentShader), compiled(false)
	{
		if (vertexShader->type() != ShaderType::VertexShader)
			throw Exception("Passed vertex shader type is not really vertex shader");
		if (fragmentShader->type() != ShaderType::FragmentShader)
			throw Exception("Passed fragment shader type is not really fragment shader");
		*programId = glCreateProgram();
	}

	void ShaderProgram::compile()
	{
		glAttachShader(*programId, *(vertexShader->shaderId));
		glAttachShader(*programId, *(fragmentShader->shaderId));
		beforeLink();
		glLinkProgram(*programId);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(*programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(*programId, 512, NULL, infoLog);
			throw Exception(std::string("Shader linking failed: ") + std::string(infoLog));
		}
		compiled = true;
	}

	ShaderProgram::~ShaderProgram()
	{
		if (programId.unique())
			glDeleteProgram(*programId);
	}

	void ShaderProgram::use()
	{
		if (!compiled)
			throw Exception("Cannot use shader which was not compiled");
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

	GLuint ShaderProgram::getProgramId()
	{
		return *programId;
	}

	GLuint ShaderProgram::getUniformLocation(std::string uniformName)
	{
		GLint uniformLoc = glGetUniformLocation(getProgramId(), uniformName.c_str());
		if (uniformLoc == 0xffffffff)
			throw Exception(std::string("Uniform '") + uniformName + std::string("' location was not found"));
		return uniformLoc;
	}

	void ShaderProgram::beforeLink()
	{
	}

	void ShaderProgram::render(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene)
	{
		use();
		prepareForRender(drawable, scene);
	}

	void ShaderProgram::prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
	}
}