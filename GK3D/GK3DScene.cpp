#include "GK3DScene.h"
#include "Drawable.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelLoader.h"
#include "GK3DSceneLoader.h"

namespace GK
{
	GK3DScene::GK3DScene(int screenWidth, int screenHeight)
		: Scene(std::vector<std::shared_ptr<Drawable> >(), Camera(screenWidth, screenHeight, glm::vec3(0, 0, -5)))
	{
		GK3DSceneLoader sceneLoader;
		sceneLoader.load();

		std::shared_ptr<DrawablesArray> drawables = sceneLoader.getDrawables();
		for (size_t i = 0; i < drawables->size(); i++)
		{
			this->getDrawables()->push_back((*drawables)[i]);
		}

		std::shared_ptr<PointLightsArray> pointLights = sceneLoader.getPointLights();
		for (size_t i = 0; i < pointLights->size(); i++)
		{
			this->getPointLights()->push_back((*pointLights)[i]);
		}

		std::shared_ptr<SpotLightsArray> spotLights = sceneLoader.getSpotLights();
		for (size_t i = 0; i < spotLights->size(); i++)
		{
			this->getSpotLights()->push_back((*spotLights)[i]);
		}
		cameraSpotLight = (*spotLights->begin());
		cameraSpotLightOn = true;

		getCamera()->setFront(sceneLoader.getCameraFront());
		getCamera()->setPosition(sceneLoader.getCameraPosition());
	}

	GK3DScene::~GK3DScene() {}

	void GK3DScene::update(GLfloat deltaTime)
	{
		cameraSpotLight->position = getCamera()->getPosition();
		cameraSpotLight->direction = getCamera()->getFront();
	}

	void GK3DScene::toggleCameraLight()
	{
		if (cameraSpotLightOn)
		{
			cameraSpotLight->material.reset(new Material());
		}
		else
		{
			cameraSpotLight->material.reset(new Material(glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		cameraSpotLightOn = !cameraSpotLightOn;
	}
}