#ifndef _GK_SCREENSHADER
#define _GK_SCREENSHADER

#include "Shader.h"

namespace GK
{
	class ScreenVertexShader : public Shader
	{
	public:
		ScreenVertexShader();
		virtual ~ScreenVertexShader();
	};

	class ScreenFragmentShader : public Shader
	{
	public:
		ScreenFragmentShader();
		virtual ~ScreenFragmentShader();
	};

	class ScreenShader : public ShaderProgram
	{
	public:
		ScreenShader();
		virtual ~ScreenShader();
		virtual void beforeLink() override;
		virtual void render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif