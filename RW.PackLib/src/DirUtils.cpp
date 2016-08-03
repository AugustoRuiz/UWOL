#include "DirUtils.h"

DirUtils::DirUtils(void)
{
}

DirUtils::~DirUtils(void)
{
}

wstring DirUtils::GetTempFileName(const wstring& path)
{
	wstring result = path + L".tmp";
	return result;
}

wstring DirUtils::NormalizePath(const wstring& path)
{
	wstring result = path;
	size_t found;

	found = result.find(L"\\");
	while(found != wstring::npos)
	{
		result.replace(found, 1, L"/");
		found = result.find(L"\\");
	}

	return result;
}

wstring DirUtils::GetDirectoryName(const wstring& path)
{
	if(DirUtils::IsDirectory(path))
	{
		return wstring(path);
	}
	else
	{
		wstring normalizedPath = DirUtils::NormalizePath(path);
		size_t lastSlashPos = normalizedPath.rfind(L"/");
		normalizedPath = normalizedPath.substr(0, lastSlashPos);
		if(DirUtils::IsDirectory(normalizedPath))
		{
			return normalizedPath;
		}
	}
	return wstring(L"");
}

bool DirUtils::IsDirectory(const wstring& path)
{
	bool result = false;

#ifdef WIN32
	WCHAR *oldCurrentDir = new WCHAR[MAX_PATH];
	// Store old directory
	GetCurrentDirectory(MAX_PATH, oldCurrentDir);

	result = (SetCurrentDirectory(path.c_str()) == FALSE ? false : true);

	// Restore old directory
	SetCurrentDirectory(oldCurrentDir);
	delete oldCurrentDir;
#else
	DIR *pdir;

	string nonUnicodePath(path.begin(), path.end());
	pdir = opendir(nonUnicodePath.c_str());
	result = (pdir != NULL);

	if(result)
	{
		closedir(pdir);
	}
#endif

	return result;
}

vector<wstring> DirUtils::GetFileList(const wstring& path)
{
	vector<wstring> result;

	wstring normalizedPath = DirUtils::NormalizePath(path);
	if(DirUtils::IsDirectory(normalizedPath))
	{
#ifdef WIN32
		DirUtils::getWin32FileList(normalizedPath, result);
#else
		DirUtils::getGekkoFileList(normalizedPath, result);
#endif
	}

	return result;
}

void DirUtils::EnsureDirectoryExists(const wstring& path)
{
	wstring normalizedPath = DirUtils::NormalizePath(path);
	vector<wstring> pathParts = StringUtils::Split(normalizedPath, L'/');

	vector<wstring>::iterator it;
	vector<wstring>::iterator itEnd = pathParts.end();

	wstring tmpPath;
	for(it = pathParts.begin();it!=itEnd;it++)
	{
		wstring current = *it;

		tmpPath.append(current + L"/");

		CreateDirectoryIfNotExists(tmpPath);
	}
}

void DirUtils::CreateDirectoryIfNotExists(const wstring& path)
{
	if(!DirUtils::IsDirectory(path))
	{
#if WIN32
		CreateDirectory(path.c_str(), NULL);
#else
		mkdir(string(path.begin(), path.end()).c_str(), 0755);
#endif
	}
}

#ifdef WIN32
void DirUtils::getWin32FileList(const wstring& path, vector<wstring>& result)
{
	HANDLE hFind;
	WIN32_FIND_DATA info;

	wstring searchPath (path);
	searchPath.append(L"/*.*");

	hFind = FindFirstFile(searchPath.c_str(), &info);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do
		{
			wstring fileName (info.cFileName);
			wstring fullPath = DirUtils::NormalizePath(path + L"/" + fileName);
			if(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(fileName != L"." && fileName != L"..")
				{
					DirUtils::getWin32FileList(fullPath, result);
				}
			}
			else
			{
				result.push_back(fullPath);
			}
		} while (FindNextFile(hFind, &info) != 0);
	}
}
#endif

#ifdef GEKKO
void DirUtils::getGekkoFileList(const wstring& path, vector<wstring>& result)
{
	DIR *directory;
	struct dirent *directoryEntry;

	string nonUnicodePath(path.begin(), path.end());

	directory = opendir(nonUnicodePath.c_str());
	if (directory)
	{
		while ((directoryEntry = readdir(directory)) != NULL)
		{
			string nonUnicodeEntryName (directoryEntry->d_name);
			wstring entryName (nonUnicodeEntryName.begin(), nonUnicodeEntryName.end());
			wstring fullPath = DirUtils::NormalizePath(path + L"/" + entryName);

			if(DirUtils::IsDirectory(fullPath))
			{
				if(nonUnicodeEntryName != "." && nonUnicodeEntryName != "..")
				{
					DirUtils::getGekkoFileList(fullPath, result);
				}
			}
			else
			{
				result.push_back(fullPath);
			}
		}
		closedir(directory);
	}
}
#endif
