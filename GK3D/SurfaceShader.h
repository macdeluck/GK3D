#ifndef _GK_SURFACESHADER
#define _GK_SURFACESHADER

#include "Shader.h"
#include "ObjectShader.h"

namespace GK
{
	class SurfaceVertexShader : public Shader
	{
	public:
		SurfaceVertexShader();
		virtual ~SurfaceVertexShader();
	};

	class SurfaceFragmentShader : public Shader
	{
	public:
		SurfaceFragmentShader();
		virtual ~SurfaceFragmentShader();
	};

	class SurfaceShader : public ShaderProgram
	{
	public:
		SurfaceShader();
		virtual ~SurfaceShader();
		virtual void beforeLink() override;
		virtual void prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif