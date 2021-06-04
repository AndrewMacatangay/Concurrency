//This program demonstrates how a coarse grain lock works.
//The lock covers a larger portion of the code which can
//decrease the risk of deadlocks (acquiring less locks),
//but it doesn't allow as much concurrency since one
//thread is holding the lock for a longer period of time.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

long sum = 0;
mutex m;

void func1()
{
	unique_lock<mutex> l(m);
	
	for(int x = 1; x <= 500000; x++)
		sum += x;
}

void func2()
{
	unique_lock<mutex> l(m);
	
	for(int x = 500001; x <= 1000000; x++)
		sum += x;
}

int main()
{
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();
	cout << "Sum: " << sum << endl;
}