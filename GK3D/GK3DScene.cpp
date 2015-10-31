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
			glm::vec3(1.2f, 1.0f, 2.0f),
			glm::vec3(0.5f, 0.5f, 0.5f))
		};
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)
		};
		std::shared_ptr<Material> cubesMaterial;
		cubesMaterial.reset(new Material(
			glm::vec3(0.0215f, 0.1745f, 0.0215f),
			glm::vec3(0.07568f, 0.61424f, 0.07568f),
			glm::vec3(0.633f, 0.727811f, 0.633f),
			0.6f * 128.f));
		for (GLuint i = 0; i < 10; i++)
		{
			GLfloat angle = 20.0f * i;
			boxInstances.push_back(DrawableInstance(objectShader,
				cubesMaterial,
				cubePositions[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				angle * 1.0f,
				angle * 0.3f,
				angle * 0.5f));
		}
		box.reset(
			new Drawable(
			getBoxVertices(),
			boxInstances));
		getDrawables().lock()->push_back(box);
	}

	GK3DScene::~GK3DScene() {}

	void GK3DScene::update(GLfloat deltaTime)
	{
		/*
		glm::vec3 lightColor;
		lightColor.x = sin((SDL_GetTicks() / 1000.0f) * 2.0f);
		lightColor.y = sin((SDL_GetTicks() / 1000.0f) * 0.7f);
		lightColor.z = sin((SDL_GetTicks() / 1000.0f) * 1.3f);

		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

		(*getDrawables().lock()->begin())->getInstances().begin()->material->diffuse = diffuseColor;
		(*getDrawables().lock()->begin())->getInstances().begin()->material->ambient = ambientColor;*/
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