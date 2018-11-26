#include "YMMnistImage.h"

#define max(X, Y) (X) > (Y) ? (X) : (Y)

byte YMMnistImage::ToLabel(const YMMnistImageTransParam &param)
{
	byte label = 0;

	// C1 convolutions
	std::vector<YMMnistImage> c1Output;
	c1Output.resize(param.C1.size());
	for (size_t i = 0; i < c1Output.size(); ++i)
	{
		c1Output[i] = Convolution(param.C1[i]);
	}
	
	// S2 pools
	std::vector<YMMnistImage> s2Output = std::move(c1Output);
	for (auto &item : s2Output)
	{
		item = Subsampling(param.S2);
	}

	// C3 convolutions

	// S4 pools

	// C5 layer

	// C6 layer

	// C7 output

	return label;
}

YMMnistImage YMMnistImage::Convolution(const YMConvolutionCore &core) const
{
	YMMnistImage result;

	result.m_width = m_width - core.radiusH * 2;
	result.m_height = m_height - core.radiusV * 2;
	result.m_pixels.resize(result.m_width * result.m_height);
	result.m_label = m_label;

	for (int i = 0; i < result.m_height; ++i)
	{
		for (int j = 0; j < result.m_width; ++j)
		{
			result.SetPixel(i, j, ConvolutionPixel(core, m_pixels, j, i, m_width));
		}
	}

	return result;
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

	res += core.bias; // ¼ÓÈëÆ«ÖÃ

	return (byte)(res & 0x00000000000000FF);
}

YMMnistImage YMMnistImage::Subsampling(int compress) const
{
	YMMnistImage result;

	result.m_width = m_width / compress;
	result.m_height = m_height / compress;
	result.m_pixels.resize(result.m_width * result.m_height);
	result.m_label = m_label;

	for (int i = 0; i < result.m_height; ++i)
	{
		for (int j = 0; j < result.m_width; ++j)
		{
			const unsigned int tempValue = MatrixMax(m_pixels, j*compress, (j + 1)*compress - 1, i*compress, (i + 1)*compress - 1, m_width);
			result.SetPixel(j, i, tempValue);
		}
	}

	return result;
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