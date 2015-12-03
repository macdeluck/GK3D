#include "SurfaceInstance.h"

GK::SurfaceInstance::SurfaceInstance(std::shared_ptr<ShaderProgram> shaderProgram, Texture firstTerrainTexture, Texture secondTerrainTexture, Texture marksTexture,
	glm::vec2 texScale,
	glm::vec2 texOffset,
	std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 scale, GLfloat angleX, GLfloat angleY, GLfloat angleZ)
	: DrawableInstance(shaderProgram, material, position, scale, angleX, angleY, angleZ),
	marksTexture(marksTexture), marksTexUsed(false), texScale(texScale), texOffset(texOffset)
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

void GK::SurfaceInstance::toggleMarksTex()
{
	marksTexUsed = !marksTexUsed;
}

void GK::SurfaceInstance::changeTexScale(float modifier)
{
	texScale = glm::clamp(texScale + glm::vec2(modifier, modifier), 0.1f, 50.0f);
}

void GK::SurfaceInstance::moveTex(glm::vec2 offset)
{
	texOffset = glm::clamp(texOffset + offset, -100.0f, 100.0f);
}
