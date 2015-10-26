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
		virtual Window& operator=(const Window& otherWindow);
		virtual ~Window();

		virtual void handleEvent(SDL_Event& e);
		virtual void focus();
		virtual int getWidth();
		virtual int getHeight();
		virtual bool hasMouseFocus();
		virtual bool hasKeyboardFocus();
		virtual bool isMinimized();
		virtual bool isMaximized();
		virtual bool isShown();
		virtual Application& getApplication();
	protected:
		virtual void render();
		virtual void update();
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