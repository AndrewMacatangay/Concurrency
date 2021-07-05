#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> monitor;
bool available;

void makeBurger()
{
	available = true;
	cout << "Burger made!" << endl;
	monitor.store(available, memory_order_release);
}

void sellBurger()
{
	while(!monitor.load(memory_order_acquire));

	available = false;
	cout << "Burger sold!" << endl;
	monitor.store(available, memory_order_release);
}

int main()
{
	monitor = false;
	available = false;

	thread t1(makeBurger);
	thread t2(sellBurger);

	t1.join();
	t2.join();

	cout << (monitor.is_lock_free() ? "Lock-free!" : "Not lock-free...") << endl;
	
	return 0;
}
