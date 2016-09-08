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
	string arg = string(argv[0]);
	wstring exePath (arg.begin(), arg.end());

	if(argc != 3) {
		cout << "Usage: pack <directory> <outputPath>" << endl;
		return -1;
	}

	arg = string(argv[1]);
	wstring dirName (arg.begin(), arg.end());
	dirName = DirUtils::GetDirectoryName(dirName);
	Packer* packer = new Packer();	
	packer->AddDirectory(dirName);

	arg = string(argv[2]);
	wstring outPath(arg.begin(), arg.end());
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

