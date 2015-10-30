#ifndef _GK_SCENE
#define _GK_SCENE

#include "Common.h"

namespace GK
{
	class Drawable;
	class Camera;

	class Scene : private std::enable_shared_from_this<Scene>
	{
	public:
		Scene(int screenWidth, int screenHeight);
		Scene(std::vector<std::shared_ptr<Drawable> > drawables, Camera camera);
		virtual ~Scene();

		virtual void update(GLfloat deltaTime);
		void render();
		std::weak_ptr<std::vector<std::shared_ptr<Drawable> > > getDrawables();
		std::weak_ptr<Camera> getCamera();
	private:
		std::shared_ptr<std::vector<std::shared_ptr<Drawable> > > drawables;
		std::shared_ptr<Camera> camera;
	};
}

#endif