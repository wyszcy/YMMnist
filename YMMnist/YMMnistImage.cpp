#include "YMMnistImage.h"

#define max(X, Y) (X) > (Y) ? (X) : (Y)

void YMMnistImage::Convolution(const YMConvolutionCore &core)
{
	const auto originWidth = m_width;
	m_width -= core.radiusH * 2;
	m_height -= core.radiusV * 2;
	decltype(m_pixels) tempPixels = std::move(m_pixels);
	m_pixels.resize(m_width - m_height);

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			SetPixel(i, j, ConvolutionPixel(core, m_pixels, j, i, originWidth));
		}
	}
}

byte YMMnistImage::ConvolutionPixel(const YMConvolutionCore &core, const std::vector<byte> &data, int left, int top, int width) const
{
	byte res = 0;

	const int bottom = top + core.radiusV * 2 + 1;
	const int right = left + core.radiusH * 2 + 1;
	for (int i = top; i < bottom; ++i)
	{
		for (int j = left; j < right; ++j)
		{
			const int tempIndex = width*i + j;
			res += core.GetPixel(j-left, i-top) * data[tempIndex];
		}
	}

	return res;
}

void YMMnistImage::Subsampling(int compress)
{
	std::vector<byte> tempPixel = std::move(m_pixels);
	const unsigned int oldWidth = m_width;
	m_width /= compress;
	m_height /= compress;
	m_pixels.resize(m_width * m_height);

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			const unsigned int tempValue = MatrixMax(tempPixel, j*compress, (j + 1)*compress - 1, i*compress, (i + 1)*compress - 1, oldWidth);
			SetPixel(j, i, tempValue);
		}
	}
}

byte YMMnistImage::MatrixMax(const std::vector<byte> &data, int left, int right, int top, int bottom, int width) const
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

byte YMMnistImage::MatrixAvg(const std::vector<byte> &data, int left, int right, int top, int bottom, int width) const
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