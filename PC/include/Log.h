#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class Log
{
public:
    Log();
    ~Log();
	static void Initialize();
	static ostream Out;
	static void Dispose();
};
