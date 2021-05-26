//Since threads will race to acquire a lock, the thread that has been
//constructed first may have a head start to acquiring the lock. To
//give both threads a fair race to the lock, we stall the execution of
//a thread while waiting for all other threads to be constructed. Once
//all threads are ready, we let them race for the lock!

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

static int go = 0, winner = 1;
static mutex m;

void func(int threadNumber)
{
	while(!go);

	m.lock();

	if (winner)
	{
		cout << "Thread " << threadNumber << " wins!" << endl;
		winner = 0;
	}
	else
		cout << "Thread " << threadNumber << " loses!" << endl;

	m.unlock();
}

int main()
{
	thread t1(func, 1);
	thread t2(func, 2);
	
	cout << "Ready... Set... Go!!!" << endl;
	go = 1;

	t1.join();
	t2.join();
	return 0;
}
