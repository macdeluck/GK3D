#include "ScreenShader.h"
#include "Shader.h"
#include "Texture.h"

namespace GK
{
	ScreenVertexShader::ScreenVertexShader()
		: Shader(Shader::FromFile("screen_vertex_shader.glsl"), ShaderType::VertexShader) {}
	ScreenVertexShader::~ScreenVertexShader() {}

	ScreenFragmentShader::ScreenFragmentShader()
		: Shader(Shader::FromFile("screen_fragment_shader.glsl"), ShaderType::FragmentShader) {}
	ScreenFragmentShader::~ScreenFragmentShader() {}

	ScreenShader::ScreenShader()
		: ShaderProgram(std::shared_ptr<ScreenVertexShader>(new ScreenVertexShader()),
			std::shared_ptr<ScreenFragmentShader>(new ScreenFragmentShader()))
	{
		Lthreshold = 0.1f;
	}

	ScreenShader::~ScreenShader() {}

	void ScreenShader::beforeLink()
	{
		GLRUN(glBindAttribLocation(getProgramId(), 0, "position"));
		GLRUN(glBindAttribLocation(getProgramId(), 2, "texCoords"));
	}

	void ScreenShader::render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{
		if (screenTexture)
		{
			bindTexture("screenTexture", screenTexture->getId());
		}
		GLRUN(glUniform1f(getUniformLocation("Lthreshold"), Lthreshold));
	}
	void ScreenShader::modMinL(float delta)
	{
		const float minL = 0.01f;
		Lthreshold += delta * 0.001f;
		if (Lthreshold < minL) Lthreshold = minL;
		if (Lthreshold > 1.0f) Lthreshold = 1.0f;
	}
}