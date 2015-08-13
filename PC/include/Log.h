#pragma once

#include <fstream>

using namespace std;

class Log
{
public:
    Log();
    ~Log();
	static void Initialize();
	static fstream Out;
	static void Dispose();
};
