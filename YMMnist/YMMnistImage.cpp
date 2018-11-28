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
		c1Output[i] += param.C1Bias[i]; // bias
	}
	for (auto &item : c1Output)
	{
		item.Sigmoid();
	}
	
	// S2 pools
	DataSet s2Output = std::move(c1Output);
	for (auto &item : s2Output)
	{
		item = item.Subsampling(param.S2);
	}

	// C3 convolutions
	const int c3Count = param.C3Bias.size();
	DataSet c3Output;
	c3Output.reserve(c3Count);
	for (size_t i = 0; i < c3Count; ++i)
	{
		for (size_t j = 0; j < s2Output.size(); ++j)
		{
			const int index = i * s2Output.size() + j;
			auto &link = param.C3[index];
			if (j == 0)
			{
				c3Output.emplace_back(s2Output[j].Convolution(link));
			}
			else
			{
				c3Output.back() += s2Output[j].Convolution(link);
			}
		}

		c3Output.back() += param.C3Bias[i];
	}
	for (auto &item : c3Output)
	{
		item.Sigmoid();
	}

	// S4 pools
	DataSet s4Output = std::move(c3Output);
	for (auto &item : s4Output)
	{
		item = item.Subsampling(param.S4);
	}

	// C5 layer
	const int c5Count = param.C5Bias.size();
	std::vector<ParamType> c5Output;
	c5Output.reserve(c5Count);
	for (size_t i = 0; i < c5Count; ++i)
	{
		for (size_t j = 0; j < s4Output.size(); ++j)
		{
			const int index = i*s4Output.size() + j;
			auto &link = param.C5[index];
			if (j == 0)
			{
				const ParamType tempValue = s4Output[j].Convolution(link).ToDouble();
				c5Output.emplace_back(tempValue);
			}
			else
			{
				const ParamType tempValue = s4Output[j].Convolution(link).ToDouble();
				c5Output.back() += tempValue;
			}
		}
	}

	// F6 layer
	std::vector<ParamType> f6Output;
	f6Output.resize(param.F6Bias.size(), 0);
	for (size_t i = 0; i < f6Output.size(); ++i)
	{
		for (size_t j = 0; j < c5Count; ++j)
		{
			const int index = i*c5Count + j;
			f6Output[i] += param.F6[index] * c5Output[j];
		}

		f6Output[i] += param.F6Bias[i];
		f6Output[i] = ::Sigmoid(f6Output[i]);
	}

	// C7 output RBF
	const int c7Output = param.output.size() / param.F6Bias.size();
	std::vector<double> output;
	output.resize(c7Output, 0.0);
	for (size_t i = 0; i < output.size(); ++i)
	{
		for (size_t j = 0; j < f6Output.size(); ++j)
		{
			const int index = i*f6Output.size() + j;
			output[i] += pow(f6Output[j] - param.output[index], 2);
		}
	}

	// 
	label = std::min_element(output.cbegin(), output.cend()) - output.cbegin();

	return label;
}

void YMMnistImage::Sigmoid()
{
	for (auto &item : m_pixels)
	{
		item = ::Sigmoid(item) * 255;
	}
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

__int64 YMMnistImage::ConvolutionPixel(const YMConvolutionCore &core, const std::vector<__int64> &data, int left, int top, int width) const
{
	__int64 res = 0;

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

ParamType YMMnistImage::ToDouble() const
{
	if (m_pixels.empty())
	{
		return 0;
	}

	const int index = m_width * m_height / 2;
	return m_pixels[index];
}

__int64 YMMnistImage::MatrixMax(const std::vector<__int64> &data, int left, int right, int top, int bottom, int width) const
{
	unsigned __int64 maxVal = 0;
	for (int i = top; i <= bottom; ++i)
	{
		for (int j = left; j <= right; ++j)
		{
			maxVal = max(maxVal, data[i*width + j]);
		}
	}

	return maxVal;
}

__int64 YMMnistImage::MatrixAvg(const std::vector<__int64> &data, int left, int right, int top, int bottom, int width) const
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

	return maxVal;
}

void YMMnistImage::Normalization(int width, int height)
{
	std::vector<__int64> newPixels;
	newPixels.resize(width*height, 0);
	const int gapH = (width - m_width) / 2;
	const int gapV = (height - m_height) / 2;

	if (gapV >= 0)
	{
		if (gapH >= 0)
		{
			for (int i = gapV; i < gapV + m_height; ++i)
			{
				memcpy(newPixels.data() + i*width + gapH, m_pixels.data() + (i - gapV)*m_width, sizeof(__int64)*m_width);
			}
		}
		else
		{
			for (int i = gapV; i < gapV + m_height; ++i)
			{
				memcpy(newPixels.data() + i*width, m_pixels.data() + (i - gapV)*m_width, sizeof(__int64)*width);
			}
		}
	}
	else
	{
		if (gapH >= 0)
		{
			for (int i = 0; i < height; ++i)
			{
				memcpy(newPixels.data() + i*width + gapH, m_pixels.data() + (i - gapV)*m_width, sizeof(__int64)*m_width);
			}
		}
		else
		{
			for (int i = 0; i < height; ++i)
			{
				memcpy(newPixels.data() + i*width, m_pixels.data() + (i - gapV)*m_width, sizeof(__int64)*width);
			}
		}
	}
	
	m_pixels = std::move(newPixels);
	m_width = width;
	m_height = height;
}