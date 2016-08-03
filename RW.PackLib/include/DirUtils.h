#pragma once

#include <string>
#include <vector>

#include "StringUtils.h"

using namespace std;

#ifdef WIN32

#include <windows.h>

#else

#include <dirent.h>
#include <sys/stat.h>

#endif

class DirUtils
{
public:
	DirUtils(void);
	~DirUtils(void);

	static wstring GetTempFileName(const wstring& path);
	static wstring NormalizePath(const wstring& path);
	static wstring GetDirectoryName(const wstring& path);
	static bool IsDirectory(const wstring& path);
	static vector<wstring> GetFileList(const wstring& path);
	static void EnsureDirectoryExists(const wstring& path);
	static void CreateDirectoryIfNotExists(const wstring& path);

private:
#ifdef WIN32
	static void getWin32FileList(const wstring& path, vector<wstring>& result);
#else
	static void getGekkoFileList(const wstring& path, vector<wstring>& result);
#endif

};
