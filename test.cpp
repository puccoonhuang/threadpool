//this is the test programm. paulhuang
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include "ThreadPool.h"

using namespace std;
using namespace TP;


#define SHOW_INFO do {\
	cout << "threadpool allocated:" << pool.allocated() <<\
			",used:" << pool.used() << ",capacity:" << pool.capacity() <<\
			",available:" << pool.available() << endl;\
		} while(0)


class CountNumber
{
	public:
		void count()
		{
			for (int i = 0; i < 5; i++)
				{
					sleep(1);
					pthread_t pid = pthread_self();
					printf("I am counter %lu:%d\n", pid, i);
				}
		}

};
	

int main(int argc, char ** argv)
{	
	ThreadPool pool(2, 3, 3);
	pool.setStackSize(1);
	SHOW_INFO;
	
	pool.addCapacity(1);
	cout << "after add capacity 1" << endl;
	SHOW_INFO;
	
	CountNumber cn;
	RunnableAdapter<CountNumber> ra(cn, &CountNumber::count);
	pool.start(ra, "counter1");		
	SHOW_INFO;
	pool.start(ra, "counter2");
	pool.start(ra, "counter3");
	pool.start(ra, "counter4");
	SHOW_INFO;

	try
	{
		pool.start(ra);
	}
	catch (TP::NoThreadAvailableException&)
	{
		cout << "no available" << endl;
	}
	catch (...)
	{
		cout << "no available threa" << endl;
	}		

	pool.joinAll();

	sleep(5);
	pool.collect();
	SHOW_INFO;

	return 0;
}

