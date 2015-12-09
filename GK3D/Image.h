#ifndef _GK_IMAGE
#define _GK_IMAGE

#include <string>
#include <vector>
#include <memory>

namespace GK
{
	enum ImageChannels
	{
		ImageRGB,
		ImageRGBA
	};

	struct Texture;

	class Image
	{
	private:
		std::vector<unsigned char> data;
		size_t width;
		size_t height;
		ImageChannels channels;
	public:

		Image();
		Image(size_t width, size_t height, ImageChannels channels);
		Image(std::vector<unsigned char> data, size_t width, size_t height, ImageChannels channels);
		virtual ~Image();
		void loadFile(std::string path, ImageChannels channels = ImageChannels::ImageRGB);
		void loadTexture(std::shared_ptr<Texture> texture, size_t width, size_t height, ImageChannels channels);
		size_t getWidth() const;
		size_t getHeight() const;
		ImageChannels getImageChannels() const;
		std::vector<unsigned char> getData();
		void setData(std::vector<unsigned char> data);
		size_t pixSize();
		size_t pixIndex(size_t x, size_t y);
	};
}

#endif