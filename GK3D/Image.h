#ifndef _GK_IMAGE
#define _GK_IMAGE

#include <string>
#include <vector>

namespace GK
{
	class Image
	{
	private:
		std::vector<unsigned char> data;
		int width;
		int height;
	public:

		Image();
		Image(std::vector<unsigned char> data, int width, int height);
		virtual ~Image();
		void loadFile(std::string path);
		int getWidth() const;
		int getHeight() const;
		std::vector<unsigned char> getData();
	};
}

#endif