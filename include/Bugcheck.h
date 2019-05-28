//
// Bugcheck.h
//
//
// Definition of the Bugcheck class and the self-testing macros.
//


#ifndef Bugcheck_INCLUDED
#define Bugcheck_INCLUDED


#include "Foundation.h"
#include <string>
#include <cstdlib>


namespace TP {


class Foundation_API Bugcheck
	/// This class provides some static methods that are
	/// used by the
	/// TP_assert_dbg(), TP_assert(), TP_check_ptr(), 
	/// TP_bugcheck() and TP_unexpected() macros. 
	/// You should not invoke these methods
	/// directly. Use the macros instead, as they
	/// automatically provide useful context information.
{
public:
	static void assertion(const char* cond, const char* file, int line, const char* text = 0);
		/// An assertion failed. Break into the debugger, if
		/// possible, then throw an AssertionViolationException.

	static void nullPointer(const char* ptr, const char* file, int line);
		/// An null pointer was encountered. Break into the debugger, if
		/// possible, then throw an NullPointerException.

	static void bugcheck(const char* file, int line);
		/// An internal error was encountered. Break into the debugger, if
		/// possible, then throw an BugcheckException.

	static void bugcheck(const char* msg, const char* file, int line);
		/// An internal error was encountered. Break into the debugger, if
		/// possible, then throw an BugcheckException.

	static void unexpected(const char* file, int line);
		/// An exception was caught in a destructor. Break into debugger,
		/// if possible and report exception. Must only be called from
		/// within a catch () block as it rethrows the exception to
		/// determine its class.

	static void debugger(const char* file, int line);
		/// An internal error was encountered. Break into the debugger, if
		/// possible.

	static void debugger(const char* msg, const char* file, int line);
		/// An internal error was encountered. Break into the debugger, if
		/// possible.

protected:
	static std::string what(const char* msg, const char* file, int line, const char* text = 0);
};


} // namespace TP


//
// useful macros (these automatically supply line number and file name)
//
#if defined(__KLOCWORK__) || defined(__clang_analyzer__)


// Short-circuit these macros when under static analysis.
// Ideally, static analysis tools should understand and reason correctly about
// noreturn methods such as Bugcheck::bugcheck(). In practice, they don't.
// Help them by turning these macros into std::abort() as described here:
// https://developer.klocwork.com/documentation/en/insight/10-1/tuning-cc-analysis#Usingthe__KLOCWORK__macro

#include <cstdlib> // for abort
#define TP_assert_dbg(cond)           do { if (!(cond)) std::abort(); } while (0)
#define TP_assert_msg_dbg(cond, text) do { if (!(cond)) std::abort(); } while (0)
#define TP_assert(cond)               do { if (!(cond)) std::abort(); } while (0)
#define TP_assert_msg(cond, text)     do { if (!(cond)) std::abort(); } while (0)
#define TP_check_ptr(ptr)             do { if (!(ptr)) std::abort(); } while (0)
#define TP_bugcheck()                 do { std::abort(); } while (0)
#define TP_bugcheck_msg(msg)          do { std::abort(); } while (0)


#else // defined(__KLOCWORK__) || defined(__clang_analyzer__)


#if defined(_DEBUG)
	#define TP_assert_dbg(cond) \
		if (!(cond)) TP::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0

	#define TP_assert_msg_dbg(cond, text) \
		if (!(cond)) TP::Bugcheck::assertion(#cond, __FILE__, __LINE__, text); else (void) 0
#else
	#define TP_assert_msg_dbg(cond, text)
	#define TP_assert_dbg(cond)
#endif


#define TP_assert(cond) \
	if (!(cond)) TP::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0


#define TP_assert_msg(cond, text) \
	if (!(cond)) TP::Bugcheck::assertion(#cond, __FILE__, __LINE__, text); else (void) 0


#define TP_check_ptr(ptr) \
	if (!(ptr)) TP::Bugcheck::nullPointer(#ptr, __FILE__, __LINE__); else (void) 0


#define TP_bugcheck() \
	TP::Bugcheck::bugcheck(__FILE__, __LINE__)


#define TP_bugcheck_msg(msg) \
	TP::Bugcheck::bugcheck(msg, __FILE__, __LINE__)


#endif // defined(__KLOCWORK__) || defined(__clang_analyzer__)


#define TP_unexpected() \
	TP::Bugcheck::unexpected(__FILE__, __LINE__);


#define TP_debugger() \
	TP::Bugcheck::debugger(__FILE__, __LINE__)


#define TP_debugger_msg(msg) \
	TP::Bugcheck::debugger(msg, __FILE__, __LINE__)


#if defined(_DEBUG)
#	define TP_stdout_dbg(outstr) \
	std::cout << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define TP_stdout_dbg(outstr)
#endif


#if defined(_DEBUG)
#	define TP_stderr_dbg(outstr) \
		std::cerr << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define TP_stderr_dbg(outstr)
#endif


//
// TP_static_assert
//
// The following was ported from <boost/static_assert.hpp>
//


template <bool x>
struct TP_STATIC_ASSERTION_FAILURE;


template <>
struct TP_STATIC_ASSERTION_FAILURE<true>
{
	enum
	{
		value = 1
	};
};


template <int x> 
struct TP_static_assert_test
{
};


#if defined(__GNUC__) && (__GNUC__ == 3) && ((__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4))
#define TP_static_assert(B) \
	typedef char TP_JOIN(TP_static_assert_typedef_, __LINE__) \
        [TP_STATIC_ASSERTION_FAILURE<(bool) (B)>::value]
#else
#define TP_static_assert(B) \
	typedef TP_static_assert_test<sizeof(TP_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
		TP_JOIN(TP_static_assert_typedef_, __LINE__) TP_UNUSED
#endif


#endif // Bugcheck_INCLUDED
