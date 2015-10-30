#include "ObjectShader.h"
#include "Scene.h"
#include "Camera.h"

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
		std::shared_ptr<ObjectFragmentShader>(new ObjectFragmentShader()))
		{
		}

	ObjectShader::~ObjectShader() {}

	void ObjectShader::beforeLink()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
		glBindAttribLocation(getProgramId(), 1, "normal");
	}

	void ObjectShader::prepareForRender(DrawableInstance drawableInstance, std::weak_ptr<Scene> scene)
	{
		glm::mat4 projection;
		std::shared_ptr<Camera> camera = scene.lock()->getCamera().lock();
		projection = glm::perspective(camera->getZoom(),
			((float)camera->getScreenWidth()) / camera->getScreenHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		model = glm::translate(model, drawableInstance.position);
		model = glm::rotate(model, drawableInstance.angleX, glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, drawableInstance.angleY, glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, drawableInstance.angleZ, glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, drawableInstance.scale);
		glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 color = drawableInstance.color;
		glUniform3f(getUniformLocation("objectColor"), color.r, color.g, color.b);
		DrawableInstance lightSource = (*(*scene.lock()->getDrawables().lock()->begin())->getInstances().begin());
		glUniform3f(getUniformLocation("lightColor"), lightSource.color.r, lightSource.color.g, lightSource.color.b);
		glUniform3f(getUniformLocation("lightPos"), lightSource.position.x, lightSource.position.y, lightSource.position.z);
	}
}