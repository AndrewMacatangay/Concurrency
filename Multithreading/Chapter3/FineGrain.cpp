//The lock covers a smaller portion of the code which can
//increase the risk of deadlocks (acquiring more locks),
//has a higher overhead from having to acquire many locks,
//but it allows greater concurrency since one thread is 
//holding the lock for a shorter period of time.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

long sum = 0;
mutex m;

void func1()
{
	for(int x = 1; x <= 500000; x++)
	{
		unique_lock<mutex> l(m);
		sum += x;
	}
}

void func2()
{
	for(int x = 500001; x <= 1000000; x++)
	{	
		unique_lock<mutex> l(m);
		sum += x;
	}
}

int main()
{
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();
	cout << "Sum: " << sum << endl;
}