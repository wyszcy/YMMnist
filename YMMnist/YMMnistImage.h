#pragma once

#include <vector>

#include "YMConvolutionCore.h"

using byte = unsigned char;

struct YMMnistImage
{
	friend class YMMnist;
public:
	YMMnistImage() = default;
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

	//
	unsigned __int32 GetWidth() const { return m_width; }
	unsigned __int32 GetHeight() const { return m_height; }
	const std::vector<byte>& GetPixels() const { return m_pixels; }
	byte GetLabel() const { return m_label; }

	//
	byte GetPixel(int x, int y) const { return m_pixels[x + m_width * y]; }
	void SetPixel(int x, int y, byte pixel) { m_pixels[x + m_width * y] = pixel; }

	//
	void Convolution(const YMConvolutionCore &core);
	void Subsampling(int compress = 2);

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