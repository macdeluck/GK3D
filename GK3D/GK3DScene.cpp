#include "GK3DScene.h"
#include "Drawable.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "Camera.h"

namespace GK
{
	std::vector<Vertex> getBoxVertices();

	GK3DScene::GK3DScene(int screenWidth, int screenHeight)
		: Scene(std::vector<std::shared_ptr<Drawable> >(), Camera(screenWidth, screenHeight, glm::vec3(0, 0, -5)))
	{
		std::shared_ptr<ShaderProgram> objectShader = std::shared_ptr<ShaderProgram>(new ObjectShader());
		objectShader->compile();
		std::shared_ptr<ShaderProgram> lightShader = std::shared_ptr<ShaderProgram>(new LightShader());
		lightShader->compile();
		std::shared_ptr<Drawable> box;
		std::vector<DrawableInstance> boxInstances = 
		{
			DrawableInstance(lightShader,
			std::shared_ptr<Material>(new Material(glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(1.0f, 1.0f, 1.0f))),
			glm::vec3(2.3f, 1.7f, -2.0f),
			glm::vec3(0.5f, 0.5f, 0.5f)),
			DrawableInstance(objectShader,
			std::shared_ptr<Material>(new Material(glm::vec3(1.0f, 0.5f, 0.31f),
				glm::vec3(1.0f, 0.5f, 0.31f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				32.0f)))
		};
		box.reset(
			new Drawable(
			getBoxVertices(),
			boxInstances));
		getDrawables().lock()->push_back(box);
	}

	GK3DScene::~GK3DScene() {}

	void GK3DScene::update(GLfloat deltaTime)
	{
		glm::vec3 lightColor;
		lightColor.x = sin((SDL_GetTicks() / 1000.0f) * 2.0f);
		lightColor.y = sin((SDL_GetTicks() / 1000.0f) * 0.7f);
		lightColor.z = sin((SDL_GetTicks() / 1000.0f) * 1.3f);

		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

		(*getDrawables().lock()->begin())->getInstances().begin()->material->diffuse = diffuseColor;
		(*getDrawables().lock()->begin())->getInstances().begin()->material->ambient = ambientColor;
	}

	std::vector<Vertex> getBoxVertices()
	{
		Vertex v = { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f };
		std::vector<Vertex> result = {
			// Positions         // Normals
			{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f },
			{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f },
			{ 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f },
			{ 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f },
			{ -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f },
			{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f },

			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
			{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f },

			{ -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f },
			{ -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f },
			{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f },
			{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f },
			{ -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f },

			{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f },
			{ 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
			{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
			{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
			{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f },

			{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f },
			{ 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f },
			{ 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f },
			{ 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f },
			{ -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f },
			{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f },

			{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			{ 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
			{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
			{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f }
		};
		return result;
	}
}