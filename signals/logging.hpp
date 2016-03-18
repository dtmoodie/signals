#pragma once
#include <boost/log/trivial.hpp>
#include "Defs.h"

// https://github.com/Microsoft/CNTK/blob/7c811de9e33d0184fdf340cd79f4f17faacf41cc/Source/Common/Include/ExceptionWithCallStack.h
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // "secure" CRT not available on all platforms  --add this at the top of all CPP files that give "function or variable may be unsafe" warnings
#endif

#ifdef _WIN32
  #define NOMINMAX
  #pragma comment(lib, "Dbghelp.lib")
#else
  #include <execinfo.h>
  #include <cxxabi.h>
#endif

#define LOG(severity) BOOST_LOG_TRIVIAL(severity) << "[" << __FUNCTION__ << "] "
#define DOIF(condition, expression, severity) if(condition) { LOG(severity) << #condition << " is true, thus performing " << #expression; expression;} else { LOG(severity) << #condition << " failed";}
#define DOIF_LOG_PASS(condition, expression, severity) if(condition) { LOG(severity) << #condition << " is true, thus performing " << #expression; expression;} 
#define DOIF_LOG_FAIL(condition, expression, severity) if(condition) { expression; } else { LOG(severity) << "Unable to perform " #expression " due to " #condition << " failed";}

namespace Signals
{
    void SIGNAL_EXPORTS collect_callstack(size_t skipLevels, bool makeFunctionNamesStandOut, const std::function<void(const std::string&)>& write);
    std::string SIGNAL_EXPORTS print_callstack(size_t skipLevels, bool makeFunctionNamesStandOut);
}
