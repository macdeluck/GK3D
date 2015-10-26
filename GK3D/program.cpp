#include "Application.h"
#include "GK3DWindow.h"

int main(int argc, char* argv[])
{
	GK::Application application = GK::Application(argc, argv);
	GK::GK3DWindow window = GK::GK3DWindow(application, 800, 600, "GK Window", true, true);
	return application.run();
}