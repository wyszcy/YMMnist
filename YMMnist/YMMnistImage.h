#pragma once

#include <vector>

#include "YMMnistDef.h"

using byte = unsigned char;

struct YMMnistImage
{
	friend class YMMnist;
public:
	YMMnistImage() = default;
	YMMnistImage(int width, int height, byte label)
	{
		m_width = width;
		m_height = height;
		m_pixels.resize(width * height, 0);
		m_label = label;
	}
	YMMnistImage(const YMMnistImage &right) = delete;
	YMMnistImage(YMMnistImage &&right)
	{
		*this = std::move(right);
	}

	YMMnistImage& operator=(const YMMnistImage &right) = delete;
	YMMnistImage& operator=(YMMnistImage &&right)
	{
		this->m_width = right.m_width;
		this->m_height = right.m_height;
		this->m_pixels = std::move(right.m_pixels);
		this->m_label = std::move(right.m_label);

		return *this;
	}

	YMMnistImage& operator+= (const YMMnistImage &right)
	{
		if (m_width == right.m_width
			&& m_height == right.m_height
			&& m_pixels.size() == right.m_pixels.size())
		{
			m_label = right.m_label;
			for (size_t i = 0; i < m_pixels.size(); ++i)
			{
				m_pixels[i] += right.m_pixels[i];
			}
		}

		return *this;
	}

	//
	unsigned __int32 GetWidth() const { return m_width; }
	unsigned __int32 GetHeight() const { return m_height; }
	const std::vector<byte>& GetPixels() const { return m_pixels; }
	byte GetLabel() const { return m_label; }

	//
	byte GetPixel(int x, int y) const { return m_pixels[x + m_width * y]; }
	void SetPixel(int x, int y, byte pixel) { m_pixels[x + m_width * y] = pixel; }

	//
	void Normalization(int width, int height);

	//
	byte ToLabel(const YMMnistImageTransParam &param);

	YMMnistImage Convolution(const YMConvolutionCore &core) const;
	YMMnistImage Subsampling(int compress = 2) const;

private:
	byte MatrixMax(const std::vector<byte> &data, int left, int right, int top, int bottom, int width) const;
	byte MatrixAvg(const std::vector<byte> &data, int left, int right, int top, int bottom, int width) const;

	byte ConvolutionPixel(const YMConvolutionCore &core, const std::vector<byte> &data, int left, int top, int width) const; // left-data, top-data

private:
	unsigned __int32 m_width = 0;
	unsigned __int32 m_height = 0;
	std::vector<byte> m_pixels;
	byte m_label;
};