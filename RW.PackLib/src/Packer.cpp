#include "Packer.h"

Packer::Packer(void)
{
	this->entries = new PackEntries();
	this->currentOffset = 0;
}

Packer::~Packer(void)
{
	delete this->entries;
}

void Packer::AddDirectory(wstring& directory)
{
	wstring normalizedDir = DirUtils::NormalizePath(directory);
	vector<wstring> files = DirUtils::GetFileList(normalizedDir);
	
	for(vector<wstring>::iterator it = files.begin(); it != files.end(); ++it)
	{
		wstring fullPath (*it);
		
		wstring relativePath (*it);
		relativePath.replace(0, normalizedDir.length() + 1, wstring(L""));
		this->AddFile(relativePath, fullPath);
	}
}

void Packer::AddFile(wstring& key, wstring& filePath)
{
	wstring lowerCaseKey(key); 
	transform(key.begin(), key.end(), lowerCaseKey.begin(), ::tolower);

	cout << "Adding file " << string(filePath.begin(), filePath.end()) << endl;

	PackEntry *result = this->entries->AddEntry(lowerCaseKey, filePath);
}

void Packer::WritePack(wstring& fileName)
{
	ofstream outputFile(fileName.c_str(), ofstream::binary);

	map<wstring, PackEntry*>::iterator it;
	map<wstring, PackEntry*>::iterator itEnd = this->entries->GetIteratorEnd();

	uint64_t headerOffset = (uint64_t) sizeof(uint64_t);
	uint64_t offs = 0;

	for(it = this->entries->GetIterator(); it!=itEnd; ++it)
	{
		headerOffset += it->second->GetHeaderSize();

		it->second->SetOffset(offs);
		offs += it->second->GetCompressedSize();
	}
	
	for(it = this->entries->GetIterator(); it!=itEnd; ++it)
	{
		it->second->SetOffset(it->second->GetOffset() + headerOffset);
		it->second->WriteHeader(outputFile);
	}

	BigEndian<uint64_t> endOfHeader;
	endOfHeader = PackEntry::END_OF_HEADER;
	outputFile.write(endOfHeader.GetChars(), sizeof(uint64_t));
	outputFile.flush();

	for(it = this->entries->GetIterator(); it!=itEnd; ++it)
	{
		it->second->WriteData(outputFile);
	}
	outputFile.flush();
	outputFile.close();
}

Packer* Packer::ReadPack(wstring& fileName)
{
	Packer* result = new Packer();

#ifdef WIN32
	ifstream inputFile(fileName.c_str(), ifstream::binary);
#else
	ifstream inputFile(string(fileName.begin(), fileName.end()).c_str(), ifstream::binary);
#endif

	uint64_t bufSize = sizeof(uint64_t);
	char* buffer = new char[(unsigned int)bufSize];

	PackEntry *entry = NULL;
	do
	{
		entry = PackEntry::ReadHeader(inputFile);
		if(entry != NULL)
		{
			result->entries->AddEntry(entry);
		}
	}while(entry != NULL);

	inputFile.close();

	result->fileName = fileName;
	
	return result;
}

PackEntries* Packer::GetEntries()
{
	return this->entries;
}

PackEntry* Packer::GetEntry(wstring& key)
{
	wstring lowerCaseKey(key); 
	transform(key.begin(), key.end(), lowerCaseKey.begin(), ::tolower);

	PackEntry* entry = this->entries->GetEntry(lowerCaseKey);
	if(entry != NULL)
	{
#ifdef WIN32
		ifstream file(this->fileName.c_str(), fstream::binary);
#else
		ifstream file(string(this->fileName.begin(), this->fileName.end()).c_str(), ifstream::binary);
#endif
		entry->ReadData(file);
		file.close();
	}
	return entry;
}

void* Packer::GetUncompressedData(PackEntry *entry)
{
#ifdef WIN32
	ifstream file(this->fileName.c_str(), fstream::binary);
#else
	ifstream file(string(this->fileName.begin(), this->fileName.end()).c_str(), ifstream::binary);
#endif
	stringstream memStream(stringstream::binary | stringstream::in | stringstream::out);

	file.seekg(entry->GetOffset());
	PackUtils::Unpack(file, entry->GetCompressedSize(), memStream);
	file.close();

	char* result = new char[(unsigned int) (entry->GetUncompressedSize())];
	memStream.seekg(0);
	memStream.read(result, entry->GetUncompressedSize());
	return result;
}


