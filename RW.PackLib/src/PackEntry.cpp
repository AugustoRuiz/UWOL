#include "PackEntry.h"

PackEntry::PackEntry()
{
	this->isDataLoaded = false;
}

PackEntry::PackEntry(wstring& entryName, wstring& path)
{
	this->isDataLoaded = false;
	this->name = entryName;
	this->offset = 0;
	this->fullPath = path;
}

PackEntry::PackEntry(wstring& entryName, void *data, uint64_t size)
{
	this->isDataLoaded = false;
	this->name = entryName;
	this->offset = 0;
	this->initEntry(entryName, data, size);
}

PackEntry::~PackEntry(void)
{
	if(this->isDataLoaded)
	{
		delete[] this->compressedData;
	}
}

void PackEntry::initEntry()
{
	if(!fullPath.empty())
	{
		struct stat results;

#ifdef WIN32
		fstream stream (fullPath.c_str(), fstream::in | fstream::binary);
#else
		fstream stream (string(fullPath.begin(), fullPath.end()).c_str(), fstream::in | fstream::binary);
#endif

		stringstream outputStream(stringstream::binary | stringstream::out | stringstream::in);
		this->compressedSize = PackUtils::Pack(stream, outputStream);
		stream.close();

		if (stat(string(fullPath.begin(), fullPath.end()).c_str(), &results) == 0)
			this->entrySize = results.st_size;

		outputStream.seekg(0);
		this->compressedData = new char[(unsigned int)this->compressedSize];
		outputStream.read(this->compressedData, this->compressedSize);
	}
	this->isDataLoaded = true;
}

void PackEntry::initEntry(wstring& entryName, void *data, uint64_t size)
{
	this->entrySize = size;
	stringstream stream((char*)data, stringstream::binary | stringstream::in);
	stringstream outputStream(stringstream::binary | stringstream::out | stringstream::in);
	this->compressedSize = PackUtils::Pack(stream, outputStream);

	outputStream.seekg(0);
	this->compressedData = new char[(unsigned int)this->compressedSize];
	outputStream.read(this->compressedData, this->compressedSize);

	this->isDataLoaded = true;
}

void PackEntry::SetOffset(uint64_t off)
{
	this->offset = off;
}

uint64_t PackEntry::GetOffset()
{
	return this->offset;
}

uint64_t PackEntry::GetCompressedSize()
{
	if(!(this->isDataLoaded))
	{
		this->initEntry();
	}
	return this->compressedSize;
}

uint64_t PackEntry::GetUncompressedSize()
{
	if(!(this->isDataLoaded))
	{
		this->initEntry();
	}
	return this->entrySize;
}

char* PackEntry::GetData()
{
	if(!(this->isDataLoaded))
	{
		this->initEntry();
	}
	return this->compressedData;
}

wstring PackEntry::GetName()
{
	return this->name;
}

void PackEntry::WriteHeader(ostream& stream)
{
	stream.write(this->offset.GetChars(), sizeof(uint64_t));
	stream.write(this->compressedSize.GetChars(), sizeof(uint64_t));
	stream.write(this->entrySize.GetChars(), sizeof(uint64_t));
	
	// Write the name. First the number of chars.
	BigEndian<uint64_t> nameLength(this->name.length());
	stream.write(nameLength.GetChars(), sizeof(uint64_t));

	// Then the characters.
	wstring::iterator it = this->name.begin();
	wstring::iterator itEnd = this->name.end();
	for(;it != itEnd; it++)
	{
		BigEndian<wchar_t> current = *it;
		stream.write(current.GetChars(), sizeof(wchar_t));
	}
}

void PackEntry::WriteData(ostream& stream)
{
	char* theData = this->GetData();
	stream.write(theData, this->GetCompressedSize());
}

PackEntry* PackEntry::ReadHeader(istream& stream)
{
	PackEntry *result = new PackEntry();

	char* bytes = new char[sizeof(uint64_t)];
	stream.read(bytes, sizeof(uint64_t));

	BigEndian<uint64_t> offs;
	offs.SetChars(bytes);
	uint64_t offsValue = offs;

	if(stream.bad() || ((offsValue & END_OF_HEADER) == END_OF_HEADER))
	{
		delete result;
		result = NULL;
	}
	else
	{
		result->offset.SetChars(bytes);

		stream.read(bytes, sizeof(uint64_t));
		result->compressedSize.SetChars(bytes);

		stream.read(bytes, sizeof(uint64_t));
		result->entrySize.SetChars(bytes);

		// Read the name. First the number of characters.
		stream.read(bytes, sizeof(uint64_t));
		BigEndian<uint64_t> nameCharCount;
		nameCharCount.SetChars(bytes);
		
		delete bytes;
		
		uint64_t nameSize = ((uint64_t)nameCharCount);
		wchar_t* nameBytes = new wchar_t[(unsigned int)nameSize];
		stream.read(reinterpret_cast<char*>(nameBytes), nameSize * sizeof(wchar_t));

		result->name.resize((unsigned int)nameSize);

		for(uint64_t i=0; i<nameSize; i++)
		{
			BigEndian<wchar_t> theCharValue;
			theCharValue.SetChars(reinterpret_cast<char*>(&nameBytes[i]));
			result->name[(unsigned int)i] = theCharValue;
		}

		delete nameBytes;
	}
	return result;
}

void PackEntry::ReadData(istream& stream)
{
	if(!this->isDataLoaded)
	{
		this->compressedData = new char[(unsigned int)this->compressedSize];
		stream.seekg((uint64_t)this->offset);
		stream.read(this->compressedData, (uint64_t)this->compressedSize);
		this->isDataLoaded = true;
	}
}

uint64_t PackEntry::GetHeaderSize()
{
	return (uint64_t)(4 * sizeof(uint64_t) + (sizeof(wchar_t) * this->name.length()));
}