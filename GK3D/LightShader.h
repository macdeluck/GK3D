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
		glm::mat4 viewMatrix;
		GLfloat zoom;
		int screenWidth, screenHeight;

		LightShader();
		virtual ~LightShader();
		virtual void update() override;
		virtual void before_link() override;
		virtual void beforeRender(DrawableInstance drawableInstance) override;
	};
}

#endif