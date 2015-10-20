#ifndef _GK_WINDOW
#define _GK_WINDOW

#include "Common.h"

namespace GK
{
	class Application;

	class Window
	{
	public:
		Window(Application& parentApplication, int width, int height, std::string title, bool shown = true, bool resizable = false);
		Window(const Window& otherWindow);
		Window& operator=(const Window& otherWindow);
		virtual ~Window();

		void handleEvent(SDL_Event& e);
		void focus();
		int getWidth();
		int getHeight();
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
		bool isMaximized();
		bool isShown();
		Application& getApplication();
	protected:
		void render();
		void update();
	private:
		void _render();
		void _update();
		void _refresh();
		void _resize();
		void _gain_gl();
		class WindowState
		{
		public:
			int windowID;
			int width;
			int height;
			bool mouseFocus;
			bool keyboardFocus;
			bool fullScreen;
			bool minimized;
			bool maximized;
			bool shown;
		};

		std::shared_ptr<WindowState> mWindowState;
		std::shared_ptr<SDL_Window> mWindow;
		SDL_GLContext mGLContext;
		Application& parentApplication;
		friend class Application;
	};
}
#endif