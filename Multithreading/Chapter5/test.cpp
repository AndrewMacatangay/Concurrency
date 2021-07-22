#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<int> number(0);
atomic<bool> go(0);

void write()
{
	for (int x = 1; x <= 1000000; x++)
		number.store(x, memory_order_relaxed);
}

void read()
{
	while(!go);
	cout << number.load(memory_order_relaxed) << endl;
}

int main()
{
	thread w(write);
	thread r1(read);
	thread r2(read);
	thread r3(read);
	thread r4(read);
	thread r5(read);
	
	go = 1;

	w.join();
	r1.join();
	r2.join();
	r3.join();
	r4.join();
	r5.join();
	return 0;
}
