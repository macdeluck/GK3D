#include "GK3DWindow.h"
#include "GK3DScene.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include <iomanip>
#include <cmath>

namespace GK
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable),
		cameraMoves(), sprintModifier(1), scene(new GK3DScene(width, height)),
		currentPolygonMode(0)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		fpsTimer.start();
		deltaTimer.start();
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
		if (event.type == SDL_MOUSEBUTTONDOWN)
			handleMouseButton(event.button);
	}

	void GK3DWindow::onBeginFrame()
	{
		fpsTimer.start();
	}

	void GK3DWindow::onRender()
	{
		scene->render();
	}

	void GK3DWindow::onUpdate()
	{
		GLfloat deltaTime = (GLfloat)deltaTimer.getTicks();
		deltaTimer.start();
		for (std::set<CameraMovementDirection>::iterator it = cameraMoves.begin();
			it != cameraMoves.end(); it++)
		{
			scene->getCamera()->move(*it, sprintModifier*deltaTime);
		}
		scene->update(deltaTime);
		scene->getCamera()->setScreenWidth(getWidth());
		scene->getCamera()->setScreenHeight(getHeight());
	}

	void GK3DWindow::onEndFrame()
	{
		int frameTicks = fpsTimer.getTicks();
		float fps = ((float)SCREEN_TICK_PER_FRAME / ((frameTicks < SCREEN_TICK_PER_FRAME) ? SCREEN_TICK_PER_FRAME : frameTicks)) * SCREEN_FPS;
		std::stringstream ss = std::stringstream("");
		glm::vec3 cameraPos = scene->getCamera()->getPosition();
		glm::vec3 cameraFront = scene->getCamera()->getFront();
		ss << std::fixed << std::setprecision(1) << std::setw(3) << "GK Window (FPS: " << fps << ") (Pos: ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraPos.x << ", ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraPos.y << ", ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraPos.z << ") (Front: ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraFront.x << ", ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraFront.y << ", ";
		ss << std::fixed << std::setprecision(3) << std::setw(9) << cameraFront.z << ")";
		SDL_SetWindowTitle(&(*(this->getWindowHandle().lock())), ss.str().c_str());
		if (frameTicks < SCREEN_TICK_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		}
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
		case SDLK_LSHIFT:
			if (type == SDL_KEYUP)
				sprintModifier = 1;
			else sprintModifier = 3;
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
		scene->getCamera()->rotate((GLfloat)x, (GLfloat)-y);
	}

	void GK3DWindow::handleMouseWheel(SDL_MouseWheelEvent event)
	{
		scene->getCamera()->zoom((GLfloat)event.y);
	}

	void GK3DWindow::handleMouseButton(SDL_MouseButtonEvent event)
	{
		if ((event.type = SDL_MOUSEBUTTONUP) && (event.button == SDL_BUTTON_LEFT))
			std::dynamic_pointer_cast<GK3DScene>(this->scene)->toggleCameraLight();
	}
}