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
		glm::mat4 viewMatrix;
		GLfloat zoom;
		int screenWidth, screenHeight;

		ObjectShader();
		virtual ~ObjectShader();
		virtual void update() override;
		virtual void before_link() override;
		virtual void beforeRender(DrawableInstance drawableInstance) override;
	};
}

#endif