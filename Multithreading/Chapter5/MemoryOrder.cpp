#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> x, y;
int r1, r2;

void func1()
{
	r1 = y.load(memory_order_relaxed);
	x.store(r1, memory_order_relaxed);
}

void func2()
{
	r2 = x.load(memory_order_relaxed);
	y.store(42, memory_order_relaxed);
}

int main()
{
	x = y = 0;

	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();

	cout << "R1: " << r1 << " R2: " << r2 << " x: " << x << " y: " << y << endl;

	return 0;
}
