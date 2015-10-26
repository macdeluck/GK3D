#include "Application.h"

namespace GK
{
	Application::Application(int argc, char* argv[])
		: windows([](std::weak_ptr<Window> p, std::weak_ptr<Window> n)
		{ 
			return p.lock()->getHashCode() < n.lock()->getHashCode();
		})
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

	bool Application::is_window_registered(std::weak_ptr<Window> window)
	{
		return (this->windows.find(window) != this->windows.end());
	}

	void Application::register_window(std::weak_ptr<Window> window)
	{
		if (is_window_registered(window))
			throw new Exception("Window registered multiple times");
		this->windows.insert(window);
	}

	void Application::unregister_window(std::weak_ptr<Window> window)
	{
		this->windows.erase(window);
	}

	int Application::run()
	{
		int result = 0;
		try
		{
			this->started();
			_main();
			this->terminated();
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

				for (std::set<std::weak_ptr<Window> >::iterator i = windows.begin(); i != windows.end(); ++i)
				{
					(*i).lock()->handleEvent(e);
				}
			}

			for (std::set<std::weak_ptr<Window> >::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				(*i).lock()->update();
			}

			for (std::set<std::weak_ptr<Window> >::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				(*i).lock()->render();
			}

			bool allWindowsClosed = true;
			for (std::set<std::weak_ptr<Window> >::iterator i = windows.begin(); i != windows.end(); ++i)
			{
				if ((*i).lock()->isShown())
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

	void Application::started() {}
	void Application::terminated() {}
}