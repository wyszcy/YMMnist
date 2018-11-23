#include "utility.h"

std::string GetCurrentPathA()
{
	char path[_MAX_PATH];
	GetModuleFileNameA(NULL, path, _MAX_PATH);
	std::string pathStr = path;
	pathStr = pathStr.substr(0, pathStr.rfind('\\'));

	return pathStr;
}

std::wstring GetCurrentPathW()
{
	wchar_t path[_MAX_PATH];
	GetModuleFileName(NULL, path, _MAX_PATH);
	std::wstring pathStr = path;
	pathStr = pathStr.substr(0, pathStr.rfind(L'\\'));

	return pathStr;
}