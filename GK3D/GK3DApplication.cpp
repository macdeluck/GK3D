#include "GK3DApplication.h"

namespace GK
{
	GK3DApplication::GK3DApplication(int argc, char* argv[]) : Application(argc, argv) {}
	GK3DApplication::~GK3DApplication() {}
	GK3DApplication::GK3DApplication(const GK3DApplication& otherApplication) : Application(otherApplication) {}
	GK3DApplication& GK3DApplication::operator=(const GK3DApplication& otherApplication)
	{
		Application::operator=(otherApplication);
		return *this;
	}

	void GK3DApplication::started()
	{
		std::cout << "Application started..." << std::endl;
		window.reset(new GK::GK3DWindow(800, 600, "GK Window", true, true));
		this->register_window(window);
	}

	void GK3DApplication::terminated()
	{
		std::cout << "Application terminated..." << std::endl;
	}
}