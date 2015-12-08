#include "ScreenShader.h"
#include "Shader.h"

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
	}

	ScreenShader::~ScreenShader() {}

	void ScreenShader::beforeLink()
	{
		GLRUN(glBindAttribLocation(getProgramId(), 0, "position"));
	}

	void ScreenShader::render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene)
	{

	}
}