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
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.3f, 0.7f, -1.0f),
			glm::vec3(0.5f, 0.5f, 0.5f)),
			DrawableInstance(objectShader, glm::vec3(1.0f, 0.5f, 0.31f))
		};
		box.reset(
			new Drawable(
			getBoxVertices(),
			boxInstances));
		getDrawables().lock()->push_back(box);
	}

	GK3DScene::~GK3DScene() {}

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