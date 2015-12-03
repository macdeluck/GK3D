#include "SurfaceInstance.h"

GK::SurfaceInstance::SurfaceInstance(std::shared_ptr<ShaderProgram> shaderProgram, Texture firstTerrainTexture, Texture secondTerrainTexture, Texture marksTexture,
	std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 scale, GLfloat angleX, GLfloat angleY, GLfloat angleZ)
	: DrawableInstance(shaderProgram, material, position, scale, angleX, angleY, angleZ),
	marksTexture(marksTexture)
{
	terrainTexes[0] = firstTerrainTexture;
	terrainTexes[1] = secondTerrainTexture;
	currentTerrainTex = 0;
	material->diffuseTex = terrainTexes[currentTerrainTex];
}

void GK::SurfaceInstance::toggleTerrainTex()
{
	currentTerrainTex = 1 - currentTerrainTex;
	material->diffuseTex = terrainTexes[currentTerrainTex];
}
