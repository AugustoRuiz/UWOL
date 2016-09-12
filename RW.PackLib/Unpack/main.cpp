// TestPackLib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	wstring exePath (argv[0]);

	if(argc != 3)
	{
		cout << "Usage: unpack <pakFile> <outputDir>" << endl;
		return -1;
	}

	wstring pakFile (argv[1]);
	wstring dirName (DirUtils::NormalizePath(argv[2]));

	DirUtils::EnsureDirectoryExists(dirName);

	dirName = DirUtils::GetDirectoryName(dirName);

	if(dirName.empty())
	{
		cout << "Couldn't find directory: " << string(dirName.begin(), dirName.end()) << endl;
		return -1;
	}

	cout << "Uncompressing package... " << string(dirName.begin(), dirName.end())  << endl;
	Packer* packer = Packer::ReadPack(pakFile);	
	PackEntries* entries = packer->GetEntries();
	
	map<std::wstring, PackEntry*>::iterator it = entries->GetIterator();
	map<std::wstring, PackEntry*>::iterator itEnd = entries->GetIteratorEnd();

	for(;it != itEnd; it++)
	{
		void* entryData = packer->GetUncompressedData(it->second);
		
		PackEntry *entry = it->second;
		wstring fileName = dirName + wstring(L"/") + entry->GetName();
		
		vector<wstring> entryNameParts = StringUtils::Split(fileName, L'/');
		entryNameParts.pop_back();
		wstring currentDirectory = StringUtils::Join(entryNameParts, L'/');

		cout << "Uncompressing " << string(fileName.begin(), fileName.end()) << "..." << endl;
		
		DirUtils::EnsureDirectoryExists(currentDirectory);

		std::ofstream entryFile(fileName.c_str(), ios::out | ios::binary);

		entryFile.write((char*)entryData, entry->GetUncompressedSize());
		entryFile.flush();
		entryFile.close();

		delete entryData;
	}

	delete packer;

	return 0;
}

