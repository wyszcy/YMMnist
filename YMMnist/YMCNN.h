#pragma once

#include "YMMnist.h"

namespace YMCNN
{
	std::vector<YMMnistImage> Convolution(const YMMnistImage &img, int coreWidth = 2, int coreHeight = 2);
	void Subsampling(YMMnistImage &img, int compress = 2);
}
