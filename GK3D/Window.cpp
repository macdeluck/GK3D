#include "Window.h"
#include "Application.h"

namespace GK
{
	Window::Window(int width, int height, std::string title, bool shown, bool resizable)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (shown)
			flags |= SDL_WINDOW_SHOWN;
		else flags |= SDL_WINDOW_HIDDEN;
		if (resizable)
			flags |= SDL_WINDOW_RESIZABLE;
		//Create window
		SDL_Window* pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, flags);
		if (pWindow == NULL)
			throw new Exception(std::string("Window could not be created: ") + std::string(SDL_GetError()));

		mGLContext = SDL_GL_CreateContext(pWindow);
		if (mGLContext == NULL)
			throw new Exception(std::string("OpenGL context could not be created: ") + std::string(SDL_GetError()));

		if (SDL_GL_MakeCurrent(pWindow, mGLContext) < 0)
			throw new Exception(std::string("OpenGL context could not be switched: ") + std::string(SDL_GetError()));
		
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			throw Exception(std::string("Error initializing GLEW: ") + std::string((char*)glewGetErrorString(glewError)));
		}

		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			throw Exception(std::string("Warning: Unable to set VSync! SDL Error: " + std::string(SDL_GetError())));
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glViewport(0, 0, width, height);

		this->mWindow.reset(pWindow, [=](SDL_Window* pWindow)
		{
			SDL_DestroyWindow(pWindow);
		});
		this->mWindowState.reset(new WindowState());
		this->mWindowState->windowID = SDL_GetWindowID(pWindow);
		this->mWindowState->width = width;
		this->mWindowState->height = height;
		this->mWindowState->mouseFocus = true;
		this->mWindowState->keyboardFocus = true;
		this->mWindowState->fullScreen = false;
		this->mWindowState->minimized = false;
		this->mWindowState->shown = false;
	}

	Window::Window(const Window& otherWindow)
	{
		this->mWindow = otherWindow.mWindow;
		this->mWindowState = otherWindow.mWindowState;
		this->mGLContext = otherWindow.mGLContext;
	}

	Window& Window::operator=(const Window& otherWindow)
	{
		this->mWindow = otherWindow.mWindow;
		this->mWindowState = otherWindow.mWindowState;
		this->mGLContext = otherWindow.mGLContext;
		return *this;
	}
	
	bool Window::operator==(const Window& otherWindow)
	{
		return otherWindow.mWindowState->windowID == mWindowState->windowID;
	}

	int Window::getHashCode()
	{
		return mWindowState->windowID;
	}

	Window::~Window()
	{
		if (mWindow.unique())
		{
			SDL_GL_DeleteContext(mGLContext);
		}
	}

	void Window::_gain_gl()
	{
		if (SDL_GL_MakeCurrent(&(*mWindow), mGLContext) < 0)
			throw new Exception(std::string("OpenGL context could not be switched: ") + std::string(SDL_GetError()));
	}

	void Window::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowState->windowID)
		{
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
				mWindowState->shown = true;
				break;

			case SDL_WINDOWEVENT_HIDDEN:
				mWindowState->shown = false;
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				mWindowState->width = e.window.data1;
				mWindowState->height = e.window.data2;
				_resize();
				_refresh();
				break;

			case SDL_WINDOWEVENT_EXPOSED:
				_refresh();
				break;

			case SDL_WINDOWEVENT_ENTER:
				mWindowState->mouseFocus = true;
				break;

			case SDL_WINDOWEVENT_LEAVE:
				mWindowState->mouseFocus = false;
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mWindowState->keyboardFocus = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				mWindowState->keyboardFocus = false;
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				mWindowState->minimized = true;
				mWindowState->maximized = false;
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				mWindowState->minimized = false;
				mWindowState->maximized = true;
				break;

			case SDL_WINDOWEVENT_RESTORED:
				mWindowState->minimized = false;
				mWindowState->maximized = false;
				break;

			case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(&(*mWindow));
				break;
			}
		}
	}

	void Window::focus()
	{
		if (!mWindowState->shown)
		{
			SDL_ShowWindow(&(*mWindow));
		}

		SDL_RaiseWindow(&(*mWindow));
	}

	void Window::update()
	{
		on_update();
	}

	void Window::render()
	{
		if (!mWindowState->minimized)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			_gain_gl();
			on_render();
			SDL_GL_SwapWindow(&(*mWindow));
		}
	}

	void Window::_refresh()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(&(*mWindow));
	}

	void Window::_resize()
	{
		glViewport(0, 0, mWindowState->width, mWindowState->height);
	}

	int Window::getWidth()
	{
		return mWindowState->width;
	}

	int Window::getHeight()
	{
		return mWindowState->height;
	}

	bool Window::hasMouseFocus()
	{
		return mWindowState->mouseFocus;
	}

	bool Window::hasKeyboardFocus()
	{
		return mWindowState->keyboardFocus;
	}

	bool Window::isMinimized()
	{
		return mWindowState->minimized;
	}

	bool Window::isMaximized()
	{
		return mWindowState->maximized;
	}

	bool Window::isShown()
	{
		return mWindowState->shown;
	}

	void Window::on_update()
	{
	}

	void Window::on_render()
	{
	}
}