#pragma once

#include "Common.h"
#include "FileFuncs.h"
#include <RW.PackLib/PackLib.h>

#ifndef TRUE
#define TRUE 1
#endif

class Pack {
public:
	static Pack* GetInstance() { return &(Pack::_instance); };

	void Initialize(const string& packPath) {
		wstring pPath = FileFuncs::ToWideStr(packPath);
		this->_packer = Packer::ReadPack(pPath);
	};

	Mix_Chunk* GetMusicChunk(const string &fileName) {
		if (this->_packer != NULL) {
			SDL_RWops* rwops = this->getRWops(fileName);
			return Mix_LoadWAV_RW(rwops, TRUE);
		}
		else {
			return Mix_LoadWAV(fileName.c_str());
		}
	}

	SDL_Surface* GetImg(const string &fileName) {
		if (this->_packer != NULL) {
			SDL_RWops* rwops = this->getRWops(fileName);
			return IMG_Load_RW(rwops, TRUE);
		}
		else {
			return IMG_Load(fileName.c_str());
		}
	}

	string GetString(const string &fileName) {
		string result;
		if (this->_packer != NULL) {
			char* data = (char*) this->getData(fileName);
			size_t count = (size_t) this->getSize(fileName);
			result = string(data, count);
			delete data;
		}
		else {
			ifstream file(fileName);
			if (file.good()) {
				result = string((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());
				file.close();
			}
		}
		return result;
	}

	istream* GetStream(const string& fileName) {
		istream* result = NULL;
		if (this->_packer != NULL) {
			char* data = (char*) this->getData(fileName);
			size_t count = (size_t) this->getSize(fileName);
			result = new stringstream(string(data, count));
			delete data;
		}
		else {
			ifstream* file = new ifstream(fileName, ios::binary);
			if (file->good()) {
				result = file;
			}
		}
		return result;
	}

private:
	static Pack _instance;
	
	Pack() { this->_packer = NULL; }

	SDL_RWops* getRWops(const string& fileName) {
		wstring fName = FileFuncs::ToWideStr(fileName);
		PackEntry* entry = this->_packer->GetEntry(fName);
		SDL_RWops* result = SDL_RWFromConstMem((const void *)(this->_packer->GetUncompressedData(entry)), (int)(entry->GetUncompressedSize()));
		return result;
	};

	void* getData(const string& fileName) {
		wstring fName = FileFuncs::ToWideStr(fileName);
		PackEntry* entry = this->_packer->GetEntry(fName);
		return this->_packer->GetUncompressedData(entry);
	}

	unsigned int getSize(const string& fileName) {
		wstring fName = FileFuncs::ToWideStr(fileName);
		return (unsigned int) (this->_packer->GetEntry(fName)->GetUncompressedSize());
	}

	Packer* _packer;
};
