//
// AtomicCounter.h
//

#ifndef AtomicCounter_INCLUDED
#define AtomicCounter_INCLUDED

#include "Foundation.h"
#include "Mutex.h"

namespace TP 
{

class Foundation_API AtomicCounter
	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	/// Typical usage of AtomicCounter is for implementing
	/// reference counting and similar things.
	/// GCC 4.1+ (Intel platforms only)
{
public:
	typedef int ValueType; /// The underlying integer type.
	
	AtomicCounter();
		/// Creates a new AtomicCounter and initializes it to zero.
		
	explicit AtomicCounter(ValueType initialValue);
		/// Creates a new AtomicCounter and initializes it with
		/// the given value.
	
	AtomicCounter(const AtomicCounter& counter);
		/// Creates the counter by copying another one.
	
	~AtomicCounter();
		/// Destroys the AtomicCounter.

	AtomicCounter& operator = (const AtomicCounter& counter);
		/// Assigns the value of another AtomicCounter.
		
	AtomicCounter& operator = (ValueType value);
		/// Assigns a value to the counter.

	operator ValueType () const;
		/// Returns the value of the counter.
		
	ValueType value() const;
		/// Returns the value of the counter.
		
	ValueType operator ++ (); // prefix
		/// Increments the counter and returns the result.
		
	ValueType operator ++ (int); // postfix
		/// Increments the counter and returns the previous value.
		
	ValueType operator -- (); // prefix
		/// Decrements the counter and returns the result.
		
	ValueType operator -- (int); // postfix
		/// Decrements the counter and returns the previous value.
		
	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

private:
	typedef int ImplType;
	ImplType _counter;
};


//
// inlines
// GCC 4.1+ atomic builtins.
//
inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	return _counter;
}

	
inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return __sync_add_and_fetch(&_counter, 1);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	return __sync_fetch_and_add(&_counter, 1);
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return __sync_sub_and_fetch(&_counter, 1);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	return __sync_fetch_and_sub(&_counter, 1);
}

	
inline bool AtomicCounter::operator ! () const
{
	return _counter == 0;
}

} // namespace TP

#endif // AtomicCounter_INCLUDED
