#include "Scene.h"
#include "Drawable.h"
#include "Camera.h"

namespace GK
{
	Scene::Scene(int screenWidth, int screenHeight)
		: drawables(new std::vector<std::shared_ptr<Drawable> >(0)),
		camera(new Camera(screenWidth, screenHeight)),
		lightSources(new PointLightsArray()), spotLights(new SpotLightsArray()), dirLights(new DirLightsArray())
	{
	}

	Scene::Scene(std::vector<std::shared_ptr<Drawable> > drawables, Camera camera)
		: drawables(new std::vector<std::shared_ptr<Drawable> >(drawables)),
		camera(new Camera(camera)),
		lightSources(new PointLightsArray()), spotLights(new SpotLightsArray()), dirLights(new DirLightsArray()) {}

	Scene::~Scene() {}

	void Scene::update(GLfloat deltaTime) {}

	void Scene::render()
	{
		preRender();
		for (size_t i = 0; i < drawables->size(); i++)
		{
			(*drawables)[i]->render(shared_from_this());
		}
		postRender();
	}

	std::shared_ptr<std::vector<std::shared_ptr<Drawable> > > Scene::getDrawables()
	{
		return drawables;
	}
	std::shared_ptr<Camera> Scene::getCamera()
	{
		return camera;
	}
	void Scene::setCamera(std::shared_ptr<Camera> camera)
	{
		this->camera = camera;
	}
	std::shared_ptr<PointLightsArray> Scene::getPointLights()
	{
		return lightSources;
	}

	std::shared_ptr<SpotLightsArray> Scene::getSpotLights()
	{
		return spotLights;
	}
	std::shared_ptr<DirLightsArray> Scene::getDirLights()
	{
		return dirLights;
	}
	std::shared_ptr<SkyBox> Scene::getSkyBox()
	{
		return skyBox;
	}
	void Scene::preRender()
	{
	}
	void Scene::postRender()
	{
	}
	void Scene::setSkyBox(std::shared_ptr<SkyBox> skyBox)
	{
		this->skyBox = skyBox;
	}
}