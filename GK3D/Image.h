#ifndef _GK_IMAGE
#define _GK_IMAGE

#include <string>
#include <vector>

namespace GK
{
	enum ImageChannels
	{
		ImageRGB,
		ImageRGBA
	};

	class Image
	{
	private:
		std::vector<unsigned char> data;
		int width;
		int height;
		ImageChannels channels;
	public:

		Image();
		Image(std::vector<unsigned char> data, int width, int height, ImageChannels channels);
		virtual ~Image();
		void loadFile(std::string path, ImageChannels channels = ImageChannels::ImageRGB);
		int getWidth() const;
		int getHeight() const;
		ImageChannels getImageChannels() const;
		std::vector<unsigned char> getData();
	};
}

#endif