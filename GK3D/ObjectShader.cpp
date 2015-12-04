#include "ObjectShader.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "Fog.h"
#include "GK3DScene.h"

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
		GLRUN(glBindAttribLocation(getProgramId(), 0, "position"));
		GLRUN(glBindAttribLocation(getProgramId(), 1, "normal"));
		GLRUN(glBindAttribLocation(getProgramId(), 2, "texCoord"));
	}

	void ObjectShader::render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Camera> camera = scene->getCamera();

		// projection
		glm::mat4 projection;
		projection = glm::perspective(camera->getZoom(),
			((float)camera->getScreenWidth()) / camera->getScreenHeight(), 0.1f, 100.0f);
		GLRUN(glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(camera->getViewMatrix())));
		GLRUN(glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection)));
		glm::mat4 model;
		model = glm::translate(model, drawableInstance->position);
		model = glm::rotate(model, glm::radians(drawableInstance->angleX), glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, glm::radians(drawableInstance->angleY), glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, glm::radians(drawableInstance->angleZ), glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, drawableInstance->scale);
		GLRUN(glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model)));
		glm::vec3 cameraPosition = camera->getPosition();
		GLRUN(glUniform3f(getUniformLocation("viewPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z));

		// fog
		std::shared_ptr<GK3DScene> gkscene = std::dynamic_pointer_cast<GK3DScene>(scene);
		GLRUN(glUniform1f(getUniformLocation("fog.density"), gkscene->getFog()->density));
		GLRUN(glUniform3f(getUniformLocation("fog.color"), gkscene->getFog()->color.r, gkscene->getFog()->color.g, gkscene->getFog()->color.b));

		// material
		std::shared_ptr<Material> material = drawableInstance->material;
		GLRUN(glUniform3f(getUniformLocation("material.ambient"), material->ambient.r, material->ambient.g, material->ambient.b));
		GLRUN(glUniform3f(getUniformLocation("material.diffuse"), material->diffuse.r, material->diffuse.g, material->diffuse.b));
		GLRUN(glUniform3f(getUniformLocation("material.specular"), material->specular.r, material->specular.g, material->specular.b));
		GLRUN(glUniform1f(getUniformLocation("material.shininess"), material->shininess));
		if (material->diffuseTex.empty())
		{
			GLRUN(glUniform1i(getUniformLocation("material.diffuseTex.used"), 0));
		}
		else
		{
			GLRUN(glUniform1i(getUniformLocation("material.diffuseTex.used"), 1));
			bindTexture("material.diffuseTex.texture", material->diffuseTex.getId());
		}
		if (material->alphaTex.empty())
		{
			GLRUN(glUniform1i(getUniformLocation("material.alphaTex.used"), 0));
		}
		else
		{
			GLRUN(glUniform1i(getUniformLocation("material.alphaTex.used"), 1));
			bindTexture("material.alphaTex.texture", material->alphaTex.getId());
		}

		std::shared_ptr<PointLightsArray> pointLights = scene->getPointLights();
		GLRUN(glUniform1i(getUniformLocation("pointLightsNum"), (int)pointLights->size()));
		for (size_t i = 0; i < pointLights->size(); i++)
		{
			std::stringstream sb = std::stringstream("");
			sb << "pointLights[" << i << "].";
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("ambient")),
				(*pointLights)[i]->material->ambient.r, (*pointLights)[i]->material->ambient.g, (*pointLights)[i]->material->ambient.b));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("diffuse")),
				(*pointLights)[i]->material->diffuse.r, (*pointLights)[i]->material->diffuse.g, (*pointLights)[i]->material->diffuse.b));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("specular")),
				(*pointLights)[i]->material->specular.r, (*pointLights)[i]->material->specular.g, (*pointLights)[i]->material->specular.b));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("position")),
				(*pointLights)[i]->position.x, (*pointLights)[i]->position.y, (*pointLights)[i]->position.z));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("constant")), (*pointLights)[i]->constantTerm));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("linear")), (*pointLights)[i]->linearTerm));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("quadratic")), (*pointLights)[i]->quadraticTerm));
		}

		std::shared_ptr<SpotLightsArray> spotLights = scene->getSpotLights();
		GLRUN(glUniform1i(getUniformLocation("spotLightsNum"), (int)spotLights->size()));
		for (size_t i = 0; i < spotLights->size(); i++)
		{
			std::stringstream sb = std::stringstream("");
			sb << "spotLights[" << i << "].";
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("ambient")),
				(*spotLights)[i]->material->ambient.r, (*spotLights)[i]->material->ambient.g, (*spotLights)[i]->material->ambient.b));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("diffuse")),
				(*spotLights)[i]->material->diffuse.r, (*spotLights)[i]->material->diffuse.g, (*spotLights)[i]->material->diffuse.b));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("specular")),
				(*spotLights)[i]->material->specular.r, (*spotLights)[i]->material->specular.g, (*spotLights)[i]->material->specular.b));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("constant")), (*spotLights)[i]->constantTerm));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("linear")), (*spotLights)[i]->linearTerm));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("quadratic")), (*spotLights)[i]->quadraticTerm));

			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("position")),
				(*spotLights)[i]->position.x, (*spotLights)[i]->position.y, (*spotLights)[i]->position.z));
			GLRUN(glUniform3f(getUniformLocation(sb.str() + std::string("direction")),
				(*spotLights)[i]->direction.x, (*spotLights)[i]->direction.y, (*spotLights)[i]->direction.z));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("cutOff")), glm::cos(glm::radians((*spotLights)[i]->cutOff))));
			GLRUN(glUniform1f(getUniformLocation(sb.str() + std::string("outerCutOff")), glm::cos(glm::radians((*spotLights)[i]->outerCutOff))));
		}
	}
}