#pragma once

#include <random>

using byte = unsigned char;

struct YMConvolutionCore
{
public:
	YMConvolutionCore(int radiusH = 2, int radiusV = 2, double bias = 0.0)
	{
		const int width = radiusH * 2 + 1;
		const int height = radiusV * 2 + 1;

		this->radiusH = radiusH;
		this->radiusV = radiusV;
		this->bias = bias;
		buf.resize(width*height, 0);
	}
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

	void InitByRandom()
	{
		std::default_random_engine e(std::random_device{}());
		std::uniform_real_distribution<double> u(-100.0, 100.0); // just set it, no why
		for (auto &item : buf)
		{
			item = u(e);
		}
	}

	byte GetPixel(int x, int y) const { return buf[x + (2*radiusH + 1) * y]; }

public:
	unsigned __int32 radiusH = 2;
	unsigned __int32 radiusV = 2;
	std::vector<double> buf;
	double bias = 0.0;
};

struct YMMnistImageTransParam
{
public:
	YMMnistImageTransParam(int c1Ct = 6, int s2Cp = 2, int c3Ct = 16, int s4Cp = 2)
	{
		C1.resize(c1Ct);
		S2 = s2Cp;
		C3.resize(c3Ct);
		C3Link.resize(C3.size()*C1.size(), true);
		S4 = s4Cp;
	}

	YMMnistImageTransParam(const YMMnistImageTransParam &right) = delete;
	YMMnistImageTransParam(YMMnistImageTransParam &&right)
	{
		*this = std::move(right);
	}

	YMMnistImageTransParam& operator=(const YMMnistImageTransParam &right) = delete;
	YMMnistImageTransParam& operator=(YMMnistImageTransParam &&right)
	{
		C1 = std::move(right.C1); 
		S2 = right.S2;
		C3 = std::move(right.C3);
		C3Link = std::move(right.C3Link);
		S4 = right.S4;
	}

	void InitByRandom()
	{
		for (auto &item : C1)
		{
			item.InitByRandom();
		}
		for (auto &item : C3)
		{
			item.InitByRandom();
		}
	}

public:
	std::vector<YMConvolutionCore> C1;
	int S2;
	std::vector<YMConvolutionCore> C3;
	std::vector<bool> C3Link;
	int S4;
};