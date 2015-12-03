#include "GK3DSceneLoader.h"
#include "Drawable.h"
#include "SpotLightInstance.h"
#include "PointLightInstance.h"
#include "SurfaceInstance.h"
#include "ModelLoader.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "Image.h"

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

	void GK3DSceneLoader::loadModelsData()
	{
		ModelLoader modelLoader;
		ModelData modelData;

		modelLoader.loadModel("assets/cube.obj", &modelData);
		modelsData[MODEL_CUBE] = std::shared_ptr<ModelData>(new ModelData(modelData));

		modelLoader.loadModel("assets/bench.obj", &modelData);
		modelsData[MODEL_BENCH] = std::shared_ptr<ModelData>(new ModelData(modelData));

		modelLoader.loadModel("assets/lamp.obj", &modelData);
		modelsData[MODEL_LAMP] = std::shared_ptr<ModelData>(new ModelData(modelData));

		modelLoader.loadModel("assets/fir.obj", &modelData);
		modelsData[MODEL_FIR] = std::shared_ptr<ModelData>(new ModelData(modelData));

		modelLoader.loadModel("assets/flashlight.obj", &modelData);
		modelsData[MODEL_FLASHLIGHT] = std::shared_ptr<ModelData>(new ModelData(modelData));
	}

	void GK3DSceneLoader::createInstances()
	{
		std::shared_ptr<SpotLightInstance> spotLight = std::shared_ptr<SpotLightInstance>(new SpotLightInstance(
			shaders[SHADER_LIGHT],
			std::shared_ptr<Material>(new Material(Material::WhiteLight))));
		spotLights->push_back(spotLight);

		createCubes();
		createBenches();
		createLamps();
		createFirs();
		createFlashLight();
	}

	void GK3DSceneLoader::createCubes()
	{
		int currentModel = MODEL_CUBE;
		const int modelsCount = 1;
		glm::vec3 modelScale = { 0.1f, 0.1f, 0.1f };
		glm::vec3 modelPositions[modelsCount] = {
			{ 0.5f, 0.5f, 0.0f }
		};
		glm::vec3 modelsAngles[modelsCount] = {
			{ 0.0f, 0.0f, 0.0f }
		};
		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::Emerald));
		createGenericModel(currentModel, modelsCount, modelScale, modelPositions, modelsAngles, defaultMaterial);
	}

	void GK3DSceneLoader::createBenches()
	{
		int currentModel = MODEL_BENCH;
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
		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::Copper));
		createGenericModel(currentModel, benchCount, benchScale, benchPositions, benchAngles, defaultMaterial);
	}

	void GK3DSceneLoader::createLampCallback(int modelType, std::shared_ptr<DrawableInstance> pointLight)
	{
		glm::vec3 lightPosition = pointLight->position;
		lightPosition.x += 0.0055f;
		lightPosition.y += 0.27f;
		lightPosition.z += -0.012f;
		std::shared_ptr<PointLightInstance> lampLight = std::shared_ptr<PointLightInstance>(new PointLightInstance(
			shaders[SHADER_LIGHT],
			std::shared_ptr<Material>(new Material(Material::WhiteLight)),
			1.0f, 0.7f, 1.8f,
			lightPosition,
			{ 0.01f, 0.01f, 0.01f },
			pointLight->angleX,
			pointLight->angleY,
			pointLight->angleZ));
		instances[MODEL_CUBE].push_back(lampLight);
		pointLights->push_back(lampLight);
	}

	void GK3DSceneLoader::createLamps()
	{
		const int currentType = MODEL_LAMP;
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
		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::Silver));
		createGenericModel(currentType, lampCount, lampScale, lampPositions, lampAngles, defaultMaterial, std::bind(&GK3DSceneLoader::createLampCallback, this, std::placeholders::_1, std::placeholders::_2));
	}

	std::shared_ptr<PointLightInstance> GK3DSceneLoader::getDamagedLamp()
	{
		if (pointLights->size() > 0)
			return (*pointLights->begin());
		return std::shared_ptr<PointLightInstance>(nullptr);
	}

	std::shared_ptr<SurfaceInstance> GK3DSceneLoader::getSurface()
	{
		return surface;
	}

	void GK3DSceneLoader::createFirs()
	{
		std::default_random_engine generator = std::default_random_engine(123);
		std::uniform_real_distribution<GLfloat> distribution = std::uniform_real_distribution<GLfloat>(-3.0f, 3.0f);

		const int firCount = 50;
		glm::vec3 firScale = { 0.0005f, 0.0005f, 0.0005f };
		std::shared_ptr<Material> material;
		std::vector<glm::vec3> positions(firCount);
		std::vector<glm::vec3> angles(firCount);

		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::GreenRubber));
		for (size_t i = 0; i < firCount; i++)
		{
			positions[i] = glm::vec3(distribution(generator), 0.0f, distribution(generator));
			angles[i] = glm::vec3(1, 1, 1);
		}
		createGenericModel(MODEL_FIR, firCount, firScale, positions.data(), angles.data(), defaultMaterial);
	}

	void GK3DSceneLoader::createFlashLight()
	{
		const int currentModel = MODEL_FLASHLIGHT;
		const int count = 1;
		glm::vec3 scale = { 0.005f, 0.005f, 0.005f };
		glm::vec3 positions[count] = { { 0.518999517f, 0.0480000004f, 0.319600284f } };
		glm::vec3 angles[count] = { { -5.0f, 90.0f, 0.0f } };
		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::BlackPlastic));
		createGenericModel(currentModel, count, scale, positions, angles, defaultMaterial);
	}

	void GK3DSceneLoader::createGenericModel(int modelType, size_t count, glm::vec3 scale, glm::vec3 * positions, glm::vec3 * angles, std::shared_ptr<Material> defaultMaterial,
		std::function<void(int, std::shared_ptr<DrawableInstance>)> createdInstanceCallback)
	{
		std::shared_ptr<Material> material;
		if (modelsData[modelType]->material)
			material = modelsData[modelType]->material;
		else material = std::shared_ptr<Material>(defaultMaterial);
		if (modelsData[modelType]->diffuseTex)
			material->diffuseTex = Texture(modelsData[modelType]->diffuseTex);
		if (modelsData[modelType]->alphaTex)
			material->alphaTex = Texture(modelsData[modelType]->alphaTex);
		for (size_t i = 0; i < count; i++)
		{
			std::shared_ptr<DrawableInstance> instance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
				shaders[SHADER_OBJECT],
				material,
				positions[i],
				scale,
				angles[i].x,
				angles[i].y,
				angles[i].z));
			instances[modelType].push_back(instance);
			if (createdInstanceCallback)
				createdInstanceCallback(modelType, instance);
		}
	}

	typedef std::map<int, std::vector<std::shared_ptr<DrawableInstance> > > InstancesDic;

	void GK3DSceneLoader::buildScene()
	{
		cameraPosition = glm::vec3(0.5f, 0.17f, -0.82f);
		cameraFront = glm::vec3(-0.46f, -0.12f, 0.88f);
		for (InstancesDic::const_iterator it = instances.begin(); it != instances.end(); ++it)
		{
			drawables->push_back(std::shared_ptr<Drawable>(
				new Drawable(modelsData[it->first]->vertices, modelsData[it->first]->indices, it->second)));
		}
	}

	void GK3DSceneLoader::buildSurface()
	{
		std::shared_ptr<Image> firstTexImage = std::shared_ptr<Image>(ModelLoader().loadImage("assets/", "grass.png"));
		std::shared_ptr<Image> secondTexImage = std::shared_ptr<Image>(ModelLoader().loadImage("assets/", "terrain.png"));
		std::vector<Vertex> vertices = {
			{ 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 20.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 20.0f, 20.0f },
			{ -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.0f }
		};
		std::vector<GLuint> indices = { 1, 2, 0, 1, 2, 3 };
		Texture firstTerrainTex = Texture(firstTexImage);
		Texture secondTerrainTex = Texture(secondTexImage);
		Material* material = new Material(
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));
		material->diffuseTex = firstTerrainTex;

		const size_t surfacesNum = 1;
		glm::vec3 positions[surfacesNum] =  {
			glm::vec3(0.0f, -0.005f, 0.0f)
		};
		surface = std::shared_ptr<SurfaceInstance>(new SurfaceInstance(
			shaders[SHADER_OBJECT],
			firstTerrainTex,
			secondTerrainTex,
			std::shared_ptr<Material>(material),
			positions[0],
			glm::vec3(3.0f, 3.0f, 3.0f)));
		drawables->push_back(std::shared_ptr<Drawable>(
			new Drawable(vertices, indices, { surface })));
	}
}