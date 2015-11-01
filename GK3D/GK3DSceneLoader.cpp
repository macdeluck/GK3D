#include "GK3DSceneLoader.h"
#include "Drawable.h"
#include "SpotLightInstance.h"
#include "PointLightInstance.h"
#include "ModelLoader.h"
#include "ObjectShader.h"
#include "LightShader.h"

#include <random>

namespace GK
{
	GK3DSceneLoader::GK3DSceneLoader()
		: pointLights(new PointLightsArray()),
		spotLights(new SpotLightsArray()),
		drawables(new DrawablesArray()),
		cameraPosition(glm::vec3(0, 0, 0)),
		cameraFront(glm::vec3(0, 0, 0))
	{}

	GK3DSceneLoader::~GK3DSceneLoader() {}

	std::shared_ptr<PointLightsArray> GK3DSceneLoader::getPointLights() { return pointLights; }
	std::shared_ptr<SpotLightsArray> GK3DSceneLoader::getSpotLights() { return spotLights; }
	std::shared_ptr<DrawablesArray> GK3DSceneLoader::getDrawables() { return drawables; }
	glm::vec3 GK3DSceneLoader::getCameraFront() { return cameraFront; }
	glm::vec3 GK3DSceneLoader::getCameraPosition() { return cameraPosition; }

	const int SHADER_OBJECT = 0;
	const int SHADER_LIGHT = 1;

	const int MODEL_CUBE = 0;
	const int MODEL_BENCH = 1;
	const int MODEL_LAMP = 2;
	const int MODEL_FIR = 3;
	const int MODEL_FLASHLIGHT = 4;

	void GK3DSceneLoader::load()
	{
		loadShaders();
		loadModelsData();
		createInstances();
		buildSurface();
		buildScene();
	}

	typedef std::map<int, std::shared_ptr<ShaderProgram> > ShadersDic;

	void GK3DSceneLoader::loadShaders()
	{
		shaders[SHADER_OBJECT] = std::shared_ptr<ShaderProgram>(new ObjectShader());
		shaders[SHADER_LIGHT] = std::shared_ptr<ShaderProgram>(new LightShader());
		for (ShadersDic::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
		{
			it->second->compile();
		}
	}

	typedef std::pair<std::vector<Vertex>, std::vector<GLuint> > ModelData;
	void GK3DSceneLoader::loadModelsData()
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		ModelLoader modelLoader;

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("assets/cube.obj", &vertices, &indices);
		modelsData[MODEL_CUBE] = ModelData(vertices, indices);

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("assets/bench.obj", &vertices, &indices);
		modelsData[MODEL_BENCH] = ModelData(vertices, indices);

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("assets/lamp.obj", &vertices, &indices);
		modelsData[MODEL_LAMP] = ModelData(vertices, indices);

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("assets/fir.obj", &vertices, &indices);
		modelsData[MODEL_FIR] = ModelData(vertices, indices);

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("assets/flashlight.obj", &vertices, &indices);
		modelsData[MODEL_FLASHLIGHT] = ModelData(vertices, indices);
	}

	void GK3DSceneLoader::createInstances()
	{
		std::shared_ptr<SpotLightInstance> spotLight = std::shared_ptr<SpotLightInstance>(new SpotLightInstance(
			shaders[SHADER_LIGHT],
			std::shared_ptr<Material>(new Material(Material::WhiteLight))));
		spotLights->push_back(spotLight);

		createBenches();
		createLamps();
		createFirs();
		createFlashLight();
	}

