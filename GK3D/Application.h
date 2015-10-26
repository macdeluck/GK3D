#ifndef _GK_APPLICATION
#define _GK_APPLICATION

#include "Window.h"
#include <set>
#include <functional>

namespace GK
{
	class Application
	{
	public:
		Application(int argc, char* argv[]);
		Application(const Application& otherApplication);
		virtual Application& operator=(const Application& otherApplication);
		virtual ~Application();

		int run();
		std::vector<std::string> getArguments();
	protected:
		virtual void started();
		virtual void terminated();
		void register_window(std::weak_ptr<Window> window);
		void unregister_window(std::weak_ptr<Window> window);
		bool is_window_registered(std::weak_ptr<Window> window);
	private:
		std::set<std::weak_ptr<Window>, std::function<bool(std::weak_ptr<Window>, std::weak_ptr<Window>)> > windows;
		std::vector<std::string> arguments;
		void _storeArguments(int argc, char* argv[]);
		void _init();
		void _cleanup();
		void _main();

		friend class Window;
	};
}

#endif