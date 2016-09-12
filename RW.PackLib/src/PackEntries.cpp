#include "PackEntries.h"

PackEntries::PackEntries(void)
{
}

PackEntries::~PackEntries(void)
{
}

PackEntry* PackEntries::AddEntry(PackEntry *entry)
{
	this->entriesMap[entry->GetName()] = entry;
	return entry;
}

PackEntry* PackEntries::AddEntry(wstring entryName, void *data, uint64_t size)
{
	PackEntry* result;
	// Does an entry exist with this name?
	if(this->entriesMap.find(entryName) == this->entriesMap.end())
	{
		// It doesn't, add a new entry.
		result = new PackEntry(entryName, data, size);
		this->entriesMap[entryName] = result;
	}
	else
	{
		// It already exists.
		result = this->entriesMap[entryName];
	}
	return result;
}

PackEntry* PackEntries::AddEntry(wstring entryName, wstring fullPath)
{
	PackEntry* result;
	// Does an entry exist with this name?
	if(this->entriesMap.find(entryName) == this->entriesMap.end())
	{
		// It doesn't, add a new entry.
		result = new PackEntry(entryName, fullPath);
		this->entriesMap[entryName] = result;
	}
	else
	{
		// It already exists.
		result = this->entriesMap[entryName];
	}
	return result;
}

PackEntry* PackEntries::GetEntry(wstring entryName)
{
	if(this->entriesMap.find(entryName) == this->entriesMap.end())
	{
		return NULL;
	}
	return this->entriesMap[entryName];
}

map<wstring, PackEntry*>::iterator PackEntries::GetIterator()
{
	return this->entriesMap.begin();
}

map<wstring, PackEntry*>::iterator PackEntries::GetIteratorEnd()
{
	return this->entriesMap.end();
}

uint64_t PackEntries::GetCount()
{
	return (uint64_t) this->entriesMap.size();
}