//
// Exception.cpp
//


#include "Exception.h"
#include <typeinfo>


namespace TP {


Exception::Exception(int code): _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, int code): _msg(msg), _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, const std::string& arg, int code): _msg(msg), _pNested(0), _code(code)
{
	if (!arg.empty())
	{
		_msg.append(": ");
		_msg.append(arg);
	}
}


Exception::Exception(const std::string& msg, const Exception& nested, int code): _msg(msg), _pNested(nested.clone()), _code(code)
{
}


Exception::Exception(const Exception& exc):
	std::exception(exc),
	_msg(exc._msg),
	_code(exc._code)
{
	_pNested = exc._pNested ? exc._pNested->clone() : 0;
}

	
Exception::~Exception() throw()
{
	delete _pNested;
}


Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this)
	{
		Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
		delete _pNested;
		_msg     = exc._msg;
		_pNested = newPNested;
		_code    = exc._code;
	}
	return *this;
}


const char* Exception::name() const throw()
{
	return "Exception";
}


const char* Exception::className() const throw()
{
	return typeid(*this).name();
}

	
const char* Exception::what() const throw()
{
	return name();
}

	
std::string Exception::displayText() const
{
	std::string txt = name();
	if (!_msg.empty())
	{
		txt.append(": ");
		txt.append(_msg);
	}
	return txt;
}


void Exception::extendedMessage(const std::string& arg)
{
	if (!arg.empty())
	{
		if (!_msg.empty()) _msg.append(": ");
		_msg.append(arg);
	}
}


Exception* Exception::clone() const
{
	return new Exception(*this);
}


void Exception::rethrow() const
{
	throw *this;
}


TP_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
TP_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
TP_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
TP_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
TP_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
TP_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
TP_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
TP_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
TP_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
TP_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
TP_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
TP_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

TP_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
TP_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
TP_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
TP_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
TP_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
TP_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
TP_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
TP_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
TP_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
TP_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
TP_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
TP_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
TP_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
TP_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

TP_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
TP_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
TP_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
TP_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
TP_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
TP_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
TP_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
TP_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
TP_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
TP_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
TP_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
TP_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
TP_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
TP_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
TP_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
TP_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
TP_IMPLEMENT_EXCEPTION(DirectoryNotEmptyException, FileException, "Directory not empty")
TP_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")
TP_IMPLEMENT_EXCEPTION(TooManyURIRedirectsException, RuntimeException, "Too many URI redirects")
TP_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")

TP_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
TP_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")


} // namespace TP
