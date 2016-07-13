#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class Log
{
public:
	static ofstream Out;
	static void Initialize(const string& path);
	static void Dispose();
};
