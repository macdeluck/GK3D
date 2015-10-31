#ifndef _GK3D_SCENE
#define _GK3D_SCENE

#include "Scene.h"

namespace GK
{
	class ObjectShader;

	class GK3DScene : public Scene
	{
	public:
		GK3DScene(int screenWidth, int screenHeight);
		virtual ~GK3DScene();
		virtual void update(GLfloat deltaTime) override;
		void toggleCameraLight();
	private:
		std::shared_ptr<SpotLightInstance> cameraSpotLight;
		bool cameraSpotLightOn;
	};
}

#endif