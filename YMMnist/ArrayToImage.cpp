// ArrayToImage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ArrayToImage.h"

#include <atlimage.h>

void ArrayToImage(const wchar_t *file, const unsigned char *buf, unsigned int width, unsigned int height)
{
	CImage im;
	if (!im.Create(width, height, 24))
	{
		return;
	}

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			unsigned char *pixel = (unsigned char *)im.GetPixelAddress(i, j);
			*pixel = buf[j*width + i];
			*(pixel + 1) = buf[j*width + i];
			*(pixel + 2) = buf[j*width + i];
		}
	}

	im.Save(file);
	im.Destroy();
}