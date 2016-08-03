#pragma once

#include <iostream>
#include <zlib.h>

using namespace std;

class PackUtils
{
public:
	PackUtils(void);
	~PackUtils(void);

	static uint64_t Pack(istream& in, ostream& out);
	static uint64_t Unpack(istream& in, uint64_t size, ostream& out);

private:
	static const unsigned int CHUNK = 16384;
};
