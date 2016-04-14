#pragma once

#include "Defs.h"
#include "logging_helper_macros.hpp"

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

#define ASSERT_OP(op, lhs, rhs) \
    if(lhs op rhs) { \
        LOG(debug) << #lhs << " " << #op << " " << #rhs << " [ " << lhs << " " << #op << " " << rhs << "]"; \
        std::stringstream ss; \
        ss << #lhs " " #op " " #rhs " [" << lhs << " " #op " " << rhs << "]"; \
        throw ExceptionWithCallStack<std::exception>(ss.str()); }

#define ASSERT_EQ(lhs, rhs)  ASSERT_OP(==, lhs, rhs)
#define ASSERT_NE(lhs, rhs)  ASSERT_OP(!=, lhs, rhs)
#define ASSERT_LE(lhs, rhs)  ASSERT_OP(<=, lhs, rhs)
#define ASSERT_LT(lhs, rhs)  ASSERT_OP(< , lhs, rhs)
#define ASSERT_GE(lhs, rhs)  ASSERT_OP(>=, lhs, rhs)
#define ASSERT_GT(lhs, rhs)  ASSERT_OP(> , lhs, rhs)

#define CHECK_OP(op, lhs, rhs, severity) \
    if(lhs op rhs) \
        LOG(severity) << #lhs << " " << #op << " " << #rhs << " [ " << lhs << " " << #op << " " << rhs << "]"

#define CHECK_EQ(lhs, rhs, severity) CHECK_OP(==, lhs, rhs, severity)
#define CHECK_NE(lhs, rhs, severity) CHECK_OP(!=, lhs, rhs, severity)
#define CHECK_LE(lhs, rhs, severity) CHECK_OP(<=, lhs, rhs, severity)
#define CHECK_LT(lhs, rhs, severity) CHECK_OP(< , lhs, rhs, severity)
#define CHECK_GE(lhs, rhs, severity) CHECK_OP(>=, lhs, rhs, severity)
#define CHECK_GT(lhs, rhs, severity) CHECK_OP(> , lhs, rhs, severity)

#define LOG_FIRST_N(severity, n) static int LOG_OCCURRENCES = 0; if(LOG_OCCURRENCES <= n) ++LOG_OCCURRENCES; if(LOG_OCCURRENCES <= n) LOG(severity)

namespace Signals
{
    void SIGNAL_EXPORTS collect_callstack(size_t skipLevels, bool makeFunctionNamesStandOut, const std::function<void(const std::string&)>& write);
    std::string SIGNAL_EXPORTS print_callstack(size_t skipLevels, bool makeFunctionNamesStandOut);
	std::string SIGNAL_EXPORTS print_callstack(size_t skipLevels, bool makeFunctionNamesStandOut, std::stringstream& ss);

	struct SIGNAL_EXPORTS IExceptionWithCallStackBase
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
        ExceptionWithCallstack(const E& exc):
            E(exc), m_callstack(print_callstack(0, true))
        {

        }
		virtual const char * CallStack() const override { return m_callStack.c_str(); }

	protected:
		std::string m_callStack;
	};
}
