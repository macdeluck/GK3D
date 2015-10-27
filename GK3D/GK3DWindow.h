#ifndef _GK3D_WINDOW
#define _GK3D_WINDOW

#include "Window.h"
#include "Drawable.h"

namespace GK
{
	class GK3DWindow : public Window
	{
	public:
		GK3DWindow(int width, int height, std::string title, bool shown = true, bool resizable = false);
		GK3DWindow(const GK3DWindow& otherWindow);
		virtual GK3DWindow& operator=(const GK3DWindow& otherWindow);
		virtual ~GK3DWindow();
	protected:
		virtual void on_render() override;
		virtual void on_update() override;
	private:
		std::shared_ptr<Drawable> box;
	};
}

#endif