//
// Event.cpp
//


#include "Event.h"
#include "Event_POSIX.cpp"


namespace TP {


Event::Event(bool autoReset): EventImpl(autoReset)
{
}


Event::~Event()
{
}


} // namespace TP
