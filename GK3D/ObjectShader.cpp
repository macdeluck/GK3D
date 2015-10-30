#include "ObjectShader.h"

namespace GK
{
	ObjectVertexShader::ObjectVertexShader() 
		: Shader(Shader::FromFile("object_vertex_shader.glsl"), ShaderType::VertexShader) {}
	ObjectVertexShader::~ObjectVertexShader() {}
	ObjectFragmentShader::ObjectFragmentShader()
		: Shader(Shader::FromFile("object_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	ObjectFragmentShader::~ObjectFragmentShader() {}

	ObjectShader::ObjectShader()
		: ShaderProgram(std::shared_ptr<ObjectVertexShader>(new ObjectVertexShader()),
		std::shared_ptr<ObjectFragmentShader>(new ObjectFragmentShader())),
		viewMatrix(), zoom(45.0f)
		{
		}

	ObjectShader::~ObjectShader() {}

	void ObjectShader::update()
	{
		glm::mat4 projection;
		projection = glm::perspective(zoom, ((float)screenWidth) / screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void ObjectShader::before_link()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
	}

	void ObjectShader::beforeRender(DrawableInstance drawableInstance)
	{
		glm::mat4 model;
		model = glm::translate(model, drawableInstance.position);
		model = glm::rotate(model, drawableInstance.angleX, glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, drawableInstance.angleY, glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, drawableInstance.angleZ, glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, drawableInstance.scale);
		glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 color = drawableInstance.color;
		glUniform3f(getUniformLocation("objectColor"), color.r, color.g, color.b);
	}
}