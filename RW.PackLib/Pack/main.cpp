// TestPackLib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	wstring exePath (argv[0]);

	if(argc != 3)
	{
		cout << "Usage: pack <directory> <outputPath>" << endl;
		return -1;
	}

	wstring dirName (argv[1]);
	dirName = DirUtils::GetDirectoryName(dirName);
	Packer* packer = new Packer();	
	packer->AddDirectory(dirName);

	wstring outPath(argv[2]);
	cout << "Writing package... " << string(outPath.begin(), outPath.end())  << endl;
	packer->WritePack(outPath);
	delete packer;

	//packer = Packer::ReadPack(L"C:\\TEST.PAK");
	//PackEntry *entry = packer->GetEntry(L"Debug/PruebaTexto.txt");
	//wchar_t* data = (wchar_t*) packer->GetUncompressedData(entry);

	//wcout.imbue(locale(""));

	//wstring test(data, 1, (entry->GetUncompressedSize() / sizeof(wchar_t)) - 1);
	//wcout << test << endl;

	//delete data;
	//delete packer;

	return 0;
}

