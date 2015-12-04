#include "Common.h"
#include "LightShader.h"
#include "Scene.h"
#include "Camera.h"
#include <cmath>
#include "Fog.h"
#include "GK3DScene.h"

namespace GK
{
	LightFragmentShader::LightFragmentShader()
		: Shader(Shader::FromFile("light_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	LightFragmentShader::~LightFragmentShader() {}

	LightShader::LightShader()
		: ShaderProgram(std::shared_ptr<ObjectVertexShader>(new ObjectVertexShader()),
		std::shared_ptr<LightFragmentShader>(new LightFragmentShader()))
	{
	}

	LightShader::~LightShader() {}

	void LightShader::beforeLink()
	{
		GLRUN(glBindAttribLocation(getProgramId(), 0, "position"));
	}

	void LightShader::render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<Camera> camera = scene->getCamera();

		// fog
		std::shared_ptr<GK3DScene> gkscene = std::dynamic_pointer_cast<GK3DScene>(scene);
		GLRUN(glUniform1f(getUniformLocation("fog.density"), gkscene->getFog()->density));
		GLRUN(glUniform3f(getUniformLocation("fog.color"), gkscene->getFog()->color.r, gkscene->getFog()->color.g, gkscene->getFog()->color.b));

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

		glm::vec3 intensity = drawableInstance->material->ambient + drawableInstance->material->diffuse;
		GLfloat maxval = fmax(fmax(intensity.r, intensity.g), intensity.b);
		if (maxval > 0)
			intensity /= maxval;
		GLRUN(glUniform3f(getUniformLocation("intensity"), intensity.r, intensity.g, intensity.b));
		GLRUN(glUniform3f(getUniformLocation("viewPos"), camera->getPosition().x, camera->getPosition().y, camera->getPosition().z));
	}
}