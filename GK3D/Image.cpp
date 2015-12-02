#include <SOIL.h>
#include <cassert>

#include "Image.h"
#include "Common.h"

namespace GK
{
	Image::Image()
		: data(0), width(0), height(0)
	{
	}
	Image::Image(std::vector<unsigned char> data, int width, int height)
		: data(data.begin(), data.end()), width(width), height(height)
	{
		assert(data.size() == width * height * 3);
	}

	Image::~Image()
	{
	}

	void Image::loadFile(std::string path)
	{
		int width, height;
		unsigned char* image;
		image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL)
			throw Exception("Couldn't load image: " + path);
		this->width = width;
		this->height = height;
		this->data = std::vector<unsigned char>(image, image + width * height * 3);
		SOIL_free_image_data(image);
	}
}