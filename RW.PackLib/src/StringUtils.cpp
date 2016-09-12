#include "StringUtils.h"

StringUtils::StringUtils(void)
{
}

StringUtils::~StringUtils(void)
{
}

std::vector<std::wstring>& StringUtils::Split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems) 
{
    std::wstringstream ss(s);
    std::wstring item;
    
	while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }

    return elems;
}

std::vector<std::wstring> StringUtils::Split(const std::wstring &s, wchar_t delim) 
{
    std::vector<std::wstring> elems;
	return StringUtils::Split(s, delim, elems);
}

std::wstring StringUtils::Join(std::vector<std::wstring>& parts, wchar_t delim)
{
	std::wstringstream ss;
	std::vector<std::wstring>::iterator it = parts.begin();
	std::vector<std::wstring>::iterator itEnd = parts.end();

	for(; it!=itEnd; it++)
	{
		if(it != parts.begin())
		{
			ss << delim;
		}
		ss << *it;
	}
	return ss.str();
}
