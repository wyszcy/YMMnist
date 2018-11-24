#pragma once

#include <vector>

#include "YMMnistImage.h"

///////////////////////////////////////////////////////
class YMMnist
{
public:
	YMMnist();
	~YMMnist();

	bool LoadSet(const char *imageFile, const char *labelFile);

	std::vector<YMMnistImage>& GetData() { return m_data; }

private:
	bool GetImageFrData(const char *file, std::vector<YMMnistImage> &data) const;
	bool GetLabelFrData(const char *file, std::vector<YMMnistImage> &data) const;

	std::vector<byte> GetByteDataFrFile(const char *file) const;

	std::vector<YMMnistImage> m_data;
};