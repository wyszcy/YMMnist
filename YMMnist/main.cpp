//

#include "stdafx.h"

#include <iostream>
#include <random>
#include <forward_list>

#include <windows.h>

#include "YMMnist.h"
#include "ArrayToImage.h"
#include "utility.h"
#include "YMCNN.h"

void TestDataValidate(const YMMnistImage &data, const wchar_t *folder)
{
	static unsigned int index = 0;
	wchar_t filename[_MAX_PATH];
	swprintf_s(filename, _MAX_PATH, LR"(%s\DataValidate\%d_%d.png)", folder, index, data.m_label);
	ArrayToImage(filename, data.m_pixels.data(), data.m_width, data.m_height);

	index += 1;
}

int main()
{
	const std::wstring path = GetCurrentPathW();

	YMMnist lib;
	if (!lib.LoadSet((GetCurrentPathA() + R"(\mnist\train-images.idx3-ubyte)").c_str(), (GetCurrentPathA() + R"(\mnist\train-labels.idx1-ubyte)").c_str()))
	{
		std::cout << "fail to load the mnist data" << std::endl;
		return -1;
	}

	std::default_random_engine e;
	std::forward_list<decltype(e())> data;
	std::uniform_int_distribution<unsigned int> u(0, lib.GetData().size() - 1);
	for (size_t i = 0; i < 30; ++i)
	{
		data.emplace_front(u(e));
	}	

	for (const auto &item : data)
	{
		TestDataValidate(lib.GetData()[item], path.c_str());
		YMCNN::Subsampling(lib.GetData()[item], 2);
		TestDataValidate(lib.GetData()[item], path.c_str());
	}

    return 0;
}

