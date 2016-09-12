#pragma once

#include <string>
#include <sstream>
#include <vector>

class StringUtils
{
public:
	StringUtils(void);
	~StringUtils(void);

	static std::vector<std::wstring>& Split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems);
	static std::vector<std::wstring> Split(const std::wstring &s, wchar_t delim);

	static std::wstring Join(std::vector<std::wstring>& parts, wchar_t delim);

};
