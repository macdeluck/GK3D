#include "Application.h"

namespace GK
{
	Application::Application(int argc, char* argv[])
	{
		_storeArguments(argc, argv);
		_init();
	}

	Application::~Application()
	{
		_cleanup();
	}

	Application::Application(const Application& otherApplication)
	{
		this->arguments = otherApplication.arguments;
	}

	Application& Application::operator=(const Application& otherApplication)
	{
		this->arguments = otherApplication.arguments;
		return *this;
	}

	void Application::_storeArguments(int argc, char* argv[])
	{
		this->arguments = std::vector<std::string>(argc);
		for (int i = 0; i < argc; i++)
		{
			this->arguments[i] = std::string(argv[i]);
		}
	}

	void Application::_init()
	{
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw Exception(std::string("SDL could not initialize! SDL Error: ") + std::string(SDL_GetError()));
		}
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0)
		{
			throw Exception(std::string("SDL could not initialize! SDL Error: ") + std::string(SDL_GetError()));
		}
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) < 0)
		{
			throw Exception(std::string("SDL could not initialize! SDL Error: ") + std::string(SDL_GetError()));
		}
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
		{
			throw Exception(std::string("SDL could not initialize! SDL Error: ") + std::string(SDL_GetError()));
		}
	}

	void Application::_cleanup()
	{
		SDL_Quit();
	}

	bool Application::is_window_registered(int windowID)
	{
		return (this->windows.find(windowID) != this->windows.end());
	}

	void Application::register_window(int windowID, Window* window)
	{
		this->windows[windowID] = window;
	}

	void Application::unregister_window(int windowID)
	{
		this->windows.erase(windowID);
	}

	int Application::run()
	{
		int result = 0;
		try
		{
			_main();
		}
		catch (std::exception& exc)
		{
			std::cerr << exc.what() << std::endl;
			result = 255;
		}
		return result;
	}

	void Application::_main()
	{
		bool quit = false;
		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

				for (std::map<int, Window*>::iterator i = windows.begin(); i != windows.end(); ++i)
				{
					(*i).second->handleEvent(e);
				}
			}

			for (std::map<int, Window*>::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				(*i).second->_update();
			}

			for (std::map<int, Window*>::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				(*i).second->_render();
			}

			bool allWindowsClosed = true;
			for (std::map<int, Window*>::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				if ((*i).second->isShown())
				{
					allWindowsClosed = false;
					break;
				}
			}

			if (allWindowsClosed)
			{
				quit = true;
			}
		}
	}
}