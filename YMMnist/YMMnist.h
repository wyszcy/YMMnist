#pragma once

#include <vector>

using byte = unsigned char;

struct YMMnistImage 
{
public:
	YMMnistImage() = default;
	YMMnistImage(const YMMnistImage &right) = delete;
	YMMnistImage(YMMnistImage &&right)
	{
		*this = std::move(right);
	}	

	YMMnistImage& operator=(const YMMnistImage &right) = delete;
	YMMnistImage& operator=(YMMnistImage &&right)
	{
		this->m_width = right.m_width;
		this->m_height = right.m_height;
		this->m_pixels = std::move(right.m_pixels);
		this->m_label = std::move(right.m_label);

		return *this;
	}

public:
	unsigned __int32 m_width = 0;
	unsigned __int32 m_height = 0;
	std::vector<byte> m_pixels;
	byte m_label;
};

///////////////////////////////////////////////////////
class YMMnist
{
public:
	YMMnist();
	~YMMnist();

	bool Load(const char *folder);

	const std::vector<YMMnistImage>& GetData() const { return m_data; }

private:
	bool GetImageFrData(const char *file, std::vector<YMMnistImage> &data) const;
	bool GetLabelFrData(const char *file, std::vector<YMMnistImage> &data) const;

	std::vector<byte> GetByteDataFrFile(const char *file) const;

	std::vector<YMMnistImage> m_data;
};