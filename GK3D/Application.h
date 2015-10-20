#ifndef _GK_APPLICATION
#define _GK_APPLICATION

#include "Window.h"
#include <map>

namespace GK
{
	class Application
	{
	public:
		Application(int argc, char* argv[]);
		Application(const Application& otherApplication);
		Application& operator=(const Application& otherApplication);
		virtual ~Application();

		int run();
		std::vector<std::string> getArguments();
	private:
		bool Application::is_window_registered(int windowID);
		void Application::register_window(int windowID, Window* window);
		void Application::unregister_window(int windowID);
		std::map<int,Window*> windows;
		std::vector<std::string> arguments;
		void _storeArguments(int argc, char* argv[]);
		void _init();
		void _cleanup();
		void _main();

		friend class Window;
	};
}

#endif