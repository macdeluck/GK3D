#include "ObjectShader.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
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

		std::shared_ptr<PointLightsArray> lightSources = scene->getLightSources();
		glUniform1ui(getUniformLocation("pointLightsNum"), lightSources->size());
		for (size_t i = 0; i < lightSources->size(); i++)
		{
			std::stringstream sb = std::stringstream("");
			sb << "pointLights[" << i << "].";
			glUniform3f(getUniformLocation(sb.str() + std::string("ambient")),
				(*lightSources)[i]->material->ambient.r, (*lightSources)[i]->material->ambient.g, (*lightSources)[i]->material->ambient.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("diffuse")),
				(*lightSources)[i]->material->diffuse.r, (*lightSources)[i]->material->diffuse.g, (*lightSources)[i]->material->diffuse.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("specular")),
				(*lightSources)[i]->material->specular.r, (*lightSources)[i]->material->specular.g, (*lightSources)[i]->material->specular.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("position")),
				(*lightSources)[i]->position.x, (*lightSources)[i]->position.y, (*lightSources)[i]->position.z);
			glUniform1f(getUniformLocation(sb.str() + std::string("constant")), (*lightSources)[i]->constantTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("linear")), (*lightSources)[i]->linearTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("quadratic")), (*lightSources)[i]->quadraticTerm);
		}

		std::shared_ptr<SpotLightsArray> spotLights = scene->getSpotLights();
		glUniform1ui(getUniformLocation("spotLightsNum"), spotLights->size());
		for (size_t i = 0; i < spotLights->size(); i++)
		{
			std::stringstream sb = std::stringstream("");
			sb << "spotLights[" << i << "].";
			glUniform3f(getUniformLocation(sb.str() + std::string("ambient")),
				(*spotLights)[i]->material->ambient.r, (*spotLights)[i]->material->ambient.g, (*spotLights)[i]->material->ambient.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("diffuse")),
				(*spotLights)[i]->material->diffuse.r, (*spotLights)[i]->material->diffuse.g, (*spotLights)[i]->material->diffuse.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("specular")),
				(*spotLights)[i]->material->specular.r, (*spotLights)[i]->material->specular.g, (*spotLights)[i]->material->specular.b);
			glUniform1f(getUniformLocation(sb.str() + std::string("constant")), (*spotLights)[i]->constantTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("linear")), (*spotLights)[i]->linearTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("quadratic")), (*spotLights)[i]->quadraticTerm);

			glUniform3f(getUniformLocation(sb.str() + std::string("position")),
				(*spotLights)[i]->position.x, (*spotLights)[i]->position.y, (*spotLights)[i]->position.z);
			glUniform3f(getUniformLocation(sb.str() + std::string("direction")),
				(*spotLights)[i]->direction.x, (*spotLights)[i]->direction.y, (*spotLights)[i]->direction.z);
			glUniform1f(getUniformLocation(sb.str() + std::string("cutOff")), glm::cos(glm::radians((*spotLights)[i]->cutOff)));
			glUniform1f(getUniformLocation(sb.str() + std::string("outerCutOff")), glm::cos(glm::radians((*spotLights)[i]->outerCutOff)));
		}
	}
}