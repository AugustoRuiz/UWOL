#pragma once

#include "Common.h"
#include "FileFuncs.h"
#include <RW.PackLib/PackLib.h>

class Pack {
public:
	static Pack* GetInstance() { return &(Pack::_instance); };

	void Initialize(const string& packPath) {
		this->_packer = Packer::ReadPack(FileFuncs::ToWideStr(packPath));
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
		PackEntry* entry = this->_packer->GetEntry(FileFuncs::ToWideStr(fileName));
		SDL_RWops* result = SDL_RWFromConstMem((const void *)(this->_packer->GetUncompressedData(entry)), (int)(entry->GetUncompressedSize()));
		return result;
	};

	void* getData(const string& fileName) {
		PackEntry* entry = this->_packer->GetEntry(FileFuncs::ToWideStr(fileName));
		return this->_packer->GetUncompressedData(entry);
	}

	unsigned int getSize(const string& fileName) {
		return (unsigned int) (this->_packer->GetEntry(FileFuncs::ToWideStr(fileName))->GetUncompressedSize());
	}

	Packer* _packer;
};
