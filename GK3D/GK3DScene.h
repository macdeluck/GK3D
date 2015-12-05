#ifndef _GK3D_SCENE
#define _GK3D_SCENE

#include "Scene.h"
#include <random>

namespace GK
{
	class ObjectShader;
	class SpotLightInstance;
	class DrawableInstance;
	class PointLightInstance;
	class SurfaceInstance;
	struct Fog;

	class GK3DScene : public Scene
	{
	private:
		std::shared_ptr<SpotLightInstance> cameraSpotLight;
		std::shared_ptr<PointLightInstance> damagedLamp;
		std::shared_ptr<SpotLightInstance> flashLight;
		std::shared_ptr<SurfaceInstance> surfaceInstance;
		std::shared_ptr<DirLight> dirLights[2];
		std::shared_ptr<Fog> fogs[2];
		int dayTime;
		bool cameraSpotLightOn;

		GLfloat damagedLightTicks;
		int currentDamagedLightMaterial;
		void damagedLampFlickering(GLfloat deltaTime);
		std::default_random_engine damagedLightGenerator;
	public:
		GK3DScene(int screenWidth, int screenHeight);
		virtual ~GK3DScene();
		virtual void update(GLfloat deltaTime) override;
		void toggleCameraLight();
		void toggleSurfaceTexture();
		void toggleSurfaceMarksTexture();
		void moveSurfaceTex(glm::vec2 offset);
		void changeSurfaceScale(float modifier);
		void toggleDayTime();
		std::shared_ptr<Fog> getFog();
	};
}

#endif