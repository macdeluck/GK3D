#include "Scene.h"
#include "Drawable.h"
#include "Camera.h"

namespace GK
{
	Scene::Scene(int screenWidth, int screenHeight)
		: drawables(new std::vector<std::shared_ptr<Drawable> >(1)),
		camera(new Camera(screenWidth, screenHeight)) {}

	Scene::Scene(std::vector<std::shared_ptr<Drawable> > drawables, Camera camera)
		: drawables(new std::vector<std::shared_ptr<Drawable> >(drawables)),
		camera(new Camera(camera)) {}

	Scene::~Scene() {}

	void Scene::update(GLfloat deltaTime) {}

	void Scene::render()
	{
		for (size_t i = 0; i < drawables->size(); i++)
		{
			(*drawables)[i]->render(shared_from_this());
		}
	}

	std::weak_ptr<std::vector<std::shared_ptr<Drawable> > > Scene::getDrawables()
	{
		return drawables;
	}
	std::weak_ptr<Camera> Scene::getCamera()
	{
		return camera;
	}
}