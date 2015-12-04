#include "Texture3D.h"
#include "Image.h"
#include "Common.h"

namespace GK
{
	Texture3D::Texture3D()
		: texture(nullptr)
	{
	}

	Texture3D::Texture3D(std::map<Face3D, std::shared_ptr<Image> > faces)
		: texture(new GLuint)
	{
		GLRUN(glGenTextures(1, &(*texture)));
		GLRUN(glBindTexture(GL_TEXTURE_CUBE_MAP, (*texture)));
		for (auto it = faces.begin(); it != faces.end(); ++it)
		{
			GLenum face = it->first;
			auto image = it->second;
			if (image->getImageChannels() == ImageChannels::ImageRGB)
			{
				GLRUN(glTexImage2D(face, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getData().data()));
			}
			else GLRUN(glTexImage2D(face, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData().data()));
		}
		GLRUN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLRUN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLRUN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLRUN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLRUN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLRUN(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
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