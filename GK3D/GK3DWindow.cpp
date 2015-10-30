#include "GK3DWindow.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GK
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
	const glm::vec3 startPosition = glm::vec3(0, 0, 5);

	class ObjectVertexShader : public Shader
	{
	public:
		ObjectVertexShader() : Shader(Shader::FromFile("object_vertex_shader.glsl"), ShaderType::VertexShader) {}
		virtual ~ObjectVertexShader() {}
	};
	class ObjectFragmentShader : public Shader
	{
	public:
		ObjectFragmentShader() : Shader(Shader::FromFile("object_fragment_shader.glsl"), ShaderType::FragmentShader) {}
		virtual ~ObjectFragmentShader() {}
	};

	class ObjectShader : public ShaderProgram
	{
	public:
		glm::mat4 viewMatrix;
		GLfloat zoom;
		int screenWidth, screenHeight;

		ObjectShader()
			: ShaderProgram(std::shared_ptr<ObjectVertexShader>(new ObjectVertexShader()),
			std::shared_ptr<ObjectFragmentShader>(new ObjectFragmentShader())),
			viewMatrix(), zoom(45.0f)
		{
		}
		virtual ~ObjectShader() {}
		virtual void update() override
		{
			glm::mat4 model;
			glm::mat4 projection;
			projection = glm::perspective(zoom, ((float)screenWidth) / screenHeight, 0.1f, 100.0f);
			glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		}
		virtual void before_link() override
		{
			glBindAttribLocation(getProgramId(), 0, "position");
			glBindAttribLocation(getProgramId(), 1, "color");
		}
		virtual void beforeRender(Drawable& drawable) override
		{
			glm::vec3 color = drawable.getObjectColor();
			glUniform3f(getUniformLocation("objectColor"), color.r, color.g, color.b);
		}
	};

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable),
		camera(new Camera(startPosition)), cameraMoves(), countedFrames(0)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		std::shared_ptr<ObjectShader> shaderProgram(new ObjectShader());
		shaderProgram->compile();
		shaderProgram->screenWidth = width;
		shaderProgram->screenHeight = height;
		std::vector<Vertex> vertices = {
			// Positions         // Colors
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(0.5f, -0.5f, -0.5f),
			Vertex(0.5f, 0.5f, -0.5f),
			Vertex(0.5f, 0.5f, -0.5f),
			Vertex(-0.5f, 0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),

			Vertex(-0.5f, -0.5f, 0.5f),
			Vertex(0.5f, -0.5f, 0.5f),
			Vertex(0.5f, 0.5f, 0.5f),
			Vertex(0.5f, 0.5f, 0.5f),
			Vertex(-0.5f, 0.5f, 0.5f),
			Vertex(-0.5f, -0.5f, 0.5f),

			Vertex(-0.5f, 0.5f, 0.5f),
			Vertex(-0.5f, 0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, 0.5f),
			Vertex(-0.5f, 0.5f, 0.5f),

			Vertex(0.5f, 0.5f, 0.5f),
			Vertex(0.5f, 0.5f, -0.5f),
			Vertex(0.5f, -0.5f, -0.5f),
			Vertex(0.5f, -0.5f, -0.5f),
			Vertex(0.5f, -0.5f, 0.5f),
			Vertex(0.5f, 0.5f, 0.5f),

			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(0.5f, -0.5f, -0.5f),
			Vertex(0.5f, -0.5f, 0.5f),
			Vertex(0.5f, -0.5f, 0.5f),
			Vertex(-0.5f, -0.5f, 0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),

			Vertex(-0.5f, 0.5f, -0.5f),
			Vertex(0.5f, 0.5f, -0.5f),
			Vertex(0.5f, 0.5f, 0.5f),
			Vertex(0.5f, 0.5f, 0.5f),
			Vertex(-0.5f, 0.5f, 0.5f),
			Vertex(-0.5f, 0.5f, -0.5f)
		};
		box.reset(new Drawable(vertices, glm::vec3(1.0f, 0.5f, 0.31f), shaderProgram));
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
		std::shared_ptr<ObjectShader> shader = std::dynamic_pointer_cast<ObjectShader>(box->getShader());
		shader->screenWidth = getWidth();
		shader->screenHeight = getHeight();
	}

	void GK3DWindow::on_render()
	{
		box->render();
	}
	void GK3DWindow::on_update()
	{
		for (std::set<CameraMovementDirection>::iterator it = cameraMoves.begin();
			it != cameraMoves.end(); it++)
		{
			camera->Move(*it, capTimer.getTicks());
		}
		std::dynamic_pointer_cast<ObjectShader>(box->getShader())->viewMatrix = camera->GetViewMatrix();
		std::dynamic_pointer_cast<ObjectShader>(box->getShader())->zoom = camera->GetZoom();
		box->getShader()->use();
		box->getShader()->update();
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
		}
		if (add)
			cameraMoves.insert(direction);
		else cameraMoves.erase(direction);
	}

	void GK3DWindow::handleMouseMotion()
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		camera->Rotate(x, -y);
	}

	void GK3DWindow::handleMouseWheel(SDL_MouseWheelEvent event)
	{
		camera->Zoom(event.y);
	}
}