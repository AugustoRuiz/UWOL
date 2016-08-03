#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>

#include "DirUtils.h"
#include "PackEntries.h"

using namespace std;

class Packer
{
public:
	Packer(void);
	~Packer(void);

	void AddDirectory(wstring& directory);
	void AddFile(wstring& key, wstring& filePath);

	void WritePack(wstring& fileName);
	static Packer* ReadPack(wstring& fileName);

	PackEntries* GetEntries();
	PackEntry* GetEntry(wstring& key);
    void* GetUncompressedData(PackEntry *entry);

private:
	PackEntries* entries;
	wstring fileName;
	uint64_t currentOffset;
};
