#include "Common.h"
#include "Camera.h"
#include "EnvShader.h"
#include "Scene.h"
#include "Skybox.h"
#include "Texture3D.h"

namespace GK
{
	EnvVertexShader::EnvVertexShader()
		: Shader(Shader::FromFile("env_vertex_shader.glsl"), ShaderType::VertexShader) {}
	EnvVertexShader::~EnvVertexShader() {}
	EnvFragmentShader::EnvFragmentShader()
		: Shader(Shader::FromFile("env_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	EnvFragmentShader::~EnvFragmentShader() {}

	EnvShader::EnvShader()
		: ShaderProgram(std::shared_ptr<EnvVertexShader>(new EnvVertexShader()),
			std::shared_ptr<EnvFragmentShader>(new EnvFragmentShader()))
	{
	}

	EnvShader::~EnvShader() {}

	void EnvShader::beforeLink()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
		glBindAttribLocation(getProgramId(), 1, "normal");
	}

	void EnvShader::render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		auto skyBox = scene->getSkyBox();
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

		if (skyBox)
		{
			if (skyBox->skyboxTexture->empty())
				throw Exception("Unable to get skybox texture");
			bindTexture("envTexture", skyBox->skyboxTexture->getId(), GL_TEXTURE_CUBE_MAP);
		}
	}
}