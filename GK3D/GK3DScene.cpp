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
#include "SurfaceInstance.h"
#include "Fog.h"
#include "DirLight.h"
#include "Skybox.h"

#include <random>

namespace GK
{
	GK3DScene::GK3DScene(int screenWidth, int screenHeight)
		: Scene(std::vector<std::shared_ptr<Drawable> >(), Camera(screenWidth, screenHeight, glm::vec3(0, 0, -5)))
	{
		GK3DSceneLoader sceneLoader;
		sceneLoader.load();

		if (sceneLoader.getSkyBox())
		{
			this->getDrawables()->push_back(sceneLoader.getSkyBox());
			if (sceneLoader.getSkyBox()->getInstances().size() > 0)
				this->setSkyBox(std::dynamic_pointer_cast<SkyBox>(*sceneLoader.getSkyBox()->getInstances().begin()));
		}
		std::shared_ptr<DrawablesArray> drawables = sceneLoader.getDrawables();
		for (size_t i = 0; i < drawables->size(); i++)
		{
			this->getDrawables()->push_back((*drawables)[i]);
		}

		surfaceInstance = sceneLoader.getSurface();

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

		dayTime = 0;
		dirLights[0] = std::shared_ptr<DirLight>(
			new DirLight(
				glm::vec3(0.644f, -0.417f, 0.602f),
				glm::vec3(0.04f, 0.04f, 0.04f),
				glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(0.5f, 0.5f, 0.5f)));
		dirLights[1] = std::shared_ptr<DirLight>(
			new DirLight(
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(1.0f, 1.0f, 1.0f)));
		fogs[0] = std::shared_ptr<Fog>(new Fog(0.5f, glm::vec3(0.1f, 0.1f, 0.1f)));
		fogs[1] = std::shared_ptr<Fog>(new Fog(0.0f, glm::vec3(0.1f, 0.1f, 0.1f)));
		this->getDirLights()->push_back(dirLights[dayTime]);

		cameraSpotLight = (*spotLights->begin());
		cameraSpotLightOn = true;
		damagedLamp = sceneLoader.getDamagedLamp();
		damagedLightTicks = 0;
		currentDamagedLightMaterial = 0;
		damagedLightGenerator = std::default_random_engine();

		getCamera()->setFront(sceneLoader.getCameraFront());
		getCamera()->setPosition(sceneLoader.getCameraPosition());
	}

	GK3DScene::~GK3DScene() {}

	void GK3DScene::update(GLfloat deltaTime)
	{
		cameraSpotLight->position = getCamera()->getPosition();
		cameraSpotLight->direction = getCamera()->getFront();
		damagedLampFlickering(deltaTime);
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

	void GK3DScene::damagedLampFlickering(GLfloat deltaTime)
	{
		const int lampMaterialsCount = 7;
		const Material lampMaterials[lampMaterialsCount] = {
			Material::WhiteLight,
			Material::RedLight,
			Material::GreenLight,
			Material::BlueLight,
			Material::CyanLight,
			Material::MagentaLight,
			Material::YellowLight
		};
		damagedLightTicks += deltaTime;
		float degrees = glm::degrees(damagedLightTicks / 1000);
		if ((damagedLightTicks > 5000) && (degrees > 360))
		{
			damagedLightTicks = 0;
			std::uniform_int_distribution<int> distribution = std::uniform_int_distribution<int>(0, lampMaterialsCount-1);
			currentDamagedLightMaterial = distribution(damagedLightGenerator);
		}
		if (damagedLamp)
		{
			if (((cos(0.01f*degrees) + 0.5*cos(45 * degrees)) > 1))
				damagedLamp->material.reset(new Material());
			else damagedLamp->material.reset(new Material(lampMaterials[currentDamagedLightMaterial]));
		}
	}

	void GK3DScene::moveSurfaceTex(glm::vec2 offset)
	{
		if (surfaceInstance)
			surfaceInstance->moveTex(offset);
	}

	void GK3DScene::changeSurfaceScale(float modifier)
	{
		if (surfaceInstance)
			surfaceInstance->changeTexScale(modifier);
	}

	void GK3DScene::toggleDayTime()
	{
		dayTime = 1 - dayTime;
		this->getDirLights()->clear();
		this->getDirLights()->push_back(dirLights[dayTime]);
	}

	std::shared_ptr<Fog> GK3DScene::getFog()
	{
		return fogs[dayTime];
	}

	void GK3DScene::toggleSurfaceTexture()
	{
		if (surfaceInstance)
			surfaceInstance->toggleTerrainTex();
	}
	void GK3DScene::toggleSurfaceMarksTexture()
	{
		if (surfaceInstance)
			surfaceInstance->toggleMarksTex();
	}
}