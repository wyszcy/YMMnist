#pragma once

#include <random>
#include <array>

using byte = unsigned char;
using Type = double;

struct YMConvolutionCore
{
public:
	YMConvolutionCore(int radiusH = 2, int radiusV = 2, Type bias = 0)
	{
		const int width = radiusH * 2 + 1;
		const int height = radiusV * 2 + 1;

		this->radiusH = radiusH;
		this->radiusV = radiusV;
		this->use = true;
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
		this->use = right.use;

		return *this;
	}

	void InitByRandom()
	{
		std::default_random_engine e(std::random_device{}());
		std::uniform_real_distribution<Type> u(-100, 100); // just set it, no why
		for (auto &item : buf)
		{
			item = u(e);
		}
	}

	byte GetPixel(int x, int y) const { return buf[x + (2*radiusH + 1) * y]; }

public:
	unsigned __int32 radiusH = 2;
	unsigned __int32 radiusV = 2;
	std::vector<Type> buf;
	bool use = true;
};

struct YMMnistImageTransParam
{
public:
	YMMnistImageTransParam(int c1Ct = 6, int s2Cp = 2, int c3Ct = 16, int s4Cp = 2, int ctC5 = 120)
	{
		C1.resize(c1Ct);
		S2 = s2Cp;
		C3.resize(c3Ct*c1Ct);
		S4 = s4Cp;
		C5.resize(ctC5*c3Ct);
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
		S4 = right.S4;
		C5 = std::move(right.C5);
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
		for (auto &item : C5)
		{
			item.InitByRandom();
		}
	}

	void InitC3LinkByDefault()
	{
		if (C1.size() == 6 && C3.size() / C1.size() == 16)
		{
			std::array<bool, 6*16> c3Link = {
				//		 0		1	   2	  3		 4		5	   6	  7		 8		9	   10	  11	 12		13	   14	  15	
				/* 0 */	true,  false, false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,  true,
				/* 1 */	true,  true,  false, false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,
				/* 2 */	true,  true,  true,  false, false, false, true,  true,  true,  false, false, true,  false, true,  true,  true,
				/* 3 */	false, true,  true,  true,  false, false, true,  true,  true,  true,  false, false, true,  false, true,  true,
				/* 4 */	false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,  true,  false, true,
				/* 5 */	false, false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,  true,  true
			};
			for (size_t i = 0; i < C3.size(); ++i)
			{
				C3[i].use = c3Link[i];
			}
		}
		else
		{
			for (auto &item : C3)
			{
				item = true;
			}
		}
	}

	bool IsC3Link(int s2Index, int c3Index) const
	{
		return C3[s2Index*C3.size() + c3Index].use;
	}

public:
	std::vector<YMConvolutionCore> C1;
	std::vector<Type> C1Bias;
	int S2;
	std::vector<YMConvolutionCore> C3;
	int S4;
	std::vector<YMConvolutionCore> C5;
};