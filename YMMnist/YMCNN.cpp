#include "YMCNN.h"

namespace YMCNN
{
	std::vector<YMMnistImage> Convolution(const YMMnistImage &img, int radiusWidth, int radiusHeight)
	{

	}

	void YMCNN::Subsampling(YMMnistImage &img, int compress)
	{
		std::vector<byte> tempPixel = std::move(img.m_pixels);
		img.m_width /= compress;
		img.m_height /= compress;
		img.m_pixels.resize(img.m_width * img.m_height);

		for (int i = 0; i < img.m_height; ++i)
		{
			for (int j = 0; j < img.m_width; ++j)
			{
				unsigned int tempValue = 0;
				img.SetPixel(j, i, tempValue);
			}
		}
	}
}
