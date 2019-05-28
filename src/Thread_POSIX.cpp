//
// Thread_POSIX.cpp
//



#include "Thread_POSIX.h"
#include "Thread.h"
#include "Exception.h"
#include "ErrorHandler.h"
#include "Timespan.h"
#include "Timestamp.h"
#include <signal.h>
#include <time.h>


//
// Block SIGPIPE in main thread.
//
namespace
{
	class SignalBlocker
	{
	public:
		SignalBlocker()
		{
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGPIPE);
			pthread_sigmask(SIG_BLOCK, &sset, 0);
		}
		~SignalBlocker()
		{
		}
	};

	static SignalBlocker signalBlocker;
}


namespace 
{
void setThreadName(pthread_t thread, const std::string& threadName)
{

	if (pthread_setname_np(thread, threadName.c_str()) == ERANGE && threadName.size() > 15)
	{
		std::string truncName(threadName, 0, 7);
		truncName.append("~");
		truncName.append(threadName, threadName.size() - 7, 7);
		pthread_setname_np(thread, truncName.c_str());
	}
}
}


namespace TP {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;


ThreadImpl::ThreadImpl():
	_pData(new ThreadData)
{
}


ThreadImpl::~ThreadImpl()
{
	if (_pData->started && !_pData->joined)
	{
		pthread_detach(_pData->thread);
	}
}


void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		_pData->policy = SCHED_OTHER;
		if (isRunningImpl())
		{
			struct sched_param par; struct MyStruct
			{

			};
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio, int policy)
{
	if (prio != _pData->osPrio || policy != _pData->policy)
	{
		if (_pData->pRunnableTarget)
		{
			struct sched_param par;
			par.sched_priority = prio;
			if (pthread_setschedparam(_pData->thread, policy, &par))
				throw SystemException("cannot set thread priority");
		}
		_pData->prio   = reverseMapPrio(prio, policy);
		_pData->osPrio = prio;
		_pData->policy = policy;
	}
}


int ThreadImpl::getMinOSPriorityImpl(int policy)
{
	return sched_get_priority_min(policy);
}


int ThreadImpl::getMaxOSPriorityImpl(int policy)
{
	return sched_get_priority_max(policy);
}


void ThreadImpl::setStackSizeImpl(int size)
{
	//use default size 0
	_pData->stackSize = 0;
}


void ThreadImpl::startImpl(SharedPtr<Runnable> pTarget)
{
	if (_pData->pRunnableTarget)
		throw SystemException("thread already running");

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (_pData->stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, _pData->stackSize))
		{
			pthread_attr_destroy(&attributes);
			throw SystemException("cannot set thread stack size");
		}
	}

	_pData->pRunnableTarget = pTarget;
	//create pthread to execute run target
	if (pthread_create(&_pData->thread, &attributes, runnableEntry, this))
	{
		_pData->pRunnableTarget = 0;
		pthread_attr_destroy(&attributes);
		throw SystemException("cannot start thread");
	}
	_pData->started = true;
	pthread_attr_destroy(&attributes);

	if (_pData->policy == SCHED_OTHER)
	{
		if (_pData->prio != PRIO_NORMAL_IMPL)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
		}
	}
	else
	{
		struct sched_param par;
		par.sched_priority = _pData->osPrio;
		if (pthread_setschedparam(_pData->thread, _pData->policy, &par))
			throw SystemException("cannot set thread priority");
	}
}


void ThreadImpl::joinImpl()
{
	if (!_pData->started) return;
	_pData->done.wait();
	void* result;
	if (pthread_join(_pData->thread, &result))
		throw SystemException("cannot join thread");
	_pData->joined = true;
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (_pData->started && _pData->done.tryWait(milliseconds))
	{
		void* result;
		if (pthread_join(_pData->thread, &result))
			throw SystemException("cannot join thread");
		_pData->joined = true;
		return true;
	}
	else if (_pData->started) return false;
	else return true;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	return _currentThreadHolder.get();
}


ThreadImpl::TIDImpl ThreadImpl::currentTidImpl()
{
	return pthread_self();
}


void ThreadImpl::sleepImpl(long milliseconds)
{
	TP::Timespan remainingTime(1000*TP::Timespan::TimeDiff(milliseconds));
	int rc;
	do
	{
		struct timespec ts;
		ts.tv_sec  = (long) remainingTime.totalSeconds();
		ts.tv_nsec = (long) remainingTime.useconds()*1000;
		TP::Timestamp start;
		rc = ::nanosleep(&ts, 0);
		if (rc < 0 && errno == EINTR)
		{
			TP::Timestamp end;
			TP::Timespan waited = start.elapsed();
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	}
	while (remainingTime > 0 && rc < 0 && errno == EINTR);
	if (rc < 0 && remainingTime > 0) throw TP::SystemException("Thread::sleep(): nanosleep() failed");
}


void* ThreadImpl::runnableEntry(void* pThread)
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));

	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sset, 0);

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	setThreadName(pThreadImpl->_pData->thread, reinterpret_cast<Thread*>(pThread)->getName());
	AutoPtr<ThreadData> pData = pThreadImpl->_pData;
	try
	{
		pData->pRunnableTarget->run();
	}
	catch (Exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (...)
	{
		ErrorHandler::handle();
	}

	pData->pRunnableTarget = 0;
	pData->done.set();
	return 0;
}


int ThreadImpl::mapPrio(int prio, int policy)
{
	int pmin = getMinOSPriorityImpl(policy);
	int pmax = getMaxOSPriorityImpl(policy);

	switch (prio)
	{
	case PRIO_LOWEST_IMPL:
		return pmin;
	case PRIO_LOW_IMPL:
		return pmin + (pmax - pmin)/4;
	case PRIO_NORMAL_IMPL:
		return pmin + (pmax - pmin)/2;
	case PRIO_HIGH_IMPL:
		return pmin + 3*(pmax - pmin)/4;
	case PRIO_HIGHEST_IMPL:
		return pmax;
	default:
		TP_bugcheck_msg("invalid thread priority");
	}
	return -1; // just to satisfy compiler - we'll never get here anyway
}


int ThreadImpl::reverseMapPrio(int prio, int policy)
{
	if (policy == SCHED_OTHER)
	{
		int pmin = getMinOSPriorityImpl(policy);
		int pmax = getMaxOSPriorityImpl(policy);
		int normal = pmin + (pmax - pmin)/2;
		if (prio == pmax)
			return PRIO_HIGHEST_IMPL;
		if (prio > normal)
			return PRIO_HIGH_IMPL;
		else if (prio == normal)
			return PRIO_NORMAL_IMPL;
		else if (prio > pmin)
			return PRIO_LOW_IMPL;
		else
			return PRIO_LOWEST_IMPL;
	}
	else return PRIO_HIGHEST_IMPL;
}


} // namespace TP
