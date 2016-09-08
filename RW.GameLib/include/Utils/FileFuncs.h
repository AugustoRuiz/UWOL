#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>

#if defined(_WIN32)
#include <windows.h>
#include <direct.h>
#else
#define DIR_MASK 0755
#endif

using namespace std;

class FileFuncs {
public:

	static void CreateDir(const string &dirName) {
#if defined(_WIN32)
		_mkdir(dirName.c_str());
#else 
		mkdir(dirName.c_str(), DIR_MASK);
#endif
	}

	static string GetDirName(const string &fileName) {
		string result(fileName);
		size_t slashPos = result.rfind("/");
		if (slashPos != string::npos) {
			// remove until slash.
			result = result.substr(0, slashPos);
		}
		slashPos = result.rfind("\\");

		if (slashPos != string::npos) {
			result = result.substr(0, slashPos);
		}
		return result;
	};

	static string GetFileName(const string &fileName) {
		string result(fileName);
		size_t slashPos = result.rfind("/");
		if (slashPos != string::npos) {
			// remove until slash.
			result = result.substr(slashPos + 1, string::npos);
		}
		slashPos = result.rfind("\\");

		if (slashPos != string::npos) {
			result = result.substr(slashPos + 1, string::npos);
		}
		return result;
	};

	static string RemoveExtension(const string &fileName) {
		string result(fileName);
		result = GetFileName(result);

		size_t dotPos = result.rfind(".");
		if (dotPos != 0 && dotPos != string::npos) {
			result = result.substr(0, dotPos);
		}
		return result;
	};

	static string Sanitize(const string &name) {
		string result(name);
		if (!name.empty()) {
			replace_if(result.begin(), result.end(), [](const char c) { return !isalnum(c); }, '_');
		}
		return result;
	};

#if defined(_WIN32)
	static wstring ToWideStr(const string &str) {
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	};

	static string ToThinStr(const wstring &wstr) {
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	};
#else
	static wstring ToWideStr(const string &str) {
		wstring result = wstring(str.begin(), str.end());
		return result;
	};

	static string ToThinStr(const wstring &wstr) {
		string result = string(wstr.begin(), wstr.end());
		return result;
	};

#endif

	static void CopyFiles(const string &srcDir, const string &dstDir, const string &exclude) {
#if defined(_WIN32)
		wstringstream searchPathSS;
		searchPathSS << ToWideStr(srcDir) << "/*.*";
		wstring searchPath = searchPathSS.str();

		WIN32_FIND_DATA fd;
		HANDLE hFnd = ::FindFirstFile(searchPath.c_str(), &fd);
		if (hFnd != INVALID_HANDLE_VALUE) {
			do {
				string fileName = ToThinStr(fd.cFileName);
				if (fileName != "." && fileName != ".." && fileName != "dl") {
					stringstream srcFileSS;
					srcFileSS << srcDir << "/" << fileName;
					stringstream dstFileSS;
					dstFileSS << dstDir << "/" << fileName;

					if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
						FileFuncs::CopyFiles(srcFileSS.str(), dstFileSS.str(), exclude);
					}
					else {
						if (FileFuncs::GetFileName(fileName) != exclude) {
							FileFuncs::CopyFile(srcFileSS.str(), dstFileSS.str());
						}
					}
				}
			} while (::FindNextFile(hFnd, &fd));
			::FindClose(hFnd);
		}

#else
#endif
	};

	static bool CopyFile(const string &src, const string &dst) {

		ifstream source(src, ios::binary);
		ofstream target(dst, ios::binary);

		bool result = source.good() && target.good();

		if (result) {
			target << source.rdbuf();
		}

		return result;
	}

	static void RemoveFiles(const string &dir) {
#if defined(_WIN32)
		wstringstream searchPathSS;
		searchPathSS << ToWideStr(dir) << "/*.*";
		wstring searchPath = searchPathSS.str();

		WIN32_FIND_DATA fd;
		HANDLE hFnd = ::FindFirstFile(searchPath.c_str(), &fd);
		if (hFnd != INVALID_HANDLE_VALUE) {
			do {
				string fileName = ToThinStr(fd.cFileName);
				if (fileName != "." && fileName != ".." && fileName != "dl") {
					stringstream srcFileSS;
					srcFileSS << dir << "/" << fileName;
					string theFileName = srcFileSS.str();
					if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
						FileFuncs::RemoveFiles(theFileName);
					}
					else {
						remove(theFileName.c_str());
					}
				}
			} while (::FindNextFile(hFnd, &fd));
			::FindClose(hFnd);
		}

#else
#endif
	};

};