// TestPackLib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef WIN32
	wstring exePath(argv[0]);
#else
	string arg = string(argv[0]);
	wstring exePath(arg.begin(), arg.end());
#endif

	if(argc != 3) {
		cout << "Usage: pack <directory> <outputPath>" << endl;
		return -1;
	}

#ifdef WIN32
	wstring dirName(argv[1]);
#else
	arg = string(argv[1]);
	wstring dirName(arg.begin(), arg.end());
#endif

	dirName = DirUtils::GetDirectoryName(dirName);
	Packer* packer = new Packer();	
	packer->AddDirectory(dirName);

#ifdef WIN32
	wstring outPath(argv[2]);
#else
	arg = string(argv[2]);
	wstring outPath(arg.begin(), arg.end());
#endif
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

