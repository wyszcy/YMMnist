// YMMnist.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <random>
#include <forward_list>

#include "YMMnist.h"
#include "ArrayToImage.h"

void TestDataValidate(const YMMnistImage &data)
{
	static unsigned int index = 0;
	wchar_t filename[_MAX_PATH];
	swprintf_s(filename, _MAX_PATH, LR"(D:\code\cpp\YMMnist\x64\Release\New folder\%d_%d.png)", index, data.m_label);
	ArrayToImage(filename, data.m_pixels.data(), data.m_width, data.m_height);

	index += 1;
}

int main()
{
	YMMnist lib;
	if (!lib.Load(R"(D:\code\cpp\YMMnist\x64\Release\mnist)"))
	{
		std::cout << "fail to load the mnist data" << std::endl;
		return -1;
	}

	std::default_random_engine e;
	std::forward_list<decltype(e())> data;
	std::uniform_int_distribution<unsigned int> u(0, lib.GetData().size() - 1);
	for (size_t i = 0; i < 100; ++i)
	{
		data.emplace_front(u(e));
	}

	for (const auto &item : data)
	{
		TestDataValidate(lib.GetData()[item]);
	}

    return 0;
}

