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
		if (shaderId == NULL)
			throw Exception("Unable to create shader");
		const GLchar* shaderSource = source.c_str();
		GLRUN(glShaderSource(*shaderId, 1, &shaderSource, NULL));
		GLRUN(glCompileShader(*shaderId));
		GLRUN(glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLRUN(glGetShaderInfoLog(*shaderId, 512, NULL, infoLog));
			throw Exception(std::string("Shader compilation failed:\n") + std::string(infoLog));
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
		if (*programId == NULL)
			throw Exception("Unable to create shader program");
	}

	void ShaderProgram::compile()
	{
		GLRUN(glAttachShader(*programId, *(vertexShader->shaderId)));
		GLRUN(glAttachShader(*programId, *(fragmentShader->shaderId)));
		beforeLink();
		GLRUN(glLinkProgram(*programId));

		GLint success;
		GLchar infoLog[512];
		GLRUN(glGetProgramiv(*programId, GL_LINK_STATUS, &success));
		if (!success) {
			GLRUN(glGetProgramInfoLog(*programId, 512, NULL, infoLog));
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
		GLRUN(glUseProgram(*programId));
	}

	void ShaderProgram::clearTextureUnits()
	{
		currentTextureUnit = 0;
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
		clearTextureUnits();
		prepareForRender(drawable, scene);
	}

	void ShaderProgram::prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
	}

	void ShaderProgram::postRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene)
	{
	}

	void ShaderProgram::bindTexture(std::string uniformName, GLuint textureID, GLenum textureType)
	{
		GLint maxTextureUnits;
		GLRUN(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits));
		if (currentTextureUnit > (GLuint)maxTextureUnits)
			throw Exception("Maximum number of texture units has been exceeded: " + std::to_string(maxTextureUnits));
		GLRUN(glUniform1i(glGetUniformLocation(getProgramId(), uniformName.c_str()), currentTextureUnit));
		GLRUN(glActiveTexture(GL_TEXTURE0 + currentTextureUnit));
		GLRUN(glBindTexture(textureType, textureID));
		currentTextureUnit++;
	}
}