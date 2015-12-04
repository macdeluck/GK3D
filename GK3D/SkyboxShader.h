#ifndef _GK_SKYBOXSHADER
#define _GK_SKYBOXSHADER

#include "Shader.h"

namespace GK
{
	class SkyboxVertexShader : public Shader
	{
	public:
		SkyboxVertexShader();
		virtual ~SkyboxVertexShader();
	};

	class SkyboxFragmentShader : public Shader
	{
	public:
		SkyboxFragmentShader();
		virtual ~SkyboxFragmentShader();
	};

	class SkyboxShader : public ShaderProgram
	{
	public:
		SkyboxShader();
		virtual ~SkyboxShader();
		virtual void beforeLink() override;
		virtual void postRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene);
	protected:
		virtual void prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif