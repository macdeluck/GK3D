#ifndef _GK_SCENE
#define _GK_SCENE

#include "Common.h"
#include <array>

#define LIGHT_SOURCES_NUM 1
#define SPOT_LIGHTS_NUM 1

namespace GK
{
	class Drawable;
	class Camera;
	struct LightSourceInstance;
	struct SpotLightInstance;

	typedef std::array<std::shared_ptr<LightSourceInstance>, LIGHT_SOURCES_NUM > LightsArray;
	typedef std::array<std::shared_ptr<SpotLightInstance>, SPOT_LIGHTS_NUM > SpotLightsArray;

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
		std::shared_ptr<LightsArray> getLightSources();
		std::shared_ptr<SpotLightsArray> getSpotLights();
	private:
		std::shared_ptr<LightsArray> lightSources;
		std::shared_ptr<SpotLightsArray> spotLights;
		std::shared_ptr<std::vector<std::shared_ptr<Drawable> > > drawables;
		std::shared_ptr<Camera> camera;
	};
}

#endif