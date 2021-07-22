#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

int main()
{
	atomic<int> x(0);

	x.store(1, memory_order_relaxed);
	cout << x.load(memory_order_relaxed) << endl;

	x.store(1, memory_order_relaxed);
	x.store(1, memory_order_relaxed);
	
	return 0;
}
