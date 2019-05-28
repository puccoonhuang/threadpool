//
// RefCountedObject.cpp
//



#include "RefCountedObject.h"


namespace TP {


RefCountedObject::RefCountedObject(): _counter(1)
{
}


RefCountedObject::~RefCountedObject()
{
}


} // namespace TP
