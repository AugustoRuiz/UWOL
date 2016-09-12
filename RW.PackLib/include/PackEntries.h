#pragma once

#include <map>
#include <string>
#include <iostream>

#include "PackEntry.h"

using namespace std;

class PackEntries
{
public:
	PackEntries(void);
	~PackEntries(void);

	PackEntry* AddEntry(PackEntry *entry);
	PackEntry* AddEntry(wstring entryName, wstring fullPath);
	PackEntry* AddEntry(wstring entryName, void *data, uint64_t size);

	PackEntry* GetEntry(wstring entryName);

	map<wstring, PackEntry*>::iterator GetIterator();
	map<wstring, PackEntry*>::iterator GetIteratorEnd();

	uint64_t GetCount();
private:
	map<wstring, PackEntry*> entriesMap;
};
