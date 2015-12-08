#ifndef _GK_SCREENSCENE
#define _GK_SCREENSCENE

#include "Scene.h"

namespace GK
{
	class Drawable;
	class ScreenShader;

	class ScreenScene : Scene
	{
	private:
		std::shared_ptr<Drawable> screenRect;
		std::shared_ptr<ScreenShader> screenShader;
	protected:
		virtual void preRender();
		virtual void postRender();
	public:
		virtual void update(GLfloat deltaTime);
		ScreenScene(int screenWidth, int screenHeight);
		virtual ~ScreenScene();
	};
}

#endif