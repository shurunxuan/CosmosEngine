#include "Compiler.h"

std::string CompilerName()
{
#if defined(__clang__)
	return "Clang/LLVM";
#elif defined(__GNUC__) || defined(__GNUG__)
	return "GNU GCC/G++";
#elif defined(_MSC_VER)
	return "Microsoft Visual Studio";
#endif
}

std::string CompilerVersion()
{
#if defined(_MSC_VER)
	std::string ver = std::to_string(_MSC_FULL_VER);
	return std::string(ver.begin(), ver.begin() + 2) + '.' + std::string(ver.begin() + 2, ver.begin() + 4) + '.' + std::string(ver.begin() + 4, ver.end());
#else
	return __VERSION__;
#endif
}

std::string CompilerString()
{
	return CompilerName() + ' ' + CompilerVersion();
}
