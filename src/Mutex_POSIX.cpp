//
// Mutex_POSIX.cpp
//


#include "Mutex_POSIX.h"
#include "Timestamp.h"
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>


namespace TP {


MutexImpl::MutexImpl()
{

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (pthread_mutex_init(&_mutex, &attr))
	{
		pthread_mutexattr_destroy(&attr);
		throw SystemException("cannot create mutex");
	}
	pthread_mutexattr_destroy(&attr);
}


MutexImpl::MutexImpl(bool fast)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, fast ? PTHREAD_MUTEX_NORMAL : PTHREAD_MUTEX_RECURSIVE);
	if (pthread_mutex_init(&_mutex, &attr))
	{
		pthread_mutexattr_destroy(&attr);
		throw SystemException("cannot create mutex");
	}
	pthread_mutexattr_destroy(&attr);
}


MutexImpl::~MutexImpl()
{
	pthread_mutex_destroy(&_mutex);
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	struct timespec abstime;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	int rc = pthread_mutex_timedlock(&_mutex, &abstime);
	if (rc == 0)
		return true;
	else if (rc == ETIMEDOUT)
		return false;
	else
		throw SystemException("cannot lock mutex");
}


FastMutexImpl::FastMutexImpl(): MutexImpl(true)
{
}


FastMutexImpl::~FastMutexImpl()
{
}


} // namespace TP
