#include "Common.h"
#include "Drawable.h"
#include "SkyboxShader.h"
#include "GK3DScene.h"
#include "Camera.h"
#include "Skybox.h"
#include "Texture3D.h"

namespace GK
{
	SkyboxVertexShader::SkyboxVertexShader()
		: Shader(Shader::FromFile("skybox_vertex_shader.glsl"), ShaderType::VertexShader) {}
	SkyboxVertexShader::~SkyboxVertexShader() {}
	SkyboxFragmentShader::SkyboxFragmentShader()
		: Shader(Shader::FromFile("skybox_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	SkyboxFragmentShader::~SkyboxFragmentShader() {}

	SkyboxShader::SkyboxShader()
		: ShaderProgram(std::shared_ptr<SkyboxVertexShader>(new SkyboxVertexShader()),
			std::shared_ptr<SkyboxFragmentShader>(new SkyboxFragmentShader()))
	{
	}

	SkyboxShader::~SkyboxShader() {}

	void SkyboxShader::beforeLink()
	{
		glBindAttribLocation(getProgramId(), 0, "position");
	}
	void SkyboxShader::postRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene)
	{
		GLRUN(glDepthMask(GL_TRUE));
	}

	void SkyboxShader::prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		auto skyBox = std::dynamic_pointer_cast<SkyBox>(drawableInstance);
		GLRUN(glDepthMask(GL_FALSE));
		std::shared_ptr<Camera> camera = scene->getCamera();

		// projection
		glm::mat4 projection;
		projection = glm::perspective(camera->getZoom(),
			((float)camera->getScreenWidth()) / camera->getScreenHeight(), 0.1f, 100.0f);
		GLRUN(glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix())));
		GLRUN(glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection)));
		if (skyBox->skyboxTexture->empty())
			throw Exception("Unable to get skybox texture");
		bindTexture("skyboxTexture", skyBox->skyboxTexture->getId(), GL_TEXTURE_CUBE_MAP);
	}

}