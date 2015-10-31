#include "GK3DSceneLoader.h"
#include "Drawable.h"
#include "SpotLightInstance.h"
#include "PointLightInstance.h"
#include "ModelLoader.h"
#include "ObjectShader.h"
#include "LightShader.h"

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

	void GK3DSceneLoader::load()
	{
		loadShaders();
		loadModelsData();
		createInstances();
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
		modelLoader.loadModel("cube.obj", &vertices, &indices);
		modelsData[MODEL_CUBE] = ModelData(vertices, indices);

		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
		modelLoader.loadModel("bench_v01.obj", &vertices, &indices);
		modelsData[MODEL_BENCH] = ModelData(vertices, indices);
	}

	void GK3DSceneLoader::createInstances()
	{
		std::shared_ptr<SpotLightInstance> spotLight = std::shared_ptr<SpotLightInstance>(new SpotLightInstance(
			shaders[SHADER_LIGHT],
			std::shared_ptr<Material>(new Material(Material::WhiteLight))));
		spotLights->push_back(spotLight);

		std::shared_ptr<DrawableInstance> benchInstance = std::shared_ptr<DrawableInstance>(new DrawableInstance(
			shaders[SHADER_OBJECT],
			std::shared_ptr<Material>(new Material(Material::Emerald)),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.001f, 0.001f, 0.001f)));

		instances[MODEL_BENCH].push_back(benchInstance);
	}

	typedef std::map<int, std::vector<std::shared_ptr<DrawableInstance> > > InstancesDic;

	void GK3DSceneLoader::buildScene()
	{
		for (InstancesDic::const_iterator it = instances.begin(); it != instances.end(); ++it)
		{
			drawables->push_back(std::shared_ptr<Drawable>(
				new Drawable(modelsData[it->first].first, modelsData[it->first].second, it->second)));
		}
	}
}