#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> a;

void func(int x)
{
	bool expected = 0;
	while(!a.compare_exchange_strong(expected, true, memory_order_acquire))
		expected = 0;

	cout << "Thread " << x << " processed!" << endl;
	a.store(0, memory_order_release);
}

int main()
{
	vector<thread> threads;
	a = false;

	for (int x = 0; x < 10; x++)
		threads.push_back(thread(func, x));

	for (thread& t : threads)
		t.join();

	cout << (a.is_lock_free() ? "Lock-Free!" : "Not Lock-Free...") << endl;

	return 0;
}
