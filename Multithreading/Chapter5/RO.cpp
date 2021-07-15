#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> value;

void read()
{
	cout << value.load(memory_order_relaxed) << endl;
}

void write(int v)
{
	value.store(v, memory_order_relaxed);
}

int main()
{
	value = 0;

	thread t1(write, 1);
	thread t2(read);
	thread t3(write, 2);
	thread t4(read);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}
