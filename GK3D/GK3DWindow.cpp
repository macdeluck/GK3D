#include "GK3DWindow.h"
#include "GK3DScene.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include <cmath>

namespace GK
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable),
		cameraMoves(), countedFrames(0), scene(new GK3DScene(width, height)),
		currentPolygonMode(0)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		fpsTimer.start();
		capTimer.start();
	}
	GK3DWindow::GK3DWindow(const GK3DWindow& otherWindow) : Window(otherWindow) {}
	GK3DWindow& GK3DWindow::operator=(const GK3DWindow& otherWindow)
	{
		Window::operator=(otherWindow);
		return *this;
	}
	GK3DWindow::~GK3DWindow() {}

	void GK3DWindow::handleEvent(SDL_Event& event)
	{
		Window::handleEvent(event);
		if ((event.type == SDL_KEYDOWN) || (event.type == SDL_KEYUP))
			handleKey(event.type, event.key.keysym.sym);
		if (event.type == SDL_MOUSEMOTION)
			handleMouseMotion();
		if (event.type == SDL_MOUSEWHEEL)
			handleMouseWheel(event.wheel);
	}

	void GK3DWindow::on_render()
	{
		scene->render();
	}
	void GK3DWindow::on_update()
	{
		for (std::set<CameraMovementDirection>::iterator it = cameraMoves.begin();
			it != cameraMoves.end(); it++)
		{
			scene->getCamera().lock()->move(*it, (GLfloat)capTimer.getTicks());
		}
		scene->getCamera().lock()->setScreenWidth(getWidth());
		scene->getCamera().lock()->setScreenHeight(getHeight());
		postFrame();
	}

	void GK3DWindow::postFrame()
	{
		++countedFrames;
		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICK_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		}
		capTimer.start();
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}
		std::stringstream ss = std::stringstream("");
		ss << "GK Window (FPS: " << avgFPS << ") ";
		SDL_SetWindowTitle(&(*(this->getWindowHandle().lock())), ss.str().c_str());
	}

	void GK3DWindow::handleKey(Uint32 type, SDL_Keycode code)
	{
		CameraMovementDirection direction;
		bool add = type == SDL_KEYDOWN;
		switch (code)
		{
		case SDLK_w:
			direction = CameraMovementDirection::FORWARD;
			break;
		case SDLK_s:
			direction = CameraMovementDirection::BACKWARD;
			break;
		case SDLK_a:
			direction = CameraMovementDirection::LEFT;
			break;
		case SDLK_d:
			direction = CameraMovementDirection::RIGHT;
			break;
		case SDLK_SPACE:
			direction = CameraMovementDirection::UP;
			break;
		case SDLK_LCTRL:
			direction = CameraMovementDirection::DOWN;
			break;
		case SDLK_ESCAPE:
			if (type == SDL_KEYUP)
				this->close();
			break;
		case SDLK_f:
			if (type == SDL_KEYUP)
			{
				GLint polygonModes[] = { GL_FILL, GL_LINE };
				glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentPolygonMode]);
				currentPolygonMode = 1 - currentPolygonMode;
			}
			break;
		}
		if (add)
			cameraMoves.insert(direction);
		else cameraMoves.erase(direction);
	}

	void GK3DWindow::handleMouseMotion()
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		scene->getCamera().lock()->rotate((GLfloat)x, (GLfloat)-y);
	}

	void GK3DWindow::handleMouseWheel(SDL_MouseWheelEvent event)
	{
		scene->getCamera().lock()->zoom((GLfloat)event.y);
	}
}