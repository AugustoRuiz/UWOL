#pragma once

#include <sys/stat.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "PackUtils.h"
#include "FixedEndian.h"

using namespace std;

#define END_OF_HEADER (((uint64_t)1) << ((CHAR_BIT * sizeof(uint64_t)) - 1))

class PackEntry
{
public:
//	static const uint64_t END_OF_HEADER = (((uint64_t)1) << ((CHAR_BIT * sizeof(uint64_t)) - 1));

	PackEntry(wstring& entryName, wstring& path);
	PackEntry(wstring& entryName, void *data, uint64_t size);
	~PackEntry(void);

	wstring GetName();
	char* GetData();
	void SetOffset(uint64_t offset);
	uint64_t GetOffset();
	uint64_t GetHeaderSize();
	
	static PackEntry* ReadHeader(istream& stream);
	void ReadData(istream& stream);

	void WriteHeader(ostream& stream);
	void WriteData(ostream& stream);

	uint64_t GetCompressedSize();
	uint64_t GetUncompressedSize();

private:
	PackEntry(void);

	void initEntry();
	void initEntry(wstring& entryName, void *data, uint64_t size);

	wstring name;
	wstring fullPath;

	BigEndian<uint64_t> offset;
	BigEndian<uint64_t> compressedSize;
	BigEndian<uint64_t> entrySize;

	bool isDataLoaded;
	char* compressedData;
};
