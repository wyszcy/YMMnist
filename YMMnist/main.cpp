//

#include "stdafx.h"

#include <iostream>
#include <random>
#include <forward_list>

#include <windows.h>

#include "YMMnist.h"
#include "ArrayToImage.h"
#include "utility.h"

void TestDataValidate(const YMMnistImage &data, const wchar_t *folder)
{
	static unsigned int index = 0;
	wchar_t filename[_MAX_PATH];
	swprintf_s(filename, _MAX_PATH, LR"(%s\DataValidate\%d_%d.png)", folder, index, data.GetLabel());
	std::vector<byte> imgData;
	imgData.resize(data.GetPixels().size());
	for (size_t i = 0; i < imgData.size(); ++i)
	{
		imgData[i] = data.GetPixels()[i];
	}
	ArrayToImage(filename, imgData.data(), data.GetWidth(), data.GetHeight());

	index += 1;
}

int main()
{
	const std::wstring path = GetCurrentPathW();

	// ÑµÁ·
	YMMnist lib;
	if (!lib.LoadSet((GetCurrentPathA() + R"(\mnist\train-images.idx3-ubyte)").c_str(), (GetCurrentPathA() + R"(\mnist\train-labels.idx1-ubyte)").c_str()))
	{
		std::cout << "fail to load the mnist data" << std::endl;
		return -1;
	}
	for (auto &item : lib.GetData())
	{
		item.Normalization(32, 32);
	}

	auto &ssss = lib.GetData().front();
	YMMnistImageTransParam param;
	param.InitByRandom();
	param.InitC3LinkByDefault();
	ssss.ToLabel(param);

	//std::default_random_engine e;
	//std::forward_list<decltype(e())> data;
	//std::uniform_int_distribution<unsigned int> u(0, lib.GetData().size() - 1);
	//for (size_t i = 0; i < 30; ++i)
	//{
	//	data.emplace_front(u(e));
	//}	

	//for (const auto &item : data)
	//{
	//	TestDataValidate(lib.GetData()[item], path.c_str());
	//	auto ssss = lib.GetData()[item].Subsampling(2);
	//	TestDataValidate(ssss, path.c_str());
	//}

    return 0;
}

