#ifndef _GK_SCENELOADER
#define _GK_SCENELOADER

#include <memory>
#include <vector>
#include <map>
#include <utility>

#include "Common.h"

namespace GK
{
	struct Vertex;
	class Drawable;

	struct DrawableInstance;
	struct PointLightInstance;
	struct SpotLightInstance;
	class ShaderProgram;
	typedef std::vector<std::shared_ptr<PointLightInstance> > PointLightsArray;
	typedef std::vector<std::shared_ptr<SpotLightInstance> > SpotLightsArray;
	typedef std::vector<std::shared_ptr<Drawable> > DrawablesArray;

	class GK3DSceneLoader
	{
	public:
		GK3DSceneLoader();
		~GK3DSceneLoader();

		void load();
		std::shared_ptr<PointLightsArray> getPointLights();
		std::shared_ptr<SpotLightsArray> getSpotLights();
		std::shared_ptr<DrawablesArray> getDrawables();
		glm::vec3 getCameraPosition();
		glm::vec3 getCameraFront();
	private:
		std::shared_ptr<PointLightsArray> pointLights;
		std::shared_ptr<SpotLightsArray> spotLights;
		std::shared_ptr<DrawablesArray> drawables;
		glm::vec3 cameraPosition;
		glm::vec3 cameraFront;

		void loadShaders();
		std::map<int, std::shared_ptr<ShaderProgram> > shaders;

		void loadModelsData();
		std::map<int, std::pair<std::vector<Vertex>, std::vector<GLuint> > > modelsData;

		void createInstances();
		void createBenches();
		void createLamps();
		std::map<int, std::vector<std::shared_ptr<DrawableInstance> > > instances;

		void buildSurface();
		void buildScene();
	};
}

#endif