#include "YMMnistDef.h"

#include <random>

double Sigmoid(double val)
{
	return 1.0 / (exp(val) + 1.0);
}

YMMnistImageTransParam::YMMnistImageTransParam(int c1Ct, int s2Cp, int c3Ct, int s4Cp, int ctC5, int ctF6, int ctOutput)
{
	C1.resize(c1Ct);
	C1Bias.resize(c1Ct);

	S2 = s2Cp;

	C3.resize(c3Ct*c1Ct);
	C3Bias.resize(c3Ct);

	S4 = s4Cp;

	C5.resize(ctC5*c3Ct);
	C5Bias.resize(ctC5);

	F6.resize(ctC5 * ctF6);
	F6Bias.resize(ctF6);

	output.resize(ctF6 * ctOutput);
}

YMMnistImageTransParam& YMMnistImageTransParam::operator=(YMMnistImageTransParam &&right)
{
	C1 = std::move(right.C1);
	C1Bias = std::move(right.C1Bias);

	S2 = right.S2;

	C3 = std::move(right.C3);
	C3Bias = std::move(right.C3Bias);

	S4 = right.S4;

	C5 = std::move(right.C5);
	C5Bias = std::move(right.C5Bias);

	F6 = std::move(right.F6);
	F6Bias = std::move(right.F6Bias);

	output = std::move(right.output);
}

void YMMnistImageTransParam::InitC3LinkByDefault()
{
	if (C1.size() == 6 && C3.size() / C1.size() == 16)
	{
		std::array<bool, 6 * 16> c3Link = {
			//		 0		1	   2	  3		 4		5	   6	  7		 8		9	   10	  11	 12		13	   14	  15	
			/* 0 */	true, false, false, false, true, true, true, false, false, true, true, true, true, false, true, true,
			/* 1 */	true, true, false, false, false, true, true, true, false, false, true, true, true, true, false, true,
			/* 2 */	true, true, true, false, false, false, true, true, true, false, false, true, false, true, true, true,
			/* 3 */	false, true, true, true, false, false, true, true, true, true, false, false, true, false, true, true,
			/* 4 */	false, false, true, true, true, false, false, true, true, true, true, false, true, true, false, true,
			/* 5 */	false, false, false, true, true, true, false, false, true, true, true, true, false, true, true, true
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

void YMMnistImageTransParam::InitByRandom()
{
	const ParamType range = 100.0;

	for (auto &item : C1)
	{
		item.InitByRandom(range);
	}
	for (auto &item : C3)
	{
		item.InitByRandom(range);
	}
	for (auto &item : C5)
	{
		item.InitByRandom(range);
	}

	//////////////////////////////////////////////////////////////////////////
	std::default_random_engine e(std::random_device{}());
	std::uniform_real_distribution<ParamType> u(-range, range);

	for (auto &item : C1Bias) item = u(e);
	for (auto &item : C3Bias) item = u(e);
	for (auto &item : C5Bias) item = u(e);
	for (auto &item : F6) item = u(e);
	for (auto &item : F6Bias) item = u(e);
	for (auto &item : output) item = u(e);
}