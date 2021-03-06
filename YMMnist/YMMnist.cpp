#include "YMMnist.h"

YMMnist::YMMnist()
{

}

YMMnist::~YMMnist()
{

}

bool YMMnist::LoadSet(const char *imageFile, const char *labelFile)
{
	m_data.swap(decltype(m_data)());
	if (!GetLabelFrData(labelFile, m_data))
	{
		return false;
	}
	if (!GetImageFrData(imageFile, m_data))
	{
		return false;
	}

	return true;
}

std::vector<byte> YMMnist::GetByteDataFrFile(const char *file) const
{
	std::vector<byte> data;

	FILE *fp = fopen(file, "rb");
	if (!fp)
	{
		return data;
	}

	fseek(fp, 0, SEEK_END);
	const auto len = _ftelli64(fp) / sizeof(byte);
	fseek(fp, 0, SEEK_SET);

	data.resize(len);
	fread(data.data(), sizeof(byte), data.size(), fp);

	fclose(fp);

	return data;
}

bool YMMnist::GetLabelFrData(const char *file, std::vector<YMMnistImage> &data) const
{
	auto byteData = GetByteDataFrFile(file);
	if (byteData.empty())
	{
		return false;
	}

	std::swap(byteData[4], byteData[7]); // conver the big end to little end
	std::swap(byteData[5], byteData[6]); // conver the big end to little end

	unsigned __int32 len = 0;
	memcpy(&len, byteData.data() + 4, sizeof(byte) * 4);
	data.resize(len);
	for (size_t i = 8; i < byteData.size(); ++i)
	{
		data[i - 8].m_label = byteData[i];
	}

	return true;
}

bool YMMnist::GetImageFrData(const char *file, std::vector<YMMnistImage> &data) const
{
	auto byteData = GetByteDataFrFile(file);
	if (byteData.empty())
	{
		return false;
	}

	std::swap(byteData[4], byteData[7]); // conver the big end to little end
	std::swap(byteData[5], byteData[6]); // conver the big end to little end

	unsigned __int32 len = 0;
	memcpy(&len, byteData.data() + 4, sizeof(byte) * 4);
	if (len != data.size())
	{
		return false;
	}

	std::swap(byteData[8], byteData[11]); // conver the big end to little end
	std::swap(byteData[9], byteData[10]); // conver the big end to little end
	std::swap(byteData[12], byteData[15]); // conver the big end to little end
	std::swap(byteData[13], byteData[14]); // conver the big end to little end

	unsigned __int32 row = 0;
	unsigned __int32 col = 0;
	memcpy(&row, byteData.data() + 8, sizeof(byte) * 4);
	memcpy(&col, byteData.data() + 12, sizeof(byte) * 4);

	for (auto &item : data)
	{
		item.m_width = col;
		item.m_height = row;
	}

	const auto pixelSize = row * col;
	for (size_t i = 0; i < len; ++i)
	{
		data[i].m_pixels.resize(pixelSize);
		for (size_t j = 0; j < pixelSize; ++j)
		{
			data[i].m_pixels[j] = byteData[16 + i*pixelSize + j];
		}
		//memcpy(data[i].m_pixels.data(), byteData.data() + 16 + i*pixelSize, pixelSize);
	}

	return true;
}

