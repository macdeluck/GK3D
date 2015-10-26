#ifndef _GK_GK3DAPPLICATION
#define _GK_GK3DAPPLICATION

#include "Application.h"
#include "GK3DWindow.h"

namespace GK
{
	class GK3DApplication : public Application
	{
	public:
		GK3DApplication(int argc, char* argv[]);
		GK3DApplication(const GK3DApplication& otherApplication);
		GK3DApplication& operator=(const GK3DApplication& otherApplication);
		virtual ~GK3DApplication();

	protected:
		virtual void started() override;
		virtual void terminated() override;
	private:
		std::shared_ptr<GK3DWindow> window;
	};
}

#endif _GK_GK3DAPPLICATION