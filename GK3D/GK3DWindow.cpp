#include "GK3DWindow.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GK
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	class GK3DVertexShader : public Shader
	{
	public:
		GK3DVertexShader() : Shader(Shader::FromFile("vertex_shader.glsl"), ShaderType::VertexShader) {}
		virtual ~GK3DVertexShader() {}
	};
	class GK3DFragmentShader : public Shader
	{
	public:
		GK3DFragmentShader() : Shader(Shader::FromFile("fragment_shader.glsl"), ShaderType::FragmentShader) {}
		virtual ~GK3DFragmentShader() {}
	};

	class GK3DShaderProgram : public ShaderProgram
	{
	public:
		glm::mat4 viewMatrix;
		int screenWidth, screenHeight;

		GK3DShaderProgram()
			: ShaderProgram(std::shared_ptr<GK3DVertexShader>(new GK3DVertexShader()),
			std::shared_ptr<GK3DFragmentShader>(new GK3DFragmentShader())),
			viewMatrix()
		{
		}
		virtual ~GK3DShaderProgram() {}
		virtual void update() override
		{
			glm::mat4 model;
			glm::mat4 projection;
			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			projection = glm::perspective(45.0f, ((float)screenWidth) / screenHeight, 0.1f, 100.0f);
			glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		}
		virtual void before_link() override
		{
			glBindAttribLocation(getProgramId(), 0, "position");
			glBindAttribLocation(getProgramId(), 1, "color");
		}
	};

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable), camera(new Camera()), cameraMoves(), countedFrames(0)
	{
		std::shared_ptr<GK3DShaderProgram> shaderProgram(new GK3DShaderProgram());
		shaderProgram->compile();
		shaderProgram->screenWidth = width;
		shaderProgram->screenHeight = height;
		std::vector<Vertex> vertices = {
			// Positions         // Colors
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),

			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),

			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),

			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),

			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),

			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f)
		};
		box.reset(new Drawable(vertices, shaderProgram));
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

		std::shared_ptr<GK3DShaderProgram> shader = std::dynamic_pointer_cast<GK3DShaderProgram>(box->getShader());
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
		std::dynamic_pointer_cast<GK3DShaderProgram>(box->getShader())->viewMatrix = camera->GetViewMatrix();
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
		ss << "GK Window (" << avgFPS << ") ";
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
		}
		if (add)
			cameraMoves.insert(direction);
		else cameraMoves.erase(direction);
	}
}