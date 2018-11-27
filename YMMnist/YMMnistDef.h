#pragma once

#include <random>
#include <array>

using ParamType = double;

extern double Sigmoid(double val);

//////////////////////////////////////////////////////////////////////////
struct YMConvolutionCore
{
public:
	YMConvolutionCore(int radiusH = 2, int radiusV = 2, ParamType bias = 0)
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

	void InitByRandom(ParamType range)
	{
		std::default_random_engine e(std::random_device{}());
		std::uniform_real_distribution<ParamType> u(-range, range); // just set it, no why
		for (auto &item : buf)
		{
			item = u(e);
		}
	}

	ParamType GetPixel(int x, int y) const { return buf[x + (2*radiusH + 1) * y]; }

public:
	unsigned __int32 radiusH = 2;
	unsigned __int32 radiusV = 2;
	std::vector<ParamType> buf;
	bool use = true;
};

//////////////////////////////////////////////////////////////////////////
struct YMMnistImageTransParam
{
public:
	YMMnistImageTransParam(int c1Ct = 6, int s2Cp = 2, int c3Ct = 16, int s4Cp = 2, int ctC5 = 120, int ctF6 = 84, int ctOutput = 10);
	YMMnistImageTransParam(const YMMnistImageTransParam &right) = delete;
	YMMnistImageTransParam(YMMnistImageTransParam &&right) { *this = std::move(right); }

	YMMnistImageTransParam& operator=(const YMMnistImageTransParam &right) = delete;
	YMMnistImageTransParam& operator=(YMMnistImageTransParam &&right);

	void InitByRandom();
	void InitC3LinkByDefault();

	bool IsC3Link(int s2Index, int c3Index) const { return C3[s2Index*C3.size() + c3Index].use; }

public:
	std::vector<YMConvolutionCore> C1;
	std::vector<ParamType> C1Bias;

	int S2;

	std::vector<YMConvolutionCore> C3;
	std::vector<ParamType> C3Bias;

	int S4;

	std::vector<YMConvolutionCore> C5;
	std::vector<ParamType> C5Bias;

	std::vector<ParamType> F6;
	std::vector<ParamType> F6Bias;

	std::vector<ParamType> output;
};