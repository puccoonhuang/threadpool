//
// Timestamp.cpp
//


#include "Timestamp.h"
#include "Timespan.h"
#include "Exception.h"
#include <algorithm>
#undef min
#undef max
#include <limits>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>


namespace TP {


const Timestamp::TimeVal Timestamp::TIMEVAL_MIN = std::numeric_limits<Timestamp::TimeVal>::min();
const Timestamp::TimeVal Timestamp::TIMEVAL_MAX = std::numeric_limits<Timestamp::TimeVal>::max();


Timestamp::Timestamp()
{
	update();
}


Timestamp::Timestamp(TimeVal tv)
{
	_ts = tv;
}


Timestamp::Timestamp(const Timestamp& other)
{
	_ts = other._ts;
}


Timestamp::~Timestamp()
{
}


Timestamp& Timestamp::operator = (const Timestamp& other)
{
	_ts = other._ts;
	return *this;
}


Timestamp& Timestamp::operator = (TimeVal tv)
{
	_ts = tv;
	return *this;
}


void Timestamp::swap(Timestamp& timestamp)
{
	std::swap(_ts, timestamp._ts);
}


Timestamp Timestamp::fromEpochTime(std::time_t t)
{
	return Timestamp(TimeVal(t)*resolution());
}


Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}


void Timestamp::update()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw SystemException("cannot get time of day");
	_ts = TimeVal(tv.tv_sec)*resolution() + tv.tv_usec;
}


Timestamp  Timestamp::operator +  (const Timespan& span) const
{
	return *this + span.totalMicroseconds();
}


Timestamp  Timestamp::operator -  (const Timespan& span) const
{
	return *this - span.totalMicroseconds();
}


Timestamp& Timestamp::operator += (const Timespan& span)
{
	return *this += span.totalMicroseconds();
}


Timestamp& Timestamp::operator -= (const Timespan& span)
{
	return *this -= span.totalMicroseconds();
}

} // namespace TP
