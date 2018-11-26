#include "YMMnistImage.h"

#define max(X, Y) (X) > (Y) ? (X) : (Y)

byte YMMnistImage::ToLabel(const YMMnistImageTransParam &param)
{
	byte label = 0;

	using DataSet = std::vector<YMMnistImage>;

	// C1 convolutions
	DataSet c1Output;
	c1Output.resize(param.C1.size());
	for (size_t i = 0; i < c1Output.size(); ++i)
	{
		c1Output[i] = Convolution(param.C1[i]);
	}
	
	// S2 pools
	DataSet s2Output = std::move(c1Output);
	for (auto &item : s2Output)
	{
		item = item.Subsampling(param.S2);
	}

	// C3 convolutions
	const int c3Count = param.C3.size() / param.C1.size();
	DataSet c3Output;
	c3Output.reserve(c3Count);
	for (size_t i = 0; i < s2Output.size(); ++i)
	{
		for (size_t j = 0; j < c3Count; ++j)
		{
			const int index = i * c3Count + j;
			auto &link = param.C3[index];
			if (j == 0)
			{
				c3Output.emplace_back(s2Output[i].Convolution(link));
			}
			else
			{
				c3Output.back() += s2Output[i].Convolution(link);
			}
		}
	}

	// S4 pools
	DataSet s4Output = std::move(c3Output);
	for (auto &item : s4Output)
	{
		item = item.Subsampling(param.S4);
	}

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

void YMMnistImage::Normalization(int width, int height)
{
	std::vector<byte> newPixels;
	newPixels.resize(width*height, 0);
	const int gapH = (width - m_width) / 2;
	const int gapV = (height - m_height) / 2;

	if (gapV >= 0)
	{
		if (gapH >= 0)
		{
			for (int i = gapV; i < gapV + m_height; ++i)
			{
				memcpy(newPixels.data() + i*width + gapH, m_pixels.data() + (i - gapV)*m_width, sizeof(byte)*m_width);
			}
		}
		else
		{
			for (int i = gapV; i < gapV + m_height; ++i)
			{
				memcpy(newPixels.data() + i*width, m_pixels.data() + (i - gapV)*m_width, sizeof(byte)*width);
			}
		}
	}
	else
	{
		if (gapH >= 0)
		{
			for (int i = 0; i < height; ++i)
			{
				memcpy(newPixels.data() + i*width + gapH, m_pixels.data() + (i - gapV)*m_width, sizeof(byte)*m_width);
			}
		}
		else
		{
			for (int i = 0; i < height; ++i)
			{
				memcpy(newPixels.data() + i*width, m_pixels.data() + (i - gapV)*m_width, sizeof(byte)*width);
			}
		}
	}
	
	m_pixels = std::move(newPixels);
	m_width = width;
	m_height = height;
}