#include "YMCNN.h"

#include <math.h>

#define max(X, Y) (X) > (Y) ? (X) : (Y)

namespace YMCNN
{
	void Convolution(YMMnistImage &img, const YMConvolutionCore &core)
	{
		img.m_width -= core.radiusH*2;
		img.m_height -= core.radiusV*2;
		decltype(img.m_pixels) tempPixels = std::move(img.m_pixels);
		img.m_pixels.resize(img.m_width - img.m_height);

		for (int i = 0; i < img.m_height; ++i)
		{
			for (int j = 0; j < img.m_width; ++j)
			{
				img.SetPixel(i, j, );
			}
		}
	}

	void YMCNN::Subsampling(YMMnistImage &img, int compress)
	{
		std::vector<byte> tempPixel = std::move(img.m_pixels);
		const unsigned int oldWidth = img.m_width;
		img.m_width /= compress;
		img.m_height /= compress;
		img.m_pixels.resize(img.m_width * img.m_height);

		for (int i = 0; i < img.m_height; ++i)
		{
			for (int j = 0; j < img.m_width; ++j)
			{
				const unsigned int tempValue = MatrixMax(tempPixel, j*compress, (j + 1)*compress - 1, i*compress, (i + 1)*compress - 1, oldWidth);
				img.SetPixel(j, i, tempValue);
			}
		}
	}

	byte MatrixMax(const std::vector<byte> &data, int left, int right, int top, int bottom, int width)
	{
		unsigned __int64 maxVal = 0;
		for (int i = top; i <= bottom; ++i)
		{
			for (int j = left; j <= right; ++j)
			{
				maxVal = max(maxVal, data[i*width + j]);
			}
		}

		return (byte)(maxVal & 0x00000000000000FF);
	}

	byte MatrixAvg(const std::vector<byte> &data, int left, int right, int top, int bottom, int width)
	{
		unsigned __int64 maxVal = 0;
		for (int i = top; i <= bottom; ++i)
		{
			for (int j = left; j <= right; ++j)
			{
				maxVal += data[i*width + j];
			}
		}

		maxVal /= (right - left + 1) * (bottom - top + 1);

		return (byte)(maxVal & 0x00000000000000FF);
	}
}
