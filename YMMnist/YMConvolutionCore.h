#pragma once

struct YMConvolutionCore
{
public:
	YMConvolutionCore() = default;
	YMConvolutionCore(const YMConvolutionCore &right) = delete;
	YMConvolutionCore(YMConvolutionCore &&right)
	{
		*this = std::move(right);
	}

	YMConvolutionCore& operator= (const YMConvolutionCore &right) = delete;
	YMConvolutionCore& operator= (YMConvolutionCore &&right)
	{
		this->radiusH = right.radiusH;
		this->radiusV = right.radiusV;
		this->buf = std::move(right.buf);

		return *this;
	}

	byte GetPixel(int x, int y) const { return buf[x + (2*radiusH + 1) * y]; }

public:
	unsigned __int32 radiusH = 2;
	unsigned __int32 radiusV = 2;
	std::vector<int> buf;
};