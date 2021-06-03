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
		m.lock();
		sum += x;
		m.unlock();
	}
}

void func2()
{
	for(int x = 500001; x <= 1000000; x++)
	{
		m.lock();
		sum += x;
		m.unlock();
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
