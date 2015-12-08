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
		std::shared_ptr<ScreenShader> screenShader;
	protected:
		virtual void preRender();
	public:
		ScreenScene();
		virtual ~ScreenScene();
	};
}

#endif