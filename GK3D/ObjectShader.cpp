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
		std::shared_ptr<Camera> camera = scene.lock()->getCamera().lock();

		// projection
		glm::mat4 projection;
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
		glm::vec3 cameraPosition = camera->getPosition();
		glUniform3f(getUniformLocation("viewPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// material
		std::shared_ptr<Material> material = drawableInstance.material;
		glUniform3f(getUniformLocation("material.ambient"), material->ambient.r, material->ambient.g, material->ambient.b);
		glUniform3f(getUniformLocation("material.diffuse"), material->diffuse.r, material->diffuse.g, material->diffuse.b);
		glUniform3f(getUniformLocation("material.specular"), material->specular.r, material->specular.g, material->specular.b);
		glUniform1f(getUniformLocation("material.shininess"), material->shininess);

		// light source
		DrawableInstance lightSource = (*(*scene.lock()->getDrawables().lock()->begin())->getInstances().begin());
		glUniform3f(getUniformLocation("light.ambient"), 
			lightSource.material->ambient.r, lightSource.material->ambient.g, lightSource.material->ambient.b);
		glUniform3f(getUniformLocation("light.diffuse"), 
			lightSource.material->diffuse.r, lightSource.material->diffuse.g, lightSource.material->diffuse.b);
		glUniform3f(getUniformLocation("light.specular"), 
			lightSource.material->specular.r, lightSource.material->specular.g, lightSource.material->specular.b);
		glUniform3f(getUniformLocation("light.position"), 
			lightSource.position.x, lightSource.position.y, lightSource.position.z);
	}
}