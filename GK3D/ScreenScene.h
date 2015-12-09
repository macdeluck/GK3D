#ifndef _GK_SCREENSCENE
#define _GK_SCREENSCENE

#include "Scene.h"

namespace GK
{
	class Drawable;
	class ScreenShader;

	class ScreenScene : public Scene
	{
	private:
		std::shared_ptr<Drawable> screenRect;
	protected:
		virtual void preRender();
	public:
		std::shared_ptr<ScreenShader> screenShader;
		ScreenScene();
		virtual ~ScreenScene();
	};
}

#endif