//
// Debugger.cpp
//

#include "Debugger.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <signal.h>


// NOTE: In this module, we use the C library functions (fputs) for,
// output since, at the time we're called, the C++ iostream objects std::cout, etc.
// might not have been initialized yet.


namespace TP {


bool Debugger::isAvailable()
{
	return false;
}


void Debugger::message(const std::string& msg)
{
	//nothing

}


void Debugger::message(const std::string& msg, const char* file, int line)
{

}


void Debugger::enter()
{

}


void Debugger::enter(const std::string& msg)
{

}


void Debugger::enter(const std::string& msg, const char* file, int line)
{

}


void Debugger::enter(const char* file, int line)
{

}


} // namespace TP
