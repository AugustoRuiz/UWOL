#include "Utils/Log.h"

ofstream Log::Out;

void Log::Initialize(const string& path) {
	Out = ofstream(path);
}

void Log::Dispose() {
	Out.close();
}
