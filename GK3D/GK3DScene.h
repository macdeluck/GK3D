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

	class GK3DScene : public Scene
	{
	public:
		GK3DScene(int screenWidth, int screenHeight);
		virtual ~GK3DScene();
		virtual void update(GLfloat deltaTime) override;
		void toggleCameraLight();
		void toggleSurfaceTexture();
		void toggleSurfaceMarksTexture();
	private:
		std::shared_ptr<SpotLightInstance> cameraSpotLight;
		std::shared_ptr<PointLightInstance> damagedLamp;
		std::shared_ptr<SpotLightInstance> flashLight;
		std::shared_ptr<SurfaceInstance> surfaceInstance;
		bool cameraSpotLightOn;

		GLfloat damagedLightTicks;
		int currentDamagedLightMaterial;
		void damagedLampFlickering(GLfloat deltaTime);
		std::default_random_engine damagedLightGenerator;
	};
}

#endif