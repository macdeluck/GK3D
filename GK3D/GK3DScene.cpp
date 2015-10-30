#include "GK3DScene.h"
#include "Drawable.h"
#include "ObjectShader.h"
#include "Camera.h"

namespace GK
{
	std::vector<Vertex> getBoxVertices();

	GK3DScene::GK3DScene(int screenWidth, int screenHeight)
		: Scene(std::vector<std::shared_ptr<Drawable> >(), Camera(screenWidth, screenHeight, glm::vec3(0, 0, -5)))
	{
		std::shared_ptr<ShaderProgram> objectShader = std::shared_ptr<ShaderProgram>(new ObjectShader());
		objectShader->compile();
		std::shared_ptr<Drawable> box;
		std::vector<DrawableInstance> boxInstances = 
		{
			DrawableInstance(
			glm::vec3(1.0f, 0.5f, 0.31f),
			glm::vec3(1.0f, 0, 0),
			glm::vec3(0.5f, 0.5f, 0.5f))
		};
		box.reset(
			new Drawable(
			getBoxVertices(),
			boxInstances,
			objectShader));
		getDrawables().lock()->push_back(box);
	}

	GK3DScene::~GK3DScene() {}

	std::vector<Vertex> getBoxVertices()
	{
		std::vector<Vertex> result = {
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
		return result;
	}
}