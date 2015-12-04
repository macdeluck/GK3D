#ifndef _GK_SKYBOX
#define _GK_SKYBOX

#include <memory>
#include "Drawable.h"

namespace GK
{
	class SkyboxShader;
	struct Texture3D;

	class SkyBox : public DrawableInstance
	{
	private:
		std::shared_ptr<Texture3D> skyboxTexture;
	public:
		SkyBox(std::shared_ptr<ShaderProgram> shaderProgram,
			std::shared_ptr<Texture3D> skyboxTexture,
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);

		virtual ~SkyBox();
		friend class SkyboxShader;
	};
}

#endif