//
// Foundation.h
//
//


#ifndef Foundation_INCLUDED
#define Foundation_INCLUDED

#ifndef TP_THREAD_STACK_SIZE
	#define TP_THREAD_STACK_SIZE 0
#endif


//set function visibility 
#if !defined(Foundation_API)
	#if !defined(TP_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Foundation_API __attribute__ ((visibility ("default")))
	#else
		#define Foundation_API
	#endif
#endif


//
// Pull in basic definitions
//
#include "Bugcheck.h"
#include "Types.h"
#include <string>


#endif // Foundation_INCLUDED