	void GK3DSceneLoader::createBenches()
	{
		const int benchCount = 2;
		glm::vec3 benchScale = { 0.001f, 0.001f, 0.001f };
		glm::vec3 benchPositions[benchCount] = {
			{ 0.0f, 0.0f, 0.0f },
			{ 0.739f, 0.0f, 0.395f }
		};
		glm::vec3 benchAngles[benchCount] = {
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 138.0f, 0.0f }
		};
		for (size_t i = 0; i < benchCount; i++)
		{
			std::shared_ptr<DrawableInstance> benchInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
				shaders[SHADER_OBJECT],
				std::shared_ptr<Material>(new Material(Material::Copper)),
				benchPositions[i],
				benchScale,
				benchAngles[i].x,
				benchAngles[i].y,
				benchAngles[i].z));
			instances[MODEL_BENCH].push_back(benchInstance);
		}
	}

	void GK3DSceneLoader::createLamps()
	{
		const int lampCount = 2;
		glm::vec3 lampScale = { 0.01f, 0.01f, 0.01f };
		glm::vec3 lampPositions[lampCount] = {
			{ 0.1f, 0.0f, 0.4f },
			{ 1.5f, 0.0f, -0.275f }
		};
		glm::vec3 lampAngles[lampCount] = {
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f }
		};
		for (size_t i = 0; i < lampCount; i++)
		{
			std::shared_ptr<DrawableInstance> lampInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
				shaders[SHADER_OBJECT],
				std::shared_ptr<Material>(new Material(Material::Silver)),
				lampPositions[i],
				lampScale,
				lampAngles[i].x,
				lampAngles[i].y,
				lampAngles[i].z));
			instances[MODEL_LAMP].push_back(lampInstance);

			glm::vec3 lightPosition = lampPositions[i];
			lightPosition.x += 0.0055f;
			lightPosition.y += 0.27f;
			lightPosition.z += -0.012f;
			std::shared_ptr<PointLightInstance> lampLight = std::shared_ptr<PointLightInstance>(new PointLightInstance(
				shaders[SHADER_LIGHT],
				std::shared_ptr<Material>(new Material(Material::WhiteLight)),
				1.0f, 0.7f, 1.8f,
				lightPosition,
				{ 0.01f, 0.01f, 0.01f },
				lampAngles[i].x,
				lampAngles[i].y,
				lampAngles[i].z));
			instances[MODEL_CUBE].push_back(lampLight);
			pointLights->push_back(lampLight);
		}
	}

	void GK3DSceneLoader::createFirs()
	{
		std::default_random_engine generator = std::default_random_engine(123);
		std::uniform_real_distribution<GLfloat> distribution = std::uniform_real_distribution<GLfloat>(-3.0f, 3.0f);

		const int firCount = 50;
		glm::vec3 firScale = { 0.0005f, 0.0005f, 0.0005f };
		for (size_t i = 0; i < firCount; i++)
		{
			glm::vec3 firPosition = glm::vec3(distribution(generator), 0.0f, distribution(generator));
			std::shared_ptr<DrawableInstance> firInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
				shaders[SHADER_OBJECT],
				std::shared_ptr<Material>(new Material(Material::GreenRubber)),
				firPosition,
				firScale));
			instances[MODEL_FIR].push_back(firInstance);
		}
	}

	void GK3DSceneLoader::createFlashLight()
	{
		glm::vec3 flashLightScale = { 0.005f, 0.005f, 0.005f };
		glm::vec3 flashLightPosition = { 0.3f, 0.025f, 0.55f };
		glm::vec3 flashLightAngle = { -5.0f, 90.0f, 0.0f };
		std::shared_ptr<DrawableInstance> flashLightInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
			shaders[SHADER_OBJECT],
			std::shared_ptr<Material>(new Material(Material::BlackPlastic)),
			flashLightPosition,
			flashLightScale,
			flashLightAngle.x,
			flashLightAngle.y,
			flashLightAngle.z));
		instances[MODEL_FLASHLIGHT].push_back(flashLightInstance);
	}

	typedef std::map<int, std::vector<std::shared_ptr<DrawableInstance> > > InstancesDic;

	void GK3DSceneLoader::buildScene()
	{
		cameraPosition = glm::vec3(0.5f, 0.17f, -0.82f);
		cameraFront = glm::vec3(-0.46f, -0.12f, 0.88f);
		for (InstancesDic::const_iterator it = instances.begin(); it != instances.end(); ++it)
		{
			drawables->push_back(std::shared_ptr<Drawable>(
				new Drawable(modelsData[it->first].first, modelsData[it->first].second, it->second)));
		}
	}

	void GK3DSceneLoader::buildSurface()
	{
		std::vector<Vertex> vertices = {
			{ 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
			{ -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{ -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
		};
		std::vector<GLuint> indices = { 1, 2, 0, 1, 2, 3 };
		std::shared_ptr<DrawableInstance> surfaceInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
			shaders[SHADER_OBJECT],
			std::shared_ptr<Material>(new Material(
				glm::vec3(0.1f, 0.2, 0.1f),
				glm::vec3(0.2f, 0.5f, 0.2f),
				glm::vec3(0.0f, 0.0f, 0.0f))),
			glm::vec3(0.0f, -0.005f, 0.0f),
			glm::vec3(3.0f, 3.0f, 3.0f)));
		drawables->push_back(std::shared_ptr<Drawable>(
			new Drawable(vertices, indices, { surfaceInstance })));
	}
}