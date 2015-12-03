#ifndef _GK_SURFACEINSTANCE
#define _GK_SURFACEINSTANCE

#include "Drawable.h"

namespace GK
{
	struct SurfaceInstance : public DrawableInstance
	{
	private:
		Texture terrainTexes[2];
		int currentTerrainTex;
	public:
		SurfaceInstance(std::shared_ptr<ShaderProgram> shaderProgram,
			Texture firstTerrainTexture,
			Texture secondTerrainTexture,
			std::shared_ptr<Material> material,
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);

		void toggleTerrainTex();
	};
}

#endif