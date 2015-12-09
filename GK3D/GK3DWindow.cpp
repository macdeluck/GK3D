#include "GK3DWindow.h"
#include "GK3DScene.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "TextureRenderer.h"
#include "ScreenScene.h"
#include "ScreenShader.h"
#include "Image.h"
#include <iomanip>
#include <cmath>
#include <SOIL.h>

namespace GK
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable),
		cameraMoves(), sprintModifier(1), scene(new GK3DScene(width, height)),
		currentPolygonMode(0)
	{
		textureRenderer = std::shared_ptr<TextureRenderer>(new TextureRenderer(width, height, 32));
		screenRenderer = std::shared_ptr<TextureRenderer>(new TextureRenderer(width, height, 32));
		screenScene = std::shared_ptr<ScreenScene>(new ScreenScene());

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
		auto oldCam = scene->getCamera();
		auto tmpCam = std::shared_ptr<Camera>(new Camera(oldCam->getScreenWidth(), oldCam->getScreenHeight(), { -0.230, 0.123f, 1.063f }, { 0.0f, 1.0f, 0.0f }, -54.0f));
		scene->setCamera(tmpCam);
		textureRenderer->lightLoad();
		scene->render();
		Material m = *std::dynamic_pointer_cast<GK3DScene>(this->scene)->planeRect->material;
		m.diffuseTex = textureRenderer->getTexture();
		std::dynamic_pointer_cast<GK3DScene>(this->scene)->planeRect->material = std::shared_ptr<Material>(new Material(m));
		scene->setCamera(oldCam);

		screenRenderer->lightLoad();
		scene->render();

		defaultRenderer()->lightLoad();
		GLint polygonModes[] = { GL_FILL, GL_LINE };
		GLRUN(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		screenScene->screenShader->screenTexture.reset(new Texture(screenRenderer->getTexture()));
		screenScene->render();
		GLRUN(glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentPolygonMode]));
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
		for (std::set<CameraMovementDirection>::iterator it = objectMoves.begin();
			it != objectMoves.end(); it++)
		{
			switch (*it)
			{
			case CameraMovementDirection::FORWARD:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->moveSurfaceTex(glm::vec2(deltaTime * 0.0001f, 0.0f));
				break;
			case CameraMovementDirection::BACKWARD:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->moveSurfaceTex(-glm::vec2(deltaTime * 0.0001f, 0.0f));
				break;
			case CameraMovementDirection::LEFT:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->moveSurfaceTex(glm::vec2(0.0f, deltaTime * 0.0001f));
				break;
			case CameraMovementDirection::RIGHT:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->moveSurfaceTex(-glm::vec2(0.0f, deltaTime * 0.0001f));
				break;
			case CameraMovementDirection::UP:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->changeSurfaceScale(deltaTime * 0.01f);
				break;
			case CameraMovementDirection::DOWN:
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->changeSurfaceScale(-deltaTime * 0.01f);
				break;
			default:
				break;
			}
		}
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
		CameraMovementDirection objectDirection;
		bool add = type == SDL_KEYDOWN;
		bool cameraMove = true;
		bool objectMove = false;
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
		case SDLK_UP:
			objectDirection = CameraMovementDirection::FORWARD;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_DOWN:
			objectDirection = CameraMovementDirection::BACKWARD;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_LEFT:
			objectDirection = CameraMovementDirection::LEFT;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_RIGHT:
			objectDirection = CameraMovementDirection::RIGHT;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_RSHIFT:
			objectDirection = CameraMovementDirection::UP;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_RCTRL:
			objectDirection = CameraMovementDirection::DOWN;
			objectMove = true;
			cameraMove = false;
			break;
		case SDLK_ESCAPE:
			cameraMove = false;
			if (type == SDL_KEYUP)
				this->close();
			break;
		case SDLK_f:
			cameraMove = false;
			if (type == SDL_KEYUP)
			{
				GLint polygonModes[] = { GL_FILL, GL_LINE };
				GLRUN(glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentPolygonMode]));
				currentPolygonMode = 1 - currentPolygonMode;
			}
			break;
		case SDLK_t:
			cameraMove = false;
			if (type == SDL_KEYUP)
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->toggleSurfaceTexture();
			break;
		case SDLK_y:
			cameraMove = false;
			if (type == SDL_KEYUP)
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->toggleSurfaceMarksTexture();
			break;
		case SDLK_p:
			cameraMove = false;
			if (type == SDL_KEYUP)
				std::dynamic_pointer_cast<GK3DScene>(this->scene)->toggleDayTime();
			break;
		case SDLK_LSHIFT:
			cameraMove = false;
			if (type == SDL_KEYUP)
				sprintModifier = 1;
			else sprintModifier = 3;
			break;
		}
		if (cameraMove)
		{
			if (add)
				cameraMoves.insert(direction);
			else cameraMoves.erase(direction);
		}
		if (objectMove)
		{
			if (add)
				objectMoves.insert(objectDirection);
			else objectMoves.erase(objectDirection);
		}
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