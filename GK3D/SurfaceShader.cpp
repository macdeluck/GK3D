#include "SurfaceShader.h"
#include "ObjectShader.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "SurfaceInstance.h"
#include "Drawable.h"

namespace GK
{
	SurfaceVertexShader::SurfaceVertexShader()
		: Shader(Shader::FromFile("surface_vertex_shader.glsl"), ShaderType::VertexShader) {}
	SurfaceVertexShader::~SurfaceVertexShader() {}

	SurfaceFragmentShader::SurfaceFragmentShader()
		: Shader(Shader::FromFile("surface_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	SurfaceFragmentShader::~SurfaceFragmentShader() {}

	SurfaceShader::SurfaceShader()
		: ShaderProgram(std::shared_ptr<SurfaceVertexShader>(new SurfaceVertexShader()),
			std::shared_ptr<SurfaceFragmentShader>(new SurfaceFragmentShader()))
	{
	}

	SurfaceShader::~SurfaceShader() {}

	void SurfaceShader::beforeLink()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
		glBindAttribLocation(getProgramId(), 1, "normal");
	}

	const int GLSurfacetextureLocationsCount = 10;
	int GLSurfacetextureLocations[GLSurfacetextureLocationsCount] = {
		GL_TEXTURE0,
		GL_TEXTURE1,
		GL_TEXTURE2,
		GL_TEXTURE3,
		GL_TEXTURE4,
		GL_TEXTURE5,
		GL_TEXTURE6,
		GL_TEXTURE7,
		GL_TEXTURE8,
		GL_TEXTURE9
	};

	void SurfaceShader::prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Camera> camera = scene->getCamera();
		std::shared_ptr<SurfaceInstance> surfaceInstance = std::dynamic_pointer_cast<SurfaceInstance>(drawableInstance);
		// projection
		glm::mat4 projection;
		projection = glm::perspective(camera->getZoom(),
			((float)camera->getScreenWidth()) / camera->getScreenHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		model = glm::translate(model, surfaceInstance->position);
		model = glm::rotate(model, glm::radians(surfaceInstance->angleX), glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, glm::radians(surfaceInstance->angleY), glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, glm::radians(surfaceInstance->angleZ), glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, surfaceInstance->scale);
		glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 cameraPosition = camera->getPosition();
		glUniform3f(getUniformLocation("viewPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// texProjection
		glm::vec2 texOffset = surfaceInstance->texOffset;
		glm::vec2 texScale = surfaceInstance->texScale;
		glm::mat3 texModel = glm::transpose(glm::mat3(
			glm::vec3(texScale.x, 0.0f, texOffset.x),
			glm::vec3(0.0f, texScale.y, texOffset.y),
			glm::vec3(0.0f, 0.0f, 1.0f)));
		glUniformMatrix3fv(getUniformLocation("texModel"), 1, GL_FALSE, glm::value_ptr(texModel));

		// material
		std::shared_ptr<Material> material = surfaceInstance->material;
		glUniform3f(getUniformLocation("material.ambient"), material->ambient.r, material->ambient.g, material->ambient.b);
		glUniform3f(getUniformLocation("material.diffuse"), material->diffuse.r, material->diffuse.g, material->diffuse.b);
		glUniform3f(getUniformLocation("material.specular"), material->specular.r, material->specular.g, material->specular.b);
		glUniform1f(getUniformLocation("material.shininess"), material->shininess);
		if (material->diffuseTex.empty())
			glUniform1i(getUniformLocation("material.diffuseTex.used"), 0);
		else
		{
			glUniform1i(getUniformLocation("material.diffuseTex.used"), 1);
			glUniform1i(glGetUniformLocation(getProgramId(), "material.diffuseTex.texture"), material->diffuseTex.getLocation());
			glActiveTexture(GLSurfacetextureLocations[material->diffuseTex.getLocation()]);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTex.getId());
		}
		if (material->alphaTex.empty())
			glUniform1i(getUniformLocation("material.alphaTex.used"), 0);
		else
		{
			glUniform1i(getUniformLocation("material.alphaTex.used"), 1);
			glUniform1i(glGetUniformLocation(getProgramId(), "material.alphaTex.texture"), material->alphaTex.getLocation());
			glActiveTexture(GLSurfacetextureLocations[material->alphaTex.getLocation()]);
			glBindTexture(GL_TEXTURE_2D, material->alphaTex.getId());
		}
		if (surfaceInstance->marksTexture.empty() || !surfaceInstance->marksTexUsed)
			glUniform1i(getUniformLocation("material.marksTex.used"), 0);
		else
		{
			glUniform1i(getUniformLocation("material.marksTex.used"), 1);
			glUniform1f(getUniformLocation("material.marksTexScale"), 10.0f);
			glUniform1i(glGetUniformLocation(getProgramId(), "material.marksTex.texture"), surfaceInstance->marksTexture.getLocation());
			glActiveTexture(GLSurfacetextureLocations[surfaceInstance->marksTexture.getLocation()]);
			glBindTexture(GL_TEXTURE_2D, surfaceInstance->marksTexture.getId());
		}

		std::shared_ptr<PointLightsArray> pointLights = scene->getPointLights();
		glUniform1i(getUniformLocation("pointLightsNum"), (int)pointLights->size());
		for (size_t i = 0; i < pointLights->size(); i++)
		{
			std::stringstream sb = std::stringstream("");
			sb << "pointLights[" << i << "].";
			glUniform3f(getUniformLocation(sb.str() + std::string("ambient")),
				(*pointLights)[i]->material->ambient.r, (*pointLights)[i]->material->ambient.g, (*pointLights)[i]->material->ambient.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("diffuse")),
				(*pointLights)[i]->material->diffuse.r, (*pointLights)[i]->material->diffuse.g, (*pointLights)[i]->material->diffuse.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("specular")),
				(*pointLights)[i]->material->specular.r, (*pointLights)[i]->material->specular.g, (*pointLights)[i]->material->specular.b);
			glUniform3f(getUniformLocation(sb.str() + std::string("position")),
				(*pointLights)[i]->position.x, (*pointLights)[i]->position.y, (*pointLights)[i]->position.z);
			glUniform1f(getUniformLocation(sb.str() + std::string("constant")), (*pointLights)[i]->constantTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("linear")), (*pointLights)[i]->linearTerm);
			glUniform1f(getUniformLocation(sb.str() + std::string("quadratic")), (*pointLights)[i]->quadraticTerm);
		}

		std::shared_ptr<SpotLightsArray> spotLights = scene->getSpotLights();
		glUniform1i(getUniformLocation("spotLightsNum"), (int)spotLights->size());
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