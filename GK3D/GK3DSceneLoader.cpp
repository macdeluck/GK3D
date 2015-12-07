#include "GK3DSceneLoader.h"
#include "Drawable.h"
#include "SpotLightInstance.h"
#include "PointLightInstance.h"
#include "SurfaceInstance.h"
#include "ModelLoader.h"
#include "ObjectShader.h"
#include "LightShader.h"
#include "SurfaceShader.h"
#include "SkyboxShader.h"
#include "EnvShader.h"
#include "Image.h"
#include "Texture3D.h"
#include "Skybox.h"
#include "INoise.h"
#include "RandomNoise.h"
#include "PerlinNoise2D.h"

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
	const int SHADER_SURFACE = 2;
	const int SHADER_SKYBOX = 3;
	const int SHADER_ENV = 4;

	const int MODEL_CUBE = 0;
	const int MODEL_BENCH = 1;
	const int MODEL_LAMP = 2;
	const int MODEL_FIR = 3;
	const int MODEL_FLASHLIGHT = 4;
	const int MODEL_SPHERE = 4;

	void GK3DSceneLoader::load()
	{
		loadShaders();
		loadModelsData();
		loadSkybox();
		createInstances();
		buildSurface();
		buildScene();
	}

	typedef std::map<int, std::shared_ptr<ShaderProgram> > ShadersDic;

	void GK3DSceneLoader::loadShaders()
	{
		shaders[SHADER_OBJECT] = std::shared_ptr<ShaderProgram>(new ObjectShader());
		shaders[SHADER_LIGHT] = std::shared_ptr<ShaderProgram>(new LightShader());
		shaders[SHADER_SURFACE] = std::shared_ptr<ShaderProgram>(new SurfaceShader());
		shaders[SHADER_SKYBOX] = std::shared_ptr<ShaderProgram>(new SkyboxShader());
		shaders[SHADER_ENV] = std::shared_ptr<ShaderProgram>(new EnvShader());
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

		modelLoader.loadModel("assets/sphere.obj", &modelData);
		modelsData[MODEL_SPHERE] = std::shared_ptr<ModelData>(new ModelData(modelData));
	}

	void GK3DSceneLoader::loadSkybox()
	{
		ModelLoader modelLoader;
		faces[Face3D::Face3DFront] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "front.jpg"));
		faces[Face3D::Face3DBack] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "back.jpg"));
		faces[Face3D::Face3DLeft] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "left.jpg"));
		faces[Face3D::Face3DRight] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "right.jpg"));
		faces[Face3D::Face3DTop] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "top.jpg"));
		faces[Face3D::Face3DBottom] = std::shared_ptr<Image>(modelLoader.loadImage("assets/skybox/", "bottom.jpg"));
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
		
		createSkybox();
		createSphere();
	}

	void GK3DSceneLoader::createCubes()
	{
		int currentModel = MODEL_CUBE;
		const int modelsCount = 1;
		glm::vec3 modelScale = { 0.1f, 0.1f, 0.1f };
		glm::vec3 modelPositions[modelsCount] = {
			{ 0.5f, 0.0f, 0.0f }
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
			{ 0.02f, 0.02f, 0.02f },
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
		createGenericModel(currentType, lampCount, lampScale, lampPositions, lampAngles, defaultMaterial, SHADER_OBJECT, std::bind(&GK3DSceneLoader::createLampCallback, this, std::placeholders::_1, std::placeholders::_2));
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

	std::shared_ptr<Drawable> GK3DSceneLoader::getSkyBox()
	{
		return skyBox;
	}

	void GK3DSceneLoader::createFirs()
	{
		std::shared_ptr<INoise> noiseGenerator = std::shared_ptr<INoise>(new RandomNoise(123, -2.5f, 2.5f));
		const int firCount = 50;
		glm::vec3 firScale = { 0.0005f, 0.0005f, 0.0005f };
		std::shared_ptr<Material> material;
		std::vector<glm::vec3> positions(firCount);
		std::vector<glm::vec3> angles(firCount);

		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material(Material::GreenRubber));
		for (size_t i = 0; i < firCount; i++)
		{
			std::vector<float> vals = noiseGenerator->generate(2);
			positions[i] = glm::vec3(vals[0], 0.0f, vals[1]);
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

	std::vector<Vertex> skyboxVertices();
	void GK3DSceneLoader::createSkybox()
	{
		int modelType = MODEL_CUBE;
		const int modelsCount = 1;
		glm::vec3 modelScale = { 50.0f, 50.0f, 50.0f };

		skyBoxInstance = std::shared_ptr<SkyBox>(new SkyBox(
			shaders[SHADER_SKYBOX],
			std::shared_ptr<Texture3D>(new Texture3D(faces)),
			modelScale));
		skyBox = std::shared_ptr<Drawable>(
			new Drawable(skyboxVertices(), { skyBoxInstance }));
	}

	void GK3DSceneLoader::createSphere()
	{
		const int currentModel = MODEL_SPHERE;
		const int count = 1;
		glm::vec3 scale = { 0.1f, 0.1f, 0.1f };
		glm::vec3 positions[count] = { { 0.0f, 1.0f, 0.0f } };
		glm::vec3 angles[count] = { { 0.0f, 0.0f, 0.0f } };
		std::shared_ptr<Material> defaultMaterial = std::shared_ptr<Material>(new Material({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.3f, 0.3f }, 128.0f));
		createGenericModel(currentModel, count, scale, positions, angles, defaultMaterial, SHADER_ENV);
	}

	void GK3DSceneLoader::createGenericModel(int modelType, size_t count, glm::vec3 scale, glm::vec3 * positions, glm::vec3 * angles, std::shared_ptr<Material> defaultMaterial, int shader,
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
				shaders[shader],
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
		cameraPosition = glm::vec3(0.507f, 0.145f, 0.773f);
		cameraFront = glm::vec3(-0.313f, 0.216f, -0.900f);
		for (InstancesDic::const_iterator it = instances.begin(); it != instances.end(); ++it)
		{
			drawables->push_back(std::shared_ptr<Drawable>(
				new Drawable(modelsData[it->first]->vertices, modelsData[it->first]->indices, it->second)));
		}
	}

	float quad(float x, float y)
	{
		return 0.1f*(x*x + y*y);
	}

	void calculateAndSetNormal(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3);
	void GK3DSceneLoader::buildSurface()
	{
		const float surface_scale = 0.1f;
		const int surface_size = 100;
		const int vertex_num = (surface_size + 1) * (surface_size + 1);
		const int indices_num = 6 * surface_size * surface_size;
		std::shared_ptr<Image> firstTexImage = std::shared_ptr<Image>(ModelLoader().loadImage("assets/", "grass.png"));
		std::shared_ptr<Image> secondTexImage = std::shared_ptr<Image>(ModelLoader().loadImage("assets/", "terrain.png"));
		std::shared_ptr<Image> leafsTexImage = std::shared_ptr<Image>(ModelLoader().loadImage("assets/", "leafs.png", true));
		std::vector<Vertex> vertices = std::vector<Vertex>(vertex_num);
		std::vector<std::vector<float> > ys = PerlinNoise2D(123, 0.0f, 0.4f, 5).generate(surface_size+1, surface_size+1);
		int i = 0;
		for (int x = 0; x < surface_size + 1; x++)
		{
			for (int y = 0; y < surface_size + 1; y++)
			{
				float rx = x*surface_scale - (surface_scale*surface_size / 2.0f);
				float ry = y*surface_scale - (surface_scale*surface_size / 2.0f);
				vertices[i] = Vertex({rx, quad(rx, ry) * ys[x][y], ry, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f});
				i++;
			}
		}
		std::vector<GLuint> indices = std::vector<GLuint>(indices_num);
		i = 0;
		for (int x = 0; x < surface_size; x++)
		{
			for (int y = 0; y < surface_size; y++)
			{
				indices[i + 2] = x + y * (surface_size + 1);
				indices[i + 1] = x + 1 + y * (surface_size + 1);
				indices[i + 0] = x + 1 + (y + 1) * (surface_size + 1);
				calculateAndSetNormal(&vertices[indices[i + 2]], &vertices[indices[i + 1]], &vertices[indices[i + 0]]);

				indices[i + 5] = x + y * (surface_size + 1);
				indices[i + 4] = x + (y + 1) * (surface_size + 1);
				indices[i + 3] = x + 1 + (y + 1) * (surface_size + 1);
				calculateAndSetNormal(&vertices[indices[i + 3]], &vertices[indices[i + 4]], &vertices[indices[i + 5]]);
				i += 6;
			}
		}

		Texture firstTerrainTex = Texture(firstTexImage);
		Texture secondTerrainTex = Texture(secondTexImage);
		Texture marksTex = Texture(leafsTexImage);
		Material* material = new Material(
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(0.0f, 0.0f, 0.0f));
		material->diffuseTex = firstTerrainTex;

		const size_t surfacesNum = 1;
		glm::vec3 positions[surfacesNum] =  {
			glm::vec3(0.0f, -0.005f, 0.0f)
		};
		surface = std::shared_ptr<SurfaceInstance>(new SurfaceInstance(
			shaders[SHADER_SURFACE],
			firstTerrainTex,
			secondTerrainTex,
			marksTex,
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			std::shared_ptr<Material>(material),
			positions[0],
			glm::vec3(3.0f, 3.0f, 3.0f)));
		drawables->push_back(std::shared_ptr<Drawable>(
			new Drawable(vertices, indices, { surface })));
	}

	void calculateAndSetNormal(Vertex* vertex1,
		Vertex* vertex2,
		Vertex* vertex3)
	{
		glm::vec3 u = glm::vec3(
			(*vertex2)[0] - (*vertex1)[0],
			(*vertex2)[1] - (*vertex1)[1],
			(*vertex2)[2] - (*vertex1)[2]);
		glm::vec3 v = glm::vec3(
			(*vertex3)[0] - (*vertex1)[0],
			(*vertex3)[1] - (*vertex1)[1],
			(*vertex3)[2] - (*vertex1)[2]);
		glm::vec3 normal = glm::normalize(glm::vec3(
			u.y*v.z - u.z*v.y,
			u.z*v.x - u.x*v.z,
			u.x*v.y - u.y*v.x));
		(*vertex1)[Vertex::VERTEX_POSITIONS + 0] = (*vertex2)[Vertex::VERTEX_POSITIONS + 0] = (*vertex3)[Vertex::VERTEX_POSITIONS + 0] = normal.x;
		(*vertex1)[Vertex::VERTEX_POSITIONS + 1] = (*vertex2)[Vertex::VERTEX_POSITIONS + 1] = (*vertex3)[Vertex::VERTEX_POSITIONS + 1] = normal.y;
		(*vertex1)[Vertex::VERTEX_POSITIONS + 2] = (*vertex2)[Vertex::VERTEX_POSITIONS + 2] = (*vertex3)[Vertex::VERTEX_POSITIONS + 2] = normal.z;
	}

	std::vector<Vertex> skyboxVertices() {
		std::vector<Vertex> result = {
			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),

			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),

			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),

			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),

			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),

			Vertex({ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
			Vertex({ 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f})
		};
		return result;
	}
}