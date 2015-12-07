#ifndef _GK_SCENE
#define _GK_SCENE

#include "Common.h"

namespace GK
{
	class Drawable;
	class Camera;
	class PointLightInstance;
	class SpotLightInstance;
	class SkyBox;
	struct DirLight;

	typedef std::vector<std::shared_ptr<PointLightInstance> > PointLightsArray;
	typedef std::vector<std::shared_ptr<SpotLightInstance> > SpotLightsArray;
	typedef std::vector<std::shared_ptr<DirLight> > DirLightsArray;

	class Scene : private std::enable_shared_from_this<Scene>
	{
	public:
		Scene(int screenWidth, int screenHeight);
		Scene(std::vector<std::shared_ptr<Drawable> > drawables, Camera camera);
		virtual ~Scene();

		virtual void update(GLfloat deltaTime);
		void render();
		std::shared_ptr<std::vector<std::shared_ptr<Drawable> > > getDrawables();
		std::shared_ptr<Camera> getCamera();
		std::shared_ptr<PointLightsArray> getPointLights();
		std::shared_ptr<SpotLightsArray> getSpotLights();
		std::shared_ptr<DirLightsArray> getDirLights();
		std::shared_ptr<SkyBox> getSkyBox();
	protected:
		void setSkyBox(std::shared_ptr<SkyBox> skyBox);
	private:
		std::shared_ptr<SkyBox> skyBox;
		std::shared_ptr<PointLightsArray> lightSources;
		std::shared_ptr<SpotLightsArray> spotLights;
		std::shared_ptr<DirLightsArray> dirLights;
		std::shared_ptr<std::vector<std::shared_ptr<Drawable> > > drawables;
		std::shared_ptr<Camera> camera;
	};
}

#endif