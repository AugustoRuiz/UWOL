#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

ostream Log::Out(cout.rdbuf());

void Log::Initialize() {
}

void Log::Dispose() {
}
