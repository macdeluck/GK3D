#include "Application.h"

int main(int argc, char* argv[])
{
	GK::Application application = GK::Application(argc, argv);
	GK::Window window = GK::Window(application, 800, 600, "GK Window", true, true);
	return application.run();
}