#include "Texture.h"
#include "Common.h"
#include "Image.h"

namespace GK
{
	Texture::Texture()
		: texture(nullptr)
	{
	}

	Texture::Texture(GLuint textureId)
		: texture(new GLuint(textureId))
	{
	}

	Texture::Texture(std::shared_ptr<Image> image)
		: texture(new GLuint)
	{
		GLRUN(glGenTextures(1, &(*texture)));
		GLRUN(glBindTexture(GL_TEXTURE_2D, (*texture)));
		if (image->getImageChannels() == ImageChannels::ImageRGB)
		{
			GLRUN(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getData().empty() ? NULL : image->getData().data()));
		}
		else
		{
			GLRUN(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData().empty() ? NULL : image->getData().data()));
		}
		GLRUN(glGenerateMipmap(GL_TEXTURE_2D));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLRUN(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture()
	{
		if (texture.unique())
			glDeleteTextures(1, &(*texture));
	}

	bool Texture::empty()
	{
		return !(bool)texture;
	}

	GLuint Texture::getId()
	{
		return *texture;
	}

}