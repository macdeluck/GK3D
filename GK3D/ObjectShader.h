#ifndef _GK_OBJECTSHADER
#define _GK_OBJECTSHADER

#include "Common.h"
#include "Shader.h"
#include "Drawable.h"

namespace GK
{
	class ObjectVertexShader : public Shader
	{
	public:
		ObjectVertexShader();
		virtual ~ObjectVertexShader();
	};
	class ObjectFragmentShader : public Shader
	{
	public:
		ObjectFragmentShader();
		virtual ~ObjectFragmentShader();
	};

	class ObjectShader : public ShaderProgram
	{
	public:
		ObjectShader();
		virtual ~ObjectShader();
		virtual void beforeLink() override;
		virtual void prepareForRender(std::shared_ptr<DrawableInstance> drawableInstance, std::shared_ptr<Scene> scene) override;
	};
}

#endif