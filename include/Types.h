//
// Types.h
//
// Definitions of fixed-size integer types for various platforms
//


#ifndef Types_INCLUDED
#define Types_INCLUDED


#include "Foundation.h"


namespace TP {
	//
	// Unix/GCC/Clang
	//
	typedef signed char            Int8;
	typedef unsigned char          UInt8;
	typedef signed short           Int16;
	typedef unsigned short         UInt16;
	typedef signed int             Int32;
	typedef unsigned int           UInt32;
	
	typedef signed long        IntPtr;
	typedef unsigned long      UIntPtr;
	#define TP_PTR_IS_64_BIT 1
	#define TP_LONG_IS_64_BIT 1
	typedef signed long        Int64;
	typedef unsigned long      UInt64;
} // namespace TP


#endif // Types_INCLUDED
