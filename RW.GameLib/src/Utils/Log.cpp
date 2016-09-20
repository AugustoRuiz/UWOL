#include "Utils/Log.h"

ofstream Log::Out;

void Log::Initialize(const string& path) {
	Out.open(path);
}

void Log::Dispose() {
	Out.close();
}
