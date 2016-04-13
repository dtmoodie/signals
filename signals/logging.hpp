#pragma once

#include "Defs.h"

#include <boost/log/trivial.hpp>

#include <sstream>
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
#define LOGIF_EQ(lhs, rhs, severity) if(lhs == rhs)  LOG(severity) << "if(" << #lhs << " == " << #rhs << ")" << "[" << lhs << " == " << rhs << "]";
#define LOGIF_NEQ(lhs, rhs, severity) if(lhs != rhs) LOG(severity) << "if(" << #lhs << " != " << #rhs << ")" << "[" << lhs << " != " << rhs << "]";

namespace Signals
{


    void SIGNAL_EXPORTS collect_callstack(size_t skipLevels, bool makeFunctionNamesStandOut, const std::function<void(const std::string&)>& write);
    std::string SIGNAL_EXPORTS print_callstack(size_t skipLevels, bool makeFunctionNamesStandOut);
	std::string SIGNAL_EXPORTS print_callstack(size_t skipLevels, bool makeFunctionNamesStandOut, std::stringstream& ss);

	struct /*interface*/ IExceptionWithCallStackBase
	{
		virtual const char * CallStack() const = 0;
		virtual ~IExceptionWithCallStackBase() throw() {}
	};

	// Exception wrapper to include native call stack string
	template <class E>
	class ExceptionWithCallStack : public E, public IExceptionWithCallStackBase
	{
	public:
		ExceptionWithCallStack(const std::string& msg, const std::string& callstack) :
			E(msg), m_callStack(callstack)
		{ }
		ExceptionWithCallStack(const E& exc, const std::string& callstack) :
			E(exc), m_callStack(callstack)
		{ }

		virtual const char * CallStack() const override { return m_callStack.c_str(); }

		static void      PrintCallStack(size_t skipLevels = 0, bool makeFunctionNamesStandOut = false);
		static std::string GetCallStack(size_t skipLevels = 0, bool makeFunctionNamesStandOut = false); // generate call stack as a string, which should then be passed to the constructor of this  --TODO: Why not generate it directly in the constructor?

	protected:
		std::string m_callStack;
	};
}
