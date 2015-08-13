#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

fstream Log::Out;

void Log::Initialize() {
	Log::Out.open("UWOL.log", ios::out);
}

void Log::Dispose() {
	Log::Out.close();
}
