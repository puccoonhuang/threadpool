//
// AtomicCounter.cpp
//
//
#include "AtomicCounter.h"


namespace TP {

// GCC 4.1+ atomic builtins.
//
AtomicCounter::AtomicCounter():
	_counter(0)
{
}

	
AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
	_counter(counter.value())
{
}


AtomicCounter::~AtomicCounter()
{
}

AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	__sync_lock_test_and_set(&_counter, counter.value());
	return *this;
}

	
AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	__sync_lock_test_and_set(&_counter, value);
	return *this;
}


} // namespace TP
