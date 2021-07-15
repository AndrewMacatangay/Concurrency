/*
Memory_order_relaxed is the memory ordering with the least overhead, but difficult to use. Atomics
with this memory ordering do not have a "synchronizes-with" relationship and will store and load
the atomic's current value without waiting for an update to the atomic. This is usually the case
when you just need to increment a value and view the current counter. The program below will run
a writer thread that will increment the atomic while the reader threads read the current value of
the atomic at that moment.

Sidenote: When there is a continuous writer thread, the readers may acquire any value (depending
on when the reader thread starts reading), but every subsequent read will either be the same
value or a more recently updated value (no overwritten values unless duplicates are allowed).
*/

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
	//Initialize a wait for the thread to read the next atomic value
	int wait = rand() % 10;

	//Sleep at the beginning. Some threads may start at v = 0, some may not
	this_thread::sleep_for(chrono::milliseconds(rand() % 10));

	//Get the value from the atomic, sleep, and get the next value until the limit is reached
	for(int v = value.load(memory_order_relaxed); v != 9; v = value.load(memory_order_relaxed))
	{
		//Lock used here to avoid race condition on std::cout
		unique_lock<mutex> l(m);
		cout << name << ": " << v << endl;
		l.unlock();

		//Sleep and get next wait value
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
	//Create random seed
	srand(time(NULL));

	//The vector serves as a writer to the atomic
	vector<thread> threads;

	//Each read thread will read the atomic
	thread t1(read, "Gonzalo");
	thread t2(read, "Joe");
	thread t3(read, "Mark");
	thread t4(read, "Sean");

	//Continuously write to the atomic
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
