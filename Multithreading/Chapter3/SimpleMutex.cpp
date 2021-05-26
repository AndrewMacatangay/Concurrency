//This program uses a mutex to stop 2 threads from acquiring
//the same resource at the same time (cout). Although there
//exists a mutex, any thread can still execute before  the other.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m;

void print(int threadNumber, int x)
{
	//m.lock();

	cout << "Thread " << threadNumber << ": " << x << endl;

	//m.unlock();
}

void func1(int threadNumber)
{
        m.lock();

        for (int x = 0; x < 100; x++)
        	print(threadNumber, x);

        m.unlock();
}


void func2(int threadNumber)
{
        //m.lock();

        for (int x = 0; x < 100; x++)
        	print(threadNumber, x);

        //m.unlock();
}

int main()
{
	thread t1(func1, 1);
	thread t2(func2, 2);
	
	t1.join();
	t2.join();
	return 0;
}
