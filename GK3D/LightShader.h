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
		virtual void beforeLink() override;
		virtual void prepareForRender(DrawableInstance drawableInstance, std::weak_ptr<Scene> scene) override;
	};
}

#endif