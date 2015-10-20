#include "Application.h"

int main(int argc, char* argv[])
{
	GK::Application application = GK::Application(argc, argv);
	GK::Window window = GK::Window(application, 800, 600, "GK Window");
	GK::Window window2 = GK::Window(application, 800, 600, "GK Window 2");
	return application.run();
}