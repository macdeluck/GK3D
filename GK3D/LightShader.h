#ifndef _GK_LIGHTSHADER
#define _GK_LIGHTSHADER

#include "Common.h"
#include "ObjectShader.h"

namespace GK
{
	class LightFragmentShader : public Shader
	{
	public:
		LightFragmentShader();
		virtual ~LightFragmentShader();
	};

	class LightShader : public ShaderProgram
	{
	public:
		LightShader();
		virtual ~LightShader();
		virtual void beforeLink() override;
		virtual void render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif