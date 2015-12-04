#include "Texture3D.h"
#include "Image.h"

namespace GK
{
	Texture3D::Texture3D()
		: texture(nullptr)
	{
	}

	Texture3D::Texture3D(std::map<Texture3DFace, std::shared_ptr<Image> > faces)
		: texture(new GLuint)
	{
		glGenTextures(1, &(*texture));
		glBindTexture(GL_TEXTURE_CUBE_MAP, (*texture));
		for (auto it = faces.begin(); it != faces.end(); ++it)
		{
			auto image = it->second;
			if (image->getImageChannels() == ImageChannels::ImageRGB)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + it->first, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getData().data());
			else glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + it->first, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData().data());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture3D::~Texture3D()
	{
		if (texture.unique())
			glDeleteTextures(1, &(*texture));
	}

	bool Texture3D::empty()
	{
		return !(bool)texture;
	}

	GLuint Texture3D::getId()
	{
		return *texture;
	}

}