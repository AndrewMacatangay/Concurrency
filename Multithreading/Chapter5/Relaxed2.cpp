#include <iostream>
#include <atomic>
#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
using namespace std;

atomic<int> value(0);
mutex m;

void read(string name)
{
	int wait = rand() % 10;
	this_thread::sleep_for(chrono::milliseconds(rand() % 10));

	for(int v = value.load(memory_order_relaxed); v != 9; v = value.load(memory_order_relaxed))
	{
		unique_lock<mutex> l(m);
		cout << name << ": " << v << endl;
		l.unlock();
		this_thread::sleep_for(chrono::milliseconds(wait));
		wait = rand() % 10;
	}
}

void write(int v)
{
	value.store(v, memory_order_relaxed);
}

int main()
{
	srand(time(NULL));

	vector<thread> threads;
	thread t1(read, "Gonzalo");
	thread t2(read, "Joe");
	thread t3(read, "Mark");
	thread t4(read, "Sean");

	for (int x = 0; x < 10; x++)
	{
		threads.push_back(thread(write, x));
		this_thread::sleep_for(5ms);
	}

	for(thread& x : threads)
		x.join();

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}
