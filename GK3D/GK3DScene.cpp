#include "GK3DScene.h"
#include "Drawable.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelLoader.h"

namespace GK
{
	GK3DScene::GK3DScene(int screenWidth, int screenHeight)
		: Scene(std::vector<std::shared_ptr<Drawable> >(), Camera(screenWidth, screenHeight, glm::vec3(0, 0, -5)))
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		ModelLoader().loadModel("cube.obj", &vertices, &indices);

		std::shared_ptr<ShaderProgram> objectShader = std::shared_ptr<ShaderProgram>(new ObjectShader());
		objectShader->compile();
		std::shared_ptr<ShaderProgram> lightShader = std::shared_ptr<ShaderProgram>(new LightShader());
		lightShader->compile();
		
		std::shared_ptr<Material> lightMaterial;
		lightMaterial.reset(new Material(glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f)));

		std::shared_ptr<Material> cameraSpotLightMaterial;
		cameraSpotLightMaterial.reset(new Material(glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f)));

		spotLight.reset(new SpotLightInstance(lightShader, cameraSpotLightMaterial));
		spotLight->position = getCamera()->getPosition();
		spotLight->direction = getCamera()->getFront();
		(*this->getSpotLights())[0] = spotLight;
		spotLightOn = true;

		std::shared_ptr<PointLightInstance> lightSource;
		lightSource.reset(new PointLightInstance(lightShader, lightMaterial,
			1.0f, 0.09f, 0.032f,
			glm::vec3(1.2f, 1.0f, 2.0f),
			glm::vec3(0.25f, 0.25f, 0.25f)));
		(*this->getLightSources())[0] = lightSource;

		std::shared_ptr<PointLightInstance> lightSource2;
		lightSource2.reset(new PointLightInstance(lightShader, lightMaterial,
			1.0f, 0.09f, 0.032f,
			glm::vec3(0, 4.0f, -11.25f),
			glm::vec3(0.25f, 0.25f, 0.25f)));
		(*this->getLightSources())[1] = lightSource2;

		std::vector<std::shared_ptr<DrawableInstance> > boxInstances = { lightSource, lightSource2 };

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
			boxInstances.push_back(std::shared_ptr<DrawableInstance>( new DrawableInstance(objectShader,
				cubesMaterial,
				cubePositions[i],
				glm::vec3(0.5f, 0.5f, 0.5f),
				angle * 1.0f,
				angle * 0.3f,
				angle * 0.5f)));
		}

		std::shared_ptr<Drawable> box;
		box.reset(
			new Drawable(
			vertices,
			indices,
			boxInstances));
		getDrawables()->push_back(box);
	}

	GK3DScene::~GK3DScene() {}

	void GK3DScene::update(GLfloat deltaTime)
	{
		spotLight->position = getCamera()->getPosition();
		spotLight->direction = getCamera()->getFront();
	}

	void GK3DScene::toggleCameraLight()
	{
		if (spotLightOn)
		{
			spotLight->material.reset(new Material());
		}
		else
		{
			spotLight->material.reset(new Material(glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(0.5f, 0.5f, 0.5f),
				glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		spotLightOn = !spotLightOn;
	}
}