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
	class Scene;
	class ScreenScene;
	class TextureRenderer;

	class GK3DWindow : public Window
	{
	public:
		GK3DWindow(int width, int height, std::string title, bool shown = true, bool resizable = false);
		GK3DWindow(const GK3DWindow& otherWindow);
		virtual GK3DWindow& operator=(const GK3DWindow& otherWindow);
		virtual ~GK3DWindow();
		virtual void handleEvent(SDL_Event& event) override;
	protected:
		virtual void onBeginFrame() override;
		virtual void onUpdate() override;
		virtual void onRender() override;
		virtual void onEndFrame() override;
	private:
		int currentPolygonMode;
		void handleKey(Uint32 state, SDL_Keycode key);
		void handleMouseMotion();
		void handleMouseWheel(SDL_MouseWheelEvent event);
		void handleMouseButton(SDL_MouseButtonEvent event);
		Timer fpsTimer;
		Timer deltaTimer;
		int sprintModifier;
		std::set<CameraMovementDirection> cameraMoves;
		std::set<CameraMovementDirection> objectMoves;
		std::shared_ptr<Scene> scene;

		std::shared_ptr<ScreenScene> screenScene;
		std::shared_ptr<TextureRenderer> textureRenderer;
	};
}

#endif