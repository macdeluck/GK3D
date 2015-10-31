#include "ObjectShader.h"
#include "LightSourceInstance.h"
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

	void ObjectShader::prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Camera> camera = scene->getCamera();

		// projection
		glm::mat4 projection;
		projection = glm::perspective(camera->getZoom(),
			((float)camera->getScreenWidth()) / camera->getScreenHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		model = glm::translate(model, drawableInstance->position);
		model = glm::rotate(model, drawableInstance->angleX, glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, drawableInstance->angleY, glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, drawableInstance->angleZ, glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, drawableInstance->scale);
		glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 cameraPosition = camera->getPosition();
		glUniform3f(getUniformLocation("viewPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// material
		std::shared_ptr<Material> material = drawableInstance->material;
		glUniform3f(getUniformLocation("material.ambient"), material->ambient.r, material->ambient.g, material->ambient.b);
		glUniform3f(getUniformLocation("material.diffuse"), material->diffuse.r, material->diffuse.g, material->diffuse.b);
		glUniform3f(getUniformLocation("material.specular"), material->specular.r, material->specular.g, material->specular.b);
		glUniform1f(getUniformLocation("material.shininess"), material->shininess);

		std::shared_ptr<std::array<std::shared_ptr<LightSourceInstance>, LIGHT_SOURCES_NUM> > lightSources = scene->getLightSources();
		for (size_t i = 0; i < lightSources->size(); i++)
		{
			glUniform3f(getUniformLocation("light.ambient"),
				(*lightSources)[i]->material->ambient.r, (*lightSources)[i]->material->ambient.g, (*lightSources)[i]->material->ambient.b);
			glUniform3f(getUniformLocation("light.diffuse"),
				(*lightSources)[i]->material->diffuse.r, (*lightSources)[i]->material->diffuse.g, (*lightSources)[i]->material->diffuse.b);
			glUniform3f(getUniformLocation("light.specular"),
				(*lightSources)[i]->material->specular.r, (*lightSources)[i]->material->specular.g, (*lightSources)[i]->material->specular.b);
			glUniform3f(getUniformLocation("light.position"),
				(*lightSources)[i]->position.x, (*lightSources)[i]->position.y, (*lightSources)[i]->position.z);
			glUniform1f(getUniformLocation("light.constant"), (*lightSources)[i]->constantTerm);
			glUniform1f(getUniformLocation("light.linear"), (*lightSources)[i]->linearTerm);
			glUniform1f(getUniformLocation("light.quadratic"), (*lightSources)[i]->quadraticTerm);
		}
	}
}