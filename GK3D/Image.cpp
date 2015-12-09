#include <SOIL.h>

#include "Image.h"
#include "Common.h"
#include "Texture.h"

namespace GK
{
	Image::Image()
		: data(0), width(0), height(0), channels(channels)
	{
	}

	Image::Image(size_t width, size_t height, ImageChannels channels)
		: data(0), width(width), height(height), channels(channels)
	{
		int pixSize = channels == ImageChannels::ImageRGB ? 3 : 4;
	}

	Image::Image(std::vector<unsigned char> data, size_t width, size_t height, ImageChannels channels)
		: data(data), width(width), height(height), channels(channels)
	{
		int pixSize = channels == ImageChannels::ImageRGB ? 3 : 4;
		if (data.size() != width * height * pixSize)
			throw Exception("Incorrect data length");
	}

	Image::~Image()
	{
	}

	void Image::loadFile(std::string path, ImageChannels channels)
	{
		int width, height;
		unsigned char* image;
		int pixSize;
		if (channels == ImageChannels::ImageRGB)
		{
			pixSize = 3;
			image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		}
		else
		{
			pixSize = 4;
			image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		}
		if (image == NULL)
			throw Exception("Couldn't load image: " + path);
		this->width = width;
		this->height = height;
		this->channels = channels;
		this->data = std::vector<unsigned char>(image, image + width * height * pixSize);
		SOIL_free_image_data(image);
	}

	void Image::loadTexture(std::shared_ptr<Texture> texture, size_t width, size_t height, ImageChannels channels)
	{
		this->width = width;
		this->height = height;
		this->channels = channels;
		GLRUN(glBindTexture(GL_TEXTURE_2D, texture->getId()));
		int pixSize = channels == ImageChannels::ImageRGB ? 3 : 4;
		GLenum format = channels == ImageChannels::ImageRGB ? GL_RGB : GL_RGBA;
		this->data = std::vector<unsigned char>(width * height * pixSize);
		GLRUN(glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, this->data.data()));
	}

	size_t Image::getWidth() const
	{
		return width;
	}

	size_t Image::getHeight() const
	{
		return height;
	}

	ImageChannels Image::getImageChannels() const
	{
		return channels;
	}

	std::vector<unsigned char> Image::getData()
	{
		return data;
	}

	void Image::setData(std::vector<unsigned char> data)
	{
		this->data = data;
		if (data.size() != width * height * pixSize())
			throw Exception("Incorrect data length");
	}

	size_t Image::pixSize()
	{
		return channels == ImageChannels::ImageRGB ? 3 : 4;
	}

	size_t Image::pixIndex(size_t x, size_t y)
	{
		return (x + y*width)*pixSize();
	}


}