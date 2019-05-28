//
// Runnable.h
//



#ifndef Runnable_INCLUDED
#define Runnable_INCLUDED


#include "Foundation.h"


namespace TP {


class Foundation_API Runnable
	/// The Runnable interface with the run() method
	/// must be implemented by classes that provide
	/// an entry point for a thread.
{
public:	
	Runnable();
	virtual ~Runnable();
	
	virtual void run() = 0;
		/// Do whatever the thread needs to do. Must
		/// be overridden by subclasses.
};


} // namespace TP


#endif // Runnable_INCLUDED
