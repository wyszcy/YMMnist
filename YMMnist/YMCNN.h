#pragma once

#include "YMMnist.h"

namespace YMCNN
{
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

	public:
		unsigned __int32 radiusH = 2;
		unsigned __int32 radiusV = 2;
		std::vector<int> buf;
	};

	void Convolution(const YMMnistImage &img, const YMConvolutionCore &core);
	void Subsampling(YMMnistImage &img, int compress = 2);

	byte MatrixMax(const std::vector<byte> &data, int left, int right, int top, int bottom, int width);
	byte MatrixAvg(const std::vector<byte> &data, int left, int right, int top, int bottom, int width);
}
