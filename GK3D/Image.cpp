#include <SOIL.h>
#include <cassert>

#include "Image.h"
#include "Common.h"

namespace GK
{
	Image::Image()
		: data(0), width(0), height(0), channels(channels)
	{
	}
	Image::Image(std::vector<unsigned char> data, int width, int height, ImageChannels channels)
		: data(data.begin(), data.end()), width(width), height(height), channels(channels)
	{
		int pixSize = channels == ImageChannels::ImageRGB ? 3 : 4;
		assert(data.size() == width * height * pixSize);
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

	int Image::getWidth() const
	{
		return width;
	}

	int Image::getHeight() const
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


}