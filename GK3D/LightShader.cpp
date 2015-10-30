#include "LightShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GK
{
	LightFragmentShader::LightFragmentShader()
		: Shader(Shader::FromFile("light_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	LightFragmentShader::~LightFragmentShader() {}

	LightShader::LightShader()
		: ShaderProgram(std::shared_ptr<ObjectVertexShader>(new ObjectVertexShader()),
		std::shared_ptr<LightFragmentShader>(new LightFragmentShader())),
		viewMatrix(), zoom(45.0f)
	{
	}

	LightShader::~LightShader() {}

	void LightShader::update()
	{
		glm::mat4 model;
		glm::mat4 projection;
		projection = glm::perspective(zoom, ((float)screenWidth) / screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void LightShader::before_link()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
	}

	void LightShader::beforeRender(DrawableInstance drawableInstance)
	{
	}
}