#ifndef _GK_ENVSHADER
#define _GK_ENVSHADER

#include "Shader.h"

namespace GK
{
	class EnvVertexShader : public Shader
	{
	public:
		EnvVertexShader();
		virtual ~EnvVertexShader();
	};

	class EnvFragmentShader : public Shader
	{
	public:
		EnvFragmentShader();
		virtual ~EnvFragmentShader();
	};

	class EnvShader : public ShaderProgram
	{
	public:
		EnvShader();
		virtual ~EnvShader();
		virtual void beforeLink() override;
	protected:
		virtual void render(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif