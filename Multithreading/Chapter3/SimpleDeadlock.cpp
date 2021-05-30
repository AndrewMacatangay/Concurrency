//This program demonstrates a simple way to deadlock.
//Of course this would be something you'd want to avoid.
//In the code below, thread 1 locks mutex 1, runs some
//code, and then thread 2 locks mutex 2. Thread 1 runs
//the for loop and then attempts to lock mutex 2 which
//is already locked. Thread 2 runs the for loop in func2
//and attempts to lock mutex 1 which was already locked
//by the first thread. Since both mutexes are locked and
//cannot be unlocked, we end up with a deadlock!

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1, m2;

void func1()
{
	lock_guard<mutex> l1(m1);

	for (int x = 0; x < 100; x++)
		cout << "In func1: " << x << endl;

	lock_guard<mutex> l2(m2);
}

void func2()
{
	lock_guard<mutex> l2(m2);

	for (int x = 0; x < 100; x++)
		cout << "In func2: " << x << endl;

	lock_guard<mutex> l1(m1);
}


int main()
{
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();

	return 0;
}
