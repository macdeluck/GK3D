#ifndef _GK3D_WINDOW
#define _GK3D_WINDOW

#include "Window.h"
#include "Shader.h"
#include "Drawable.h"
#include "Camera.h"
#include "Timer.h"
#include <set>

namespace GK
{
	class GK3DWindow : public Window
	{
	public:
		GK3DWindow(int width, int height, std::string title, bool shown = true, bool resizable = false);
		GK3DWindow(const GK3DWindow& otherWindow);
		virtual GK3DWindow& operator=(const GK3DWindow& otherWindow);
		virtual ~GK3DWindow();
		virtual void handleEvent(SDL_Event& event) override;
	protected:
		virtual void on_render() override;
		virtual void on_update() override;
	private:
		void postFrame();
		void handleKey(Uint32 state, SDL_Keycode key);
		void handleMouseMotion();
		Timer fpsTimer;
		Timer capTimer;
		int countedFrames;
		std::set<CameraMovementDirection> cameraMoves;
		std::shared_ptr<Drawable> box;
		std::shared_ptr<Camera> camera;
	};
}

#endif